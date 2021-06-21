#include "atmega128_hal.h"


#ifdef HAL_MODULE_ENABLED


volatile uint32_t tick;
HAL_TickFreqTypeDef tickFreq = HAL_TICK_FREQ_DEFAULT; // 1ms

HAL_StatusTypeDef HAL_Init(void)
{
	return HAL_InitTick();
}

HAL_StatusTypeDef HAL_InitTick(void)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint16_t prescaler;
	uint32_t ocr;
	uint32_t equation; //ctc 방정식에서의 분모 이름 진짜 애매하네

	SETB(TIMSK, 1);
	SETB(TIMSK, 6); // CTC mode
	
	equation = (tickFreq * F_CPU) / 2000; //ms
	
	if (!(equation % 8))
	{
		prescaler = 8;
	}
	else if (!(equation % 32))
	{
		prescaler = 32;
	}
	else if (!(equation % 64))
	{
		prescaler = 64;
	}
	else if (!(equation % 128))
	{
		prescaler = 128;
	}
	else if (!(equation % 256))
	{
		prescaler = 256;
	}
	else if (!(equation % 1024))
	{
		prescaler = 1024;
	}
	else
	{
		prescaler = 1;
	}
	
	ocr = (equation / prescaler) - 1;
	
	if (ocr < 255)
	{
		tickFreq = HAL_TICK_FREQ_DEFAULT;
	}
	
	switch(prescaler)
	{
		case 1:
		SETB(TCCR0, 0);
		CLRB(TCCR0, 1);
		CLRB(TCCR0, 2);
		break;
		case 8:
		CLRB(TCCR0, 0);
		SETB(TCCR0, 1);
		CLRB(TCCR0, 2);
		break;
		case 32:
		SETB(TCCR0, 0);
		SETB(TCCR0, 1);
		CLRB(TCCR0, 2);
		break;
		case 64:
		CLRB(TCCR0, 0);
		CLRB(TCCR0, 1);
		SETB(TCCR0, 2);
		break;
		case 128:
		SETB(TCCR0, 0);
		CLRB(TCCR0, 1);
		SETB(TCCR0, 2);
		break;
		case 256:
		CLRB(TCCR0, 0);
		SETB(TCCR0, 1);
		SETB(TCCR0, 2);
		break;
		case 1024:
		SETB(TCCR0, 0);
		SETB(TCCR0, 1);
		SETB(TCCR0, 2);
		break;
	}
	
	OCR0 = (uint8_t)ocr;
	sei();
	
	return status;
}

void HAL_IncTick(void)
{
	tick += tickFreq;
}

uint32_t HAL_GetTick(void)
{
	return tick;
}

void HAL_Delay(uint32_t Delay)
{
	uint32_t tickstart = HAL_GetTick();
	uint32_t wait = Delay;

	if (wait < HAL_MAX_DELAY)
	{
		wait += (uint32_t)tickFreq;
	}
	
	while ((HAL_GetTick() - tickstart) < wait);
}

HAL_StatusTypeDef SetTickFreq(uint32_t Freq)
{
	HAL_StatusTypeDef status = HAL_OK;
	HAL_TickFreqTypeDef prevFreq;
	
	if (tickFreq != Freq)
	{
		prevFreq = tickFreq;
		
		tickFreq = Freq;
		
		status = HAL_InitTick();
		
		if (status != HAL_OK)
		{
			tickFreq = prevFreq;
		}
	}
	
	return status;
}

uint32_t GetTickFreq(void)
{
	return tickFreq;
}


#endif