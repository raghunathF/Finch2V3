/*
 * FinchEncoders.c
 *
 * Created: 12/10/2018 1:54:09 PM
 *  Author: raghu
 */ 

#include <asf.h> 
#include "FinchEncoders.h"
#include "FinchControlLoop.h"


#define ENCODER_LEFT_EIC_LINE			0
#define ENCODER_RIGHT_EIC_LINE			1
#define ENCODER_TC  					TC1
#define SPEED_CONTROL_TIME_THRESHOLD	12

//globals
extern uint16_t left_encoder_ticks;
extern uint16_t right_encoder_ticks;
extern volatile bool updateControlLoop;
extern volatile bool speedControl;
extern volatile bool refreshTicks;
extern volatile bool updateSpeedControlLoop;
extern volatile bool velocityControlUpdate;


extern volatile bool LEDSPITimerUpdate;
extern volatile bool motorSPITimerUpdate;


extern volatile bool updateSensorControlLoop;

void tc_callback_OF(struct tc_module *const module_inst)
{
	//Each loop is 8msec
	//port_pin_set_output_level(PROFILE_PIN,true);
	//control_loop_motors();
	static uint8_t countThreshold = 0;
	countThreshold++; 
	
	LEDSPITimerUpdate = true;
	motorSPITimerUpdate = true;
	
	//updateSensorControlLoop = true;
	if ( countThreshold > SPEED_CONTROL_TIME_THRESHOLD)
	{
		velocityControlUpdate  = true;
		//updateSpeedControlLoop = true;
		countThreshold = 0;
	}
	//port_pin_set_output_level(PROFILE_PIN,false);
}


//Registering the callback
void init_timer_callbacks()
{
	tc_register_callback(&tc_encoder_capture, tc_callback_OF,TC_CALLBACK_OVERFLOW);
	tc_enable_callback(&tc_encoder_capture, TC_CALLBACK_OVERFLOW);
}

//Timer for encoder operations
//10 msec
void configure_timer_encoder()
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	struct tc_events config_events;
	
	config_tc.enable_capture_on_channel[0] = true;
	config_tc.counter_size = TC_COUNTER_SIZE_16BIT; //8
	config_tc.clock_source = GCLK_GENERATOR_0;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV8; //1024
	//config_tc.counter_8_bit.period = 0xFF;
	tc_init(&tc_encoder_capture, ENCODER_TC, &config_tc);
	tc_enable(&tc_encoder_capture);
}

//Left encoder external interrupt initialization
void configure_extint_left_encoder()
{

	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin           = PIN_PA16A_EIC_EXTINT0;
	config_extint_chan.gpio_pin_mux       = MUX_PA16A_EIC_EXTINT0;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;
	extint_chan_set_config(ENCODER_LEFT_EIC_LINE, &config_extint_chan);
}

//Right encoder external interrupt initialization
void configure_extint_right_encoder()
{
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin           = PIN_PA15A_EIC_EXTINT1;
	config_extint_chan.gpio_pin_mux       = MUX_PA15A_EIC_EXTINT1;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;
	extint_chan_set_config(ENCODER_RIGHT_EIC_LINE, &config_extint_chan);
}


//External
void extint_detection_left_encoder_callback(void)
{
	volatile static uint16_t count_high = 0;
	volatile static uint16_t count_low  = 0;
	port_pin_set_output_level(PROFILE_PIN_2,true);
	left_encoder_ticks++;
	if(left_encoder_ticks >32000)
	{
		refreshTicks = true;
		left_encoder_ticks = 0;
	}
	port_pin_set_output_level(PROFILE_PIN_2,false);
}

void extint_detection_right_encoder_callback(void)
{
	volatile static uint16_t count_high = 0;
	volatile static uint16_t count_low  = 0;
	//port_pin_set_output_level(PROFILE_PIN,true);
	right_encoder_ticks++;
	if(right_encoder_ticks >32000)
	{
		right_encoder_ticks = 0;
	}
	//port_pin_set_output_level(PROFILE_PIN,false);
}

void configure_extint_right_encoder_callbacks(void)
{
	extint_register_callback(extint_detection_right_encoder_callback,ENCODER_RIGHT_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(ENCODER_RIGHT_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
}


void configure_extint_left_encoder_callbacks(void)
{
	extint_register_callback(extint_detection_left_encoder_callback,ENCODER_LEFT_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(ENCODER_LEFT_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
}

void encoder_timer_init()
{
	configure_timer_encoder();
	init_timer_callbacks();
}

//Initializing the encoders
void encoders_init()
{
	
	//Initialize the external interrupt channel for both encoders
	configure_extint_left_encoder();
	configure_extint_right_encoder();
	
	//Initialize the callbacks for external interrupt channel for both encoders
	configure_extint_left_encoder_callbacks();
	configure_extint_right_encoder_callbacks();
	
	//Initialize the timer encoder
	encoder_timer_init();
	
}