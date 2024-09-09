/* ********************************************************************** */
/*                                                                        */
/*   DRIVER.C                                                             */
/*                                                                        */
/* ********************************************************************** */

/* -- HEADER FILES ------------------------------------------------------ */

#include "driver.h"

/* -- PUBLIC DATA DEFINITIONS ------------------------------------------- */

unsigned char far *BitmapBuffer = NULL;
unsigned short far *ScaleLookupTable = NULL;

/* -- PRIVATE DATA DEFINITIONS ------------------------------------------ */

static const void *DriverInterface[52] = {
	InitDriver,
	ShutdownDriver,
	AllocBuffers,
	FreeBuffers,
	GetBitmapBufferAddress,
	PrintDriverInfo,
	GetScreenResolution,
	GetColor,
	SetColor,
	GetScale,
	SetScale,
	SetWindow,
	SetWindowOrigin,
	SetCursorState,
	SetCursorStyle,
	SaveWindowState,
	RestoreWindowState,
	DrawArcAbsolute,
	DrawWideArcAbsolute32,
	DrawWideLineAbsolute,
	DrawWideLineRelative,
	DrawCircleAbsolute,
	DrawCircleRelative,
	DrawDashedLineAbsolute,
	DrawDashedLineRelative,
	DrawGridDots,
	DrawGridDotsRelative,
	SetWindow32,
	DrawSolidLineAbsolute,
	DrawSolidLineRelative,
	MoveAbsolute,
	MoveRelative,
	DrawBitmapRelative,
	DrawFixedStringAbsolute,
	DrawFixedStringRelative,
	DrawVariableStringAbsolute,
	DrawVariableStringRelative,
	DrawFixedTextAbsolute,
	DrawFixedTextRelative,
	DrawVariableTextAbsolute,
	DrawVariableTextRelative,
	DrawWideLineAbsolute32,
	DrawCircleAbsolute32,
	ClearRectangle,
	FillRectangle,
	HighlightRectangle,
	SetModeHighlight,
	SetModeNormal,
	SetModeXor,
	SaveRectangle,
	RestoreRectangle,
	CopyRectangle
};

static char CopyrightString[] =
	"0xBADC0DED OrCAD Release IV/386+ Modified VESA VBE 1.2+ Driver\r\n";

/* -- PRIVATE FUNCTION PROTOTYPES --------------------------------------- */

static unsigned short GetDriverLoadSize(void);
void ResizeDriverMemoryBlock(unsigned short size);
void DOSPrintChar(const char ch);

/* -- CODE -------------------------------------------------------------- */

/* ====================================================================== */
/*                                                                        */
/*   DriverMain                                                           */
/*                                                                        */
/* ====================================================================== */

unsigned short DriverMain(struct DriverInfo far *const driverInfo)
{
	unsigned short size = GetDriverLoadSize();

	if (driverInfo->Magic != 0xa55a) {
		PrintString(CopyrightString);
		/* think about this - hardly makes any sense */
		ResizeDriverMemoryBlock(size);
	}

	driverInfo->Version = 13;
	driverInfo->Interface = DriverInterface;
  
  PrintString(CopyrightString);

	if ((driverInfo->Magic == 0xa55a) || (driverInfo->Magic == 0xa55b)) {
		driverInfo->Attributes = 0x10;
	}

	return size;
}

/* ====================================================================== */
/*                                                                        */
/*   InitDriver                                                           */
/*                                                                        */
/* ====================================================================== */

void InitDriver(unsigned short far *const bitmapBufferSegment,
	unsigned short maxScale, unsigned char far *const dosErrorCode)
{
	/* put this inside video.c */
	if (InitMemory() == false) {
		*dosErrorCode = -1;
		return;
	}

	if (InitVideo(ResolutionX, ResolutionY) == false) {
		*dosErrorCode = -1;
		return;
	}

	AllocBuffers(bitmapBufferSegment, maxScale, dosErrorCode);

	Color = 15;
	Scale = 1;
	TopOfWindowStateStack = 0;
	SavedRectangle = false;
	CharSetSpecial = false;
	CharSetIEEE = false;
	LineDrawingMode = SOLID;
	SetCursorStyle(0);
	SetModeNormal();
}

/* ====================================================================== */
/*                                                                        */
/*   ShutdownDriver                                                       */
/*                                                                        */
/* ====================================================================== */

void ShutdownDriver()
{
	FreeBuffers();
	ShutdownVideo();
	ShutdownMemory();
}

/* ====================================================================== */
/*                                                                        */
/*   AllocBuffers                                                         */
/*                                                                        */
/* ====================================================================== */

void AllocBuffers(unsigned short far *const bitmapBufferSegment,
	const unsigned short maxScale, unsigned char far *const dosErrorCode)
{
	if (BitmapBuffer == NULL) {
		*dosErrorCode = AllocMemory((16384 / 16), &BitmapBuffer);
		if (*dosErrorCode != 0) {
			FreeBuffers();
			return;
		}
	}

	if ((ScaleLookupTable == NULL) && (maxScale > 1)) {
		*dosErrorCode = AllocMemory((65536 / 16), &ScaleLookupTable);
		if (*dosErrorCode != 0) {
			FreeBuffers();
			return;
		}
	}

	*dosErrorCode = 0;
	*bitmapBufferSegment = FP_SEG(BitmapBuffer);
}

/* ====================================================================== */
/*                                                                        */
/*   FreeBuffers                                                          */
/*                                                                        */
/* ====================================================================== */

void FreeBuffers()
{
	if (BitmapBuffer != NULL) {
		FreeMemory(BitmapBuffer);
		BitmapBuffer = NULL;
	}

	if (ScaleLookupTable != NULL) {
		FreeMemory(ScaleLookupTable);
		ScaleLookupTable = NULL;
	}
}

/* ====================================================================== */
/*                                                                        */
/*   GetDriverLoadSize                                                    */
/*                                                                        */
/* ====================================================================== */

unsigned short GetDriverLoadSize()
{
	return (((unsigned short)&end + 15) / 16);
}

/* ====================================================================== */
/*                                                                        */
/*   PrintString                                                          */
/*                                                                        */
/* ====================================================================== */

void PrintString(const char *s)
{
	for (; *s; s++) {
		DOSPrintChar(*s);
	}
}

/* ====================================================================== */
/*                                                                        */
/*   ResizeDriverMemoryBlock                                              */
/*                                                                        */
/* ====================================================================== */

#pragma aux ResizeDriverMemoryBlock =	\
	"mov	ah, 4ah"		\
	"push	cs"			\
	"pop	es"			\
	"int	21h"			\
	parm [bx] nomemory		\
	modify exact [ax bx es] nomemory;

/* ====================================================================== */
/*                                                                        */
/*   DOSPrintChar                                                         */
/*                                                                        */
/* ====================================================================== */

#pragma aux DOSPrintChar =		\
	"mov	ah, 02h"		\
	"int	21h"			\
	parm [dl] nomemory		\
	modify exact [ax] nomemory;

/* -- END --------------------------------------------------------------- */