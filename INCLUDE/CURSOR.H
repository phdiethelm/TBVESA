/* ********************************************************************** */
/*                                                                        */
/*   CURSOR.H                                                             */
/*                                                                        */
/* ********************************************************************** */

#ifndef __CURSOR_H__
#define __CURSOR_H__

/* -- HEADER FILES ------------------------------------------------------ */

#include "global.h"
#include "state.h"
#include "video.h"
#include "resolutn.h"

/* -- PRAGMAS ----------------------------------------------------------- */

#pragma aux (DRIVER) SetCursorState;
#pragma aux (DRIVER) SetCursorStyle;

/* -- PUBLIC DATA DECLARATIONS ------------------------------------------ */

extern unsigned char CursorStyle;
extern unsigned char CursorState;

/* -- PUBLIC FUNCTION DECLARATIONS -------------------------------------- */

extern void SetCursorState(unsigned char state, short x, short y);
extern unsigned char SetCursorStyle(unsigned char style);

#endif

/* -- END --------------------------------------------------------------- */
