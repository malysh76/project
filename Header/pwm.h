/*
 * pwm.h
 *
 *  Created on: 06.01.2011
 *      Author: 129
 */

#ifndef PWM_H_
#define PWM_H_
#include "tgrn.h"
// ограничение
#define SATURAT(val, min, max)\
{\
	if(val < min)	val = min;\
	else if (val>max) val = max;\
}



//-----  ќЌ—“јЌ“џ --------------------------------------------------------------
#define TPWM		(PLLCPUOUT*1000/FPWM)	// период PWM (80 000 к√ц / 30 к√ц = 2666)
#define MAXPWM		(TPWM/2)	// максимальное значение регистра сравнени€


#endif /* PWM_H_ */
