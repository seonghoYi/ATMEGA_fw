#ifndef DRIVEMOTOR_H_
#define DRIVEMOTOR_H_

#include "hw_def.h"

#ifdef _USE_HW_DRIVEMOTOR
#define DRIVEMOTOR_MAX_CH	HW_DRIVEMOTOR_MAX_CH

typedef struct drivemotor_driver_t
{
	bool (*init)(void);
	
	
	
} drivemotor_driver_t;



#endif
#endif /* DRIVEMOTOR_H_ */