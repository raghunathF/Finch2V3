/*
 * globalVariables.h
 *
 * Created: 12/10/2018 2:14:44 PM
 *  Author: raghu
 */ 


#ifndef GLOBALVARIABLES_H_
#define GLOBALVARIABLES_H_

#include <asf.h>

#define SPI_DATA_LENGTH 20

volatile uint16_t	left_encoder_ticks  = 0;
volatile uint16_t	right_encoder_ticks = 0;
volatile bool		transferDone		= false;
volatile uint8_t	DMASourceRegister[NOLEDS*NoBytesLED+2*STARTZEROS];
volatile bool		updateControlLoop	= false;

bool spi_reset_1 = false;


volatile uint8_t	sensor_outputs[20];
volatile uint16_t	gleftTicks = 0;
volatile uint16_t	grightTicks = 0;


volatile uint8_t	transmit_value[SPI_DATA_LENGTH];
volatile uint8_t	temp_receive[SPI_DATA_LENGTH];
volatile uint8_t	received_value[SPI_DATA_LENGTH];
volatile bool		transfer_complete_spi_slave = false;


volatile uint8_t	serial_timeout_count  = 0;
volatile bool		serial_timeout		= false;
volatile bool		transcation_start		= false;
volatile bool		powerButtonCheck		= false;

volatile bool		updateSpeedControlLoop = false;
volatile bool		velocityControlUpdate  = false;
volatile bool		refreshTicks           = false;


volatile bool       LEDSPIUpdate           = false;
volatile bool		LEDSPITimerUpdate      = false;
volatile bool       motorSPIUpdate         = false;
volatile bool       motorSPITimerUpdate    = false;

volatile int16_t kpTerm = 0;
volatile int16_t kiTerm = 0;
volatile int16_t kdTerm = 0;
volatile uint8_t kSpeed = 0;

#endif /* GLOBALVARIABLES_H_ */