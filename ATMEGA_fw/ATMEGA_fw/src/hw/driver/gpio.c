#include "gpio.h"

#ifdef _USE_HW_GPIO
typedef struct
{
	uint8_t port;
	uint8_t pin;
	uint8_t mode;
	GPIO_PinState on_state;
	GPIO_PinState off_state;
} gpio_tbl_t;


gpio_tbl_t gpio_tbl[GPIO_MAX_CH] = 
{
	{GPIOA, PIN0, _DEF_OUTPUT, PIN_SET, PIN_RESET},
};

bool gpioPinMode(uint8_t ch, uint8_t mode);

bool gpioInit(void)
{
	bool ret = true;
	
	for(int i = 0; i < GPIO_MAX_CH; i++)
	{
		gpioPinMode(i, gpio_tbl[i].mode);
	}
	return ret;
}

bool gpioPinMode(uint8_t ch, uint8_t mode)
{
	bool ret = false;
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	if (ch < 0 || ch >= GPIO_MAX_CH) return ret;
	
	switch(mode)
	{
		case _DEF_INPUT:
			GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		break;
		
		case _DEF_OUTPUT:
			GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
		break;
		default:
		break;
	}
	
	GPIO_InitStruct.Pin = gpio_tbl[ch].pin;
	GPIO_Init(gpio_tbl[ch].port, &GPIO_InitStruct);
	ret = true;
	
	return ret;
}

void gpioPinWrite(uint8_t ch, bool value)
{
	if (ch < 0 || ch >= GPIO_MAX_CH) return;
	if (value)
	{
		GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].on_state);
	}
	else
	{
		GPIO_WritePin(gpio_tbl[ch].port, gpio_tbl[ch].pin, gpio_tbl[ch].off_state);
	}
}

bool gpioPinRead(uint8_t ch)
{
	bool ret = false;
	if (ch < 0 || ch >= GPIO_MAX_CH) return ret;
	
	if (GPIO_ReadPin(gpio_tbl[ch].port, gpio_tbl[ch].pin) == gpio_tbl[ch].on_state)
	{
		ret = true;	
	}
	return ret;
}

void gpioPinToggle(uint8_t ch)
{
	if (ch < 0 || ch >= GPIO_MAX_CH) return;
	GPIO_TogglePin(gpio_tbl[ch].port, gpio_tbl[ch].pin);
}

#endif