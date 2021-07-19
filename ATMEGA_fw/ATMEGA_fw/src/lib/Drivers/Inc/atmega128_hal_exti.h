#ifndef ATMEGA128_HAL_EXTI_H_
#define ATMEGA128_HAL_EXTI_H_

#include "atmega128_hal_def.h"

typedef struct 
{
	uint8_t Line;
	
	uint8_t Trigger;
	
} EXTI_HandleTypedef;


#define EXTI_LINE_0		0
#define EXTI_LINE_1		1
#define EXTI_LINE_2		2
#define EXTI_LINE_3		3
#define EXTI_LINE_4		4
#define EXTI_LINE_5		5
#define EXTI_LINE_6		6
#define EXTI_LINE_7		7


#define EXTI_TRIGGER_LOW		0
#define EXTI_TRIGGER_BOTH		1
#define EXTI_TRIGGER_FALLING	2
#define EXTI_TRIGGER_RISING		3

HAL_StatusTypeDef HAL_EXTI_SetConfig(EXTI_HandleTypedef *hexti);
HAL_StatusTypeDef HAL_EXTI_ClearConfig(EXTI_HandleTypedef *hexti);	

void INT0_Callback(void);
void INT1_Callback(void);
void INT2_Callback(void);
void INT3_Callback(void);
void INT4_Callback(void);
void INT5_Callback(void);
void INT6_Callback(void);
void INT7_Callback(void);

#endif /* ATMEGA128_HAL_EXTI_H_ */