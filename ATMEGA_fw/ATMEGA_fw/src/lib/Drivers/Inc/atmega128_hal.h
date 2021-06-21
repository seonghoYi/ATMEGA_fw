#ifndef ATMEGA128_HAL_H_
#define ATMEGA128_HAL_H_

#include "atmega128_conf.h"


typedef enum
{
	HAL_TICK_FREQ_250HZ		= 4U,
	HAL_TICK_FREQ_500HZ		= 2U,
	HAL_TICK_FREQ_1KHZ		= 1U,
	HAL_TICK_FREQ_DEFAULT	= HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;


extern volatile uint32_t tick;
extern HAL_TickFreqTypeDef tickFreq;

HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_InitTick(void);
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);
HAL_StatusTypeDef SetTickFreq(uint32_t Freq);
uint32_t GetTickFreq(void);


#endif /* ATMEGA128_HAL_H_ */