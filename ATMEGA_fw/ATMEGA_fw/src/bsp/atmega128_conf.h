#ifndef ATMEGA128_CONF_H_
#define ATMEGA128_CONF_H_



#define HAL_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_TIMER_MODULE_ENABLED


#ifdef HAL_GPIO_MODULE_ENABLED
#include "atmega128_hal_gpio.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
#include "atmega128_hal_uart.h"
#endif

#ifdef HAL_TIMER_MODULE_ENABLED
#include "atmega128_hal_timer.h"
#endif

#endif /* ATMEGA128_CONF_H_ */