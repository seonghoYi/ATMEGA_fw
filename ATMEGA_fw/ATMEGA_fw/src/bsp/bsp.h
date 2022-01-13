#ifndef BSP_H_
#define BSP_H_

#include "def.h"

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



void bspInit(void);

void delay(uint32_t ms);
uint32_t millis(void);

#endif /* BSP_H_ */