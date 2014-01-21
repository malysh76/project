/*
 * tgrn.h
 *
 *  Created on: 27.12.2010
 *      Author: 129
 */
#ifndef __TGRN_H__
	#define __TGRN_H__
	#define K_GAMMA		(65536/360)
	extern 	short 			PWM_sin		(unsigned short gamma);
	extern	short 			PWM_cos		(unsigned short gamma);
	extern	unsigned short 	PWM_atan	(short Ud, short Uq);
#endif
