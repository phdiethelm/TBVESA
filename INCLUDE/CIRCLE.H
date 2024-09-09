/* ********************************************************************** */
/*                                                                        */
/*   CIRCLE.H                                                             */
/*                                                                        */
/* ********************************************************************** */

#ifndef __CIRCLE_H__
#define __CIRCLE_H__

/* -- HEADER FILES ------------------------------------------------------ */

#include "global.h"
#include "driver.h"
#include "line.h"
#include "state.h"
#include "video.h"

/* -- PRAGMAS ----------------------------------------------------------- */

#pragma aux (DRIVER) DrawArcAbsolute;
#pragma aux (DRIVER) DrawWideArcAbsolute32;
#pragma aux (DRIVER) DrawCircleAbsolute;
#pragma aux (DRIVER) DrawCircleRelative;
#pragma aux (DRIVER) DrawCircleAbsolute32;

/* -- PUBLIC FUNCTION DECLARATIONS -------------------------------------- */

extern void DrawArcAbsolute(short xc, short yc, short xa, short ya,
	short xb, short yb, unsigned short radius, unsigned short thickness);
extern void DrawWideArcAbsolute32(long xc, long yc, long xa, long ya,
	long xb, long yb, unsigned long radius, unsigned long thickness,
	unsigned char fillMode);
extern void DrawCircleAbsolute(short xc, short yc, unsigned short length,
	unsigned short height, unsigned char fill);
extern void DrawCircleRelative(short dx, short dy, unsigned short length,
	unsigned short height, unsigned char fill);
extern void DrawCircleAbsolute32(long xc, long yc, unsigned long radius,
	unsigned char fill);

#endif

/* -- END --------------------------------------------------------------- */
