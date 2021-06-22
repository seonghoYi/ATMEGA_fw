#include "bsp.h"

typedef enum
{
	TICK_FREQ_250HZ		= 4U,
	TICK_FREQ_500HZ		= 2U,
	TICK_FREQ_1KHZ		= 1U,
	TICK_FREQ_DEFAULT	= TICK_FREQ_1KHZ
} TickFreqTypeDef;



void initTick(void);
uint32_t getTick(void);

volatile uint32_t tick;
TickFreqTypeDef tickFreq = TICK_FREQ_DEFAULT; // 1ms

void bspInit(void)
{
	initTick();
}


void delay(uint32_t ms)
{
	uint32_t tickstart = getTick();
	uint32_t wait = ms;

	if (wait < 0xFFFFFFFF) // max delay
	{
		wait += (uint32_t)tickFreq;
	}
	
	while ((getTick() - tickstart) < wait);
}

uint32_t millis(void)
{
	return getTick();
}

void initTick(void)
{
	uint16_t prescaler;
	uint32_t ocr;
	uint32_t equation; //ctc 방정식에서의 분모 이름 진짜 애매하네

	SET_BIT(TIMSK, 1 << OCIE0);
	SET_BIT(TCCR0, 1 << WGM00); // CTC mode
	
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
		tickFreq = TICK_FREQ_DEFAULT;
	}
	
	switch(prescaler)
	{
		case 1:
		CLR_BIT(TCCR0, (1 << CS01) | (1 << CS02));
		SET_BIT(TCCR0, (1 << CS00));
		break;
		case 8:
		CLR_BIT(TCCR0, (1 << CS00) | (1 << CS02));
		SET_BIT(TCCR0, (1 << CS02));
		break;
		case 32:
		SET_BIT(TCCR0, (1 << CS00) | (1 << CS01));
		CLR_BIT(TCCR0, (1 << CS02));
		break;
		case 64:
		CLR_BIT(TCCR0, (1 << CS00) | (1 << CS01));
		SET_BIT(TCCR0, (1 << CS02));
		break;
		case 128:
		SET_BIT(TCCR0, (1 << CS00) | (1 << CS02));
		CLR_BIT(TCCR0, (1 << CS01));
		break;
		case 256:
		SET_BIT(TCCR0, (1 << CS01) | (1 << CS02));
		CLR_BIT(TCCR0, (1 << CS00));
		break;
		case 1024:
		SET_BIT(TCCR0, (1 << CS00) | (1 << CS01) | (1 << CS02));
		break;
	}
	
	OCR0 = (uint8_t)ocr;
	sei();
}

void incTick(void)
{
	tick += tickFreq;
}

uint32_t getTick(void)
{
	return tick;
}

