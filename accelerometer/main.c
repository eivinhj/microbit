#include <stdlib.h>
#include <stdio.h>

#include "twi.h"
#include "accel.h"
#include "ubit_led_matrix.h"
#include "uart.h"
#include "utility.h"


#define accel_adress 0x1D
#define WHO_AM_I_adress 0x0D

//UART-functions
ssize_t _write(int fd, const void *buf, size_t count);
ssize_t _read(int fd, void *buf, size_t count);

int main() 
{
	uart_init();
	twi_init();
	accel_init();

	/*uint8_t * data_buffer = (uint8_t *)malloc(3 * sizeof(int));
	
	twi_multi_read(accel_adress, WHO_AM_I_adress, 1, data_buffer);

	while(1) iprintf("I am %d", data_buffer[0]); */
	

	int * data_buffer = (int *)malloc(3 * sizeof(int));

	while(1)
	{
		accel_read_x_y_z(data_buffer);
		int x_acc = data_buffer[0];
		int y_acc = data_buffer[1];
		int z_acc = data_buffer[2];
	
		utility_print(&uart_send,"X: %6d, Y: %6d, Z: %6d\n\r", x_acc, y_acc, z_acc);
	}



	free(data_buffer);

	return 0;
}

//Implementation of UART-functions
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
