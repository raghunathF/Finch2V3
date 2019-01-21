/*
 * FinchControlLoop.h
 *
 * Created: 12/10/2018 1:59:23 PM
 *  Author: raghu
 */ 
#ifndef FINCHCONTROLLOOP_H_
#define FINCHCONTROLLOOP_H_

typedef struct {
	uint8_t speed;
	bool direction;
	uint16_t ticks;
}motor;

void controlMotors( motor* leftMotor , motor* rightMotor);
void move_motor(bool left_direction , uint16_t left_ticks, bool right_direction , uint16_t right_ticks);

void update_PID_values(uint8_t* pid_values );

void motorSpeedControl(motor * leftMotor , motor * rightMotor);

void velocityControlStart();
void motorControlLoop();


#endif /* FINCHCONTROLLOOP_H_ */