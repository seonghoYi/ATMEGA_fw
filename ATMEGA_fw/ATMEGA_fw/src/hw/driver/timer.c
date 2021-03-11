#include "timer.h"
#include "gpio.h"

#ifdef _USE_HW_TIM



typedef struct
{
	bool is_open;
	
	void (*func_ovf)(void);
	void (*func_oc)(void);
	void (*func_ic)(void);
	TIM_HandleTypeDef *h_tim;
	
} tim_t;


tim_t tim_tbl[TIM_MAX_CH];



TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;


bool timerInit(void)
{
	bool ret = true;
	
	for (int i = 0; i < TIM_MAX_CH; i++)
	{
		tim_tbl[i].is_open	= false;
		tim_tbl[i].func_ovf = NULL;
		tim_tbl[i].func_oc	= NULL;
		tim_tbl[i].func_ic	= NULL;
		tim_tbl[i].h_tim	= NULL;
	}
	
	return ret;
}

bool timerStart(uint8_t ch, uint16_t tcnt, uint16_t ocr, uint16_t icr)
{
	bool ret = false;
	tim_t *p_tim = &tim_tbl[ch];
	
	switch(ch)
	{
		case _DEF_TIM0:
		p_tim->h_tim = &htim1;
		
		htim1.TIMn = TIM0;
		htim1.Init.Prescaler	= TIM_CLK_PRESCALER_256;
		htim1.Init.Source		= TIM_INTCLK_SOURCE;
		htim1.Init.CountMode	= TIM_MOD_CTC_FORCE;
		htim1.Init.InputCapture = TIM_IC_DISABLE;
		htim1.Init.Output		= TIM_PORT_TGL_OC;
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
			p_tim->is_open = true;
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
		p_tim->h_tim = &htim2;
		
		htim2.TIMn = TIM2;
		htim2.Init.Prescaler	= TIM_CLK_PRESCALER_256;
		htim2.Init.Source		= TIM_INTCLK_SOURCE;
		htim2.Init.CountMode	= TIM_MOD_CTC_FORCE;
		htim2.Init.InputCapture = TIM_IC_DISABLE;
		htim2.Init.Output		= TIM_PORT_TGL_OC;
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
			p_tim->is_open = true;
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
		p_tim->h_tim = &htim3;
		
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
			p_tim->is_open = true;
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
		p_tim->h_tim = &htim4;
		
		htim4.TIMn = TIM3;
		break;
	}
	
	
	
	
	
	return ret;
}

bool timerStop(uint8_t ch)
{
	bool ret = true;
	tim_t *p_tim = &tim_tbl[ch];
	
	TIM_Stop(p_tim->h_tim);
	TIM_Stop_Normal_IT(p_tim->h_tim);
	TIM_Stop_OC_IT(p_tim->h_tim);
	TIM_Stop_IC_IT(p_tim->h_tim);

	
	
	return ret;
}


void timerAttachOVFInterrupt(uint8_t ch, void (*func)())
{
	tim_t *p_tim = &tim_tbl[ch];
	p_tim->func_ovf = func;
}

void timerAttachOCInterrupt(uint8_t ch, void (*func)())
{
	tim_t *p_tim = &tim_tbl[ch];
	p_tim->func_oc = func;
}

void timerAttachICInterrupt(uint8_t ch, void (*func)())
{
	tim_t *p_tim = &tim_tbl[ch];
	p_tim->func_ic = func;
}




uint16_t timerGetTcnt(uint8_t ch)
{
	uint16_t ret;
	tim_t *p_tim = &tim_tbl[ch];
	
	ret = p_tim->h_tim->Init.Tcnt;
	
	return ret;
}

bool timerSetTcnt(uint8_t ch, uint16_t tcnt)
{
	bool ret = false;
	tim_t *p_tim = &tim_tbl[ch];
	ret = timerStart(ch, tcnt, p_tim->h_tim->Init.Ocr, p_tim->h_tim->Init.Icr);
	
	return ret;
}

uint16_t timerGetOcr(uint8_t ch)
{
	uint16_t ret;
	tim_t *p_tim = &tim_tbl[ch];
	ret = p_tim->h_tim->Init.Ocr;

	return ret;
}

bool timerSetOcr(uint8_t ch, uint16_t ocr)
{
	bool ret = false;
	tim_t *p_tim = &tim_tbl[ch];
	ret = timerStart(ch, p_tim->h_tim->Init.Tcnt, ocr, p_tim->h_tim->Init.Icr);

	return ret;
}

uint16_t timerGetIcr(uint8_t ch)
{
	uint16_t ret;
	tim_t *p_tim = &tim_tbl[ch];
	ret = p_tim->h_tim->Init.Icr;
	
	return ret;
}

bool timerSetIcr(uint8_t ch, uint16_t icr)
{
	bool ret = false;
	tim_t *p_tim = &tim_tbl[ch];
	ret = timerStart(ch, p_tim->h_tim->Init.Tcnt, p_tim->h_tim->Init.Ocr, icr);

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