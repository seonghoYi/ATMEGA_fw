#ifndef ATMEGA128_H_
#define ATMEGA128_H_


#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stddef.h>

#define SET_BIT(PORT, BIT)						(PORT |= (BIT))
#define CLR_BIT(PORT, BIT)						(PORT &= ~(BIT))
#define READ_BIT(PORT, BIT)						(PORT & (BIT))
#define CLEAR_REG(REG)							(REG = 0x00)
#define WRITE_REG(REG, VAL)						(REG = VAL)
#define READ_REG(REG)							(REG)
#define MODIFY_REG(REG, CLEARMASK, SETMASK)		WRITE_REG((REG), (READ_REG(REG) & (~(CLEARMASK))) | (SETMASK))



#endif /* ATMEGA128_H_ */