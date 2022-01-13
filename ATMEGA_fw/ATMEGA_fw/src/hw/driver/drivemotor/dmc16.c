#include "drivemotor/dmc16.h"
//#include "pwm.h"
#include "gpio.h"

#ifdef _USE_HW_DMC16

typedef struct
{
	bool is_open;
	bool normal_rotate;
	bool reverse_rotate;
	dmc16_HandlerTypeDef *h_dmc16;
} dmc16_t;


dmc16_t dmc16_tbl[DMC16_MAX_CH];


dmc16_HandlerTypeDef h_dmc16_1;
dmc16_HandlerTypeDef h_dmc16_2;

bool dmc16Enable(void);
bool dmc16Disable(void);

bool dmc16Init(uint8_t ch_)
{
	bool ret = false;
	
	dmc16_t *p_dmc16 = &dmc16_tbl[ch_];

	switch(ch_)
	{
		case _DEF_DMC16_1:
		p_dmc16->h_dmc16				= &h_dmc16_1;
		
		p_dmc16->normal_rotate			= _DEF_CCW;
		p_dmc16->reverse_rotate			= _DEF_CW;
		p_dmc16->h_dmc16->Init.ch		= _DEF_DMC16_1;
		p_dmc16->h_dmc16->Init.pwm_ch	= _DEF_TIM_CH_A;
		p_dmc16->h_dmc16->enable		= false;
		p_dmc16->h_dmc16->direction		= _NORMAL_ROTATION;
		p_dmc16->h_dmc16->speed			= 0;
		
		break;
		case _DEF_DMC16_2:
		p_dmc16->h_dmc16				= &h_dmc16_2;
		
		p_dmc16->normal_rotate			= _DEF_CW;
		p_dmc16->reverse_rotate			= _DEF_CCW;
		p_dmc16->h_dmc16->Init.ch		= _DEF_DMC16_2;
		p_dmc16->h_dmc16->Init.pwm_ch	= _DEF_TIM_CH_B;
		p_dmc16->h_dmc16->enable		= false;
		p_dmc16->h_dmc16->direction		= _NORMAL_ROTATION;
		p_dmc16->h_dmc16->speed			= 0;
		
		break;
		default:
		break;
	}
	
	p_dmc16->is_open = true;
	
	
	DDRB |= (1<<PIN5) | (1<<PIN6); //Output enable
	
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1);
	TCCR1A &= ~((1<<COM1A0) | (1<<COM1B0)); //Fast PWM channel-A,B Non inverting mode
	
	TCCR1A |= (1<<WGM10) | (1<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B &= ~(1<<WGM13); //Fast PWM 10-bit mode
	
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~((1<<CS10) | (1<<CS12)); //Prescaler 8 -> 1.953KHz PWM in 16MHz system clock
	
	
	TCCR1A &= ~((1<<COM1A0) | (1<<COM1A1) | (1<<COM1B0) | (1<<COM1B1)); //Off PWM wave output
	PORTB &= ~((1<<PIN5) | (1<<PIN6)); //Output pull low  //Port 모드가 normal 일 경우 low 상태로 유지되게 하기 위함
	
	ret = true;
	
	dmc16SetDirection(_DEF_DMC16_1, p_dmc16->h_dmc16->direction);
	dmc16SetDirection(_DEF_DMC16_2, p_dmc16->h_dmc16->direction);
	
	return ret;
}

bool dmc16DriverInit(drivemotor_driver_t *p_driver)
{
	p_driver->init			= dmc16Init;
	p_driver->startMotor	= dmc16Start;
	p_driver->stopMotor		= dmc16Stop;
	p_driver->breakMotor	= dmc16Hold;
	p_driver->setSpeed		= dmc16SetSpeed;
	p_driver->getSpeed		= dmc16GetSpeed;
	p_driver->setDirection	= dmc16SetDirection;
	p_driver->getDirection	= dmc16GetDirection;
	p_driver->setCallBack	= NULL;
	return true;
}

bool dmc16Start(uint8_t ch_)
{
	bool ret = false;
	dmc16_t *p_dmc16 = &dmc16_tbl[ch_];
	
	if(p_dmc16->h_dmc16->Init.pwm_ch == _DEF_TIM_CH_A)
	{
		TCCR1A |= (1<<COM1A1);
		TCCR1A &= ~(1<<COM1A0); //Fast PWM channel-A Non inverting mode
	}
	else if(p_dmc16->h_dmc16->Init.pwm_ch == _DEF_TIM_CH_B)
	{
		TCCR1B |= (1<<COM1B1);
		TCCR1B &= ~(1<<COM1B0); //Fast PWM channel-B Non inverting mode	
	}
	
	dmc16Enable();
	
	return ret;
}

bool dmc16Stop(uint8_t ch_)
{
	bool ret = true;
	dmc16_t *p_dmc16 = &dmc16_tbl[ch_];

	dmc16SetSpeed(p_dmc16->h_dmc16->Init.ch, 0);
	
	if(p_dmc16->h_dmc16->Init.pwm_ch == _DEF_TIM_CH_A)
	{
		TCCR1A &= ~((1<<COM1A0) | (1<<COM1A1)); //Off PWM OCA output
	}
	else if(p_dmc16->h_dmc16->Init.pwm_ch == _DEF_TIM_CH_B)
	{
		TCCR1B &= ~((1<<COM1B0) | (1<<COM1B1)); //Off PWM OCB output
	}

	dmc16Disable();
	return ret;	
}

bool dmc16Hold(uint8_t ch_)
{
	bool ret = dmc16Stop(ch_);
	ret = dmc16Enable();
	
	return ret;	
}

bool dmc16Enable(void)
{
	bool ret = true;
	gpioPinWrite(_PIN_GPIO_DMC16_EN, true);
	return ret;
}
bool dmc16Disable(void)
{
	bool ret = true;
	gpioPinWrite(_PIN_GPIO_DMC16_EN, false);
	return ret;
}

bool dmc16SetDirection(uint8_t ch_, bool dir_)
{
	bool ret = true;
	dmc16_t *p_dmc16 = &dmc16_tbl[ch_];
	uint8_t side = 0;
	p_dmc16->h_dmc16->direction = dir_;
	
	switch(ch_)
	{
		case _DEF_DMC16_1:
		side = _PIN_GPIO_DMC16_LDIR;
		break;
		case _DEF_DMC16_2:
		side = _PIN_GPIO_DMC16_RDIR;
		break;
		default:
		break;
	}
	
	if (p_dmc16->h_dmc16->direction)
	{
		gpioPinWrite(side, p_dmc16->reverse_rotate);
	}
	else
	{
		gpioPinWrite(side, p_dmc16->normal_rotate);
	}
	return ret;
}

bool dmc16GetDirection(uint8_t ch_)
{
	bool ret;
	
	dmc16_t *p_dmc16 = &dmc16_tbl[ch_];
	
	if (p_dmc16->h_dmc16->direction)
	{
		ret = p_dmc16->normal_rotate;
	}
	else
	{
		ret = p_dmc16->reverse_rotate;
	}
	return ret;	
}

bool dmc16SetSpeed(uint8_t ch_, uint16_t speed_)
{
	bool ret = false;

	dmc16_t *p_dmc16 = &dmc16_tbl[ch_];
	
	if (speed_ < 0 || speed_ > 100)
	{
		return ret;
	}
	
	p_dmc16->h_dmc16->speed = 511 * speed_ / 100;
	
	if(p_dmc16->h_dmc16->Init.pwm_ch == _DEF_TIM_CH_A)
	{
		OCR1A = p_dmc16->h_dmc16->speed;
	}
	else if(p_dmc16->h_dmc16->Init.pwm_ch == _DEF_TIM_CH_B)
	{
		OCR1B = p_dmc16->h_dmc16->speed;
	}
	
	ret = true;
	
	return ret;
}

uint16_t dmc16GetSpeed(uint8_t ch_)
{
	uint16_t ret = 0;
	
	dmc16_t *p_dmc16 = &dmc16_tbl[ch_];
	ret = p_dmc16->h_dmc16->speed;
	return ret;
}

#endif