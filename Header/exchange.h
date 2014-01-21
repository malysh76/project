/*
 * exchange.h
 *
 *  Created on: 13.07.2011
 *      Author: 129
 */

#ifndef EXCHANGE_H_
#define EXCHANGE_H_
// зависимости
#include "1986ve9x_irq.h"

#define	EX_BUSY_SET		PORT_SET(PORT_EX_CTRL,BIT_BUSY)
#define	EX_BUSY_CLR		PORT_CLR(PORT_EX_CTRL,BIT_BUSY)
#define	EX_BUSY_FLP		PORT_FLP(PORT_EX_CTRL,BIT_BUSY)

#define	EX_EXT_SET		PORT_SET(PORT_EX_CTRL,BIT_EXT)
#define	EX_EXT_CLR		PORT_CLR(PORT_EX_CTRL,BIT_EXT)
#define	EX_EXT_FLP		PORT_FLP(PORT_EX_CTRL,BIT_EXT)

#define	EX_MODE_SET		PORT_SET(PORT_EX_CTRL,BIT_MODE)
#define	EX_MODE_CLR		PORT_CLR(PORT_EX_CTRL,BIT_MODE)
#define	EX_MODE_FLP		PORT_FLP(PORT_EX_CTRL,BIT_MODE)

#define EX_BUSY			PORT_IN(PORT_EX_CTRL,BIT_BUSY)
#define EX_MODE			PORT_IN(PORT_EX_CTRL,BIT_MODE)
#define EX_EXT			PORT_IN(PORT_EX_CTRL,BIT_EXT)


#define INIT_EXT(GPIOX,bit,IRQn) \
{\
	GPIOX->FUNC &= ~((unsigned long)3<<(bit*2));\
	GPIOX->FUNC |= (unsigned long)2<<(bit*2);\
	GPIOX->DIGEN |= 1<<bit;\
	NVIC_EnableIRQ(IRQn);\
}



//------------------------------------------------------------------------------
// получить данные энкодера 5 байт
// 0 - данные получены
// 1 - ошибка приема
//------------------------------------------------------------------------------
/*
__forceinline unsigned char EX_GetData (unsigned char * pdata)
{
#define WAIT_EX		100
	unsigned long wait = WAIT_EX;
	unsigned char res = 1;
	while(wait--)
	{
		if(!EX_BUSY)
			{
				*(__packed unsigned short*)&pdata[0] = PORT_EX_WORD_LO->RXTX;
				*(__packed unsigned short*)&pdata[2] = PORT_EX_WORD_ME->RXTX;
				pdata[4] = (PORT_EX_BYTE_HI->RXTX)&0x01FE;
				res = 0;
				break;
			}
	}
	return res;
}
*/
//------------------------------------------------------------------------------
// экспорт функций
extern void 			EX_Init		(unsigned char typeboard);
#endif /* EXCHANGE_H_ */
