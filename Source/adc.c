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
// �����������
#include "prio.h"
#include "encoder.h"
#include "exchange.h"
#include "position.h"

#include "uart.h"
#include "dac.h"
#include "spi.h"

TMODE	workMode = WORK;		 	// ����� ������
volatile long last_ang;		 //��������� ��������� ����


volatile unsigned long 	Adc[16];						// ������� ����� ���


void ADC_Init(void)
{
// ������������ ������
	PER_CLOCK_ADC_INIT;
	PORT_ADC_FUNC_INIT;
	PORT_ADC_DIGEN_INIT;
	PORT_ADC_RGND_INIT;
	PORT_ADC_RVCC_INIT;
	PORT_ADC->DDR &= ~MASK_PORT_ADC;
//	PORT_ADC->DDR |= MASK_PORT_ADC;

// ������������ ADC->CONFIG[0]
	ADC->CONFIG[0] = ADC_CFG_INIT;

// ������������ ADC->CONFIG[1]
	ADC->CONFIG[1] = ADC_CFG_INIT;


// ������������ ADC->STATUS[0]
	ADC->STATUS[0] = 0;
	ADC->STATUS[0] |= (u32)1<<4; 	// ���������� ���������� �� ��������� ��������������

// ������������ ADC->STATUS[1]
	ADC->STATUS[1] = 0;
	ADC->STATUS[1] |= (u32)1<<4; 	// ���������� ���������� �� ��������� ��������������

	NVIC_EnableIRQ(ADC_IRQn);		// ���������� ���������� �� ���
	NVIC_SetPriority(ADC_IRQn,PRIO_ADC);
}
/*------------------------------------------------------------------------------
 * ���������� ���������� ���
 *----------------------------------------------------------------------------*/
void __irq ADC_IRQHandler(void)
{
//#define DEBUG_ADC_IRQHandler	// ������� ��� ��������

	unsigned char nADC;		// ����� ADC
	unsigned long res;		// ��������� ���������
	unsigned char ch;		// ����� ������
//	static  long nTurns = NTURNS_INIT;// ���������� ��������
//	long delta_ang;			// ������� ����� ������� � ��������� ���������� ������
	static unsigned short ADC_State = 0; // ��������� ������ ADC



// ����������� ������ ADC
	nADC = ((ADC->STATUS[0]&0x04)?0:1);
// ���������� ����������
	res = ADC->RESULT[nADC];
// ����������� ������ ������
	ch = (res>>16)&0x0F;
// ���������� ���������� � �����
	Adc[ch] = res&0x0FFF;



// ��������� �������� ���������� ����������
	ADC_State |= (u16)1<<ch;

	switch(ADC_State)
	{
	// ��������� ������ ���������� +27�
	case (u16)1<<ADC_U27:

		CtrlPwm[CPWM_U27] = Adc[ADC_U27];
		ADC_State = 0;
		break;

	// ��������� ������ ���������� +310�
	case (u16)1<<ADC_U310:
		// �� ��������!
		CtrlPwm[CPWM_U310] = Adc[ADC_U310];
		ADC_State = 0;
		#ifdef CONTROL_ANALOG
	   	//���������� ����������
	    ADC_GO_ON_Uin; // ��������� �������������� �������� ����������		  
	    #endif
		break;

// ��������� ������ �������� ����������
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



