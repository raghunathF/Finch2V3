/*
 * FinchControlLoop.c
 *
 * Created: 12/10/2018 1:59:01 PM
 *  Author: raghu
 */ 

#include <asf.h>
#include "FinchMotorControlLoop.h"
#include "FinchMotors.h"
#include "FinchEncoders.h"

//Defines
#define NO_TICKS_PER_CM			   7
#define MAX_SPEED_THRESHOLD        15*NO_TICKS_PER_CM
#define MAX_SPEED                 100
#define MINIMUM_SPEED_CONTROL     10
#define KP_CONTROL                1
#define KI_CONTROL                2500
#define KD_CONTROL                2
#define MIN_SPEED                 5

#define VELOCITY_KP_CONTROL       1500
#define VELOCITY_KI_CONTROL       10000
#define VELOCITY_KD_CONTROL       5

#define MAX_SPEED_VALUE           250

#define INITIAL_SPEED_CONTROLLER  10

#define LEFTMOTOR                 1
#define RIGHTMOTOR	              2


#define IDEAL_MODE               0
#define VELOCITY_CONTROL_MODE	 1
#define TICK_CONTROL_MODE        2

#define MAXTICKSPER100m	 200


//Globals
extern uint16_t left_encoder_ticks;
extern uint16_t right_encoder_ticks;
extern volatile bool updateControlLoop;
extern volatile uint16_t gleftTicks;
extern volatile uint16_t grightTicks;
extern volatile bool velocityControlUpdate;

extern volatile int16_t kpTerm;
extern volatile int16_t kiTerm;
extern volatile int16_t kdTerm;
extern volatile uint8_t kSpeed;



static uint8_t   motorControlStatus = IDEAL_MODE;
static uint8_t	 reqLeftSpeed;
static uint8_t	 reqRightSpeed;
static bool      leftDirection; 
static bool      rightDirection;
static uint16_t  reqLeftTicks;
static uint16_t  reqRightTicks;

uint16_t prevCount = 0;
extern bool refreshTicks;



static motor GLeftMotor  ;
static motor GRightMotor ;

static uint16_t left_total_req_ticks = 0;
static uint16_t right_total_req_ticks = 0;
static bool left_dir  = false;
static bool right_dir = false;

volatile uint8_t speed_storage[1000];

//
static uint16_t KPVelocityControl = VELOCITY_KP_CONTROL;
static uint16_t KIVelocityControl = VELOCITY_KI_CONTROL;
static uint16_t KDVelocityControl = VELOCITY_KD_CONTROL; 

static uint16_t kp_constant = KP_CONTROL;
static uint16_t ki_constant = KI_CONTROL;
static uint16_t kd_constant = KD_CONTROL;

/*
void control_loop_motors()
{
	volatile uint8_t speed = 0;
	
	static uint16_t k =0;
	uint16_t temp_speed = 0;
	
	static uint32_t sum_integral_left = 0;
	static uint16_t  prev_error_left   = 0;
	static bool rightMotion = false;
	static bool leftMotion = false;
	
	static uint32_t sum_integral_right = 0;
	static uint16_t  prev_error_right   = 0;
	
	uint16_t  current_error   = 0;
	volatile int16_t   diff_derivative = 0;
	static bool first_time = false;
	if(updateControlLoop == true)
	{
		updateControlLoop = false;
		if(first_time == false);
		{
			first_time = true ;
			prev_error_left  = abs(left_total_req_ticks - left_encoder_ticks) ;
			prev_error_right = abs(right_total_req_ticks - right_encoder_ticks) ;
		}
		//Left Motor
		if((left_encoder_ticks - left_total_req_ticks) >= 0)
		{
			set_motor_left(left_dir ,  0);
			sum_integral_left = 0;
			//port_pin_set_output_level(PROFILE_PIN_2,true);
			//delay_cycles_us(1);
			//port_pin_set_output_level(PROFILE_PIN_2,false);
			if(rightMotion == false )
			{
				rightMotion = true;
				gleftTicks = left_encoder_ticks;
			}
			
			//tc_disable_callback(&tc_encoder_capture, TC_CALLBACK_OVERFLOW);
		}
		else
		{
			//Proportional control
			// Integral
			current_error   = abs(left_total_req_ticks - left_encoder_ticks) ;
			sum_integral_left   +=  current_error;
			if(sum_integral_left > 100 * ki_constant)
			{
				sum_integral_left  =  100 * ki_constant;
			}
			diff_derivative =  (current_error - prev_error_left);
			temp_speed = current_error/kp_constant + sum_integral_left/ki_constant ;//+ diff_derivative/kd_constant ;
			if(temp_speed > 255)
			{
				speed = 255;
			}
			else
			{
				speed = temp_speed;
			}
			
			speed_storage[k] = sum_integral_left/KI_CONTROL ;
			//sum_integral_storage[k] = sum_integral_left/KI_CONTROL ;
			//diff_derivative_storage[k] = diff_derivative/KD_CONTROL ;
			//proportional_storage[k] = current_error/KP_CONTROL;
			
			k++;
			if(k == 1000)
			{
				k=0;
			}
			set_motor_left(left_dir ,  speed);
			prev_error_left  = current_error ;
		}
		
		//Right Motor
		if((right_encoder_ticks - right_total_req_ticks) >= 0)
		{
			set_motor_right(right_dir , 0 );
			sum_integral_right = 0;
			//port_pin_set_output_level(PROFILE_PIN,true);
			//delay_cycles_us(1);
			//port_pin_set_output_level(PROFILE_PIN,false);
			if(leftMotion == false)
			{
				leftMotion = true;
				grightTicks = right_encoder_ticks;
			}
			
			
			//delay_cycles_ms(1000);
		}
		else
		{
			//PID control
			current_error   = abs(right_total_req_ticks - right_encoder_ticks) ;
			sum_integral_right   +=  current_error;
			if(sum_integral_right > 255 * ki_constant)
			{
				sum_integral_right  =  100 * ki_constant;
			}
			diff_derivative =  (current_error - prev_error_right);
			temp_speed = current_error/kp_constant + sum_integral_right/ki_constant + diff_derivative/kd_constant ;
			if(temp_speed > 255)
			{
				speed = 255;
			}
			else
			{
				speed = temp_speed;
			}
			//speed = 15;
			set_motor_right(right_dir ,  speed);
		}
		
		if(((right_encoder_ticks - right_total_req_ticks) >= 0) && ((left_encoder_ticks - left_total_req_ticks) >= 0) )
		{
			//port_pin_set_output_level(PROFILE_PIN,true);
			//delay_cycles_us(1);
			//port_pin_set_output_level(PROFILE_PIN,false);
			set_motor_left(left_dir , 0 );
			set_motor_right(right_dir , 0 );
			rightMotion = false;
			leftMotion = false;
			tc_disable_callback(&tc_encoder_capture, TC_CALLBACK_OVERFLOW);
		}
	}
	
}



void calculate_initial_speed_left()
{
	if(left_total_req_ticks > MAX_SPEED_THRESHOLD)
	{
		set_motor_left(left_dir , MAX_SPEED );
	}
}


void calculate_initial_speed_right()
{
	if(right_total_req_ticks > MAX_SPEED_THRESHOLD)
	{
		set_motor_right(right_dir , MAX_SPEED );
	}
}

void ticks_left()
{
	//calculate_initial_speed_left();
	left_encoder_ticks = 0;
	//gleftTicks = 0;
	//tc_enable_callback(&tc_encoder_capture, TC_CALLBACK_OVERFLOW);
	
}

void ticks_right()
{
	//calculate_initial_speed_right();
	right_encoder_ticks = 0;
	//grightTicks = 0;
	//tc_enable_callback(&tc_encoder_capture, TC_CALLBACK_OVERFLOW);
}

*/

void update_PID_values(uint8_t* pid_values )
{
	/*
	kp_constant   = (pid_values[0] << 8) & 0xFF00 ;
	kp_constant  |=  pid_values[1];
	
	ki_constant   = (pid_values[2] << 8) & 0xFF00 ;
	ki_constant  |=  pid_values[3];
	
	kd_constant   = (pid_values[4] << 8) & 0xFF00 ;
	kd_constant  |=  pid_values[5];
	*/
	
	KPVelocityControl   = (pid_values[0] << 8) & 0xFF00 ;
	KPVelocityControl  |=  pid_values[1];
	
	KIVelocityControl   = (pid_values[2] << 8) & 0xFF00 ;
	KIVelocityControl  |=  pid_values[3];
	
	KDVelocityControl   = (pid_values[4] << 8) & 0xFF00 ;
	KDVelocityControl  |=  pid_values[5];

}


/*
void move_motor(bool left_direction , uint16_t left_ticks, bool right_direction , uint16_t right_ticks)
{
	if((left_ticks > 0) || (right_ticks > 0))
	{
		if(left_ticks > 0)
		{
			left_dir  = left_direction;
			left_total_req_ticks = left_ticks;
			ticks_left();
		}
		if(right_ticks > 0)
		{
			right_dir = right_direction;
			right_total_req_ticks = right_ticks;
			ticks_right();
		}
		tc_enable_callback(&tc_encoder_capture, TC_CALLBACK_OVERFLOW);
	}
}
*/

/*
uint8_t velocityControl(uint16_t recorded , uint16_t set , uint8_t LRMotor)
{
	uint8_t speed        = 0;
	uint8_t initialSpeed = 0;
	initialSpeed  = set *  INITIAL_SPEED_CONTROLLER;
	speed          = initialSpeed +  abs(recorded - set) * VELOCITY_KP_CONTROL;
	switch(LRMotor)
	{
		case LEFTMOTOR:
			set_motor_left(left_dir,speed)
			break;
		case RIGHTMOTOR:
			set_motor_right(right_dir,speed)
			break;
		default:
			break;
	}
	return speed;
}
*/

#define INITIALCONTROL 2


void velocityControlLeft(volatile uint16_t feedback , volatile uint16_t set)
{
	int currentError = 0;
	uint8_t speed = 0;
	static int sumOfErrors  = 0;
	static int prevError = 0;
	currentError = (set - feedback);
	sumOfErrors += currentError;
	if(sumOfErrors > 4000)
	{
		sumOfErrors = 4000;
	}
	kpTerm = currentError/KPVelocityControl;
	kiTerm = sumOfErrors/KIVelocityControl;
	//kdTerm = (currentError - prevError)/KDVelocityControl ;
	speed = ((currentError*1000)/KPVelocityControl) + ((sumOfErrors*100)/KIVelocityControl) ;//+ (currentError - prevError)/KDVelocityControl ;
	kSpeed = speed;
	if(speed > MAX_SPEED_VALUE)
	{
		speed = MAX_SPEED;
	}
	prevError = currentError;
	set_motor_left(left_dir , speed);
}



void setInitialSpeed(bool motor , uint16_t ticks)
{
	set_motor_left(left_dir , ticks/INITIALCONTROL);
}


void velocityControlLoop(motor* leftMotor , motor* rightMotor)
{
	uint16_t ticksPer100m    = 0;

	if((velocityControlUpdate == true) && (motorControlStatus == VELOCITY_CONTROL_MODE))
	{
		velocityControlUpdate = false;
		if(leftMotor->speed > 0)
		{
			ticksPer100m       =  left_encoder_ticks ;
			left_encoder_ticks =  0;
			gleftTicks         =  ticksPer100m;
			velocityControlLeft(ticksPer100m , leftMotor->speed);
			/*
			if((prevCount == 0) || (refreshTicks == true))
			{
				if(prevCount == 0)
				{
					//setInitialSpeed(LEFTMOTOR , leftMotor->speed);
					prevCount = 1; 
				}
				if(refreshTicks == true)
				{
					refreshTicks = false;
				}
				//prevCount = left_encoder_ticks;
				left_encoder_ticks = prevCount;
			}
			else
			{
				ticksPer100m   =  left_encoder_ticks - prevCount;
				gleftTicks     =  ticksPer100m;
				prevCount      =  left_encoder_ticks;
				velocityControlLeft(ticksPer100m , leftMotor->speed);
			}
			*/	
		}
		if(reqRightSpeed > 0)
		{
			
		}
	}
}


void motorControlLoop()
{
	velocityControlLoop(&GLeftMotor,&GRightMotor);
}


void velocityControlStart()
{
	prevCount = 0;
	left_encoder_ticks   = 0;
	right_encoder_ticks  = 0;
}


void motorSpeedControl(motor * leftMotor , motor * rightMotor)
{
	GLeftMotor.speed                =  ((leftMotor->speed)*MAXTICKSPER100m)/255;
	GLeftMotor.direction            =  leftMotor->direction;
	GLeftMotor.ticks                =  leftMotor->ticks;
	GRightMotor.speed               =  ((rightMotor->speed)*MAXTICKSPER100m)/255;
	GRightMotor.direction           =  rightMotor->direction;
	GRightMotor.ticks               =  rightMotor->ticks;
	velocityControlStart();
}


void controlMotors( motor* leftMotor , motor* rightMotor)
{
	if((leftMotor->ticks == 0) && (rightMotor->ticks == 0))
	{
		if((leftMotor->speed > 0 )|| (rightMotor->speed > 0))
		{
			motorControlStatus = VELOCITY_CONTROL_MODE;
			motorSpeedControl(&leftMotor , &rightMotor);
		}
	}
	else if((leftMotor->ticks == 0) && (rightMotor->ticks == 0) && (leftMotor->speed == 0) && (rightMotor->speed == 0))
	{
		turn_off_motors();
		motorControlStatus = IDEAL_MODE;
	}
}
	/*
	//Complete Velocity Control
	if((left == 0) && (right_encoder_tick_input == 0))
	{
		if((leftMotorSpeed  > 0) || (rightMotorSpeed > 0))
		{
			motorSpeedControl(leftMotorDirection,leftMotorSpeed , rightMotorDirection,rightMotorDirection);
		}
	}
	else if((left_encoder_tick_input > 0 ) || ((right_encoder_tick_input > 0 )))
	{
		//moveMotorTickSpeedControl();
	}
	*/
//}
