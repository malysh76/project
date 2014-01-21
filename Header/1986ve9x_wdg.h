#ifndef __WDG_H
#define __WDG_H

#include "common1.h"
#include "1986ve9x_clk.h"

//Описание регистров блока сторожевых таймеров
#define IWDG_BASE	(PERIPH_BASE + 0x68000)			//Сторожевой таймер IWDG
#define IWDG_KR		*(__IO u16*)(IWDG_BASE + 0x00)	// [15:0] Регистр Ключа
#define IWDG_PR		*(__IO u8*)(IWDG_BASE + 0x04)	// [2:0]  Делитель частоты сторожевого таймера
#define IWDG_RLR	*(__IO u16*)(IWDG_BASE + 0x08)	// [11:0] Регистр основания счета сторожевого таймера
#define IWDG_SR		*(__IO u8*)(IWDG_BASE + 0x0C)	// [1:0] Регистр статуса сторожевого таймера

//Оконный сторожевой таймер
#define WWDG_BASE	(PERIPH_BASE + 0x60000)			//Сторожевой таймер WWDG
#define WWDG_CR		*(__IO u8*) (WWDG_BASE + 0x00)	//[7:0] Регистр управления
#define WWDG_CFR	*(__IO u16*)(WWDG_BASE + 0x04)	//[9:0] Регистр конфигурации                         
#define WWDG_SR		*(__IO u16*)(WWDG_BASE + 0x08)	//[0]   Регистр статуса

/* биты регистра WWDG_SR */
#define RVU			0x01
#define PVU			0x02
/*
 * Значение ключа (только запись, читается 0000h). 
 * Эти биты должны перезаписываться программно через
 * определённые интервалы ключевым значением AAAAh, в
 * противном случае сторожевой таймер генерирует сброс, если
 * таймер достиг значения нуля.
 * Запись ключевого значения 5555h разрешает доступ по записи к
 * регистрам IWDG_PR и IWDG_RLR.
 * Запись ключевого значения CCCCh разрешает работу сторожевого
 * таймера.
*/
#define IWDG_KR_RESET	0xAAAA
#define IWDG_KR_ACCESS	0x5555
#define IWDG_KR_START	0xCCCC

#define IWDG_INIT()	\
{\
	PER_CLOCK |= (unsigned long)1<<nIWDT;\
	IWDG_KR = IWDG_KR_ACCESS;\
	IWDG_PR = 0;\
	IWDG_KR = IWDG_KR_ACCESS;\
	IWDG_RLR = 0x1;\
	IWDG_KR = IWDG_KR_START;\
}

#define IWDG_RST()		IWDG_KR = IWDG_KR_RESET

#endif
