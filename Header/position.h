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

// количество угловых точек (1...255)
#define MAX_POINT		30
#define KVIN			0x200000/1000000	// коэфициент пересчета об/мин в диск/такт
#define KIQIN			1000/24				// коэффициент IQ 24 мА/дискрет
#define DELAY_1000us	1000/33				// для формирования задержки 1000 мкс на таймере 33 мкс
#define DELAY_4000us	4000/33				// для формирования задержки 4000 мкс на таймере 33 мкс
#define DELAY_300us		300/33				// для формирования задержки 300 мкс на таймере 33 мкс
#define DELAY_100ms		100000/33			// для формирования задержки 100 мс на таймере 33 мкс
#define KU				1//40				//
#define KOSS			18//54				//
#define KOST			-10//-36			//
#define KOSU			5//85				//
#define KP				1//15				//
#define KA				700/10000//371/10000 // - 650 недобег
#define KTP				307					//
#define KV2				18//36//18					//

#define SIZE_OF_EVENTS		28 // размер буфера для хранения событий


/*------------------------------------------------------------------------------
 * СТРУКТУРА ДЛЯ ХРАНЕНИЯ КОНФИГУРАЦИИ В EEPROM
 *-----------------------------------------------------------------------------*/
typedef __packed struct
		{
			unsigned long AngPnt[MAX_POINT];	// 		  	массив значений угла точек положения
			unsigned long RezAng[64-MAX_POINT]; //     /256 резерв точек
			unsigned long AngInit;				//    4/260 начальный угол
			unsigned long AngleD;				// 	  4/264 поправка для совмещения электрического и механического угла
			unsigned long Rez[62];				//  248/512	резер

		}TCFG;

/*------------------------------------------------------------------------------
 * РАБОЧАЯ СТРУКТУРА РЕГУЛЯТОРА ПОЛОЖЕНИЯ
 *-----------------------------------------------------------------------------*/
typedef struct
		{
			long 			angle_init;	// угол исходного положения
			long			intgr;		// интегратор
			unsigned char 	npoint;		// номер заданной точки угла
			long			vin;		// задание скорости
			unsigned long	tim;		// таймаут на переключение в регулятор положения
			unsigned long	tim_rdy;	// таймер сигнала готовности
			char 		change_task;	// признак смены задания
		}TPOS;

// структура фильтра первого порядка Direct Form II
typedef struct
		{
			 const long NOM[2];	// числитель
			 const long DEN[2];	// знаменатель
			 volatile long X1;	// X на предыдущей итерации
			 volatile long Y1;	// Y на пердыдущей итерации
		}TDirectForm1;

// структура фильтра второго порядка Direct Form II
typedef struct
		{
			 const long NOM[3];	// числитель
			 const long DEN[3];	// знаменатель
			 /*volatile*/ long X[2];// X на предыдущей итерации
			 /*volatile*/ long Y[2];// Y на пердыдущей итерации
		}TDirectForm2;

// структура хранения событий в памяти
typedef __packed struct
		{
			unsigned long workTime;	// 4/4
			unsigned char point;	// 1/5
			unsigned long angle;	// 4/9 байт
		}TEvent;

// структура счетчиков событий
typedef struct
		{
			TEvent * pRdEvent;		// указатель на последнее считанное событие
			TEvent * pWrEvent;		// указатель на последнее записанное событие
		}TEventPnt;

// экспорт переменных
extern TCFG	Config;
extern TPOS Pos;
extern char var;
extern TEvent		Events[SIZE_OF_EVENTS]; // буфер хранения событий
extern TEventPnt	EventPnt;				// указатели на события



//------------------------------------------------------------------------------
// вывод данных в DAC
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
