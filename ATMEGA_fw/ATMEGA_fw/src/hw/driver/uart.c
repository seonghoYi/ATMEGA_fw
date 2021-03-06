#include "uart.h"
#include "qbuffer.h"

#ifdef _USE_HW_UART

#define UART_BUF_MAX_SIZE	256

static bool is_open[UART_MAX_CH];

static qbuffer_t qbuffer[UART_MAX_CH];
static uint8_t rx_buf[UART_MAX_CH][UART_BUF_MAX_SIZE];
static uint8_t rx_data[UART_MAX_CH];

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;


bool uartInit(void)
{
	for (int i=0; i<UART_MAX_CH; i++)
	{
		is_open[i] = false;
	}

	return true;
}


bool uartOpen(uint8_t ch, uint32_t baud)
{
	bool ret = false;


	switch(ch)
	{
		case _DEF_UART0:
		huart1.USARTn				= USART0;
		huart1.Init.BaudRate		= baud;
		huart1.Init.StopBits		= UART_STOPBITS_1;
		huart1.Init.Parity			= UART_PARITY_NONE;
		huart1.Init.WordLength		= UART_WORDLENGTH_8B;
		huart1.Init.Mode			= UART_MODE_RX_TX;
		huart1.Init.OverSampling	= UART_OVERSAMPLING_8;

		qbufferCreate(&qbuffer[_DEF_UART0], (uint8_t *)&rx_buf[_DEF_UART0][0], UART_BUF_MAX_SIZE);
		
		

		if (UART_Init(&huart1) != HAL_OK)
		{
			ret = false;
		}
		else
		{
			ret = true;
			is_open[_DEF_UART0] = true;
			
			if (UART_Receive_IT(&huart1, (uint8_t *)&rx_data[_DEF_UART0], 1) != HAL_OK)
			{
				ret = false;
			}
		}
		break;
		case _DEF_UART1:
		huart2.USARTn				= USART1;
		huart2.Init.BaudRate		= baud;
		huart2.Init.StopBits		= UART_STOPBITS_1;
		huart2.Init.Parity			= UART_PARITY_NONE;
		huart2.Init.WordLength		= UART_WORDLENGTH_8B;
		huart2.Init.Mode			= UART_MODE_RX_TX;
		huart2.Init.OverSampling	= UART_OVERSAMPLING_8;

		qbufferCreate(&qbuffer[_DEF_UART1], (uint8_t *)&rx_buf[_DEF_UART1][0], UART_BUF_MAX_SIZE);
		
		

		if (UART_Init(&huart2) != HAL_OK)
		{
			ret = false;
		}
		else
		{
			ret = true;
			is_open[_DEF_UART1] = true;
			
			if (UART_Receive_IT(&huart2, (uint8_t *)&rx_data[_DEF_UART1], 1) != HAL_OK)
			{
				ret = false;
			}
		}
		break;
	}
	return ret;
}

uint32_t uartAvailable(uint8_t ch)
{
	uint32_t ret = 0;

	switch(ch)
	{
		case _DEF_UART0:
		ret = qbufferAvailable(&qbuffer[_DEF_UART0]);
		break;
		case _DEF_UART1:
		ret = qbufferAvailable(&qbuffer[_DEF_UART1]);
		break;
	}
	return ret;
}

uint8_t uartRead(uint8_t ch)
{
	uint8_t ret = 0;
	
	switch(ch)
	{
		case _DEF_UART0:
		if (qbufferRead(&qbuffer[_DEF_UART0], &ret, 1) == false)
		{
			ret = -1;
		}
		break;
		case _DEF_UART1:
		if (qbufferRead(&qbuffer[_DEF_UART1], &ret, 1) == false)
		{
			ret = -1;
		}
		break;
	}
	//qbufferRead(&qbuffer[ch], &ret, 1);
	return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
	uint32_t ret = 0;
	HAL_StatusTypeDef status;
	switch(ch)
	{
		case _DEF_UART0:
		status = UART_Transmit(&huart1, p_data, length, 100);
		if (status == HAL_OK)
		{
			ret = length;
		}
		break;
		case _DEF_UART1:
		status = UART_Transmit(&huart2, p_data, length, 100);
		if (status == HAL_OK)
		{
			ret = length;
		}
		break;
	}
	return ret;
}

uint32_t uartPrintf(uint8_t ch, char *fmt, ...)
{
	char buf[256];
	va_list args;
	int len;
	uint32_t ret;

	va_start(args, fmt);
	len = vsnprintf(&buf[0], 256, fmt, args);

	ret = uartWrite(ch, (uint8_t *)&buf[0], len);

	va_end(args);
	return ret;
}

uint32_t uartGetBaud(uint8_t ch)
{
	uint32_t baud = 0;
	switch(ch)
	{
		case _DEF_UART0:
		baud = huart1.Init.BaudRate;
		break;
		case _DEF_UART1:
		baud = huart2.Init.BaudRate;
		break;
	}
	return baud;
}

bool uartFlush(uint8_t ch)
{
	bool ret = true;
	
	switch(ch)
	{
		case _DEF_UART0:
		qbufferFlush(&qbuffer[_DEF_UART0]);
		break;
		case _DEF_UART1:
		qbufferFlush(&qbuffer[_DEF_UART1]);
		break;
	}
	
	return ret;
}


void UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	return;
}

void UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	switch(huart->USARTn)
	{
		case USART0:
		qbufferWrite(&qbuffer[_DEF_UART0], huart->pRxBuffPtr, huart->RxXferSize);
		//uartPrintf(_DEF_UART0, "Rx callback available: %d", uartAvailable(_DEF_UART0));
		//uartPrintf(_DEF_UART0, "Rx callback called\n");
		break;
		case USART1:
		qbufferWrite(&qbuffer[_DEF_UART1], huart->pRxBuffPtr, huart->RxXferSize);
		//uartPrintf(_DEF_UART1, "Rx callback available: %d", uartAvailable(_DEF_UART1));
		//uartPrintf(_DEF_UART0, "Rx callback called\n");
		break;
		default:
		break;
	}
}



#endif