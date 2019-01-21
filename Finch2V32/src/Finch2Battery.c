/*
 * Finch2Battery.c
 *
 * Created: 12/26/2018 2:54:55 PM
 *  Author: raghu
 */ 

#include <asf.h>
#include "Finch2Battery.h"
#include "Finch2ADC.h"

#define BATTERY_ANALOG_CHANNEL 0

uint8_t readBatteryValue()
{
	uint8_t batteryValue = 0;
	batteryValue = adcStartReadResult(BATTERY_ANALOG_CHANNEL);
	return batteryValue;
}