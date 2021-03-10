#include "ap.h"


void apInit(void)
{
	//uartOpen(_DEF_UART0, 115200);
	timerStart(_DEF_TIM0, 0, 255, 0);
	timerStart(_DEF_TIM1, 0, 1, 0);
	timerStart(_DEF_TIM2, 0, 250, 5000);
	sei();
}


void apMain(void)
{	
	//char *str = "Hello, World!";
	char input = 0;
	
	//uartPrintf(_DEF_UART0, "baud: %lu%s", uartGetBaud(_DEF_UART0), "bps\n");
	
	while(true)
	{
		timerSetOcr(_DEF_TIM0, 1);
		timerSetOcr(_DEF_TIM2, 250);
		_delay_ms(500);
		timerSetOcr(_DEF_TIM0, 255);
		timerSetOcr(_DEF_TIM2, 500);
		_delay_ms(500);
		/*
		gpioPinToggle(_DEF_GPIO0);
		_delay_ms(500);
		*/
		
		//uartPrintf(_DEF_UART0, "uart0 : %s\n", str);
		//gpioPinToggle(_DEF_GPIO0);
		
		//uartPrintf(_DEF_UART0, "available : %d\n", input, uartAvailable(_DEF_UART0));
		
		/*
		if (uartAvailable(_DEF_UART0) > 0)
		{
			gpioPinWrite(_DEF_GPIO0, true);
			input = uartRead(_DEF_UART0);
			uartPrintf(_DEF_UART0, "echo : %c, available : %d\n", input, uartAvailable(_DEF_UART0));
			
		}
		*/

	}
}

