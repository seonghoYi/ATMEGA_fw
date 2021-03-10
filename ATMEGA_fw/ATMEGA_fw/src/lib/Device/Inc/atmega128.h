#ifndef ATMEGA128_H_
#define ATMEGA128_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stddef.h>

#define SETB(PORT, BIT)		(PORT |= (1 << BIT))
#define CLRB(PORT, BIT)		(PORT &= ~(1 << BIT))
#define READB(PORT, BIT)	(PORT & (1 << BIT))
#define TGLB(PORT, BIT)		(PORT ^= (1 << BIT)) // toggle



typedef struct
{
	volatile uint8_t *sfior;
	volatile uint8_t *eicra;
	volatile uint8_t *eicrb;
	volatile uint8_t *eimsk;
	volatile uint8_t *eifr;
	volatile uint8_t *timsk;
	volatile uint8_t *etimsk;
	volatile uint8_t *tifr;
	volatile uint8_t *etifr;	
} SYS_TypeDef;

#define GPIOA 0
#define GPIOB 1
#define GPIOC 2
#define GPIOD 3
#define GPIOE 4
#define GPIOF 5
#define GPIOG 6

typedef struct
{
	volatile uint8_t *DDRn;
	volatile uint8_t *PORTn;
	volatile uint8_t *PINn;
} GPIO_TypeDef;



#define USART0	0
#define USART1	1

typedef struct
{
	volatile uint8_t *UDRn;
	volatile uint8_t *UCSRnA;
	volatile uint8_t *UCSRnB;
	volatile uint8_t *UCSRnC;
	volatile uint8_t *UBRRnH;
	volatile uint8_t *UBRRnL;
} USART_TypeDef;


#define TIM0	0
#define TIM1	1
#define TIM2	2
#define TIM3	3

typedef struct  
{
	volatile uint8_t *TCCRn;
	volatile uint8_t *TCNTn;
	volatile uint8_t *OCRn;
	volatile uint8_t *assr;
} TIM8_TypeDef;

typedef struct
{
	volatile uint8_t *TCCRnA;
	volatile uint8_t *TCCRnB;
	volatile uint8_t *TCCRnC;
	volatile uint8_t *TCNTnL;
	volatile uint8_t *TCNTnH;
	volatile uint8_t *OCRnAL;
	volatile uint8_t *OCRnAH;
	volatile uint8_t *OCRnBL;
	volatile uint8_t *OCRnBH;
	volatile uint8_t *ICRnL;
	volatile uint8_t *ICRnH;
} TIM16_TypeDef;


#endif /* ATMEGA128_H_ */