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
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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

	FILE *stream;
	
	int coord[130];
	int radarX=240;
		int radarY=50;
		double corner=0;
		
	

	// откроем файл с координатами и
	// отрисуем, до начала раоты с кнопками
start:

	if ((stream=fopen("file.txt","r"))==NULL)
	{
		printf("can't open file");
	} else {
		char buf[500];
		int i=0;
		char *pch;
		while(!feof(stream))
		{
			fgets(buf, 500, stream);
		}
		pch = strtok(buf,",");

		// получим список координат
		//в массиве coord[i]
		while (pch != NULL)
		{
			coord[i]=atoi(pch);
			//printf("%d - ", coord[i]);
			i++;
			pch = strtok(NULL, ",");
		}
		// получен массив координат из файла

		/* инициализаци€ переменных дл€ отрисовки линий на график */




label:

		// нарисуем полукруг с ос€ми
		// ось Y
		drawLineScreen(240, 271, 240, 0, RGB(100,100,100));
		// ось X
		drawLineScreen(60, 270, 420, 270, RGB(100,100,100));
		// полукруг равнй 150 см дистанции
		int j;
		drawCircle(240, 270, 150*1.5, RGB(50,50,50));
		drawCircle(240, 270, 100*1.5, RGB(50,50,50));
		drawCircle(240, 270, 50*1.5, RGB(50,50,50));
		drawCircle(240, 270, 10*1.5, RGB(50,50,50));
		drawFilledCircle(240, 270, 5, RGB(255,0,0));
		
		int k,x0,y0,x1,y1;
		x0=0; 	x1=20; 		y0=150;		y1=150;		k=0;
		int tmp=0;

		// нарисуем координаты на полуокружность
		double A, B;
		k=0;
		x0=240;
		y0=270;
		int x2,y2;
		x1=240-150*1.5;
		y1=270;

		double corn=0;	// угол поворота
		//printf("sinuses:");



		while(k<129)
				{
					// вычисл€ем ј
					A=sin(corn)*coord[k];
					// вычисл€ем B
					B=cos(corn)*coord[k];
					//printf("%f  | ", A);
					k++;
					corn=corn+0.024;
					//putPixelScreen(RGB(255,0,0), x0-(int) B*1.5, y0-(int) A*1.5);
					x2=x0-(int) B*1.5;
					y2=y0-(int) A*1.5;
					if (coord[k]>149 || y2>265) {
						//drawLineScreen(x1, y1, x2, y2, RGB(0,0,0));
						putPixelScreen(RGB(255,0,51), x0-(int) B*1.5, y0-(int) A*1.5);
					} else {
						drawLineScreen(x1, y1, x2, y2, RGB(255,0,51));
					}
					x1=x2;
					y1=y2;
				}
		// перерисуем линию
		
		drawLineScreen(240, 271, radarX, radarY, RGB(255,0,204));
		
		flipScreen();
		clearScreen(RGB(255,255,255));

		int bit=0;

		
		while(!done)
		{
			
			sceDisplayWaitVblankStart(); 
			sceCtrlReadBufferPositive(&pad, 1); 
			if (pad.Buttons != 0){
				if (pad.Buttons & PSP_CTRL_LEFT){
				// тащим линию влево
					// тоесть мен€ем координату x-5 например
						//corner=corner-0.024;
						//radarX=radarX-sin(corner);
						//if (radarX<240) {
						//	radarY=radarY+cos(corner);
						//} else {
						//	radarY=radarY-cos(corner);
						//}
					if(radarX-5<0) radarX=480;
					radarX=radarX-5;
					
						//printf("%d", radarX);
				}
				
				if (pad.Buttons & PSP_CTRL_RIGHT){
				// тащим вправо
	/*					corner=corner+0.024;
						radarX=radarX+sin(corner);
						if (radarX<240) {
							radarY=radarY-cos(corner);
						} else {
							radarY=radarY+cos(corner);
						}*/
					if (radarX+5>480){
						radarX=0;
					}
					radarX=radarX+5;
				}	 
				if (pad.Buttons & PSP_CTRL_UP){
					if(radarY-5<0) radarY=270;
					radarY=radarY-5;
				} 
				if (pad.Buttons & PSP_CTRL_DOWN){
					if(radarY+5>270) radarY=0;
					radarY=radarY+5;
				} 
				if (pad.Buttons & PSP_CTRL_TRIANGLE){
					// загрузка нового файла

				} 
			}
			
			goto label;

		}
		
		

		
		

       }

		
	sceKernelExitGame();
	return 0;

}

int atoi(char s[])
   {
       int i, n;

       n = 0;
       for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i)
           n = 10 * n + (s[i] - '0');
       return n;
   }

int ArraySize(int array[])
{
	printf("array size");
	int i = 0;
	while(array[i] != NULL) i++;
	return i;
}

