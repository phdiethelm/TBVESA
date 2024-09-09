/* ********************************************************************** */
/*                                                                        */
/*   RECTNGLE.C                                                           */
/*                                                                        */
/* ********************************************************************** */

/* -- HEADER FILES ------------------------------------------------------ */

#include "rectngle.h"

/* -- PUBLIC DATA DEFINITIONS ------------------------------------------- */

bool SavedRectangle;

/* -- PRIVATE DATA DEFINITIONS ------------------------------------------ */

static short RectangleStartX;
static short RectangleStartY;
static short RectangleEndX;
static short RectangleEndY;

/* -- PRIVATE FUNCTION DECLARATIONS ------------------------------------- */

static int ClipRectangle(short far *xa, short far *ya,
	short far *xb, short far *yb);

/* -- CODE -------------------------------------------------------------- */

/* ====================================================================== */
/*                                                                        */
/*   ClearRectangle                                                       */
/*                                                                        */
/* ====================================================================== */

void ClearRectangle(short xa, short ya, short xb, short yb)
{
	short x;
	short y;
	unsigned short vmNextLineOffset;
	unsigned short fbNextLineOffset;
	unsigned char far *vm;
	unsigned char far *fb;

	if (ClipRectangle(&xa, &ya, &xb, &yb) == false) {
		return;
	}

	vmNextLineOffset = BytesPerLine - ((xb - xa) + 1);
	fbNextLineOffset = ResolutionX  - ((xb - xa) + 1);
	vm = AccessVideoMemory(WRITE, xa, ya);
	fb = AccessFrameBuffer(xa, ya);

	for (y = ya; y <= yb; y++) {
		for (x = xa; x <= xb; x++) {
			*vm = 0;
			*fb = 0;

			vm++;
			if (FP_OFF(vm) == 0) {
				NextVideoMemoryBlock(WRITE);
			}

			fb++;
			if (FP_OFF(fb) == 0) {
				NextFrameBufferBlock();
			}
		}

		if (y != yb) {
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
/*   FillRectangle                                                        */
/*                                                                        */
/* ====================================================================== */

void FillRectangle(short xa, short ya, short xb, short yb)
{
	short x;
	short y;
	unsigned short vmNextLineOffset;
	unsigned short fbNextLineOffset;
	unsigned char far *vm;
	unsigned char far *fb;
	unsigned char pixel;

	if (ClipRectangle(&xa, &ya, &xb, &yb) == false) {
		return;
	}

	vmNextLineOffset = BytesPerLine - ((xb - xa) + 1);
	fbNextLineOffset = ResolutionX  - ((xb - xa) + 1);
	vm = AccessVideoMemory(WRITE, xa, ya);
	fb = AccessFrameBuffer(xa, ya);

	for (y = ya; y <= yb; y++) {
		for (x = xa; x <= xb; x++) {
			pixel = *fb;
			if (Mode == OR) {
				pixel |= MapMask;
			} else {
				pixel ^= MapMask;
			}

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

		if (y != yb) {
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
/*   HighlightRectangle                                                   */
/*                                                                        */
/* ====================================================================== */

void HighlightRectangle(short xa, short ya, short xb, short yb)
{
	short x;
	short y;
	unsigned short vmNextLineOffset;
	unsigned short fbNextLineOffset;
	unsigned char far *vm;
	unsigned char far *fb;
	unsigned char pixel;

	if (ClipRectangle(&xa, &ya, &xb, &yb) == false) {
		return;
	}

	vmNextLineOffset = BytesPerLine - ((xb - xa) + 1);
	fbNextLineOffset = ResolutionX  - ((xb - xa) + 1);
	vm = AccessVideoMemory(WRITE, xa, ya);
	fb = AccessFrameBuffer(xa, ya);

	for (y = ya; y <= yb; y++) {
		for (x = xa; x <= xb; x++) {
			pixel = *fb ^ 0x0f;
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

		if (y != yb) {
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
/*   SaveRectangle                                                        */
/*                                                                        */
/* ====================================================================== */

void SaveRectangle(short xa, short ya, short xb, short yb)
{
	short x;
	short y;
	unsigned short vmNextLineOffset;
	unsigned short fbNextLineOffset;
	unsigned char far *vm;
	unsigned char far *fb;

	vmNextLineOffset = BytesPerLine - ((xb - xa) + 1);
	fbNextLineOffset = ResolutionX  - ((xb - xa) + 1);
	vm = AccessVideoMemory(READ, xa, ya);
	fb = AccessFrameBuffer(xa, (ya + ResolutionY));

	for (y = ya; y <= yb; y++) {
		for (x = xa; x <= xb; x++) {
			*fb = *vm;

			vm++;
			if (FP_OFF(vm) == 0) {
				NextVideoMemoryBlock(READ);
			}

			fb++;
			if (FP_OFF(fb) == 0) {
				NextFrameBufferBlock();
			}
		}

		if (y != yb) {
			vm += vmNextLineOffset;
			if (FP_OFF(vm) < vmNextLineOffset) {
				NextVideoMemoryBlock(READ);
			}

			fb += fbNextLineOffset;
			if (FP_OFF(fb) < fbNextLineOffset) {
				NextFrameBufferBlock();
			}
		}
	}

	RectangleStartX = xa;
	RectangleStartY = ya;
	RectangleEndX = xb;
	RectangleEndY = yb;
	SavedRectangle = true;
}

/* ====================================================================== */
/*                                                                        */
/*   RestoreRectangle                                                     */
/*                                                                        */
/* ====================================================================== */

void RestoreRectangle()
{
	short xa;
	short ya;
	short xb;
	short yb;
	short x;
	short y;
	unsigned short vmNextLineOffset;
	unsigned short fbNextLineOffset;
	unsigned char far *vm;
	unsigned char far *fb;

	if (SavedRectangle == false) {
		return;
	}

	xa = RectangleStartX;
	ya = RectangleStartY;
	xb = RectangleEndX;
	yb = RectangleEndY;

	vmNextLineOffset = BytesPerLine - ((xb - xa) + 1);
	fbNextLineOffset = ResolutionX  - ((xb - xa) + 1);
	vm = AccessVideoMemory(WRITE, xa, ya);
	fb = AccessFrameBuffer(xa, (ya + ResolutionY));

	for (y = ya; y <= yb; y++) {
		for (x = xa; x <= xb; x++) {
			*vm = *fb;

			vm++;
			if (FP_OFF(vm) == 0) {
				NextVideoMemoryBlock(WRITE);
			}

			fb++;
			if (FP_OFF(fb) == 0) {
				NextFrameBufferBlock();
			}
		}

		if (y != yb) {
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

	/* think of a better way to handle this */
	vm = AccessVideoMemory(READ, xa, ya);
	fb = AccessFrameBuffer(xa, ya);

	for (y = ya; y <= yb; y++) {
		for (x = xa; x <= xb; x++) {
			*fb = *vm;

			vm++;
			if (FP_OFF(vm) == 0) {
				NextVideoMemoryBlock(READ);
			}

			fb++;
			if (FP_OFF(fb) == 0) {
				NextFrameBufferBlock();
			}
		}

		if (y != yb) {
			vm += vmNextLineOffset;
			if (FP_OFF(vm) < vmNextLineOffset) {
				NextVideoMemoryBlock(READ);
			}

			fb += fbNextLineOffset;
			if (FP_OFF(fb) < fbNextLineOffset) {
				NextFrameBufferBlock();
			}
		}
	}

	SavedRectangle = false;
}

/* ====================================================================== */
/*                                                                        */
/*   CopyRectangle                                                        */
/*                                                                        */
/* ====================================================================== */

void CopyRectangle(short xa, short ya, short xb, short yb)
{
}

/* ====================================================================== */
/*                                                                        */
/*   ClipRectangle                                                        */
/*                                                                        */
/* ====================================================================== */

int ClipRectangle(short far *xa, short far *ya,	short far *xb, short far *yb)
{
	if ((*xa > *xb) || (*ya > *yb)) {
		return false;
	}

	if ((*xa > WindowEndX) || (*xb < WindowStartX) ||
	    (*ya > WindowEndY) || (*yb < WindowStartY )) {
		return false;
	}

	if (*xa < WindowStartX) {
		*xa = WindowStartX;
	}

	if (*xb > WindowEndX) {
		*xb = WindowEndX;
	}

	if (*ya < WindowStartY ) {
		*ya = WindowStartY ;
	}

	if (*yb > WindowEndY) {
		*yb = WindowEndY;
	}

	Translate(xa, ya);
	Translate(xb, yb);
	return true;
}

/* -- END --------------------------------------------------------------- */
