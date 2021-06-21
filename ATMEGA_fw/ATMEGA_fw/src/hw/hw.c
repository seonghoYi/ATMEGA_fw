#include "hw.h"

void hwInit(void)
{
	bspInit();
	gpioInit();
	uartInit();
	//timerInit();
	//a4988Init();
	//ctcInit();
	//pwmInit();
	//motorInit();
	//rosServerInit();
}