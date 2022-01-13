#include "servo/sg90.h"
#include "pwm.h"
#include "gpio.h"


#ifdef _USE_HW_SG90

typedef struct sg90_t
{
	bool is_open;
	sg90_HandlerTypeDef *h_sg90;
} sg90_t;

sg90_t sg90_tbl[SG90_MAX_CH];


sg90_HandlerTypeDef h_sg90_1;
sg90_HandlerTypeDef h_sg90_2;

bool sg90Init(uint8_t ch_)
{
	bool ret = false;
	
	sg90_t *p_sg90_t = &sg90_tbl[ch_];
	
	switch(ch_)
	{
		case _DEF_SG90_1:
		p_sg90_t->h_sg90 = &h_sg90_1;
		p_sg90_t->h_sg90->Init.ch = _DEF_SG90_1;
		p_sg90_t->h_sg90->Init.pwm_ch = _DEF_TIM_CH_A;
		p_sg90_t->h_sg90->angle = 0;
		
		break;
		case _DEF_SG90_2:
		p_sg90_t->h_sg90 = &h_sg90_2;
		p_sg90_t->h_sg90->Init.ch = _DEF_SG90_2;
		p_sg90_t->h_sg90->Init.pwm_ch = _DEF_TIM_CH_B;
		p_sg90_t->h_sg90->angle = 0;
		break;
	}

	p_sg90_t->is_open = true;
	ret = true;
	
	
	DDRE |= (1<<PIN3) | (1<<PIN4); //Output enable
	
	TCCR3A |= (1<<COM3A1) | (1<<COM3B1);
	TCCR3A &= ~((1<<COM3A0) | (1<<COM3B0)); //Fast PWM channel-A,B Non inverting mode
	
	TCCR3A |= (1<<WGM31);
	TCCR3B |= (1<<WGM32) | (1<<WGM33); 
	TCCR3A &= ~(1<<WGM30); //Fast PWM ICR TOP mode
	
	TCCR3B |= (1<<CS30) | (1<<CS31);
	TCCR3B &= ~(1<<CS32); 
	ICR3 = 4999; //Prescaler 64 -> 50Hz PWM in 16MHz system clock
	
	
	TCCR3A &= ~((1<<COM3A0) | (1<<COM3A1) | (1<<COM3B0) | (1<<COM3B1)); //Off PWM wave output
	PORTE &= ~((1<<PIN3) | (1<<PIN4)); //Output pull low  //Port 모드가 normal 일 경우 low 상태로 유지되게 하기 위함

	return ret;
}

bool sg90DriverInit(servo_driver_t *p_driver)
{
	p_driver->init = sg90Init;
	p_driver->setAngle = sg90Write;
	p_driver->getAngle = sg90Read;
	
	return true;
}

bool sg90Write(uint8_t ch_, uint8_t angle)
{
	sg90_t *p_sg90 = &sg90_tbl[ch_];
	
	p_sg90->h_sg90->angle = angle;
	
	// T=20ms 0.5ms~2.5ms
	int range = ICR3 / 8;
	int32_t duty = map(angle, 0, 180, range/5, range); //0.5ms와 2.5ms 를 0도~180도의 범위로 사상
	
	if(p_sg90->h_sg90->Init.pwm_ch == _DEF_TIM_CH_A)
	{
		TCCR3A |= (1<<COM3A1);
		TCCR3A &= ~(1<<COM3A0); //Fast PWM channel-A Non inverting mode
		OCR3A = duty;
	}
	else if(p_sg90->h_sg90->Init.pwm_ch == _DEF_TIM_CH_B)
	{
		TCCR3A |= (1<<COM3B1);
		TCCR3A &= ~(1<<COM3B0); //Fast PWM channel-B Non inverting mode
		OCR3B = duty;
	}
	
	delay(200); //바로 응답할 수 없으므로 200ms 대기
	
	if(p_sg90->h_sg90->Init.pwm_ch == _DEF_TIM_CH_A)
	{
		TCCR3A &= ~((1<<COM3A0) | (1<<COM3A1)); //Off PWM OCA output
	}
	else if(p_sg90->h_sg90->Init.pwm_ch == _DEF_TIM_CH_B)
	{
		TCCR3B &= ~((1<<COM3B0) | (1<<COM3B1)); //Off PWM OCB output
	}

	
	return true;
}

uint8_t	sg90Read(uint8_t ch_)
{
	sg90_t *p_sg90_t = &sg90_tbl[ch_];
	return p_sg90_t->h_sg90->angle;
}

#endif