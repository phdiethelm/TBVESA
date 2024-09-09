/* ********************************************************************** */
/*                                                                        */
/*   STATE.H                                                              */
/*                                                                        */
/* ********************************************************************** */

#ifndef __STATE_H__
#define __STATE_H__

/* -- HEADER FILES ------------------------------------------------------ */

#include "global.h"
#include "driver.h"
#include "resolutn.h"

/* -- PRAGMAS ----------------------------------------------------------- */

#pragma aux (DRIVER) GetBitmapBufferAddress;
#pragma aux (DRIVER) GetColor;
#pragma aux (DRIVER) SetColor;
#pragma aux (DRIVER) GetScale;
#pragma aux (DRIVER) SetScale;
#pragma aux (DRIVER) GetScreenResolution;
#pragma aux (DRIVER) SetWindow;
#pragma aux (DRIVER) SetWindowOrigin;
#pragma aux (DRIVER) SaveWindowState;
#pragma aux (DRIVER) RestoreWindowState;
#pragma aux (DRIVER) SetWindow32;
#pragma aux (DRIVER) MoveAbsolute;
#pragma aux (DRIVER) MoveRelative;
#pragma aux (DRIVER) SetModeHighlight;
#pragma aux (DRIVER) SetModeNormal;
#pragma aux (DRIVER) SetModeXor;

/* -- TYPE DEFINITIONS -------------------------------------------------- */

struct WindowState {
	unsigned char Color;
	unsigned short Scale;
	unsigned short DrawingPositionX;
	unsigned short DrawingPositionY;
	unsigned short WindowStartX;
	unsigned short WindowStartY;
	unsigned short WindowEndX;
	unsigned short WindowEndY;
	unsigned short WindowOriginX;
	unsigned short WindowOriginY;
};

/* -- PUBLIC DATA DECLARATIONS ------------------------------------------ */

extern unsigned char Color;
extern unsigned short Scale;
extern unsigned short DrawingPositionX;
extern unsigned short DrawingPositionY;
extern short WindowStartX;
extern short WindowStartY;
extern short WindowEndX;
extern short WindowEndY;
extern unsigned short WindowOriginX;
extern unsigned short WindowOriginY;
extern long WindowStartX32;
extern long WindowStartY32;
extern long WindowEndX32;
extern long WindowEndY32;
extern unsigned long Scale32;
extern unsigned long Scale32Div2;
extern unsigned char Mode;
extern bool CharSetSpecial;
extern bool CharSetIEEE;
extern unsigned char MapMask;
extern int LineDrawingMode;
extern int TopOfWindowStateStack;
extern unsigned char Palette[16];
extern unsigned char MapMaskMap[16];
extern struct WindowState WindowStateStack[8];

/* -- PUBLIC FUNCTION PROTOTYPES ---------------------------------------- */

extern unsigned char far *GetBitmapBufferAddress(void);
extern void GetScreenResolution(unsigned short far *const maxX,
	unsigned short far *const maxY);
extern unsigned char GetColor(void);
extern void SetColor(unsigned char index);
extern unsigned short GetScale(void);
extern void SetScale(unsigned short scale);
extern void SetWindow(short minX, short minY, short maxX, short maxY);
extern void SetWindowOrigin(unsigned short originX, unsigned short originY);
extern void SaveWindowState(void);
extern void RestoreWindowState(void);
extern void SetWindow32(long minX, long minY, long maxX, long maxY,
	unsigned long scale);
extern void MoveAbsolute(short x, short y);
extern void MoveRelative(short dx, short dy);
extern void SetModeHighlight(void);
extern void SetModeNormal(void);
extern void SetModeXor(void);

#endif

/* -- END --------------------------------------------------------------- */
