#include "line_led.h"
#include "gpio.h"


#ifdef _USE_HW_LINE_LED

void lineLedOn(void)
{
	gpioPinWrite(_DEF_GPIO_LINE_LED, true);
}

void lineLedOff(void)
{
	gpioPinWrite(_DEF_GPIO_LINE_LED, false);
}

#endif