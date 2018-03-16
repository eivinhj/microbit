#include "uart.h"
#include "gpio.h"
#include <stdint.h>
#include <stdio.h>

ssize_t _write(int fd, const void *buf, size_t count);
ssize_t _read(int fd, void *buf, size_t count);
void set_led_matrix_on(void);
void set_led_matrix_off(void);
typedef enum tag_bool
{   FALSE = 0,
    TRUE = 1
} bool;
       



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
	
	bool led_matrix_on = TRUE;

	while (1)
	{
		gpio_in = GPIO->IN;

		uint32_t btn_a_not_pressed_new = gpio_in & __BUTTON_A_BIT_MASK__ ;
		uint32_t btn_b_not_pressed_new = gpio_in & __BUTTON_B_BIT_MASK__;
		/* Check if button B is pressed;
		 * turn on LED matrix if it is. */
		if(!btn_b_not_pressed_new && btn_b_not_pressed)
		{
			set_led_matrix_on();
			bool led_matrix_on = TRUE;   ff
			iprintf("Button B is pressed");
		} 

		/* Check if button A is pressed;
		 * turn off LED matrix if it is. */
		else if (!btn_a_not_pressed_new && btn_a_not_pressed)
		{
			set_led_matrix_off();
			bool led_matrix_on = FALSE;
		    iprintf("Button A is pressed");
		} 

		btn_a_not_pressed = btn_a_not_pressed_new;
		btn_b_not_pressed = btn_b_not_pressed_new;
		
		if(uart_read() != '\0')
		{
		    if(led_matrix_on == TRUE)
		    {
		        set_led_matrix_off();
		        led_matrix_on = FALSE;
		    }
		    else
		    {
		        set_led_matrix_on();
		        led_matrix_on = TRUE;
		    }
		}

		sleep = 10000;
		while(--sleep);
	}
}

ssize_t _write(int fd, const void *buf, size_t count){
char * letter = (char *)(buf);
for(int i = 0; i < count; i++){
uart_send(*letter);
letter++;
}
return count;
}

ssize_t _read(int fd, void *buf, size_t count){
char *str = (char *)(buf);
char letter;
do {
letter = uart_read();
} while(letter == '\0');
*str = letter;
return 1;
}

void set_led_matrix_on(void)
{
    uint32_t gpio_out = GPIO->OUT;
	gpio_out = gpio_out & ~(0xFFF0);
	gpio_out = gpio_out |  0xE000; 
	GPIO->OUT = gpio_out;
}
void set_led_matrix_off(void)
{
    uint32_t gpio_out = GPIO->OUT;
	gpio_out = gpio_out & ~(0xFFF0);
	gpio_out = gpio_out |  0x0000; 
	GPIO->OUT = gpio_out;
}
