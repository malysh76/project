/*
 * ports.h
 *
 *  Created on: 21.07.2011
 *      Author: 129
 */

#ifndef PORTS_H_
#define PORTS_H_

extern void PORT_InitDI(GPIO_TypeDef *Port,char bit,char pull,char gist,char fltr);
extern void PORT_InitDO(GPIO_TypeDef *Port,char bit,char pwr,char drv);
extern void PORT_InitFN(GPIO_TypeDef *Port,char bit,unsigned char mode, unsigned char pwr);
#endif /* PORTS_H_ */
