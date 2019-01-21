/*
 * Finch2SPISlave.h
 *
 * Created: 12/14/2018 11:33:26 AM
 *  Author: raghu
 */ 


#ifndef FINCH2SPISLAVE_H_
#define FINCH2SPISLAVE_H_


#define SLAVE_MUX_SETTING SPI_SIGNAL_MUX_SETTING_D
#define SLAVE_MISO  PINMUX_PA22D_SERCOM2_PAD0
#define SLAVE_SCK   PINMUX_PA23D_SERCOM2_PAD1
#define SLAVE_SS    PINMUX_PA24D_SERCOM2_PAD2
#define SLAVE_MOSI  PINMUX_PA25D_SERCOM2_PAD3


#define SPI_SLAVE_MUX_SETTING SLAVE_MUX_SETTING
#define SPI_SLAVE_PINMUX_PAD0 SLAVE_MISO
#define SPI_SLAVE_PINMUX_PAD1 SLAVE_SCK
#define SPI_SLAVE_PINMUX_PAD2 SLAVE_SS
#define SPI_SLAVE_PINMUX_PAD3 SLAVE_MOSI

#define SLAVE_SPI_MODULE  SERCOM2

extern volatile bool transfer_complete_spi_slave;
extern volatile uint8_t received_value[20];
extern volatile uint8_t sensor_outputs[20];
extern volatile uint8_t transmit_value[20];
extern volatile uint8_t temp_receive[20];

struct spi_module spi_slave_instance;

void spi_slave_init();
void check_buffer();
void disableSlaveSPI();


#endif /* FINCH2SPISLAVE_H_ */