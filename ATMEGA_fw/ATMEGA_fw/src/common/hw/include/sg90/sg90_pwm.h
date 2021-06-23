#ifndef SG90_PWM_H_
#define SG90_PWM_H_

#include "hw_def.h"

#ifdef _USE_HW_SG90
#define	SG90_MAX_CH		HW_SG90_MAX_CH
#include "sg90.h"


bool sg90PWMDriver(sg90_driver_t *p_driver);

#endif
#endif /* SG90_PWM_H_ */