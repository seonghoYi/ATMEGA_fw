#include "main.h"
#include "atmega128_it.h"


#ifdef _USE_HW_UART
extern UART_HandleTypeDef huart1;
ISR(USART0_RX_vect)
{
	UART_RxIRQHandler(&huart1);
}

ISR(USART0_TX_vect)
{
	UART_TxIRQHandler(&huart1);
}

#endif


#ifdef _USE_HW_TIM
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

ISR(TIMER0_OVF_vect)
{
	TIM0_OVF_IRQHandler(&htim1);
}

ISR(TIMER0_COMP_vect)
{
	TIM0_OC_IRQHandler(&htim1);
}

ISR(TIMER1_OVF_vect)
{
	
}

ISR(TIMER1_COMPA_vect)
{
	
}

ISR(TIMER1_COMPB_vect)
{
	
}

ISR(TIMER1_COMPC_vect)
{
	
}

ISR(TIMER1_CAPT_vect)
{
	
}

ISR(TIMER2_OVF_vect)
{
	TIM2_OVF_IRQHandler(&htim2);
}

ISR(TIMER2_COMP_vect)
{
	TIM2_OC_IRQHandler(&htim2);
}

ISR(TIMER3_OVF_vect)
{
	
}

ISR(TIMER3_COMPA_vect)
{
	
}

ISR(TIMER3_COMPB_vect)
{
	
}

ISR(TIMER3_COMPC_vect)
{
	
}

ISR(TIMER3_CAPT_vect)
{
	
}
#endif