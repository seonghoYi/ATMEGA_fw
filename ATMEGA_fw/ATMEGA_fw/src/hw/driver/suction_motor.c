#include "suction_motor.h"
#include "timer.h"

#ifdef _USE_HW_SUCTION_MOTOR

static bool is_init = false;

bool suctionMotorInit(void)
{
	timerBegin(_DEF_TIM1);
	is_init = true;
	return true;
}

bool suctionMotorIsInit(void)
{
	
}

void suctionMotorRun(void)
{
	
}

void suctionMotorStop(void)
{
	
}

#endif