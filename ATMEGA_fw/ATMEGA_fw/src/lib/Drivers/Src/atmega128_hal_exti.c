#include "atmega128_hal.h"

#ifdef HAL_EXTI_MODULE_ENABLED

HAL_StatusTypeDef HAL_EXTI_SetConfig(EXTI_HandleTypedef *hexti)
{
	if (hexti == NULL)
	{
		return HAL_ERROR;
	}
	

	switch (hexti->Line)
	{
		case EXTI_LINE_0:
		case EXTI_LINE_1:
		case EXTI_LINE_2:
		case EXTI_LINE_3:
		if (hexti->Trigger == EXTI_TRIGGER_BOTH)
		{
			return HAL_ERROR;
		}
		EICRA |= hexti->Trigger << hexti->Line * 2;
		break;
		case EXTI_LINE_4:
		case EXTI_LINE_5:
		case EXTI_LINE_6:
		case EXTI_LINE_7:
		EICRB |= hexti->Trigger << (hexti->Line - 4) * 2;
		break;
		default:
		break;
	}
	
	SETB(EIMSK, hexti->Line);
	
	return HAL_OK;
}

HAL_StatusTypeDef HAL_EXTI_ClearConfig(EXTI_HandleTypedef *hexti)
{
	if (hexti == NULL)
	{
		return HAL_ERROR;
	}
	
	CLRB(EIMSK, hexti->Line);
	return HAL_OK;
}



__attribute__((weak)) void INT0_Callback(void)
{
	UNUSED(NULL);
}

__attribute__((weak)) void INT1_Callback(void)
{
	UNUSED(NULL);
}

__attribute__((weak)) void INT2_Callback(void)
{
	UNUSED(NULL);
}

__attribute__((weak)) void INT3_Callback(void)
{
	UNUSED(NULL);
}

__attribute__((weak)) void INT4_Callback(void)
{
	UNUSED(NULL);
}

__attribute__((weak)) void INT5_Callback(void)
{
	UNUSED(NULL);
}

__attribute__((weak)) void INT6_Callback(void)
{
	UNUSED(NULL);
}

__attribute__((weak)) void INT7_Callback(void)
{
	UNUSED(NULL);
}


#endif