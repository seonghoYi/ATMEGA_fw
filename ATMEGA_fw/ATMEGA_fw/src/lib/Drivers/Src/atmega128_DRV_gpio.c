#include "atmega128_DRV.h"

#ifdef DRV_GPIO_MODULE_ENABLED


GPIO_TypeDef GPIO_descripter[] = {
	{&DDRA, &PORTA, &PINA},
	{&DDRB, &PORTB, &PINB},
	{&DDRC, &PORTC, &PINC},
	{&DDRD, &PORTD, &PIND},
	{&DDRE, &PORTE, &PINE},
	{&DDRF, &PORTF, &PINF},
	{&DDRG, &PORTG, &PING},
	};
	
void GPIO_Init(uint8_t port, GPIO_InitTypeDef *GPIO_Init)
{
	GPIO_TypeDef *gpio = &GPIO_descripter[port];
	
	if(GPIO_Init->Mode == GPIO_MODE_INPUT)
	{
		CLRB(*(gpio->DDRn), GPIO_Init->Pin);
	}
	else if(GPIO_Init->Mode == GPIO_MODE_OUTPUT)
	{
		SETB(*(gpio->DDRn), GPIO_Init->Pin);
	}
}

GPIO_PinState GPIO_ReadPin(uint8_t port, uint8_t pin)
{
	GPIO_PinState ret = 0x00;
	GPIO_TypeDef *gpio = &GPIO_descripter[port];

	ret = READB(*(gpio->PINn), pin);
	return ret;
}

void GPIO_WritePin(uint8_t port, uint8_t pin, GPIO_PinState pinState)
{
	GPIO_TypeDef *gpio = &GPIO_descripter[port];
	
	if(pinState == PIN_RESET)
	{
		CLRB(*(gpio->PORTn), pin);
	}
	else if(pinState == PIN_SET)
	{
		SETB(*(gpio->PORTn), pin);
	}
	
}

void GPIO_TogglePin(uint8_t port, uint8_t pin)
{
	GPIO_TypeDef *gpio = &GPIO_descripter[port];
	TGLB(*(gpio->PORTn), pin);
}


#endif