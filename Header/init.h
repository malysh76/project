/*
 * init.h
 *
 *  Created on: 01.03.2011
 *      Author: 129
 */

#ifndef INIT_H_
#define INIT_H_

// частота резонатора в МГц
#define HSE		8
// тактовая частота, VU
//#define


// ЭКСПОРТ ПЕРЕМЕННЫХ

// ЭКСПОРТ ФУНКЦИЙ
extern	void Init(void);
extern	void INIT_CPUPLL(void);
extern	void INIT_Timer(void);

#endif /* INIT_H_ */
