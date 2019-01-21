/*
 * Finch2LineSensor.c
 *
 * Created: 12/26/2018 9:23:58 AM
 *  Author: raghu
 */ 

#include <asf.h>
#include "Finch2lineSensor.h"
#include "Finch2ADC.h"

#define LEFT_LINE_ANALOG_CHANNEL    3
#define RIGHT_LINE_ANALOG_CHANNEL   6

uint8_t readLineSensors()
{
	uint8_t leftLineValue  = 0;
	uint8_t rightLineValue = 0;
	volatile uint8_t lineValue      = 0;
	//Switch on the IR LED
	//port_pin_set_output_level(LINE_LED, true);
	delay_cycles_us(1);
	leftLineValue   = port_pin_get_input_level(LEFT_LINE_PIN);
	rightLineValue  = port_pin_get_input_level(RIGHT_LINE_PIN);
	leftLineValue   = leftLineValue << 4;
	lineValue       = leftLineValue | rightLineValue;
	//port_pin_set_output_level(LINE_LED, false);
	return lineValue;
}

void initLineSensor()
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);

	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(LINE_LED, &config_port_pin);
	port_pin_set_output_level(LINE_LED, true);
	
	config_port_pin.direction = PORT_PIN_DIR_INPUT;
	port_pin_set_config(LEFT_LINE_PIN, &config_port_pin);
	port_pin_set_config(RIGHT_LINE_PIN, &config_port_pin);
	
}
