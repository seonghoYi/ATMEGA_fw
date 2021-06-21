#ifndef ATMEGA128_HAL_DEF_H_
#define ATMEGA128_HAL_DEF_H_

#include "atmega128.h"

typedef enum 
{
	HAL_OK = 0x00,
	HAL_ERROR = 0x01,
	HAL_BUSY = 0x02,
	HAL_TIMEOUT = 0x03
} HAL_StatusTypeDef;


#define HAL_MAX_DELAY 0xFFFFFFFFU
#define UNUSED(x) (void)x


#endif /* ATMEGA128_HAL_DEF_H_ */