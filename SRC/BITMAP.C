/* ********************************************************************** */
/*                                                                        */
/*   BITMAP.C                                                             */
/*                                                                        */
/* ********************************************************************** */

/* -- HEADER FILES ------------------------------------------------------ */

#include "bitmap.h"

/* -- CODE -------------------------------------------------------------- */

static short clipxa, clipya, clipxb, clipyb;

static void ClpPixel(short x, short y)
{
	if (x >= clipxa && x <= clipxb && y >= clipya && y <= clipyb)
	{
		PutPixel(x, y);
	}
}

#define sign(a) (((a)<0)? -1:1)

static void Translate2(short far *const x, short far *const y)
{
	*x -= WindowStartX;
	*y -= WindowStartY;

	if (Scale > 1) {
		*x = sign(*x) * ScaleLookupTable[abs(*x)];
		*y = sign(*y) * ScaleLookupTable[abs(*y)];
	}

	*x += WindowOriginX;
	*y += WindowOriginY;
}

/* ====================================================================== */
/*                                                                        */
/*   DrawBitmapRelative                                                   */
/*                                                                        */
/* ====================================================================== */

void DrawBitmapRelative(const unsigned char far *bitmap, unsigned short rows,
	unsigned short cols, short dx, short dy, unsigned short xsize,
	unsigned short ysize, unsigned char function)
{
	short xa, xb, ya, yb;
	unsigned int i, j;
	int b = 0;

	xa = DrawingPositionX = DrawingPositionX + dx;
	ya = DrawingPositionY = DrawingPositionY + dy;

	if (xsize * ysize == 0)
	{
		xsize = cols * 8 - 1;
		ysize = rows - 1;
	}

	xb = xa + ((function & 1)? ysize : xsize) * Scale;
	yb = ya + ((function & 1)? xsize : ysize) * Scale;

	if (xa > WindowEndX || ya > WindowEndY || xb < WindowStartX || yb < WindowStartY)
	{
		return;
	}

	if (xb > WindowEndX || yb > WindowEndY || xa < WindowStartX || ya < WindowStartY)
	{
		clipxa = WindowStartX;
		clipya = WindowStartY;
		clipxb = WindowEndX;
		clipyb = WindowEndY;
		Translate(&clipxa, &clipya);
		Translate(&clipxb, &clipyb);
		function += 8;
	}
	Translate2(&xa, &ya);
	for (i = 0; i <= ysize; i++)
	{
		for (j = 0; j <= xsize; j++)
		{
			if (*bitmap & (0x80 >> b))
			{
				switch (function)
				{
					case 0:  PutPixel(xa +         j, ya +         i); break;
					case 1:  PutPixel(xa +         i, ya + xsize - j); break;
					case 2:  PutPixel(xa + xsize - j, ya + ysize - i); break;
					case 3:  PutPixel(xa + ysize - i, ya +         j); break;
					case 4:  PutPixel(xa + xsize - j, ya +         i); break;
					case 5:  PutPixel(xa +         i, ya +         j); break;
					case 6:  PutPixel(xa +         j, ya + ysize - i); break;
					case 7:  PutPixel(xa + ysize - i, ya + xsize - j); break;

					case 8:  ClpPixel(xa +         j, ya +         i); break;
					case 9:  ClpPixel(xa +         i, ya + xsize - j); break;
					case 10: ClpPixel(xa + xsize - j, ya + ysize - i); break;
					case 11: ClpPixel(xa + ysize - i, ya +         j); break;
					case 12: ClpPixel(xa + xsize - j, ya +         i); break;
					case 13: ClpPixel(xa +         i, ya +         j); break;
					case 14: ClpPixel(xa +         j, ya + ysize - i); break;
					case 15: ClpPixel(xa + ysize - i, ya + xsize - j); break;
				}
			}
			if (++b == 8)
			{
				b = 0;
				++bitmap;
			}
		}
		if (b)
		{
			b = 0;
			++bitmap;
		}
	}
}

/* -- END --------------------------------------------------------------- */
