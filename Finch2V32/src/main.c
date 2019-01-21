/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to system_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "Finch2AddLED.h"
#include "Finch2SPISlave.h"
#include "Finch2SPISlaveControl.h"
#include "Finch2PowerButton.h"
#include "FinchMotorControlLoop.h"
#include "FinchMotors.h"
#include "FinchEncoders.h"
#include "Finch2Ultrasound.h"
#include "Finch2InterruptHandler.h"
#include "Finch2ReadSensors.h"
#include "Finch2InterruptHandler.h"
#include "Finch2ADC.h"
#include "Finch2lineSensor.h"
#include "test.h"
#include "globalVariables.h"


void switchOffMotors()
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(AIN1_PIN, &config_port_pin);
	port_pin_set_config(AIN2_PIN, &config_port_pin);
	port_pin_set_config(BIN1_PIN, &config_port_pin);
	port_pin_set_config(BIN2_PIN, &config_port_pin);
	port_pin_set_output_level(AIN1_PIN , false);
	port_pin_set_output_level(AIN2_PIN , false);
	port_pin_set_output_level(BIN1_PIN , false);
	port_pin_set_output_level(BIN2_PIN , false);
}

void configureEncoders()
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	port_pin_set_config(LEFT_ENC_PIN, &config_port_pin);
	port_pin_set_config(RIGHT_ENC_PIN, &config_port_pin);
}

void testInit()
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(PROFILE_PIN, &config_port_pin);
	port_pin_set_config(PROFILE_PIN_2, &config_port_pin);
	port_pin_set_output_level(PROFILE_PIN,false);
	port_pin_set_output_level(PROFILE_PIN_2,false);
}

int main (void)
{
	system_init();
	delay_init();
	RGBPI55Init();
	power_button_init();
	
	//Configure ADC
	//configure_adc();
	
	initInterruptHandler();
	testInit();
	
	//initLineSensor();
	configureUltrasound();
	
	//switchOffMotors();
	//configureEncoders();
	
	configure_motors();
	encoders_init();
	
	spi_slave_init();
	static motor leftMotor;
	static motor rightMotor;
	
	leftMotor.direction = 0;
	leftMotor.speed = 100;
	leftMotor.ticks = 0;
	
	rightMotor.direction = 0;
	rightMotor.speed = 0;
	rightMotor.ticks = 0;
	
	controlMotors( &leftMotor , &rightMotor);
	
	//move_motor(true , 12000 , true , 12000);
	//set_motor_left(0, 255);
	//set_motor_right(0, 255);
	
	while(1)
	{
		check_power_off_button();
		spi_main_loop();
		controlLoop();
		checkUltrasound();
		motorControlLoop();
		readSensors();
		//control_loop_motors();
		delay_cycles_us(300);
		//fadeREDControlled();	
	}	
	/* Insert application code here, after the board has been initialized. */
}
