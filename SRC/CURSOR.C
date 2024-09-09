/* ********************************************************************** */
/*                                                                        */
/*   CURSOR.C                                                             */
/*                                                                        */
/* ********************************************************************** */

/* -- HEADER FILES ------------------------------------------------------ */

#include "cursor.h"

/* -- PUBLIC DATA DEFINITIONS ------------------------------------------- */

unsigned char CursorStyle = 0;
unsigned char CursorState = 0;

/* -- PRIVATE DATA DEFINITIONS ------------------------------------------ */

static unsigned char Cursor[15][8] = {
	0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x0f, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x0f, 0x0f, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff,
	0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0xff, 0xff, 0xff,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0xff, 0xff,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0xff,
	0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x00,
	0x0f, 0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00,
	0x0f, 0x0f, 0x00, 0x0f, 0x0f, 0x00, 0xff, 0xff,
	0x0f, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0xff, 0xff,
	0x00, 0xff, 0xff, 0x00, 0x0f, 0x0f, 0x00, 0xff,
	0xff, 0xff, 0xff, 0x00, 0x0f, 0x0f, 0x00, 0xff,
	0xff, 0xff, 0xff, 0xff, 0x00, 0x0f, 0x0f, 0x00,
	0xff, 0xff, 0xff, 0xff, 0x00, 0x0f, 0x0f, 0x00,
	0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00
};

static unsigned char CursorBackground[15][8];

static unsigned short CursorPositionX;
static unsigned short CursorPositionY;
static unsigned int CursorVisibleColumns = 0;
static unsigned int CursorVisibleRows = 0;

/* -- PRIVATE FUNCTION DECLARATIONS ------------------------------------- */

static void DrawCursor(unsigned short x, unsigned short y,
	unsigned int cols, unsigned int rows);
static void SaveCursorBackground(unsigned short x, unsigned short y,
	unsigned int cols, unsigned int rows);
static void RestoreCursorBackground(short x, short y,
	unsigned int cols, unsigned int rows);

/* -- CODE -------------------------------------------------------------- */

/* ====================================================================== */
/*                                                                        */
/*   SetCursorState                                                       */
/*                                                                        */
/* ====================================================================== */

void SetCursorState(unsigned char state, short x, short y)
{
	if ((CursorVisibleRows > 0) && (CursorVisibleRows > 0)) {
		RestoreCursorBackground(CursorPositionX, CursorPositionY,
			CursorVisibleColumns, CursorVisibleRows);
	}

	if (CursorStyle != 0) {
		SetModeXor();
		SetColor(8);

		if (CursorState != 0) {
			if ((state == 0) || (x != CursorPositionX)) {
				DrawLineFunction(CursorPositionX,
					WindowStartY,
					CursorPositionX,
					WindowEndY);
			}

			if ((state == 0) || (y != CursorPositionY)) {
				DrawLineFunction(WindowStartX,
					CursorPositionY,
					WindowEndX,
					CursorPositionY);
			}
		}

		if (state != 0) {
			if (x != CursorPositionX) {
				DrawLineFunction(x, WindowStartY,
					x, WindowEndY);
			}

			if (y != CursorPositionY) {
				DrawLineFunction(WindowStartX, y,
					WindowEndX, y);
			}
		}

		SetModeNormal();
	}

	if (state != 0) {
		CursorPositionX = x;
		CursorPositionY = y;

		if ((x < WindowStartX) || (y < WindowStartY)) {
			CursorVisibleColumns = 0;
			CursorVisibleRows = 0;
			return;
		}

		Translate(&x, &y);

		if ((x >= ResolutionX) || (y >= ResolutionY)) {
			CursorVisibleColumns = 0;
			CursorVisibleRows = 0;
			return;
		}

		if (x + 7 >= ResolutionX) {
			CursorVisibleColumns = (ResolutionX - 1) - x;
		} else {
			CursorVisibleColumns = 8;
		}

		if (y + 14 >= ResolutionY) {
			CursorVisibleRows = (ResolutionY - 1) - y;
		} else {
			CursorVisibleRows = 15;
		}

		SaveCursorBackground(x, y,
			CursorVisibleColumns, CursorVisibleRows);
		DrawCursor(x, y,
			CursorVisibleColumns, CursorVisibleRows);
	} else {
		CursorPositionX = -1;
		CursorPositionY = -1;
		CursorVisibleColumns = 0;
		CursorVisibleRows = 0;
	}

	CursorState = state;
}

/* ====================================================================== */
/*                                                                        */
/*   SetCursorStyle                                                       */
/*                                                                        */
/* ====================================================================== */

unsigned char SetCursorStyle(unsigned char newStyle)
{
	unsigned char oldStyle;

	SetCursorState(0, 0, 0);
	oldStyle = CursorStyle;
	CursorStyle = newStyle;
	return oldStyle;
}

/* ====================================================================== */
/*                                                                        */
/*   DrawCursor                                                           */
/*                                                                        */
/* ====================================================================== */

void DrawCursor(unsigned short x, unsigned short y,
	unsigned int cols, unsigned int rows)
{
	unsigned int i;
	unsigned int j;
	unsigned short vmNextLineOffset;
	unsigned short fbNextLineOffset;
	unsigned char far *vm;
	unsigned char far *fb;
	unsigned char *cursorData;
	unsigned char pixel;

	vmNextLineOffset = BytesPerLine - cols;
	fbNextLineOffset = ResolutionX  - cols;
	vm = AccessVideoMemory(WRITE, x, y);
	fb = AccessFrameBuffer(x, y);

	for (j = 0; j < rows; j++) {
		cursorData = Cursor[j];
		for (i = 0; i < cols; i++) {
			pixel = *cursorData++;
			if (pixel != 0xff) {
				*vm = pixel;
				*fb = pixel;
			}

			vm++;
			if (FP_OFF(vm) == 0) {
				NextVideoMemoryBlock(WRITE);
			}

			fb++;
			if (FP_OFF(fb) == 0) {
				NextFrameBufferBlock();
			}
		}

		if (j != (rows - 1)) {
			vm += vmNextLineOffset;
			if (FP_OFF(vm) < vmNextLineOffset) {
				NextVideoMemoryBlock(WRITE);
			}

			fb += fbNextLineOffset;
			if (FP_OFF(fb) < fbNextLineOffset) {
				NextFrameBufferBlock();
			}
		}
	}
}

/* ====================================================================== */
/*                                                                        */
/*   SaveCursorBackground                                                 */
/*                                                                        */
/* ====================================================================== */

void SaveCursorBackground(unsigned short x, unsigned short y,
	unsigned int cols, unsigned int rows)
{
	unsigned int i;
	unsigned int j;
	unsigned short fbNextLineOffset;
	unsigned char far *fb;
	unsigned char *cursorBackground;

	fbNextLineOffset = ResolutionX - cols;
	fb = AccessFrameBuffer(x, y);
	cursorBackground = CursorBackground;

	for (j = 0; j < rows; j++) {
		for (i = 0; i < cols; i++) {
			*cursorBackground++ = *fb;

			fb++;
			if (FP_OFF(fb) == 0) {
				NextFrameBufferBlock();
			}
		}

		if (j != (rows - 1)) {
			fb += fbNextLineOffset;
			if (FP_OFF(fb) < fbNextLineOffset) {
				NextFrameBufferBlock();
			}
		}
	}
}

/* ====================================================================== */
/*                                                                        */
/*   RestoreCursorBackground                                              */
/*                                                                        */
/* ====================================================================== */

void RestoreCursorBackground(short x, short y, unsigned int cols,
	unsigned int rows)
{
	unsigned int i;
	unsigned int j;
	unsigned short vmNextLineOffset;
	unsigned short fbNextLineOffset;
	unsigned char far *vm;
	unsigned char far *fb;
	unsigned char *cursorBackground;
	unsigned char pixel;

	Translate(&x, &y);

	vmNextLineOffset = BytesPerLine - cols;
	fbNextLineOffset = ResolutionX  - cols;
	vm = AccessVideoMemory(WRITE, x, y);
	fb = AccessFrameBuffer(x, y);
	cursorBackground = CursorBackground;

	for (j = 0; j < rows; j++) {
		for (i = 0; i < cols; i++) {
			pixel = *cursorBackground++;
			*vm = pixel;
			*fb = pixel;

			vm++;
			if (FP_OFF(vm) == 0) {
				NextVideoMemoryBlock(WRITE);
			}

			fb++;
			if (FP_OFF(fb) == 0) {
				NextFrameBufferBlock();
			}
		}

		if (j != (rows - 1)) {
			vm += vmNextLineOffset;
			if (FP_OFF(vm) < vmNextLineOffset) {
				NextVideoMemoryBlock(WRITE);
			}

			fb += fbNextLineOffset;
			if (FP_OFF(fb) < fbNextLineOffset) {
				NextFrameBufferBlock();
			}
		}
	}
}

/* -- END --------------------------------------------------------------- */
