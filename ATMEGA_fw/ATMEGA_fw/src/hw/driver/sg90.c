#include "sg90.h"

#ifdef _USE_HW_SG90

static sg90_driver_t sg90;
static bool is_init = false;


bool servoInit(void)
{
	is_init = sg90PWMDriver(&sg90);
	sg90.begin();
	return true;
}

bool servoIsInit(void)
{
	return is_init;
}

void servoRun(uint16_t L_ocr, uint16_t R_ocr) // 250 ~ 500
{
	if ((L_ocr < 250) || (L_ocr > 500) || (R_ocr < 250) || (R_ocr > 500))
	{
		return;
	}
	
	sg90.setOcr(_DEF_SERVO_L, L_ocr);
	sg90.setOcr(_DEF_SERVO_R, R_ocr);
	sg90.start();
}

void servoStop(void)
{
	sg90.stop();
}


#endif
