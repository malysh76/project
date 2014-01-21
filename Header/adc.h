/*
 * adc.h
 *
 *  Created on: 28.02.2011
 *      Author: 129
 */

#ifndef ADC_H_
#define ADC_H_

// каналы adc
#define ADC_U27		2
#define ADC_U310	3
#define ADC_UIN		4
//#define ADC_DEBUG	7

//#define NTURNS_INIT		0//64	// значение инициализации количества оборотов

// ------------ »Ќ»÷»јЋ»«ј÷»я ѕќ–“ј ƒЋя ADC ----------------------------------
#define PORT_ADC	 GPIOD
// маска используемых аналоговых портов
#define MASK_PORT_ADC			(\
								((u32)1<<0)|		/*Uref		*/\
								((u32)1<<1)|		/*Uref		*/\
								((u32)1<<ADC_U27)|	/*канал Ia т*/\
								((u32)1<<ADC_U310)|	/*канал Ib т*/\
								((u32)1<<ADC_UIN)	/*канал Uin */\
								)
// включение тактовой частоты
#define PER_CLOCK_ADC_INIT {PER_CLOCK |= (unsigned long)1<<nPORTD;\
							PER_CLOCK |= (unsigned long)1<<nADC;}


//–ежим работы вывода порта
//00 Ц порт
//01 Ц основна€ функци€
//10 Ц альтернативна€ функци€
//11 Ц переопределенна€ функци€
#define PORT_ADC_FUNC_INIT PORT_ADC->FUNC &= ~(\
											((u32)3<<(0*2))|		/*Uref		*/\
											((u32)3<<(1*2))|		/*Uref		*/\
											((u32)3<<(ADC_U27*2))|	/*канал Ia	*/\
											((u32)3<<(ADC_U310*2))|	/*канал Ib	*/\
											((u32)3<<(ADC_UIN*2))	/*канал Uin */\
											)

// –ежим работы контроллера
//0 Ц аналоговый
//1 Ц цифровой
#define	PORT_ADC_DIGEN_INIT {PORT_ADC->DIGEN &= ~MASK_PORT_ADC;\
							 PORT_ADC->DIGEN |= ((u32)1<<11) | ((u32)1<<12);}

// регистр разрешени€ подд€жки вывода к GND (100K)
//0 Ц подт€жка в питание выключена
//1 Ц подт€жка в питание включена (есть подт€жка ~100 )
#define	PORT_ADC_RGND_INIT PORT_ADC->RGND &= ~MASK_PORT_ADC

// регистр разрешени€ подд€жки вывода к VCC (100K)
//0 Ц подт€жка в питание выключена
//1 Ц подт€жка в питание включена (есть подт€жка ~100 )
#define	PORT_ADC_RVCC_INIT	PORT_ADC->RVCC &= ~MASK_PORT_ADC

// источник опорного напр€жени€
// 0 - внутреннее от AUcc и AGND
// 1 - внешнее от ADC0_REF+ и ADC0_REF-
#ifdef ADC_REF_INT
	#define ADC_REF			((u32)0<<11)
	#warning ADC_REF_INT
#else
	#ifdef ADC_REF_EXT
		#define ADC_REF		((u32)1<<11)
	#warning ADC_REF_EXT
	#endif
#endif

//¬ыбор коэффициента делени€ входной частоты
//0000 Ц  CLK
//0001 Ц  CLK/2
//0010 Ц  CLK/4	20 ћ√÷
//0011 Ц  CLK/8
//Е
//1111 Ц CLK/32768
#define ADC_CLK_20MHZ	((u32)2<<12)
#define ADC_CLK_10MHZ	((u32)3<<12)
#define ADC_CLK_5MHZ	((u32)4<<12)
#define ADC_CLK_2_5MHZ	((u32)5<<12)

//#define ADC_DIV_CLK		((u32)4<<12)

// включение ј÷ѕ
#define ADC_ADON		1			// включение 1 ј÷ѕ

#define ADC_CFG_INIT \
			(ADC_CLK_10MHZ|			/*CLK/4 = 80/4 = 20 ћ√ц*/\
			 ADC_REF|				/*источник опорного напр€жени€*/\
			 ADC_ADON)				/* включение  ј÷ѕ*/

// запуск преобразовани€ ADCx канала chs

//#define ADC_GO_ON(adc,chs) {\
//							/*ADC->CONFIG[adc-1] &= ~0x000001F0;*/\
//							ADC->CONFIG[adc-1] = ADC_CFG_INIT|\
//							((unsigned long)chs<<4)|\
//							((unsigned long)1<<1);\
//							}

#define ADC_GO_ON(adc,chs) {\
							ADC->CONFIG[adc-1] &= ~0x000001F0;\
							ADC->CONFIG[adc-1] |= ((unsigned long)chs<<4);\
							ADC->CONFIG[adc-1] |= ((unsigned long)1<<1);\
							}
// запуск измерени€ напр€жени€ борт сети +27¬ и высокого напр€жени€ +310¬
#define ADC_GO_ON_U27  ADC_GO_ON(1,ADC_U27)
// запуск измерени€ Uin
#define ADC_GO_ON_Uin ADC_GO_ON(1,ADC_UIN)
// задержка выборки
#define ADC_ACCESS_TIME {__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();}

// режимы работы усилител€
typedef enum
		{
			INIT = 0,	// режим инициализации
			WORK = 1,	// рабочий режим
			TEST = 2	// тестовый режим(не работает регул€тор положени€)
		}TMODE;


// экспорт переменных
extern	volatile unsigned long 	Adc[16];						// рабочий буфер ј÷ѕ
extern 	TMODE 					workMode;

// экспорт функций
extern		void 			ADC_Init		(void);

#endif /* ADC_H_ */
