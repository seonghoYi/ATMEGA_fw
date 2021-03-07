#include "main.h"
#include "atmega128_it.h"


extern UART_HandleTypeDef huart1;


ISR(USART0_RX_vect)
{
	UART_RxIRQHandler(&huart1);
}

ISR(USART0_TX_vect)
{
	UART_TxIRQHandler(&huart1);
}
