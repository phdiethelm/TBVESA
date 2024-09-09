/* ********************************************************************** */
/*                                                                        */
/*   DRIVER.H                                                             */
/*                                                                        */
/* ********************************************************************** */

#ifndef __DRIVER_H__
#define __DRIVER_H__

/* -- HEADER FILES ------------------------------------------------------ */

#include "global.h"
#include "bitmap.h"
#include "circle.h"
#include "cursor.h"
#include "grid.h"
#include "line.h"
#include "memory.h"
#include "rectngle.h"
#include "resolutn.h"
#include "state.h"
#include "text.h"
#include "video.h"

/* -- PRAGMAS ----------------------------------------------------------- */

#pragma aux (DRIVER) DriverMain;
#pragma aux (DRIVER) InitDriver;
#pragma aux (DRIVER) ShutdownDriver;
#pragma aux (DRIVER) AllocBuffers;
#pragma aux (DRIVER) FreeBuffers;

/* -- TYPE DEFINITIONS -------------------------------------------------- */

struct DriverInfo {
	unsigned short Version;
	void far *Interface;
	unsigned short Magic;
	unsigned short Attributes;
};

/* -- EXTERNAL DATA DECLARATIONS ---------------------------------------- */

extern int end;

/* -- PUBLIC DATA DECLARATIONS ------------------------------------------ */

extern unsigned short far *ScaleLookupTable;
extern unsigned char far *BitmapBuffer;

/* -- PUBLIC FUNCTION DECLARATIONS -------------------------------------- */

extern void PrintString(const char *s);

extern unsigned short DriverMain(struct DriverInfo far *const driverInfo);
extern void InitDriver(unsigned short far *const bitmapBufferSegment,
	unsigned short maxScale, unsigned char far *const dosError);
extern void ShutdownDriver(void);
extern void AllocBuffers(unsigned short far *const bitmapBufferSegment,
	unsigned short maxScale, unsigned char far *const dosError);
extern void FreeBuffers(void);

#endif

/* -- END --------------------------------------------------------------- */
