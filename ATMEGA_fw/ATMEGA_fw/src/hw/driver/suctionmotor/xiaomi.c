#include "suctionmotor/xiaomi.h"

#ifdef _USE_HW_XIAOMI_GEN_1ST

typedef struct
{
	bool is_open;
	xiaomigen1st_HandlerTypeDef *h_xiaomi;
} xiaomigen1st_t;


xiaomigen1st_t xiaomigen1st_tbl[XIAOMI_GEN_1ST_MAX_CH];


xiaomigen1st_HandlerTypeDef h_xiaomi_1;


bool xiaomiInit(uint8_t ch_)
{
	bool ret = true;
	
	xiaomigen1st_t *p_xiaomi = &xiaomigen1st_tbl[ch_];

	switch(ch_)
	{
		case _DEF_XIAOMI_1:
		p_xiaomi->h_xiaomi				= &h_xiaomi_1;
		
		p_xiaomi->h_xiaomi->Init.ch		= _DEF_XIAOMI_1;
		p_xiaomi->h_xiaomi->speed		= 0;
		
		break;
		default:
		break;
	}

	p_xiaomi->is_open = true;

	DDRB |= (1<<PIN7);
	
	TCCR2 |= (1<<WGM20) | (1<<WGM21); //Fast PWM
	
	TCCR2 |= (1<<COM21);
	TCCR2 &= ~(1<<COM20); //Fast PWM Non inverting mode
	
	TCCR2 |= (1<<CS20);
	TCCR2 &= ~((1<<CS21) | (1<<CS22)); //Prescaler 1 -> 62.5KHz PWM in 16MHz system clock
	
	TCCR2 &= ~((1<<COM20) | (1<<COM21)); //Off PWM wave output
	
	PORTB &= ~(1<<PIN7); //Output pull low  //Port 모드가 normal 일 경우 low 상태로 유지되게 하기 위함
	
	return ret;
}

bool xiaomiDriverInit(suctionmotor_driver_t *p_driver)
{
	p_driver->init = xiaomiInit;
	p_driver->startMotor = xiaomiStart;
	p_driver->stopMotor = xiaomiStop;
	p_driver->setSpeed = xiaomiSetSpeed;
	p_driver->getSpeed = xiaomiGetSpeed;
	return true;
}

bool xiaomiStart(uint8_t ch_)
{
	bool ret = true;
	
	TCCR2 |= (1<<COM21);
	TCCR2 &= ~(1<<COM20); //Fast PWM Non inverting mode

	return ret;
}

bool xiaomiStop(uint8_t ch_)
{
	bool ret = true;
	
	TCCR2 &= ~((1<<COM20) | (1<<COM21)); //Off PWM wave output

	return ret;
}

bool xiaomiSetSpeed(uint8_t ch_, uint16_t speed_)
{
	bool ret = false;

	xiaomigen1st_t *p_xiaomi = &xiaomigen1st_tbl[ch_];
	
	if (speed_ < 0 || speed_ > 100)
	{
		return ret;
	}
	
	p_xiaomi->h_xiaomi->speed = 255 * speed_ / 100;

	OCR2 = p_xiaomi->h_xiaomi->speed;
	
	ret = true;
	return ret;
}

uint16_t xiaomiGetSpeed(uint8_t ch_)
{
	uint16_t ret = 0;
	
	xiaomigen1st_t *p_xiaomi = &xiaomigen1st_tbl[ch_];
	ret = p_xiaomi->h_xiaomi->speed;
	return ret;
}


#endif