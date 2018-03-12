#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>

#define GPIO ((NRF_GPIO_REG*)0x50000000)
#define __BUTTON_A_PIN__ 17
#define __BUTTON_B_PIN__ 26
#define __BUTTON_A_BIT_MASK__ 0x20000;
#define __BUTTON_B_BIT_MASK__ 0x4000000;

typedef struct {
	volatile uint32_t RESERVED0[321];
	volatile uint32_t OUT;
	volatile uint32_t OUTSET;
	volatile uint32_t OUTCLR;
	volatile uint32_t IN;
	volatile uint32_t DIR;
	volatile uint32_t DIRSET;
	volatile uint32_t DIRCLR;
	volatile uint32_t RESERVED1[120];
	volatile uint32_t PIN_CNF[32];
} NRF_GPIO_REG;

#endif
