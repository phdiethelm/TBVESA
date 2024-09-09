/* ********************************************************************** */
/*                                                                        */
/*   VIDEO.H                                                              */
/*                                                                        */
/* ********************************************************************** */

#ifndef __VIDEO_H__
#define __VIDEO_H__

/* -- HEADER FILES ------------------------------------------------------ */

#include "global.h"
#include "driver.h"
#include "memory.h"
#include "state.h"

/* -- TYPE DEFINITIONS -------------------------------------------------- */

struct VBEInfo {
	char Signature[4];
	unsigned char MinorVersion;
	unsigned char MajorVersion;
	char far *OEMString;
	unsigned long Capabilities;
	unsigned short far *Modes;
	unsigned short Memory;
	unsigned char Reserved[236];
};

struct VBEModeInfo {
	unsigned short ModeAttributes;
	unsigned char WindowAAttributes;
	unsigned char WindowBAttributes;
	unsigned short WindowGranularity;
	unsigned short WindowSize;
	unsigned short WindowASegment;
	unsigned short WindowBSegment;
	void (far *WindowFunction)(void);
	unsigned short BytesPerLine;
	unsigned short ResolutionX;
	unsigned short ResolutionY;
	unsigned char CharSizeX;
	unsigned char CharSizeY;
	unsigned char NumberOfPlanes;
	unsigned char BitsPerPixel;
	unsigned char NumberOfBanks;
	unsigned char MemoryModel;
	unsigned char BankSize;
	unsigned char NumberOfImagePages;
	unsigned char Reserved1;
	unsigned char RedMaskSize;
	unsigned char RedFieldPosition;
	unsigned char GreenMaskSize;
	unsigned char GreenFieldPosition;
	unsigned char BlueMaskSize;
	unsigned char BlueFieldPosition;
	unsigned char ReservedMaskSize;
	unsigned char ReservedFieldPosition;
	unsigned char DirectColorModeInfo;
	unsigned char Reserved2[216];
};

/* -- PUBLIC DATA DECLARATIONS ------------------------------------------ */

extern bool DualWindowSystem;
extern unsigned short BytesPerLine;

/* -- PUBLIC FUNCTION DECLARATIONS -------------------------------------- */

extern bool InitVideo(unsigned short resolutionX, unsigned short resolutionY);
extern void ShutdownVideo(void);
extern void Translate(short far *const x, short far *const y);
extern void PutPixel(unsigned short x, unsigned short y);
extern unsigned char far *AccessVideoMemory(int method,
	unsigned short x, unsigned short y);
extern unsigned char far *AccessFrameBuffer(unsigned short x,
	unsigned short y);
extern void NextVideoMemoryBlock(int method);
extern void NextFrameBufferBlock(void);

#endif

/* -- END --------------------------------------------------------------- */
