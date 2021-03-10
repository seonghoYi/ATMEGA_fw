#include "timer.h"
#include "gpio.h"

#ifdef _USE_HW_TIM


bool is_open[TIM_MAX_CH];

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;


bool timerInit(void)
{
	bool ret = true;
	
	for (int i = 0; i < TIM_MAX_CH; i++)
	{
		is_open[i] = false;
	}
	return ret;
}

bool timerStart(uint8_t ch, uint16_t tcnt, uint16_t ocr, uint16_t icr)
{
	bool ret = false;
	
	switch(ch)
	{
		case _DEF_TIM0:
		htim1.TIMn = TIM0;
		htim1.Init.Prescaler	= TIM_CLK_PRESCALER_256;
		htim1.Init.Source		= TIM_INTCLK_SOURCE;
		htim1.Init.CountMode	= TIM_MOD_FASTPWM;
		htim1.Init.InputCapture = TIM_IC_DISABLE;
		htim1.Init.Output		= TIM_PORT_OC_NONINV;
		htim1.Init.Channel		= TIM_CH_NONE;
		htim1.Init.Tcnt			= tcnt;
		htim1.Init.Ocr			= ocr;
		htim1.Init.Icr			= icr;
		
		if (TIM_Init(&htim1) != OK)
		{
			return ret;
		}
		else
		{
			ret = true;
			is_open[_DEF_TIM0] = true;
			/*
			if (TIM_Start(&htim1) != OK || TIM_Start_Normal_IT(&htim1) != OK)
			{
				ret = false;
			}
			*/
			if (TIM_Start(&htim1) != OK)
			{
				ret = false;
			}
		}
		break;
		case _DEF_TIM1:
		htim2.TIMn = TIM2;
		htim2.Init.Prescaler	= TIM_CLK_PRESCALER_256;
		htim2.Init.Source		= TIM_INTCLK_SOURCE;
		htim2.Init.CountMode	= TIM_MOD_FASTPWM;
		htim2.Init.InputCapture = TIM_IC_DISABLE;
		htim2.Init.Output		= TIM_PORT_OC_NONINV;
		htim2.Init.Channel		= TIM_CH_NONE;
		htim2.Init.Tcnt			= tcnt;
		htim2.Init.Ocr			= ocr;
		htim2.Init.Icr			= icr;
		
		if (TIM_Init(&htim2) != OK)
		{
			return ret;
		}
		else
		{
			ret = true;
			is_open[_DEF_TIM1] = true;
			/*
			if (TIM_Start(&htim2) != OK || TIM_Start_Normal_IT(&htim2) != OK)
			{
				ret = false;
			}
			*/
			if (TIM_Start(&htim2) != OK)
			{
				ret = false;
			}
		}
		break;
		case _DEF_TIM2:
		htim3.TIMn = TIM1;
		htim3.Init.Prescaler	= TIM_CLK_PRESCALER_64;
		htim3.Init.Source		= TIM_INTCLK_SOURCE;
		htim3.Init.CountMode	= TIM_MOD_FASTPWM_ICR;
		htim3.Init.InputCapture = TIM_IC_DISABLE;
		htim3.Init.Output		= TIM_PORT_OC_NONINV;
		htim3.Init.Channel		= TIM_CH_A;
		htim3.Init.Tcnt			= tcnt;
		htim3.Init.Ocr			= ocr;
		htim3.Init.Icr			= icr;
		if (TIM_Init(&htim3) != OK)
		{
			return ret;
		}
		else
		{
			ret = true;
			is_open[_DEF_TIM2] = true;
			/*
			if (TIM_Start(&htim2) != OK || TIM_Start_Normal_IT(&htim2) != OK)
			{
				ret = false;
			}
			*/
			if (TIM_Start(&htim3) != OK)
			{
				ret = false;
			}
		}
		
		break;
		case _DEF_TIM3:
		htim4.TIMn = TIM3;
		break;
	}
	
	
	
	
	
	return ret;
}

bool timerStop(uint8_t ch)
{
	bool ret = true;
	
	switch(ch)
	{
		case _DEF_TIM0:
		TIM_Stop(&htim1);
		TIM_Stop_Normal_IT(&htim1);
		TIM_Stop_OC_IT(&htim1);
		TIM_Stop_IC_IT(&htim1);
		break;
	}
	
	
	return ret;
}


uint16_t timerGetTcnt(uint8_t ch)
{
	uint16_t ret;
	switch(ch)
	{
		case _DEF_TIM0:
		ret = htim1.Init.Tcnt;
		break;
		case _DEF_TIM1:
		ret = htim2.Init.Tcnt;
		break;
		case _DEF_TIM2:
		ret = htim3.Init.Tcnt;
		break;
		case _DEF_TIM3:
		ret = htim4.Init.Tcnt;
		break;
		default:
		ret = 0;
		break;
	}
	return ret;
}

bool timerSetTcnt(uint8_t ch, uint16_t tcnt)
{
	bool ret = false;
	switch(ch)
	{
		case _DEF_TIM0:
		ret = timerStart(ch, tcnt, htim1.Init.Ocr, htim1.Init.Icr);
		break;
		case _DEF_TIM1:
		ret = timerStart(ch, tcnt, htim2.Init.Ocr, htim2.Init.Icr);
		break;
		case _DEF_TIM2:
		ret = timerStart(ch, tcnt, htim3.Init.Ocr, htim3.Init.Icr);
		break;
		case _DEF_TIM3:
		ret = timerStart(ch, tcnt, htim4.Init.Ocr, htim4.Init.Icr);
		break;
		default:
		break;
	}
	
	
	return ret;
}

uint16_t timerGetOcr(uint8_t ch)
{
	uint16_t ret;
	switch(ch)
	{
		case _DEF_TIM0:
		ret = htim1.Init.Ocr;
		break;
		case _DEF_TIM1:
		ret = htim2.Init.Ocr;
		break;
		case _DEF_TIM2:
		ret = htim3.Init.Ocr;
		break;
		case _DEF_TIM3:
		ret = htim4.Init.Ocr;
		break;
		default:
		ret = 0;
		break;
	}
	return ret;
}

bool timerSetOcr(uint8_t ch, uint16_t ocr)
{
	bool ret = false;
	switch(ch)
	{
		case _DEF_TIM0:
		ret = timerStart(ch, htim1.Init.Tcnt, ocr, htim1.Init.Icr);
		break;
		case _DEF_TIM1:
		ret = timerStart(ch, htim2.Init.Tcnt, ocr, htim2.Init.Icr);
		break;
		case _DEF_TIM2:
		ret = timerStart(ch, htim3.Init.Tcnt, ocr, htim3.Init.Icr);
		break;
		case _DEF_TIM3:
		ret = timerStart(ch, htim4.Init.Tcnt, ocr, htim4.Init.Icr);
		break;
		default:
		break;
	}
	return ret;
}

uint16_t timerGetIcr(uint8_t ch)
{
	uint16_t ret;
	switch(ch)
	{
		case _DEF_TIM0:
		ret = htim1.Init.Icr;
		break;
		case _DEF_TIM1:
		ret = htim2.Init.Icr;
		break;
		case _DEF_TIM2:
		ret = htim3.Init.Icr;
		break;
		case _DEF_TIM3:
		ret = htim4.Init.Icr;
		break;
		default:
		ret = 0;
		break;
	}
	return ret;
}

bool timerSetIcr(uint8_t ch, uint16_t icr)
{
	bool ret = false;
	switch(ch)
	{
		case _DEF_TIM0:
		ret = timerStart(ch, htim1.Init.Tcnt, htim1.Init.Ocr, icr);
		break;
		case _DEF_TIM1:
		ret = timerStart(ch, htim2.Init.Tcnt, htim2.Init.Ocr, icr);
		break;
		case _DEF_TIM2:
		ret = timerStart(ch, htim3.Init.Tcnt, htim3.Init.Ocr, icr);
		break;
		case _DEF_TIM3:
		ret = timerStart(ch, htim4.Init.Tcnt, htim4.Init.Ocr, icr);
		break;
		default:
		break;
	}
	return ret;
}




volatile int i = 0;
volatile int j = 0;
void TIM0_OVF_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (i == 250)
	{
		gpioPinToggle(_DEF_GPIO0);
		i = 0;
	}
	i++;
	TIM_Start(htim);
}

void TIM2_OVF_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (j == 500)
	{
		gpioPinToggle(_DEF_GPIO1);
		j = 0;
	}
	j++;
	TIM_Start(htim);
}



#endif