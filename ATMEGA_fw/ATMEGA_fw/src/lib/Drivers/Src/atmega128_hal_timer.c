#include "atmega128_hal.h"

#ifdef HAL_TIMER_MODULE_ENABLED

TIM8_TypeDef TIM8_descripter[] = {
	{&TCCR0, &TCNT0, &OCR0, &ASSR},
	{&TCCR2, &TCNT2, &OCR2, NULL}
};

TIM16_TypeDef TIM16_descripter[] = {
	{&TCCR1A, &TCCR1B, &TCCR1C, &TCNT1L,&TCNT1H, &OCR1AL, &OCR1AH, &OCR1BL, &OCR1BH, &ICR1L, &ICR1H},
	{&TCCR3A, &TCCR3B, &TCCR3C, &TCNT3L,&TCNT3H, &OCR3AL, &OCR3AH, &OCR3BL, &OCR3BH, &ICR3L, &ICR3H}
};




void TIM0_CLK_Configuration(TIM8_HandleTypeDef *htim);
void TIM2_CLK_Configuration(TIM8_HandleTypeDef *htim);
void TIM1_CLK_Configuration(TIM16_HandleTypeDef *htim);
void TIM3_CLK_Configuration(TIM16_HandleTypeDef *htim);


HAL_StatusTypeDef TIM8_Base_Init(TIM8_HandleTypeDef *htim)
{
	TIM8_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM0:
		tim = &TIM8_descripter[0];
		break;
		case TIM2:
		tim = &TIM8_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	htim->Regs = tim;

	if (htim->TIMn == TIM2)
	{
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
	
	return HAL_OK;
}


HAL_StatusTypeDef TIM8_Base_Start(TIM8_HandleTypeDef *htim)
{
	TIM8_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM0:
		tim = &TIM8_descripter[0];
		break;
		case TIM2:
		tim = &TIM8_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	*(tim->TCNTn) = htim->Init.Tcnt;
	
	switch(htim->TIMn)
	{
		case TIM0:
		TIM0_CLK_Configuration(htim);
		break;
		case TIM2:
		TIM2_CLK_Configuration(htim);
		break;
		default:
		break;
	}
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_Base_Stop(TIM8_HandleTypeDef *htim)
{
	TIM8_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM0:
		tim = &TIM8_descripter[0];
		break;
		case TIM2:
		tim = &TIM8_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	*(tim->TCCRn) &= ~0x07;
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_OC_Init(TIM8_HandleTypeDef *htim, TIM8_OC_InitTypeDef *htim_oc)
{
	TIM8_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM0:
		tim = &TIM8_descripter[0];
		break;
		case TIM2:
		tim = &TIM8_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	
	TIM8_Base_Init(htim);
	switch(htim_oc->OCMode)
	{
		case TIM8_OC_MOD_NORMAL:
		CLRB((*tim->TCCRn), 6);
		CLRB((*tim->TCCRn), 3);
		break;
		case TIM8_OC_MOD_CTC:
		CLRB((*tim->TCCRn), 6);
		SETB((*tim->TCCRn), 3);
		break;
		default:
		break;
	}
	
	switch(htim_oc->OCOutput)
	{
		case TIM8_OC_NORMALOUT:
		CLRB((*tim->TCCRn), 7);
		break;
		case TIM8_OC_FORCEDOUT:
		SETB((*tim->TCCRn), 7);
		break;
		default:
		break;
	}
	
	switch(htim_oc->OCWave_COM)
	{
		case TIM8_OC_COM_NORMAL:
		CLRB((*tim->TCCRn), 4);
		CLRB((*tim->TCCRn), 5);
		break;
		case TIM8_OC_COM_TOGGLE:
		SETB((*tim->TCCRn), 4);
		CLRB((*tim->TCCRn), 5);
		break;
		case TIM8_OC_COM_CLR:
		CLRB((*tim->TCCRn), 4);
		SETB((*tim->TCCRn), 5);
		break;
		case TIM8_OC_COM_SET:
		SETB((*tim->TCCRn), 4);
		SETB((*tim->TCCRn), 5);
		break;
		default:
		break;
	}
	
	(*tim->OCRn) = htim_oc->Ocr;
	(*tim->TCNTn) = htim_oc->Tcnt;
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_OC_Start(TIM8_HandleTypeDef *htim)
{
	TIM8_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM0:
		tim = &TIM8_descripter[0];
		break;
		case TIM2:
		tim = &TIM8_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	*(tim->TCNTn) = htim->Init.Tcnt;
	
	switch(htim->TIMn)
	{
		case TIM0:
		TIM0_CLK_Configuration(htim);
		break;
		case TIM2:
		TIM2_CLK_Configuration(htim);
		break;
		default:
		break;
	}
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_OC_Stop(TIM8_HandleTypeDef *htim)
{
	TIM8_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM0:
		tim = &TIM8_descripter[0];
		break;
		case TIM2:
		tim = &TIM8_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	*(tim->TCCRn) &= ~0x07;
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_PWM_Init(TIM8_HandleTypeDef *htim, TIM8_PWM_InitTypeDef *htim_pwm)
{
	TIM8_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM0:
		tim = &TIM8_descripter[0];
		break;
		case TIM2:
		tim = &TIM8_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	TIM8_Base_Init(htim);
	
	switch(htim_pwm->PWMMode)
	{
		case TIM8_PWM_MOD_FASTPWM:
		SETB(*(tim->TCCRn), 3);
		SETB(*(tim->TCCRn), 6);
		break;
		case TIM8_PWM_MOD_PCPWM:
		SETB(*(tim->TCCRn), 3);
		CLRB(*(tim->TCCRn), 6);
		break;
		default:
		break;
	}
	
	switch(htim_pwm->PWMWave_COM)
	{
		case TIM8_PWM_COM_NORMAL:
		CLRB((*tim->TCCRn), 4);
		CLRB((*tim->TCCRn), 5);
		break;
		case TIM8_PWM_COM_NONINV:
		CLRB((*tim->TCCRn), 4);
		SETB((*tim->TCCRn), 5);
		break;
		case TIM8_PWM_COM_INV:
		SETB((*tim->TCCRn), 4);
		SETB((*tim->TCCRn), 5);
		break;
		default:
		break;
	}
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_PWM_Start(TIM8_HandleTypeDef *htim)
{
	TIM8_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM0:
		tim = &TIM8_descripter[0];
		break;
		case TIM2:
		tim = &TIM8_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	switch(htim->TIMn)
	{
		case TIM0:
		TIM0_CLK_Configuration(htim);
		break;
		case TIM2:
		TIM2_CLK_Configuration(htim);
		break;
		default:
		break;
	}
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_PWM_Stop(TIM8_HandleTypeDef *htim)
{
	TIM8_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM0:
		tim = &TIM8_descripter[0];
		break;
		case TIM2:
		tim = &TIM8_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	*(tim->TCCRn) &= ~0x07;
	return HAL_OK;
}


HAL_StatusTypeDef TIM8_Base_Start_IT(TIM8_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM0:
		SETB(TIMSK, 0);
		break;
		case TIM2:
		SETB(TIMSK, 6);
		break;
		default:
		break;
	}
	TIM8_Base_Start(htim);
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_Base_Stop_IT(TIM8_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM0:
		CLRB(TIMSK, 0);
		break;
		case TIM2:
		CLRB(TIMSK, 6);
		break;
		default:
		break;
	}
	TIM8_Base_Stop(htim);
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_OC_Start_IT(TIM8_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM0:
		SETB(TIMSK, 1);
		break;
		case TIM2:
		SETB(TIMSK, 7);
		break;
		default:
		break;
	}
	TIM8_OC_Start(htim);
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM8_OC_Stop_IT(TIM8_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM0:
		CLRB(TIMSK, 1);
		break;
		case TIM2:
		CLRB(TIMSK, 7);
		break;
		default:
		break;
	}
	TIM8_OC_Stop(htim);
	
	return HAL_OK;
}



HAL_StatusTypeDef TIM16_Base_Init(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	htim->Regs = tim;
	
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
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_Base_Start(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	*(tim->TCNTnH) = (htim->Init.Tcnt >> 8) & 0xFF;
	*(tim->TCNTnL) = htim->Init.Tcnt & 0xFF;
	
	switch(htim->TIMn)
	{
		case TIM1:
		TIM1_CLK_Configuration(htim);
		break;
		case TIM3:
		TIM3_CLK_Configuration(htim);
		break;
		default:
		break;
	}
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_Base_Stop(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	*(tim->TCCRnB) &= ~0x07;
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_OC_Init(TIM16_HandleTypeDef *htim, TIM16_OC_InitTypeDef *htim_oc)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	TIM16_Base_Init(htim);
	
	switch(htim_oc->OCMode)
	{
		case TIM16_OC_MOD_NORMAL:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM16_OC_MOD_CTC_ICR:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM16_OC_MOD_CTC_OCR:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		default:
		break;
	}

	
	if (htim->Init.Channel == TIM16_CHANNEL_A)
	{
		switch(htim_oc->OCOutput)
		{
			case TIM16_OC_NORMALOUT:
			CLRB(*(tim->TCCRnC), 7);
			break;
			case TIM16_OC_FORCEDOUT:
			SETB(*(tim->TCCRnC), 7);
			break;
			default:
			break;
		}
		
		switch(htim_oc->OCWave_COM)
		{
			case TIM16_OC_COM_NORMAL:
			CLRB(*(tim->TCCRnA), 6);
			CLRB(*(tim->TCCRnA), 7);
			break;
			case TIM16_OC_COM_TOGGLE:
			SETB(*(tim->TCCRnA), 6);
			CLRB(*(tim->TCCRnA), 7);
			break;
			case TIM16_OC_COM_CLR:
			CLRB(*(tim->TCCRnA), 6);
			SETB(*(tim->TCCRnA), 7);
			break;
			case TIM16_OC_COM_SET:
			SETB(*(tim->TCCRnA), 6);
			SETB(*(tim->TCCRnA), 7);
			break;
			default:
			break;
		}
		
		*(tim->OCRnAH) = (htim_oc->Ocr >> 8) & 0xFF;
		*(tim->OCRnAL) = htim_oc->Ocr & 0xFF;
	}
	else if(htim->Init.Channel == TIM16_CHANNEL_B)
	{
		switch(htim_oc->OCOutput)
		{
			case TIM16_OC_NORMALOUT:
			CLRB(*(tim->TCCRnC), 6);
			break;
			case TIM16_OC_FORCEDOUT:
			SETB(*(tim->TCCRnC), 6);
			break;
			default:
			break;
		}
		
		switch(htim_oc->OCWave_COM)
		{
			case TIM16_OC_COM_NORMAL:
			CLRB(*(tim->TCCRnA), 4);
			CLRB(*(tim->TCCRnA), 5);
			break;
			case TIM16_OC_COM_TOGGLE:
			SETB(*(tim->TCCRnA), 4);
			CLRB(*(tim->TCCRnA), 5);
			break;
			case TIM16_OC_COM_CLR:
			CLRB(*(tim->TCCRnA), 4);
			SETB(*(tim->TCCRnA), 5);
			break;
			case TIM16_OC_COM_SET:
			SETB(*(tim->TCCRnA), 4);
			SETB(*(tim->TCCRnA), 5);
			break;
			default:
			break;
		}
		*(tim->OCRnBH) = (htim_oc->Ocr >> 8) & 0xFF;
		*(tim->OCRnBL) = htim_oc->Ocr & 0xFF;
	}
	else if(htim->Init.Channel == TIM16_CHANNEL_C)
	{
		switch(htim_oc->OCOutput)
		{
			case TIM16_OC_NORMALOUT:
			CLRB(*(tim->TCCRnC), 5);
			break;
			case TIM16_OC_FORCEDOUT:
			SETB(*(tim->TCCRnC), 5);
			break;
			default:
			break;
		}
		
		switch(htim_oc->OCWave_COM)
		{
			case TIM16_OC_COM_NORMAL:
			CLRB(*(tim->TCCRnA), 2);
			CLRB(*(tim->TCCRnA), 3);
			break;
			case TIM16_OC_COM_TOGGLE:
			SETB(*(tim->TCCRnA), 2);
			CLRB(*(tim->TCCRnA), 3);
			break;
			case TIM16_OC_COM_CLR:
			CLRB(*(tim->TCCRnA), 2);
			SETB(*(tim->TCCRnA), 3);
			break;
			case TIM16_OC_COM_SET:
			SETB(*(tim->TCCRnA), 2);
			SETB(*(tim->TCCRnA), 3);
			break;
			default:
			break;
		}
	}
	
	*(tim->TCNTnH) = (htim_oc->Tcnt >> 8) & 0xFF;
	*(tim->TCNTnL) = htim_oc->Tcnt & 0xFF;
	*(tim->ICRnH) = (htim_oc->Icr >> 8) & 0xFF;
	*(tim->ICRnL) = htim_oc->Icr & 0xFF;
	
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_OC_Start(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	
	switch(htim->TIMn)
	{
		case TIM1:
		TIM1_CLK_Configuration(htim);
		break;
		case TIM3:
		TIM3_CLK_Configuration(htim);
		break;
		default:
		break;
	}
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_OC_Stop(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	*(tim->TCCRnB) &= ~0x07;
	return HAL_OK;	
}

HAL_StatusTypeDef TIM16_PWM_Init(TIM16_HandleTypeDef *htim, TIM16_PWM_InitTypeDef *htim_pwm)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	TIM16_Base_Init(htim);
	
	switch(htim_pwm->PWMMode)
	{
		case TIM16_PWM_MOD_FASTPWM_ICR:
		CLRB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_FASTPWM_OCR:
		SETB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_PCPWM_ICR:
		CLRB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_PCPWM_OCR:
		SETB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_PFCPWM_ICR:
		CLRB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_PFCPWM_OCR:
		SETB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		SETB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_FASTPWM_8:
		SETB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_FASTPWM_9:
		CLRB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_FASTPWM_10:
		SETB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		SETB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_PCPWM_8:
		SETB(*(tim->TCCRnA), 0);
		CLRB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_PCPWM_9:
		CLRB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		case TIM16_PWM_MOD_PCPWM_10:
		SETB(*(tim->TCCRnA), 0);
		SETB(*(tim->TCCRnA), 1);
		CLRB(*(tim->TCCRnB), 3);
		CLRB(*(tim->TCCRnB), 4);
		break;
		default:
		break;
	}
	if (htim->Init.Channel == TIM16_CHANNEL_A)
	{
		switch(htim_pwm->PWMWave_COM)
		{
			case TIM16_PWM_COM_NORMAL:
			CLRB(*(tim->TCCRnA), 6);
			CLRB(*(tim->TCCRnA), 7);
			break;
			case TIM16_PWM_COM_TOGGLE:
			SETB(*(tim->TCCRnA), 6);
			CLRB(*(tim->TCCRnA), 7);
			break;
			case TIM16_PWM_COM_NONINV:
			CLRB(*(tim->TCCRnA), 6);
			SETB(*(tim->TCCRnA), 7);
			break;
			case TIM16_PWM_COM_INV:
			SETB(*(tim->TCCRnA), 6);
			SETB(*(tim->TCCRnA), 7);
			break;
			default:
			break;
		}
	}
	else if (htim->Init.Channel == TIM16_CHANNEL_B)
	{
		switch(htim_pwm->PWMWave_COM)
		{
			case TIM16_PWM_COM_NORMAL:
			CLRB(*(tim->TCCRnA), 4);
			CLRB(*(tim->TCCRnA), 5);
			break;
			case TIM16_PWM_COM_TOGGLE:
			SETB(*(tim->TCCRnA), 4);
			CLRB(*(tim->TCCRnA), 5);
			break;
			case TIM16_PWM_COM_NONINV:
			CLRB(*(tim->TCCRnA), 4);
			SETB(*(tim->TCCRnA), 5);
			break;
			case TIM16_PWM_COM_INV:
			SETB(*(tim->TCCRnA), 4);
			SETB(*(tim->TCCRnA), 5);
			break;
			default:
			break;
		}
	}
	else if (htim->Init.Channel == TIM16_CHANNEL_C)
	{
		switch(htim_pwm->PWMWave_COM)
		{
			case TIM16_PWM_COM_NORMAL:
			CLRB(*(tim->TCCRnA), 2);
			CLRB(*(tim->TCCRnA), 3);
			break;
			case TIM16_PWM_COM_TOGGLE:
			SETB(*(tim->TCCRnA), 2);
			CLRB(*(tim->TCCRnA), 3);
			break;
			case TIM16_PWM_COM_NONINV:
			CLRB(*(tim->TCCRnA), 2);
			SETB(*(tim->TCCRnA), 3);
			break;
			case TIM16_PWM_COM_INV:
			SETB(*(tim->TCCRnA), 2);
			SETB(*(tim->TCCRnA), 3);
			break;
			default:
			break;
		}
	}
	
	
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_PWM_Start(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	
	switch(htim->TIMn)
	{
		case TIM1:
		TIM1_CLK_Configuration(htim);
		break;
		case TIM3:
		TIM3_CLK_Configuration(htim);
		break;
		default:
		break;
	}
	
	return HAL_OK;
}
HAL_StatusTypeDef TIM16_PWM_Stop(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	*(tim->TCCRnB) &= ~0x07;
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_IC_Init(TIM16_HandleTypeDef *htim, TIM16_IC_InitTypeDef *htim_ic)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}
HAL_StatusTypeDef TIM16_IC_Start(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}
HAL_StatusTypeDef TIM16_IC_Stop(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = NULL;
	switch(htim->TIMn)
	{
		case TIM1:
		tim = &TIM16_descripter[0];
		break;
		case TIM3:
		tim = &TIM16_descripter[1];
		break;
		default:
		break;
	}
	
	if (tim == NULL)
	{
		return HAL_ERROR;
	}
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_Base_Start_IT(TIM16_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM1:
		SETB(TIMSK, 2);
		break;
		case TIM3:
		SETB(ETIMSK, 2);
		break;
		default:
		break;
	}
	TIM16_Base_Start(htim);
	
	return HAL_OK;
}
HAL_StatusTypeDef TIM16_Base_Stop_IT(TIM16_HandleTypeDef *htim)
{
	switch(htim->TIMn)
	{
		case TIM1:
		CLRB(TIMSK, 2);
		break;
		case TIM3:
		CLRB(ETIMSK, 2);
		break;
		default:
		break;
	}
	TIM16_Base_Stop(htim);
	return HAL_OK;
}
HAL_StatusTypeDef TIM16_OC_Start_IT(TIM16_HandleTypeDef *htim)
{
	if (htim->Init.Channel == TIM16_CHANNEL_A)
	{
		switch(htim->TIMn)
		{
			case TIM1:
			SETB(TIMSK, 4);
			break;
			case TIM3:
			SETB(ETIMSK, 4);
			break;
			default:
			break;
		}
	}
	else if (htim->Init.Channel == TIM16_CHANNEL_B)
	{
		switch(htim->TIMn)
		{
			case TIM1:
			SETB(TIMSK, 3);
			break;
			case TIM3:
			SETB(ETIMSK, 3);
			break;
			default:
			break;
		}
	}
	else if (htim->Init.Channel == TIM16_CHANNEL_C)
	{
		switch(htim->TIMn)
		{
			case TIM1:
			SETB(ETIMSK, 0);
			break;
			case TIM3:
			SETB(ETIMSK, 1);
			break;
			default:
			break;
		}
	}
	TIM16_OC_Start(htim);
	
	return HAL_OK;
}
HAL_StatusTypeDef TIM16_OC_Stop_IT(TIM16_HandleTypeDef *htim)
{
	if (htim->Init.Channel == TIM16_CHANNEL_A)
	{
		switch(htim->TIMn)
		{
			case TIM1:
			CLRB(TIMSK, 4);
			break;
			case TIM3:
			CLRB(ETIMSK, 4);
			break;
			default:
			break;
		}
	}
	else if (htim->Init.Channel == TIM16_CHANNEL_B)
	{
		switch(htim->TIMn)
		{
			case TIM1:
			CLRB(TIMSK, 3);
			break;
			case TIM3:
			CLRB(ETIMSK, 3);
			break;
			default:
			break;
		}
	}
	else if (htim->Init.Channel == TIM16_CHANNEL_C)
	{
		switch(htim->TIMn)
		{
			case TIM1:
			CLRB(ETIMSK, 0);
			break;
			case TIM3:
			CLRB(ETIMSK, 1);
			break;
			default:
			break;
		}
	}
	TIM16_OC_Stop(htim);
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_IC_Start_IT(TIM16_HandleTypeDef *htim)
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
	TIM16_IC_Start(htim);
	return HAL_OK;
}

HAL_StatusTypeDef TIM16_IC_Stop_IT(TIM16_HandleTypeDef *htim)
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
	TIM16_IC_Stop(htim);
	return HAL_OK;
}


void TIM0_CLK_Configuration(TIM8_HandleTypeDef *htim)
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
}

void TIM2_CLK_Configuration(TIM8_HandleTypeDef *htim)
{
	TIM8_TypeDef *tim = &TIM8_descripter[1];
	
	if ((*(tim->TCCRn) & 0x03) == 0x06 || (*(tim->TCCRn) & 0x03) == 0x07)
		return;
	
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
}

void TIM1_CLK_Configuration(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = &TIM16_descripter[0];
	
	if ((*(tim->TCCRnB) & 0x03) == 0x06 || (*(tim->TCCRnB) & 0x03) == 0x07)
		return;
	
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
}

void TIM3_CLK_Configuration(TIM16_HandleTypeDef *htim)
{
	TIM16_TypeDef *tim = &TIM16_descripter[1];
	
	if ((*(tim->TCCRnB) & 0x03) == 0x06 || (*(tim->TCCRnB) & 0x03) == 0x07)
		return;
	
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
}


void TIM0_OVF_IRQHandler(TIM8_HandleTypeDef *htim)
{
	TIM0_OVF_ElapsedCallback(htim);
}
void TIM1_OVF_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM1_OVF_ElapsedCallback(htim);
}
void TIM2_OVF_IRQHandler(TIM8_HandleTypeDef *htim)
{
	TIM2_OVF_ElapsedCallback(htim);
}
void TIM3_OVF_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM3_OVF_ElapsedCallback(htim);
}

void TIM0_OC_IRQHandler(TIM8_HandleTypeDef *htim)
{
	TIM0_OC_ElapsedCallback(htim);	
}

void TIM2_OC_IRQHandler(TIM8_HandleTypeDef *htim)
{
	TIM2_OC_ElapsedCallback(htim);
}

void TIM1_OCA_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM1_OCA_ElapsedCallback(htim);
}

void TIM1_OCB_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM1_OCB_ElapsedCallback(htim);
}

void TIM1_OCC_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM1_OCC_ElapsedCallback(htim);
}

void TIM1_CAPT_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM1_CAPT_CpltCallback(htim);
}

void TIM3_OCA_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM3_OCA_ElapsedCallback(htim);	
}

void TIM3_OCB_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM3_OCB_ElapsedCallback(htim);
}

void TIM3_OCC_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM3_OCC_ElapsedCallback(htim);
}

void TIM3_CAPT_IRQHandler(TIM16_HandleTypeDef *htim)
{
	TIM3_CAPT_CpltCallback(htim);
}

__attribute__((weak)) void TIM0_OVF_ElapsedCallback(TIM8_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM1_OVF_ElapsedCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM2_OVF_ElapsedCallback(TIM8_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM3_OVF_ElapsedCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM0_OC_ElapsedCallback(TIM8_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM2_OC_ElapsedCallback(TIM8_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM1_OCA_ElapsedCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM1_OCB_ElapsedCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM1_OCC_ElapsedCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM1_CAPT_CpltCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM3_OCA_ElapsedCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM3_OCB_ElapsedCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM3_OCC_ElapsedCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}

__attribute__((weak)) void TIM3_CAPT_CpltCallback(TIM16_HandleTypeDef *htim)
{
	UNUSED(htim);
}


#endif