/*
 * CFile1.c
 *
 * Created: 12/11/2018 4:41:08 PM
 *  Author: raghu
 */ 

#include <asf.h>
#include "Finch2AddLEDControl.h"

void fadeRED()
{
	volatile static colorInfo test_color;
	uint8_t i = 0;
	test_color.GBrightness = 0;
	test_color.RBrightness = 0;
	test_color.BBrightness = 0;
	for(i=0;i<255;i++)
	{
		test_color.RBrightness = i;
		test_color.GBrightness = i;
		test_color.BBrightness = i;
		RGBPI55SetLED(0,test_color);
		RGBPI55SetLED(1,test_color);
		RGBPI55SetLED(2,test_color);
		delay_ms(1);
	}
	
	for(i=255;i>0;i--)
	{
		test_color.RBrightness = i;
		test_color.GBrightness = i;
		test_color.BBrightness = i;
		RGBPI55SetLED(0,test_color);
		RGBPI55SetLED(1,test_color);
		RGBPI55SetLED(2,test_color);
		delay_ms(1);
	}
}

void fadeREDControlled()
{
	volatile static colorInfo testColor[3];
	uint8_t i,j,k =0;
	for(k=0;k<3;k++)
	{
		for(i=0;i<255;i++)
		{
			for(j=0;j<3;j++)
			{
				
				if(k==j)
				{
					testColor[j].RBrightness =	i;
					testColor[j].GBrightness =	i;
					testColor[j].BBrightness =	i;
				}
				else
				{
					testColor[j].RBrightness =	0;
					testColor[j].GBrightness =	0;
					testColor[j].BBrightness =	0;
				}
			}
			RGBPI55SetAllLED(testColor);
		}
	}
}

void testADDLED()
{
	fadeRED();
}
