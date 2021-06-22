#include "uart.h"
#include "qbuffer.h"

#ifdef _USE_HW_UART

#define UART_BUF_MAX_SIZE	256

static bool is_open[UART_MAX_CH];

static qbuffer_t qbuffer[UART_MAX_CH];
static uint8_t rx_buf[UART_MAX_CH][UART_BUF_MAX_SIZE];
static uint8_t rx_data[UART_MAX_CH];
static uint32_t baudrate[] = {9600, 9600, };


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
	float ubrr_f;
	uint32_t ubrr;
	
	baudrate[ch] = baud;
	ubrr_f = (float)F_CPU / (16.0 * (float)baud) - 0.5;
	ubrr = ubrr_f;
	
	if (ch > UART_MAX_CH)
	{
		return ret;
	}
	
	
	switch(ch)
	{
		case _DEF_UART1:
			WRITE_REG(UBRR0H, (ubrr >> 8) & 0xFF);
			WRITE_REG(UBRR0L, ubrr & 0xFF);
			CLR_BIT(UCSR0A, (1 << U2X0) | (1 << MPCM0));
			SET_BIT(UCSR0B, (1 << RXEN0) | (1 << TXEN0));
			CLR_BIT(UCSR0B, (1 << UCSZ02));
			SET_BIT(UCSR0C, (1 << UCSZ01) | (1 << UCSZ02));
			CLR_BIT(UCSR0C, (1 << UMSEL0) | (1 << UPM00) | (1 << UPM01) | (1 << USBS0));
			
			qbufferCreate(&qbuffer[_DEF_UART1], (uint8_t *)&rx_buf[_DEF_UART1][0], UART_BUF_MAX_SIZE);
			
			SET_BIT(UCSR0B, (1 << RXCIE0));
			
			is_open[_DEF_UART1] = true;
			ret = true;
		break;
		case _DEF_UART2:
			WRITE_REG(UBRR1H, (ubrr >> 8) & 0xFF);
			WRITE_REG(UBRR1L, ubrr & 0xFF);
			CLR_BIT(UCSR1A, (1 << U2X1) | (1 << MPCM1));
			SET_BIT(UCSR1B, (1 << RXEN1) | (1 << TXEN1));
			CLR_BIT(UCSR1B, (1 << UCSZ12));
			SET_BIT(UCSR1C, (1 << UCSZ11) | (1 << UCSZ12));
			CLR_BIT(UCSR1C, (1 << UMSEL1) | (1 << UPM10) | (1 << UPM11) | (1 << USBS1));
		
			qbufferCreate(&qbuffer[_DEF_UART2], (uint8_t *)&rx_buf[_DEF_UART2][0], UART_BUF_MAX_SIZE);
		
			SET_BIT(UCSR1B, (1 << RXCIE1));
		
			is_open[_DEF_UART2] = true;
			ret = true;
		break;
	}
	

	return ret;
}

uint32_t uartAvailable(uint8_t ch)
{
	uint32_t ret = 0;

	switch(ch)
	{
		case _DEF_UART1:
		ret = qbufferAvailable(&qbuffer[_DEF_UART1]);
		break;
		case _DEF_UART2:
		ret = qbufferAvailable(&qbuffer[_DEF_UART2]);
		break;
	}
	return ret;
}

uint8_t uartRead(uint8_t ch)
{
	uint8_t ret = 0;
	
	switch(ch)
	{
		case _DEF_UART1:
		if (qbufferRead(&qbuffer[_DEF_UART1], &ret, 1) == false)
		{
			ret = -1;
		}
		break;
		case _DEF_UART2:
		if (qbufferRead(&qbuffer[_DEF_UART2], &ret, 1) == false)
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
	uint32_t ret		= 0;
	uint32_t tickstart	= 0U;
	switch(ch)
	{
		case _DEF_UART1:
			for(int i = 0; i < length; i++)
			{
				tickstart = millis();
				while (!(UCSR0A & (1 << UDRE0)))
				{
					if (millis() - tickstart >= 100)
					{
						return ret;
					}
				}

				UDR0 = *(p_data + i);
			}
		break;
		case _DEF_UART2:
			for(int i = 0; i < length; i++)
			{
				tickstart = millis();
				while (!(UCSR1A & (1 << UDRE1)))
				{
					if (millis() - tickstart >= 100)
					{
						return ret;
					}
				}

				UDR1 = *(p_data + i);
			}
		break;
	}
	
	ret = length;
	
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
		case _DEF_UART1:
		baud = baudrate[_DEF_UART1];
		break;
		case _DEF_UART2:
		baud = baudrate[_DEF_UART2];
		break;
	}
	return baud;
}

bool uartFlush(uint8_t ch)
{
	bool ret = true;
	
	switch(ch)
	{
		case _DEF_UART1:
		qbufferFlush(&qbuffer[_DEF_UART1]);
		break;
		case _DEF_UART2:
		qbufferFlush(&qbuffer[_DEF_UART2]);
		break;
	}
	
	return ret;
}


void UART_TxCpltCallback(uint8_t ch)
{
	return;
}

void UART_RxCpltCallback(uint8_t ch)
{
	switch(ch)
	{
		case _DEF_UART1:
		rx_data[_DEF_UART1] = UDR0;
		qbufferWrite(&qbuffer[_DEF_UART1], &rx_data[_DEF_UART1], 1);
		//uartPrintf(_DEF_UART0, "Rx callback available: %d", uartAvailable(_DEF_UART0));
		//uartPrintf(_DEF_UART0, "Rx callback called\n");
		break;
		case _DEF_UART2:
		rx_data[_DEF_UART2] = UDR1;
		qbufferWrite(&qbuffer[_DEF_UART2], &rx_data[_DEF_UART2], 1);
		//uartPrintf(_DEF_UART1, "Rx callback available: %d", uartAvailable(_DEF_UART1));
		//uartPrintf(_DEF_UART0, "Rx callback called\n");
		break;
	}
}



#endif