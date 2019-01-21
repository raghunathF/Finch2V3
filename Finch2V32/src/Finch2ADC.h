/*
 * Finch2ADC.h
 *
 * Created: 12/26/2018 9:42:57 AM
 *  Author: raghu
 */ 


#ifndef FINCH2ADC_H_
#define FINCH2ADC_H_


void configure_adc();
uint16_t adcStartReadResult(const enum adc_positive_input analogPin); 



#endif /* FINCH2ADC_H_ */