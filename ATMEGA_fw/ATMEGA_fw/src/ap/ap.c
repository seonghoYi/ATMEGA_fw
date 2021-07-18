#include "ap.h"

#define P_GAIN	50 //50
#define I_GAIN	30 //30
#define D_GAIN	30 //30

/*
volatile double L_interval_time;
volatile double L_starttick;
volatile double R_interval_time;
volatile double R_starttick;
*/
// T method

typedef struct
{
	volatile int L_edge;
	volatile int R_edge;
} encoder_pulse_t; // M method

typedef struct 
{
	int Left_rpm_goal;
	int Right_rpm_goal;
	float prev_Lrpm;
	float prev_Rrpm;
	
	float ep_LeftTerm;
	float ei_LeftTerm;
	float ed_LeftTerm;
	float ep_RightTerm;
	float ei_RightTerm;
	float ed_RightTerm;
	
	float prevLeftError;
	float prevRightError;
	
	float dt;
} pid_t;



static encoder_pulse_t	encoder_pulse;

void apInit(void)
{
	/*
	//uartOpen(_DEF_UART1, 38400);
	uartOpen(_DEF_UART0, 38400);

	hc05_t bt_handle;
	hc05Open(&bt_handle, _DEF_UART1, 38400);
	uartPrintf(_DEF_UART1, "yes\n");
	*/
	
	uartOpen(_DEF_UART0, 38400);
	sei();
}


void rosServerInit(ros_t *p_ros);
void rosServerRun(ros_t *p_ros);
void calculatePID(pid_t *p_pid_variables);


void apMain(void)
{	
	pid_t pid_variables;
	
	pid_variables.Left_rpm_goal = 20;
	pid_variables.Right_rpm_goal = 20;
	pid_variables.ep_LeftTerm = 0;
	pid_variables.ep_RightTerm = 0;
	pid_variables.ei_LeftTerm = 0;
	pid_variables.ei_RightTerm = 0;
	pid_variables.ed_LeftTerm = 0;
	pid_variables.ed_RightTerm = 0;
	pid_variables.prev_Lrpm = 0;
	pid_variables.prev_Rrpm = 0;
	pid_variables.prevLeftError = 0;
	pid_variables.prevRightError = 0;
	
	ros_t ros_handle;
	rosServerInit(&ros_handle);
	
	
	motorSetLeftDirection(true);
	motorSetRightDirection(true);
	motorRun();
	//motorSetSpeed(40);
	
	int starttick = millis();
	
	while(true)
	{
		/*
		delay(1000);
		servoWrite(_DEF_SERVO_1, 180);
		servoWrite(_DEF_SERVO_2, 0);
		delay(1000);
		servoWrite(_DEF_SERVO_1, 0);
		servoWrite(_DEF_SERVO_2, 180);
		*/
		
		int dt = millis() - starttick;
		if (dt >= 100)
		{
			starttick = millis(); // 0.000007s
			pid_variables.dt = (float)dt/1000; // 0.000066625s
			
			calculatePID(&pid_variables);
		}
	
		/*
		if (uartAvailable(_DEF_UART1))
		{
			uint8_t rx_data = uartRead(_DEF_UART1);
			uartWrite(_DEF_UART1, &rx_data, 1);
		}
		*/
		//rosServerRun();
	}
}


void rosStopMotor(uint8_t *params);
void rosRunMotor(uint8_t *params);
void rosSetLeftSpeed(uint8_t *params);
void rosSetRightSpeed(uint8_t *params);
void rosSetLeftDirection(uint8_t *params);
void rosSetRightDirection(uint8_t *params);
void rosBTSetConfigMode(uint8_t *params);
void rosBTClearConfigMode(uint8_t *params);
void rosBTWrite(uint8_t *params);
void rosBTPrintf(uint8_t *params);

void rosServerInit(ros_t *p_ros)
{
	rosInit();
	rosLoadDriver(p_ros);
	
	rosAddService(p_ros, rosStopMotor);
	rosAddService(p_ros, rosRunMotor);
	rosAddService(p_ros, rosSetLeftSpeed);
	rosAddService(p_ros, rosSetRightSpeed);
	rosAddService(p_ros, rosSetLeftDirection);
	rosAddService(p_ros, rosSetRightDirection);
	rosAddService(p_ros, rosBTSetConfigMode);
	rosAddService(p_ros, rosBTClearConfigMode);
	rosAddService(p_ros, rosBTWrite);
	rosAddService(p_ros, rosBTPrintf);
	
	rosOpen(p_ros, _DEF_ROS0, 38400);
}


void rosServerRun(ros_t *p_ros)
{
	uint8_t service_id;
	if (rosReceivePacket(p_ros))
	{
		service_id = p_ros->packet.inst;
		if ((service_id < 0 || service_id >= ROS_MAX_SERVICE))
		{
			return;
		}
		//ros_handle.driver.write(ros_handle.ch, (uint8_t*)&(ros_handle.packet.msgs[0]), 1);
		rosCallService(p_ros, service_id, &(p_ros->packet.msgs[0]));
	}
}

void rosStopMotor(uint8_t *params)
{
	motorStop();
}

void rosRunMotor(uint8_t *params)
{
	motorRun();
}

void rosSetLeftSpeed(uint8_t *params)
{
	motorSetLeftSpeed((uint16_t)params[0]);
}

void rosSetRightSpeed(uint8_t *params)
{
	motorSetRightSpeed((uint16_t)params[0]);
}

void rosSetLeftDirection(uint8_t *params)
{
	motorSetLeftDirection(params[0]);
}

void rosSetRightDirection(uint8_t *params)
{
	motorSetRightDirection(params[0]);
}

void rosBTSetConfigMode(uint8_t *params)
{
	hc05SetConfigMode();
}

void rosBTClearConfigMode(uint8_t *params)
{
	hc05ClearConfigMode();
}

void rosBTWrite(uint8_t *params)
{
	hc05Write(&params[1], params[0]);
}

void rosBTPrintf(uint8_t *params)
{
	hc05Printf(params);
}

void calculatePID(pid_t *p_pid_variables)
{
	float L_rpm = 0;
	float R_rpm = 0;
	L_rpm = 60 * encoder_pulse.L_edge / 254.4 / p_pid_variables->dt; // 0.000026s
	R_rpm = 60 * encoder_pulse.R_edge / 254.4 / p_pid_variables->dt; // 0.000026s
	//uartPrintf(_DEF_UART0, "L_rpm: %d, R_rpm: %d\n", L_rpm, R_rpm);
	//uartPrintf(_DEF_UART0, "%d\n", dt);
	encoder_pulse.L_edge = 0; // 0.0000005s
	encoder_pulse.R_edge = 0; // 0.0000005s
	
	//R_rpm = L_rpm;
	//L_rpm = R_rpm;
	
	if (p_pid_variables->prev_Lrpm - L_rpm > 0)
	{
		if ((p_pid_variables->prev_Lrpm - L_rpm) > 15)
		{
			L_rpm = p_pid_variables->prev_Lrpm * 0.98 + L_rpm * 0.02;
		}
	}
	else
	{
		if (-(p_pid_variables->prev_Lrpm - L_rpm) > 15)
		{
			L_rpm = p_pid_variables->prev_Lrpm * 0.02 + L_rpm * 0.98;
		}
	}
	
	if (p_pid_variables->prev_Rrpm - R_rpm > 0)
	{
		if ((p_pid_variables->prev_Rrpm - R_rpm) > 15)
		{
			R_rpm = p_pid_variables->prev_Rrpm * 0.98 + R_rpm * 0.02;
		}
	}
	else
	{
		if (-(p_pid_variables->prev_Rrpm - R_rpm) > 15)
		{
			R_rpm = p_pid_variables->prev_Rrpm * 0.02 + R_rpm * 0.98;
		}
	}
	
	//uartPrintf(_DEF_UART0, "goal: %d, L_rpm: %d, R_rpm: %d, time: %d\n", rpm_goal, L_rpm, R_rpm, millis());
	
	
	//uartPrintf(_DEF_UART0, "Goal_rpm: %d, L_rpm: %d, R_rpm: %d\n", rpm_goal, L_rpm, R_rpm);
	
	p_pid_variables->ep_LeftTerm = p_pid_variables->Left_rpm_goal - L_rpm; // 0.000021875s
	p_pid_variables->ep_RightTerm = p_pid_variables->Right_rpm_goal - R_rpm; // 0.000021875s
	
	if (p_pid_variables->ei_LeftTerm < 100 && p_pid_variables->ei_LeftTerm > -100) // 0.000017s
	{
		p_pid_variables->ei_LeftTerm += p_pid_variables->ep_LeftTerm * p_pid_variables->dt; //0.00003375s
	}
	
	if (p_pid_variables->ei_RightTerm < 100 && p_pid_variables->ei_RightTerm > -100) // 0.000017s
	{
		p_pid_variables->ei_RightTerm += p_pid_variables->ep_RightTerm * p_pid_variables->dt; //0.00003375s
	}
	
	p_pid_variables->ed_LeftTerm = -(p_pid_variables->ep_LeftTerm - p_pid_variables->prevLeftError) * p_pid_variables->dt; //0.000032625s
	p_pid_variables->ed_RightTerm = -(p_pid_variables->ep_RightTerm - p_pid_variables->prevRightError) * p_pid_variables->dt; //0.000032625s
	
	//uartPrintf(_DEF_UART0, "ep_l: %d, ep_r: %d, ei_l: %d, ei_r: %d, ed_l: %d, ed_r: %d\n", ep_LeftTerm, ep_RightTerm, ei_LeftTerm, ei_RightTerm, ed_LeftTerm, ed_RightTerm);
	
	p_pid_variables->prevLeftError = p_pid_variables->ep_LeftTerm; // 0.00000325s
	p_pid_variables->prevRightError = p_pid_variables->ep_RightTerm; // 0.00000325s
	
	float Left_duty = (P_GAIN * p_pid_variables->ep_LeftTerm + I_GAIN * p_pid_variables->ei_LeftTerm + D_GAIN * p_pid_variables->ed_LeftTerm) / 100; //0.00014725s
	float Right_duty = (P_GAIN * p_pid_variables->ep_RightTerm + I_GAIN * p_pid_variables->ei_RightTerm + D_GAIN * p_pid_variables->ed_RightTerm) / 100; //0.00014725s
	//uartPrintf(_DEF_UART0, "goal: %d, Left rpm: %d, Right rpm: %d, Left duty: %d, Right duty: %d\n", rpm_goal, L_rpm, R_rpm, Left_duty, Right_duty);
	
	//uartPrintf(_DEF_UART0, "goal: %d, Left rpm: %d, Right rpm: %d, ep_l: %d, ep_r: %d, ei_l: %d, ei_r: %d, ed_l: %d, ed_r: %d, time: %lu\n", rpm_goal, (int)L_rpm, (int)R_rpm, (int)ep_LeftTerm, (int)ep_RightTerm, (int)ei_LeftTerm, (int)ei_RightTerm, (int)ed_LeftTerm, (int)ed_RightTerm, millis());
	
	
	int duty_L = 50 + (int)(Left_duty + 0.5); // 0.000027125s
	int duty_R = 50 + (int)(Right_duty + 0.5); // 0.000027125s
	
	//uartPrintf(_DEF_UART0, "goal: %d, Left rpm: %d, Right rpm: %d, Left duty: %d, Right duty: %d\n", rpm_goal, L_rpm, R_rpm, duty_L, duty_R);
	
	if (duty_L <= 100 && duty_L >= 0)
	{
		motorSetLeftSpeed(duty_L);
	}
	else
	{
		duty_L = 50;
		motorSetLeftSpeed(duty_L);
	}
	
	if (duty_R <= 100 && duty_R >= 0)
	{
		motorSetRightSpeed(duty_R);
	}
	else
	{
		duty_R = 50;
		motorSetRightSpeed(duty_R);
	}
	
	p_pid_variables->prev_Lrpm = L_rpm;
	p_pid_variables->prev_Rrpm = R_rpm;
}

	


void INT5_Callback(void)
{
	encoder_pulse.L_edge++;
	//L_interval_time = (double)millis() - L_starttick;
	//L_starttick = (double)millis();
	//uartPrintf(_DEF_UART0, "L_INT\n");//, %ld\n", millis());
}


void INT6_Callback(void)
{
	encoder_pulse.R_edge++;
	//R_interval_time = (double)millis() - R_starttick;
	//R_starttick = (double)millis();
	//uartPrintf(_DEF_UART0, "R_INT\n");//, %ld\n", millis());
}
