#include "pwm.h"
#include "gpiote.h"
#include "ppi.h"
#include "timer.h"
#include "accel.h"
#include "ubit_led_matrix.h"
#include "uart.h"
#include "utility.h"
#include "twi.h"

#include <stdlib.h>

#define accel_adress 0x1D

int main() 
{

	uart_init();
	twi_init();
	accel_init();
	ubit_led_matrix_init();
	int prescaler = 9;
	int period = 625; 
	int init_duty = 500; //47
	pwm_init(prescaler, period, init_duty);
	
	while(1)
	{
		int * data_buffer = (int *)malloc(3 * sizeof(int));
		accel_read_x_y_z(data_buffer);

		double accel_x = data_buffer[0];
		if(accel_x > 256) accel_x = 256;
		else if (accel_x < -256) accel_x =-256;
		int init_duty_new = 47 + accel_x/256 * 16; 
		pwm_set_ticks(init_duty_new);


		int x_acc = data_buffer[0];
		int y_acc = data_buffer[1];
		int z_acc = data_buffer[2];

		utility_print(&uart_send,"X: %6d, Y: %6d, Z: %6d\n\r", x_acc, y_acc, z_acc);
		int x_accel = data_buffer[0];
		int y_accel = data_buffer[1];
		int x_dot = x_accel / 50;
		int y_dot = - y_accel / 50;
		ubit_led_matrix_light_only_at(x_dot, y_dot);

		int sleep = 1000;
		while(sleep--);
		

	}
	return 0;
} 