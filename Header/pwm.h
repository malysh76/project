/*
 * pwm.h
 *
 *  Created on: 06.01.2011
 *      Author: 129
 */

#ifndef PWM_H_
#define PWM_H_
#include "tgrn.h"
// �����������
#define SATURAT(val, min, max)\
{\
	if(val < min)	val = min;\
	else if (val>max) val = max;\
}



//----- ��������� --------------------------------------------------------------
#define TPWM		(PLLCPUOUT*1000/FPWM)	// ������ PWM (80 000 ��� / 30 ��� = 2666)
#define MAXPWM		(TPWM/2)	// ������������ �������� �������� ���������


#endif /* PWM_H_ */
