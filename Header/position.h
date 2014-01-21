/*
 * position.h
 *
 *  Created on: 02.02.2011
 *      Author: 129
 */

#ifndef POSITION_H_
#define POSITION_H_
#include "adc.h"
//#include "encoder.h"
#include "defports.h"
#include "stdlib.h"
#include "dac.h"
#include "sin.h"
#include "timer.h"

// ���������� ������� ����� (1...255)
#define MAX_POINT		30
#define KVIN			0x200000/1000000	// ���������� ��������� ��/��� � ����/����
#define KIQIN			1000/24				// ����������� IQ 24 ��/�������
#define DELAY_1000us	1000/33				// ��� ������������ �������� 1000 ��� �� ������� 33 ���
#define DELAY_4000us	4000/33				// ��� ������������ �������� 4000 ��� �� ������� 33 ���
#define DELAY_300us		300/33				// ��� ������������ �������� 300 ��� �� ������� 33 ���
#define DELAY_100ms		100000/33			// ��� ������������ �������� 100 �� �� ������� 33 ���
#define KU				1//40				//
#define KOSS			18//54				//
#define KOST			-10//-36			//
#define KOSU			5//85				//
#define KP				1//15				//
#define KA				700/10000//371/10000 // - 650 �������
#define KTP				307					//
#define KV2				18//36//18					//

#define SIZE_OF_EVENTS		28 // ������ ������ ��� �������� �������


/*------------------------------------------------------------------------------
 * ��������� ��� �������� ������������ � EEPROM
 *-----------------------------------------------------------------------------*/
typedef __packed struct
		{
			unsigned long AngPnt[MAX_POINT];	// 		  	������ �������� ���� ����� ���������
			unsigned long RezAng[64-MAX_POINT]; //     /256 ������ �����
			unsigned long AngInit;				//    4/260 ��������� ����
			unsigned long AngleD;				// 	  4/264 �������� ��� ���������� �������������� � ������������� ����
			unsigned long Rez[62];				//  248/512	�����

		}TCFG;

/*------------------------------------------------------------------------------
 * ������� ��������� ���������� ���������
 *-----------------------------------------------------------------------------*/
typedef struct
		{
			long 			angle_init;	// ���� ��������� ���������
			long			intgr;		// ����������
			unsigned char 	npoint;		// ����� �������� ����� ����
			long			vin;		// ������� ��������
			unsigned long	tim;		// ������� �� ������������ � ��������� ���������
			unsigned long	tim_rdy;	// ������ ������� ����������
			char 		change_task;	// ������� ����� �������
		}TPOS;

// ��������� ������� ������� ������� Direct Form II
typedef struct
		{
			 const long NOM[2];	// ���������
			 const long DEN[2];	// �����������
			 volatile long X1;	// X �� ���������� ��������
			 volatile long Y1;	// Y �� ���������� ��������
		}TDirectForm1;

// ��������� ������� ������� ������� Direct Form II
typedef struct
		{
			 const long NOM[3];	// ���������
			 const long DEN[3];	// �����������
			 /*volatile*/ long X[2];// X �� ���������� ��������
			 /*volatile*/ long Y[2];// Y �� ���������� ��������
		}TDirectForm2;

// ��������� �������� ������� � ������
typedef __packed struct
		{
			unsigned long workTime;	// 4/4
			unsigned char point;	// 1/5
			unsigned long angle;	// 4/9 ����
		}TEvent;

// ��������� ��������� �������
typedef struct
		{
			TEvent * pRdEvent;		// ��������� �� ��������� ��������� �������
			TEvent * pWrEvent;		// ��������� �� ��������� ���������� �������
		}TEventPnt;

// ������� ����������
extern TCFG	Config;
extern TPOS Pos;
extern char var;
extern TEvent		Events[SIZE_OF_EVENTS]; // ����� �������� �������
extern TEventPnt	EventPnt;				// ��������� �� �������



//------------------------------------------------------------------------------
// ����� ������ � DAC
//------------------------------------------------------------------------------
__forceinline void POS_OutDac (void)
{
	long dac0, dac1;
	dac0 = CtrlPwm[CtrlPwm[CPWM_DAC0]]*CtrlPwm[CPWM_SCALE_DAC0]/10000;
	dac1 = CtrlPwm[CtrlPwm[CPWM_DAC1]]*CtrlPwm[CPWM_SCALE_DAC1]/10000;
	SATURAT(dac0,-2040,2040);
	SATURAT(dac1,-2040,2040);
	dac0 +=2048;
	dac1 +=2048;


	DAC0_WR(dac0);
	DAC1_WR(dac1);
}


#endif /* POSITION_H_ */
