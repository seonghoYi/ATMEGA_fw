#ifndef DEF_H_
#define DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define _DEF_GPIO_BT_RST		0
#define _DEF_GPIO_BT_CFG		1
#define _DEF_GPIO_RUN_LED		2
#define	_DEF_GPIO_SUCTION_RELAY 3
#define _DEF_GPIO_MOTOR_EN		4
#define _DEF_GPIO_MOTOR_DIR_L	5
#define _DEF_GPIO_MOTOR_DIR_R	6
#define _DEF_GPIO_MOTOR_PWM_L	7
#define _DEF_GPIO_MOTOR_PWM_R	8
#define _DEF_GPIO_SUCTION_PWM	9


#define _DEF_INPUT		0
#define _DEF_OUTPUT		1


#define _DEF_UART1		0
#define _DEF_UART2		1

#define _DEF_TIM_CH_A		0
#define _DEF_TIM_CH_B		1
#define _DEF_TIM_CH_C		2

#define _DEF_XIAOMI_1	0

#define _DEF_SUCTION_1	0

#define _DEF_SG90_1		0
#define _DEF_SG90_2		1

#define _DEF_SERVO_1	0
#define _DEF_SERVO_2	1

#define _DEF_CW			0
#define _DEF_CCW		1

#define _DEF_DMC16_1	0
#define _DEF_DMC16_2	1

#define _DEF_HC05_1		0

#define _DEF_ROS1		0


#define constrain(in,low,high) ((in)<(low)?(low):((in)>(high)?(high):(in)))

#ifndef map
#define map(input, in_min, in_max, out_min, out_max) ((input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)
#endif


#endif /* DEF_H_ */