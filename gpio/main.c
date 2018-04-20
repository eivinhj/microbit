#include <stdint.h>

#define __GPIO_BASE_ADDRESS__ 0x50000000

#define GPIO ((NRF_GPIO_REGS*)__GPIO_BASE_ADDRESS__)
#define __BUTTON_A_PIN__ 17
#define __BUTTON_B_PIN__ 26
#define __BUTTON_A_BIT_MASK__ 0x20000; // (1<<17)
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
	volatile uint32_t RESERVED1[120]; //__RESERVED_SIZE_!__ 0x700 / 4 in base 10 || 448 ELLER 120
	volatile uint32_t PIN_CNF[32];
} NRF_GPIO_REGS;


int main(){
	// Configure LED Matrix
	for(int i = 4; i <= 15; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}

	// Configure buttons
	GPIO->PIN_CNF[__BUTTON_A_PIN__] = 0;
	GPIO->PIN_CNF[__BUTTON_B_PIN__] = 0;

	int sleep = 0;

	uint32_t gpio_in = GPIO->IN;
	uint32_t btn_a_not_pressed = gpio_in & __BUTTON_A_BIT_MASK__ ;
	uint32_t btn_b_not_pressed = gpio_in & __BUTTON_B_BIT_MASK__;

	while(1){

		gpio_in = GPIO->IN;

		uint32_t btn_a_not_pressed_new = gpio_in & __BUTTON_A_BIT_MASK__ ;
		uint32_t btn_b_not_pressed_new = gpio_in & __BUTTON_B_BIT_MASK__;
		/* Check if button B is pressed;
		 * turn on LED matrix if it is. */
		if(!btn_b_not_pressed_new && btn_b_not_pressed)
		{
			uint32_t gpio_out = GPIO->OUT;
			gpio_out = gpio_out & ~(0xFFF0);
			gpio_out = gpio_out |  0xE000; 
			GPIO->OUT = gpio_out;
		} 

		/* Check if button A is pressed;
		 * turn off LED matrix if it is. */
		else if (!btn_a_not_pressed_new && btn_a_not_pressed)
		{
			uint32_t gpio_out = GPIO->OUT;
			gpio_out = gpio_out & ~(0xFFF0);
			gpio_out = gpio_out |  0x0000; 
			GPIO->OUT = gpio_out;
		} 

		
		btn_a_not_pressed = btn_a_not_pressed_new;
		btn_b_not_pressed = btn_b_not_pressed_new;
		sleep = 10000;
		while(--sleep);
	}
	return 0;
}
