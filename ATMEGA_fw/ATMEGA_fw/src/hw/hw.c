#include "hw.h"

void hwInit(void)
{
	bspInit();
	gpioInit();
	uartInit();
	//ctcInit();
	pwmInit();
	hc05Init();
	suctionMotorInit();
	motorInit();
	//rosServerInit();
	
	runLedInit();
	lineLedInit();
}