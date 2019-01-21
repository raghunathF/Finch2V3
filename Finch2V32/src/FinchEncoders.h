/*
 * FinchEncoders.h
 *
 * Created: 12/10/2018 1:54:25 PM
 *  Author: raghu
 */ 


#ifndef FINCHENCODERS_H_
#define FINCHENCODERS_H_

struct  tc_module    tc_encoder_capture;

void encoders_init(void);
void configure_extint_left_encoder(void);
void configure_extint_right_encoder(void);
void configure_extint_left_encoder_callbacks(void);
void configure_extint_right_encoder_callbacks(void);
void encoder_timer_init(void);
void configure_timer_encoder(void);
void init_timer_callbacks(void);


#endif /* FINCHENCODERS_H_ */