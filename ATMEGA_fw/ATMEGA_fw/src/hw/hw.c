#include "hw.h"

void hwInit(void)
{
	bspInit();
	gpioInit();
	uartInit();
	//ctcInit();
	pwmInit();
	motorInit();
	//rosServerInit();
}