#ifndef ATMEGA128_DRV_TIMER_H_
#define ATMEGA128_DRV_TIMER_H_

#include "atmega128_DRV_def.h"

typedef struct
{
	uint8_t		Source;
	uint8_t		Prescaler;
	uint8_t		CountMode;
	uint8_t		InputCapture;
	uint8_t		Output;
	uint8_t		Channel;
	uint16_t	Tcnt;
	uint16_t	Ocr;
	uint16_t	Icr;

} TIM_InitTypeDef;



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
	
	TIM_InitTypeDef Init;

	uint8_t	ErrorCode;
	
} TIM_HandleTypeDef;





#define TIM_CLK_PRESCALER_1		0
#define TIM_CLK_PRESCALER_8		1
#define TIM_CLK_PRESCALER_32	2 //only timer0
#define TIM_CLK_PRESCALER_64	3
#define TIM_CLK_PRESCALER_128	4 //only timer0
#define TIM_CLK_PRESCALER_256	5
#define TIM_CLK_PRESCALER_1024	6

#define TIM_INTCLK_SOURCE		0
#define TIM_EXTCLK_FALLING		1
#define TIM_EXTCLK_RISING		2

#define TIM_MOD_NORMAL			0
#define TIM_MOD_CTC				1
#define TIM_MOD_FASTPWM			2
#define TIM_MOD_PCPWM			3
	
#define TIM_MOD_NORMAL_FORCE	4
#define TIM_MOD_CTC_FORCE		5

#define TIM_MOD_CTC_OCR			1
#define TIM_MOD_FASTPWM_OCR		2
#define TIM_MOD_PCPWM_OCR		3

#define TIM_MOD_FPCPWM_OCR		8
#define TIM_MOD_FPCPWM_ICR		9
#define	TIM_MOD_PCPWM_ICR		10
#define TIM_MOD_CTC_ICR			11
#define TIM_MOD_FASTPWM_ICR		12


#define TIM_MOD_FASTPWM_8B		13
#define TIM_MOD_FASTPWM_9B		14
#define TIM_MOD_FASTPWM_10B		15

#define TIM_MOD_PCPWM_8B		16
#define TIM_MOD_PCPWM_9B		17
#define TIM_MOD_PCPWM_10B		18

	

#define TIM_PORT_NORMAL			0
#define TIM_PORT_TGL_OC			1
#define TIM_PORT_CLR_OC			2
#define TIM_PORT_SET_OC			3
#define TIM_PORT_OC_NONINV		4
#define TIM_PORT_OC_INV			5

#define TIM_PORT_FASTPWM_OCA_OUT	6
#define TIM_PORT_PCPWM_OCA_OUT	7
#define TIM_PORT_FPCPWM_OCA_OUT	8

#define TIM_IC_DISABLE			0
#define TIM_IC_NNC_FALLING		1
#define TIM_IC_NNC_RISING		2
#define TIM_IC_NC_FALLING		3
#define TIM_IC_NC_RISING		4

#define TIM_CH_NONE				0
#define TIM_CH_A				1
#define TIM_CH_B				2
#define TIM_CH_C				3



StatusTypeDef TIM_Init(TIM_HandleTypeDef *htim);
StatusTypeDef TIM_Start(TIM_HandleTypeDef *htim);
StatusTypeDef TIM_Stop(TIM_HandleTypeDef *htim);
StatusTypeDef TIM_Start_Normal_IT(TIM_HandleTypeDef *htim);
StatusTypeDef TIM_Stop_Normal_IT(TIM_HandleTypeDef *htim);
StatusTypeDef TIM_Start_OC_IT(TIM_HandleTypeDef *htim);
StatusTypeDef TIM_Stop_OC_IT(TIM_HandleTypeDef *htim);
StatusTypeDef TIM_Start_IC_IT(TIM_HandleTypeDef *htim);
StatusTypeDef TIM_Stop_IC_IT(TIM_HandleTypeDef *htim);


void TIM0_OVF_IRQHandler(TIM_HandleTypeDef *htim);
void TIM1_OVF_IRQHandler(TIM_HandleTypeDef *htim);
void TIM2_OVF_IRQHandler(TIM_HandleTypeDef *htim);
void TIM3_OVF_IRQHandler(TIM_HandleTypeDef *htim);

void TIM0_OC_IRQHandler(TIM_HandleTypeDef *htim);
void TIM2_OC_IRQHandler(TIM_HandleTypeDef *htim);
void TIM1_OCA_IRQHandler(TIM_HandleTypeDef *htim);
void TIM1_OCB_IRQHandler(TIM_HandleTypeDef *htim);
void TIM1_OCC_IRQHandler(TIM_HandleTypeDef *htim);
void TIM1_CAPT_IRQHandler(TIM_HandleTypeDef *htim);
void TIM3_OCA_IRQHandler(TIM_HandleTypeDef *htim);
void TIM3_OCB_IRQHandler(TIM_HandleTypeDef *htim);
void TIM3_OCC_IRQHandler(TIM_HandleTypeDef *htim);
void TIM3_CAPT_IRQHandler(TIM_HandleTypeDef *htim);

void TIM0_OVF_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM1_OVF_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM2_OVF_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM3_OVF_ElapsedCallback(TIM_HandleTypeDef *htim);

void TIM0_OC_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM2_OC_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM1_OCA_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM1_OCB_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM1_OCC_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM1_CAPT_CpltCallback(TIM_HandleTypeDef *htim);
void TIM3_OCA_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM3_OCB_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM3_OCC_ElapsedCallback(TIM_HandleTypeDef *htim);
void TIM3_CAPT_CpltCallback(TIM_HandleTypeDef *htim);
#endif /* ATMEGA128_DRV_TIMER_H_ */