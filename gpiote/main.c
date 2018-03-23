#include "gpio.h"
#include "gpiote.h"
#include "ppi.h"

#define __BUTTON_A_PIN__ 17
#define __BUTTON_B_PIN__ 26

#define __LED_SRC_0__ 13
#define __LED_SRC_1__ 14
#define __LED_SRC_2__ 15

int main() 
{

	//Config LEDpins to ground
	for(int i = 4; i <= 15; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}

	GPIOTE->CONFIG[0] = (0b10 << 16) | (__BUTTON_A_PIN__ << 8) | 0b1;
	GPIOTE->CONFIG[1] = (0b1 << 20) | (0b11 << 16) | (__LED_SRC_0__ << 8) | 0b11;
	GPIOTE->CONFIG[2] = (0b11 << 16) | (__LED_SRC_1__ << 8) | 0b11;
	GPIOTE->CONFIG[3] = (0b11 << 16) | (__LED_SRC_2__ << 8) | 0b11;

	PPI->PPI_CH[0].EEP = (uint32_t)&(GPIOTE->IN[0]);
	PPI->PPI_CH[0].TEP = (uint32_t)&(GPIOTE->OUT[1]);	
	PPI->PPI_CH[1].EEP = (uint32_t)&(GPIOTE->IN[0]);
	PPI->PPI_CH[1].TEP = (uint32_t)&(GPIOTE->OUT[2]);	
	PPI->PPI_CH[2].EEP = (uint32_t)&(GPIOTE->IN[0]);
	PPI->PPI_CH[2].TEP = (uint32_t)&(GPIOTE->OUT[3]);

	PPI->CHENSET = 0b111;

	while (1) 
	{
		//PÅSKESUS

	}
	return 0;
}