#include "pwm.h"
#include "gpiote.h"
#include "ppi.h"
#include "timer.h"

int main() 
{
	int prescaler = 9;
	int period = 625; 
	int init_duty = 47;
	void pwm_init(int prescaler, int period, int init_duty);
	
	
}