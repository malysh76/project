/*
 * encoder.h
 *
 *  Created on: 27.04.2011
 *      Author: 129
 */

#ifndef ENCODER_H_
#define ENCODER_H_

//#include "MDR32Fx.h"
#include "1986ve9x_clk.h"
#include "1986ve9x_gpio.h"
#include "timer.h"
#define ENC_MAX_DIGIT			21	// ����������� ���������� ���������� ��������
// ------------ ������������� ����� ��� �������� ----------------------------------
#define PORT_ENC	 		GPIOB
#define DIGIT_ENC_CLOCK		5
#define DIGIT_ENC_DATA		6
#define INIT_PORT_ENC \
{\
/* ��������� �������� �������*/\
	PER_CLOCK |= (unsigned long)1<<nPORTB;\
/* ����� ������ �����������	*/\
/*0 � ����������			*/\
/*1 � ��������				*/\
/*��������� ������� ��������� �����*/\
	PORT_ENC->FUNC &= ~((unsigned long)3<<(DIGIT_ENC_CLOCK*2)|(unsigned long)3<<(DIGIT_ENC_DATA*2));\
	PORT_ENC->DIGEN |=  (unsigned long)1<<DIGIT_ENC_DATA|(unsigned long)1<<DIGIT_ENC_CLOCK;\
/*����������� 0 - ����, 1 - �����*/\
	PORT_ENC->DDR |= (unsigned long)1<<DIGIT_ENC_CLOCK;\
/*����� �������� ����������� 				*/\
/*00 � ��������������� (���������� ��������)*/\
/*01 � ��������� �����						*/\
/*10 � ������� �����						*/\
/*11 � ����������� ������� �����			*/\
	PORT_ENC->PWR |=  (unsigned long)2<<(2*DIGIT_ENC_CLOCK);\
}



//--------------------- ���������� ��������� -----------------------------------
// ���������� clock

#define ENC_CLK_SET				PORT_ENC->RXTX = (unsigned long)1<<DIGIT_ENC_CLOCK
#define ENC_CLK_CLR				PORT_ENC->RXTX = 0//~((unsigned long)1<<DIGIT_ENC_CLOCK)

#define ENC_MAKE_CLOCK \
{\
	ENC_CLK_CLR;\
	DelayNs(500);\
	ENC_CLK_SET;\
	DelayNs(500);\
}
// ������ data
#define ENC_DATA				PORT_IN(PORT_ENC,DIGIT_ENC_DATA) // ((PORT_ENC->RXTX >> DIGIT_ENC_DATA)&0x01)


//-------------- ��������� -----------------------------------------------------
// ��������� ������ ������� ����������� �������� � ����������� �����������������
typedef __packed union
		{
		__packed struct
			{
				unsigned	vlc	:11;	// 	11/11	�������� (580 ������� + ����)
				unsigned	ang	:21;	// 	21/38	����
				unsigned	rez1:1;		//  02/40	������ ��� ������������ (����� DAC)
				unsigned	acl	:6;		// 	06/17	��������� (20 ������� + ����)
				unsigned	rez2:1;		//  02/40	������ ��� ������������ (����� DAC)
			}Data;
		__packed struct
			{
			unsigned short  word_lo;	//  16/16   ��� ������
			unsigned short  word_me;	//	16/32
			unsigned char	byte_hi;	//  08/40
			}Dump;
		}TEncoder;

//-----------------------------------------------------------------------------
__forceinline unsigned long ENC_GetData(unsigned char digit)
{
	unsigned long res = 0;	// ���������
	unsigned char flt = 0;	// ���������� ��� ���������� ��������� ������
	unsigned char cnt = 0;	// ������� ��������
#define PERIOD	1440		// 1300 - 1,8 ���
	// �������� �� ���������� ���������� ��������
	digit = (digit>ENC_MAX_DIGIT?ENC_MAX_DIGIT:digit);
	digit++;
	ENC_CLK_CLR;
	DelayNs(500);	// 0,45 ���
	while(digit)
	{
		ENC_CLK_SET;
		DelayNs(PERIOD/2);
		ENC_CLK_CLR;
		flt += ENC_DATA;
		DelayNs(PERIOD/4);
		flt += ENC_DATA;
		DelayNs(PERIOD/4);
		flt += ENC_DATA;
		digit--;
		res |= ((flt>>1)&0x01)<<digit;
/*		if(!cnt)
			if(!res)
				return 0xffffffff;	// ����� ���� ALARM = 0;
			else
				res = 0;			// ������� ������� ALARM � ����������
*/
		cnt++;
		flt = 0;
	}
	ENC_CLK_SET;
	res &=0x1FFFFF;
//	DelayNs(16000);
//	DelayNs(28000);
	return res;
}

//-------- ������� ���������� --------------------------------------------------
extern volatile TEncoder Encoder;
extern volatile unsigned long delay;
//-------- ������� ������� -----------------------------------------------------
extern void				ENC_Init	(void);



#endif /* ENCODER_H_ */
