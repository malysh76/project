/*
 * boot.h
 *
 *  Created on: 18.05.2011
 *      Author: 129
 */

#ifndef BOOT_H_
#define BOOT_H_
#include "protocol.h"
#define VERBOOT				0x0100

#define SIGN_REPROG			0x0C0C0C0C

typedef void(*TWorkFunc)(void);

// экспорт переменных
extern TInfo	Info;


#endif /* BOOT_H_ */
