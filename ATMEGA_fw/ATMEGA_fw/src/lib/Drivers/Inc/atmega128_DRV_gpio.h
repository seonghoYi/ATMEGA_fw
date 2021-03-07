#ifndef ATMEGA128_DRV_GPIO_H_
#define ATMEGA128_DRV_GPIO_H_

#include "atmega128_DRV_def.h"


#define PUDISABLE() (SFIOR |= 1 << PUD)
#define PUENABLE()  (SFIOR &= ~(1 << PUD))

typedef struct 
{
	uint8_t Pin;
	uint8_t Mode;
}GPIO_InitTypeDef;

typedef enum
{
	PIN_RESET = 0x00,
	PIN_SET	
} GPIO_PinState;

#define GPIO_PIN0			0x00
#define GPIO_PIN1			0x01
#define GPIO_PIN2			0x02
#define GPIO_PIN3			0x03
#define GPIO_PIN4			0x04
#define GPIO_PIN5			0x05
#define GPIO_PIN6			0x06
#define GPIO_PIN7			0x07


#define GPIO_MODE_INPUT		0x00
#define GPIO_MODE_OUTPUT	0x01


void GPIO_Init(uint8_t port, GPIO_InitTypeDef *GPIO_Init);
GPIO_PinState GPIO_ReadPin(uint8_t port, uint8_t pin);
void GPIO_WritePin(uint8_t port, uint8_t pin, GPIO_PinState pinState);
void GPIO_TogglePin(uint8_t port, uint8_t pin);

#endif /* ATMEGA128_DRV_GPIO_H_ */