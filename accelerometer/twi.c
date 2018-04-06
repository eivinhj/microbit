#include "twi.h"
#include "gpio.h"

#define SDA_PIN 30
#define SCL_PIN 0


#define TWI0 ((NRF_TWI_REG*)0x40003000)
typedef struct {
volatile uint32_t STARTRX;
volatile uint32_t Reserved0;
volatile uint32_t STARTTX;
volatile uint32_t Reserved1[2];
volatile uint32_t STOP;
volatile uint32_t Reserved2;
volatile uint32_t SUSPEND;
volatile uint32_t RESUME;
volatile uint32_t Reserved3[56];
volatile uint32_t STOPPED;
volatile uint32_t RXDREADY;
volatile uint32_t Reserved4[4];
volatile uint32_t TXDSENT;
volatile uint32_t Reserved5;
volatile uint32_t ERROR;
volatile uint32_t Reserved6[4];
volatile uint32_t BB;
volatile uint32_t Reserved7[49];
volatile uint32_t SHORTS;
volatile uint32_t Reserved8[63];
volatile uint32_t INTEN;
volatile uint32_t INTENSET;
volatile uint32_t INTENCLR;
volatile uint32_t Reserved9[110];
volatile uint32_t ERRORSRC;
volatile uint32_t Reserved10[14];
volatile uint32_t ENABLE;
volatile uint32_t Reserved11;
volatile uint32_t PSELSCL;
volatile uint32_t PSELSDA;
volatile uint32_t Reserved12[2];
volatile uint32_t RXD;
volatile uint32_t TXD;
volatile uint32_t Reserved13;
volatile uint32_t FREQUENCY;
volatile uint32_t Reserved14[24];
volatile uint32_t ADDRESS;
} NRF_TWI_REG;

void twi_init() 
{
	GPIO->PIN_CNF[SCL_PIN] = (6 << 8) | (3 << 2);

	GPIO->PIN_CNF[SDA_PIN] = (6 << 8) | (3 << 2);

	TWI0->PSELSCL = SCL_PIN;
	TWI0->PSELSDA = SDA_PIN;

	//TWI0->RXDREADY = 0;
	//TWI0->TXDSENT = 0;
	//TWI0->ERROR = 0;

	TWI0 ->FREQUENCY = 0x01980000; //100 kbps transfer frequency

	TWI0->ENABLE = 5; //Enable TWI
}

void twi_multi_read(
	uint8_t slave_address,
	uint8_t start_register,
	int registers_to_read,
	uint8_t * data_buffer
	) 
{
	TWI0->ADDRESS = slave_address;
	TWI0->TXDSENT = 0;
	TWI0->STARTTX = 1;
	TWI0->TXD = start_register;
	while (!TWI0->TXDSENT)
	{
		//Throw garbage
		if(TWI0->ERROR)
				return;
	}
	TWI0->RXDREADY = 0;
	TWI0->STARTRX = 1;
	for (int i = 0; i < registers_to_read - 1; i++) 
	{
		while (!TWI0->RXDREADY)
		{
			//Throw garbage
			if(TWI0->ERROR)
					return;
		}
		TWI0->RXDREADY = 0;
		data_buffer[i] = TWI0->RXD;
		TWI0->RESUME = 1;
	}

	while (!TWI0->RXDREADY)
	{
		//Throw garbage
		if(TWI0->ERROR)
				return;
	}
	TWI0->RXDREADY = 0;
	TWI0->STOP = 1;
	data_buffer[registers_to_read - 1] = TWI0->RXD;

	
}

void twi_multi_write(
uint8_t slave_address,
uint8_t start_register,
int registers_to_write,
uint8_t * data_buffer
)
{
	TWI0->ADDRESS = slave_address;
	TWI0->TXDSENT = 0;
	TWI0->STARTTX = 1;
	TWI0->TXD = start_register;
	while (!TWI0->TXDSENT)
	{
		//Throw garbage
		if(TWI0->ERROR)
				return;
	}

	//TWI0->STARTTX = 1;	//UNODVENDIG?

	for (int i = 0; i < registers_to_write; i++) 
	{
		TWI0->TXDSENT = 0;
		TWI0->TXD = data_buffer[i];
		while (!TWI0->TXDSENT)
		{
			//Throw garbage
			if(TWI0->ERROR)
					return;
		}
	}
	TWI0->STOP = 0;
}