/* ********************************************************************** */
/*                                                                        */
/*   CIRCLE.C                                                             */
/*                                                                        */
/* ********************************************************************** */

/* -- HEADER FILES ------------------------------------------------------ */

#include "circle.h"

/* -- PRIVATE FUNCTION DECLARATIONS ------------------------------------- */

static void DrawArc32(long xc, long yc, long xa, long ya, long xb, long yb,
	unsigned long radius, unsigned long thickness);
static void DrawCircleSector(short xc, short yc, short dx, short dy,
	unsigned short distance, int type, int sector, unsigned char fill);
static void DrawCircleSector32(long xc, long yc, long dx, long dy,
	int sector, unsigned char fill);
static void DrawCirclePoint(short x, short y);
static void DrawCirclePoint32(long x, long y);
static unsigned char GetQuadrant(long xc, long yc, long far *const x,
	long far *const y, long radius);

/* -- CODE -------------------------------------------------------------- */

/* ====================================================================== */
/*                                                                        */
/*   DrawArcAbsolute                                                      */
/*                                                                        */
/* ====================================================================== */

void DrawArcAbsolute(short xc, short yc, short xa, short ya,
	short xb, short yb, unsigned short radius, unsigned short thickness)
{
	short xi;
	short yi;
	short xo;
	short yo;
	unsigned short radiusi;
	unsigned short radiuso;
	int epsi;
	int ui;
	int vi;
	int epso;
	int uo;
	int vo;
	short mulX;
	short mulY;
	unsigned char drawPoint;

	/*
	 * this departs from the behaviour of the reference implementation
	 * which updated DrawingPositionX and DrawingPositionY only if
	 * the arc was at least partly visible
	 */
	DrawingPositionX = xc;
	DrawingPositionY = yc;

	if (((max(xa, xb) + thickness) < WindowStartX)
	||  ((min(xa, xb) - thickness) > WindowEndX)
	||  ((max(ya, yb) + thickness) < WindowStartY)
	||  ((min(ya, yb) - thickness) > WindowEndY)) {
		return;
	}

	if ((xa < xc) || (xb < xc)) {
		mulX = -Scale;
	} else {
		mulX = Scale;
	}

	if ((ya < yc) || (yb < yc)) {
		mulY = -Scale;
	} else {
		mulY = Scale;
	}

	xa = abs(xa - DrawingPositionX);
	ya = abs(ya - DrawingPositionY);
	xb = abs(xb - DrawingPositionX);
	yb = abs(yb - DrawingPositionY);

	if (Scale > 1) {
		xa = ScaleLookupTable[xa];
		ya = ScaleLookupTable[ya];
		xb = ScaleLookupTable[xb];
		yb = ScaleLookupTable[yb];
		thickness = ScaleLookupTable[thickness];
		radius = ScaleLookupTable[radius];
	}

	if (thickness == 0) {
		thickness = 1;
	}

	if (radius == 0) {
		radius = 1;
	}

	if (thickness == 1) {
		xi = radius;
		yi = 0;

		epsi = 2 * (radius - 1);
		ui = 4 * radius;
		vi = 0;

		drawPoint = 0;
		while (xi >= 0) {
			if (drawPoint != 0) {
				DrawCirclePoint((xc + (xi * mulX)),
					(yc + (yi * mulY)));
			}

			if (((xi == xa) && (yi == ya))
			||  ((xi == xb) && (yi == yb))) {
				drawPoint ^= 0xff;
			}

			if (epsi > 0) {
				yi++; vi += 4; epsi -= vi;
			} else {
				xi--; ui -= 4; epsi += ui;
			}
		}

		DrawCirclePoint((xc + (xa * mulX)), (yc + (ya * mulY)));
		DrawCirclePoint((xc + (xb * mulX)), (yc + (yb * mulY)));
	} else {
		radiusi = radius - (thickness / 2);
		xi = radiusi;
		yi = 0;

		epsi = 2 * (radiusi - 1);
		ui = 4 * radiusi;
		vi = 0;

		radiuso = radius + (thickness / 2);
		xo = radiuso;
		yo = 0;

		epso = 2 * (radiuso - 1);
		uo = 4 * radiuso;
		vo = 0;

		DrawLineFunction(xc + (xi * mulX), yc + (yi * mulY),
			xc + (xo * mulX), yc + (yo * mulY));

		while (yo != radiuso) {
			while (epso <= 0) {
				uo -= 4; epso += uo; xo--;
			}

			vo += 4; epso -= vo; yo++;

			if (yi < radiusi) {
				if (epsi > 0) {
					vi += 4; epsi -= vi; yi++;
				}

				while (epsi <= 0) {
					xi--;
					if (xi <= 0) {
						xi = 0;
						break;
					}

					ui -= 4; epsi += ui;
				}
			} else {
				yi++;
			}

			DrawLineFunction(xc + (xi * mulX), yc + (yi * mulY),
				xc + (xo * mulX), yc + (yo * mulY));
		}
	}

	DrawingPositionX = xc;
	DrawingPositionY = yc;
}

/* ====================================================================== */
/*                                                                        */
/*   DrawWideArcAbsolute32                                                */
/*                                                                        */
/* ====================================================================== */

void DrawWideArcAbsolute32(long xc, long yc, long xa, long ya,
	long xb, long yb, unsigned long radius, unsigned long thickness,
	unsigned char fillMode)
{
	unsigned char quadrants;
	long t;
	long xbi;
	long xbo;
	long xarc;
	long yai;
	long yao;
	long yarc;

	if (((fillMode == 0) || (fillMode == 3)) && (thickness >= 3)) {
		quadrants  = GetQuadrant(xc, yc, &xa, &ya, radius);
		quadrants |= GetQuadrant(xc, yc, &xb, &yb, radius) << 2;

		if ((quadrants == 4)  || (quadrants == 6)
		||  (quadrants == 12) || (quadrants == 14)) {
			t = xa; xa = xb; xb = t;
			t = ya; ya = yb; yb = t;
		}

		switch (quadrants) {
			case 1:
			case 4:
				xbi =  xb - (thickness / 2);
				xbo =  xb + (thickness / 2);
				xarc = xa - (thickness / 2);
				yai =  ya + (thickness / 2);
				yao =  ya - (thickness / 2);
				yarc = yb + (thickness / 2);
				break;
			case 6:
			case 9:
				xbi =  xb + (thickness / 2);
				xbo =  xb - (thickness / 2);
				xarc = xa + (thickness / 2);
				yai =  ya + (thickness / 2);
				yao =  ya - (thickness / 2);
				yarc = yb + (thickness / 2);
				break;
			case 11:
			case 14:
				xbi =  xb + (thickness / 2);
				xbo =  xb - (thickness / 2);
				xarc = xa + (thickness / 2);
				yai =  ya - (thickness / 2);
				yao =  ya + (thickness / 2);
				yarc = yb - (thickness / 2);
				break;
			case 3:
			case 12:
				xbi =  xb - (thickness / 2);
				xbo =  xb + (thickness / 2);
				xarc = xa - (thickness / 2);
				yai =  ya - (thickness / 2);
				yao =  ya + (thickness / 2);
				yarc = yb - (thickness / 2);
				break;
			default:
				return;
		}

		DrawArc32(xc, yc, xa, yai, xbo, yb, radius - (thickness / 2), 1);
		DrawArc32(xc, yc, xa, yao, xbo, yb, radius + (thickness / 2), 1);

		if (fillMode == 3) {
			DrawArc32(xc, ya, xc, yai, xarc, ya, (thickness / 2), 1);
			DrawArc32(xc, ya, xarc, ya, xc, yao, (thickness / 2), 1);
			DrawArc32(xb, yc, xbi, yc, xb, yarc, (thickness / 2), 1);
			DrawArc32(xb, yc, xb, yarc, xbo, yc, (thickness / 2), 1);
		} else {
			DrawWideLineAbsolute32(xbi, yc, xbo, yc, 0, 0);
			DrawWideLineAbsolute32(xc, yai, xc, yao, 0, 0);
		}
	} else {
		DrawArc32(xc, yc, xa, ya, xb, yb, radius, thickness);

		if (fillMode == 2) {
			DrawCircleAbsolute32(xa, ya, (thickness / 2), 1);
			DrawCircleAbsolute32(xb, yb, (thickness / 2), 1);
		}
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawCircleAbsolute                                                   */
/*                                                                        */
/* ====================================================================== */

void DrawCircleAbsolute(short xc, short yc, unsigned short length,
	unsigned short height, unsigned char fill)
{
	short dx;
	short dy;
	int eps;
	int u;
	int v;
	unsigned short radius;
	unsigned short distance;
	int type;
	int sector;

	DrawingPositionX = xc;
	DrawingPositionY = yc;

	if (((signed short)(xc - length) > WindowEndX)
	||  ((signed short)(xc + length) < WindowStartX)
	||  ((signed short)(yc - height) > WindowEndY)
	||  ((signed short)(yc + height) < WindowStartY)) {
		return;
	}

	if (Scale > 1) {
		length = ScaleLookupTable[length];
		height = ScaleLookupTable[height];
	}

	if ((length == 0) || (height == 0)) {
		return;
	}

	radius = min(length, height);
	distance = abs(length - height);

	if (length == height) {
		type = 0;

	/*
	 * horizontal and vertical lines between both half-circles are
	 * drawn 1 pixel shorter in both directions as compared to the
	 * reference implementation; this yields better visual results
	 * when drawn in xor-mode
	 */
	} else if (length > height) {
		type = 1;
		DrawLineFunction((xc - distance + 1), (yc - radius),
			(xc + distance - 1), (yc - radius));
		DrawLineFunction((xc - distance + 1), (yc + radius),
			(xc + distance - 1), (yc + radius));
	} else {
		type = -1;
		DrawLineFunction((xc - radius), (yc - distance + 1),
			(xc - radius), (yc + distance - 1));
		DrawLineFunction((xc + radius), (yc - distance + 1),
			(xc + radius), (yc + distance - 1));
	}

	for (sector = 0; sector < 4; sector++) {
		dx = 0;
		dy = (radius * Scale);

		eps = 1 - radius;
		u = 1;
		v = 1 - (2 * radius);

		while(dx < dy) {
			if ((fill == 0)
			||  ((fill != 0) && ((sector == 1) || (sector == 2)))) {
				DrawCircleSector(xc, yc, dx, dy, distance,
					type, sector, fill);
			}

			u += 2;
			if (eps < 0) {
				v += 2; eps += u;
			} else {
				if ((fill != 0) && ((sector == 0) || (sector == 3))) {
					DrawCircleSector(xc, yc, dx, dy, distance,
					type, sector, fill);
				}
				v += 4; eps += v;
				dy -= Scale;
			}
			dx += Scale;
		}

		if ((dx == dy) && ((sector == 0) || (sector == 3))) {
			DrawCircleSector(xc, yc, dx, dy, distance, type,
				sector, fill);
		}
	}

	/*
	 * on exit, DrawingPositionX and DrawingPositionY may have values
	 * different to those of the reference implementation
	 */
	DrawingPositionX = xc;
	DrawingPositionY = yc;
}

/* ====================================================================== */
/*                                                                        */
/*   DrawCircleRelative                                                   */
/*                                                                        */
/* ====================================================================== */

void DrawCircleRelative(short dx, short dy, unsigned short length,
	unsigned short height, unsigned char fill)
{
	DrawCircleAbsolute((DrawingPositionX + dx), (DrawingPositionY + dy),
		length, height, fill);
}

/* ====================================================================== */
/*                                                                        */
/*   DrawCircleAbsolute32                                                 */
/*                                                                        */
/* ====================================================================== */

void DrawCircleAbsolute32(long xc, long yc, unsigned long radius,
	unsigned char fill)
{
	long dx;
	long dy;
	short x;
	short y;
	int eps;
	int u;
	int v;
	int sector;

	if (((xc - radius) > WindowEndX32)
	||  ((xc + radius) < WindowStartX32)
	||  ((yc - radius) > WindowEndY32)
	||  ((yc + radius) < WindowStartY32)) {
		return;
	}

	radius /= Scale32;
	if (radius == 0) {
		return;
	}

	x = DrawingPositionX;
	y = DrawingPositionY;

	for (sector = 0; sector < 4; sector++) {
		dx = 0;
		dy = (radius * Scale32);

		eps = 1 - radius;
		u = 1;
		v = 1 - (2 * radius);

		while(dx < dy) {
			if ((fill == 0)
			||  ((fill != 0) && ((sector == 1) || (sector == 2)))) {
				DrawCircleSector32(xc, yc, dx, dy, sector, fill);
			}

			u += 2;
			if (eps < 0) {
				v += 2; eps += u;
			} else {
				if ((fill != 0) && ((sector == 0) || (sector == 3))) {
					DrawCircleSector32(xc, yc, dx, dy, sector, fill);
				}
				v += 4; eps += v;
				dy -= Scale32;
			}
			dx += Scale32;
		}

		if ((dx == dy) && ((sector == 0) || (sector == 3))) {
			DrawCircleSector32(xc, yc, dx, dy, sector, fill);
		}
	}

	/*
	 * on exit, DrawingPositionX and DrawingPositionY may have values
	 * different to those of the reference implementation
	 */
	DrawingPositionX = x;
	DrawingPositionY = y;
}

/* ====================================================================== */
/*                                                                        */
/*   DrawArc32                                                            */
/*                                                                        */
/* ====================================================================== */

void DrawArc32(long xc, long yc, long xa, long ya, long xb, long yb,
	unsigned long radius, unsigned long thickness)
{
	long xi;
	long yi;
	long xo;
	long yo;
	unsigned long radiusi;
	unsigned long radiuso;
	long epsi;
	long ui;
	long vi;
	long epso;
	long uo;
	long vo;
	long mulX;
	long mulY;

	if (((max(xa, xb) + thickness) < WindowStartX32)
	||  ((min(xa, xb) - thickness) > WindowEndX32)
	||  ((max(ya, yb) + thickness) < WindowStartY32)
	||  ((min(ya, yb) - thickness) > WindowEndY32)) {
		return;
	}

	thickness /= Scale32;
	if (thickness == 0) {
		thickness = 1;
	}

	radius /= Scale32;
	if (radius == 0) {
		radius = 1;
	}

	if ((xa < xc) || (xb < xc)) {
		mulX = -Scale32;
	} else {
		mulX = Scale32;
	}

	if ((ya < yc) || (yb < yc)) {
		mulY = -Scale32;
	} else {
		mulY = Scale32;
	}

	if (thickness == 1) {
		xi = radius;
		yi = 0;

		epsi = 2 * (radius - 1);
		ui = 4 * radius;
		vi = 0;

		while (xi >= 0) {
			DrawCirclePoint32(xc + (xi * mulX),
				yc + (yi * mulY));

			if (epsi > 0) {
				vi += 4; epsi -= vi; yi++;
			} else {
				ui -= 4; epsi += ui; xi--;
			}
		}
	} else {
		radiusi = radius - (thickness / 2);
		xi = radiusi;
		yi = 0;

		epsi = (radiusi - 1) * 2;
		ui = 4 * radiusi;
		vi = 0;

		radiuso = radius + (thickness / 2);
		xo = radiuso;
		yo = 0;

		epso = (radiuso - 1) * 2;
		uo = 4 * radiuso;
		vo = 0;

		DrawWideLineAbsolute32((xc + (xi * mulX)),
			(yc + (yi * mulY)), (xc + (xo * mulX)),
			(yc + (yo * mulY)), 0, 0);

		while (yo != radiuso) {
			while (epso <= 0) {
				uo -= 4; epso += uo; xo--;
			}

			vo += 4; epso -= vo; yo++;

			if (yi < radiusi) {
				if (epsi > 0) {
					vi += 4; epsi -= vi; yi++;
				}

				while (epsi <= 0) {
					xi--;
					if (xi <= 0) {
						xi = 0;
						break;
					}

					ui -= 4; epsi += ui;
				}
			} else {
				yi++;
			}

			DrawWideLineAbsolute32((xc + (xi * mulX)),
				(yc + (yi * mulY)), (xc + (xo * mulX)),
				(yc + (yo * mulY)), 0, 0);
		}
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawCircleSector                                                     */
/*                                                                        */
/* ====================================================================== */

void DrawCircleSector(short xc, short yc, short dx, short dy,
	unsigned short distance, int type, int sector, unsigned char fill)
{
	short t;

	switch (sector) {
		case 0:
			break;
		case 1:
			t = dx; dx = dy; dy = t;
			break;
		case 2:
			if (dx == 0) {
				return;
			}
			t = dx; dx = dy; dy = -t;
			break;
		case 3:
			dy = -dy;
			break;

	}

	if (fill == 0) {
		if (type == 0) {
			DrawCirclePoint((xc - dx), (yc - dy));

			if (dx != 0) {
				DrawCirclePoint((xc + dx), (yc - dy));
			}
		} else if (type == 1) {
			DrawCirclePoint((xc - dx - distance), (yc - dy));
			DrawCirclePoint((xc + dx + distance), (yc - dy));
		} else {
			DrawCirclePoint((xc - dx), (yc - dy - distance));
			DrawCirclePoint((xc + dx), (yc - dy - distance));
		}
	} else {
		if (type == 0) {
			DrawLineFunction((xc - dx), (yc - dy), (xc + dx), (yc - dy));
		} else if (type == 1) {
			DrawLineFunction((xc - dx - distance), (yc - dy), (xc - distance), (yc - dy));
			DrawLineFunction((xc + dx + distance), (yc - dy), (xc + distance), (yc - dy));
		} else {
			DrawLineFunction((xc - dx), (yc - dy - distance), (xc + dx), (yc - distance));
			DrawLineFunction((xc - dx), (yc - dy - distance), (xc + dx), (yc - distance));
		}
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawCircleSector32                                                   */
/*                                                                        */
/* ====================================================================== */

void DrawCircleSector32(long xc, long yc, long dx, long dy, int sector,
	unsigned char fill)
{
	long t;

	switch (sector) {
		case 0:
			break;
		case 1:
			t = dx; dx = dy; dy = t;
			break;
		case 2:
			if (dx == 0) {
				return;
			}
			t = dx; dx = dy; dy = -t;
			break;
		case 3:
			dy = -dy;
			break;
	}

	if (fill == 0) {
		DrawCirclePoint32((xc - dx), (yc - dy));

		if (dx != 0) {
			DrawCirclePoint32((xc + dx), (yc - dy));
		}
	} else {
		DrawWideLineAbsolute32((xc - dx), (yc - dy),
			(xc + dx), (yc - dy), 0, 0);
	}
}

/* ====================================================================== */
/*                                                                        */
/*   DrawCirclePoint                                                      */
/*                                                                        */
/* ====================================================================== */

void DrawCirclePoint(short x, short y)
{
	if ((x < WindowStartX) || (x > WindowEndX)
	||  (y < WindowStartY) || (y > WindowEndY)) {
		return;
	}

	Translate(&x, &y);
	PutPixel(x, y);
}

/* ====================================================================== */
/*                                                                        */
/*   DrawCirclePoint32                                                    */
/*                                                                        */
/* ====================================================================== */

void DrawCirclePoint32(long x, long y)
{
	if ((x < WindowStartX32) || (x > WindowEndX32)
	||  (y < WindowStartY32) || (y > WindowEndY32)) {
		return;
	}

	x -= WindowStartX32;
	y -= WindowStartY32;

	x = ((long long)x + Scale32Div2) / Scale32;
	y = ((long long)y + Scale32Div2) / Scale32;

	x += WindowOriginX;
	y += WindowOriginY;

	PutPixel(x, y);
}

/* ====================================================================== */
/*                                                                        */
/*   GetQuadrant                                                          */
/*                                                                        */
/* ====================================================================== */

unsigned char GetQuadrant(long xc, long yc, long far *const x,
	long far *const y, long radius)
{
	unsigned char quadrant;

	if (*x == xc) {
		if (*y > yc) {
			quadrant = 3;
			*y = yc + radius;
		} else {
			quadrant = 1;
			*y = yc - radius;
		}
	} else {
		if (*x > xc) {
			quadrant = 2;
			*x = xc + radius;
		} else {
			quadrant = 0;
			*x = xc - radius;
		}
	}

	return quadrant;
}

/* -- END --------------------------------------------------------------- */
