/*
 * eeprom.h
 *
 *  Created on: 05.05.2011
 *      Author: 129
 */

#ifndef EEPROM_H_
#define EEPROM_H_
#include "common1.h"
#include "1986ve9x_systick.h"
#include "1986ve9x_irq.h"


#define SIZE_OF_PAGE		0x1000
// распределение памяти
#define EEPROM_ADR_INF	0x08000000
#define EEPROM_ADR_BAS	0x08000000

#define EEPROM_CMD	(*(volatile unsigned long *) 0x40018000)
#define EEPROM_ADR	(*(volatile unsigned long *) 0x40018004)
#define EEPROM_DI	(*(volatile unsigned long *) 0x40018008)
#define EEPROM_DO	(*(volatile unsigned long *) 0x4001800C)
#define EEPROM_KEY	(*(volatile unsigned long *) 0x40018010)

#define VAL_EEPROM_KEY 0x8AAA5551

#define NVSTR	(unsigned long)0x2000
#define PROG	(unsigned long)0x1000
#define MAS1    (unsigned long)0x800
#define ERASE	(unsigned long)0x400
#define IFREN	(unsigned long)0x200
#define SE		(unsigned long)0x100
#define YE		(unsigned long)0x80
#define XE   	(unsigned long)0x40
#define RD   	(unsigned long)(0x04)
#define WR   	(unsigned long)(0x02)
#define CON	 	(unsigned long)0x1

// переход в режим программирования
#define EEPROM_PRG()	{EEPROM_irq(DISABLE); EEPROM_KEY = VAL_EEPROM_KEY;EEPROM_CMD &= 0x38; EEPROM_CMD |= CON;__nop();__nop();}
// переход в рабочий режим
#define EEPROM_WRK()	{EEPROM_CMD &= 0x38;EEPROM_KEY = ~VAL_EEPROM_KEY; EEPROM_irq(ENABLE);}

// тип памяти
typedef enum
		{
			MEM_BAS = 0,						// основная память
			MEM_INF = ((unsigned long)1<<9),	// информационная память
		}TTypeMem;

//typedef enum
//		{
//			ENABLE,
//			DISABLE
//		}TIRQ_State;

// диапазон от 1 us до 16000 us
#define EEPROM_Sleep(us) \
								{\
									SysTick->LOAD = (((us*2000)/25)-16);\
									SysTick->VAL = 0;\
									SysTick->CTRL = 0x05;\
									while(!(SysTick->CTRL&(1<<16)));\
									SysTick->CTRL = 0x04;\
								}

// экспорт функций
//extern void 			EEPROM_irq			(TIRQ_State irq_state);
extern void 			EEPROM_irq			(FunctionalState irq_state);
extern void 			EEPROM_Init			(void);
extern void 			EEPROM_Erase		(TTypeMem TypeMem);
extern void 			EEPROM_ErasePage	(TTypeMem TypeMem, unsigned long Addr);
extern void 			EEPROM_WriteData	(TTypeMem TypeMem, unsigned long Addr, unsigned char* Data, unsigned short len);
extern void 			EEPROM_ReadData		(TTypeMem TypeMem, unsigned long Addr, unsigned char* Data, unsigned short len);

#endif /* EEPROM_H_ */
