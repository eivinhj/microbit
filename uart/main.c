#include "uart.h"
#include "gpio.h"
#include <stdint.h>


int main()
{
	// Configure LED Matrix
	for(int i = 4; i <= 15; i++){
		GPIO->DIRSET = (1 << i);
		GPIO->OUTCLR = (1 << i);
	}

	// Configure buttons
	GPIO->PIN_CNF[__BUTTON_A_PIN__] = 0;
	GPIO->PIN_CNF[__BUTTON_B_PIN__] = 0;

	uart_init();

	int sleep = 0;

	uint32_t gpio_in = GPIO->IN;
	uint32_t btn_a_not_pressed = gpio_in & __BUTTON_A_BIT_MASK__ ;
	uint32_t btn_b_not_pressed = gpio_in & __BUTTON_B_BIT_MASK__;

	while (1)
	{
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
			uart_send('B');
		} 

		/* Check if button A is pressed;
		 * turn off LED matrix if it is. */
		else if (!btn_a_not_pressed_new && btn_a_not_pressed)
		{
			uint32_t gpio_out = GPIO->OUT;
		gpio_out = gpio_out & ~(0xFFF0);
		gpio_out = gpio_out |  0x0000; 
		GPIO->OUT = gpio_out;
		uart_send('A');
		} 

		btn_a_not_pressed = btn_a_not_pressed_new;
		btn_b_not_pressed = btn_b_not_pressed_new;

		sleep = 10000;
		while(--sleep);
	}
}
