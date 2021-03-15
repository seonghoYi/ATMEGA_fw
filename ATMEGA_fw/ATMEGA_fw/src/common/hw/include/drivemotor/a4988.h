#ifndef A4988_H_
#define A4988_H_

#include "hw_def.h"

#ifdef _USE_HW_A4988

#include "drivemotor.h"
#define  A4988_MAX_CH HW_A4988_MAX_CH

typedef struct
{
	uint8_t ch;
	uint8_t timer;
	uint8_t phase;
	
} a4988_InitTypeDef;

typedef struct
{
	a4988_InitTypeDef	Init;
	bool				enable;
	bool				direction;
	uint8_t				speed;
} a4988_HandlerTypeDef;



#define A4988_FULL_STEP			0
#define A4988_HALF_STEP			1	
#define A4988_QUATER_STEP		2
#define A4988_EIGHTH_STEP		3
#define A4988_SIXTEENTH_STEP	4

bool	a4988Init(uint8_t ch_);
bool	a4988DriverInit(drivemotor_driver_t *p_driver);
bool	a4988Start(uint8_t ch_);
bool	a4988Stop(uint8_t ch_);
bool	a4988Hold(uint8_t ch_);

bool	a4988Enable(void);
bool	a4988Disable(void);
bool	a4988SetDirection(uint8_t ch_, bool dir_);
bool	a4988GetDirection(uint8_t ch_);
bool	a4988SetSpeed(uint8_t ch_, uint8_t speed_);
uint8_t a4988GetSpeed(uint8_t ch_);

void a4988SetCallBack(uint8_t ch_, void (*p_func)(void));


#endif
#endif /* A4988_H_ */