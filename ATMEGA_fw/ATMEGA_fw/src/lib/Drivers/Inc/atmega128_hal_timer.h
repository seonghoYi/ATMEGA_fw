#ifndef ATMEGA128_HAL_TIMER_H_
#define ATMEGA128_HAL_TIMER_H_

#include "atmega128_hal_def.h"

typedef struct
{
	uint8_t		Source;
	uint8_t		Prescaler;
	uint8_t		Tcnt;
	
} TIM8_Base_InitTypeDef;

typedef struct
{
	uint8_t		OCMode;
	uint8_t		OCOutput;
	uint8_t		OCWave_COM;
	uint8_t		Tcnt;
	uint8_t		Ocr;
	
} TIM8_OC_InitTypeDef;


typedef struct
{
	uint8_t		PWMMode;
	uint8_t		PWMWave_COM;
	uint8_t		Tcnt;
	uint8_t		Ocr;
	
} TIM8_PWM_InitTypeDef;

typedef struct
{
	uint8_t		Source;
	uint8_t		Prescaler;
	uint8_t		Channel;
	uint16_t	Tcnt;
	
} TIM16_Base_InitTypeDef;

typedef struct
{
	uint8_t		OCMode;
	uint8_t		OCOutput;
	uint8_t		OCWave_COM;
	uint16_t	Tcnt;
	uint16_t	Ocr;
	uint16_t	Icr;
	
} TIM16_OC_InitTypeDef;

typedef struct
{
	uint8_t		ICPolarity;
	uint8_t		ICFilter;
	uint16_t	Tcnt;
	uint16_t	Ocr;
	uint16_t	Icr;
	
} TIM16_IC_InitTypeDef;

typedef struct
{
	uint8_t		PWMMode;
	uint8_t		PWMWave_COM;
	uint16_t	Tcnt;
	uint16_t	Ocr;
	uint16_t	Icr;
	
} TIM16_PWM_InitTypeDef;



typedef enum
{
	TIM_STATE_RESET				= 0x00,    
	TIM_STATE_READY             = 0x01,    
	TIM_STATE_BUSY              = 0x02,    
	TIM_STATE_TIMEOUT           = 0x03,    
	TIM_STATE_ERROR             = 0x04     
} TIM_StateTypeDef;

typedef struct
{
	uint8_t TIMn;
	
	TIM8_Base_InitTypeDef Init;
	TIM8_TypeDef *Regs;
	
	uint8_t	ErrorCode;
	
} TIM8_HandleTypeDef;

typedef struct
{
	uint8_t TIMn;
	
	TIM16_Base_InitTypeDef Init;
	TIM16_TypeDef *Regs;
	
	uint8_t	ErrorCode;
	
} TIM16_HandleTypeDef;


#define TIM_INTCLK_SOURCE			0
#define TIM_EXTCLK_FALLING			1
#define TIM_EXTCLK_RISING			2

#define TIM_CLK_PRESCALER_1			0
#define TIM_CLK_PRESCALER_8			1
#define TIM_CLK_PRESCALER_32		2 //only timer0
#define TIM_CLK_PRESCALER_64		3
#define TIM_CLK_PRESCALER_128		4 //only timer0
#define TIM_CLK_PRESCALER_256		5
#define TIM_CLK_PRESCALER_1024		6

#define TIM8_OC_NORMALOUT			0
#define TIM8_OC_FORCEDOUT			1

#define TIM8_OC_MOD_NORMAL			0
#define TIM8_OC_MOD_CTC				1

#define TIM8_OC_COM_NORMAL			0
#define TIM8_OC_COM_TOGGLE			1
#define TIM8_OC_COM_CLR				2
#define TIM8_OC_COM_SET				3

#define TIM8_PWM_MOD_FASTPWM		0
#define TIM8_PWM_MOD_PCPWM			1

#define TIM8_PWM_COM_NORMAL			0
#define TIM8_PWM_COM_NONINV			1
#define TIM8_PWM_COM_INV			2


#define TIM16_CHANNEL_A				1
#define TIM16_CHANNEL_B				2
#define TIM16_CHANNEL_C				4				

#define TIM16_OC_NORMALOUT			0
#define TIM16_OC_FORCEDOUT			1

#define TIM16_OC_MOD_NORMAL			0
#define TIM16_OC_MOD_CTC_OCR		1
#define TIM16_OC_MOD_CTC_ICR		2

#define TIM16_OC_COM_NORMAL			0
#define TIM16_OC_COM_TOGGLE			1
#define TIM16_OC_COM_CLR			2
#define TIM16_OC_COM_SET			3

#define TIM16_PWM_MOD_FASTPWM_OCR	0
#define TIM16_PWM_MOD_FASTPWM_ICR	1
#define TIM16_PWM_MOD_PCPWM_OCR		2
#define TIM16_PWM_MOD_PCPWM_ICR		3
#define TIM16_PWM_MOD_PFCPWM_OCR	4
#define TIM16_PWM_MOD_PFCPWM_ICR	5
#define TIM16_PWM_MOD_FASTPWM_8		6
#define TIM16_PWM_MOD_FASTPWM_9		7
#define TIM16_PWM_MOD_FASTPWM_10	8
#define TIM16_PWM_MOD_PCPWM_8		9
#define TIM16_PWM_MOD_PCPWM_9		10
#define TIM16_PWM_MOD_PCPWM_10		11

#define TIM16_PWM_COM_NORMAL		0
#define TIM16_PWM_COM_TOGGLE		1
#define TIM16_PWM_COM_NONINV		2
#define TIM16_PWM_COM_INV			3

#define TIM16_IC_USE_ENABLE_NC		0
#define TIM16_IC_USE_DISABLE_NC		1
#define TIM16_IC_CAPT_FALLING		0
#define TIM16_IC_CAPT_RISING		1



HAL_StatusTypeDef TIM8_Base_Init(TIM8_HandleTypeDef *htim);
HAL_StatusTypeDef TIM8_Base_Start(TIM8_HandleTypeDef *htim);
HAL_StatusTypeDef TIM8_Base_Stop(TIM8_HandleTypeDef *htim);
HAL_StatusTypeDef TIM8_OC_Init(TIM8_HandleTypeDef *htim, TIM8_OC_InitTypeDef *htim_oc);
HAL_StatusTypeDef TIM8_OC_Start(TIM8_HandleTypeDef *htim);
HAL_StatusTypeDef TIM8_OC_Stop(TIM8_HandleTypeDef *htim);
HAL_StatusTypeDef TIM8_PWM_Init(TIM8_HandleTypeDef *htim, TIM8_PWM_InitTypeDef *htim_pwm);
HAL_StatusTypeDef TIM8_PWM_Start(TIM8_HandleTypeDef *htim);
HAL_StatusTypeDef TIM8_PWM_Stop(TIM8_HandleTypeDef *htim);

HAL_StatusTypeDef TIM8_Base_Start_IT(TIM8_HandleTypeDef *htim);
HAL_StatusTypeDef TIM8_Base_Stop_IT(TIM8_HandleTypeDef *htim);
HAL_StatusTypeDef TIM8_OC_Start_IT(TIM8_HandleTypeDef *htim);
HAL_StatusTypeDef TIM8_OC_Start_IT(TIM8_HandleTypeDef *htim);


HAL_StatusTypeDef TIM16_Base_Init(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_Base_Start(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_Base_Stop(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_OC_Init(TIM16_HandleTypeDef *htim, TIM16_OC_InitTypeDef *htim_oc);
HAL_StatusTypeDef TIM16_OC_Start(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_OC_Stop(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_PWM_Init(TIM16_HandleTypeDef *htim, TIM16_PWM_InitTypeDef *htim_pwm);
HAL_StatusTypeDef TIM16_PWM_Start(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_PWM_Stop(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_IC_Init(TIM16_HandleTypeDef *htim, TIM16_IC_InitTypeDef *htim_ic);
HAL_StatusTypeDef TIM16_IC_Start(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_IC_Stop(TIM16_HandleTypeDef *htim);

HAL_StatusTypeDef TIM16_Base_Start_IT(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_Base_Stop_IT(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_OC_Start_IT(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_OC_Start_IT(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_IC_Start_IT(TIM16_HandleTypeDef *htim);
HAL_StatusTypeDef TIM16_IC_Stop_IT(TIM16_HandleTypeDef *htim);


void TIM0_OVF_IRQHandler(TIM8_HandleTypeDef *htim);
void TIM1_OVF_IRQHandler(TIM16_HandleTypeDef *htim);
void TIM2_OVF_IRQHandler(TIM8_HandleTypeDef *htim);
void TIM3_OVF_IRQHandler(TIM16_HandleTypeDef *htim);

void TIM0_OC_IRQHandler(TIM8_HandleTypeDef *htim);
void TIM2_OC_IRQHandler(TIM8_HandleTypeDef *htim);
void TIM1_OCA_IRQHandler(TIM16_HandleTypeDef *htim);
void TIM1_OCB_IRQHandler(TIM16_HandleTypeDef *htim);
void TIM1_OCC_IRQHandler(TIM16_HandleTypeDef *htim);
void TIM1_CAPT_IRQHandler(TIM16_HandleTypeDef *htim);
void TIM3_OCA_IRQHandler(TIM16_HandleTypeDef *htim);
void TIM3_OCB_IRQHandler(TIM16_HandleTypeDef *htim);
void TIM3_OCC_IRQHandler(TIM16_HandleTypeDef *htim);
void TIM3_CAPT_IRQHandler(TIM16_HandleTypeDef *htim);

void TIM0_OVF_ElapsedCallback(TIM8_HandleTypeDef *htim);
void TIM1_OVF_ElapsedCallback(TIM16_HandleTypeDef *htim);
void TIM2_OVF_ElapsedCallback(TIM8_HandleTypeDef *htim);
void TIM3_OVF_ElapsedCallback(TIM16_HandleTypeDef *htim);

void TIM0_OC_ElapsedCallback(TIM8_HandleTypeDef *htim);
void TIM2_OC_ElapsedCallback(TIM8_HandleTypeDef *htim);
void TIM1_OCA_ElapsedCallback(TIM16_HandleTypeDef *htim);
void TIM1_OCB_ElapsedCallback(TIM16_HandleTypeDef *htim);
void TIM1_OCC_ElapsedCallback(TIM16_HandleTypeDef *htim);
void TIM1_CAPT_CpltCallback(TIM16_HandleTypeDef *htim);
void TIM3_OCA_ElapsedCallback(TIM16_HandleTypeDef *htim);
void TIM3_OCB_ElapsedCallback(TIM16_HandleTypeDef *htim);
void TIM3_OCC_ElapsedCallback(TIM16_HandleTypeDef *htim);
void TIM3_CAPT_CpltCallback(TIM16_HandleTypeDef *htim);
#endif /* ATMEGA128_HAL_TIMER_H_ */