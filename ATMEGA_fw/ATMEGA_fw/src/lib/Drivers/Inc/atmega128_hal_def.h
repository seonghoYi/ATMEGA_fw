#ifndef ATMEGA128_HAL_DEF_H_
#define ATMEGA128_HAL_DEF_H_

#include "atmega128.h"

typedef enum 
{
	OK = 0x00,
	ERROR = 0x01,
	BUSY = 0x02,
	TIMEOUT = 0x03
} StatusTypeDef;

#define UNUSED(x) (void)x


#endif /* ATMEGA128_HAL_DEF_H_ */