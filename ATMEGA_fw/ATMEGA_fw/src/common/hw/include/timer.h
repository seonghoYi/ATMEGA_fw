#ifndef TIMER_H_
#define TIMER_H_

#include "hw_def.h"

#ifdef _USE_HW_TIMER
#define TIMER8_MAX_CH		HW_TIMER8_MAX_CH
#define TIMER16_MAX_CH		HW_TIMER16_MAX_CH

#define TIM0	0
#define TIM1	1
#define TIM2	2
#define TIM3	3

#define CHANNEL_A	_DEF_CH_A
#define CHANNEL_B	_DEF_CH_B
#define CHANNEL_C	_DEF_CH_C




bool timerInit(void);
bool timerBegin(uint8_t ch_);
bool timerIsOpen(uint8_t ch_);
bool timerStart(uint8_t ch_);
bool timerStop(uint8_t ch_);

bool timerSetTcnt(uint8_t ch_, uint16_t tcnt_);
uint16_t timerGetTcnt(uint8_t ch_);
bool timer8SetOcr(uint8_t ch_, uint8_t ocr_);
uint8_t timer8GetOcr(uint8_t ch_);
bool timer16SetOcr(uint8_t ch_, uint16_t ocr_, uint8_t channel_);
uint16_t timer16GetOcr(uint8_t ch_, uint8_t channel_);
bool timer16SetIcr(uint8_t ch_, uint16_t icr_);
uint16_t timer16GetIcr(uint8_t ch_);



#endif
#endif /* TIMER_H_ */