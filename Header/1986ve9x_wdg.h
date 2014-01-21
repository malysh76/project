#ifndef __WDG_H
#define __WDG_H

#include "common1.h"
#include "1986ve9x_clk.h"

//�������� ��������� ����� ���������� ��������
#define IWDG_BASE	(PERIPH_BASE + 0x68000)			//���������� ������ IWDG
#define IWDG_KR		*(__IO u16*)(IWDG_BASE + 0x00)	// [15:0] ������� �����
#define IWDG_PR		*(__IO u8*)(IWDG_BASE + 0x04)	// [2:0]  �������� ������� ����������� �������
#define IWDG_RLR	*(__IO u16*)(IWDG_BASE + 0x08)	// [11:0] ������� ��������� ����� ����������� �������
#define IWDG_SR		*(__IO u8*)(IWDG_BASE + 0x0C)	// [1:0] ������� ������� ����������� �������

//������� ���������� ������
#define WWDG_BASE	(PERIPH_BASE + 0x60000)			//���������� ������ WWDG
#define WWDG_CR		*(__IO u8*) (WWDG_BASE + 0x00)	//[7:0] ������� ����������
#define WWDG_CFR	*(__IO u16*)(WWDG_BASE + 0x04)	//[9:0] ������� ������������                         
#define WWDG_SR		*(__IO u16*)(WWDG_BASE + 0x08)	//[0]   ������� �������

/* ���� �������� WWDG_SR */
#define RVU			0x01
#define PVU			0x02
/*
 * �������� ����� (������ ������, �������� 0000h). 
 * ��� ���� ������ ���������������� ���������� �����
 * ����������� ��������� �������� ��������� AAAAh, �
 * ��������� ������ ���������� ������ ���������� �����, ����
 * ������ ������ �������� ����.
 * ������ ��������� �������� 5555h ��������� ������ �� ������ �
 * ��������� IWDG_PR � IWDG_RLR.
 * ������ ��������� �������� CCCCh ��������� ������ �����������
 * �������.
*/
#define IWDG_KR_RESET	0xAAAA
#define IWDG_KR_ACCESS	0x5555
#define IWDG_KR_START	0xCCCC

#define IWDG_INIT()	\
{\
	PER_CLOCK |= (unsigned long)1<<nIWDT;\
	IWDG_KR = IWDG_KR_ACCESS;\
	IWDG_PR = 0;\
	IWDG_KR = IWDG_KR_ACCESS;\
	IWDG_RLR = 0x1;\
	IWDG_KR = IWDG_KR_START;\
}

#define IWDG_RST()		IWDG_KR = IWDG_KR_RESET

#endif
