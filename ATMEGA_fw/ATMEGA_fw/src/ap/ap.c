#include "ap.h"
#include "uart.h"

void apInit(void)
{
	//uartOpen(_DEF_UART1, 38400);
	sei();
}

void apMain(void)
{	
	while(true)
	{
		/*
		if (uartAvailable(_DEF_UART1))
		{
			uint8_t rx_data = uartRead(_DEF_UART1);
			uartWrite(_DEF_UART1, &rx_data, 1);
		}
		*/
		rosServerRun();
	}
}
