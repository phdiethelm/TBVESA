
/* ********************************************************************** */
/*                                                                        */
/*   LINE.C                                                               */
/*                                                                        */
/* ********************************************************************** */

/* -- HEADER FILES ------------------------------------------------------ */

#include "line.h"

/* -- PRIVATE FUNCTION DECLARATIONS ------------------------------------- */

static bool ClipLine(short far *const xa, short far *const ya,
	short far *const xb, short far *const yb);
static unsigned char GetOutcode(short x, short y);
static void DrawSolidLine(unsigned short xa, unsigned short ya,
	unsigned short xb, unsigned short yb);
static void DrawSolidLineHorizontal(unsigned short x,
	unsigned short y, short length);
static void DrawSolidLineVertical(unsigned short x,
	unsigned short y, short length);
static void DrawDashedLine(unsigned short xa, unsigned short ya,
	unsigned short xb, unsigned short yb);

/* -- CODE -------------------------------------------------------------- */

/* ====================================================================== */
/*                                                                        */
/*   DrawWideLineAbsolute                                                 */
/*                                                                        */
/* ====================================================================== */

void DrawWideLineAbsolute(short xa, short ya, short xb, short yb)
{
	short dx, dy;

	DrawingPositionX = xa;
	dx = (xb - xa);
	DrawingPositionY = ya;
	dy = (yb - ya);

	DrawWideLineRelative(dx, dy);
}

/* ====================================================================== */
/*                                                                        */
/*   DrawWideLineRelative                                                 */
/*                                                                        */
/* ====================================================================== */

void DrawWideLineRelative(short dx, short dy)
{
	unsigned short savedX;
	unsigned short savedY;
	unsigned short xa;
	unsigned short ya;
	unsigned short xb;
	unsigned short yb;
	unsigned short index;

	static short table[64] = {
		 0, -1,  0, -1,  0,  1,  0,  1,
		-1,  0, -1,  0,  1,  0,  1,  0,
		-1,  0,  0, -1,  0,  1,  1,  0,
		 0, -1,  1,  0, -1,  0,  0,  1,

		 0, -2,  0, -2,  0,  2,  0,  2,
		-2,  0, -2,  0,  2,  0,  2,  0,
		-2,  0,  0, -2,  0,  2,  2,  0,
		 0, -2,  2,  0, -2,  0,  0,  2
	};

	if (Scale > 2) {
		DrawSolidLineRelative(dx, dy);
		return;
	}

	if (dx == 0) {
		DrawingPositionX -= Scale;
		DrawSolidLineRelative(0, dy);
		DrawingPositionX += (Scale << 1);
		DrawSolidLineRelative(0, -dy);
		DrawingPositionX -= Scale;
		DrawSolidLineRelative(0, dy);
	} else if (dy == 0) {
		DrawingPositionY -= Scale;
		DrawSolidLineRelative(dx, 0);
		DrawingPositionY += (Scale << 1);
		DrawSolidLineRelative(-dx, 0);
		DrawingPositionY -= Scale;
		DrawSolidLineRelative(dx, 0);
	} else {
		if (abs(dx) < abs(dy)) {
			index = 0;
		} else if (abs(dx) > abs(dy)) {
			index = 8;
		} else if (dx == dy) {
			index = 24;
		} else {
			index = 16;
		}

		if (Scale == 2) {
			index += 32;
		}

		savedX = DrawingPositionX;
		savedY = DrawingPositionY;

		xa = savedX + table[index];
		ya = savedY + table[index+1];
		xb = savedX + dx + table[index+2];
		yb = savedY + dy + table[index+3];
		DrawLineFunction(xa, ya, xb, yb);

		xa = savedX + table[index+4];
		ya = savedY + table[index+5];
		xb = savedX + dx + table[index+6];
		yb = savedY + dy + table[index+7];
		DrawLineFunction(xa, ya, xb, yb);

		xa = savedX;
		ya = savedY;
		xb = savedX + dx;
		yb = savedY + dy;
		DrawLineFunction(xa, ya, xb, yb);
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawDashedLineAbsolute                                               */
/*                                                                        */
/* ====================================================================== */

void DrawDashedLineAbsolute(short xa, short ya, short xb, short yb)
{
	DrawingPositionX = xb;
	DrawingPositionY = yb;

	if (ClipLine(&xa, &ya, &xb, &yb) == true) {
		DrawDashedLine(xa, ya, xb, yb);
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawDashedLineRelative                                               */
/*                                                                        */
/* ====================================================================== */

void DrawDashedLineRelative(short dx, short dy)
{
	short xa;
	short ya;
	short xb;
	short yb;

	xa = DrawingPositionX;
	ya = DrawingPositionY;
	xb = xa + dx;
	yb = ya + dy;

	DrawDashedLineAbsolute(xa, ya, xb, yb);
}

/* ====================================================================== */
/*                                                                        */
/*   DrawSolidLineAbsolute                                                */
/*                                                                        */
/* ====================================================================== */

void DrawSolidLineAbsolute(short xa, short ya, short xb, short yb)
{
	DrawingPositionX = xb;
	DrawingPositionY = yb;

	if (ClipLine(&xa, &ya, &xb, &yb) == true) {
		DrawSolidLine(xa, ya, xb, yb);
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawSolidLineRelative                                                */
/*                                                                        */
/* ====================================================================== */

void DrawSolidLineRelative(short dx, short dy)
{
	short xa;
	short ya;
	short xb;
	short yb;

	xa = DrawingPositionX;
	ya = DrawingPositionY;
	xb = xa + dx;
	yb = ya + dy;

	DrawSolidLineAbsolute(xa, ya, xb, yb);
}

/* ====================================================================== */
/*                                                                        */
/*   DrawWideLineAbsolute32                                               */
/*                                                                        */
/* ====================================================================== */

/*
void DrawWideLineAbsolute32(long xa, long ya, long xb, long yb,
	unsigned long thickness, unsigned char fillType)
{
}
*/

/* ====================================================================== */
/*                                                                        */
/*   DrawLineFunction                                                     */
/*                                                                        */
/* ====================================================================== */

void DrawLineFunction(short xa, short ya, short xb, short yb)
{
	if (LineDrawingMode == SOLID) {
		DrawSolidLineAbsolute(xa, ya, xb, yb);
	} else if (LineDrawingMode == DASHED) {
		DrawDashedLineAbsolute(xa, ya, xb, yb);
	}
}

/* ====================================================================== */
/*                                                                        */
/*   ClipLine                                                             */
/*                                                                        */
/* ====================================================================== */

bool ClipLine(short far *const xa, short far *const ya,
	short far *const xb, short far *const yb)
{
	short dx;
	short dy;
	short t;
	unsigned char outcode;
	unsigned char outcodeA;
	unsigned char outcodeB;

	dx = (*xb - *xa);
	dy = (*yb - *ya);

	if (dx == 0) {
		if ((*xa < WindowStartX) || (*xa > WindowEndX)) {
			return false;
		}

		if (*ya > *yb) {
			t = *ya; *ya = *yb; *yb = t;
		}

		if ((*ya > WindowEndY) || (*yb < WindowStartY)) {
			return false;
		}

		if (*ya < WindowStartY) {
			*ya = WindowStartY;
		}

		if (*yb > WindowEndY) {
			*yb = WindowEndY;
		}
	} else if (dy == 0) {
		if ((*ya < WindowStartY) || (*ya > WindowEndY)) {
			return false;
		}

		if (*xa > *xb) {
			t = *xa; *xa = *xb; *xb = t;
		}

		if ((*xa > WindowEndX) || (*xb < WindowStartX)) {
			return false;
		}

		if (*xa < WindowStartX) {
			*xa = WindowStartX;
		}

		if (*xb > WindowEndX) {
			*xb = WindowEndX;
		}
	} else {
		for (;;) {
			outcodeA = GetOutcode(*xa, *ya);
			outcodeB = GetOutcode(*xb, *yb);

			if ((outcodeA == INSIDE) && (outcodeB == INSIDE)) {
				break;
			} else if (outcodeA & outcodeB) {
				return false;
			}

			if (outcodeA == INSIDE) {
				t = *xa; *xa = *xb; *xb = t;
				t = *ya; *ya = *yb; *yb = t;
				outcode = outcodeB;
			} else {
				outcode = outcodeA;
			}

			dx = (*xb - *xa);
			dy = (*yb - *ya);

			if (outcode & LEFT) {
				*ya += ((long)dy * (WindowStartX - *xa)) / dx;
				*xa = WindowStartX;
			} else if (outcode & RIGHT) {
				*ya += ((long)dy * (WindowEndX - *xa)) / dx;
				*xa = WindowEndX;
			} else if (outcode & TOP) {
				*xa += ((long)dx * (WindowStartY - *ya)) / dy;
				*ya = WindowStartY;
			} else if (outcode & BOTTOM) {
				*xa += ((long)dx * (WindowEndY - *ya)) / dy;
				*ya = WindowEndY;
			}
		}
	}

	Translate(xa, ya);
	Translate(xb, yb);
	return true;
}

/* ====================================================================== */
/*                                                                        */
/*   GetOutcode                                                           */
/*                                                                        */
/* ====================================================================== */

unsigned char GetOutcode(short x, short y)
{
	unsigned char outcode = INSIDE;

	if (x < WindowStartX) {
		outcode |= LEFT;
	} else if (x > WindowEndX) {
		outcode |= RIGHT;
	}

	if (y < WindowStartY) {
		outcode |= TOP;
	} else if (y > WindowEndY) {
		outcode |= BOTTOM;
	}

	return outcode;
}

/* ====================================================================== */
/*                                                                        */
/*   DrawSolidLine                                                        */
/*                                                                        */
/* ====================================================================== */

void DrawSolidLine(unsigned short xa, unsigned short ya,
	unsigned short xb, unsigned short yb)
{
	unsigned short x;
	unsigned short y;
	unsigned short t;
	short dx;
	short dy;
	short stepX;
	short stepY;
	short e;

	dx = (xb - xa);
	dy = (yb - ya);

	if (dy == 0) {
		DrawSolidLineHorizontal(xa, ya, dx);
	} else if (dx == 0) {
		DrawSolidLineVertical(xa, ya, dy);
	} else {
		if (dx < 0) {
			dx = -dx;
			stepX = -1;
		} else {
			stepX = 1;
		}

		if (dy < 0) {
			dy = -dy;
			stepY = -1;
		} else {
			stepY = 1;
		}

		dx <<= 1;
		dy <<= 1;

		if (dx > dy) {
			if (stepX == 1) {
				t = xa; xa = xb; xb = t;
				t = ya; ya = yb; yb = t;
				stepY = -stepY;
			}

			y = ya;
			e = -(dx >> 1);
			for (x = xa; x != (xb - 1); x--) {
				PutPixel(x, y);

				e += dy;
				if (e >= 0) {
					y += stepY;
					e -= dx;
				}
			}
		} else {
			if (stepY == 1) {
				t = xa; xa = xb; xb = t;
				t = ya; ya = yb; yb = t;
				stepX = -stepX;
			}

			x = xa;
			e = -(dy >> 1);
			for (y = ya; y != (yb - 1); y--) {
				PutPixel(x, y);

				e += dx;
				if (e >= 0) {
					x += stepX;
					e -= dy;
				}
			}
		}
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawSolidLineHorizontal                                              */
/*                                                                        */
/* ====================================================================== */

void DrawSolidLineHorizontal(unsigned short x,
	unsigned short y, short length)
{
	unsigned char far* vm;
	unsigned char far* fb;
	unsigned char pixel;

	if (length < 0) {
		length = -length;
		x -= length;
	}

	vm = AccessVideoMemory(WRITE, x, y);
	fb = AccessFrameBuffer(x, y);

	while (length >= 0) {
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

		length--;
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawSolidLineVertical                                                */
/*                                                                        */
/* ====================================================================== */

void DrawSolidLineVertical(unsigned short x,
	unsigned short y, short length)
{
	unsigned char far* vm;
	unsigned char far* fb;
	unsigned char pixel;

	if (length < 0) {
		length = -length;
		y -= length;
	}

	vm = AccessVideoMemory(WRITE, x, y);
	fb = AccessFrameBuffer(x, y);

	while (length >= 0) {
		pixel = *fb;
		if (Mode == OR) {
			pixel |= MapMask;
		} else {
			pixel ^= MapMask;
		}

		*vm = pixel;
		*fb = pixel;

		vm += BytesPerLine;
		if (FP_OFF(vm) < BytesPerLine) {
			NextVideoMemoryBlock(WRITE);
		}

		fb += ResolutionX;
		if (FP_OFF(fb) < ResolutionX) {
			NextFrameBufferBlock();
		}

		length--;
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawDashedLine                                                       */
/*                                                                        */
/* ====================================================================== */

void DrawDashedLine(unsigned short xa, unsigned short ya,
	unsigned short xb, unsigned short yb)
{
	unsigned short x;
	unsigned short y;
	unsigned short t;
	short dx = (xb - xa);
	short dy = (yb - ya);
	short stepX;
	short stepY;
	short e;
	unsigned short stipple = 0;

	if (dx < 0) {
		dx = -dx;
		stepX = -1;
	} else {
		stepX = 1;
	}

	if (dy < 0) {
		dy = -dy;
		stepY = -1;
	} else {
		stepY = 1;
	}

	dx <<= 1;
	dy <<= 1;

	if (dx > dy) {
		if (stepX == 1) {
			t = xa; xa = xb; xb = t;
			t = ya; ya = yb; yb = t;
			stepY = -stepY;
		}

		y = ya;
		e = -(dx >> 1);
		for (x = xa; x != (xb - 1); x--) {
			if ((stipple & 2) == 0) {
				PutPixel(x, y);
			}

			stipple++;

			e += dy;
			if (e >= 0) {
				y += stepY;
				e -= dx;
			}
		}
	} else {
		if (stepY == 1) {
			t = xa; xa = xb; xb = t;
			t = ya; ya = yb; yb = t;
			stepX = -stepX;
		}

		x = xa;
		e = -(dy >> 1);
		for (y = ya; y != (yb - 1); y--) {
			if ((stipple & 2) == 0) {
				PutPixel(x, y);
			}

			stipple++;

			e += dx;
			if (e >= 0) {
				x += stepX;
				e -= dy;
			}
		}
	}
}

/* -- END --------------------------------------------------------------- */
