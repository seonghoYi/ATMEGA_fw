#ifndef HW_DEF_H_
#define HW_DEF_H_

#include "def.h"
#include "bsp.h"

#define _USE_HW_SYSTICK

#define _USE_HW_UART
#define		 HW_UART_MAX_CH				2

#define _USE_HW_ROS
#define		 HW_ROS_PACKET_BUF_MAX		512

#define _USE_HW_TIMER
#define		 HW_TIMER8_MAX_CH			1
#define		 HW_TIMER16_MAX_CH			2

#define _USE_HW_SG90
#define		 HW_SG90_MAX_CH				2

#define _USE_HW_SUCTION_MOTOR
#define		 HW_SUCTION_MOTOR_MAX_CH	1

#endif /* HW_DEF_H_ */