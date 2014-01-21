/*
 * p7e.h
 *
 *  Created on: 28.06.2013
 *      Author: 129
 */

#ifndef P7E_H_
#define P7E_H_
#include "MDR32Fx.h"
#include "uart.h"
#include "defports.h"


/*=========================== КОМАНДЫ ПРОТОКОЛА 7E ===========================*/
		//Команда	Параметры	Описание
#define P7E_CMD_VN_CTRL_R    0x91	//Целое, знаковое 16 бит	Принять вход ВН
#define P7E_CMD_VN_CTRL_A    0x11	//2 целых знаковых 16бит
									//1-й ток электродвигателя
									//2-й скорость электродвигателя	Ответ на п.1
#define P7E_CMD_VN_STAT_R    0xA1	//Нет	Сообщить статус ВН
#define P7E_CMD_VN_STAT_A    0x21	//1 байт, набор бит указывающий неиспри	Ответ на п.3
#define P7E_CMD_VN_LOT_R     0xB1	//Нет	Сообщить серийный номер ВН
#define P7E_CMD_VN_LOT_A     0x31	//4 байта, 2 байта – 2-е буквы серийного номера в кодировке ANSI, 2 байты – номер (беззнаковое целое) 	Ответ на п.5
#define P7E_CMD_GN_CTRL_R    0x92	//Аналогично п.1	Принять вход ГН
#define P7E_CMD_GN_CTRL_A    0x12	//Аналогично п.2	Ответ на п.7
#define P7E_CMD_GN_STAT_R    0xA2	//Аналогично п.3	Сообщить статус ГН
#define P7E_CMD_GN_STAT_A    0x22	//Аналогично п.4	Ответ на п.9
#define P7E_CMD_GN_LOT_R     0xB2	//Аналогично п.5	Сообщить серийный номер ГН
#define P7E_CMD_GN_LOT_A     0x32	//Аналогично п.6	Ответ на п.11
#define P7E_CMD_VKL_R        0xC1	//1 байт, в 3х младших битах которого  указывается, что нужно включить:
									//01h – ДВ-30
									//02h – ДВ-ПУЛ
									//04h – ДВ-ПУП	Включение ДВ-30, ДВ-ПУЛ, ДВ-ПУП
#define P7E_CMD_VKL_A        0x41	//1 байт, в 3х младших битах которого  указывается статус того, что включено:
									//01h – ДВ-30
									//02h – ДВ-ПУЛ
									//04h – ДВ-ПУП	Ответ на п.13
#define P7E_CMD_ECHO        0xFF	// эхо

/*=========================== АДРЕСА ПРОТОКОЛА 7E ===========================*/
#define P7E_ADDR_VN			6<<3|1
#define P7E_ADDR_GN			6<<3|2





#define P7E_START	0x7E

// структура для интерфейсных функций
typedef struct
{
	unsigned char adr;		// адрес
	unsigned char nb;		// количество байт отправляемых данных в поле DATA
	unsigned char cmd;		// команда
	union
	{
		unsigned char byte[4];	// данные в байтах
		signed short  int16[2]; // данные в 16 битных целых знаковых
	};
}TP7E;



extern TP7E P7E_TxMsg;
extern TP7E P7E_RxMsg;
extern unsigned char P7E_WorkAddr;


/*------------------------------------------------------------------------------
 * отправить сообщение
 *----------------------------------------------------------------------------*/
__forceinline void P7E_PutMsg(TP7E * Msg)
{
	unsigned char packet[16];
	unsigned char i = 0;
	unsigned char id = 0;
	unsigned char cs = P7E_START;


	packet[i++] = P7E_START;		// старт
	packet[i++] = Msg->adr;			// адрес
	cs += Msg->adr;
	packet[i++] = Msg->nb+5;		// количество байт с учетом всех служебных байт
	if((packet[2]) == P7E_START)
		packet[i++] = 0x00;
	cs += packet[2];
	packet[i++] = Msg->cmd;
	cs += Msg->cmd;
	// упаковка данных с учетом стаффинга
	while(id<Msg->nb)
	{
		packet[i++] = Msg->byte[id];
		cs += Msg->byte[id];
		if(Msg->byte[id]==P7E_START)
			packet[i++] = 0x00;
		id++;
	}
	packet[i++] = cs;
	if(cs == P7E_START)
		packet[i++] = 0;
	RS422_PutData(packet,i);
}


/*------------------------------------------------------------------------------
 * получить сообщение
 * 0 - получено сообщение
 * 1 - нет сообщений
 * 2 - идёт прием сообщения
 *----------------------------------------------------------------------------*/
//extern unsigned char P7E_GetMsg(TP7E * Msg);
//unsigned char P7E_GetMsg(TP7E * Msg)


__forceinline unsigned char P7E_GetMsg(TP7E * Msg)
{
	unsigned char data;
	static unsigned char nbyte = 0;
	static unsigned char staff = 0;	// признак проверки стаффинга
	static unsigned char packet_cs = 0; // контрольная сумма пакета
	static unsigned char intern_cs = 0; // контрольная сумма расчетная

	static enum{
		WAIT_START,
		WAIT_ADR,
		WAIT_NB,
		WAIT_CMD,
		WAIT_DATA,
		WAIT_CS
	}P7E_State = WAIT_START;
	 while(!RS422_EMPTY())
	 //if(RS422_EMPTY())
	//	return 1;
	{
	data = RS422_GetChar();

	switch(P7E_State)
	{
	case WAIT_START:
		if(!staff)
		{
			if(data == P7E_START)
			{
				staff = 1;
				return 2;
			}
			else
				return 1;
		}
		else
		{
			staff = 0;
			if(data != 0x00)
			{
				nbyte = 0;
				intern_cs = P7E_START;
				P7E_State = WAIT_ADR;
				// сразу переход на WAIT_ADR return или break не ставить
			}
			else
				return 1;
		}

	case WAIT_ADR:
		if(data != P7E_WorkAddr)
		{
			// сообщение не нам
			//дальше не смысла собирать
			P7E_State = WAIT_START;
			PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //Передатчик RS выключен
			return 1;
		}
		Msg->adr = data;
		intern_cs += data;
		P7E_State = WAIT_NB;
		PORT_SET(PORT_RSTX_DE,BIT_RSTX_DE); //Передатчик RS включен
		return 2;


	case WAIT_NB:
		if(!staff)
		{
			intern_cs += data;
			Msg->nb = data-5;	// вычисляем количество данных в поле data
			if(data == P7E_START)
				staff = 1;
			else
				P7E_State = WAIT_CMD;
			return 2;
		}
		else
		{
			staff = 0;
			if(data == 0x00)
				P7E_State = WAIT_CMD;
			else
			{
				P7E_State = WAIT_START;
				PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //Передатчик RS выключен
				return 1;
			}
		}

	case WAIT_CMD:
		intern_cs += data;
		Msg->cmd = data;
		P7E_State = WAIT_DATA;
		return 2;


	case WAIT_DATA:
		if(nbyte < Msg->nb)
		{
			if(!staff)
			{
				intern_cs += data;
				Msg->byte[nbyte] = data;
				if(data == P7E_START)
				{
					staff = 1;
				}
				else
				{
					nbyte++;
				}
				return 2;
			}
			else
			{
				staff = 0;
				if(data == 0x00)
				{
					nbyte++;
					return 2; // продолжаем собирать сообщение
				}
				else
				{
					P7E_State = WAIT_START;	// ошибка стафинга
					PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //Передатчик RS выключен
					return 1;
				}
			}
		}
		else
		{
			P7E_State = WAIT_CS;
		}


	case WAIT_CS:
		if(!staff)
		{
			packet_cs = data;
			if(data == P7E_START)
			{
				staff = 1;
				return 2;
			}
			P7E_State = WAIT_START;
			if(packet_cs == intern_cs)
				return 0; // пакет собран
			else
			 {
				PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //Передатчик RS выключен
				return 1; // ошибка cs
			 }
		}
		else
		{
			staff = 0;
			P7E_State = WAIT_START;
			if(data == 0x00)
			{
				if(packet_cs == intern_cs)
					return 0; // пакет собран
				else
				{
					PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //Передатчик RS выключен
					return 1; // ошибка cs
				}
			}
			else
			{
				PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //Передатчик RS выключен
				return 1;	  // ошибка стафинга
			}
		}
	}
  }
	return 1;
}


#endif /* P7E_H_ */
