#ifndef SUCTION_MOTOR_H_
#define SUCTION_MOTOR_H_

#include "hw_def.h"

#ifdef _USE_HW_SUCTION_MOTOR
#define SUCTION_MOTOR_MAX_CH	HW_SUCTION_MOTOR_MAX_CH

bool suctionMotorInit(void);
bool suctionMotorIsInit(void);
void suctionMotorRun(void);
void suctionMotorStop(void);


#endif
#endif /* SUCTION_MOTOR_H_ */