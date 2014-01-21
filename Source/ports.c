/*
 * ports.c
 *
 *  Created on: 21.07.2011
 *      Author: 129
 */
#include "common1.h"
#include "1986ve9x_gpio.h"
#include "ports.h"
/*------------------------------------------------------------------------------
 * ������������� ��������� ������
 * ������� ���������:
 * Port - 	��������� �� ��������� �����
 * bit	-	������ �����
 * pwr 	-	00 � ��������������� (���������� ��������)
			01 � ��������� �����
			10 � ������� �����
			11 � ����������� ������� �����
 * drv	-	0 � ����������� �������
			1 � �������� ����
 *----------------------------------------------------------------------------*/

void PORT_InitDO(GPIO_TypeDef *Port,char bit,char pwr,char drv)
{
	unsigned long maskport = (unsigned long)1<<bit;
// ����� ����������� �����
	Port->DDR |= maskport;
// ����� ������� �����
	Port->FUNC &= ~((unsigned long)3<<bit*2);
// ����� ������ ������ �����
	Port->DIGEN |=	maskport;
// ����� ��������� ������
	Port->PWR  &= ~((unsigned long)3<<(bit*2));
	Port->PWR  |= (unsigned long)pwr<<(bit*2);
// ������� ���������� �������� ������ � GND (100K)
	Port->RGND &=~maskport;
// ������� ���������� �������� ������ � VCC (100K)
	Port->RVCC &=~maskport;
// ������� ���������� ������ ������ "�������� ����"
	Port->ODEN	= (drv? Port->ODEN|maskport : Port->ODEN&(~maskport));
// ������� ���������� �������� ����������� (400mV)
//	Port->TREN	= (gist? Port->TREN|maskport : Port->TREN&(~maskport));
// ������� ���������� �������� ������� "������"
//	Port->GFEN	= (fltr? Port->GFEN|maskport : Port->GFEN&(~maskport));
	Port->RXTX  = 0;
}

/*------------------------------------------------------------------------------
 * ������������� ��������� �����
 * ������� ���������:
 * Port - 	��������� �� ��������� �����
 * bit	-	������ �����
 * pull	-	0 - �������� ���������
 * 			1 - �������� � VCC,
 * 			2 - � GND
 * gist -	0 � ������� ����� �������� ���������� 200 ��.
			1 � ������� ����� ������� ���������� 400 ��.
 * fltr	-	0 - ������ ������ ��������
 * 			1 - ������ ������ �������
 *----------------------------------------------------------------------------*/

void PORT_InitDI(GPIO_TypeDef *Port,char bit,char pull,char gist,char fltr)
{
	unsigned long maskport = (unsigned long)1<<bit;
// ����� ����������� �����
	Port->DDR &= ~maskport;
// ����� ������� �����
	Port->FUNC &= ~((unsigned long)3<<bit*2);;
// ����� ������ ������ �����
	Port->DIGEN |=	maskport;
// ����� ��������� ������
//	Port->PWR = 	PORTD_PWR;
// ������� ���������� �������� ������ � GND (100K)
// ������� ���������� �������� ������ � VCC (100K)
	switch(pull)
	{
	case 0:
		Port->RGND &=~maskport;
		Port->RVCC &=~maskport;
		break;
	case 1:
		Port->RGND &=~maskport;
		Port->RVCC |= maskport;
		break;
	case 2:
		Port->RGND |= maskport;
		Port->RVCC &=~maskport;
		break;
	}
// ������� ���������� ������ ������ "�������� ����"
//	Port->ODEN	= PORTD_ODEN;
// ������� ���������� �������� ����������� (400mV)
	Port->TREN	= (gist? Port->TREN|maskport : Port->TREN&(~maskport));
// ������� ���������� �������� ������� "������"
	Port->GFEN	= (fltr? Port->GFEN|maskport : Port->GFEN&(~maskport));
}

//---------------------------------------------------------------------
// ������������� ����� �� ������������� �������
//---------------------------------------------------------------------
void PORT_InitFN(GPIO_TypeDef *Port,char bit,unsigned char mode, unsigned char pwr)
{
	unsigned long maskport = (unsigned long)1<<bit;
// ����� ����������� �����
	Port->DDR &= ~maskport;
// ����� ������� �����
	Port->FUNC &= ~((unsigned long)3<<bit*2);
	Port->FUNC |= ((unsigned long)mode<<bit*2);
// ����� ������ ������ �����
	Port->DIGEN |=	maskport;
// ����� ��������� ������
	Port->PWR &= ~((unsigned long)3<<bit*2);
	Port->PWR |= ((unsigned long)pwr<<bit*2);
// ������� ���������� �������� ������ � GND (100K)
	Port->RGND &=~maskport;
// ������� ���������� �������� ������ � VCC (100K)
	Port->RVCC &=~maskport;
// ������� ���������� ������ ������ "�������� ����"
	Port->ODEN	&= ~maskport;
// ������� ���������� �������� ����������� (400mV)
	Port->TREN	&= ~maskport;
// ������� ���������� �������� ������� "������"
	Port->GFEN	&= ~maskport;
}
