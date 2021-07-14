#include "hw.h"

void hwInit(void)
{
	bspInit();
	gpioInit();
	extiInit();
	uartInit();
	//ctcInit();
	pwmInit();
	hc05Init();
	suctionMotorInit();
	motorInit();
	//rosServerInit();
	
	runLedInit();
	//lineLedInit();
}