/*
 * encoder.h
 *
 *  Created on: 27.04.2011
 *      Author: 129
 */

#ifndef ENCODER_H_
#define ENCODER_H_

//#include "MDR32Fx.h"
#include "1986ve9x_clk.h"
#include "1986ve9x_gpio.h"
#include "timer.h"
#define ENC_MAX_DIGIT			21	// максимально допустимое количество разр€дов
// ------------ »Ќ»÷»јЋ»«ј÷»я ѕќ–“ј ƒЋя ЁЌ ќƒ≈–ј ----------------------------------
#define PORT_ENC	 		GPIOB
#define DIGIT_ENC_CLOCK		5
#define DIGIT_ENC_DATA		6
#define INIT_PORT_ENC \
{\
/* включение тактовой частоты*/\
	PER_CLOCK |= (unsigned long)1<<nPORTB;\
/* –ежим работы контроллера	*/\
/*0 Ц аналоговый			*/\
/*1 Ц цифровой				*/\
/*включение функции цифрового порта*/\
	PORT_ENC->FUNC &= ~((unsigned long)3<<(DIGIT_ENC_CLOCK*2)|(unsigned long)3<<(DIGIT_ENC_DATA*2));\
	PORT_ENC->DIGEN |=  (unsigned long)1<<DIGIT_ENC_DATA|(unsigned long)1<<DIGIT_ENC_CLOCK;\
/*направление 0 - вход, 1 - выход*/\
	PORT_ENC->DDR |= (unsigned long)1<<DIGIT_ENC_CLOCK;\
/*–ежим мощности передатчика 				*/\
/*00 Ц зарезервировано (передатчик отключен)*/\
/*01 Ц медленный фронт						*/\
/*10 Ц быстрый фронт						*/\
/*11 Ц максимально быстрый фронт			*/\
	PORT_ENC->PWR |=  (unsigned long)2<<(2*DIGIT_ENC_CLOCK);\
}



//--------------------- ”ѕ–ј¬Ћ≈Ќ»≈ ЁЌ ќƒ≈–ќћ -----------------------------------
// управление clock

#define ENC_CLK_SET				PORT_ENC->RXTX = (unsigned long)1<<DIGIT_ENC_CLOCK
#define ENC_CLK_CLR				PORT_ENC->RXTX = 0//~((unsigned long)1<<DIGIT_ENC_CLOCK)

#define ENC_MAKE_CLOCK \
{\
	ENC_CLK_CLR;\
	DelayNs(500);\
	ENC_CLK_SET;\
	DelayNs(500);\
}
// чтение data
#define ENC_DATA				PORT_IN(PORT_ENC,DIGIT_ENC_DATA) // ((PORT_ENC->RXTX >> DIGIT_ENC_DATA)&0x01)


//-------------- —“–” “”–џ -----------------------------------------------------
// структура обмена данными обработчика энкодера с центральным микроконтроллером
typedef __packed union
		{
		__packed struct
			{
				unsigned	vlc	:11;	// 	11/11	скорость (580 дискрет + знак)
				unsigned	ang	:21;	// 	21/38	угол
				unsigned	rez1:1;		//  02/40	резерв дл€ выравнивани€ (здесь DAC)
				unsigned	acl	:6;		// 	06/17	ускорение (20 дискрет + знак)
				unsigned	rez2:1;		//  02/40	резерв дл€ выравнивани€ (здесь DAC)
			}Data;
		__packed struct
			{
			unsigned short  word_lo;	//  16/16   дл€ обмена
			unsigned short  word_me;	//	16/32
			unsigned char	byte_hi;	//  08/40
			}Dump;
		}TEncoder;

//-----------------------------------------------------------------------------
__forceinline unsigned long ENC_GetData(unsigned char digit)
{
	unsigned long res = 0;	// результат
	unsigned char flt = 0;	// переменна€ дл€ фильтрации считанных данных
	unsigned char cnt = 0;	// счетчик разр€дов
#define PERIOD	1440		// 1300 - 1,8 мкс
	// проверка на допустимое количество разр€дов
	digit = (digit>ENC_MAX_DIGIT?ENC_MAX_DIGIT:digit);
	digit++;
	ENC_CLK_CLR;
	DelayNs(500);	// 0,45 мкс
	while(digit)
	{
		ENC_CLK_SET;
		DelayNs(PERIOD/2);
		ENC_CLK_CLR;
		flt += ENC_DATA;
		DelayNs(PERIOD/4);
		flt += ENC_DATA;
		DelayNs(PERIOD/4);
		flt += ENC_DATA;
		digit--;
		res |= ((flt>>1)&0x01)<<digit;
/*		if(!cnt)
			if(!res)
				return 0xffffffff;	// выход если ALARM = 0;
			else
				res = 0;			// очистка разр€да ALARM в результате
*/
		cnt++;
		flt = 0;
	}
	ENC_CLK_SET;
	res &=0x1FFFFF;
//	DelayNs(16000);
//	DelayNs(28000);
	return res;
}

//-------- Ё —ѕќ–“ ѕ≈–≈ћ≈ЌЌџ’ --------------------------------------------------
extern volatile TEncoder Encoder;
extern volatile unsigned long delay;
//-------- Ё —ѕќ–“ ‘”Ќ ÷»… -----------------------------------------------------
extern void				ENC_Init	(void);



#endif /* ENCODER_H_ */
