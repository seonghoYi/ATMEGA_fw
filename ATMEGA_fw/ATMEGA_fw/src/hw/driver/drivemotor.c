﻿#include "drivemotor.h"

#ifdef _USE_HW_DRIVEMOTOR

#ifdef _USE_HW_A4988
#include "drivemotor/a4988.h"
#endif


static drivemotor_driver_t motor;
static bool is_init = false;


void motorBreak(void);
void motorSetSpeed(uint8_t speed_);
void motorSetMotionState(uint8_t motion_);

bool motorInit(void)
{
#ifdef _USE_HW_A4988
	if(a4988Init(_DEF_A4988_0) && a4988Init(_DEF_A4988_1))
	{
		is_init = a4988DriverInit(&motor);
	}
#endif

	motor.setCallBack(_DEF_A4988_0, NULL);
	motor.setCallBack(_DEF_A4988_1, NULL);
	motorSetMotionState(0);
	motorSetSpeed(0);
	motorBreak();
	
	return true;
}

bool motorIsInit(void)
{
	return is_init;
}

void motorRun(void)
{
#ifdef _USE_HW_A4988
	motor.startMotor(_DEF_A4988_0);
	motor.startMotor(_DEF_A4988_1);
#endif
}

void motorStop(void)
{
#ifdef _USE_HW_A4988
	motor.stopMotor(_DEF_A4988_0);
	motor.stopMotor(_DEF_A4988_1);
#endif	
}

void motorBreak(void)
{
#ifdef _USE_HW_A4988
	motor.breakMotor(_DEF_A4988_0);
	motor.breakMotor(_DEF_A4988_1);
#endif	
}

void motorSetSpeed(uint8_t speed_)
{
	const uint8_t power_max = 255;
	uint16_t power_ = (power_max * speed_) / 100;
	
	if (speed_ > 100 || speed_ < 0)
	{
		return;
	}
	
#ifdef _USE_HW_A4988
	motor.setSpeed(_DEF_A4988_0, (uint8_t) power_);
	motor.setSpeed(_DEF_A4988_1, (uint8_t) power_);
#endif
}

uint8_t* motorGetSpeed(void)
{
	const uint8_t power_max = 255;
	uint8_t speed_[2] = {0};
	static uint8_t power_[2] = {0};
#ifdef _USE_HW_A4988
	speed_[0] = motor.getSpeed(_DEF_A4988_0);
	speed_[1] = motor.getSpeed(_DEF_A4988_1);
	power_[0] = (uint8_t)((speed_[0] / power_max) * 100);
	power_[1] = (uint8_t)((speed_[1] / power_max) * 100);
	return power_;
#endif
}

void motorSetMotionState(uint8_t motion_)
{
	
#ifdef _USE_HW_A4988
	switch(motion_)
	{
		case 0:
		motor.setDirection(_DEF_A4988_0, _DEF_CCW);
		motor.setDirection(_DEF_A4988_1, _DEF_CW);
		break;
		case 1:
		motor.setDirection(_DEF_A4988_0, _DEF_CW);
		motor.setDirection(_DEF_A4988_1, _DEF_CW);
		break;
		case 2:
		motor.setDirection(_DEF_A4988_0, _DEF_CW);
		motor.setDirection(_DEF_A4988_1, _DEF_CCW);
		break;
		case 3:
		motor.setDirection(_DEF_A4988_0, _DEF_CCW);
		motor.setDirection(_DEF_A4988_1, _DEF_CCW);
		break;
		default:
		break;
	}
#endif
}

uint8_t motorGetMotionState(void)
{
#ifdef _USE_HW_A4988
	if (motor.getDirection(_DEF_A4988_0) == _DEF_CCW && motor.getDirection(_DEF_A4988_1) == _DEF_CW)
	{
		return 0;
	}
	else if (motor.getDirection(_DEF_A4988_0) == _DEF_CW && motor.getDirection(_DEF_A4988_1) == _DEF_CW)
	{
		return 1;
	}
	else if (motor.getDirection(_DEF_A4988_0) == _DEF_CW && motor.getDirection(_DEF_A4988_1) == _DEF_CCW)
	{
		return 2;
	}
	else if (motor.getDirection(_DEF_A4988_0) == _DEF_CCW && motor.getDirection(_DEF_A4988_1) == _DEF_CCW)
	{
		return 3;
	}
#endif
	return 0;
}

void motorAdvance(void)
{
#ifdef _USE_HW_A4988
	motorSetMotionState(0);
	motorRun();
#endif
}

void motorGoBackward(void)
{
#ifdef _USE_HW_A4988
	motorSetMotionState(2);
	motorRun();
#endif
}

void motorSpin(bool spinwise_)
{
#ifdef _USE_HW_A4988
	if (spinwise_)
	{
		motorSetMotionState(3);
	}
	else
	{
		motorSetMotionState(1);
	}
#endif
}

void motorSteering(int8_t steering_degrees_)
{
	const uint8_t power_max = 255;
	uint8_t steering_bias_proportion = abs(steering_degrees_);
	uint8_t *power_proportion = NULL;
		
	if (steering_degrees_ < -100 || steering_degrees_ > 100)
	{
		return;
	}
#ifdef _USE_HW_A4988
	power_proportion = motorGetSpeed();

	if (steering_degrees_ < 0)
	{
		if (power_proportion[0] >= power_proportion[1])
		{
			steering_bias_proportion = (uint8_t)((power_proportion[0] * steering_bias_proportion) / 100);
			motor.setSpeed(_DEF_A4988_0, (uint8_t)((power_max * steering_bias_proportion) / 100));
			motor.setSpeed(_DEF_A4988_1, (uint8_t)((power_max * power_proportion[0]) / 100));
		}
		else if (power_proportion[0] < power_proportion[1])
		{
			steering_bias_proportion = (uint8_t)((power_proportion[1] * steering_bias_proportion) / 100);
			motor.setSpeed(_DEF_A4988_0, (uint8_t)((power_max * steering_bias_proportion) / 100));
			motor.setSpeed(_DEF_A4988_1, (uint8_t)((power_max * power_proportion[1]) / 100));
		}
	}
	else if (steering_degrees_ > 0)
	{
		if (power_proportion[0] >= power_proportion[1])
		{
			steering_bias_proportion = (uint8_t)((power_proportion[0] * steering_bias_proportion) / 100);
			motor.setSpeed(_DEF_A4988_0, (uint8_t)((power_max * power_proportion[0]) / 100));
			motor.setSpeed(_DEF_A4988_1, (uint8_t)((power_max * steering_bias_proportion) / 100));
		}
		else if (power_proportion[0] < power_proportion[1])
		{
			steering_bias_proportion = (uint8_t)((power_proportion[1] * steering_bias_proportion) / 100);
			motor.setSpeed(_DEF_A4988_0, (uint8_t)((power_max *  power_proportion[1]) / 100));
			motor.setSpeed(_DEF_A4988_1, (uint8_t)((power_max *steering_bias_proportion) / 100));
		}
	}
	else if (steering_degrees_ == 0)
	{
		if (power_proportion[0] >= power_proportion[1])
		{
			steering_bias_proportion = power_proportion[0];
			motor.setSpeed(_DEF_A4988_0, (uint8_t)((power_max * steering_bias_proportion) / 100));
			motor.setSpeed(_DEF_A4988_1, (uint8_t)((power_max * steering_bias_proportion) / 100));
		}
		else if (power_proportion[0] < power_proportion[1])
		{
			steering_bias_proportion = power_proportion[1];
			motor.setSpeed(_DEF_A4988_0, (uint8_t)((power_max * steering_bias_proportion) / 100));
			motor.setSpeed(_DEF_A4988_1, (uint8_t)((power_max * steering_bias_proportion) / 100));
		}
	}
#endif
}



#endif