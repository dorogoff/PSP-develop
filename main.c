/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * main.c - Basic Input demo -- reads from control pad and indicates button
 *          presses.
 *
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 * Copyright (c) 2005 James Forshaw <tyranid@gmail.com>
 * Copyright (c) 2005 John Kelley <ps2dev@kelley.ca>
 * Copyright (c) 2005 Donour Sizemore <donour@uchicago.edu>
 *
 * $Id: main.c 1095 2005-09-27 21:02:16Z jim $
 */
#include <malloc.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>
#include <psprtc.h>
#include <stdio.h>
#include "graphics.h"

/* Define the module info section */

PSP_MODULE_INFO("Radar Example", 0, 1, 1);

typedef unsigned char byte;
#define RGB(r,g,b) ((u32)((byte)(r)|((byte)(g) << 8)|((byte)(b) << 16)))

/* Define the main thread's attribute value (optional) */
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

/* Define printf, just to make typing easier */
#define printf	pspDebugScreenPrintf

void dump_threadstatus(void);

int done = 0;

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
	done = 1;
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();

	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread,
				     0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

int main(void)
{
	
	SetupCallbacks();
	pspDebugScreenInit();
	initGraphics();
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	SceCtrlData pad;

	int x=0, y=0;

	while(1) {

		sceDisplayWaitVblankStart(); 

		sceCtrlReadBufferPositive(&pad, 1); 

		if (pad.Buttons & PSP_CTRL_UP) {
			if (y-1 < 0){
				y=272;
				printf("y=0");
			}
			y -= 1;
		}

		if (pad.Buttons & PSP_CTRL_DOWN)
		{
			if (y+1 > 272){
				y=0;
				printf("y=0");
			}

				y += 1;
		}

		if (pad.Buttons & PSP_CTRL_LEFT) x -= 1;

		if (pad.Buttons & PSP_CTRL_RIGHT) x += 1;

		drawLineScreen(x + 10, y + 10, x + 20, y + 20, 

		RGB(255, 255, 255));

		drawLineScreen(x + 10, y + 20, x + 20, y + 10, 

		RGB(255, 255, 255));

		flipScreen();

		clearScreen(RGB(0, 0, 0));

		}

	sceKernelExitGame();
    return 0;

}