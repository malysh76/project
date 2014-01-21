/*
 * dac.h
 *
 *  Created on: 11.04.2011
 *      Author: 129
 */

#ifndef DAC_H_
#define DAC_H_
//�����������
#include "1986ve9x_dac.h"
// ------------ ������������� ����� ��� DAC ----------------------------------
#define PORT_DAC	 GPIOE
// ����� ������������ ���������� ������
#define MASK_PORT_DAC	(((unsigned long)1<<9)|((unsigned long)1<<0))
// ��������� �������� �������
#define PER_CLOCK_DAC_INIT {PER_CLOCK |= (unsigned long)1<<nPORTE;\
							PER_CLOCK |= (unsigned long)1<<nDAC;}

//����� ������ ������ �����
//00 � ����
//01 � �������� �������
//10 � �������������� �������
//11 � ���������������� �������
#define PORT_DAC_FUNC_INIT PORT_DAC->FUNC &= ~(((unsigned long)3<<(9*2))|((unsigned long)3<<(0*2)))

// ����� ������ �����������
//0 � ����������
//1 � ��������
#define	PORT_DAC_DIGEN_INIT PORT_DAC->DIGEN &= ~MASK_PORT_DAC

// ������� ���������� �������� ������ � GND (100K)
//0 � �������� � ������� ���������
//1 � �������� � ������� �������� (���� �������� ~100�)
#define	PORT_DAC_RGND_INIT PORT_DAC->RGND &= ~MASK_PORT_DAC

// ������� ���������� �������� ������ � VCC (100K)
//0 � �������� � ������� ���������
//1 � �������� � ������� �������� (���� �������� ~100�)
#define	PORT_DAC_RVCC_INIT	PORT_DAC->RVCC &= ~MASK_PORT_DAC


//4          Cfg_SYNC_A          ������������� DAC0 � DAC1
//                                0 � �����������
//                                1 � ����������
#define 		Cfg_SYNC_A		  0

// 3             Cfg_ON_DAC1      ��������� DAC1
//                                1 � �������
//                                0 � ��������
#define 		Cfg_ON_DAC1		  1

// 2             Cfg_ON_DAC0      ��������� DAC0
//                                1 � �������
//                                0 � ��������
#define 		Cfg_ON_DAC0		  1

// 1               Cfg_M_REF1     ����� ��������� �������� ���������� DAC1
//                                0 � � �������� �������� ���������� ������������ ����������
//                                ������� � ������ AUcc.
//                                1 � � �������� �������� ���������� ������������ ����������
//                                �� ����� �������� ���������� DAC1_REF.
#define 		Cfg_M_REF1		  0

// 0               Cfg_M_REF0     ����� ��������� �������� ���������� DAC0
//                                0 � � �������� �������� ���������� ������������ ����������
//                                ������� � ������ AUcc.
//                                1 � � �������� �������� ���������� ������������ ����������
//                                �� ����� �������� ���������� DAC0_REF.
#define			Cfg_M_REF0		  0

#define			DAC_CFG_INIT		(unsigned long)Cfg_SYNC_A <<4|\
									(unsigned long)Cfg_ON_DAC1<<3|\
									(unsigned long)Cfg_ON_DAC0<<2|\
									(unsigned long)Cfg_M_REF1 <<1|\
									(unsigned long)Cfg_M_REF0 <<0
// ������ � dac
#define			DAC0_WR(dac)		DAC->DAC0 = ((unsigned long)dac)
#define			DAC1_WR(dac)		DAC->DAC1 = ((unsigned long)dac)
// ������ dac
#define			DAC0_RD				(DAC->DAC0 & 0x0FFF)
#define			DAC1_RD				(DAC->DAC1 & 0x0FFF)

// ������� �������
extern void DAC_Init (void);

#endif /* DAC_H_ */
