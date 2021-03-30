#include "ros_server.h"

#ifdef _USE_HW_ROS_SERVER

#ifdef _USE_HW_DRIVEMOTOR
enum
{
	STOP = 0x00,
	HOLD,
	ADVANCE,
	REVERSE,
	TURN
	
};
	


#include "drivemotor.h"
void stop(uint8_t *params);
void hold(uint8_t *params);
void advance(uint8_t *params);
void reverse(uint8_t *params);
void turn(uint8_t *params);
void steering(uint8_t *params);
#endif



ros_t ros_handle;




void rosServerInit(void)
{
	rosInit();
	rosLoadDriver(&ros_handle);
	rosOpen(&ros_handle, _DEF_ROS0, 38400);
	rosAddService(&ros_handle, stop);
	rosAddService(&ros_handle, hold);
	rosAddService(&ros_handle, advance);
	rosAddService(&ros_handle, reverse);
	rosAddService(&ros_handle, turn);
	rosAddService(&ros_handle, steering);
	//uint8_t buf = ros_handle.service_index;
	//ros_handle.driver.write(ros_handle.ch, &buf, 1);
}

void rosServerRun(void)
{
	//char *str = "hello";
	//rosSendInst(&ros_handle, 0x00, 0x00, (uint8_t*)str, 6);
	
	if (rosReceivePacket(&ros_handle))
	{
		
		if ((ros_handle.packet.msgs[0] < 0 || ros_handle.packet.msgs[0] >= ROS_MAX_SERVICE) && ros_handle.packet.msg_len <= 0)
		{
			return;
		}
		//ros_handle.driver.write(ros_handle.ch, (uint8_t*)&(ros_handle.packet.msgs[0]), 1);
		rosCallService(&ros_handle, ros_handle.packet.msgs[0], &ros_handle.packet.msgs[1]);
	}
}


#ifdef _USE_HW_DRIVEMOTOR

void stop(uint8_t *params)
{
	motorBreak();
	_delay_ms(100);
	motorStop();
}

void hold(uint8_t *params)
{
	motorBreak();
}

void advance(uint8_t *params)
{
	motorSetSpeed(params[0]);
	motorAdvance();
}

void reverse(uint8_t *params)
{
	motorSetSpeed(params[0]);
	motorGoBackward();
}

void turn(uint8_t *params)
{
	motorSetSpeed(params[1]);
	motorSpin(params[0]);
}

void steering(uint8_t *params)
{
	motorSetSpeed(params[1]);
	motorSteering(params[0]);
}



#endif

#endif