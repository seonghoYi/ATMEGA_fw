#include "uart.h"
#include "qbuffer.h"

#ifdef _USE_HW_UART

#define UART_BUF_MAX_SIZE	256

static bool is_open[UART_MAX_CH];

static qbuffer_t qbuffer[UART_MAX_CH];
static uint8_t rx_buf[UART_MAX_CH][UART_BUF_MAX_SIZE];
static uint8_t rx_data[UART_MAX_CH];


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
	
	switch(ch)
	{
		case _DEF_UART1:
		ubrr_f = (float)F_CPU / (float)(16 * baud) - 0.5;	//-1.0 + 0.5 => 반올림, datasheet 173.p
		UBRR0L = (uint16_t)ubrr_f & 0xFF;
		UBRR0H = (uint16_t)ubrr_f >> 8;
		UCSR0A &= ~((1<<U2X0) | (1<<MPCM0));					//Off Double speed && Multi-processor comm mode
		UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);		//Enable rx interrupt && rx&tx
		
		UCSR0B &= ~(1<<UCSZ02);
		UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);					//Character size 8-bit mode
		
		UCSR0C &= ~(1<<UMSEL0);									//Asynchronous mode
		UCSR0C &= ~((1<<UPM00) | (1<<UPM01));					//Non-parity mode
		UCSR0C &= ~(1<<USBS0);									//Stopbit 1-bit
		
		qbufferCreate(&qbuffer[_DEF_UART1], (uint8_t *)&rx_buf[_DEF_UART1][0], UART_BUF_MAX_SIZE);
		
		ret = true;
		break;
		case _DEF_UART2:
		ubrr_f = (float)F_CPU / (float)(16 * baud) - 0.5;	//-1.0 + 0.5 => 반올림, datasheet 173.p
		UBRR1L = (uint16_t)ubrr_f & 0xFF;
		UBRR1H = (uint16_t)ubrr_f >> 8;
		UCSR1A &= ~((1<<U2X1) | (1<<MPCM1));				//Off Double speed && Multi-processor comm mode
		UCSR1B |= (1<<RXCIE1) | (1<<RXEN1) | (1<<TXEN1);	//Enable rx interrupt && rx&tx

		UCSR1B &= ~(1<<UCSZ12);
		UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11);				//Character size 8-bit mode

		UCSR1C &= ~(1<<UMSEL1);								//Asynchronous mode
		UCSR1C &= ~((1<<UPM10) | (1<<UPM11));				//Non-parity mode
		UCSR1C &= ~(1<<USBS1);								//Stopbit 1-bit

		qbufferCreate(&qbuffer[_DEF_UART2], (uint8_t *)&rx_buf[_DEF_UART2][0], UART_BUF_MAX_SIZE);
		
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
	return ret;
}

uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length)
{
	uint32_t ret = 0;

	switch(ch)
	{
		case _DEF_UART1:
		for(int i = 0; i < length; i++)
		{
			while(!(UCSR0A & (1<<UDRE0)));
			UDR0 = *(p_data + i);
		}
		
		ret = length;
		break;
		case _DEF_UART2:
		for(int i = 0; i < length; i++)
		{
			while(!(UCSR1A & (1<<UDRE1)));
			UDR1 = *(p_data + i);
		}
		
		ret = length;
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
		case _DEF_UART1:
		baud = UBRR0L | UBRR0H << 8;
		break;
		case _DEF_UART2:
		baud = UBRR1L | UBRR1H << 8;
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


ISR(USART0_RX_vect)
{
	rx_data[_DEF_UART1] = UDR0;
	qbufferWrite(&qbuffer[_DEF_UART1], &rx_data[_DEF_UART1], 1);
}

ISR(USART1_RX_vect)
{
	rx_data[_DEF_UART2] = UDR1;
	qbufferWrite(&qbuffer[_DEF_UART2], &rx_data[_DEF_UART2], 1);
}



#endif