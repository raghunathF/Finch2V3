/*
 * Finch2InterruptHandler.c
 *
 * Created: 12/14/2018 4:12:27 PM
 *  Author: raghu
 */ 

#include <asf.h>
#define EIC_PRIORITY	0	
#define OTHER_PRIOROTY	1	

void initInterruptHandler()
{
	//Enable NVIC
	NVIC_EnableIRQ(EIC_IRQn);
	NVIC_EnableIRQ(DMAC_IRQn);
	NVIC_EnableIRQ(SERCOM0_IRQn);
	NVIC_EnableIRQ(SERCOM1_IRQn);
	NVIC_EnableIRQ(SERCOM2_IRQn);
	NVIC_EnableIRQ(TCC0_IRQn);
	NVIC_EnableIRQ(TC1_IRQn); 
	NVIC_EnableIRQ(TC2_IRQn);  
	
	NVIC_SetPriority(EIC_IRQn, EIC_PRIORITY );
	NVIC_SetPriority(DMAC_IRQn, OTHER_PRIOROTY);
	NVIC_SetPriority(SERCOM0_IRQn, OTHER_PRIOROTY);
	NVIC_SetPriority(SERCOM1_IRQn, OTHER_PRIOROTY);
	NVIC_SetPriority(SERCOM1_IRQn, OTHER_PRIOROTY);
	NVIC_SetPriority(TCC0_IRQn, OTHER_PRIOROTY);
	NVIC_SetPriority(TC1_IRQn,OTHER_PRIOROTY);
	NVIC_SetPriority(TC1_IRQn, OTHER_PRIOROTY);
	//Change the priority levels
}