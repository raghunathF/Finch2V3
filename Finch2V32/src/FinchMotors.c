/*
 * FinchMotors.c
 *
 * Created: 12/10/2018 1:06:39 PM
 *  Author: raghu
 */ 
#include <asf.h>
#include "FinchMotors.h"

//Globals

//Modules
static struct tcc_module tcc_instance0;


/**************************************************************************************/

void config_timer_waveform()
{
	struct tcc_config config_tcc;
	tcc_get_config_defaults(&config_tcc, TCC0);
	struct port_config config_port_pin;
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	
	//
	config_tcc.counter.clock_prescaler = TCC_CLOCK_PRESCALER_DIV256;
	config_tcc.counter.period		   = 0x00FF;
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	
	
	//Wave Polarity
	config_tcc.compare.wave_polarity[LEFT_MOTOR_CLOCK_CH]		= TCC_WAVE_POLARITY_0;
	config_tcc.compare.wave_polarity[RIGHT_MOTOR_CLOCK_CH]		= TCC_WAVE_POLARITY_0;
	config_tcc.compare.wave_polarity[LEFT_MOTOR_ANTI_CH]		= TCC_WAVE_POLARITY_0;
	config_tcc.compare.wave_polarity[RIGHT_MOTOR_ANTI_CH]		= TCC_WAVE_POLARITY_0;
	
	//Right Motor
	config_tcc.compare.match[RIGHT_MOTOR_CLOCK_CH]			= 0x00;
	config_tcc.pins.enable_wave_out_pin[RIGHT_MOTOR_CLOCK_WO] = true;
	config_tcc.pins.wave_out_pin[RIGHT_MOTOR_CLOCK_WO]        = PIN_PA04F_TCC0_WO0; // this is no longer correct --- should be now
	config_tcc.pins.wave_out_pin_mux[RIGHT_MOTOR_CLOCK_WO]    = PINMUX_PA04F_TCC0_WO0; // this is no longer correct --- should be now
	
	config_tcc.compare.match[RIGHT_MOTOR_ANTI_CH]			= 0x00;
	config_tcc.pins.enable_wave_out_pin[RIGHT_MOTOR_ANTI_WO] = true;
	config_tcc.pins.wave_out_pin[RIGHT_MOTOR_ANTI_WO]        = PIN_PA05F_TCC0_WO1; // this is no longer correct --- should be now
	config_tcc.pins.wave_out_pin_mux[RIGHT_MOTOR_ANTI_WO]    = PINMUX_PA05F_TCC0_WO1; // this is no longer correct --- should be now
	
	//
	config_tcc.compare.match[LEFT_MOTOR_CLOCK_CH]				= 0x00;
	config_tcc.pins.enable_wave_out_pin[LEFT_MOTOR_CLOCK_WO]  = true;
	config_tcc.pins.wave_out_pin[LEFT_MOTOR_CLOCK_WO]         = PIN_PA10F_TCC0_WO2; //this is no longer correct--- should be now
	config_tcc.pins.wave_out_pin_mux[LEFT_MOTOR_CLOCK_WO]     = PINMUX_PA10F_TCC0_WO2; // this is no longer correct --- should be now
	
	config_tcc.compare.match[LEFT_MOTOR_ANTI_CH]				= 0x00;
	config_tcc.pins.enable_wave_out_pin[LEFT_MOTOR_ANTI_WO]  = true;
	config_tcc.pins.wave_out_pin[LEFT_MOTOR_ANTI_WO]         = PIN_PA11F_TCC0_WO3; //this is no longer correct--- should be now
	config_tcc.pins.wave_out_pin_mux[LEFT_MOTOR_ANTI_WO]     = PINMUX_PA11F_TCC0_WO3; // this is no longer correct --- should be now
	
	
	tcc_init(&tcc_instance0, TCC0, &config_tcc);
	tcc_enable(&tcc_instance0);
	
}

void set_motor_left(uint8_t dir, uint8_t speed)
{
	static uint8_t old_left_dir   = 0;
	static uint8_t old_left_speed = 0;
	if((old_left_dir != dir)||(old_left_speed != speed))
	{
		// need to configure PWM with opposite pins
		if(dir == 0)
		{
			tcc_set_compare_value(&tcc_instance0, LEFT_MOTOR_CLOCK_CH, speed);
			tcc_set_compare_value(&tcc_instance0, LEFT_MOTOR_ANTI_CH, 0x00);
		}
		else
		{
			tcc_set_compare_value(&tcc_instance0, LEFT_MOTOR_CLOCK_CH, 0x00);
			tcc_set_compare_value(&tcc_instance0, LEFT_MOTOR_ANTI_CH, speed);
		}
		
	}
	old_left_speed	= speed;
	old_left_dir	= dir;
}


void set_motor_right(uint8_t dir, uint8_t speed)
{
	static uint8_t old_right_dir = 0;
	static uint8_t old_right_speed = 0;
	if((old_right_dir != dir ) || (old_right_speed != speed))
	{
		// need to configure PWM with opposite pins
		if(dir == 0)
		{
			tcc_set_compare_value(&tcc_instance0, RIGHT_MOTOR_CLOCK_CH, speed);
			tcc_set_compare_value(&tcc_instance0, RIGHT_MOTOR_ANTI_CH, 0x00);
		}
		else
		{
			tcc_set_compare_value(&tcc_instance0, RIGHT_MOTOR_CLOCK_CH, 0x00);
			tcc_set_compare_value(&tcc_instance0, RIGHT_MOTOR_ANTI_CH, speed);
		}
	}
	old_right_speed     = speed;
	old_right_dir		= dir;
}

void turn_off_motors()
{
	// disable the chip to save power -
	set_motor_left(0,0);
	set_motor_right(0,0);
}


void configure_motors()
{
	config_timer_waveform();
}