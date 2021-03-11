#ifndef PWM_H_
#define PWM_H_

#include "hw_def.h"

#ifdef _USE_HW_PWM
#define PWM_MAX_CH	HW_PWM_MAX_CH

bool pwmInit(void);


#endif /* PWM_H_ */