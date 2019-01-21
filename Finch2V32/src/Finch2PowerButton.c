#include <asf.h>
#include "Finch2PowerButton.h"
#include "Finch2AddLEDControl.h"
#include "Finch2Ultrasound.h"
#include "Finch2SPISlave.h"


#define ON_CONDITION 1
#define OFF_CONDITION 2

struct tc_module tc_instance2;
extern volatile bool overflowFlag;
extern volatile bool powerButtonCheck;


void init_power_pin()
{
	//Power on enable
	struct port_config power_button_config;
	port_get_config_defaults(&power_button_config);
	power_button_config.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(POWER_ON_BTN_PIN,&power_button_config);
	
	//Power off read
	power_button_config.direction  = PORT_PIN_DIR_INPUT;
	power_button_config.input_pull = SYSTEM_PINMUX_PIN_PULL_DOWN;
	port_pin_set_config(POWER_OFF_BTN_PIN,&power_button_config);
	
}


void power_button_on()
{
	port_pin_set_output_level(POWER_ON_BTN_PIN, true);
}

void power_off()
{
	delay_ms(1000);
	port_pin_set_output_level(POWER_ON_BTN_PIN, false);	
}



void stop_all_actuators()
{
	//RGB LED off
	//Motors off
}

void system_power_off()
{
	//stop_all_actuators();
	power_off();
}

void sendColorLED(colorInfo* test ,uint8_t  r , uint8_t g, uint8_t b)
{
	
	uint8_t i =0;
	for(i=0;i<3;i++)
	{
		test[i].RBrightness =	r;
		test[i].GBrightness =	g;
		test[i].BBrightness =	b;
	}
	
}

void powerLEDIndication(uint8_t powerOnOff)
{
	static colorInfo testColor[3];
	
	switch( powerOnOff)
	{
		case ON_CONDITION:
			sendColorLED(testColor,0,150,0);
			break;
		case OFF_CONDITION:
			sendColorLED(testColor,150,0,0);
			break;
		default:
			break;
	}
	port_pin_set_output_level(PROFILE_PIN,true);
	RGBPI55SetAllLED(testColor);
	port_pin_set_output_level(PROFILE_PIN,false);
	delay_cycles_ms(500);
	
	sendColorLED(testColor,0,0,0);
	port_pin_set_output_level(PROFILE_PIN,true);
	RGBPI55SetAllLED(testColor);
	port_pin_set_output_level(PROFILE_PIN,false);
}

void check_power_off_button()
{
	//volatile uint32_t power_off_button   = 0;
	volatile static uint16_t count_power_button = 0;
	static bool power_button_value_off = false;
	volatile bool power_button_value = false;
	
	
	if(powerButtonCheck == true)
	{
		powerButtonCheck = false;
		//setup_ultrasound();
		power_button_value = port_pin_get_input_level(POWER_OFF_BTN_PIN);
		//volatile const uint32_t *input_port        = INPUT_REG_ADDRESS;
		//power_off_button = *input_port & MASK_POWER_OFF;
		//if(power_off_button == POWER_OFF_VALUE )
		//{
		if(power_button_value == true)
		{
			count_power_button = count_power_button + 1 ;
			if(count_power_button > POWER_OFF_LIMIT)
			{
				if(power_button_value_off == true)
				{
					//Switch off SPI 
					disableSlaveSPI();
					powerLEDIndication(OFF_CONDITION);
					system_power_off();
					count_power_button = 0;
				}
			}
		}
		else
		{
			count_power_button = 0;
			power_button_value_off = true;
		}
	}
	
}



void power_button_init()
{
	delay_cycles_ms(1000);
	init_power_pin();
	power_button_on();
	powerLEDIndication(ON_CONDITION);
	delay_cycles_ms(5000);
	//init timer for 80 msec
}