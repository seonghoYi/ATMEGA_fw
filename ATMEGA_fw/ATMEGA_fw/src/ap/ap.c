#include "ap.h"


void apInit(void)
{
	uartOpen(_DEF_UART1, 115200);
	//timerStart(_DEF_TIM0, 0, 124, 0);
	//timerStart(_DEF_TIM1, 0, 1, 0);
	//timerStart(_DEF_TIM2, 0, 250, 5000);
	//a4988Start(_DEF_A4988_0, _DEF_CW, 124);
	
	ctcBegin(_DEF_TIM0);
	ctcSetOcr(_DEF_TIM0, 100, _DEF_CH_NONE);
	ctcStart(_DEF_TIM0);
	
	/*
	pwmBegin(_DEF_TIM1);
	pwm16ChannelConfig(_DEF_TIM1, _DEF_CH_A);
	pwmSetOcr(_DEF_TIM1, 250, _DEF_CH_A);
	pwmSetIcr(_DEF_TIM1, 5000);
	pwmStart(_DEF_TIM1);
	*/
	/*
	ctcBegin(_DEF_TIM1);
	ctc16ChannelConfig(_DEF_TIM1, _DEF_CH_A);
	ctcSetOcr(_DEF_TIM1, 50, _DEF_CH_A);
	//ctcSetIcr(_DEF_TIM1, 50);
	ctcStart(_DEF_TIM1);
	*/
	sei();
}


void apMain(void)
{	
	//char *str = "Hello, World!";
	uint16_t tcnt = 0;
	
	//uartPrintf(_DEF_UART0, "baud: %lu%s", uartGetBaud(_DEF_UART0), "bps\n");
	
	while(true)
	{
		//timerSetOcr(_DEF_TIM0, 1);
		//timerSetOcr(_DEF_TIM2, 250);
		//a4988SetSpeed(_DEF_A4988_0, 30);
		
		ctcSetTcnt(_DEF_TIM0, 0);
		tcnt = ctcGetTcnt(_DEF_TIM0);
		ctcSetOcr(_DEF_TIM0, 50, _DEF_CH_NONE);
		
		//pwmSetOcr(_DEF_TIM1, 250, _DEF_CH_A);
		//ctcSetTcnt(_DEF_TIM1, 0);
		//ctcSetOcr(_DEF_TIM1, 50, _DEF_CH_A);
		//ctcSetIcr(_DEF_TIM1, 50)
		uartPrintf(_DEF_UART1, "TCNT: %d\n", tcnt);
		_delay_ms(500);
		//a4988SetSpeed(_DEF_A4988_0, 124);

		ctcSetTcnt(_DEF_TIM0, 0);
		tcnt = ctcGetTcnt(_DEF_TIM0);
		ctcSetOcr(_DEF_TIM0, 100, _DEF_CH_NONE);
		
		//ctcSetTcnt(_DEF_TIM1, 0);
		//ctcSetOcr(_DEF_TIM1, 100, _DEF_CH_A);
		//ctcSetIcr(_DEF_TIM1, 100);
		uartPrintf(_DEF_UART1, "TCNT: %d\n", tcnt);
		_delay_ms(500);
		//timerSetOcr(_DEF_TIM0, 255);
		//timerSetOcr(_DEF_TIM2, 500);
		//a4988SetSpeed(_DEF_A4988_0, 255);
		
		ctcSetTcnt(_DEF_TIM0, 0);
		tcnt = ctcGetTcnt(_DEF_TIM0);
		ctcSetOcr(_DEF_TIM0, 200, _DEF_CH_NONE);
		
		//ctcSetTcnt(_DEF_TIM1, 0);
		//ctcSetOcr(_DEF_TIM1, 150, _DEF_CH_A);
		//ctcSetIcr(_DEF_TIM1, 200);
		//pwmSetOcr(_DEF_TIM1, 500, _DEF_CH_A);
		uartPrintf(_DEF_UART1, "TCNT: %d\n", tcnt);
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

