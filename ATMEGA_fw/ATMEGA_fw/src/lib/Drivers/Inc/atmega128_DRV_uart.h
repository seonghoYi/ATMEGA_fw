#ifndef ATMEGA128_DRV_UART_H_
#define ATMEGA128_DRV_UART_H_

#include "atmega128_DRV_def.h"

typedef struct
{
	uint32_t BaudRate;
	
	uint8_t WordLength;
	
	uint8_t StopBits;
	
	uint8_t Parity;
	
	uint8_t Mode;
	
	uint8_t OverSampling;
	
} UART_InitTypeDef;


typedef enum
{
	UART_STATE_RESET			= 0x00,
	
	UART_STATE_READY			= 0x01,
	
	UART_STATE_BUSY				= 0X02,
	
	UART_STATE_BUSY_TX			= 0x03,
	
	UART_STATE_BUSY_RX			= 0x04,
	
	UART_STATE_BUSY_TX_RX		= 0x05,
	
	UART_STATE_BUSY_TIMEOUT		= 0x06,
	
	UART_STATE_BUSY_ERROR		= 0xE0
} UART_StateTypeDef;

typedef struct
{
	uint8_t USARTn;

	UART_InitTypeDef Init;
	
	uint8_t *pTxBuffPtr;
	uint8_t *pRxBuffPtr;
	
	uint16_t TxXferSize;
	uint16_t RxXferSize;
	
	uint16_t TxXferCount;
	uint16_t RxXferCount;
	
	uint8_t	ErrorCode;
} UART_HandleTypeDef;


#define UART_ERROR_NONE			0x00


#define UART_WORDLENGTH_8B		0x00
#define UART_WORDLENGTH_5B		0x01
#define	UART_WORDLENGTH_6B		0x02
#define UART_WORDLENGTH_7B		0x03
#define	UART_WORDLENGTH_9B		0x04


#define UART_STOPBITS_1			0x00
#define UART_STOPBITS_2			0x01

#define UART_PARITY_NONE		0x00
#define UART_PARITY_EVEN		0x01
#define UART_PARITY_ODD			0x02

#define UART_OVERSAMPLING_8		0x00
#define UART_OVERSAMPLING_16	0x01

#define UART_MODE_RX			0x00
#define UART_MODE_TX			0x01
#define UART_MODE_RX_TX			0x02



StatusTypeDef UART_Init(UART_HandleTypeDef *huart);
StatusTypeDef UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
StatusTypeDef UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);

void UART_TxIRQHandler(UART_HandleTypeDef *huart);
void UART_RxIRQHandler(UART_HandleTypeDef *huart);
void UART_TxCpltCallback(UART_HandleTypeDef *huart);
void UART_RxCpltCallback(UART_HandleTypeDef *huart);

#endif /* ATMEGA128_DRV_UART_H_ */