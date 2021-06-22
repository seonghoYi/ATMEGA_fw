#include "main.h"

#ifdef _USE_HW_UART
ISR(USART0_RX_vect)
{
	UART_RxCpltCallback(_DEF_UART1);
}

ISR(USART0_TX_vect)
{

}

ISR(USART1_RX_vect)
{
	UART_RxCpltCallback(_DEF_UART2);
}

ISR(USART1_TX_vect)
{

}

#endif


#ifdef _USE_HW_TIM
#ifndef _USE_HW_SYSTICK
ISR(TIMER0_OVF_vect)
{
	
}

ISR(TIMER0_COMP_vect)
{

}
#endif

#ifdef _USE_HW_SYSTICK
ISR(TIMER0_COMP_vect)
{
	incTick();
}
#endif

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

}

ISR(TIMER2_COMP_vect)
{

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