/*
 * adc.c
 *
 *  Created on: 28.02.2011
 *      Author: 129
 */
#include "common1.h"
#include "1986ve9x_irq.h"
#include "1986ve9x_clk.h"
#include "1986ve9x_adc.h"
#include "1986ve9x_gpio.h"
#include "1986ve9x_tim.h"
#include "defports.h"
#include "sin.h"
#include "adc.h"
#include "stdlib.h"
// зависимости
#include "prio.h"
#include "encoder.h"
#include "exchange.h"
#include "position.h"

#include "uart.h"
#include "dac.h"
#include "spi.h"

TMODE	workMode = WORK;		 	// режим работы
volatile long last_ang;		 //последний считанный угол


volatile unsigned long 	Adc[16];						// рабочий буфер АЦП


void ADC_Init(void)
{
// конфигурация портов
	PER_CLOCK_ADC_INIT;
	PORT_ADC_FUNC_INIT;
	PORT_ADC_DIGEN_INIT;
	PORT_ADC_RGND_INIT;
	PORT_ADC_RVCC_INIT;
	PORT_ADC->DDR &= ~MASK_PORT_ADC;
//	PORT_ADC->DDR |= MASK_PORT_ADC;

// конфигурация ADC->CONFIG[0]
	ADC->CONFIG[0] = ADC_CFG_INIT;

// конфигурация ADC->CONFIG[1]
	ADC->CONFIG[1] = ADC_CFG_INIT;


// конфигурация ADC->STATUS[0]
	ADC->STATUS[0] = 0;
	ADC->STATUS[0] |= (u32)1<<4; 	// разрешение прерываний по окончанию преобразования

// конфигурация ADC->STATUS[1]
	ADC->STATUS[1] = 0;
	ADC->STATUS[1] |= (u32)1<<4; 	// разрешение прерываний по окончанию преобразования

	NVIC_EnableIRQ(ADC_IRQn);		// Разрешение прерывания по ФЦП
	NVIC_SetPriority(ADC_IRQn,PRIO_ADC);
}
/*------------------------------------------------------------------------------
 * ОБРАБОТЧИК ПРЕРЫВАНИЯ АЦП
 *----------------------------------------------------------------------------*/
void __irq ADC_IRQHandler(void)
{
//#define DEBUG_ADC_IRQHandler	// отладка без энкодера

	unsigned char nADC;		// номер ADC
	unsigned long res;		// результат измерения
	unsigned char ch;		// номер канала
//	static  long nTurns = NTURNS_INIT;// количество оборотов
//	long delta_ang;			// разница между текущим и последним считанными углами
	static unsigned short ADC_State = 0; // состояние работы ADC



// определение номера ADC
	nADC = ((ADC->STATUS[0]&0x04)?0:1);
// считывание результата
	res = ADC->RESULT[nADC];
// определение номера канала
	ch = (res>>16)&0x0F;
// сохранение результата в буфер
	Adc[ch] = res&0x0FFF;



// установка признака считанного результата
	ADC_State |= (u16)1<<ch;

	switch(ADC_State)
	{
	// состояние чтения напряжения +27В
	case (u16)1<<ADC_U27:

		CtrlPwm[CPWM_U27] = Adc[ADC_U27];
		ADC_State = 0;
		break;

	// состояние чтения напряжения +310В
	case (u16)1<<ADC_U310:
		// НЕ РАБОТАЕТ!
		CtrlPwm[CPWM_U310] = Adc[ADC_U310];
		ADC_State = 0;
		#ifdef CONTROL_ANALOG
	   	//Аналоговое управление
	    ADC_GO_ON_Uin; // запускаем преобразование входного напряжения		  
	    #endif
		break;

// состояние чтения входного напряжения
	case (u16)1<<ADC_UIN:
//		CtrlPwm[CPWM_IQIN_] = Adc[ADC_UIN];
//		CtrlPwm[CPWM_DEBUG1] = ch;
		ADC_State = 0;
//		DAC1_WR(4000);
		break;

	default:
		break;
	}
}



