#include "gpio.h"

#ifdef _USE_HW_GPIO

#define PIN_SET		1
#define PIN_RESET	0


typedef struct
{
	volatile uint8_t *ddr;
	volatile uint8_t *out;
	volatile uint8_t *in;
	uint8_t dir;
	uint8_t pin;
	bool on_state;
	bool off_state;
	bool init_value;
} gpio_tbl_t;



gpio_tbl_t gpio_tbl[GPIO_MAX_CH] = 
{
	{&DDRA, &PORTA, &PINA, _DEF_OUTPUT, PIN0, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_BT_RST
	{&DDRA, &PORTA, &PINA, _DEF_OUTPUT, PIN1, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_BT_CFG
	{&DDRA, &PORTA, &PINA, _DEF_OUTPUT, PIN2, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_RUN_LED
	{&DDRA, &PORTA, &PINA, _DEF_OUTPUT, PIN3, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_SUCTION_RELAY
	{&DDRG, &PORTG, &PING, _DEF_OUTPUT, PIN0, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_MOTOR_EN
	{&DDRG, &PORTG, &PING, _DEF_OUTPUT, PIN1, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_MOTOR_DIR_L
	{&DDRG, &PORTG, &PING, _DEF_OUTPUT, PIN2, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_MOTOR_DIR_R
	{&DDRB, &PORTB, &PINB, _DEF_OUTPUT, PIN5, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_MOTOR_PWM_L
	{&DDRB, &PORTB, &PINB, _DEF_OUTPUT, PIN6, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_MOTOR_PWM_R
	{&DDRB, &PORTB, &PINB, _DEF_OUTPUT, PIN7, PIN_SET, PIN_RESET, false}, // _DEF_GPIO_SUCTION_PWM
};

bool gpioPinMode(uint8_t ch, uint8_t mode);

bool gpioInit(void)
{
	bool ret = true;
	
	for(int i = 0; i < GPIO_MAX_CH; i++)
	{
		gpioPinMode(i, gpio_tbl[i].dir);
	}
	return ret;
}

bool gpioPinMode(uint8_t ch, uint8_t mode)
{
	bool ret = false;
	
	if (ch < 0 || ch >= GPIO_MAX_CH) return ret;
	
	gpio_tbl_t *p_gpio = &gpio_tbl[ch];
	
	switch(mode)
	{
		case _DEF_INPUT:
			*(p_gpio->ddr) &= ~(1<<p_gpio->pin);
		break;
		
		case _DEF_OUTPUT:
			*(p_gpio->ddr) |= (1<<p_gpio->pin);
		break;
		default:
		break;
	}
	
	gpioPinWrite(ch, p_gpio->init_value);
	
	ret = true;
	
	return ret;
}

void gpioPinWrite(uint8_t ch, bool value)
{
	if (ch < 0 || ch >= GPIO_MAX_CH) return;
	
	gpio_tbl_t *p_gpio = &gpio_tbl[ch];
	
	if (value)
	{
		if (p_gpio->on_state)
		{
			*(p_gpio->out) |= (1<<p_gpio->pin);
		}
		else
		{
			*(p_gpio->out) &= ~(1<<p_gpio->pin);
		}
	}
	else
	{
		if (p_gpio->on_state)
		{
			*(p_gpio->out) &= ~(1<<p_gpio->pin);
		}
		else
		{
			*(p_gpio->out) |= (1<<p_gpio->pin);
		}
	}
	
}

bool gpioPinRead(uint8_t ch)
{
	bool ret = false;
	if (ch < 0 || ch >= GPIO_MAX_CH) return ret;
	
	gpio_tbl_t *p_gpio = &gpio_tbl[ch];
	
	if ((*(p_gpio->in) & (1<<p_gpio->pin)) >> p_gpio->pin == p_gpio->on_state)
	{
		ret = true;
	}

	return ret;
}

void gpioPinToggle(uint8_t ch)
{
	if (ch < 0 || ch >= GPIO_MAX_CH) return;
	gpio_tbl_t *p_gpio = &gpio_tbl[ch];
	
	*(p_gpio->out) ^= (1<<p_gpio->pin);
}

#endif