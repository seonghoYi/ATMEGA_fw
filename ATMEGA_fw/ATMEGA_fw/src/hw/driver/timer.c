#include "timer.h"


#ifdef _USE_HW_TIMER

typedef struct
{
	bool is_open;
	uint8_t tccr;
	uint8_t tcnt;
	uint8_t ocr;
} tim8_t;

typedef struct
{
	bool is_open;
	uint8_t tccra;
	uint8_t tccrb;
	uint8_t tccrc;
	uint16_t tcnt;
	uint16_t ocr[3];
	uint16_t icr;
} tim16_t;

static tim8_t	tim8_tbl[TIMER8_MAX_CH];
static tim16_t	tim16_tbl[TIMER16_MAX_CH];


bool timerInit(void)
{
	bool ret = true;
	
	for (int i = 0; i < TIMER8_MAX_CH; i++)
	{
		tim8_tbl[i].is_open = false;
		tim8_tbl[i].tccr = 0;
		tim8_tbl[i].ocr = 0;
		tim8_tbl[i].tcnt = 0;
	}
	for (int i = 0; i < TIMER16_MAX_CH; i++)
	{
		tim16_tbl[i].is_open = false;
		tim16_tbl[i].tccra = 0;
		tim16_tbl[i].tccrb = 0;
		tim16_tbl[i].tccrc = 0;
		tim16_tbl[i].tcnt = 0;
		tim16_tbl[i].ocr[0] = 0;
		tim16_tbl[i].ocr[1] = 0;
		tim16_tbl[i].ocr[2] = 0;
		tim16_tbl[i].icr = 0;
	}
	
	return ret;
}

bool timerBegin(uint8_t ch_)
{
	bool ret = true;
	
	switch(ch_)
	{
		case TIM2:
			SET_BIT(tim8_tbl[_DEF_TIM1].tccr, (1 << WGM20));
			SET_BIT(tim8_tbl[_DEF_TIM1].tccr, (1 << CS20)); // Phase correct PWM f=31.372KHz (F_CPU = 16MHz) (in 8MHz case, set timer as same then will be f=15.686KHz)
			
			SET_BIT(tim8_tbl[_DEF_TIM1].tccr, (1 << COM21)); // port oc2 non inverted output
			
			tim8_tbl[_DEF_TIM1].is_open = true;
		break;
		case TIM1:
			SET_BIT(tim16_tbl[_DEF_TIM2].tccra, (1 << WGM11) | (1 << WGM10)); 
			SET_BIT(tim16_tbl[_DEF_TIM2].tccrb, (1 << WGM12) | (1 << CS11)); // 10-bit Fast PWM f=1.953KHz (F_CPU = 16MHz) (in 8MHz case, set timer as 9-bit Fast PWM prescaler 8 then will be same)
			
			SET_BIT(tim16_tbl[_DEF_TIM2].tccra, (1 << COM1A1) | (1 << COM1B1)); // port A, B non inverted output
			
			tim16_tbl[_DEF_TIM2].is_open = true;
		break;
		case TIM3:
			SET_BIT(tim16_tbl[_DEF_TIM3].tccra, (1 << WGM31));
			SET_BIT(tim16_tbl[_DEF_TIM3].tccrb, (1 << WGM33) | (1 << WGM32) | (1 << CS31) | (1 << CS30));
			tim16_tbl[_DEF_TIM3].icr = 4999; // Fast PWM f=50Hz (in 8MHz case, set ICR3 as 2499)
			
			SET_BIT(tim16_tbl[_DEF_TIM3].tccra, (1 << COM3A1) | (1 << COM3B1)); // port A, B non inverted output
			
			tim16_tbl[_DEF_TIM3].is_open = true;
		break;
		#ifdef _USE_HW_SYSTICK
		case TIM0:
		
		break;
		#endif
	}
	
	return ret;
}

bool timerIsOpen(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2: return tim8_tbl[_DEF_TIM1].is_open;
		case TIM1: return tim16_tbl[_DEF_TIM2].is_open;
		case TIM3: return tim16_tbl[_DEF_TIM3].is_open;
		case TIM0: return tim8_tbl[_DEF_TIM4].is_open;
		default:   return false;
	}
}


bool timerStart(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2:
			WRITE_REG(TCCR2, tim8_tbl[_DEF_TIM1].tccr);
			WRITE_REG(TCNT2, tim8_tbl[_DEF_TIM1].tcnt);
			WRITE_REG(OCR2, tim8_tbl[_DEF_TIM1].ocr);
		break;
		case TIM1:
			WRITE_REG(TCCR1A, tim16_tbl[_DEF_TIM2].tccra);
			WRITE_REG(TCCR1B, tim16_tbl[_DEF_TIM2].tccrb);
			WRITE_REG(TCCR1C, tim16_tbl[_DEF_TIM2].tccrc);
			WRITE_REG(OCR1A, tim16_tbl[_DEF_TIM2].ocr[CHANNEL_A]);
			WRITE_REG(OCR1B, tim16_tbl[_DEF_TIM2].ocr[CHANNEL_B]);
			WRITE_REG(OCR1C, tim16_tbl[_DEF_TIM2].ocr[CHANNEL_C]);
			WRITE_REG(TCNT1, tim16_tbl[_DEF_TIM2].tcnt);
			WRITE_REG(ICR1, tim16_tbl[_DEF_TIM2].icr);
		break;
		case TIM3:	
			WRITE_REG(TCCR3A, tim16_tbl[_DEF_TIM3].tccra);
			WRITE_REG(TCCR3B, tim16_tbl[_DEF_TIM3].tccrb);
			WRITE_REG(TCCR3C, tim16_tbl[_DEF_TIM3].tccrc);
			WRITE_REG(OCR3A, tim16_tbl[_DEF_TIM3].ocr[CHANNEL_A]);
			WRITE_REG(OCR3B, tim16_tbl[_DEF_TIM3].ocr[CHANNEL_B]);
			WRITE_REG(OCR3C, tim16_tbl[_DEF_TIM3].ocr[CHANNEL_C]);
			WRITE_REG(TCNT3, tim16_tbl[_DEF_TIM3].tcnt);
			WRITE_REG(ICR3, tim16_tbl[_DEF_TIM3].icr);
		break;
		case TIM0:
			WRITE_REG(TCCR0, tim8_tbl[_DEF_TIM4].tccr);
			WRITE_REG(TCNT0, tim8_tbl[_DEF_TIM4].tcnt);
			WRITE_REG(OCR0, tim8_tbl[_DEF_TIM4].ocr);
		break;
	}
	return true;
}

bool timerStop(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2:
			CLEAR_REG(TCCR2);
			CLEAR_REG(TCNT2);
			CLEAR_REG(OCR2);
		break;
		case TIM1:
			CLEAR_REG(TCCR1A);
			CLEAR_REG(TCCR1B);
			CLEAR_REG(TCCR1C);
			CLEAR_REG(OCR1A);
			CLEAR_REG(OCR1B);
			CLEAR_REG(OCR1C);
			CLEAR_REG(TCNT1);
			CLEAR_REG(ICR1);
		break;
		case TIM3:
			CLEAR_REG(TCCR3A);
			CLEAR_REG(TCCR3B);
			CLEAR_REG(TCCR3C);
			CLEAR_REG(OCR3A);
			CLEAR_REG(OCR3B);
			CLEAR_REG(OCR3C);
			CLEAR_REG(TCNT3);
			CLEAR_REG(ICR3);
		break;
		case TIM0:
			CLEAR_REG(TCCR0);
			CLEAR_REG(TCNT0);
			CLEAR_REG(OCR0);
		break;
	}
	return true;
}


bool timerSetTcnt(uint8_t ch_, uint16_t tcnt_)
{
	switch(ch_)
	{
		case TIM2:
			tim8_tbl[_DEF_TIM1].tcnt = (uint8_t)tcnt_;
			TCNT2 = tim8_tbl[_DEF_TIM1].tcnt;
		break;
		case TIM1:
			tim16_tbl[_DEF_TIM2].tcnt = tcnt_;
			TCNT1 = tim16_tbl[_DEF_TIM2].tcnt;
		break;
		case TIM3:
			tim16_tbl[_DEF_TIM3].tcnt = tcnt_;
			TCNT3 = tim16_tbl[_DEF_TIM3].tcnt;
		break;
		case TIM0:
			tim8_tbl[_DEF_TIM4].tcnt = (uint8_t)tcnt_;
			TCNT0 = tim8_tbl[_DEF_TIM4].tcnt;
		default: 
		return false;
	}
	return true;
}

uint16_t timerGetTcnt(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2: return (uint16_t)TCNT2;
		case TIM1: return TCNT1;
		case TIM3: return TCNT3;
		case TIM0: return (uint16_t)TCNT0;
		default: return 0;
	}
}

bool timer8SetOcr(uint8_t ch_, uint8_t ocr_)
{
	switch(ch_)
	{
		case TIM2:
			tim8_tbl[_DEF_TIM1].ocr = ocr_;
			OCR2 = tim8_tbl[_DEF_TIM1].ocr;
		break;
		case TIM0:
			tim8_tbl[_DEF_TIM4].ocr = ocr_;
			OCR0 = tim8_tbl[_DEF_TIM4].ocr;
		break;
		default: 
		return false;
	}
	return true;
}

uint8_t timer8GetOcr(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2: return tim8_tbl[_DEF_TIM1].ocr;
		case TIM0: return tim8_tbl[_DEF_TIM4].ocr;
		default: return 0;
	}
}

bool timer16SetOcr(uint8_t ch_, uint16_t ocr_, uint8_t channel_)
{
	switch(ch_)
	{
		case TIM1:
			if (channel_ == CHANNEL_A)
			{
				tim16_tbl[_DEF_TIM2].ocr[CHANNEL_A] = ocr_;
				OCR1A = tim16_tbl[_DEF_TIM2].ocr[CHANNEL_A];
			}
			else if (channel_ == CHANNEL_B)
			{
				tim16_tbl[_DEF_TIM2].ocr[CHANNEL_B] = ocr_;
				OCR1B = tim16_tbl[_DEF_TIM2].ocr[CHANNEL_B];
			}
			else if (channel_ == CHANNEL_C)
			{
				tim16_tbl[_DEF_TIM2].ocr[CHANNEL_C] = ocr_;
				OCR1C = tim16_tbl[_DEF_TIM2].ocr[CHANNEL_C];
			}
		break;
		case TIM3:
			if (channel_ == CHANNEL_A)
			{
				tim16_tbl[_DEF_TIM3].ocr[CHANNEL_A] = ocr_;
				OCR3A = tim16_tbl[_DEF_TIM3].ocr[CHANNEL_A];
			}
			else if (channel_ == CHANNEL_B)
			{
				tim16_tbl[_DEF_TIM3].ocr[CHANNEL_B] = ocr_;
				OCR3B = tim16_tbl[_DEF_TIM3].ocr[CHANNEL_B];
			}
			else if (channel_ == CHANNEL_C)
			{
				tim16_tbl[_DEF_TIM3].ocr[CHANNEL_C] = ocr_;
				OCR3C = tim16_tbl[_DEF_TIM3].ocr[CHANNEL_C];
			}
		break;
		default:
		return false;
	}
	return true;
}

uint16_t timer16GetOcr(uint8_t ch_, uint8_t channel_)
{
	switch(ch_)
	{
		case TIM1: return tim16_tbl[_DEF_TIM2].ocr[channel_];
		case TIM3: return tim16_tbl[_DEF_TIM3].ocr[channel_];
		default:   return 0;
	}
}

bool timer16SetIcr(uint8_t ch_, uint16_t icr_)
{
	switch(ch_)
	{
		case TIM1:
			tim16_tbl[_DEF_TIM2].icr = icr_;
			ICR1 = tim16_tbl[_DEF_TIM2].icr;
		break;
		case TIM3:
			tim16_tbl[_DEF_TIM3].icr = icr_;
			ICR3 = tim16_tbl[_DEF_TIM3].icr;
		break;
		default:
		return false;
	}
	
	return true;
}

uint16_t timer16GetIcr(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM1: return tim16_tbl[_DEF_TIM2].icr;
		case TIM3: return tim16_tbl[_DEF_TIM3].icr;
		default:   return 0;
	}
}

#endif