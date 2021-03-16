#include "gpio.h"
#include "ctc.h"
#include "drivemotor/a4988.h"



#ifdef _USE_HW_A4988



typedef struct
{
	bool is_open;
	bool normal_rotate;
	bool reverse_rotate;
	a4988_HandlerTypeDef *h_a4988;
} a4988_t;


	
a4988_t a4988_tbl[A4988_MAX_CH];


a4988_HandlerTypeDef h_a4988_1;
a4988_HandlerTypeDef h_a4988_2;

bool a4988Enable(void);
bool a4988Disable(void);

bool a4988Init(uint8_t ch_)
{
	bool ret = false;
	
	
	a4988_t *p_a4988 = &a4988_tbl[ch_];

	switch(ch_)
	{
		case _DEF_A4988_0:
		p_a4988->h_a4988				= &h_a4988_1;
		
		p_a4988->normal_rotate			= _DEF_CW;
		p_a4988->reverse_rotate			= _DEF_CCW;
		p_a4988->h_a4988->Init.ch		= _DEF_A4988_0;
		p_a4988->h_a4988->Init.timer	= TIM0;
		p_a4988->h_a4988->Init.phase	= A4988_FULL_STEP;
		p_a4988->h_a4988->enable		= false;
		p_a4988->h_a4988->direction		= _NORMAL_ROTATION;
		p_a4988->h_a4988->speed			= 0;
		
		break;
		case _DEF_A4988_1:
		p_a4988->h_a4988				= &h_a4988_2;
		
		p_a4988->normal_rotate			= _DEF_CW;
		p_a4988->reverse_rotate			= _DEF_CCW;
		p_a4988->h_a4988->Init.ch		= _DEF_A4988_1;
		p_a4988->h_a4988->Init.timer	= TIM2;
		p_a4988->h_a4988->Init.phase	= A4988_FULL_STEP;
		p_a4988->h_a4988->enable		= false;
		p_a4988->h_a4988->direction		= _NORMAL_ROTATION;
		p_a4988->h_a4988->speed			= 0;
		
		break;
		default:
		break;
	}
	
	
	if (ctcBegin(p_a4988->h_a4988->Init.timer) != true)
	{
		ret = false;
	}
	else
	{
		p_a4988->is_open = true;
		ret = true;
	}
	
	return ret;
}


bool a4988DriverInit(drivemotor_driver_t *p_driver)
{
	p_driver->init			= a4988Init;
	p_driver->startMotor	= a4988Start;
	p_driver->stopMotor		= a4988Stop;
	p_driver->breakMotor	= a4988Hold;
	p_driver->setSpeed		= a4988SetSpeed;
	p_driver->getSpeed		= a4988GetSpeed;
	p_driver->setDirection	= a4988SetDirection;
	p_driver->getDirection	= a4988GetDirection;
	p_driver->setCallBack	= a4988SetCallBack;
	return true;
}

bool a4988Start(uint8_t ch_)
{
	bool ret = false;
	a4988_t *p_a4988 = &a4988_tbl[ch_];
	
	if (ctcStart(p_a4988->h_a4988->Init.timer) != true)
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
	a4988Enable();
	
	return ret;
}


bool a4988Stop(uint8_t ch_)
{
	bool ret = false;
	a4988_t *p_a4988 = &a4988_tbl[ch_];
	
	if (ctcStop(p_a4988->h_a4988->Init.timer) != true)
	{
		ret = false;
	}
	else
	{
		ret = true;
	}
	_delay_ms(3);
	a4988Disable();
	return ret;
}

bool a4988Hold(uint8_t ch_)
{
	bool ret = a4988Stop(ch_);
	ret = a4988Enable();
	
	return ret;
}

bool a4988Enable(void)
{
	bool ret = true;
	gpioPinWrite(_PIN_GPIO_A4988_EN, true);
	return ret;
}

bool a4988Disable()
{
	bool ret = true;
	gpioPinWrite(_PIN_GPIO_A4988_EN, false);
	return ret;
}

bool a4988SetDirection(uint8_t ch_, bool dir_)
{
	bool ret = true;
	a4988_t *p_a4988 = &a4988_tbl[ch_];
	uint8_t side = 0;
	p_a4988->h_a4988->direction = dir_;
	
	switch(ch_)
	{
		case _DEF_A4988_0:
		side = _PIN_GPIO_A4988_LDIR;
		break;
		case _DEF_A4988_1:
		side = _PIN_GPIO_A4988_RDIR;
		break;
		default:
		break;
	}
	
	if (p_a4988->h_a4988->direction)
	{
		gpioPinWrite(side, p_a4988->normal_rotate);
	}
	else
	{
		gpioPinWrite(side, p_a4988->reverse_rotate);
	}
	return ret;
}

bool a4988GetDirection(uint8_t ch_)
{
	bool ret;
	
	a4988_t *p_a4988 = &a4988_tbl[ch_];
	/*
	uint8_t side = 0;
	
	switch(ch_)
	{
		case _DEF_A4988_0:
		side = _PIN_GPIO_A4988_LDIR;
		break;
		case _DEF_A4988_1:
		side = _PIN_GPIO_A4988_RDIR;
		break;
		default:
		break;
	}
	*/
	if (p_a4988->h_a4988->direction)
	{
		ret = p_a4988->normal_rotate;
	}
	else
	{
		ret = p_a4988->reverse_rotate;
	}
	return ret;
}

bool a4988SetSpeed(uint8_t ch_, uint8_t speed_)
{
	bool ret = false;

	a4988_t *p_a4988 = &a4988_tbl[ch_];
	
	p_a4988->h_a4988->speed = speed_;
	
	ctcSetTcnt(p_a4988->h_a4988->Init.timer, 0);
	if (p_a4988->h_a4988->Init.timer == TIM0 || p_a4988->h_a4988->Init.timer == TIM2)
	{
		ret = ctcSetOcr(p_a4988->h_a4988->Init.timer, p_a4988->h_a4988->speed, _DEF_CH_NONE);	
	}
	else if (p_a4988->h_a4988->Init.timer == TIM1 || p_a4988->h_a4988->Init.timer == TIM3)
	{
		ret = ctcSetOcr(p_a4988->h_a4988->Init.timer, p_a4988->h_a4988->speed, _DEF_CH_A);	
	}
	return ret;
}

uint8_t a4988GetSpeed(uint8_t ch_)
{
	uint8_t ret = 0;
	
	a4988_t *p_a4988 = &a4988_tbl[ch_];
	ret = p_a4988->h_a4988->speed;
	return ret;
}

void a4988SetCallBack(uint8_t ch_, void (*p_func)(void))
{
	
}

#endif