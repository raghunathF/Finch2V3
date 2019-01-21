/*
 * Finch2ReadSensors.c
 *
 * Created: 12/17/2018 11:49:36 AM
 *  Author: raghu
 */ 

#include <asf.h>
#include "Finch2ReadSensors.h"
#include "Finch2lineSensor.h"
#include "Finch2Ultrasound.h"
#include "Finch2Battery.h"

#define SENSORCOUNTTHRESHOLD	10
#define NOISETHRESHOLD			10
#define THRESHOLDMISSMAX		3


extern volatile int16_t kpTerm;
extern volatile int16_t kiTerm;
extern volatile int16_t kdTerm;
extern volatile uint8_t kSpeed;



extern volatile uint16_t gleftTicks;
extern volatile uint16_t grightTicks;

void filterUltrasound(uint16_t inputValue )
{
	static uint16_t filteredValue = 0;  
	static uint8_t  thresholdMissCount = 0;
	static bool		firstTime = false;
	if(firstTime == true)
	{
		filteredValue = inputValue ;
		firstTime     = false;
	}
	
	else
	{
		if(abs(inputValue - filteredValue) < NOISETHRESHOLD)
		{
			filteredValue =  (filteredValue*9  +  inputValue)/10;
			thresholdMissCount = 0;
		}
		else
		{
			thresholdMissCount++;
			if(thresholdMissCount > THRESHOLDMISSMAX)
			{
				thresholdMissCount = 0;
			}
		}
	}
	
	
}

void readSensors()
{
	uint16_t ultrasoundValue =0;
	uint8_t  lineSensorValue = 0;
	uint8_t  batteryVoltage = 0;
	static uint8_t sensorUpdateCount	= 0;
	
	ultrasoundValue = getUltrasoundDistance();
	filterUltrasound(ultrasoundValue);
	
	
	sensorUpdateCount++;
	
	if(sensorUpdateCount > SENSORCOUNTTHRESHOLD)
	{
		
		
		
		//lineSensorValue   = readLineSensors();
		//batteryVoltage    = readBatteryValue();
		
		sensorUpdateCount = 0;
		//sensor_outputs[2] = (ultrasoundValue & 0xFF00)>>8;
		//sensor_outputs[3] = ultrasoundValue & 0x00FF;
		
		//sensor_outputs[4] = lineSensorValue;
		
		//left ticks
		//kpTerm = kpTerm * 10;
		//kdTerm = kdTerm * 10;
		//kiTerm = kiTerm * 10;
		
		// 
		sensor_outputs[4] = (kpTerm  & 0xFF00) >>8;
		sensor_outputs[5] =  kpTerm  &  0x00FF;
		
		//sensor_outputs[4] = kSpeed; 
		
		sensor_outputs[6] = (kiTerm  & 0xFF00) >>8;
		sensor_outputs[7] =  kiTerm   &  0x00FF;
		
		sensor_outputs[8] = (kdTerm  & 0xFF00) >>8;
		sensor_outputs[9] =  kdTerm   &  0x00FF;
		
		
		sensor_outputs[2] = (gleftTicks  & 0xFF00) >>8;
		sensor_outputs[3] =  gleftTicks   &  0x00FF;


		/*
		//right ticks
		sensor_outputs[7] = (grightTicks & 0xFF00) >>8;
		sensor_outputs[8] =  grightTicks & 0x00FF;
		*/
		
		//sensor_outputs[9] =  batteryVoltage;
		
	}
}