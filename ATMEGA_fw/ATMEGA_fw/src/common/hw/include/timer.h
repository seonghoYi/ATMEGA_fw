#ifndef TIMER_H_
#define TIMER_H_

#include "hw_def.h"

#ifdef _USE_HW_TIM
#define TIM_MAX_CH		HW_TIM_MAX_CH


bool timerInit(void);
bool timerStart(uint8_t ch, uint16_t tcnt, uint16_t ocr, uint16_t icr);
bool timerStop(uint8_t ch);

uint16_t timerGetTcnt(uint8_t ch);
bool timerSetTcnt(uint8_t ch, uint16_t tcnt);
uint16_t timerGetOcr(uint8_t ch);
bool timerSetOcr(uint8_t ch, uint16_t ocr);
uint16_t timerGetIcr(uint8_t ch);
bool timerSetIcr(uint8_t ch, uint16_t icr);

#endif
#endif /* TIMER_H_ */