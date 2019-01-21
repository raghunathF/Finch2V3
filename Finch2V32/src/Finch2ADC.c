/*
 * Finch2ADC.c
 *
 * Created: 12/26/2018 9:42:36 AM
 *  Author: raghu
 */ 

#include <asf.h>
#include "Finch2ADC.h"

/********************************************************************************************/
struct adc_module adc_instance;
uint16_t* adc_result = NULL;

/********************************************************************************************
Reference is VDDANA/1.48
Sampling time -- 48Mhz/16
Max value of ADC -- 255
For HUmmingBird Bit board 3.3V/1.48 = 2.29
/********************************************************************************************/
void configure_adc()
{
	struct adc_config conf_adc;
	adc_get_config_defaults(&conf_adc);
	adc_result = malloc(sizeof(uint16_t));

	conf_adc.reference			= ADC_REFCTRL_REFSEL_INTVCC0;
	conf_adc.clock_prescaler	= ADC_CLOCK_PRESCALER_DIV128;
	conf_adc.positive_input		= 0;
	conf_adc.negative_input		= ADC_NEGATIVE_INPUT_GND;
	conf_adc.resolution			= ADC_RESOLUTION_8BIT;
	conf_adc.left_adjust		= true;

	adc_init(&adc_instance, ADC, &conf_adc);
	adc_enable(&adc_instance);
}



uint16_t adcStartReadResult(const enum adc_positive_input analogPin)
{
	uint16_t temp = 0;
	adc_set_positive_input(&adc_instance, analogPin );
	adc_start_conversion(&adc_instance);
	while((adc_get_status(&adc_instance) & ADC_STATUS_RESULT_READY) != 1);
	adc_read(&adc_instance, adc_result);
	temp = *adc_result;
	return temp;
}


