/*
 * CtrlPwm.h
 *
 *  Created on: 18.04.2011
 *      Author: 129
 */

#ifndef CTRLPWM_H_
#define CTRLPWM_H_

#define SIZE_CTRL_PWM		100	// размер массива переменной CtrlPwm

// перечень переменных в массиве CtrlPwm.
#define CPWM_MGN			0	// магнитуда
#define CPWM_ANGL			1	// текущий угол для PWM машины
#define CPWM_BETTA			2	// угол вектора напряжения в координатах статора
#define	CPWM_IQIN			3	// задание
#define CPWM_IQ				4	// составляющая тока q
#define CPWM_ID				5	// составляющая тока d
#define CPWM_DIQ			6	// ошибка по току Iq
#define CPWM_DID			7	// ошибка по току Id
#define CPWM_UD				8	// напряжения
#define CPWM_UQ				9	// напряжения
#define	CPWM_TETTA			10	// угол вектора напряжения
#define CPWM_IA				11	// ток в фазе A с учетом смещения 0
#define CPWM_IB				12	// ток в фазе B с учетом смещения 0
#define CPWM_IC				13	// ток в фазе С с учетом смещения 0
#define CPWM_OFST_IA		14	// смещение нуля тока A
#define CPWM_OFST_IB		15	// смещение нуля тока B
#define CPWM_OFST_IC		16	// смещение нуля тока C
#define CPWM_VLC			17	// скорость
#define CPWM_ACL			18	// ускорение
#define CPWM_DEBUG1			19	// отладка1
#define CPWM_DEBUG2			20	// отладка 2
#define CPWM_DANGLE			21	// поправка механического угла к электрическому
#define CPWM_ANGLINIT		22	// базовый угол оптической системы
#define CPWM_DEBUG3			23	// отладка 2
#define CPWM_DEBUG4			24	// отладка 2
#define CPWM_KPI1			25	// коэфициент пропорц в обратной связи по току
#define CPWM_KP				26	// отладка коэфициента пропорциональности
#define CPWM_KU				27	// отладка коэфициента интегратора
#define CPWM_KOSS			28	// отладка коэфициента ос скорости
#define CPWM_KOSU			29	// отладка коэфициента ос ускорения
#define CPWM_IQINN			30	// текущее управление
#define CPWM_KA				31	// коэфициент при квадрате скорости
#define CPWM_ANGLFUL		32	// полный угол 21 разряд
#define CPWM_DAC0			33	// код переменной для вывода в DAC0
#define CPWM_DAC1			34	// код переменной для вывода в DAC1
#define CPWM_SCALE_DAC0		35	// коэфициент для вывода в DAC0
#define CPWM_SCALE_DAC1		36	// коэфициент для вывода в DAC1
#define CPWM_KTP			37	//
#define CPWM_KOST			38	//
#define CPWM_DELTA			39	// значение ошибки
#define CPWM_KV				40	// коэфициент обратной связи
#define CPWM_K1				41	//K1
#define CPWM_K2				42	//K2
#define CPWM_DEBUG7			43	//
#define CPWM_DEBUG8			44	//
#define CPWM_DEBUG9			45	//
#define CPWM_DEBUG10		46	//
#define CPWM_POINT			47	// заданная точка позиционирования
#define CPWM_MODE			48	// режим работы коммутатора
#define CPWM_ANGLTASK		49	// текущий заданный угол на шаге заданным в CPWM_POINT
#define CPWM_VERSOFT		50  // версия прошивки
#define CPWM_WORD1			51	//word1
#define CPWM_WORD2			52	//word2
#define CPWM_WORD3			53	//word3
#define CPWM_INCR			54	//increm
#define CPWM_COUNT			55	//count
#define CPWM_U27			56	//+27В
#define CPWM_U310			57	//+310В
#define CPWM_IQIN_			58	//заданное управление
#define CPWM_CAN_COUNT		59	//CAN Count
#define CPWM_CAN_KOMAND		60	//Команда
#define CPWM_CAN_ID			61	//CAN ID
#define CPWM_CAN_DLC		62	//CAN DLC
#define CPWM_CAN_DATAH		63	//CAN DATAH
#define CPWM_CAN_DATAL		64	//CAN DATAL
#define CPWM_RS_DATA3		65	//данные байт 3
#define CPWM_SWITCH			66	//Комутатор - Выбор двигателя
#define CPWM_TEMP3			67	//temp3
#define CPWM_POR_IQIN		68	//Ограничение Iqin по мощности
#define CPWM_IALFA			69	//ialfa
#define CPWM_IBETTA			70	//ibeta
#define CPWM_INTIQ			71	//интегратор Iq
#define CPWM_INTID			72	//интегратор Iq
#define CPWM_VLC_FLTR		73  // фильтрованная скорость
#define CPWM_POR2_IQIN		74	// порог ограничения IQIN
#define CPWM_KINTV			75	// коэфициент ОС по скорости при интеграторе
#define CPWM_INTVLC			76	// интегратор ошибки по скорости
#define CPWM_CAN_STATUS_IN	77  // входной статус заданный по CAN
#define CPWM_CAN_STATUS_UM	78  // выходной статус УМ
#define CPWM_TIMCNT			79	// значение регистра CNT таймера основного цикла




#endif /* CTRLPWM_H_ */
