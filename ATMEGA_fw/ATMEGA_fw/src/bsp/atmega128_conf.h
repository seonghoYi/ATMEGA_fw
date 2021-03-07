#ifndef ATMEGA128_CONF_H_
#define ATMEGA128_CONF_H_



#define DRV_MODULE_ENABLED
#define DRV_GPIO_MODULE_ENABLED
#define DRV_UART_MODULE_ENABLED



#ifdef DRV_GPIO_MODULE_ENABLED
#include "atmega128_DRV_gpio.h"
#endif

#ifdef DRV_UART_MODULE_ENABLED
#include "atmega128_DRV_uart.h"
#endif

#endif /* ATMEGA128_CONF_H_ */