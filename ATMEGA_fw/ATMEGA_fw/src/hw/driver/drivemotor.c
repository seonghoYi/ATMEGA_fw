#include "drivemotor.h"

#ifdef _USE_HW_DRIVEMOTOR

#ifdef _USE_HW_DMC16
#include "drivemotor/dmc16.h"
#endif


static drivemotor_driver_t motor;
static bool is_init = false;



bool motorInit(void)
{
#ifdef _USE_HW_DMC16
	if (dmc16Init(_DEF_DMC16_1) && dmc16Init(_DEF_DMC16_2))
	{
		is_init = dmc16DriverInit(&motor);
	}
	
	motorSetLeftSpeed(0);
	motorSetRightSpeed(0);
	motorBreak();
	motorStop();
	return true;
	
#endif
}

bool motorIsInit(void)
{
	return is_init;
}

void motorRun(void)
{
#ifdef _USE_HW_DMC16
	motor.startMotor(_DEF_DMC16_1);
	motor.startMotor(_DEF_DMC16_2);
#endif
}

void motorStop(void)
{
#ifdef _USE_HW_DMC16
	motor.stopMotor(_DEF_DMC16_1);
	motor.stopMotor(_DEF_DMC16_2);
#endif
}

void motorBreak(void)
{
#ifdef _USE_HW_DMC16
	motor.breakMotor(_DEF_DMC16_1);
	motor.breakMotor(_DEF_DMC16_2);
#endif
}

void motorSetLeftSpeed(uint16_t speed_)
{
#ifdef _USE_HW_DMC16
	motor.setSpeed(_DEF_DMC16_1, speed_);
#endif
}

void motorSetRightSpeed(uint16_t speed_)
{
#ifdef _USE_HW_DMC16
	motor.setSpeed(_DEF_DMC16_2, speed_);
#endif
}


uint16_t motorGetLeftSpeed(void)
{
	uint16_t ret = 0;
	ret = motor.getSpeed(_DEF_DMC16_1);
	return ret;
}

uint16_t motorGetRightSpeed(void)
{
	uint16_t ret = 0;
	ret = motor.getSpeed(_DEF_DMC16_2);
	return ret;
}

void motorSetLeftDirection(bool dir)
{
#ifdef _USE_HW_DMC16
	motor.setDirection(_DEF_DMC16_1, dir);
#endif
}

void motorSetRightDirection(bool dir)
{
#ifdef _USE_HW_DMC16
	motor.setDirection(_DEF_DMC16_2, dir);
#endif
}


#endif