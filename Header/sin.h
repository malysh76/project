/*
 * sin.h
 *
 *  Created on: 23.05.2011
 *      Author: 129
 */

#ifndef SIN_H_
#define SIN_H_
#define SIZEOFEXP	80

// ÄËÈÒÅËÜÍÎÑÒÜ ÏÅÐÅÕÎÄÍÎÃÎ ÏÐÎÖÅÑÑÀ
#define trans_5ms
//#define trans_10ms
//#define trans_13ms
//#define trans_13ms_m
//#define trans_debug

#ifdef trans_5ms
#define SIZEOFTASK	167
#endif

#ifdef trans_10ms
#define SIZEOFTASK	334 // 167
#endif

#ifdef trans_13ms
#define SIZEOFTASK	394 // 167
#endif

#ifdef trans_13ms_m
#define SIZEOFTASK	394 // 167
#endif

#ifdef trans_debug
#define SIZEOFTASK	1 // 167
#endif

#include "stdlib.h"
extern const unsigned short 	sin_tbl[16385];
extern const unsigned short 	atan_tbl[8193];
extern const 		  short		exp[SIZEOFEXP];
extern const 		  short     stepTask[SIZEOFTASK];

__forceinline short satofvel (long vlc)
{
	vlc = abs(vlc);
	if(vlc>=SIZEOFEXP)
		vlc = SIZEOFEXP-1;
	return exp[vlc];
}
#endif /* SIN_H_ */
