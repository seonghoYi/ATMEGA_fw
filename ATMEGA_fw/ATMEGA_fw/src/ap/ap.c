#include "ap.h"


void apInit(void)
{
	uartOpen(_DEF_UART1, 115200);

	motorInit();
	sei();
}


void apMain(void)
{	
	//char *str = "Hello, World!";
	
	//uartPrintf(_DEF_UART0, "baud: %lu%s", uartGetBaud(_DEF_UART0), "bps\n");
	motorSetSpeed(80);
	motorAdvance();
	
	while(true)
	{
		uint16_t ocr = 0;
		
		
		uartPrintf(_DEF_UART1, "speed : 100\n");
		motorSetSpeed(100);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 90\n");
		motorSetSpeed(90);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 80\n");
		motorSetSpeed(80);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 70\n");
		motorSetSpeed(70);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 60\n");
		motorSetSpeed(60);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 50\n");
		motorSetSpeed(50);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 40\n");
		motorSetSpeed(40);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 30\n");
		motorSetSpeed(30);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 20\n");
		motorSetSpeed(20);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 10\n");
		motorSetSpeed(10);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		uartPrintf(_DEF_UART1, "speed : 0\n");
		motorSetSpeed(0);
		ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(500);
		
		//motorSetSpeed(80);
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
		/*
		uint16_t tcnt = ctcGetTcnt(_DEF_TIM0);
		uint16_t ocr = ctcGetOcr(_DEF_TIM0, _DEF_CH_NONE);
		uartPrintf(_DEF_UART1, "tcnt : %d\n", tcnt);
		uartPrintf(_DEF_UART1, "ocr : %d\n", ocr);
		_delay_ms(100);
		*/
	}
}

