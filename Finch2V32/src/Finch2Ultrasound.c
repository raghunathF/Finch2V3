/*
 * Finch2Ultrasound.c
 *
 * Created: 12/14/2018 11:18:13 AM
 *  Author: raghu
 */ 

#include <asf.h>
#include "Finch2Ultrasound.h"


//
#define US_ECHO_PIN_EIC  PIN_PA06A_EIC_EXTINT6
#define US_ECHO_PIN_MUX  PINMUX_PA06A_EIC_EXTINT6
#define US_ECHO_EIC_LINE 6


//

extern volatile bool powerButtonCheck;
volatile static uint32_t distance_16b=0;
static bool readyDistance = false;
struct tc_module tc_instance2;


static bool USInterruptStatus = false;
static bool disableUltrasoundInterrupt = false;
static bool ultrasoundUpdateCheck = false;

//300 msec
void configure_tc2()
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	config_tc.counter_size		= TC_COUNTER_SIZE_16BIT; //16
	config_tc.clock_source		= GCLK_GENERATOR_0;
	config_tc.clock_prescaler	= TC_CLOCK_PRESCALER_DIV256; 
	config_tc.count_direction	= TC_COUNT_DIRECTION_UP;
	tc_init(&tc_instance2, TC2, &config_tc);
	tc_enable(&tc_instance2);
}


void configure_echo_extint()
{
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults(&config_extint_chan);
	config_extint_chan.gpio_pin           = US_ECHO_PIN_EIC;
	config_extint_chan.gpio_pin_mux       = US_ECHO_PIN_MUX;
	config_extint_chan.gpio_pin_pull      = EXTINT_PULL_DOWN;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(US_ECHO_EIC_LINE, &config_extint_chan);
}

void setup_ultrasound()
{
	//Trigger High-Low 100us
	//extint_chan_enable_callback(US_ECHO_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	/*
	if(USInterruptStatus == false)
	{
		USInterruptStatus = true ;
 		disableUltrasoundInterrupt = false;
		//Clear the ultrasound interrupts
		extint_chan_enable_callback(US_ECHO_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
		
	}
	*/
	extint_chan_enable_callback(US_ECHO_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	port_pin_set_output_level(TRIG_PIN, true);
	delay_cycles_us(10);
	port_pin_set_output_level(TRIG_PIN, false);
	//Enable the interrupt
	//extint_chan_enable_callback(US_ECHO_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	//Initialize the counter
	tc_set_count_value(&tc_instance2, 0);
}

void TC2_OV_callback(struct tc_module *const module_inst)
{
	//Power button check
	ultrasoundUpdateCheck = true;
	powerButtonCheck = true;
}

void configurePins()
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);

	// Set trigger pin to output, initialize to trigger pin to zero
	config_port_pin.direction = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(TRIG_PIN, &config_port_pin);
	port_pin_set_output_level(TRIG_PIN, false);
	
	//echo pin to input
	//config_port_pin.direction = PORT_PIN_DIR_INPUT;
	//port_pin_set_config(ECHO_PIN, &config_port_pin);
}

void new_echo_detection_callback(void)
{
	
	static uint16_t rising_edge = 0;
	//serial_timeout_count = serial_timeout_count + 1;
	
	bool pin_state = port_pin_get_input_level(ECHO_PIN);
	if(pin_state)
	{
		rising_edge = tc_get_count_value(&tc_instance2);
	}
	else
	{
		distance_16b = tc_get_count_value(&tc_instance2) - rising_edge;
		//disable the interrupt
		//disableUltrasoundInterrupt = true;
		//port_pin_set_output_level(PROFILE_PIN_2, true);
		//delay_cycles_us(1);
		extint_chan_disable_callback(US_ECHO_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
		//port_pin_set_output_level(PROFILE_PIN_2, false);
		readyDistance = true;
	}
	
}

uint16_t getUltrasoundDistance()
{
	static uint16_t distance_ultrasound = 0 ;
	if(readyDistance)
	{
		readyDistance		= false;
		distance_ultrasound	= distance_16b;
	}
	return distance_ultrasound;
}

void configure_TC2_callback()
{
	tc_register_callback(&tc_instance2, TC2_OV_callback,TC_CALLBACK_OVERFLOW);
	tc_enable_callback(&tc_instance2, TC_CALLBACK_OVERFLOW);
}

void checkUltrasound()
{
	/*
	if(disableUltrasoundInterrupt == true)
	{
		disableUltrasoundInterrupt = false;
		USInterruptStatus = false;
		//port_pin_set_output_level(PROFILE_PIN_2, true);
		//delay_cycles_us(5);
		extint_chan_disable_callback(US_ECHO_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
		//port_pin_set_output_level(PROFILE_PIN_2, false);
	}
	*/
	if(ultrasoundUpdateCheck == true)
	{
		ultrasoundUpdateCheck = false;
		setup_ultrasound();
	}
	
	//setup_ultrasound();
	
}

void configure_ext_callback()
{
	extint_register_callback(new_echo_detection_callback,US_ECHO_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(US_ECHO_EIC_LINE,EXTINT_CALLBACK_TYPE_DETECT);
}

void configureModules()
{
	configure_tc2();
	configure_echo_extint();
}

void configureCallbacks()
{
	configure_TC2_callback();
	configure_ext_callback();
}

void configureUltrasound()
{
	configurePins();
	configureModules();
	configureCallbacks();
}