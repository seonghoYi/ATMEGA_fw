#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>


/*----------------------------MACRO FUNCTIONS DEFINITIONS----------------------------*/
#define SET_BIT(PORT, BIT)						(PORT |= (BIT))
#define CLR_BIT(PORT, BIT)						(PORT &= ~(BIT))
#define READ_BIT(PORT, BIT)						(PORT & (BIT))
#define TOGGLE_BIT(PORT, BIT)					(PORT ^= (BIT))
#define CLEAR_REG(REG)							(REG = 0x00)
#define WRITE_REG(REG, VAL)						(REG = VAL)
#define READ_REG(REG)							(REG)
#define MODIFY_REG(REG, CLEARMASK, SETMASK)		WRITE_REG((REG), (READ_REG(REG) & (~(CLEARMASK))) | (SETMASK))
/*----------------------------MACRO FUNCTIONS DEFINITIONS END----------------------------*/

/*----------------------------SYSTEM DEFINITIONS----------------------------*/
#define SYSTICK_MODULE_ENABLED			//systick 활성화
#define GPIO_MODULE_ENABLED
#define UART_MODULE_ENABLED				//uart 활성화
#define TIMER_MODULE_ENABLED


/*----------------------------SYSTEM DEFINITIONS END----------------------------*/

/*----------------------------GLOBAL H/W MACRO DEFINITIONS----------------------------*/
#define	HW_UART_MAX_CH				2
#define UART_BUF_MAX_SIZE			256

#define	HW_TIMER8_MAX_CH			1
#define	HW_TIMER16_MAX_CH			2

#define GPIO_MAX_CH					5

#define GPIOA						0
#define GPIOB						1
#define GPIOC						2
#define GPIOD						3
#define GPIOE						4
#define GPIOF						5
#define GPIOG						6

#define _USE_ROS_MODULE				//ros 사용
#define _USE_BLUETOOTH_MODULE		
#define _USE_SERVO_MODULE		
#define _USE_SUCTION_MOTOR_MODULE
/*----------------------------GLOBAL H/W MACRO DEFINITIONS END----------------------------*/

/*----------------------------GLOBAL MACRO CONSTANT DEFINITIONS----------------------------*/
/*-----------UART-----------*/
#define _DEF_UART1			0
#define _DEF_UART2			1
/*--------------------------*/

/*-----------TIMER-----------*/
#define _DEF_TIM1			0		// timer2
#define _DEF_TIM4			1		// timer0
#define _DEF_TIM2			0		// timer1
#define _DEF_TIM3			1		// timer3
#define _DEF_CH_A			0
#define _DEF_CH_B			1
#define _DEF_CH_C			2
/*---------------------------*/

/*-----------GPIO-----------*/
#define _DEF_INPUT			0
#define _DEF_OUTPUT			1

#define _DEF_RESET			0
#define _DEF_SET			1

#define _DEF_BT_PORT		GPIOA
#define _DEF_LINE_LED_PORT	GPIOA
#define _DEF_RUN_LED_PORT	GPIOB
#define _DEF_MOTOR_EN_PORT	GPIOG
/*--------------------------*/

/*-----------BT-----------*/
#define _DEF_GPIO_BT_RST	0		//active low
#define _DEF_GPIO_BT_CFG	1		//active high
#define _DEF_BT_SLAVE		0
#define _DEF_BT_MASTER		1
/*------------------------*/
/*----------------------------GLOBAL MACRO CONSTANT DEFINITIONS END----------------------------*/

/*----------------------------QUEUE BUFFER----------------------------*/

typedef struct
{
	uint32_t in;
	uint32_t out;
	uint32_t len;

	uint8_t *p_buf;
} qbuffer_t;

bool qbufferCreate(qbuffer_t *p_node, uint8_t *p_buf, uint32_t length)
{
	bool ret = true;

	p_node->in    = 0;
	p_node->out   = 0;
	p_node->len   = length;
	p_node->p_buf = p_buf;

	return ret;
}

bool qbufferWrite(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
	bool ret = 0;
	uint32_t next_in;

	for (int i=0; i<length; i++)
	{
		next_in = (p_node->in + 1) %p_node->len;

		if (next_in != p_node->out)
		{
			if (p_node->p_buf != NULL)
			{
				p_node->p_buf[p_node->in] = p_data[i];
			}
			p_node->in = next_in;
		}
		else
		{
			ret = false;
			break;
		}
	}
	return ret;
}


bool qbufferRead(qbuffer_t *p_node, uint8_t *p_data, uint32_t length)
{
	bool ret = true;

	for (int i=0; i<length; i++)
	{
		if (p_node->p_buf != NULL)
		{
			p_data[i] = p_node->p_buf[p_node->out];
		}

		if (p_node->out != p_node->in)
		{
			p_node->out = (p_node->out + 1) % p_node->len;
		}
		else
		{
			ret = false;
			break;
		}
	}
	return ret;
}

uint32_t qbufferAvailable(qbuffer_t *p_node)
{
	uint32_t ret;

	ret = (p_node->in - p_node->out) %p_node->len;

	return ret;
}


void qbufferFlush(qbuffer_t *p_node)
{
	p_node->in  = 0;
	p_node->out = 0;
}

/*----------------------------QUEUE BUFFER END----------------------------*/

/*----------------------------SYSTICK----------------------------*/
/* 이 블록 외부에서는 millis() 함수를 사용할 것 */
#ifdef SYSTICK_MODULE_ENABLED

typedef enum
{
	TICK_FREQ_250HZ		= 4U,
	TICK_FREQ_500HZ		= 2U,
	TICK_FREQ_1KHZ		= 1U,
	TICK_FREQ_DEFAULT	= TICK_FREQ_1KHZ
} TickFreqTypeDef;

uint32_t getTick(void);

volatile uint32_t tick;
TickFreqTypeDef tickFreq = TICK_FREQ_DEFAULT; // 1ms


void delay(uint32_t ms)
{
	uint32_t tickstart = getTick();
	uint32_t wait = ms;

	if (wait < 0xFFFFFFFF) // max delay
	{
		wait += (uint32_t)tickFreq;
	}
	
	while ((getTick() - tickstart) < wait);
}

uint32_t millis(void)
{
	return getTick();
}

void initTick(void)
{
	uint16_t prescaler;
	uint32_t ocr;
	uint32_t equation; //ctc 방정식에서의 분모 이름 진짜 애매하네

	SET_BIT(TIMSK, 1 << OCIE0);
	SET_BIT(TCCR0, 1 << WGM00); // CTC mode
	
	equation = (tickFreq * F_CPU) / 2000; //ms
	
	if (!(equation % 8))
	{
		prescaler = 8;
	}
	else if (!(equation % 32))
	{
		prescaler = 32;
	}
	else if (!(equation % 64))
	{
		prescaler = 64;
	}
	else if (!(equation % 128))
	{
		prescaler = 128;
	}
	else if (!(equation % 256))
	{
		prescaler = 256;
	}
	else if (!(equation % 1024))
	{
		prescaler = 1024;
	}
	else
	{
		prescaler = 1;
	}
	
	ocr = (equation / prescaler) - 1;
	
	if (ocr < 255)
	{
		tickFreq = TICK_FREQ_DEFAULT;
	}
	
	switch(prescaler)
	{
		case 1:
		CLR_BIT(TCCR0, (1 << CS01) | (1 << CS02));
		SET_BIT(TCCR0, (1 << CS00));
		break;
		case 8:
		CLR_BIT(TCCR0, (1 << CS00) | (1 << CS02));
		SET_BIT(TCCR0, (1 << CS02));
		break;
		case 32:
		SET_BIT(TCCR0, (1 << CS00) | (1 << CS01));
		CLR_BIT(TCCR0, (1 << CS02));
		break;
		case 64:
		CLR_BIT(TCCR0, (1 << CS00) | (1 << CS01));
		SET_BIT(TCCR0, (1 << CS02));
		break;
		case 128:
		SET_BIT(TCCR0, (1 << CS00) | (1 << CS02));
		CLR_BIT(TCCR0, (1 << CS01));
		break;
		case 256:
		SET_BIT(TCCR0, (1 << CS01) | (1 << CS02));
		CLR_BIT(TCCR0, (1 << CS00));
		break;
		case 1024:
		SET_BIT(TCCR0, (1 << CS00) | (1 << CS01) | (1 << CS02));
		break;
	}
	
	OCR0 = (uint8_t)ocr;
	sei();
}

void incTick(void)
{
	tick += tickFreq;
}

uint32_t getTick(void)
{
	return tick;
}




ISR(TIMER0_OVF_vect)
{
	incTick();
}



#endif
/*----------------------------SYSTICK END----------------------------*/

/*----------------------------GPIO----------------------------*/
#ifdef GPIO_MODULE_ENABLED

#define PIN_RESET		0
#define PIN_SET			1

#define INPUT			_DEF_INPUT
#define	OUTPUT			_DEF_OUTPUT

typedef struct
{
	volatile uint8_t *DDRn;
	volatile uint8_t *PORTn;
	volatile uint8_t *PINn;
} GPIO_TypeDef;

static const GPIO_TypeDef GPIO_descripter[] = {
	{&DDRA, &PORTA, &PINA},
	{&DDRB, &PORTB, &PINB},
	{&DDRC, &PORTC, &PINC},
	{&DDRD, &PORTD, &PIND},
	{&DDRE, &PORTE, &PINE},
	{&DDRF, &PORTF, &PINF},
	{&DDRG, &PORTG, &PING},
};

typedef enum
{
	on_state,
	off_state
} GPIO_PinState;

typedef struct
{
	uint8_t port;
	uint8_t pin;
	uint8_t mode;
	GPIO_PinState on_state;
	GPIO_PinState off_state;
} gpio_tbl_t;

gpio_tbl_t gpio_tbl[GPIO_MAX_CH] =
{
	{_DEF_BT_PORT, PIN0, OUTPUT, PIN_SET, PIN_RESET}, // bt_rst
	{_DEF_BT_PORT, PIN1, OUTPUT, PIN_RESET, PIN_SET}, // bt_cfg
	{_DEF_LINE_LED_PORT, PIN2, OUTPUT, PIN_SET, PIN_RESET}, // line_led
	{_DEF_RUN_LED_PORT, PIN3, OUTPUT, PIN_SET, PIN_RESET}, // run_led
	{_DEF_MOTOR_EN_PORT, PIN0, OUTPUT, PIN_SET, PIN_RESET}, // motor_enable
};

bool gpioPinMode(uint8_t ch, uint8_t mode);

bool gpioInit(void)
{
	bool ret = true;
	
	for(int i = 0; i < GPIO_MAX_CH; i++)
	{
		gpioPinMode(i, gpio_tbl[i].mode);
	}
	return ret;
}

bool gpioPinMode(uint8_t ch, uint8_t mode)
{
	bool ret = false;
	if (ch < 0 || ch >= GPIO_MAX_CH) return ret;
	
	switch(mode)
	{
		case _DEF_INPUT:
			CLR_BIT(*(GPIO_descripter[ch].PORTn), gpio_tbl[ch].pin);
		break;
		case _DEF_OUTPUT:
			SET_BIT(*(GPIO_descripter[ch].PORTn), gpio_tbl[ch].pin);
		break;
		default:
		break;
	}
	ret = true;
	
	return ret;
}

void gpioPinWrite(uint8_t ch, bool value)
{
	if (ch < 0 || ch >= GPIO_MAX_CH) return;
	if (value)
	{
		if (gpio_tbl[ch].off_state == PIN_RESET)
		{
			*(GPIO_descripter[ch].DDRn) = false;
		}
		else if (gpio_tbl[ch].off_state == PIN_SET)
		{
			*(GPIO_descripter[ch].DDRn) = true;
		}
	}
	else
	{
		if (gpio_tbl[ch].on_state == PIN_RESET)
		{
			*(GPIO_descripter[ch].DDRn) = true;
		}
		else if (gpio_tbl[ch].on_state == PIN_SET)
		{
			*(GPIO_descripter[ch].DDRn) = false;
		}
	}
}

bool gpioPinRead(uint8_t ch)
{
	if (ch < 0 || ch >= GPIO_MAX_CH) return false;
	return READ_BIT(*(GPIO_descripter[ch].PINn), gpio_tbl[ch].pin);
}

void gpioPinToggle(uint8_t ch)
{
	if (ch < 0 || ch >= GPIO_MAX_CH) return;
	TOGGLE_BIT(*(GPIO_descripter[ch].DDRn), gpio_tbl[ch].pin);
}

#endif
/*----------------------------GPIO END----------------------------*/

/*----------------------------UART----------------------------*/
#ifdef UART_MODULE_ENABLED
#define UART_MAX_CH		HW_UART_MAX_CH

static bool uart_is_open[UART_MAX_CH];

static qbuffer_t qbuffer[UART_MAX_CH];
static uint8_t rx_buf[UART_MAX_CH][UART_BUF_MAX_SIZE];
static uint8_t rx_data[UART_MAX_CH];
static uint32_t baudrate[] = {9600, 9600, };


bool uartInit(void)
{
	for (int i=0; i<UART_MAX_CH; i++)
	{
		uart_is_open[i] = false;
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
		
		uart_is_open[_DEF_UART1] = true;
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
		
		uart_is_open[_DEF_UART2] = true;
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

ISR(USART0_RX_vect)
{
	UART_RxCpltCallback(_DEF_UART1);
}

ISR(USART1_RX_vect)
{
	UART_RxCpltCallback(_DEF_UART2);
}


#endif
/*----------------------------UART END----------------------------*/

/*----------------------------TIMER----------------------------*/
#ifdef TIMER_MODULE_ENABLED

#define TIMER8_MAX_CH		HW_TIMER8_MAX_CH
#define TIMER16_MAX_CH		HW_TIMER16_MAX_CH

#define TIM0	0
#define TIM1	1
#define TIM2	2
#define TIM3	3

#define CHANNEL_A	_DEF_CH_A
#define CHANNEL_B	_DEF_CH_B
#define CHANNEL_C	_DEF_CH_C

typedef struct
{
	bool is_open;
	uint8_t tccr;
	uint8_t tcnt;
	uint8_t ocr;
} tim8_t;

typedef struct
{
	bool is_open;
	uint8_t tccra;
	uint8_t tccrb;
	uint8_t tccrc;
	uint16_t tcnt;
	uint16_t ocr[3];
	uint16_t icr;
} tim16_t;

static tim8_t	tim8_tbl[TIMER8_MAX_CH];
static tim16_t	tim16_tbl[TIMER16_MAX_CH];


bool timerInit(void)
{
	bool ret = true;
	
	for (int i = 0; i < TIMER8_MAX_CH; i++)
	{
		tim8_tbl[i].is_open = false;
		tim8_tbl[i].tccr = 0;
		tim8_tbl[i].ocr = 0;
		tim8_tbl[i].tcnt = 0;
	}
	for (int i = 0; i < TIMER16_MAX_CH; i++)
	{
		tim16_tbl[i].is_open = false;
		tim16_tbl[i].tccra = 0;
		tim16_tbl[i].tccrb = 0;
		tim16_tbl[i].tccrc = 0;
		tim16_tbl[i].tcnt = 0;
		tim16_tbl[i].ocr[0] = 0;
		tim16_tbl[i].ocr[1] = 0;
		tim16_tbl[i].ocr[2] = 0;
		tim16_tbl[i].icr = 0;
	}
	
	return ret;
}

bool timerBegin(uint8_t ch_)
{
	bool ret = true;
	
	switch(ch_)
	{
		case TIM2:
		SET_BIT(tim8_tbl[_DEF_TIM1].tccr, (1 << WGM20));
		SET_BIT(tim8_tbl[_DEF_TIM1].tccr, (1 << CS20)); // Phase correct PWM f=31.372KHz (F_CPU = 16MHz) (in 8MHz case, set timer as same then will be f=15.686KHz)
		
		SET_BIT(tim8_tbl[_DEF_TIM1].tccr, (1 << COM21)); // port oc2 non inverted output
		
		tim8_tbl[_DEF_TIM1].is_open = true;
		break;
		case TIM1:
		SET_BIT(tim16_tbl[_DEF_TIM2].tccra, (1 << WGM11) | (1 << WGM10));
		SET_BIT(tim16_tbl[_DEF_TIM2].tccrb, (1 << WGM12) | (1 << CS11)); // 10-bit Fast PWM f=1.953KHz (F_CPU = 16MHz) (in 8MHz case, set timer as 9-bit Fast PWM prescaler 8 then will be same)
		
		SET_BIT(tim16_tbl[_DEF_TIM2].tccra, (1 << COM1A1) | (1 << COM1B1)); // port A, B non inverted output
		
		tim16_tbl[_DEF_TIM2].is_open = true;
		break;
		case TIM3:
		SET_BIT(tim16_tbl[_DEF_TIM3].tccra, (1 << WGM31));
		SET_BIT(tim16_tbl[_DEF_TIM3].tccrb, (1 << WGM33) | (1 << WGM32) | (1 << CS31) | (1 << CS30));
		tim16_tbl[_DEF_TIM3].icr = 4999; // Fast PWM f=50Hz (in 8MHz case, set ICR3 as 2499)
		
		SET_BIT(tim16_tbl[_DEF_TIM3].tccra, (1 << COM3A1) | (1 << COM3B1)); // port A, B non inverted output
		
		tim16_tbl[_DEF_TIM3].is_open = true;
		break;
		#ifdef SYSTICK_MODULE_ENABLED
		case TIM0:
		
		break;
		#endif
	}
	
	return ret;
}

bool timerIsOpen(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2: return tim8_tbl[_DEF_TIM1].is_open;
		case TIM1: return tim16_tbl[_DEF_TIM2].is_open;
		case TIM3: return tim16_tbl[_DEF_TIM3].is_open;
		case TIM0: return tim8_tbl[_DEF_TIM4].is_open;
		default:   return false;
	}
}


bool timerStart(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2:
		WRITE_REG(TCCR2, tim8_tbl[_DEF_TIM1].tccr);
		WRITE_REG(TCNT2, tim8_tbl[_DEF_TIM1].tcnt);
		WRITE_REG(OCR2, tim8_tbl[_DEF_TIM1].ocr);
		break;
		case TIM1:
		WRITE_REG(TCCR1A, tim16_tbl[_DEF_TIM2].tccra);
		WRITE_REG(TCCR1B, tim16_tbl[_DEF_TIM2].tccrb);
		WRITE_REG(TCCR1C, tim16_tbl[_DEF_TIM2].tccrc);
		WRITE_REG(OCR1A, tim16_tbl[_DEF_TIM2].ocr[CHANNEL_A]);
		WRITE_REG(OCR1B, tim16_tbl[_DEF_TIM2].ocr[CHANNEL_B]);
		WRITE_REG(OCR1C, tim16_tbl[_DEF_TIM2].ocr[CHANNEL_C]);
		WRITE_REG(TCNT1, tim16_tbl[_DEF_TIM2].tcnt);
		WRITE_REG(ICR1, tim16_tbl[_DEF_TIM2].icr);
		break;
		case TIM3:
		WRITE_REG(TCCR3A, tim16_tbl[_DEF_TIM3].tccra);
		WRITE_REG(TCCR3B, tim16_tbl[_DEF_TIM3].tccrb);
		WRITE_REG(TCCR3C, tim16_tbl[_DEF_TIM3].tccrc);
		WRITE_REG(OCR3A, tim16_tbl[_DEF_TIM3].ocr[CHANNEL_A]);
		WRITE_REG(OCR3B, tim16_tbl[_DEF_TIM3].ocr[CHANNEL_B]);
		WRITE_REG(OCR3C, tim16_tbl[_DEF_TIM3].ocr[CHANNEL_C]);
		WRITE_REG(TCNT3, tim16_tbl[_DEF_TIM3].tcnt);
		WRITE_REG(ICR3, tim16_tbl[_DEF_TIM3].icr);
		break;
		case TIM0:
		WRITE_REG(TCCR0, tim8_tbl[_DEF_TIM4].tccr);
		WRITE_REG(TCNT0, tim8_tbl[_DEF_TIM4].tcnt);
		WRITE_REG(OCR0, tim8_tbl[_DEF_TIM4].ocr);
		break;
	}
	return true;
}

bool timerStop(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2:
		CLEAR_REG(TCCR2);
		CLEAR_REG(TCNT2);
		CLEAR_REG(OCR2);
		break;
		case TIM1:
		CLEAR_REG(TCCR1A);
		CLEAR_REG(TCCR1B);
		CLEAR_REG(TCCR1C);
		CLEAR_REG(OCR1A);
		CLEAR_REG(OCR1B);
		CLEAR_REG(OCR1C);
		CLEAR_REG(TCNT1);
		CLEAR_REG(ICR1);
		break;
		case TIM3:
		CLEAR_REG(TCCR3A);
		CLEAR_REG(TCCR3B);
		CLEAR_REG(TCCR3C);
		CLEAR_REG(OCR3A);
		CLEAR_REG(OCR3B);
		CLEAR_REG(OCR3C);
		CLEAR_REG(TCNT3);
		CLEAR_REG(ICR3);
		break;
		case TIM0:
		CLEAR_REG(TCCR0);
		CLEAR_REG(TCNT0);
		CLEAR_REG(OCR0);
		break;
	}
	return true;
}


bool timerSetTcnt(uint8_t ch_, uint16_t tcnt_)
{
	switch(ch_)
	{
		case TIM2:
		tim8_tbl[_DEF_TIM1].tcnt = (uint8_t)tcnt_;
		TCNT2 = tim8_tbl[_DEF_TIM1].tcnt;
		break;
		case TIM1:
		tim16_tbl[_DEF_TIM2].tcnt = tcnt_;
		TCNT1 = tim16_tbl[_DEF_TIM2].tcnt;
		break;
		case TIM3:
		tim16_tbl[_DEF_TIM3].tcnt = tcnt_;
		TCNT3 = tim16_tbl[_DEF_TIM3].tcnt;
		break;
		case TIM0:
		tim8_tbl[_DEF_TIM4].tcnt = (uint8_t)tcnt_;
		TCNT0 = tim8_tbl[_DEF_TIM4].tcnt;
		default:
		return false;
	}
	return true;
}

uint16_t timerGetTcnt(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2: return (uint16_t)TCNT2;
		case TIM1: return TCNT1;
		case TIM3: return TCNT3;
		case TIM0: return (uint16_t)TCNT0;
		default: return 0;
	}
}

bool timer8SetOcr(uint8_t ch_, uint8_t ocr_)
{
	switch(ch_)
	{
		case TIM2:
		tim8_tbl[_DEF_TIM1].ocr = ocr_;
		OCR2 = tim8_tbl[_DEF_TIM1].ocr;
		break;
		case TIM0:
		tim8_tbl[_DEF_TIM4].ocr = ocr_;
		OCR0 = tim8_tbl[_DEF_TIM4].ocr;
		break;
		default:
		return false;
	}
	return true;
}

uint8_t timer8GetOcr(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM2: return tim8_tbl[_DEF_TIM1].ocr;
		case TIM0: return tim8_tbl[_DEF_TIM4].ocr;
		default: return 0;
	}
}

bool timer16SetOcr(uint8_t ch_, uint16_t ocr_, uint8_t channel_)
{
	switch(ch_)
	{
		case TIM1:
		if (channel_ == CHANNEL_A)
		{
			tim16_tbl[_DEF_TIM2].ocr[CHANNEL_A] = ocr_;
			OCR1A = tim16_tbl[_DEF_TIM2].ocr[CHANNEL_A];
		}
		else if (channel_ == CHANNEL_B)
		{
			tim16_tbl[_DEF_TIM2].ocr[CHANNEL_B] = ocr_;
			OCR1B = tim16_tbl[_DEF_TIM2].ocr[CHANNEL_B];
		}
		else if (channel_ == CHANNEL_C)
		{
			tim16_tbl[_DEF_TIM2].ocr[CHANNEL_C] = ocr_;
			OCR1C = tim16_tbl[_DEF_TIM2].ocr[CHANNEL_C];
		}
		break;
		case TIM3:
		if (channel_ == CHANNEL_A)
		{
			tim16_tbl[_DEF_TIM3].ocr[CHANNEL_A] = ocr_;
			OCR3A = tim16_tbl[_DEF_TIM3].ocr[CHANNEL_A];
		}
		else if (channel_ == CHANNEL_B)
		{
			tim16_tbl[_DEF_TIM3].ocr[CHANNEL_B] = ocr_;
			OCR3B = tim16_tbl[_DEF_TIM3].ocr[CHANNEL_B];
		}
		else if (channel_ == CHANNEL_C)
		{
			tim16_tbl[_DEF_TIM3].ocr[CHANNEL_C] = ocr_;
			OCR3C = tim16_tbl[_DEF_TIM3].ocr[CHANNEL_C];
		}
		break;
		default:
		return false;
	}
	return true;
}

uint16_t timer16GetOcr(uint8_t ch_, uint8_t channel_)
{
	switch(ch_)
	{
		case TIM1: return tim16_tbl[_DEF_TIM2].ocr[channel_];
		case TIM3: return tim16_tbl[_DEF_TIM3].ocr[channel_];
		default:   return 0;
	}
}

bool timer16SetIcr(uint8_t ch_, uint16_t icr_)
{
	switch(ch_)
	{
		case TIM1:
		tim16_tbl[_DEF_TIM2].icr = icr_;
		ICR1 = tim16_tbl[_DEF_TIM2].icr;
		break;
		case TIM3:
		tim16_tbl[_DEF_TIM3].icr = icr_;
		ICR3 = tim16_tbl[_DEF_TIM3].icr;
		break;
		default:
		return false;
	}
	
	return true;
}

uint16_t timer16GetIcr(uint8_t ch_)
{
	switch(ch_)
	{
		case TIM1: return tim16_tbl[_DEF_TIM2].icr;
		case TIM3: return tim16_tbl[_DEF_TIM3].icr;
		default:   return 0;
	}
}


#endif
/*----------------------------TIMER END----------------------------*/

/*----------------------------BLUETOOTH----------------------------*/
#ifdef _USE_BLUETOOTH_MODULE
#define BT_BUF_MAX	64

typedef struct
{
	uint8_t name[BT_BUF_MAX];
	uint8_t pswd[BT_BUF_MAX];
	uint8_t	slave_addr[BT_BUF_MAX];
} bt_cfg_t;

typedef struct
{
	uint16_t msg_len;
	uint8_t *msgs;
} bt_msg_t;

typedef struct
{
	bool			is_open;
	uint8_t			ch;
	uint32_t		baud;
	uint32_t		pre_time;
	bt_msg_t		msg;
	bt_cfg_t		bt_cfg;
} bt_t;

bool btOpen(bt_t *p_bt, uint8_t ch_, uint32_t baud_)
{
	bool ret = true;
	p_bt->ch = ch_;
	p_bt->baud = baud_;
	p_bt->msg.msgs = NULL;
	p_bt->is_open = uartOpen(p_bt->ch, baud_);
	return ret;
}

uint32_t btAvailable(bt_t *p_bt)
{
	uint32_t ret = uartAvailable(p_bt->ch);
	return ret;
}

uint8_t btRead(bt_t *p_bt)
{
	uint8_t ret = uartRead(p_bt->ch);
	return ret;
}

uint32_t btWrite(bt_t *p_bt)
{
	uint32_t ret;
	ret = uartWrite(p_bt->ch, p_bt->msg.msgs, p_bt->msg.msg_len);
	return ret;
}

bool btBegin(bt_t *p_bt, uint8_t mode)
{
	char atCommand[BT_BUF_MAX];
	char response[BT_BUF_MAX];
	int index = 0;
	uint32_t tickstart;
	
	gpioPinWrite(_DEF_GPIO_BT_CFG, _DEF_SET);
	gpioPinWrite(_DEF_GPIO_BT_RST, _DEF_SET);
	delay(10);
	gpioPinWrite(_DEF_GPIO_BT_RST, _DEF_RESET);
	
	p_bt->msg.msgs = (uint8_t*)&atCommand[0];
	switch(mode)
	{
		case _DEF_BT_MASTER:
			sprintf(&atCommand[0], "AT+NAME=%s\r\n", (char*)&(p_bt->bt_cfg.name[0]));
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			sprintf(&atCommand[0], "AT+PSWD=%s\r\n", (char*)&(p_bt->bt_cfg.pswd[0]));
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			sprintf(&atCommand[0], "AT+UART=%d,0,0\r\n", (int)p_bt->baud);
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			sprintf(&atCommand[0], "AT+ROLE=1\r\n");
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			sprintf(&atCommand[0], "AT+INIT\r\n");
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			sprintf(&atCommand[0], "AT+PAIR=%s,20\r\n", (char*)&(p_bt->bt_cfg.slave_addr[0]));
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			tickstart = millis();
			while(!(btAvailable(p_bt) > 0))
			{
				if (millis() - tickstart > 100)
				{
					return false;
				}
			}
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			
		break;
		case _DEF_BT_SLAVE:
			sprintf(&atCommand[0], "AT+NAME=%s\r\n", (char*)&(p_bt->bt_cfg.name[0]));
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			sprintf(&atCommand[0], "AT+PSWD=%s\r\n", (char*)&(p_bt->bt_cfg.pswd[0]));
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			sprintf(&atCommand[0], "AT+UART=%d,0,0\r\n", (int)p_bt->baud);
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			sprintf(&atCommand[0], "AT+ROLE=0\r\n");
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
			
			sprintf(&atCommand[0], "AT+INIT\r\n");
			p_bt->msg.msg_len = strlen(&atCommand[0]);
			btWrite(p_bt);
			delay(500);
			
			while(btAvailable(p_bt) > 0)
			{
				response[index++] = btRead(p_bt);
			}
			if (!strncmp(&response[0], "OK\r\n", 4)) return false;
			index = 0;
		break;
	}
	
	gpioPinWrite(_DEF_GPIO_BT_CFG, _DEF_RESET);
	gpioPinWrite(_DEF_GPIO_BT_RST, _DEF_SET);
	delay(10);
	gpioPinWrite(_DEF_GPIO_BT_RST, _DEF_RESET);
	
	return true;
}

#endif
/*----------------------------BLUETOOTH END----------------------------*/

/*----------------------------ROS----------------------------*/
#ifdef _USE_ROS_MODULE
#define ROS_MAX_SERVICE		128
#define ROS_PACKET_BUF_MAX	512

enum
{
	ROS_INST_PING	= 0x01,
	ROS_INST_RQSVC	= 0x02,
	ROS_INST_WAITRQ	= 0x03
};

enum
{
	ROS_ID_PUBLISHER			= 0x00,
	ROS_ID_SUBSCRIBER			= 0x01,
	ROS_ID_SERVICE_SERVER		= 0x02,
	ROS_ID_SERVICE_CLIENT		= 0x04,
	ROS_ID_PARAMETER_REQUEST	= 0x06,
	ROS_ID_LOG					= 0x07,
	ROS_ID_TIME					= 0x0A,
	ROS_ID_TX_STOP				= 0x0B
};



typedef struct
{
	uint8_t		sync;
	uint8_t		protocol_version;
	uint16_t	msg_len;
	uint8_t		msg_len_checksum;
	uint8_t		id;
	uint8_t		inst;
	uint8_t		*msgs;
	uint8_t		checksum;
} ros_packet_t;


typedef struct
{
	bool			is_open;
	uint8_t			ch;
	uint32_t		baud;
	uint8_t			state;
	uint8_t			index;
	uint32_t		pre_time;
	
	ros_packet_t	packet;
	uint8_t			packet_buf[ROS_PACKET_BUF_MAX];
	
	void (*func[ROS_MAX_SERVICE])(uint8_t *params);
	uint8_t			service_index;
} ros_t;


#define MID	0x01

enum
{
	ROS_PKT_SYNC1 = 0,
	ROS_PKT_SYNC2,
	ROS_PKT_LEN1,
	ROS_PKT_LEN2,
	ROS_PKT_LENCHECK,
	ROS_PKT_ID1,
	ROS_PKT_ID2
};

enum
{
	ROS_STATE_SYNC1 = 0,
	ROS_STATE_SYNC2,
	ROS_STATE_LEN1,
	ROS_STATE_LEN2,
	ROS_STATE_LENCHECK,
	ROS_STATE_ID1,
	ROS_STATE_ID2,
	ROS_STATE_MSGS,
	ROS_STATE_CS
};

bool rosOpen(ros_t *p_ros, uint8_t ch_, uint32_t baud_)
{
	bool ret = true;
	p_ros->ch = ch_;
	p_ros->baud = baud_;
	p_ros->index = 0;
	p_ros->state = ROS_STATE_SYNC1;
	p_ros->packet.msgs = NULL;
	p_ros->is_open = uartOpen(p_ros->ch, 38400);
	
	for (int i = 0; i < ROS_MAX_SERVICE; i++)
	{
		p_ros->func[i] = NULL;
	}
	p_ros->service_index = 0;
	
	ret = p_ros->is_open;
	return ret;
}

bool rosIsOpen(ros_t *p_ros)
{
	return p_ros->is_open;
}

bool rosClose(ros_t *p_ros)
{
	bool ret = true;
	
	return ret;
}

bool rosSendInst(ros_t *p_ros, uint8_t id_, uint8_t inst_, uint8_t *p_msg_, uint8_t msg_len_)
{
	bool ret = true;
	//uint8_t packet_len;
	uint8_t index;
	uint32_t checksum = 0;
	
	if (p_ros->is_open != true)
	{
		return false;
	}
	
	//packet_len = msg_len_ + 7;
	
	p_ros->packet_buf[ROS_PKT_SYNC1] = 0xFF;
	p_ros->packet_buf[ROS_PKT_SYNC2] = 0xFF;
	p_ros->packet_buf[ROS_PKT_LEN1]	= (msg_len_ >> 0) & 0xFF;
	p_ros->packet_buf[ROS_PKT_LEN2]	= (msg_len_ >> 8) & 0xFF;
	p_ros->packet_buf[ROS_PKT_LENCHECK] = 255 - (p_ros->packet_buf[ROS_PKT_LEN1] + p_ros->packet_buf[ROS_PKT_LEN2]) % 256;
	p_ros->packet_buf[ROS_PKT_ID1] = inst_;
	p_ros->packet_buf[ROS_PKT_ID2] = id_;
	
	index = 7;
	
	p_ros->packet.msgs = &p_ros->packet_buf[7];
	
	for (int i = 0; i < msg_len_; i++)
	{
		p_ros->packet_buf[index++] = p_msg_[i];
		checksum += p_msg_[i];
	}
	
	p_ros->packet_buf[index++] = 255 - (uint8_t)(checksum % 256);
	
	uartWrite(p_ros->ch, p_ros->packet_buf, index);
	
	return ret;
}


bool rosReceivePacket(ros_t *p_ros)
{
	bool ret = false;
	uint8_t rx_data;
	uint8_t index;
	uint32_t buf;
	
	if (p_ros->is_open != true)
	{
		return false;
	}
	
	if (uartAvailable(p_ros->ch) > 0)
	{
		//rx_data = p_ros->driver.available(p_ros->ch);
		//p_ros->driver.write(p_ros->ch, &rx_data, 1);
		rx_data = uartRead(p_ros->ch);
		//p_ros->driver.write(p_ros->ch, &rx_data, 1);
	}
	else
	{
		return false;
	}
	
	if (millis() - p_ros->pre_time >= 100)
	{
		p_ros->state = ROS_STATE_SYNC1;
	}
	p_ros->pre_time = millis();

	switch(p_ros->state)
	{
		case ROS_STATE_SYNC1:
			if (rx_data == 0xFF)
			{
				p_ros->packet_buf[ROS_PKT_SYNC1] = rx_data;
				p_ros->state = ROS_STATE_SYNC2;	
			}
			else
			{
				p_ros->state = ROS_STATE_SYNC1;	
			}
			break;
		case ROS_STATE_SYNC2:
			if (rx_data == 0xFF)
			{
				p_ros->packet_buf[ROS_PKT_SYNC2] = rx_data;
				p_ros->state = ROS_STATE_LEN1;
			}
			else
			{
				p_ros->state = ROS_STATE_SYNC1;
			}
			break;
		case ROS_STATE_LEN1:
			p_ros->packet_buf[ROS_PKT_LEN1] = rx_data;
			p_ros->state = ROS_STATE_LEN2;
			break;
		case ROS_STATE_LEN2:
			p_ros->packet_buf[ROS_PKT_LEN2] = rx_data;
			p_ros->state = ROS_STATE_LENCHECK;
			break;
		case ROS_STATE_LENCHECK:
			p_ros->packet_buf[ROS_STATE_LENCHECK] = rx_data;
			
			buf = p_ros->packet_buf[ROS_PKT_LEN1] + p_ros->packet_buf[ROS_PKT_LEN2];
			p_ros->packet.msg_len_checksum = 255 - (uint8_t)(buf % 256);
			
			if (p_ros->packet.msg_len_checksum == p_ros->packet_buf[ROS_PKT_LENCHECK])
			{
				p_ros->state = ROS_STATE_ID1;
			}
			else
			{
				p_ros->state = ROS_STATE_SYNC1;
			}
			p_ros->packet.msg_len =	(p_ros->packet_buf[ROS_PKT_LEN1] >> 0) & 0xFF;
			p_ros->packet.msg_len |= (p_ros->packet_buf[ROS_PKT_LEN2] >> 8) & 0xFF;
			
			break;
		case ROS_STATE_ID1:
			p_ros->packet_buf[ROS_STATE_ID1] = rx_data;
			p_ros->state = ROS_STATE_ID2;
			break;
		case ROS_STATE_ID2:
			p_ros->packet_buf[ROS_STATE_ID2] = rx_data;
			p_ros->index = 7;
			p_ros->packet.msgs = &p_ros->packet_buf[7];
			if (p_ros->packet.msg_len > 0)
			{
				p_ros->state = ROS_STATE_MSGS;
			}
			else
			{
				p_ros->state = ROS_STATE_SYNC1;
				ret = true;
			}
			break;
		case ROS_STATE_MSGS:
			index = p_ros->index;
			p_ros->index++;
			p_ros->packet_buf[index] = rx_data;
			
			if (p_ros->index >= p_ros->packet.msg_len + 7)
			{
				p_ros->state = ROS_STATE_CS;
				/*
				p_ros->state = ROS_STATE_SYNC1;
				p_ros->index = 0;
				ret = true;
				*/
				//p_ros->driver.write(p_ros->ch, p_ros->packet_buf, p_ros->packet.msg_len + 7);
			}
			break;
		case ROS_STATE_CS:
			index = p_ros->index;
			p_ros->packet_buf[index] = rx_data;
			buf = 0;
			for (int i = 0; i < p_ros->packet.msg_len; i++)
			{
				buf += p_ros->packet_buf[7 + i];
			}
			p_ros->packet.checksum = 255 - (uint8_t)(buf % 256);
			//p_ros->driver.write(p_ros->ch, (uint8_t *)&(p_ros->packet.checksum), 1);
			if (p_ros->packet_buf[index] == p_ros->packet.checksum)
			{
				ret = true;
				p_ros->state = ROS_STATE_SYNC1;
			}
			
			p_ros->index = 0;
		break;
		default:
			break;
	}
	if (ret == true)
	{
		p_ros->packet.id = p_ros->packet_buf[ROS_PKT_ID2];
		p_ros->packet.inst = p_ros->packet_buf[ROS_PKT_ID1];
	}
	
	return ret;
}


void rosAddService(ros_t *p_ros, void (*func)(uint8_t *params))
{
	p_ros->func[p_ros->service_index] = func;
	p_ros->service_index++;
}

void rosCallService(ros_t *p_ros, uint8_t service_ch_, uint8_t *params_)
{
	if (service_ch_ < 0 || service_ch_ >= ROS_MAX_SERVICE)
	{
		return;
	}
	
	p_ros->func[service_ch_](params_);
}

#endif
/*----------------------------ROS END----------------------------*/

/*----------------------------SERVO----------------------------*/
#ifdef _USE_SERVO_MODULE

#define _DEF_SERVO_L	_DEF_CH_A
#define _DEF_SERVO_R	_DEF_CH_B

static const uint8_t servo_ch_tbl[] = {_DEF_TIM3, };
static bool servo_is_init = false;


bool servoInit(void)
{
	servo_is_init = true;
	timerBegin(servo_ch_tbl[0]);
	return true;
}

bool servoIsInit(void)
{
	return servo_is_init;
}

void servoRun(uint16_t L_ocr, uint16_t R_ocr) // 250 ~ 500
{
	if ((L_ocr < 250) || (L_ocr > 500) || (R_ocr < 250) || (R_ocr > 500))
	{
		return;
	}
	
	timer16SetOcr(servo_ch_tbl[0], L_ocr, _DEF_SERVO_L);
	timer16SetOcr(servo_ch_tbl[0], R_ocr, _DEF_SERVO_R);
	timerStart(servo_ch_tbl[0]);
}

void servoStop(void)
{
	timerStop(servo_ch_tbl[0]);
}

#endif
/*----------------------------SERVO END----------------------------*/


/*----------------------------SUCTION MOTOR----------------------------*/
#ifdef _USE_SUCTION_MOTOR_MODULE

static uint8_t suction_ch_tbl[] = {_DEF_TIM1, };
static bool suction_is_init = false;

bool suctionMotorInit(void)
{
	timerBegin(suction_ch_tbl[0]);
	suction_is_init = true;
	return true;
}

bool suctionMotorIsInit(void)
{
	return suction_is_init;
}

void suctionMotorRun(void)
{
	timer8SetOcr(suction_ch_tbl[0], 254);
	timerStart(suction_ch_tbl[0]);
}

void suctionMotorStop(void)
{
	timerStop(suction_ch_tbl[0]);
}

#endif
/*----------------------------SUCTION MOTOR END----------------------------*/

void mcuInit(void)
{
	gpioInit();
	uartInit();
	timerInit();
	initTick();
	sei();
}

void hwInit(void)
{
	servoInit();
	suctionMotorInit();
}

int main(void)
{
	mcuInit();
	hwInit();
	
	bt_t bt;
	ros_t ros;
	
	btOpen(&bt, _DEF_UART1, 38400);
	rosOpen(&ros, _DEF_UART2, 38400);
	
	
	while(1)
	{
		
	}
}