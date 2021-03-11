#include "timer.h"
#include "gpio.h"
#include "drivemotor/a4988.h"



#ifdef _USE_HW_A4988

#define _DEF_LEFT_EN_PIN
#define _DEF_RIGHT_EN_PIN
#define _DEF_LEFT_DIR_PIN
#define _DEF_RIGHT_DIR_PIN



typedef struct
{
	bool is_open;
	
	a4988_HandlerTypeDef *h_a4988;
	TIM_HandleTypeDef *h_tim;
} a4988_t;


a4988_t a4988_tbl[A4988_MAX_CH];


a4988_HandlerTypeDef h_a4988_1;
a4988_HandlerTypeDef h_a4988_2;

bool a4988Init()
{
	bool ret = true;
	
	for (int i = 0; i < A4988_MAX_CH; i++)
	{
		a4988_tbl[i].is_open	= false;
		a4988_tbl[i].h_a4988	= NULL;
	}
	return ret;
}


bool a4988DriverInit(drivemotor_driver_t *p_driver)
{
	p_driver->init = a4988Init;
	
	return true;
}

bool a4988Start(uint8_t ch_, bool dir_, uint8_t speed_)
{
	bool ret = false;
	a4988_t *p_a4988 = &a4988_tbl[ch_];
	
	switch(ch_)
	{
		case _DEF_A4988_0:
		p_a4988->h_a4988				= &h_a4988_1;
		
		p_a4988->h_a4988->Init.ch		= _DEF_A4988_0;
		p_a4988->h_a4988->Init.timer	= _DEF_TIM0;
		p_a4988->h_a4988->Init.phase	= A4988_FULL_STEP;
		p_a4988->h_a4988->enable		= false;
		p_a4988->h_a4988->direction		= _DEF_CW;
		p_a4988->h_a4988->speed			= 0;		
		break;
		
		case _DEF_A4988_1:
		p_a4988->h_a4988				= &h_a4988_2;

		p_a4988->h_a4988->Init.ch		= _DEF_A4988_1;
		p_a4988->h_a4988->Init.timer	= _DEF_TIM1;
		p_a4988->h_a4988->Init.phase	= A4988_FULL_STEP;
		p_a4988->h_a4988->enable		= false;
		p_a4988->h_a4988->direction		= _DEF_CW;
		p_a4988->h_a4988->speed			= 0;
		break;
	}
	
	if (timerStart(p_a4988->h_a4988->Init.timer, 0, 0, 0) != true)
	{
		return ret;
	}
	else
	{
		p_a4988->is_open = true;
		ret = true;
	}
	
	return ret;
}


bool a4988Reset(uint8_t ch_, bool dir_, uint8_t speed_)
{
	
}

bool a4988Enable(uint8_t ch_)
{
	
}

bool a4988Disable(uint8_t ch_)
{
	
}

bool a4988SetDirection(uint8_t ch_, bool dir_)
{
	
}

bool a4988SetSpeed(uint8_t ch_, uint8_t speed_)
{
	bool ret = true;
	a4988_t *p_a4988 = &a4988_tbl[ch_];
	p_a4988->h_a4988->speed = speed_;
	timerSetOcr(p_a4988->h_a4988->Init.timer, p_a4988->h_a4988->speed);
	return ret;
}




#endif