/*
 * SPISlaveControl.c
 *
 * Created: 12/14/2018 11:33:45 AM
 *  Author: raghu
 */ 


#include <asf.h>
#include "Finch2AddLEDControl.h"
#include "Finch2SPISlaveControl.h"
#include "Finch2SPISlave.h"
#include "FinchMotorControlLoop.h"
#include "FinchMotors.h"

/********************************************************************************************/
#define   APP
/********************************************************************************************/

#define PID_TUNING		0x10
#define LEDS			0xD0
#define MOTORS_NE		0xD1
#define MOTORS_E		0xD2
#define SETALL			0xDA
#define STOPALL			0xDB

static colorInfo LEDColor[3];
static motor leftMotor;
static motor rightMotor;


/********************************************************************************************
To ensure that there all the SPI commands that we are waiting for once started takes less than
16 msec to complete we used the timer interrupt of the LED to make it happen.If serial timeout occurred
then reset the SPI so that we have the right SPI bytes in the buffer during the start
********************************************************************************************/
void check_timeout()
{
	
	if(transcation_start == true)
	{
		if(serial_timeout == true)
		{
			serial_timeout_count = 0;
			serial_timeout = false;
			transcation_start = false;
			
			spi_reset(&spi_slave_instance);
			spi_slave_init();
			spi_transceive_buffer_job(&spi_slave_instance, sensor_outputs, received_value,SPI_LENGTH);
		}
	}
}

void readLED2Struct(colorInfo* LEDColor, uint8_t* inputValues)
{
	//1
	LEDColor[0].RBrightness = inputValues[0];
	LEDColor[0].GBrightness = inputValues[1];
	LEDColor[0].BBrightness = inputValues[2];
	
	//2
	LEDColor[1].RBrightness = inputValues[3];
	LEDColor[1].GBrightness = inputValues[4];
	LEDColor[1].BBrightness = inputValues[5];
	
	//3
	LEDColor[2].RBrightness = inputValues[6];
	LEDColor[2].GBrightness = inputValues[7];
	LEDColor[2].BBrightness = inputValues[8];
	
}

void packMotors(uint8_t* receive)
{
	//Left Motor
	leftMotor.speed                   =   receive[0] & 0x7F;
	leftMotor.direction               =  (receive[0] & 0x80) >>7;
	leftMotor.ticks                   =  (receive[1]<<8)&0xFF00;
	leftMotor.ticks                  |=  (receive[2]);
	
	//Right Motor
	rightMotor.speed                   =   receive[3] & 0x7F;
	rightMotor.direction               =  (receive[3] & 0x80) >>7;
	rightMotor.ticks                   =  (receive[4]<<8)&0xFF00;
	rightMotor.ticks                  |=  (receive[5]);	
}


void controlLoop()
{
	if((LEDSPIUpdate == true) && (LEDSPITimerUpdate == true))
	{
		LEDSPIUpdate         = false;
		LEDSPITimerUpdate = false;
		RGBPI55SetAllLED(LEDColor);
	}
	if((motorSPIUpdate == true) && (motorSPITimerUpdate == true))
	{
		LEDSPIUpdate           = false;
		motorSPITimerUpdate = false;
		controlMotors(&leftMotor, &rightMotor);
	}
}


void spi_main_loop()
{
	
	volatile enum status_code error_code = 0x10;
	volatile static uint16_t count_buffer = 0;
	

	//Left Motor
	uint8_t   leftMotorSpeed = 0;
	uint8_t   leftMotorDirection = 0;
	//Right Motor
	uint8_t   rightMotorSpeed = 0;
	uint8_t   rightMotorDirection = 0;
	//
	uint8_t i    = 0;
	uint8_t rw   = 0;
	uint8_t mode = 0;
	static bool test = true;
	transmit_value[0] = 0x88;
	transmit_value[1] = 0xAA;
	transmit_value[2] = 0xBB;
	transmit_value[3] = 0xCC;
	//check_timeout();
	if(transfer_complete_spi_slave == true)
	{
		check_buffer();
		mode = temp_receive[0];
		switch(mode)
		{
			case PID_TUNING:
				update_PID_values(temp_receive+1);
				break;
			
			//Addressable LEDS
			case LEDS:
			    //port_pin_set_output_level(PROFILE_PIN,true);
				readLED2Struct(LEDColor , temp_receive+1);
				//port_pin_set_output_level(PROFILE_PIN,false);
				LEDSPIUpdate = true;
				//RGBPI55SetAllLED(LEDColor);
				break;
			
			//Motors with encoders
			case MOTORS_E:
			    packMotors(temp_receive+1);
				motorSPIUpdate = true;
				//move_motor(temp_receive[1], left_encoder_tick_input , temp_receive[4] , right_encoder_tick_input);
				break;
				
			//Motors without encoders
			case MOTORS_NE:
			
				leftMotorDirection         = temp_receive[1]&0x01;
				leftMotorSpeed			   = temp_receive[2];
				
				rightMotorDirection        = temp_receive[3]&0x01;
				rightMotorSpeed            = temp_receive[4];
				
				set_motor_left(leftMotorDirection   , leftMotorSpeed);
				set_motor_right(rightMotorDirection , rightMotorSpeed);
				break;
				
			case SETALL:
				readLED2Struct(LEDColor , temp_receive+1);
				RGBPI55SetAllLED(LEDColor);
				//controlMotors(temp_receive+10);
				break;
			
			case STOPALL:
				//stop the encoder timer
				//stop the motors
				//clear the necessary variables
				//switch off LEDS
				clearLEDS();
				break;

			default:
				break;
		}	
		check_buffer();	//Look for more SPI commands if occurred , while completing one operation
	}
	
}