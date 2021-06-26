﻿#include "suctionmotor.h"

#ifdef _USE_HW_SUCTION_MT

#ifdef _USE_HW_XIAOMI_GEN_1ST
#include "suctionmotor/xiaomi.h"
#endif

static suctionmotor_driver_t motor;
static bool is_init = false;

void suctionMotorStop(void);
void suctionMotorSetSpeed(uint16_t speed_);

bool suctionMotorInit(void)
{
#ifdef _USE_HW_SUCTION_MT
	if (xiaomiInit(_DEF_SUCTION_0))
	{
		is_init = xiaomiDriverInit(&motor);
	}
	suctionMotorSetSpeed(0);
	suctionMotorStop();
	return true;
	
#endif
}

bool suctionMotorIsInit(void)
{
	return is_init;
}
void suctionMotorRun(void)
{
#ifdef _USE_HW_SUCTION_MT
	motor.startMotor(_DEF_SUCTION_0);
#endif
}

void suctionMotorStop(void)
{
#ifdef _USE_HW_SUCTION_MT
	motor.stopMotor(_DEF_SUCTION_0);
#endif
}

void suctionMotorSetSpeed(uint16_t speed_)
{
#ifdef _USE_HW_SUCTION_MT
	motor.setSpeed(_DEF_SUCTION_0, speed_);
#endif
}

uint16_t* suctionMotorGetSpeed(void)
{
	static uint16_t ret[SUCTION_MT_MAX_CH];
#ifdef _USE_HW_SUCTION_MT
	ret[_DEF_SUCTION_0] = motor.getSpeed(_DEF_SUCTION_0);
#endif
	return &ret[0];
}


#endif