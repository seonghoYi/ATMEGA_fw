#include "ap.h"

#define P_GAIN	1.1f //1.1f
#define I_GAIN	0.9f //0.9f
#define D_GAIN	0.12f //0.12f 

//ziegler-nichols method Kcr=2.0 Pcr=0.4 Kp=1.2, Ki=0.6, Kd=0.15 dt=0.1
//ziegler-nichols method Kcr=2.0 Pcr=0.3 Kp=1.5, Ki=0.9, Kd=0.06 dt=0.1


typedef struct
{
	volatile int L_edge;
	volatile int R_edge;
} encoder_pulse_t; // M method

typedef struct 
{
	float Left_rpm_goal;
	float Right_rpm_goal;
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
	hc05Open(_DEF_UART0, 9600);
	sei();
}


void rosServerInit(ros_t *p_ros);
void rosServerRun(ros_t *p_ros);
void calculatePID(pid_t *p_pid_variables);
void rosPublishRPM(ros_t *p_ros, pid_t *p_pid_variables);
void rosBTCallback(ros_t *p_ros);

void apMain(void)
{	
	pid_t pid_variables; //pid 구조체 선언
	pid_variables.Left_rpm_goal = 0;
	pid_variables.Right_rpm_goal = 0;
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
	// 구조체 초기화
	
	ros_t ros_handle; //ros 구조체 선언
	rosServerInit(&ros_handle); //ros 서비스 초기화
	
	//motorSetLeftDirection(true);
	//motorSetRightDirection(true);
	//motorRun();
	//motorSetSpeed(40);
	
	motorStop();

	
	int starttick = millis();
	ros_handle.pre_time = (uint32_t)starttick;
	
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
		if (dt >= 100) //0.1초마다 동작
		{
			//uartPrintf(_DEF_UART0, "%lu\n", millis());
			starttick = millis(); // 0.000007s
			pid_variables.dt = (float)dt/1000; // 0.000066625s //float 로 형변환해주지 않으면 계산적 오류 발생
			
			calculatePID(&pid_variables);
			rosPublishRPM(&ros_handle, &pid_variables);
		}
		
		
		rosServerRun(&ros_handle);
		rosBTCallback(&ros_handle);
		//uartPrintf(_DEF_UART1, "ok\n");
	}
}


/*---------------------------------ROS---------------------------------*/
static float left_goal;
static float right_goal;
static bool left_dir;
static bool right_dir;

void rosStopMotor(uint8_t *params);
void rosRunMotor(uint8_t *params);
void rosSetLeftSpeed(uint8_t *params);
void rosSetRightSpeed(uint8_t *params);
void rosSetLeftDirection(uint8_t *params);
void rosSetRightDirection(uint8_t *params);
void rosBTSetConfigMode(uint8_t *params);
void rosBTClearConfigMode(uint8_t *params);
void rosBTWrite(uint8_t *params);
void rosSuctionRun(uint8_t *params);
void rosSuctionStop(uint8_t *params);
void rosWriteServo(uint8_t *params);

void rosServerInit(ros_t *p_ros)
{
	rosLoadDriver(p_ros);
	rosOpen(p_ros, _DEF_ROS0, 38400);
		
	rosAddService(p_ros, rosStopMotor);
	rosAddService(p_ros, rosRunMotor);
	rosAddService(p_ros, rosSetLeftSpeed);
	rosAddService(p_ros, rosSetRightSpeed);
	rosAddService(p_ros, rosSetLeftDirection);
	rosAddService(p_ros, rosSetRightDirection);
	rosAddService(p_ros, rosBTSetConfigMode);
	rosAddService(p_ros, rosBTClearConfigMode);
	rosAddService(p_ros, rosBTWrite);
	rosAddService(p_ros, rosSuctionRun);
	rosAddService(p_ros, rosSuctionStop);
	rosAddService(p_ros, rosWriteServo);
	
	
	
	//left_goal = 20;
	//right_goal = 20;
}


void rosServerRun(ros_t *p_ros)
{
	uint8_t service_id;
	//p_ros->driver.write(p_ros->ch, &service_id, 1);
	//uartPrintf(_DEF_UART1, "ok\n");
	if (rosReceivePacket(p_ros))
	{
		service_id = p_ros->packet.inst;
		//p_ros->driver.write(p_ros->ch, &service_id, 1);
		//hc05Printf("Call service: %X\n", service_id); //디버그 용
		if ((service_id < 0 || service_id >= ROS_MAX_SERVICE))
		{
			return;
		}
		//p_ros->driver.write(p_ros->ch, (uint8_t *)&service_id, 1);
		//p_ros->driver.write(p_ros->ch, (uint8_t*)&(p_ros->packet.msgs[0]), 1);
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
	left_goal = params[0];
	hc05Printf("%d\n", (int)left_goal);
	//motorSetLeftSpeed((uint16_t)params[0]);
}

void rosSetRightSpeed(uint8_t *params)
{
	right_goal = params[0];
	hc05Printf("%d\n", (int)right_goal);
	//motorSetRightSpeed((uint16_t)params[0]);
}

void rosSetLeftDirection(uint8_t *params)
{
	left_dir = (bool)params[0];
	motorSetLeftDirection(params[0]);
}

void rosSetRightDirection(uint8_t *params)
{
	right_dir = (bool)params[0];
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

void rosSuctionRun(uint8_t *params)
{
	suctionMotorSetSpeed(99);
	suctionMotorRun();
}

void rosSuctionStop(uint8_t *params)
{
	/*
	suctionMotorSetSpeed(10);
	suctionMotorRun();
	*/
	suctionMotorStop();
}

void rosWriteServo(uint8_t *params)
{
	switch(params[0])
	{
		case 0:
			servoWrite(_DEF_SERVO_1, params[1]);
		break;
		case 1:
			servoWrite(_DEF_SERVO_2, params[1]);
		break;
	}
}

/*---------------------------------ROS_END---------------------------------*/


void rosBTCallback(ros_t *p_ros)
{
	uint8_t buf;
	if (hc05Available() > 0)
	{
		buf = hc05Read();
		rosSendInst(p_ros, 2, 1, &buf, 1);
	}
}




/*---------------------------------PID_CONTROL---------------------------------*/
#define ALPHA	0.98 //LFP 알파값

void calculatePID(pid_t *p_pid_variables)
{
	float L_rpm = 0;
	float R_rpm = 0;
	L_rpm = 60 * encoder_pulse.L_edge * 2 / (254.4 * p_pid_variables->dt);
	R_rpm = 60 * encoder_pulse.R_edge * 2 / (254.4 * p_pid_variables->dt);
	//M 방식 rpm 변환 edge가 2개 이므로 2를 곱함
	
	//hc05Printf("%d, %d\n", encoder_pulse.L_edge, encoder_pulse.R_edge);

	encoder_pulse.L_edge = 0;
	encoder_pulse.R_edge = 0;
	
	if (p_pid_variables->Left_rpm_goal != left_goal || p_pid_variables->Right_rpm_goal != right_goal)
	{
		
		p_pid_variables->ep_LeftTerm = 0;
		p_pid_variables->ep_RightTerm = 0;
		p_pid_variables->ei_LeftTerm = 0;
		p_pid_variables->ei_RightTerm = 0;
		p_pid_variables->ed_LeftTerm = 0;
		p_pid_variables->ed_RightTerm = 0;
		p_pid_variables->prevLeftError = 0;
		p_pid_variables->prevRightError = 0;
		
		
		
		/*
		p_pid_variables->prev_Lrpm = 0;
		p_pid_variables->prev_Rrpm = 0;
		
		L_rpm = 0;
		R_rpm = 0;
		*/
		p_pid_variables->Left_rpm_goal = left_goal;
		p_pid_variables->Right_rpm_goal = right_goal; //목표 rpm 업데이트
		
	}
	
	//p_pid_variables->Left_rpm_goal = left_goal;
	//p_pid_variables->Right_rpm_goal = right_goal; //목표 rpm 업데이트

	
	//R_rpm = L_rpm;
	//L_rpm = R_rpm;
	
	
	
	/*
	if (p_pid_variables->prev_Lrpm - L_rpm > 0)
	{
		if ((p_pid_variables->prev_Lrpm - L_rpm) > 5)
		{
			L_rpm = p_pid_variables->prev_Lrpm * ALPHA + L_rpm * (1-ALPHA);
		}
	}
	else if (p_pid_variables->prev_Lrpm - L_rpm < 0)
	{
		if (-(p_pid_variables->prev_Lrpm - L_rpm) > 5)
		{
			L_rpm = p_pid_variables->prev_Lrpm * (1-ALPHA) + L_rpm * ALPHA;
		}
	}
	
	if (p_pid_variables->prev_Rrpm - R_rpm > 0)
	{
		if ((p_pid_variables->prev_Rrpm - R_rpm) > 5)
		{
			R_rpm = p_pid_variables->prev_Rrpm * ALPHA + R_rpm * (1-ALPHA);
		}
	}
	else if (p_pid_variables->prev_Rrpm - R_rpm < 0)
	{
		if (-(p_pid_variables->prev_Rrpm - R_rpm) > 5)
		{
			R_rpm = p_pid_variables->prev_Rrpm * (1-ALPHA) + R_rpm * ALPHA;
		}
	}
	*/
	
	//디지털 LFP
	/*
		현재값과 이전값의 차이가 15 이상이면 이전값을 98% 반영, 현재값은 2%만을 반영
		순간적으로 튀는 값 억제
	*/
	
	//uartPrintf(_DEF_UART0, "goal: %d, L_rpm: %d, R_rpm: %d, time: %d\n", rpm_goal, L_rpm, R_rpm, millis());
	
	
	//uartPrintf(_DEF_UART0, "Goal_rpm: %d, L_rpm: %d, R_rpm: %d\n", rpm_goal, L_rpm, R_rpm);
	
	p_pid_variables->ep_LeftTerm = p_pid_variables->Left_rpm_goal - L_rpm; // 0.000021875s
	p_pid_variables->ep_RightTerm = p_pid_variables->Right_rpm_goal - R_rpm; // 0.000021875s
	/*
	if (p_pid_variables->ei_LeftTerm < 100 && p_pid_variables->ei_LeftTerm > -100) // 0.000017s
	{
		p_pid_variables->ei_LeftTerm += p_pid_variables->ep_LeftTerm * p_pid_variables->dt; //0.00003375s
	}
	
	if (p_pid_variables->ei_RightTerm < 100 && p_pid_variables->ei_RightTerm > -100) // 0.000017s
	{
		p_pid_variables->ei_RightTerm += p_pid_variables->ep_RightTerm * p_pid_variables->dt; //0.00003375s
	}
	*/
	
	p_pid_variables->ei_LeftTerm += p_pid_variables->ep_LeftTerm * p_pid_variables->dt; //0.00003375s
	p_pid_variables->ei_RightTerm += p_pid_variables->ep_RightTerm * p_pid_variables->dt; //0.00003375s
	
	p_pid_variables->ei_LeftTerm = constrain(p_pid_variables->ei_LeftTerm, -100, 100);
	p_pid_variables->ei_RightTerm = constrain(p_pid_variables->ei_RightTerm, -100, 100);
	
	p_pid_variables->ed_LeftTerm = -(p_pid_variables->ep_LeftTerm - p_pid_variables->prevLeftError) / p_pid_variables->dt; //0.000032625s
	p_pid_variables->ed_RightTerm = -(p_pid_variables->ep_RightTerm - p_pid_variables->prevRightError) / p_pid_variables->dt; //0.000032625s

	p_pid_variables->ed_LeftTerm = constrain(p_pid_variables->ed_LeftTerm, -50, 50);
	p_pid_variables->ed_RightTerm = constrain(p_pid_variables->ed_RightTerm, -50, 50);
	//미분의 결과는 값이 커서 제어에 악영향을 줄 수 있으므로 -50 ~ 50의 범위로 제한
	
	//uartPrintf(_DEF_UART0, "ep_l: %d, ep_r: %d, ei_l: %d, ei_r: %d, ed_l: %d, ed_r: %d\n", ep_LeftTerm, ep_RightTerm, ei_LeftTerm, ei_RightTerm, ed_LeftTerm, ed_RightTerm);
	
	p_pid_variables->prevLeftError = p_pid_variables->ep_LeftTerm; // 0.00000325s
	p_pid_variables->prevRightError = p_pid_variables->ep_RightTerm; // 0.00000325s
	
	float Left_duty = (P_GAIN * p_pid_variables->ep_LeftTerm + I_GAIN * p_pid_variables->ei_LeftTerm + D_GAIN * p_pid_variables->ed_LeftTerm); //0.00014725s
	float Right_duty = (P_GAIN * p_pid_variables->ep_RightTerm + I_GAIN * p_pid_variables->ei_RightTerm + D_GAIN * p_pid_variables->ed_RightTerm); //0.00014725s
	
	//uartPrintf(_DEF_UART0, "goal: %d, Left rpm: %d, Right rpm: %d, Left duty: %d, Right duty: %d\n", rpm_goal, L_rpm, R_rpm, Left_duty, Right_duty);
	
	//uartPrintf(_DEF_UART0, "goal: %d, Left rpm: %d, Right rpm: %d, ep_l: %d, ep_r: %d, ei_l: %d, ei_r: %d, ed_l: %d, ed_r: %d, time: %lu\n", p_pid_variables->Left_rpm_goal, (int)L_rpm, (int)R_rpm, (int)(P_GAIN * p_pid_variables->ep_LeftTerm / 100), (int)(P_GAIN * p_pid_variables->ep_RightTerm / 100), (int)(I_GAIN * p_pid_variables->ei_LeftTerm / 100), (int)(I_GAIN * p_pid_variables->ei_RightTerm / 100), (int)(D_GAIN * p_pid_variables->ed_LeftTerm / 100), (int)(D_GAIN * p_pid_variables->ed_RightTerm / 100), millis());
	//hc05Printf("Left rpm: %d, Right rpm: %d, ep_l: %d, ep_r: %d, ei_l: %d, ei_r: %d, ed_l: %d, ed_r: %d\n", (int)L_rpm, (int)R_rpm, (int)(P_GAIN * p_pid_variables->ep_LeftTerm / 100), (int)(P_GAIN * p_pid_variables->ep_RightTerm / 100), (int)(I_GAIN * p_pid_variables->ei_LeftTerm / 100), (int)(I_GAIN * p_pid_variables->ei_RightTerm / 100), (int)(D_GAIN * p_pid_variables->ed_LeftTerm / 100), (int)(D_GAIN * p_pid_variables->ed_RightTerm / 100));
	
	int duty_L = p_pid_variables->Left_rpm_goal + (int)(Left_duty + 0.5); // 0.000027125s
	int duty_R = p_pid_variables->Right_rpm_goal + (int)(Right_duty + 0.5); // 0.000027125s
	//int duty_L = (int)(Left_duty + 0.5); // 0.000027125s
	//int duty_R = (int)(Right_duty + 0.5); // 0.000027125s
	//실수를 정수로 변환하는 과정에서 손실되는 소수점을 반올림 처리
	
	duty_L = constrain(duty_L, 0, 100);
	duty_R = constrain(duty_R, 0, 100);
	
	//hc05Printf("%d, %d\n", duty_L, duty_R);
	
	//uartPrintf(_DEF_UART0, "goal: %d, Left rpm: %d, Right rpm: %d, Left duty: %d, Right duty: %d\n", rpm_goal, L_rpm, R_rpm, duty_L, duty_R);
	//hc05Printf("Left rpm: %d, Right rpm: %d, Left duty: %d, Right duty: %d\n", (int)L_rpm, (int)R_rpm, duty_L, duty_R);
	
	motorSetLeftSpeed(duty_L);
	motorSetRightSpeed(duty_R);
	
	p_pid_variables->prev_Lrpm = L_rpm; 
	p_pid_variables->prev_Rrpm = R_rpm;
	//다음 계산을 위해서 현재값 보존
}


void rosPublishRPM(ros_t *p_ros, pid_t *p_pid_variables)
{
	uint8_t msg[] = {(uint8_t)left_dir, (uint8_t)right_dir, ((uint16_t)p_pid_variables->prev_Lrpm) & 0xFF, ((uint16_t)p_pid_variables->prev_Lrpm >> 8) & 0xFF, ((uint16_t)p_pid_variables->prev_Rrpm) & 0xFF, ((uint16_t)p_pid_variables->prev_Rrpm >> 8) & 0xFF};
	rosSendInst(p_ros, 2, 0, &msg[0], 6); // Little endian 방식으로 전송
	hc05Printf("goal: %d, L_rpm: %d, R_rpm: %d, time:%lu\n", (int)p_pid_variables->Right_rpm_goal, left_dir ? -(int)p_pid_variables->prev_Lrpm : (int)p_pid_variables->prev_Lrpm, right_dir ? -(int)p_pid_variables->prev_Rrpm : (int)p_pid_variables->prev_Rrpm, (uint32_t)millis());
}



void INT5_Callback(void)
{
	encoder_pulse.L_edge++; //하강엣지에서 동작
	//L_interval_time = (double)millis() - L_starttick;
	//L_starttick = (double)millis();
	//uartPrintf(_DEF_UART0, "L_INT\n");//, %ld\n", millis());
}


void INT6_Callback(void)
{
	encoder_pulse.R_edge++; //상승엣지에서 동작
	//R_interval_time = (double)millis() - R_starttick;
	//R_starttick = (double)millis();
	//uartPrintf(_DEF_UART0, "R_INT\n");//, %ld\n", millis());
}


/*---------------------------------PID_CONTROL_END---------------------------------*/