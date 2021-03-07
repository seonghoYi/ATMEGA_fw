#include "atmega128_DRV.h"



#ifdef DRV_UART_MODULE_ENABLED

USART_TypeDef USART_descripter[] = {
	{&UDR0, &UCSR0A, &UCSR0B, &UCSR0C, &UBRR0H, &UBRR0L},
	{&UDR1, &UCSR1A, &UCSR1B, &UCSR1C, &UBRR1H, &UBRR1L},	
};

StatusTypeDef UART_Init(UART_HandleTypeDef *huart)
{
	uint32_t temp;
	USART_TypeDef *usart = &USART_descripter[huart->USARTn];
	if (huart == NULL)
	{
		return ERROR;
	}
	
	
	switch(huart->Init.OverSampling)
	{
		case UART_OVERSAMPLING_8:
		temp = (float)(F_CPU / (float)(16 * huart->Init.BaudRate)) - 0.5; //-1.0 + 0.5 => for rounding
		*(usart->UBRRnH) = (temp >> 8) & 0xFF;
		*(usart->UBRRnL) = temp & 0xFF;
		CLRB(*(usart->UCSRnA), 1);
		break;
		case UART_OVERSAMPLING_16:
		temp = (float)(F_CPU / (float)(8 * huart->Init.BaudRate)) - 0.5;
		*(usart->UBRRnH) = (temp >> 8) & 0xFF;
		*(usart->UBRRnL) = temp & 0xFF;
		SETB(*(usart->UCSRnA), 1);
		break;
		default:
		break;
	}
	
	switch(huart->Init.WordLength)
	{
		case UART_WORDLENGTH_8B:
		CLRB(*(usart->UCSRnB), 2);
		SETB(*(usart->UCSRnC), 2);
		SETB(*(usart->UCSRnC), 1);
		break;
		case UART_WORDLENGTH_5B:
		CLRB(*(usart->UCSRnB), 2);
		CLRB(*(usart->UCSRnC), 2);
		CLRB(*(usart->UCSRnC), 1);
		break;
		case UART_WORDLENGTH_6B:
		CLRB(*(usart->UCSRnB), 2);
		CLRB(*(usart->UCSRnC), 2);
		SETB(*(usart->UCSRnC), 1);
		break;
		case UART_WORDLENGTH_7B:
		CLRB(*(usart->UCSRnB), 2);
		SETB(*(usart->UCSRnC), 2);
		CLRB(*(usart->UCSRnC), 1);
		break;
		case UART_WORDLENGTH_9B:
		SETB(*(usart->UCSRnB), 2);
		SETB(*(usart->UCSRnC), 2);
		SETB(*(usart->UCSRnC), 1);
		break;
		default:
		break;
	}
	
	switch(huart->Init.StopBits)
	{
		case UART_STOPBITS_1:
		CLRB(*(usart->UCSRnC), 4);
		break;
		case UART_STOPBITS_2:
		SETB(*(usart->UCSRnC), 4);
		break;
		default:
		break;
	}
	
	switch(huart->Init.Parity)
	{
		case UART_PARITY_NONE:
		CLRB(*(usart->UCSRnC), 6);
		CLRB(*(usart->UCSRnC), 5);
		break;
		case UART_PARITY_EVEN:
		SETB(*(usart->UCSRnC), 6);
		CLRB(*(usart->UCSRnC), 5);
		break;
		case  UART_PARITY_ODD:
		SETB(*(usart->UCSRnC), 6);
		SETB(*(usart->UCSRnC), 5);
		break;
		default:
		break;
	}
	
	
	switch(huart->Init.Mode)
	{
		case UART_MODE_RX:
		SETB(*(usart->UCSRnB), 4);
		break;
		case UART_MODE_TX:
		SETB(*(usart->UCSRnB), 3);
		break;
		case UART_MODE_RX_TX:
		SETB(*(usart->UCSRnB), 3);
		SETB(*(usart->UCSRnB), 4);
		break;
		default:
		break;
	}
	
	
	return OK;
}

StatusTypeDef UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	uint8_t			*pdata8bits		= NULL;
	uint16_t		*pdata16bits	= NULL;
	USART_TypeDef	*usart			= &USART_descripter[huart->USARTn];
	
	if (usart == NULL)
	{
		return ERROR;
	}
	
	huart->ErrorCode			= UART_ERROR_NONE;
	
	huart->TxXferSize			= Size;
	huart->TxXferCount			= Size;
	

	
	if (huart->Init.WordLength == UART_WORDLENGTH_9B && huart->Init.Parity == UART_PARITY_NONE)
	{
		pdata8bits	= NULL;
		pdata16bits = (uint16_t *) pData;
	}
	else
	{
		pdata8bits	= pData;
		pdata16bits = NULL;
	}
	

	for(int i = 0; i < huart->TxXferCount; i++)
	{
		while (!(*(usart->UCSRnA) & 0x20));
		if (pdata16bits == NULL)
		{
			*(usart->UDRn) = *(pdata8bits + i);
		}
		else
		{
			*(usart->UDRn) = *(pdata16bits + i);
		}
		
	}
	
	
	return OK;
}

StatusTypeDef UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	uint8_t			*pdata8bits		= NULL;
	uint16_t		*pdata16bits	= NULL;
	USART_TypeDef	*usart			= &USART_descripter[huart->USARTn];
	
	if (usart == NULL)
	{
		return ERROR;
	}
	
	huart->ErrorCode				= UART_ERROR_NONE;
	
	huart->RxXferSize				= Size;
	huart->RxXferCount				= Size;
	
	
	if (huart->Init.WordLength == UART_WORDLENGTH_9B && huart->Init.Parity == UART_PARITY_NONE)
	{
		pdata8bits	= NULL;
		pdata16bits = (uint16_t *) pData;
	}
	else
	{
		pdata8bits	= pData;
		pdata16bits = NULL;
	}

	for(int i = 0; i < huart->TxXferCount; i++)
	{
		while (!(*(usart->UCSRnA) & 0x80) && !(*(usart->UCSRnA) & 0x20));
		if (pdata16bits == NULL)
		{
			*(pdata8bits + i) = *(usart->UDRn);
		}
		else
		{
			*(pdata16bits + i) = *(usart->UDRn);
		}
	}

	return OK;
}

StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	USART_TypeDef *usart = &USART_descripter[huart->USARTn];
	
	if (usart == NULL)
	{
		return ERROR;
	}
	
	huart->pTxBuffPtr	= pData;
	huart->TxXferSize	= Size;
	huart->TxXferCount	= Size;
	
	SETB(*(usart->UCSRnB), 6);
	return OK;
}

StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
	USART_TypeDef *usart = &USART_descripter[huart->USARTn];
	
	if (usart == NULL)
	{
		return ERROR;
	}
	
	huart->pRxBuffPtr	= pData;
	huart->RxXferSize	= Size;
	huart->RxXferCount	= Size;
	
	SETB(*(usart->UCSRnB), 7);
	
	return OK;
}



void UART_TxIRQHandler(UART_HandleTypeDef *huart)
{
	UART_TxCpltCallback(huart);
}

void UART_RxIRQHandler(UART_HandleTypeDef *huart)
{
	UART_Receive(huart, huart->pRxBuffPtr, huart->RxXferSize, 100);
	UART_RxCpltCallback(huart);
}

__attribute__((weak)) void UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	return;
}

__attribute__((weak)) void UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	return;
}







#endif