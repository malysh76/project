/*
 * ports.c
 *
 *  Created on: 21.07.2011
 *      Author: 129
 */
#include "common1.h"
#include "1986ve9x_gpio.h"
#include "ports.h"
/*------------------------------------------------------------------------------
 * »Ќ»÷»јЋ»«ј÷»я ÷»‘–ќ¬ќ√ќ ¬џ’ќƒј
 * ¬’ќƒЌџ≈ ѕј–јћ≈“–џ:
 * Port - 	указатель на структуру порта
 * bit	-	разр€д порта
 * pwr 	-	00 Ц зарезервировано (передатчик отключен)
			01 Ц медленный фронт
			10 Ц быстрый фронт
			11 Ц максимально быстрый фронт
 * drv	-	0 Ц управл€емый драйвер
			1 Ц открытый сток
 *----------------------------------------------------------------------------*/

void PORT_InitDO(GPIO_TypeDef *Port,char bit,char pwr,char drv)
{
	unsigned long maskport = (unsigned long)1<<bit;
// выбор направлени€ порта
	Port->DDR |= maskport;
// выбор функции порта
	Port->FUNC &= ~((unsigned long)3<<bit*2);
// выбор режима работы порта
	Port->DIGEN |=	maskport;
// выбор выходного фронта
	Port->PWR  &= ~((unsigned long)3<<(bit*2));
	Port->PWR  |= (unsigned long)pwr<<(bit*2);
// регистр разрешени€ подд€жки вывода к GND (100K)
	Port->RGND &=~maskport;
// регистр разрешени€ подд€жки вывода к VCC (100K)
	Port->RVCC &=~maskport;
// регистр разрешени€ режима вывода "открытый сток"
	Port->ODEN	= (drv? Port->ODEN|maskport : Port->ODEN&(~maskport));
// регистр разрешени€ входного гистерезиса (400mV)
//	Port->TREN	= (gist? Port->TREN|maskport : Port->TREN&(~maskport));
// регистр разрешени€ входного фильтра "иголок"
//	Port->GFEN	= (fltr? Port->GFEN|maskport : Port->GFEN&(~maskport));
	Port->RXTX  = 0;
}

/*------------------------------------------------------------------------------
 * »Ќ»÷»јЋ»«ј÷»я ÷»‘–ќ¬ќ√ќ ¬’ќƒј
 * ¬’ќƒЌџ≈ ѕј–јћ≈“–џ:
 * Port - 	указатель на структуру порта
 * bit	-	разр€д порта
 * pull	-	0 - подт€жка выключена
 * 			1 - подт€жка к VCC,
 * 			2 - к GND
 * gist -	0 Ц триггер Ўмита выключен гистерезис 200 м¬.
			1 Ц триггер Ўмита включен гистерезис 400 м¬.
 * fltr	-	0 - фильтр иголок выключен
 * 			1 - фильтр иголок включен
 *----------------------------------------------------------------------------*/

void PORT_InitDI(GPIO_TypeDef *Port,char bit,char pull,char gist,char fltr)
{
	unsigned long maskport = (unsigned long)1<<bit;
// выбор направлени€ порта
	Port->DDR &= ~maskport;
// выбор функции порта
	Port->FUNC &= ~((unsigned long)3<<bit*2);;
// выбор режима работы порта
	Port->DIGEN |=	maskport;
// выбор выходного фронта
//	Port->PWR = 	PORTD_PWR;
// регистр разрешени€ подд€жки вывода к GND (100K)
// регистр разрешени€ подд€жки вывода к VCC (100K)
	switch(pull)
	{
	case 0:
		Port->RGND &=~maskport;
		Port->RVCC &=~maskport;
		break;
	case 1:
		Port->RGND &=~maskport;
		Port->RVCC |= maskport;
		break;
	case 2:
		Port->RGND |= maskport;
		Port->RVCC &=~maskport;
		break;
	}
// регистр разрешени€ режима вывода "открытый сток"
//	Port->ODEN	= PORTD_ODEN;
// регистр разрешени€ входного гистерезиса (400mV)
	Port->TREN	= (gist? Port->TREN|maskport : Port->TREN&(~maskport));
// регистр разрешени€ входного фильтра "иголок"
	Port->GFEN	= (fltr? Port->GFEN|maskport : Port->GFEN&(~maskport));
}

//---------------------------------------------------------------------
// инициализаци€ порта на алтернативные функции
//---------------------------------------------------------------------
void PORT_InitFN(GPIO_TypeDef *Port,char bit,unsigned char mode, unsigned char pwr)
{
	unsigned long maskport = (unsigned long)1<<bit;
// выбор направлени€ порта
	Port->DDR &= ~maskport;
// выбор функции порта
	Port->FUNC &= ~((unsigned long)3<<bit*2);
	Port->FUNC |= ((unsigned long)mode<<bit*2);
// выбор режима работы порта
	Port->DIGEN |=	maskport;
// выбор выходного фронта
	Port->PWR &= ~((unsigned long)3<<bit*2);
	Port->PWR |= ((unsigned long)pwr<<bit*2);
// регистр разрешени€ подд€жки вывода к GND (100K)
	Port->RGND &=~maskport;
// регистр разрешени€ подд€жки вывода к VCC (100K)
	Port->RVCC &=~maskport;
// регистр разрешени€ режима вывода "открытый сток"
	Port->ODEN	&= ~maskport;
// регистр разрешени€ входного гистерезиса (400mV)
	Port->TREN	&= ~maskport;
// регистр разрешени€ входного фильтра "иголок"
	Port->GFEN	&= ~maskport;
}
