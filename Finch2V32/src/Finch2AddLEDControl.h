/*
 * Finch2AddLEDControl.h
 *
 * Created: 12/11/2018 2:46:22 PM
 *  Author: raghu
 */ 


#ifndef FINCH2ADDLEDCONTROL_H_
#define FINCH2ADDLEDCONTROL_H_

typedef struct
{
	uint8_t RBrightness ;
	uint8_t GBrightness ;
	uint8_t BBrightness ;
}colorInfo;


void RGBPI55SetLED(uint8_t LEDNumber , colorInfo color_led );
void RGBPI55SetAllLED(colorInfo*  colorLED);
void clearLEDS();



#endif /* FINCH2ADDLEDCONTROL_H_ */