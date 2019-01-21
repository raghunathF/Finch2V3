/*
 * FinchMotors.h
 *
 * Created: 12/10/2018 1:06:52 PM
 *  Author: raghu
 */ 


#ifndef FINCHMOTORS_H_
#define FINCHMOTORS_H_


#define LEFT_MOTOR_CLOCK_CH 2
#define LEFT_MOTOR_CLOCK_WO 2

#define LEFT_MOTOR_ANTI_CH 3
#define LEFT_MOTOR_ANTI_WO 3

#define RIGHT_MOTOR_CLOCK_CH 0
#define RIGHT_MOTOR_CLOCK_WO 0

#define RIGHT_MOTOR_ANTI_CH 1
#define RIGHT_MOTOR_ANTI_WO 1

void configure_motors();
void set_motor_left(uint8_t dir, uint8_t speed);
void set_motor_right(uint8_t dir, uint8_t speed);
void turn_off_motors();



#endif /* FINCHMOTORS_H_ */