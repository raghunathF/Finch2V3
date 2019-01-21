/*
 * Finch2SPISlaveControl.h
 *
 * Created: 12/14/2018 11:34:24 AM
 *  Author: raghu
 */ 


#ifndef FINCH2SPISLAVECONTROL_H_
#define FINCH2SPISLAVECONTROL_H_

extern volatile bool transcation_start;
extern volatile bool serial_timeout;

#define SPI_LENGTH 7

extern volatile uint8_t serial_timeout_count;
extern volatile bool status_battery ;
extern volatile bool init_status_battery ;
extern volatile bool LEDSPIUpdate;
extern volatile bool motorSPIUpdate;
extern volatile bool LEDSPITimerUpdate;
extern volatile bool motorSPITimerUpdate;



void spi_main_loop();
void controlLoop();



#endif /* FINCH2SPISLAVECONTROL_H_ */