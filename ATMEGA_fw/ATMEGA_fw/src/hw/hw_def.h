#ifndef HW_DEF_H_
#define HW_DEF_H_


#include "def.h"
#include "bsp.h"

#define _USE_HW_GPIO
#define		 HW_GPIO_MAX_CH			6

#define _USE_HW_UART
#define		 HW_UART_MAX_CH			2

#define _USE_HW_CTC
#define		 HW_CTC_MAX_CH			4

#define _USE_HW_PWM
#define		 HW_PWM_MAX_CH			4

#define _USE_HW_A4988
#define      HW_A4988_MAX_CH		2

#define _USE_HW_DRIVEMOTOR
#define      HW_DRIVEMOTOR_MAX_CH	2

#define _REVERSE_ROTATION			0
#define _NORMAL_ROTATION			1

#define _PIN_GPIO_A4988_EN			0
#define _PIN_GPIO_A4988_LDIR		1
#define _PIN_GPIO_A4988_RDIR		2

#define _USE_HW_ROS
#define		 HW_ROS_PACKET_BUF_MAX	512

#define _USE_HW_ROS_SERVER


#endif /* HW_DEF_H_ */