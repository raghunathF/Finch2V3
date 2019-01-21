/*
 * Finch2SPISave.c
 *
 * Created: 12/14/2018 11:33:11 AM
 *  Author: raghu
 */ 


#include <asf.h>
#include "Finch2SPISlave.h"

/********************************************************************************************/
#define SPI_LENGTH 10
#define LENGTH_SET_ALL 16
#define LENGTH_SINGLE   10

#define SETALL_COMMAND   0xDA

#define DEVICE_ID_HARDWARE_FIRMWARE		0x11

/********************************************************************************************/
volatile uint8_t ring_buffer[200];
volatile uint8_t tail_pointer=0;
volatile uint8_t head_pointer=0;
extern volatile uint8_t serial_timeout_count;
extern volatile bool spi_reset_1 ;

/********************************************************************************************/

/********************************************************************************************
SPI transfer Mode 0
Pre-load is important it helps during a reset to have the sensor 0 and sensor 1 value loaded
********************************************************************************************/
void configure_spi_slave(void)
{
	struct spi_config config_spi_slave;
	/* Configure, initialize and enable SERCOM SPI module */
	spi_get_config_defaults(&config_spi_slave);
	config_spi_slave.transfer_mode = SPI_TRANSFER_MODE_0;
	config_spi_slave.mode = SPI_MODE_SLAVE;
	config_spi_slave.mode_specific.slave.preload_enable = true;
	config_spi_slave.mode_specific.slave.frame_format = SPI_FRAME_FORMAT_SPI_FRAME;
	config_spi_slave.mux_setting = SPI_SLAVE_MUX_SETTING;
	config_spi_slave.pinmux_pad0 = SPI_SLAVE_PINMUX_PAD0;
	config_spi_slave.pinmux_pad1 = SPI_SLAVE_PINMUX_PAD1;
	config_spi_slave.pinmux_pad2 = SPI_SLAVE_PINMUX_PAD2;
	config_spi_slave.pinmux_pad3 = SPI_SLAVE_PINMUX_PAD3;
	config_spi_slave.receiver_enable  = true;
	spi_init(&spi_slave_instance,  SLAVE_SPI_MODULE, &config_spi_slave);
	spi_enable(&spi_slave_instance);
}


/********************************************************************************************
Checks the ring buffer if there any new data in the buffer if it is present it will be
transferred into the temp_receive global variable and we also look into the length of commands
********************************************************************************************/
void check_buffer()
{
	uint8_t i =0;
	if(tail_pointer == head_pointer)
	{
		transfer_complete_spi_slave = false;
		tail_pointer = 0;
		head_pointer = 0;
	}
	else
	{
		//Transfer receive
		if(ring_buffer[tail_pointer] == SETALL_COMMAND )
		{
			for(i=0; i<LENGTH_SET_ALL;i++)
			{
				temp_receive[i] = ring_buffer[tail_pointer];
				tail_pointer++;
			}
		}
		else
		{
			for(i=0; i<LENGTH_SINGLE;i++)
			{
				temp_receive[i] = ring_buffer[tail_pointer];
				tail_pointer++;
			}
		}
	}

}


static void spi_slave_callback(struct spi_module *const module)
{
	uint8_t i = 0;
	transfer_complete_spi_slave = true;
	serial_timeout_count = 0;
	//port_pin_set_output_level(PROFILE_PIN_2,true);
	if(spi_reset_1 == true )
	{
		//port_pin_set_output_level(PROFILE_PIN_2,true);
		spi_reset_1 = false;
		spi_reset(&spi_slave_instance);
		spi_slave_init();
		spi_transceive_buffer_job(&spi_slave_instance, sensor_outputs, received_value,SPI_LENGTH);
		//port_pin_set_output_level(PROFILE_PIN_2,false);
	}
	else
	{
		if(received_value[0] == SETALL_COMMAND )
		{ 
			for(i=0; i<LENGTH_SET_ALL;i++)
			{
				ring_buffer[head_pointer] = received_value[i];
				head_pointer++;
			}
		}

		else
		{
			for(i=0 ; i<LENGTH_SINGLE;i++)
			{
				ring_buffer[head_pointer] = received_value[i];
				head_pointer++;
			}
		}
		spi_transceive_buffer_job(&spi_slave_instance, sensor_outputs, received_value,SPI_LENGTH);
	}
	//port_pin_set_output_level(PROFILE_PIN_2,false);
}



void configure_spi_slave_callbacks(void)
{
	spi_register_callback(&spi_slave_instance, spi_slave_callback,SPI_CALLBACK_BUFFER_TRANSCEIVED);
	spi_enable_callback(&spi_slave_instance, SPI_CALLBACK_BUFFER_TRANSCEIVED);
}

void disableSlaveSPI()
{
	spi_disable(&spi_slave_instance);
}


void spi_slave_init()
{
	volatile enum status_code error_code = 0x10;
	static bool init = false;
	const uint8_t firmware_version[10] = {DEVICE_ID_HARDWARE_FIRMWARE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	sensor_outputs[0]  = DEVICE_ID_HARDWARE_FIRMWARE;
	//Initial values are the firmware version
	configure_spi_slave();
	configure_spi_slave_callbacks();
	if(init == false)
	{
		init = true;
		do
		{
			error_code = spi_transceive_buffer_job(&spi_slave_instance, sensor_outputs, received_value,SPI_LENGTH);
		} while (error_code != STATUS_OK );
	}
}
