#include "exti.h"


enum ExtiLine
{
	EXTI_LINE_0 = 0x00U,
	EXTI_LINE_1,
	EXTI_LINE_2,
	EXTI_LINE_3,
	EXTI_LINE_4,
	EXTI_LINE_5,
	EXTI_LINE_6,
	EXTI_LINE_7
};

enum ExtiMode
{
	EXTI_TRIGGER_LOW = 0x00U,
	EXTI_TRIGGER_BOTH,
	EXTI_TRIGGER_FALLING,
	EXTI_TRIGGER_RISING
};


typedef struct
{
	uint8_t exti_line;
	
	uint8_t mode;
} exti_tbl_t;


exti_tbl_t exti_tbl[EXTI_MAX_CH] = 
{
	{EXTI_LINE_5, EXTI_TRIGGER_FALLING},
	{EXTI_LINE_6, EXTI_TRIGGER_RISING},
	{EXTI_LINE_7, EXTI_TRIGGER_FALLING},
};


bool extiInit(void)
{
	bool ret = true;
	
	for(int i = 0; i < EXTI_MAX_CH; i++)
	{
		extiMode(exti_tbl[i].exti_line, exti_tbl[i].mode);
	}
	
	return ret;
}

bool extiMode(uint8_t line, uint8_t mode)
{
	bool ret = false;
	
	switch(line)
	{
	case EXTI_LINE_0:
	case EXTI_LINE_1:
	case EXTI_LINE_2:
	case EXTI_LINE_3:
	if (mode == EXTI_TRIGGER_BOTH)
	{
		return ret;
	}
	EICRA |= mode << line * 2;
	ret = true;
	break;
	case EXTI_LINE_4:
	case EXTI_LINE_5:
	case EXTI_LINE_6:
	case EXTI_LINE_7:
	EICRB |= mode << (line - 4) * 2;
	ret = true;
	break;
	}
	
	EIMSK |= (1<<line);

	return ret;
}