#ifndef __CAN_H
#define __CAN_H
#include "MDR32F9Qx_can.h"
#include "defports.h"

#define CAN_BUF_DLC_STD   ((uint32_t)0x00000A00)    /* Standard format */
#define CAN_BUF_DLC_EXT   ((uint32_t)0x00001A00)    /* Extended format */

// MDR_CANx - определяется в defports.h
//******************************************************************************************************************
// ------------ ИНИЦИАЛИЗАЦИЯ ПОРТА ДЛЯ CAN ----------------------------------

//CAN1
//**********************************
#define PORT_CAN1	 GPIOD			 //A	 //B	 //C	  //D	  //E
#define PORT_CAN2	 GPIOF			 //A	 //B	 //C	  //D	  //E
#define CAN1_TX	 13				 	 //6	 //2	 //8	  //13	  //1
#define CAN1_RX	 14				     //7	 //3	 //9	  //14	  //0
#define CAN2_TX	 3				 	 //6	 //2	 //8	  //13	  //1
#define CAN2_RX	 2				     //7	 //3	 //9	  //14	  //0
#define	PERNUM_nPORT_CAN1	 nPORTD	 //A	 //B	 //C	  //D	  //E
#define	PERNUM_nPORT_CAN2	 nPORTF	 //A	 //B	 //C	  //D	  //E
#define	PERNUM_nCAN1	 	 nCAN1	 //		 //		 //		  //	  //
#define	PERNUM_nCAN2	 	 nCAN2	 //		 //		 //		  //	  //
//Режим работы вывода порта			 //		 //		 //		  //	  //
//0 – порт							 //		 //		 //		  //	  //
//1 – основная функция				 //		 //		 //		  //	  //
//2 – альтернативная функция		 //		 //		 //		  //	  //
//3 – переопределенная функция		 //		 //		 //		  //	  //
#define	MODE_WORK_CAN1	 3			 //2	 //3	 //1	  //3	  //3
#define	MODE_WORK_CAN2	 3			 //2	 //3	 //1	  //3	  //3

//**********************************

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// маска используемых аналоговых портов
#define MASK_PORT_CAN1	(((unsigned long)1<<CAN1_TX)|\
						 ((unsigned long)1<<CAN1_RX))
						 
#define MASK_PORT_CAN2	(((unsigned long)1<<CAN2_TX)|\
						 ((unsigned long)1<<CAN2_RX))						 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// включение тактовой частоты
#define PER_CLOCK_CAN1_INIT {PER_CLOCK |= (unsigned long)1<<PERNUM_nPORT_CAN1;\
							PER_CLOCK |= (unsigned long)1<<PERNUM_nCAN1;}

#define PER_CLOCK_CAN2_INIT {PER_CLOCK |= (unsigned long)1<<PERNUM_nPORT_CAN2;\
							PER_CLOCK |= (unsigned long)1<<PERNUM_nCAN2;}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Режим работы вывода порта												 
#define PORT_CAN1_FUNC_INIT {PORT_CAN1->FUNC &= ~(((unsigned long)3<<(CAN1_TX*2))|\
					      						((unsigned long)3<<(CAN1_RX*2)));\
												PORT_CAN1->FUNC |=   ((unsigned long)MODE_WORK_CAN1<<(CAN1_TX*2))|\
						  						((unsigned long)MODE_WORK_CAN1<<(CAN1_RX*2));}                                            

#define PORT_CAN2_FUNC_INIT {PORT_CAN2->FUNC &= ~(((unsigned long)3<<(CAN2_TX*2))|\
					      						((unsigned long)3<<(CAN2_RX*2)));\
												PORT_CAN2->FUNC |=   ((unsigned long)MODE_WORK_CAN2<<(CAN2_TX*2))|\
						  						((unsigned long)MODE_WORK_CAN2<<(CAN2_RX*2));}                                            
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++													


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
// Режим работы контроллера
//0 – аналоговый
//1 – цифровой
#define	PORT_CAN1_DIGEN_INIT PORT_CAN1->DIGEN |= MASK_PORT_CAN1
#define	PORT_CAN2_DIGEN_INIT PORT_CAN2->DIGEN |= MASK_PORT_CAN2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// регистр разрешения подтяжки вывода к GND (100K)
//0 – подтяжка в питание выключена
//1 – подтяжка в питание включена (есть подтяжка ~100К)
#define	PORT_CAN1_RGND_INIT PORT_CAN1->RGND &= ~MASK_PORT_CAN1
#define	PORT_CAN2_RGND_INIT PORT_CAN2->RGND &= ~MASK_PORT_CAN2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// регистр разрешения подтяжки вывода к VCC (100K)
//0 – подтяжка в питание выключена
//1 – подтяжка в питание включена (есть подтяжка ~100К)
#define	PORT_CAN1_RVCC_INIT PORT_CAN1->RVCC &= ~MASK_PORT_CAN1
#define	PORT_CAN2_RVCC_INIT PORT_CAN2->RVCC &= ~MASK_PORT_CAN2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define	PORT_CAN1_DDR_INIT PORT_CAN1->DDR |= MASK_PORT_CAN1
#define	PORT_CAN2_DDR_INIT PORT_CAN2->DDR |= MASK_PORT_CAN2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define	PORT_CAN1_PWR_INIT {PORT_CAN1->PWR &= ~(((unsigned long)3<<(CAN1_TX*2))|\
											  ((unsigned long)3<<(CAN1_RX*2)));\
							PORT_CAN1->PWR |=  (((unsigned long)2<<(CAN1_TX*2))|\
						   					  ((unsigned long)2<<(CAN1_RX*2)));}
#define	PORT_CAN2_PWR_INIT {PORT_CAN2->PWR &= ~(((unsigned long)3<<(CAN2_TX*2))|\
											  ((unsigned long)3<<(CAN2_RX*2)));\
							PORT_CAN2->PWR |=  (((unsigned long)2<<(CAN2_TX*2))|\
						   					  ((unsigned long)2<<(CAN2_RX*2)));}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//*******************************************************************************************************************

typedef struct
{
	unsigned long SID;		// стандартный идентификатор
	unsigned char RTR;		// признак удаленного запроса
	unsigned char nbyte;	// количество байт
	union
	{
		unsigned char  ubyte[8]; 	// беззнаковые байты
		unsigned long  ulong[2]; 	// беззнаковые слова
		__packed struct				// структура информационного поля
		{
			unsigned char status;	// статусный байт
			signed short I;			// значение тока
			signed short V;			// значение скорости
		}UM_INF5;
		__packed struct				// структура команды управления
		{
			unsigned char status;	// статусный байт
			signed short ctrl;		// значение сигнала управления
		}UM_CTRL3;

	}Data;
}TCANMsg;





// НАЗНАЧЕНИЕ РАБОЧИХ БУФЕРОВ CAN

#define BUF_CMD_UM_STOP		1 	//  1.  номер буфера для Адресная команда «Стоп передача»
#define BUF_CMD_UM_STOPALL	2	//  2.  номер буфера для Широковещательная команда «Стоп передача»
#define BUF_CMD_UM_RTR		3	//  3.  номер буфера для Адресная команда «Немедленный ответ»
#define BUF_CMD_UM_RTRALL	4	//  4.  номер буфера для Широковещательная  команда «Немедленный ответ»
#define BUF_CMD_UM_CTRL		5	//  5.  номер буфера для Адресная команда «Управление» (управление электроприводом)
#define BUF_CMD_UM_CYCLE	6	//  6.  номер буфера для Адресная команда «Циклический ответ»(Т0 – в поле DATA, ЦМР = 0,05 мс)
#define BUF_CMD_UM_CYCLEALL	7	//  7.  номер буфера для Широковещательная команда «Циклический ответ»(Т0 – в поле DATA, ЦМР = 0,05 мс)
#define BUF_CMD_UM_ANSW		8	//  8.  номер буфера для «Адресный ответ» на  команды  п.3,4,5,6,7
#define BUF_CMD_UM_SYNCALL	9	//  9.  номер буфера для Широковещательная  команда синхронизации
#define BUF_CMD_UM_PARAM	10	//  10. номер буфера для Адресная параметрическая команда
#define BUF_CMD_UM_PRMALL	11	//  11. номер буфера для Широковещательная параметрическая команда
#define BUF_CMD_UM_PRMANSW	12	//  12. номер буфера для Ответ на параметрическую команду п.9,10

#define BUF_CMD_UM_CTRL2	13	// номер 2-го буфера для приема команды управления 
#define BUF_RX_NOFLTR		30	//  30. номер буфер для приема обычных сообщений
#define BUF_TX_NOFLTR		31	//  31. номер буфер для передачи сообщений



// АДРЕСА УСИЛИТЕЛЕЙ МОЩНОСТИ

#define ADDR_UM0	0x00									// 		усилитель мощности 0
#define ADDR_UM1	0x01		// усилитель ВН							// 		усилитель мощности 1
#define ADDR_UM2	0x02		// усилитель ГН							// 		усилитель мощности 2
#define ADDR_UM3	0x03									// 		усилитель мощности 3
#define ADDR_UM4	0x04									// 		усилитель мощности 4
#define ADDR_UM5	0x05									// 		усилитель мощности 5
#define ADDR_UM6	0x06									// 		усилитель мощности 6
#define ADDR_UM7	0x07									// 		усилитель мощности 7





// КОМАНДЫ ДЛЯ УСИЛИТЕЛЕЙ МОЩНОСТИ

//						DSCR    TYPECMD	  ANSW   ADR_TYPE
#define CMD_UM_STOP		(0x0<<9 | 0<<8 	| 0<<7 | 0x2<<3) 	//  1.  Адресная команда «Стоп передача» 
#define CMD_UM_STOPALL	(0x0<<9 | 0<<8 	| 0<<7 | 0x0<<3)	//  2.  Широковещательная команда «Стоп передача» 
#define CMD_UM_RTR		(0x1<<9 | 0<<8 	| 0<<7 | 0x2<<3)	//  3.  Адресная команда «Немедленный ответ» 
#define CMD_UM_RTRALL	(0x1<<9 | 0<<8 	| 0<<7 | 0x0<<3)	//  4.  Широковещательная  команда «Немедленный ответ» 
#define CMD_UM_CTRL		(0x3<<9 | 0<<8 	| 0<<7 | 0x2<<3)	//  5.  Адресная команда «Управление» (управление электроприводом) 
#define CMD_UM_CYCLE	(0x2<<9 | 0<<8 	| 0<<7 | 0x2<<3)	//  6.  Адресная команда «Циклический ответ»(Т0 – в поле DATA, ЦМР = 0,05 мс)  
#define CMD_UM_CYCLEALL	(0x2<<9 | 0<<8 	| 0<<7 | 0x0<<3)	//  7.  Широковещательная команда «Циклический ответ»(Т0 – в поле DATA, ЦМР = 0,05 мс)
#define CMD_UM_ANSW		(0x0<<9 | 0<<8 	| 1<<7 | 0x2<<3)	//  8.  «Адресный ответ» на  команды  п.3,4,5,6,7 
#define CMD_UM_SYNCALL	(0x0<<9 | 0<<8 	| 0<<7 | 0x0<<3)	//  9.  Широковещательная  команда синхронизации
#define CMD_UM_PARAM	(0x0<<9 | 1<<8 	| 0<<7 | 0x2<<3)	//  10. Адресная параметрическая команда 
#define CMD_UM_PRMALL	(0x0<<9 | 1<<8 	| 0<<7 | 0x0<<3)	//  11. Широковещательная параметрическая команда 
#define CMD_UM_PRMANSW	(0x0<<9 | 1<<8 	| 1<<7 | 0x2<<3)	//  12. Ответ на параметрическую команду п.9,10 





// ФОРМИРОВАНИЕ СТАНДАРТНОГО ИДЕНТИФИКАТОРА

#define SID(adr,cmd)	((adr|cmd)<<18)





// ИДЕНТИФИКАТОРЫ ДЛЯ СООБЩЕНИЙ

#define SID_UM_STOP		 SID(CMD_UM_STOP	,CanAddr)
#define SID_UM_STOPALL	 SID(CMD_UM_STOPALL	,1      )
#define SID_UM_RTR		 SID(CMD_UM_RTR		,CanAddr)
#define SID_UM_RTRALL	 SID(CMD_UM_RTRALL	,1      )
#define SID_UM_CTRL		 SID(CMD_UM_CTRL	,CanAddr)
#define SID_UM_CYCLE	 SID(CMD_UM_CYCLE	,CanAddr)
#define SID_UM_CYCLEALL	 SID(CMD_UM_CYCLEALL,1      )
#define SID_UM_ANSW		 SID(CMD_UM_ANSW	,CanAddr)
#define SID_UM_SYNCALL	 SID(CMD_UM_SYNC	,0      )
#define SID_UM_PARAM	 SID(CMD_UM_PARAM	,CanAddr)
#define SID_UM_PRMALL	 SID(CMD_UM_PRMALL	,1      )
#define SID_UM_PRMANSW	 SID(CMD_UM_PRMANSW	,CanAddr)



// КОМАНДЫ ПЕРЕДАЮЩИЕСЯ В ПАРАМЕТРИЧЕСКИХ СООБЩЕНИЯХ

#define PRM_CMD_RST		0xFF	// сброс
#define PRM_CMD_ERR		0x40	// аварийный код
#define PRM_CMD_INF_RD	0x80	// чтение данных об изготовлении
#define PRM_CMD_UID		0x82	// уникальный ID
#define PRM_CMD_INF_WR	0x84	// запись данных об изготовлении
#define PRM_CMD_INF_RST	0x88	// Сброс флага запрета  перезаписи данных об изготовлении

// ЭКСПОРТ ПЕРЕМЕННЫХ
extern TCANMsg CANMsgTx;		// переменная для передаваемых сообщений
extern TCANMsg CANMsgRx;		// переменная для принимаемых сообщений
extern unsigned long CanAddr;	// рабочий CAN адрес


// ЭКСПОРТ ФУНКЦИЙ
extern void 			CAN_init (void);
extern void 			CAN2_init (void);




// INLINE ФУНКЦИИ
/*-----------------------------------------------------------------------
 * отправка сообщений в CAN со стандартным идентификатором
 * pCANMsg - указатель на отправляемое CAN сообщение
 * 0 - удачная отправка
 * 1 - буфер передатчика занят
 *----------------------------------------------------------------------*/
__forceinline unsigned char CAN_Put(TCANMsg * pCANMsg)
{
//	if(MDR_CANx->BUF_CON[BUF_TX_NOFLTR]&CAN_BUF_CON_TX_REQ)
//		return 1;
//	else

	if(MDR_CANx->TX & ((unsigned long)1<<BUF_TX_NOFLTR) )
	{
		MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DLC = CAN_BUF_DLC_STD;
		// проверка RTR запроса
		if(pCANMsg->RTR)
			MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DLC += CAN_RTR_REMOTE;
		else
			MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DLC += pCANMsg->nbyte;

		MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].ID  = pCANMsg->SID;
		MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DATAL = pCANMsg->Data.ulong[0];
		MDR_CANx->CAN_BUF[BUF_TX_NOFLTR].DATAH = pCANMsg->Data.ulong[1];
//		MDR_CANx->BUF_CON[BUF_TX_NOFLTR] = CAN_BUF_CON_TX_REQ|CAN_BUF_CON_EN;
		MDR_CANx->BUF_CON[BUF_TX_NOFLTR] |= CAN_BUF_CON_TX_REQ;
		return 0;
	}
	return 1;
}


/*-----------------------------------------------------------------------
 * получение принятых сообщений из конкретного буфера
 * pCANMsg - указатель на принимаемое CAN сообщение
 * numbuffer - номер буфера
 * 0 - сообщение принято
 * 1 - нет сообщений
 *----------------------------------------------------------------------*/
__forceinline unsigned char CAN_GetBuffer(TCANMsg * pCANMsg,unsigned char numbuffer)
{
//	if(MDR_CANx->BUF_CON[numbuffer]&CAN_BUF_CON_RX_FULL)
//	{
		pCANMsg->nbyte = MDR_CANx->CAN_BUF[numbuffer].DLC & CAN_DLC_DATA_LENGTH;
		pCANMsg->SID = MDR_CANx->CAN_BUF[numbuffer].ID;
		pCANMsg->Data.ulong[0] = MDR_CANx->CAN_BUF[numbuffer].DATAL;
		pCANMsg->Data.ulong[1] = MDR_CANx->CAN_BUF[numbuffer].DATAH;
//		CAN_ITClearRxTxPendingBit(MDR_CANx, numbuffer, CAN_STATUS_RX_READY);
	  MDR_CANx->BUF_CON[numbuffer] &= ~CAN_BUF_CON_RX_FULL; // сбросили флаг принятого сообщения 
		return 0;
//	}
//	else
//		return 1;
}


/*-----------------------------------------------------------------------
 * получение принятых сообщений из всех доступных буферов
 * pCANMsg - - указатель на принимаемое CAN сообщение
 * i - номер буфера принятого сообщения
 * -1 - нет сообщений
 *----------------------------------------------------------------------*/
__forceinline signed char CAN_Get(TCANMsg * pCANMsg)
{

	signed char i=32;
	if(MDR_CANx->STATUS&CAN_STATUS_RX_READY)
	{
		while(i)
			if(!CAN_GetBuffer(pCANMsg,--i))
			{
				
				return i;
			}
	} 
	return -1;
}




#endif
