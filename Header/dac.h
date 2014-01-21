/*
 * dac.h
 *
 *  Created on: 11.04.2011
 *      Author: 129
 */

#ifndef DAC_H_
#define DAC_H_
//зависимости
#include "1986ve9x_dac.h"
// ------------ »Ќ»÷»јЋ»«ј÷»я ѕќ–“ј ƒЋя DAC ----------------------------------
#define PORT_DAC	 GPIOE
// маска используемых аналоговых портов
#define MASK_PORT_DAC	(((unsigned long)1<<9)|((unsigned long)1<<0))
// включение тактовой частоты
#define PER_CLOCK_DAC_INIT {PER_CLOCK |= (unsigned long)1<<nPORTE;\
							PER_CLOCK |= (unsigned long)1<<nDAC;}

//–ежим работы вывода порта
//00 Ц порт
//01 Ц основна€ функци€
//10 Ц альтернативна€ функци€
//11 Ц переопределенна€ функци€
#define PORT_DAC_FUNC_INIT PORT_DAC->FUNC &= ~(((unsigned long)3<<(9*2))|((unsigned long)3<<(0*2)))

// –ежим работы контроллера
//0 Ц аналоговый
//1 Ц цифровой
#define	PORT_DAC_DIGEN_INIT PORT_DAC->DIGEN &= ~MASK_PORT_DAC

// регистр разрешени€ подд€жки вывода к GND (100K)
//0 Ц подт€жка в питание выключена
//1 Ц подт€жка в питание включена (есть подт€жка ~100 )
#define	PORT_DAC_RGND_INIT PORT_DAC->RGND &= ~MASK_PORT_DAC

// регистр разрешени€ подд€жки вывода к VCC (100K)
//0 Ц подт€жка в питание выключена
//1 Ц подт€жка в питание включена (есть подт€жка ~100 )
#define	PORT_DAC_RVCC_INIT	PORT_DAC->RVCC &= ~MASK_PORT_DAC


//4          Cfg_SYNC_A          —инхронизаци€ DAC0 и DAC1
//                                0 Ц асинхронные
//                                1 Ц синхронные
#define 		Cfg_SYNC_A		  0

// 3             Cfg_ON_DAC1      ¬ключение DAC1
//                                1 Ц включен
//                                0 Ц выключен
#define 		Cfg_ON_DAC1		  1

// 2             Cfg_ON_DAC0      ¬ключение DAC0
//                                1 Ц включен
//                                0 Ц выключен
#define 		Cfg_ON_DAC0		  1

// 1               Cfg_M_REF1     ¬ыбор источника опорного напр€жени€ DAC1
//                                0 Ц в качестве опорного напр€жени€ используетс€ напр€жение
//                                питани€ с вывода AUcc.
//                                1 Ц в качестве опорного напр€жени€ используетс€ напр€жение
//                                на входе опорного напр€жени€ DAC1_REF.
#define 		Cfg_M_REF1		  0

// 0               Cfg_M_REF0     ¬ыбор источника опорного напр€жени€ DAC0
//                                0 Ц в качестве опорного напр€жени€ используетс€ напр€жение
//                                питани€ с вывода AUcc.
//                                1 Ц в качестве опорного напр€жени€ используетс€ напр€жение
//                                на входе опорного напр€жени€ DAC0_REF.
#define			Cfg_M_REF0		  0

#define			DAC_CFG_INIT		(unsigned long)Cfg_SYNC_A <<4|\
									(unsigned long)Cfg_ON_DAC1<<3|\
									(unsigned long)Cfg_ON_DAC0<<2|\
									(unsigned long)Cfg_M_REF1 <<1|\
									(unsigned long)Cfg_M_REF0 <<0
// запись в dac
#define			DAC0_WR(dac)		DAC->DAC0 = ((unsigned long)dac)
#define			DAC1_WR(dac)		DAC->DAC1 = ((unsigned long)dac)
// чтение dac
#define			DAC0_RD				(DAC->DAC0 & 0x0FFF)
#define			DAC1_RD				(DAC->DAC1 & 0x0FFF)

// экспорт функций
extern void DAC_Init (void);

#endif /* DAC_H_ */
