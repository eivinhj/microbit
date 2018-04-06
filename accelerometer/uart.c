#include <stdint.h>
#include "uart.h"
#include "gpio.h"

#define TGT_RXD_PIN 25
#define TGT_TXD_PIN 24
#define TGT_RXD_MASK 0x1000000
#define TGT_TXD_MASK 0x800000


#define UART ((NRF_UART_REG*)0x40002000)
typedef struct {
volatile uint32_t STARTRX;
volatile uint32_t STOPRX;
volatile uint32_t STARTTX;
volatile uint32_t STOPTX;
volatile uint32_t RESERVED_0[3];
volatile uint32_t SUSPEND;
volatile uint32_t RESERVED_1[56];
volatile uint32_t CTS;
volatile uint32_t NCTS;
volatile uint32_t RXDRDY;
volatile uint32_t RESERVED_10[4];
volatile uint32_t TXDRDY;
volatile uint32_t RESERVED_2[1];
volatile uint32_t ERROR;
volatile uint32_t RESERVED_3[7];
volatile uint32_t RXTO;
volatile uint32_t RESERVED_4[110];
volatile uint32_t INTEN;
volatile uint32_t INTENSET;
volatile uint32_t INTENCLR;
volatile uint32_t RESERVED_5[93];
volatile uint32_t ERRORSRC;
volatile uint32_t RESERVED_6[31];
volatile uint32_t ENABLE;
volatile uint32_t RESERVED_7[1];
volatile uint32_t PSELRTS;
volatile uint32_t PSELTXD;
volatile uint32_t PSELCTS;
volatile uint32_t PSELRXD;
volatile uint32_t RXD;
volatile uint32_t TXD;
volatile uint32_t RESERVED_8[1];
volatile uint32_t BAUDRATE;
volatile uint32_t RESERVED_9[17];
volatile uint32_t CONFIG;
} NRF_UART_REG;


void uart_init()
{
	GPIO->PIN_CNF[TGT_TXD_PIN] = 1;
	GPIO->PIN_CNF[TGT_RXD_PIN] = 0;

	UART->PSELTXD = TGT_TXD_PIN;
	UART->PSELRXD = TGT_RXD_PIN;
	UART->PSELRTS = 0xFFFFFFFF;
	UART->PSELCTS = 0xFFFFFFFF;

	UART->BAUDRATE = 0x00275000;

	UART->ENABLE = 4;
	UART->STARTRX = 1;
}

void uart_send(char letter)
{
	UART->STARTTX = 1;


	UART->TXDRDY = 0;
	UART->TXD = letter;
	
	int sleep = 0;
	while(!(UART->TXDRDY))
	{
		sleep = 10000;
		while(--sleep);
	}
	
	UART->STOPTX = 1;
	//UART->STARTRX = 1;
	return;

}

char uart_read()
{
	if (UART->RXDRDY) 
	{
		UART->RXDRDY = 0;
		char input = UART->RXD;
		return input;
	}
	else
	{
		return '\0';
	}
	return '\0';
}
