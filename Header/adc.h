/*
 * adc.h
 *
 *  Created on: 28.02.2011
 *      Author: 129
 */

#ifndef ADC_H_
#define ADC_H_

// ������ adc
#define ADC_U27		2
#define ADC_U310	3
#define ADC_UIN		4
//#define ADC_DEBUG	7

//#define NTURNS_INIT		0//64	// �������� ������������� ���������� ��������

// ------------ ������������� ����� ��� ADC ----------------------------------
#define PORT_ADC	 GPIOD
// ����� ������������ ���������� ������
#define MASK_PORT_ADC			(\
								((u32)1<<0)|		/*Uref		*/\
								((u32)1<<1)|		/*Uref		*/\
								((u32)1<<ADC_U27)|	/*����� Ia �*/\
								((u32)1<<ADC_U310)|	/*����� Ib �*/\
								((u32)1<<ADC_UIN)	/*����� Uin */\
								)
// ��������� �������� �������
#define PER_CLOCK_ADC_INIT {PER_CLOCK |= (unsigned long)1<<nPORTD;\
							PER_CLOCK |= (unsigned long)1<<nADC;}


//����� ������ ������ �����
//00 � ����
//01 � �������� �������
//10 � �������������� �������
//11 � ���������������� �������
#define PORT_ADC_FUNC_INIT PORT_ADC->FUNC &= ~(\
											((u32)3<<(0*2))|		/*Uref		*/\
											((u32)3<<(1*2))|		/*Uref		*/\
											((u32)3<<(ADC_U27*2))|	/*����� Ia	*/\
											((u32)3<<(ADC_U310*2))|	/*����� Ib	*/\
											((u32)3<<(ADC_UIN*2))	/*����� Uin */\
											)

// ����� ������ �����������
//0 � ����������
//1 � ��������
#define	PORT_ADC_DIGEN_INIT {PORT_ADC->DIGEN &= ~MASK_PORT_ADC;\
							 PORT_ADC->DIGEN |= ((u32)1<<11) | ((u32)1<<12);}

// ������� ���������� �������� ������ � GND (100K)
//0 � �������� � ������� ���������
//1 � �������� � ������� �������� (���� �������� ~100�)
#define	PORT_ADC_RGND_INIT PORT_ADC->RGND &= ~MASK_PORT_ADC

// ������� ���������� �������� ������ � VCC (100K)
//0 � �������� � ������� ���������
//1 � �������� � ������� �������� (���� �������� ~100�)
#define	PORT_ADC_RVCC_INIT	PORT_ADC->RVCC &= ~MASK_PORT_ADC

// �������� �������� ����������
// 0 - ���������� �� AUcc � AGND
// 1 - ������� �� ADC0_REF+ � ADC0_REF-
#ifdef ADC_REF_INT
	#define ADC_REF			((u32)0<<11)
	#warning ADC_REF_INT
#else
	#ifdef ADC_REF_EXT
		#define ADC_REF		((u32)1<<11)
	#warning ADC_REF_EXT
	#endif
#endif

//����� ������������ ������� ������� �������
//0000 �  CLK
//0001 �  CLK/2
//0010 �  CLK/4	20 ���
//0011 �  CLK/8
//�
//1111 � CLK/32768
#define ADC_CLK_20MHZ	((u32)2<<12)
#define ADC_CLK_10MHZ	((u32)3<<12)
#define ADC_CLK_5MHZ	((u32)4<<12)
#define ADC_CLK_2_5MHZ	((u32)5<<12)

//#define ADC_DIV_CLK		((u32)4<<12)

// ��������� ���
#define ADC_ADON		1			// ��������� 1 ���

#define ADC_CFG_INIT \
			(ADC_CLK_10MHZ|			/*CLK/4 = 80/4 = 20 ���*/\
			 ADC_REF|				/*�������� �������� ����������*/\
			 ADC_ADON)				/* ���������  ���*/

// ������ �������������� ADCx ������ chs

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
// ������ ��������� ���������� ���� ���� +27� � �������� ���������� +310�
#define ADC_GO_ON_U27  ADC_GO_ON(1,ADC_U27)
// ������ ��������� Uin
#define ADC_GO_ON_Uin ADC_GO_ON(1,ADC_UIN)
// �������� �������
#define ADC_ACCESS_TIME {__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();}

// ������ ������ ���������
typedef enum
		{
			INIT = 0,	// ����� �������������
			WORK = 1,	// ������� �����
			TEST = 2	// �������� �����(�� �������� ��������� ���������)
		}TMODE;


// ������� ����������
extern	volatile unsigned long 	Adc[16];						// ������� ����� ���
extern 	TMODE 					workMode;

// ������� �������
extern		void 			ADC_Init		(void);

#endif /* ADC_H_ */
