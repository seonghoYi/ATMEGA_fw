#include "hw.h"

void hwInit(void)
{
	bspInit();
	gpioInit();
	extiInit();
	uartInit();
	hc05Init();
	suctionMotorInit();
	motorInit();
	rosInit();
	runLedInit();
	servoInit();
}