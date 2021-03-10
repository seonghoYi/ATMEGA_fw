#include "atmega128_DRV.h"

#ifdef DRV_TIMER_MODULE_ENABLED

TIM8_TypeDef TIM8_descripter[] = {
	{&TCCR0, &TCNT0, &OCR0, &ASSR},
	{&TCCR2, &TCNT2, &OCR2, NULL}
};

TIM16_TypeDef TIM16_descripter[] = {
	{&TCCR1A, &TCCR1B, &TCCR1C, &TCNT1L,&TCNT1H, &OCR1AL, &OCR1AH, &OCR1BL, &OCR1BH, &ICR1L, &ICR1H},
	{&TCCR3A, &TCCR3B, &TCCR3C, &TCNT3L,&TCNT3H, &OCR3AL, &OCR3AH, &OCR3BL, &OCR3BH, &ICR3L, &ICR3H}
};


StatusTypeDef TIM8_Init(TIM_HandleTypeDef *htim);
StatusTypeDef TIM16_Init(TIM_HandleTypeDef *htim);

StatusTypeDef TIM_Init(TIM_HandleTypeDef *htim)
{
	if (htim->TIMn == TIM0 || htim->TIMn == TIM2)
	{
		TIM8_Init(htim);
	}
	else if (htim->TIMn == TIM1 || htim->TIMn == TIM3)
	{
		TIM16_Init(htim);
	}
	
	return OK;
}

StatusTypeDef TIM8_Init(TIM_HandleTypeDef *htim)
{
	TIM8_TypeDef *tim = NULL;

	if (htim->TIMn == TIM0)
	{
		tim = &TIM8_descripter[0];
		switch(htim->Init.Prescaler)
		{
			case TIM_CLK_PRESCALER_1:
			CLRB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_8:
			CLRB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_32:
			CLRB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_64:
			SETB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_128:
			SETB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_256:
			SETB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_1024:
			SETB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			default:
			break;
		}
	}
	else if (htim->TIMn == TIM2)
	{
		tim = &TIM8_descripter[1];
		switch(htim->Init.Prescaler)
		{
			case TIM_CLK_PRESCALER_1:
			CLRB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_8:
			CLRB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_64:
			CLRB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_256:
			SETB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_1024:
			SETB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			default:
			break;
		}
		
		switch(htim->Init.Source)
		{
			case TIM_INTCLK_SOURCE:
			break;
			case TIM_EXTCLK_FALLING:
			*(tim->TCCRn) &= ~0x07;
			SETB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_EXTCLK_RISING:
			*(tim->TCCRn) &= ~0x07;
			SETB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			default:
			break;
		}
	}
	
	if (tim == NULL)
	{
		return ERROR;
	}
	
	switch(htim->Init.CountMode)
	{
		case TIM_MOD_NORMAL:
		CLRB(*(tim->TCCRn), 7);
		CLRB(*(tim->TCCRn), 6);
		CLRB(*(tim->TCCRn), 3);
		break;
		case TIM_MOD_CTC:
		CLRB(*(tim->TCCRn), 7);
		CLRB(*(tim->TCCRn), 6);
		SETB(*(tim->TCCRn), 3);
		break;
		case TIM_MOD_FASTPWM:
		CLRB(*(tim->TCCRn), 7);
		SETB(*(tim->TCCRn), 6);
		SETB(*(tim->TCCRn), 3);
		break;
		case TIM_MOD_PCPWM:
		CLRB(*(tim->TCCRn), 7);
		SETB(*(tim->TCCRn), 6);
		CLRB(*(tim->TCCRn), 3);
		break;
		case TIM_MOD_NORMAL_FORCE:
		SETB(*(tim->TCCRn), 7);
		CLRB(*(tim->TCCRn), 6);
		CLRB(*(tim->TCCRn), 3);
		break;
		case TIM_MOD_CTC_FORCE:
		SETB(*(tim->TCCRn), 7);
		CLRB(*(tim->TCCRn), 6);
		CLRB(*(tim->TCCRn), 3);
		break;
		default:
		break;
	}
	
	switch(htim->Init.Output)
	{
		case TIM_PORT_NORMAL:
		CLRB(*(tim->TCCRn), 4);
		CLRB(*(tim->TCCRn), 5);
		break;
		case TIM_PORT_TGL_OC:
		SETB(*(tim->TCCRn), 4);
		CLRB(*(tim->TCCRn), 5);
		break;
		case TIM_PORT_CLR_OC:
		CLRB(*(tim->TCCRn), 4);
		SETB(*(tim->TCCRn), 5);
		break;
		case TIM_PORT_SET_OC:
		SETB(*(tim->TCCRn), 4);
		SETB(*(tim->TCCRn), 5);
		break;
		case TIM_PORT_OC_NONINV:
		CLRB(*(tim->TCCRn), 4);
		SETB(*(tim->TCCRn), 5);
		break;
		case TIM_PORT_OC_INV:
		SETB(*(tim->TCCRn), 4);
		SETB(*(tim->TCCRn), 5);
		break;
		default:
		break;
	}
	
	*(tim->TCNTn) = (uint8_t) htim->Init.Tcnt;
	*(tim->OCRn) = (uint8_t) htim->Init.Ocr;
	
	
	return OK;
}

StatusTypeDef TIM16_Init(TIM_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	
	if (htim->TIMn == TIM1)
	{
		tim = &TIM16_descripter[0];	
	}
	else if (htim->TIMn == TIM3)
	{
		tim = &TIM16_descripter[1];
	}
	
	if (tim == NULL)
	{
		return ERROR;
	}
	
	switch(htim->Init.Prescaler)
	{
		case TIM_CLK_PRESCALER_1:
		CLRB(*(tim->TCCRnB), 2);
		CLRB(*(tim->TCCRnB), 1);
		SETB(*(tim->TCCRnB), 0);
		break;
		case TIM_CLK_PRESCALER_8:
		CLRB(*(tim->TCCRnB), 2);
		SETB(*(tim->TCCRnB), 1);
		CLRB(*(tim->TCCRnB), 0);
		break;
		case TIM_CLK_PRESCALER_64:
		CLRB(*(tim->TCCRnB), 2);
		SETB(*(tim->TCCRnB), 1);
		SETB(*(tim->TCCRnB), 0);
		break;
		case TIM_CLK_PRESCALER_256:
		SETB(*(tim->TCCRnB), 2);
		CLRB(*(tim->TCCRnB), 1);
		CLRB(*(tim->TCCRnB), 0);
		break;
		case TIM_CLK_PRESCALER_1024:
		SETB(*(tim->TCCRnB), 2);
		CLRB(*(tim->TCCRnB), 1);
		SETB(*(tim->TCCRnB), 0);
		break;
		default:
		break;
	}
	
	switch(htim->Init.Source)
	{
		case TIM_INTCLK_SOURCE:
		break;
		case TIM_EXTCLK_FALLING:
		*(tim->TCCRnB) &= ~0x07;
		SETB(*(tim->TCCRnB), 2);
		SETB(*(tim->TCCRnB), 1);
		CLRB(*(tim->TCCRnB), 0);
		break;
		case TIM_EXTCLK_RISING:
		*(tim->TCCRnB) &= ~0x07;
		SETB(*(tim->TCCRnB), 2);
		SETB(*(tim->TCCRnB), 1);
		SETB(*(tim->TCCRnB), 0);
		break;
		default:
		break;
	}
	
	switch(htim->Init.CountMode)
	{
		case TIM_MOD_NORMAL:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_CTC_OCR:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_FASTPWM_OCR:
		SETB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_PCPWM_OCR:
		SETB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_FPCPWM_OCR:
		SETB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_CTC_ICR:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_FASTPWM_ICR:
		CLRB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_PCPWM_ICR:
		CLRB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_FPCPWM_ICR:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_FASTPWM_8B:
		SETB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_FASTPWM_9B:
		CLRB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_FASTPWM_10B:
		SETB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_PCPWM_8B:
		SETB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_PCPWM_9B:
		CLRB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_PCPWM_10B:
		SETB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM_MOD_NORMAL_FORCE:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(*(tim->TCCRnC), 7);
			}
			else if (htim->Init.Channel == TIM_CH_B)
			{
				SETB(*(tim->TCCRnC), 6);
			}
			else if (htim->Init.Channel == TIM_CH_C)
			{
				SETB(*(tim->TCCRnC), 5);
			}
		break;
		case TIM_MOD_CTC_FORCE:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(*(tim->TCCRnC), 7);
			}
			else if (htim->Init.Channel == TIM_CH_B)
			{
				SETB(*(tim->TCCRnC), 6);
			}
			else if (htim->Init.Channel == TIM_CH_C)
			{
				SETB(*(tim->TCCRnC), 5);
			}
		break;
		default:
		break;

	}
	
	switch(htim->Init.Output)
	{
		case TIM_PORT_NORMAL:
			if (htim->Init.Channel == TIM_CH_A)
			{
				CLRB(*(tim->TCCRnA), 6);
				CLRB(*(tim->TCCRnA), 7);
			}
			else if(htim->Init.Channel == TIM_CH_B)
			{
				CLRB(*(tim->TCCRnA), 4);
				CLRB(*(tim->TCCRnA), 5);
			}
			else if(htim->Init.Channel == TIM_CH_C)
			{
				CLRB(*(tim->TCCRnA), 2);
				CLRB(*(tim->TCCRnA), 3);
			}
		break;
		case TIM_PORT_TGL_OC:
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(*(tim->TCCRnA), 6);
				CLRB(*(tim->TCCRnA), 7);
			}
			else if(htim->Init.Channel == TIM_CH_B)
			{
				SETB(*(tim->TCCRnA), 4);
				CLRB(*(tim->TCCRnA), 5);
			}
			else if(htim->Init.Channel == TIM_CH_C)
			{
				SETB(*(tim->TCCRnA), 2);
				CLRB(*(tim->TCCRnA), 3);
			}
		break;
		case TIM_PORT_CLR_OC:
			if (htim->Init.Channel == TIM_CH_A)
			{
				CLRB(*(tim->TCCRnA), 6);
				SETB(*(tim->TCCRnA), 7);
			}
			else if(htim->Init.Channel == TIM_CH_B)
			{
				CLRB(*(tim->TCCRnA), 4);
				SETB(*(tim->TCCRnA), 5);
			}
			else if(htim->Init.Channel == TIM_CH_C)
			{
				CLRB(*(tim->TCCRnA), 2);
				SETB(*(tim->TCCRnA), 3);
			}
		break;
		case TIM_PORT_SET_OC:
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(*(tim->TCCRnA), 6);
				SETB(*(tim->TCCRnA), 7);
			}
			else if(htim->Init.Channel == TIM_CH_B)
			{
				SETB(*(tim->TCCRnA), 4);
				SETB(*(tim->TCCRnA), 5);
			}
			else if(htim->Init.Channel == TIM_CH_C)
			{
				SETB(*(tim->TCCRnA), 2);
				SETB(*(tim->TCCRnA), 3);
			}
		break;
		case TIM_PORT_FASTPWM_OCA_OUT:
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(*(tim->TCCRnA), 6);
				CLRB(*(tim->TCCRnA), 7);
			}
			else if(htim->Init.Channel == TIM_CH_B)
			{
				SETB(*(tim->TCCRnA), 4);
				CLRB(*(tim->TCCRnA), 5);
			}
			else if(htim->Init.Channel == TIM_CH_C)
			{
				SETB(*(tim->TCCRnA), 2);
				CLRB(*(tim->TCCRnA), 3);
			}
		break;
		case TIM_PORT_PCPWM_OCA_OUT:
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(*(tim->TCCRnA), 6);
				CLRB(*(tim->TCCRnA), 7);
			}
			else if(htim->Init.Channel == TIM_CH_B)
			{
				SETB(*(tim->TCCRnA), 4);
				CLRB(*(tim->TCCRnA), 5);
			}
			else if(htim->Init.Channel == TIM_CH_C)
			{
				SETB(*(tim->TCCRnA), 2);
				CLRB(*(tim->TCCRnA), 3);
			}
		break;
		case TIM_PORT_FPCPWM_OCA_OUT:
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(*(tim->TCCRnA), 6);
				CLRB(*(tim->TCCRnA), 7);
			}
			else if(htim->Init.Channel == TIM_CH_B)
			{
				SETB(*(tim->TCCRnA), 4);
				CLRB(*(tim->TCCRnA), 5);
			}
			else if(htim->Init.Channel == TIM_CH_C)
			{
				SETB(*(tim->TCCRnA), 2);
				CLRB(*(tim->TCCRnA), 3);
			}
		break;
		case TIM_PORT_OC_NONINV:
			if (htim->Init.Channel == TIM_CH_A)
			{
				CLRB(*(tim->TCCRnA), 6);
				SETB(*(tim->TCCRnA), 7);
			}
			else if(htim->Init.Channel == TIM_CH_B)
			{
				CLRB(*(tim->TCCRnA), 4);
				SETB(*(tim->TCCRnA), 5);
			}
			else if(htim->Init.Channel == TIM_CH_C)
			{
				CLRB(*(tim->TCCRnA), 2);
				SETB(*(tim->TCCRnA), 3);
			}
		break;
		case TIM_PORT_OC_INV:
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(*(tim->TCCRnA), 6);
				SETB(*(tim->TCCRnA), 7);
			}
			else if(htim->Init.Channel == TIM_CH_B)
			{
				SETB(*(tim->TCCRnA), 4);
				SETB(*(tim->TCCRnA), 5);
			}
			else if(htim->Init.Channel == TIM_CH_C)
			{
				SETB(*(tim->TCCRnA), 2);
				SETB(*(tim->TCCRnA), 3);
			}
		break;
		default:
		break;
	}
	return OK;
}


StatusTypeDef TIM_Start(TIM_HandleTypeDef *htim)
{
	
	if (htim->TIMn == TIM0)
	{
		TIM8_TypeDef *tim = &TIM8_descripter[0];
		switch(htim->Init.Prescaler)
		{
			case TIM_CLK_PRESCALER_1:
			CLRB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_8:
			CLRB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_32:
			CLRB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_64:
			SETB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_128:
			SETB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_256:
			SETB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_1024:
			SETB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			default:
			break;
		}
		*(tim->TCNTn) = (uint8_t) htim->Init.Tcnt;
		*(tim->OCRn) = (uint8_t) htim->Init.Ocr;
	}
	else if (htim->TIMn == TIM2)
	{
		TIM8_TypeDef *tim = &TIM8_descripter[1];
		switch(htim->Init.Prescaler)
		{
			case TIM_CLK_PRESCALER_1:
			CLRB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_8:
			CLRB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_64:
			CLRB(*(tim->TCCRn), 2);
			SETB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_256:
			SETB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			CLRB(*(tim->TCCRn), 0);
			break;
			case TIM_CLK_PRESCALER_1024:
			SETB(*(tim->TCCRn), 2);
			CLRB(*(tim->TCCRn), 1);
			SETB(*(tim->TCCRn), 0);
			break;
			default:
			break;
		}
		*(tim->TCNTn) = (uint8_t) htim->Init.Tcnt;
		*(tim->OCRn) = (uint8_t) htim->Init.Ocr;
	}
	else if (htim->TIMn == TIM1 || htim->TIMn == TIM3)
	{
		TIM16_TypeDef *tim = NULL;
		if (htim->TIMn == TIM1)
		{
			tim = &TIM16_descripter[0];
		}
		else if(htim->TIMn == TIM3)
		{
			tim = &TIM16_descripter[1];
		}
		if (tim == NULL)
		{
			return ERROR;
		}
		switch(htim->Init.Prescaler)
		{
			case TIM_CLK_PRESCALER_1:
			CLRB(*(tim->TCCRnB), 2);
			CLRB(*(tim->TCCRnB), 1);
			SETB(*(tim->TCCRnB), 0);
			break;
			case TIM_CLK_PRESCALER_8:
			CLRB(*(tim->TCCRnB), 2);
			SETB(*(tim->TCCRnB), 1);
			CLRB(*(tim->TCCRnB), 0);
			break;
			case TIM_CLK_PRESCALER_64:
			CLRB(*(tim->TCCRnB), 2);
			SETB(*(tim->TCCRnB), 1);
			SETB(*(tim->TCCRnB), 0);
			break;
			case TIM_CLK_PRESCALER_256:
			SETB(*(tim->TCCRnB), 2);
			CLRB(*(tim->TCCRnB), 1);
			CLRB(*(tim->TCCRnB), 0);
			break;
			case TIM_CLK_PRESCALER_1024:
			SETB(*(tim->TCCRnB), 2);
			CLRB(*(tim->TCCRnB), 1);
			SETB(*(tim->TCCRnB), 0);
			break;
			default:
			break;
		}
		*(tim->TCNTnH) = 0xFF & (htim->Init.Tcnt >> 8);
		*(tim->TCNTnL) = 0xFF & (htim->Init.Tcnt);
		*(tim->ICRnH) = 0xFF & (htim->Init.Icr >> 8);
		*(tim->ICRnL) = 0xFF & (htim->Init.Icr);
		
		switch(htim->Init.Channel)
		{
			case TIM_CH_A:
			*(tim->OCRnAH) = 0xFF & (htim->Init.Ocr >> 8);
			*(tim->OCRnAL) = 0xFF & (htim->Init.Ocr);
			break;
			case TIM_CH_B:
			*(tim->OCRnBH) = 0xFF & (htim->Init.Ocr >> 8);
			*(tim->OCRnBL) = 0xFF & (htim->Init.Ocr);
			break;
			default:
			break;
		}
	}
	
	
	return OK;
}

StatusTypeDef TIM_Stop(TIM_HandleTypeDef *htim)
{	
	if (htim->TIMn == TIM0)
	{
		TIM8_TypeDef *tim = &TIM8_descripter[0];
		*(tim->TCCRn) &= ~0x07;
	}
	else if (htim->TIMn == TIM2)
	{
		TIM8_TypeDef *tim = &TIM8_descripter[1];
		*(tim->TCCRn) &= ~0x07;
	}
	else if (htim->TIMn == TIM1)
	{
		TIM16_TypeDef *tim = &TIM16_descripter[0];
		*(tim->TCCRnB) &= ~0x07;
	}
	else if (htim->TIMn == TIM3)
	{
		TIM16_TypeDef *tim = &TIM16_descripter[1];
		*(tim->TCCRnB) &= ~0x07;
	}
	return OK;
}

StatusTypeDef TIM_Start_Normal_IT(TIM_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM0:
		SETB(TIMSK, 0);
		break;
		case TIM2:
		SETB(TIMSK, 6);
		break;
		case TIM1:
		SETB(TIMSK, 2);
		break;
		case TIM3:
		SETB(ETIMSK, 2);
		break;
	}
	
	return OK;
}

StatusTypeDef TIM_Stop_Normal_IT(TIM_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM0:
		CLRB(TIMSK, 0);
		break;
		case TIM2:
		CLRB(TIMSK, 6);
		break;
		case TIM1:
		CLRB(TIMSK, 2);
		break;
		case TIM3:
		CLRB(ETIMSK, 2);
		break;
	}
	return OK;
}

StatusTypeDef TIM_Start_OC_IT(TIM_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM0:
		SETB(TIMSK, 1);
		break;
		case TIM2:
		SETB(TIMSK, 7);
		break;
		case TIM1:
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(TIMSK, 4);
			}
			else if (htim->Init.Channel == TIM_CH_B)
			{
				SETB(TIMSK, 3);
			}
			else if (htim->Init.Channel == TIM_CH_C)
			{
				SETB(ETIMSK, 0);
			}
		break;
		case TIM3:
			if (htim->Init.Channel == TIM_CH_A)
			{
				SETB(ETIMSK, 4);
			}
			else if (htim->Init.Channel == TIM_CH_B)
			{
				SETB(ETIMSK, 3);
			}
			else if (htim->Init.Channel == TIM_CH_C)
			{
				SETB(ETIMSK, 1);
			}
		break;
	}
	
	return OK;
}

StatusTypeDef TIM_Stop_OC_IT(TIM_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM0:
		CLRB(TIMSK, 1);
		break;
		case TIM2:
		CLRB(TIMSK, 7);
		break;
		case TIM1:
		if (htim->Init.Channel == TIM_CH_A)
		{
			CLRB(TIMSK, 4);
		}
		else if (htim->Init.Channel == TIM_CH_B)
		{
			CLRB(TIMSK, 3);
		}
		else if (htim->Init.Channel == TIM_CH_C)
		{
			CLRB(ETIMSK, 0);
		}
		break;
		case TIM3:
		if (htim->Init.Channel == TIM_CH_A)
		{
			CLRB(ETIMSK, 4);
		}
		else if (htim->Init.Channel == TIM_CH_B)
		{
			CLRB(ETIMSK, 3);
		}
		else if (htim->Init.Channel == TIM_CH_C)
		{
			CLRB(ETIMSK, 1);
		}
		break;
	}
	return OK;
}

StatusTypeDef TIM_Start_IC_IT(TIM_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM1:
			SETB(TIMSK, 5);
		break;
		case TIM3:
			SETB(ETIMSK, 5);
		break;
		default:
		break;
	}
	
	return OK;
}

StatusTypeDef TIM_Stop_IC_IT(TIM_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM1:
		CLRB(TIMSK, 5);
		break;
		case TIM3:
		CLRB(ETIMSK, 5);
		break;
		default:
		break;
	}
	return OK;
}



void TIM0_OVF_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM0_OVF_ElapsedCallback(htim);
}

void TIM1_OVF_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM1_OVF_ElapsedCallback(htim);
}

void TIM2_OVF_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM2_OVF_ElapsedCallback(htim);
}

void TIM3_OVF_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM3_OVF_ElapsedCallback(htim);
}

void TIM0_OC_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM0_OC_ElapsedCallback(htim);
}

void TIM2_OC_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM2_OC_ElapsedCallback(htim);
}

void TIM1_OCA_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM1_OCA_ElapsedCallback(htim);
}
void TIM1_OCB_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM1_OCB_ElapsedCallback(htim);
}
void TIM1_OCC_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM1_OCC_ElapsedCallback(htim);
}

void TIM3_OCA_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM3_OCA_ElapsedCallback(htim);
}
void TIM3_OCB_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM3_OCB_ElapsedCallback(htim);
}
void TIM3_OCC_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM3_OCC_ElapsedCallback(htim);
}

__attribute__((weak)) void TIM0_OVF_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}

__attribute__((weak)) void TIM1_OVF_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}

__attribute__((weak)) void TIM2_OVF_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}

__attribute__((weak)) void TIM3_OVF_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}
__attribute__((weak)) void TIM0_OC_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}
__attribute__((weak)) void TIM2_OC_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}

__attribute__((weak)) void TIM1_OCA_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}
__attribute__((weak)) void TIM1_OCB_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}
__attribute__((weak)) void TIM1_OCC_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}

__attribute__((weak)) void TIM3_OCA_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}
__attribute__((weak)) void TIM3_OCB_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}
__attribute__((weak)) void TIM3_OCC_ElapsedCallback(TIM_HandleTypeDef *htim)
{
	return;
}

#endif