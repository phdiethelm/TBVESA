/* ********************************************************************** */
/*                                                                        */
/*   VIDEO.C                                                              */
/*                                                                        */
/* ********************************************************************** */

/* -- HEADER FILES ------------------------------------------------------ */

#include "video.h"

/* -- PUBLIC DATA DEFINITIONS ------------------------------------------- */

bool DualWindowSystem;
unsigned short BytesPerLine;

/* -- PRIVATE DATA DEFINITIONS ------------------------------------------ */

static bool VideoModeSwitched = 0;
static unsigned short SavedVideoMode;
static unsigned int BankShift;
static unsigned char ReadWindow;
static unsigned char WriteWindow;
static unsigned short ReadWindowSegment;
static unsigned short WriteWindowSegment;
static void (far *WindowFunction)(void);
static struct VBEInfo VBEInfo;
static struct VBEModeInfo VBEModeInfo;
static unsigned short CurrentVMReadBlock;
static unsigned short CurrentVMWriteBlock;
static unsigned int CurrentFBBlock;

/* -- PRIVATE FUNCTION PROTOTYPES --------------------------------------- */

static void SetReadWindow(unsigned short block);
static void SetWriteWindow(unsigned short block);
static void AccessFrameBufferBlock(unsigned int block);
unsigned short VBEGetInfo(struct VBEInfo far *const vbeInfo);
unsigned short VBEGetModeInfo(unsigned short mode,
	struct VBEModeInfo far *const vbeModeInfo);
unsigned short VBESetMode(unsigned short mode);
unsigned short VBEGetMode(void);
void VBESetWindow(unsigned char window, unsigned short bank);
void VBESetWindowDirect(unsigned char window, unsigned short bank);
unsigned short VBEGetWindow(unsigned char window);

/* -- CODE -------------------------------------------------------------- */

/* ====================================================================== */
/*                                                                        */
/*   InitVideo                                                            */
/*                                                                        */
/* ====================================================================== */

bool InitVideo(unsigned short resolutionX, unsigned short resolutionY)
{
	const unsigned short far *p;
	unsigned short mode;

	if (VideoModeSwitched == true) {
		return true;
	}

	if (VBEGetInfo(&VBEInfo) != 0x004f) {
		PrintString("No VBE detected!\r\n");
		return false;
	}

	if ((VBEInfo.Signature[0] != 'V')
	||  (VBEInfo.Signature[1] != 'E')
	||  (VBEInfo.Signature[2] != 'S')
	||  (VBEInfo.Signature[3] != 'A')
	|| ((VBEInfo.MajorVersion == 1)
		&& (VBEInfo.MinorVersion < 2))
	||  (VBEInfo.MajorVersion < 1)) {
		PrintString("VBE 1.2 or higher required!\r\n");
		return false;
	}

	mode = 0;
	for (p = VBEInfo.Modes; *p != -1; p++) {
		if ((*p & 0x0100) == 0) {
			continue;
		}

		if (VBEGetModeInfo(*p, &VBEModeInfo) != 0x004f) {
			continue;
		}

		if (((VBEModeInfo.ModeAttributes & 0x1b) == 0x1b)
		&&  ((VBEModeInfo.WindowAAttributes & 0x01)
			|| (VBEModeInfo.WindowBAttributes & 0x01))
		&&   (VBEModeInfo.ResolutionX == resolutionX)
		&&   (VBEModeInfo.ResolutionY == resolutionY)
		&&   (VBEModeInfo.BitsPerPixel == 8)
		&&   (VBEModeInfo.NumberOfPlanes == 1)
		&&   (VBEModeInfo.MemoryModel == 4)) {
			mode = *p;
			break;
		}
	}

	if (mode == 0) {
		PrintString("Required video mode not available!\r\n");
		return false;
	}

	SavedVideoMode = VBEGetMode();
	if (SavedVideoMode == -1) {
		PrintString("Failed to save current video mode!\r\n");
		return false;
	}

	if (VBESetMode(mode & 0x7fff) != 0x004f) {
		PrintString("Failed to set required video mode!\r\n");
		return false;
	}

	BankShift = 0;
	while ((64 >> BankShift) != VBEModeInfo.WindowGranularity) {
		BankShift++;
	}

	if ((VBEModeInfo.WindowAAttributes & 0x03) == 0x03) {
		ReadWindow = 0;
		ReadWindowSegment = VBEModeInfo.WindowASegment;
	} else {
		ReadWindow = 1;
		ReadWindowSegment = VBEModeInfo.WindowBSegment;
	}

	if (((VBEModeInfo.WindowAAttributes & 0x05) == 0x05)
	&&  ((VBEModeInfo.WindowBAttributes & 0x05) == 0x05)) {
		if (ReadWindow == 0) {
			WriteWindow = 1;
			WriteWindowSegment = VBEModeInfo.WindowBSegment;
		} else {
			WriteWindow = 0;
			WriteWindowSegment = VBEModeInfo.WindowASegment;
		}
	} else {
		if ((VBEModeInfo.WindowAAttributes & 0x05) == 0x05) {
			WriteWindow = 0;
			WriteWindowSegment = VBEModeInfo.WindowASegment;
		} else {
			WriteWindow = 1;
			WriteWindowSegment = VBEModeInfo.WindowBSegment;
		}
	}

	if (ReadWindow == WriteWindow) {
		DualWindowSystem = false;
	} else {
		DualWindowSystem = true;
	}

	CurrentVMReadBlock = -1;
	CurrentVMWriteBlock = -1;
	CurrentFBBlock = -1;
	BytesPerLine = VBEModeInfo.BytesPerLine;
	WindowFunction = VBEModeInfo.WindowFunction;
	VideoModeSwitched = true;
	return true;
}

/* ====================================================================== */
/*                                                                        */
/*   ShutdownVideo                                                        */
/*                                                                        */
/* ====================================================================== */

void ShutdownVideo()
{
	if (VideoModeSwitched == false) {
		return;
	}

	VBESetMode(SavedVideoMode);
	VideoModeSwitched = false;
}

/* ====================================================================== */
/*                                                                        */
/*   Translate                                                            */
/*                                                                        */
/* ====================================================================== */

void Translate(short far *const x, short far *const y)
{
	*x -= WindowStartX;
	*y -= WindowStartY;

	if (Scale > 1) {
		*x = ScaleLookupTable[*x];
		*y = ScaleLookupTable[*y];
	}

	*x += WindowOriginX;
	*y += WindowOriginY;
}

/* ====================================================================== */
/*                                                                        */
/*   PutPixel                                                             */
/*                                                                        */
/* ====================================================================== */

void PutPixel(unsigned short x, unsigned short y)
{
	unsigned char far *vm;
	unsigned char far *fb;
	unsigned char pixel;

	vm = AccessVideoMemory(WRITE, x, y);
	fb = AccessFrameBuffer(x, y);

	pixel = *fb;
	if (Mode == OR) {
		pixel |= MapMask;
	} else {
		pixel ^= MapMask;
	}

	*vm = pixel;
	*fb = pixel;
}

/* ====================================================================== */
/*                                                                        */
/*   AccessVideoMemory                                                    */
/*                                                                        */
/* ====================================================================== */

unsigned char far *AccessVideoMemory(int method,
	unsigned short x, unsigned short y)
{
	unsigned long address;
	unsigned int block;
	unsigned short offset;
	unsigned char far *vm;

	address = ((long)y * BytesPerLine) + x;
	block = (unsigned short)(address >> 16);
	offset = (unsigned short)(address & 0xffff);

	if (method == READ) {
		vm = MK_FP(ReadWindowSegment, offset);
		SetReadWindow(block);
	} else {
		vm = MK_FP(WriteWindowSegment, offset);
		SetWriteWindow(block);
	}

	return vm;
}

/* ====================================================================== */
/*                                                                        */
/*   AccessFrameBuffer                                                    */
/*                                                                        */
/* ====================================================================== */

unsigned char far *AccessFrameBuffer(unsigned short x, unsigned short y)
{
	unsigned long address;
	unsigned int block;
	unsigned short offset;
	unsigned char far *fb;

	address = ((long)y * ResolutionX) + x;
	block = (unsigned short)(address >> 16);
	offset = (unsigned short)(address & 0xffff);

	fb = MK_FP(FP_SEG(TransferBuffer), offset);
	AccessFrameBufferBlock(block);
	return fb;
}

/* ====================================================================== */
/*                                                                        */
/*   NextVideoMemoryBlock                                                 */
/*                                                                        */
/* ====================================================================== */

void NextVideoMemoryBlock(int method)
{
	if (method == READ) {
		SetReadWindow(CurrentVMReadBlock + 1);
	} else {
		SetWriteWindow(CurrentVMWriteBlock + 1);
	}
}

/* ====================================================================== */
/*                                                                        */
/*   NextFrameBufferBlock                                                 */
/*                                                                        */
/* ====================================================================== */

void NextFrameBufferBlock()
{
	AccessFrameBufferBlock(CurrentFBBlock + 1);
}

/* ====================================================================== */
/*                                                                        */
/*   SetReadWindow                                                        */
/*                                                                        */
/* ====================================================================== */

void SetReadWindow(unsigned short block)
{
	unsigned short bank;

	if (CurrentVMReadBlock == block) {
		return;
	}

	bank = block << BankShift;

	if (WindowFunction != 0) {
		VBESetWindowDirect(ReadWindow, bank);
	} else {
		VBESetWindow(ReadWindow, bank);
	}

	CurrentVMReadBlock = block;
	if (DualWindowSystem == false) {
		CurrentVMWriteBlock = block;
	}
}

/* ====================================================================== */
/*                                                                        */
/*   SetWriteWindow                                                       */
/*                                                                        */
/* ====================================================================== */

void SetWriteWindow(unsigned short block)
{
	unsigned short bank;

	if (CurrentVMWriteBlock == block) {
		return;
	}

	bank = block << BankShift;

	if (WindowFunction != 0) {
		VBESetWindowDirect(WriteWindow, bank);
	} else {
		VBESetWindow(WriteWindow, bank);
	}

	CurrentVMWriteBlock = block;
	if (DualWindowSystem == false) {
		CurrentVMReadBlock = block;
	}
}

/* ====================================================================== */
/*                                                                        */
/*   AccessFrameBufferBlock                                               */
/*                                                                        */
/* ====================================================================== */

void AccessFrameBufferBlock(unsigned int block)
{
	if (CurrentFBBlock == block) {
		return;
	}

	if (CurrentFBBlock != -1) {
		MoveExtendedMemory(0, (unsigned long)TransferBuffer,
			FrameBufferHandle, ((unsigned long)CurrentFBBlock << 16));
	}

	MoveExtendedMemory(FrameBufferHandle, ((unsigned long)block << 16), 0,
		(unsigned long)TransferBuffer);
	CurrentFBBlock = block;
}

/* ====================================================================== */
/*                                                                        */
/*   VBEGetInfo                                                           */
/*                                                                        */
/* ====================================================================== */

#pragma aux VBEGetInfo =		\
	"mov	ax, 4f00h"		\
	"int	10h"			\
	parm [es di]			\
	value [ax]			\
	modify exact [ax];

/* ====================================================================== */
/*                                                                        */
/*   VBEGetModeInfo                                                       */
/*                                                                        */
/* ====================================================================== */

#pragma aux VBEGetModeInfo =		\
	"mov	ax, 4f01h"		\
	"int	10h"			\
	parm [cx] [es di]		\
	value [ax]			\
	modify exact [ax];

/* ====================================================================== */
/*                                                                        */
/*   VBESetMode                                                           */
/*                                                                        */
/* ====================================================================== */

#pragma aux VBESetMode =		\
	"mov	ax, 4f02h"		\
	"int	10h"			\
	parm [bx] nomemory		\
	value [ax]			\
	modify exact [ax] nomemory;

/* ====================================================================== */
/*                                                                        */
/*   VBEGetMode                                                           */
/*                                                                        */
/* ====================================================================== */

#pragma aux VBEGetMode =		\
	"mov	ax, 4f03h"		\
	"int	10h"	 		\
	"cmp	ax, 004fh"		\
	"je	done"			\
	"mov	bx, -1"   		\
	"done:"				\
	parm [] nomemory 		\
	value [bx]			\
	modify exact [ax bx] nomemory;

/* ====================================================================== */
/*                                                                        */
/*   VBESetWindow                                                         */
/*                                                                        */
/* ====================================================================== */

#pragma aux VBESetWindow = 		\
	"mov	ax, 4f05h"		\
	"mov	bh, 00h"		\
	"int	10h"			\
	parm [bl] [dx] nomemory		\
	modify exact [ax bh dx] nomemory;

/* ====================================================================== */
/*                                                                        */
/*   VBESetWindowDirect                                                   */
/*                                                                        */
/* ====================================================================== */

#pragma aux VBESetWindowDirect =	\
	"mov	bh, 00h"		\
	"call	dword ptr [WindowFunction]"	\
	parm [bl] [dx] nomemory		\
	modify exact [ax bh dx] nomemory;

/* ====================================================================== */
/*                                                                        */
/*   VBEGetWindow                                                         */
/*                                                                        */
/* ====================================================================== */

#pragma aux VBEGetWindow = 		\
	"mov	ax, 4f05h"		\
	"mov	bh, 01h"		\
	"int	10h"			\
	parm [bl] nomemory		\
	value [dx]			\
	modify exact [ax bh dx] nomemory;

/* -- END --------------------------------------------------------------- */
