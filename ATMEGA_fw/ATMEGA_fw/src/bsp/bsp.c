#include "bsp.h"



static volatile uint32_t ms_cnt = 0;

ISR(TIMER0_COMP_vect)
{
	ms_cnt++;
}



void bspInit(void)
{
	//systick timer
	TCCR0 &= ~0xFF;						//초기화
	TCCR0 &= ~((1<<WGM00) | (1<<WGM01));	//Timer mode normal
	TCCR0 &= ~((1<<COM00) | (1<<COM01));	//Timer port mode normal
	TCCR0 |= (1<<CS00) | (1<<CS01);		//Prescaler 32
	OCR0 = 250;							//Period 1ms
	
	TIMSK |= (1<<OCIE0);				//TIMER0 output compare match interrupt enable
	
	sei();
}


void delay(uint32_t ms)
{
	uint32_t pre_time = millis();
	
	while(millis() - pre_time < ms);
}

uint32_t millis(void)
{
	/**********************************************************************************************/
	
	//32비트값을 읽는 도중 인터럽트 발생시 값이 엉망이 될 수 있으므로 해당 구문 전후로 인터럽트 비활성화 후 복구
	
	/**********************************************************************************************/
	
	uint32_t ret;
	uint8_t old_sreg = SREG;
	
	cli();
	ret = ms_cnt; 
	SREG = old_sreg;
	
	return ret;
}