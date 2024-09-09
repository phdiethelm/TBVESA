/* ********************************************************************** */
/*                                                                        */
/*   GLOBAL.H                                                             */
/*                                                                        */
/* ********************************************************************** */

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/* -- HEADER FILES ------------------------------------------------------ */

#include <stdlib.h>
#include <i86.h>

/* -- PRAGMAS ---------------------------------------------------------- */

#pragma aux DRIVER "*" 		\
	parm reverse routine [] \
	modify [ax dx es] 	\
	loadds far;

/* -- MACROS ------------------------------------------------------------ */

#define NORMAL  0
#define AND	1
#define OR	2
#define XOR	3

#define READ	0
#define WRITE	1

#define SOLID	0
#define DASHED	1

void int3(void);

#pragma aux int3 =	\
	"int	03h"	\
	parm []		\
	modify exact []	\

/* -- TYPE DEFINITIONS -------------------------------------------------- */

typedef enum {false = 0, true = -1} bool;

#endif

/* -- END --------------------------------------------------------------- */
