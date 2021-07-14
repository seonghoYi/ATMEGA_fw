#include "ap.h"


volatile int L_edge;
volatile int R_edge;

volatile double L_interval_time;
volatile double L_starttick;
volatile double R_interval_time;
volatile double R_starttick;

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

void apMain(void)
{	
	gpioPinWrite(_DEF_GPIO_MOTOR_EN, true);
	gpioPinWrite(_DEF_GPIO_MOTOR_DIR_L, true);
	gpioPinWrite(_DEF_GPIO_MOTOR_DIR_R, false);
	
	int rpm_goal = 30;
	
	
	motorSetLeftDirection(true);
	motorSetRightDirection(true);
	motorRun();
	
	uint32_t L_rpm = 0;
	uint32_t R_rpm = 0;
	int starttick = millis();
	//L_starttick = (double)millis();
	//R_starttick = (double)millis();
	
	int kp = 50;
	int ki = 0;
	int kd = 0;
	
	int ep_LeftTerm = 0;
	int ei_LeftTerm = 0;
	int ed_LeftTerm = 0;
	int ep_RightTerm = 0;
	int ei_RightTerm = 0;
	int ed_RightTerm = 0;
	
	int prevLeftError = 0;
	int prevRightError = 0;
	
	while(true)
	{	
		int dt = millis() - starttick;
		if (dt > 100)
		{
			L_rpm = (60 * L_edge) / (0.1 * 254.4);
			R_rpm = (60 * R_edge) / (0.1 * 254.4);
			L_edge = 0;
			R_edge = 0;
			
			starttick = millis();
		}
		
		
		
		ep_LeftTerm = rpm_goal - L_rpm;
		ep_RightTerm = rpm_goal - R_rpm;
		
		if (ei_LeftTerm <= 100 && ei_LeftTerm >= -100 )
		{
			ei_LeftTerm += ep_LeftTerm * dt;
		}
		
		if (ei_RightTerm <= 100 && ei_RightTerm >= -100 )
		{
			ei_RightTerm += ep_RightTerm * dt;
		}
		
		ed_LeftTerm = -(ep_LeftTerm - prevLeftError) * dt;
		ed_RightTerm = -(ep_RightTerm - prevRightError) * dt;
		
		
		prevLeftError = ep_LeftTerm;
		prevRightError = ep_RightTerm;
		
		int Left_duty = kp * ep_LeftTerm + ki * ei_LeftTerm + kd * ed_LeftTerm;
		int Right_duty = kp * ep_RightTerm + ki * ei_RightTerm + kd * ed_RightTerm;
		
		motorSetLeftDirection(50 + Left_duty);
		motorSetRightDirection(50 + Right_duty);
		
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




void INT5_Callback(void)
{
	L_edge++;
	//L_interval_time = (double)millis() - L_starttick;
	//L_starttick = (double)millis();
	//uartPrintf(_DEF_UART0, "L_INT, %ld\n", millis());
}


void INT6_Callback(void)
{
	R_edge++;
	//R_interval_time = (double)millis() - R_starttick;
	//R_starttick = (double)millis();
	//uartPrintf(_DEF_UART0, "R_INT, %ld\n", millis());
}