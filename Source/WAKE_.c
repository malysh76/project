/************************************************************************************/
/* 								Стек протокола WAKE									*/
/************************************************************************************/
#include "CRC.h"
//#include "UART.h"
#include "usb.h"
#include "WAKE.h"
// для отладки
#include "defports.h"

/*==================================================================================*/
/* Глобальные переменные															*/
/*==================================================================================*/
unsigned char TxMsg[256];
unsigned char RxMsg[256];

/*==================================================================================*/
/* Локальные переменные																*/
/*==================================================================================*/
static unsigned char	my_addr = 0;		// Адрес отправителя
static unsigned short	max_pack_size;		// Максимальный размер пакета для устройства
static TWAKE_STATUS		RcvStatus;		// Состояние процесса приема сообщений
static TWAKE_PACKET		RxPacket;		// Принимаемый пакет данных
static TWAKE_PACKET		TxPacket;		// Передаваемый пакет данных

/*==================================================================================*/
/* Прототипы функций																*/
/*==================================================================================*/
void 					WAKE_Init			(unsigned short max_msg_size);
unsigned char 			WAKE_GetMsg 		(
											 unsigned char *pCmd,
											 unsigned char* pMsg,
											 unsigned char* pMsgLen);
void 					WAKE_PutMsg			(
											 unsigned char cmd,
											 unsigned char* pMsg,
											 unsigned char msg_len);
static void 			WAKE_PutErrMsg		(
											 unsigned char err);
static unsigned char 	WAKE_GetPacket 		(TWAKE_STATUS* pStatus,
											 TWAKE_PACKET* pPacket,
											 unsigned char data);
static void 			WAKE_PutPacket		(
											 TWAKE_PACKET* pPacket);
static unsigned char 	WAKE_PutStuffChar	(unsigned char state,
											 unsigned char data,
											 unsigned char* buf);

/*==================================================================================*/
/* Инициализация протокола															*/
/*	Входные параметры:																*/
/*		addr - адрес устройства в сети, при работе 2-х устройств адрес 0			*/
/*	Выходные параметры:	нет															*/
/*==================================================================================*/
void WAKE_Init (unsigned short max_msg_size)
{
// Локальные переменные
// Программа

	max_pack_size = max_msg_size;	// максимальный размер пакета
}
/*==================================================================================*/
/* Прием сообщения																	*/
/*	Входные параметры:																*/
/*		port - номер порта приема данных											*/
/*		pMsg - указатель на принимаемое сообщение									*/
/*		pMsgLen - указатель на переменную для возврата размера принятого сообщения	*/
/*	Выходные параметры:																*/
/*		0x00 - сообщение принято													*/
/*		0xFF - ожидание приема сообщения											*/
/*==================================================================================*/
unsigned char WAKE_GetMsg ( unsigned char *pCmd, unsigned char* pMsg, unsigned char* pMsgLen)
{
// Локальные переменные
	unsigned char 	data;			// очередной байт данных
// Программа
// Определение указателя на тело сообщения
	RxPacket.Msg = pMsg;
// Обработка буфера
	while (!PORT_EMPTY())
	{
		//WDT_Reset();
	// Чтение данных из буфера порта
		data = PORT_GetChar();

	// Декодирование сообщения
		switch (WAKE_GetPacket(&RcvStatus, &RxPacket, data))
		{
			case 0:
			// Проверка признака сообщения
				if(RxPacket.sign == 0x01)
					WAKE_PutPacket(&TxPacket); // повтор пакета в случае ошибки
				else
				{
					*pMsgLen = RxPacket.len;
					*pCmd 	 = RxPacket.sign;
					return 0;
				}
			break;

			case 1:
			// ошибка стаффинга
			// Отправка сообщения об ошибке пакета
				WAKE_PutErrMsg(ERR_STAFF);
			break;

			case 2:
			// ошибка пакета
			// Отправка сообщения об ошибке пакета
				WAKE_PutErrMsg(ERR_SIGN);
			break;

			case 3:
			// Отправка сообщения об ошибке пакета
				WAKE_PutErrMsg(ERR_LEN);
			break;
			case 4:
				// ошибка crc
			// Отправка сообщения об ошибке пакета
				WAKE_PutErrMsg(ERR_CRC);
			break;

		}
	}
// Нет сообщения
	return 0xFF;
}

/*==================================================================================*/
/* Передача сообщения																*/
/*	Входные параметры:																*/
/*		port - номер порта передачи данных											*/
/*		addr_dest - адрес назначения сообщения при работе в сети					*/
/*		pMsg - указатель на передаваемое сообщение									*/
/*		msg_len - размера передаваемого сообщения									*/
/*	Выходные параметры:	нет															*/
/*==================================================================================*/
void WAKE_PutMsg (unsigned char cmd, unsigned char* pMsg, unsigned char msg_len)
{
// Локальные переменные
// Программа
// Инициализация процесса передачи
	TxPacket.sign = cmd;
	TxPacket.len = msg_len;
	TxPacket.Msg = pMsg;
// Отправка пакета
	WAKE_PutPacket(&TxPacket);
}

/*==================================================================================*/
/* Отправка сообщения о канальной ошибке											*/
/*	Входные параметры:																*/
/*		port - номер порта передачи данных											*/
/*		sign - тип канальной ошибки													*/
/*	Выходные параметры:	нет															*/
/*==================================================================================*/
static void WAKE_PutErrMsg (unsigned char err)
{
// Локальные переменные
// Программа
// Инициализация процесса передачи
	TxPacket.sign = 0x01;
	TxPacket.len = 1;
	TxPacket.Msg = &err;
// Отправка пакета
	WAKE_PutPacket(&TxPacket);
}

/*==================================================================================*/
/* Прием пакета WAKE																*/
/*	Входные параметры:																*/
/*		pStatus - указатель на состояние приема										*/
/*		pPacket - указатель на пакет												*/
/*		data - очередной байт данных												*/
/*	Выходные параметры:																*/
/*		0x00 - сообщение принято													*/
/*		0x01 - ошибка стаффинга														*/
/*		0x02 - ошибка признака пакета												*/
/*		0x03 - ошибка размера пакета												*/
/*		0x04 - ошибка контрольной суммы пакета										*/
/*		0xFF - ожидание приема пакета												*/
/*==================================================================================*/
static unsigned char WAKE_GetPacket (TWAKE_STATUS* pStatus, TWAKE_PACKET* pPacket, unsigned char data)
{
// Локальные переменные
// Программа
// Проверка на стартовый символ
	if (data == FEND)
	{
	// Инициализация приема сообщения
		pStatus->state = WAIT_ADDR;
	// Инициализация процесса приема сообщения
		pStatus->stuff = 0;
		pStatus->nbt = 0;
		pStatus->crc = WAKE_CRC_INIT;

	}
	else if (pStatus->state != WAIT_FEND)
	{

	// Обработка стаффинга
		if (pStatus->stuff)
		{
		// Сброс признака стаффинга
			pStatus->stuff = 0;
		// Разбор байта стаффинга
			switch (data)
			{
				case TFEND:
					data = FEND;
				break;
				case TFESC:
					data = FESC;
				break;
				default:
					pStatus->state = WAIT_FEND;
				return 1;	// ошибка стаффинга
			}
		}
		else if (data == FESC)
		{
		// Установка признака стаффинга
			pStatus->stuff = 1;
		// Нет сообщения
			return 0xFF;
		}
	// Обработка пакета
		switch (pStatus->state)
		{
			case WAIT_ADDR:

			// Проверка наличия адреса в сообщении
				if (data & 0x80)
				{
				// Извлечение адреса из байта данных
					data &= 0x7F;
				// Проверка адреса назначения
					if ((data == 0) || (data == my_addr))
					// Переход к приему признака
						pStatus->state = WAIT_SIGN;
					else
						pStatus->state = WAIT_FEND;
					break;
				}
			case WAIT_SIGN:
			// Проверка признака сообщения на корректность
				if (data & 0x80)
				{
				// Переход к ожиданию следующего сообщения
					pStatus->state = WAIT_FEND;
				// Возвращение ошибки сообщения
					return 2;	// ошибка признака пакета
				}
				else
				{
				// Сохранение признака сообщения
					pPacket->sign = data;
				// Переход к приему количества байт данных в сообщении
					pStatus->state = WAIT_LEN;
				}
			break;
			case WAIT_LEN:

			// Прием размера сообщения
				pPacket->len = data;

				// Проверка валидности размера сообщения
				if (pPacket->len > max_pack_size)
				{
				// Переход к ожиданию следующего сообщения
					pStatus->state = WAIT_FEND;
				// Возвращение ошибки сообщения
					return 3;	// ошибка длины пакета
				}
				else
				{
				// Инициализация счетчика байт данных
					pStatus->nbt = 0;
				// Переход к приему данных
					pStatus->state = WAIT_DATA;
				}

			break;
			case WAIT_DATA:
			// Проверка получения всего сообщения
				if (pStatus->nbt == pPacket->len)
				{
				// Переход к ожиданию следующего сообщения
					pStatus->state = WAIT_FEND;
				// Проверка контрольной суммы сообщения
					if (pStatus->crc != data)
						return 4;	// ошибка контрольной суммы пакета
					else
						return 0;	// сообщение получено
				}
				else
					pPacket->Msg[pStatus->nbt++] = data;
			break;
		}
	}
// Подсчет контрольной суммы
	pStatus->crc = CalcByteCRC8(pStatus->crc, data);
// Нет сообщения
	return 0xFF;
}

/*==================================================================================*/
/* Отправка пакета WAKE 															*/
/*	Входные параметры:																*/
/*		port - номер порта передачи													*/
/*		addr_dest - адрес назначения пакета											*/
/*		pPacket - указатель на пакет для отправки									*/
/*	Выходные параметры:	нет															*/
/*==================================================================================*/
static void WAKE_PutPacket (TWAKE_PACKET* pPacket)
{
// Локальные переменные
	TWAKE_STATUS	TxStatus = {SEND_FEND,0,0,0};	// Состояние процесса передачи
	unsigned char	data = 0;						// текущий байт данных
	unsigned char	tran_sign = 1;					// признак процесса отправки 
//
	unsigned char	tx_cnt = 0;						// количество байт данных в буфере
	unsigned char	TxBuf[96];						// промежуточный буфер не должен 
													// быть больше буфера отправки USB (100 байт)
													
// Программа
// Передача сообщения
	while(tran_sign)
	{
	// Обработка состояний процесса передачи сообщения
		switch(TxStatus.state)
		{
			case SEND_FEND:
			// Передача признака начала сообщения
				data = FEND;
			// Инициализация процесса передачи
				TxStatus.nbt = 0;
				TxStatus.crc = WAKE_CRC_INIT;
			// Переход к передаче адреса
				TxStatus.state = SEND_SIGN;
			break;
			case SEND_SIGN:
			// Передача признака пакета
				data = pPacket->sign & 0x7F;
			// Переход к передаче размера сообщения
				TxStatus.state = SEND_LEN;
			break;
			case SEND_LEN:
			// Передача размера сообщения
				data = pPacket->len;
				// Переход в режим передачи данных
				TxStatus.state = SEND_DATA;
			break;
			case SEND_DATA:
			// Проверка окончания передачи всего сообщения
				if (TxStatus.nbt == pPacket->len)
				{
				// Сброс признака передачи сообщения
					tran_sign = 0;
				// Передача контрольной суммы сообщения
					data = TxStatus.crc;
				}
				else
					data = pPacket->Msg[TxStatus.nbt++];
			break;
			default:
			// сброс признака передачи сообщения
				tran_sign = 0;
		}
	// Передача данных в порт
		tx_cnt += WAKE_PutStuffChar(TxStatus.state, data, &TxBuf[tx_cnt]);
	// Отправка данных в порт
		if (tx_cnt > (sizeof(TxBuf) - 2))
		{
			PORT_PutData(TxBuf, tx_cnt);
		// Сброс счетчика данных
			tx_cnt = 0;
		}	
	// Вычисление истинного значения адреса назначения для CRC
	//	if (TxStatus.state == SEND_SIGN)
	//		data &= 0x7F;
	// Подсчет контрольной суммы сообщения
		TxStatus.crc = CalcByteCRC8(TxStatus.crc,data);
	}
// Отправка оставшихся данных в порт
	if (tx_cnt)
	{
			PORT_PutData(TxBuf, tx_cnt);
	}
}

/*==================================================================================*/
/* Запись байта данных в буфер со стаффингом										*/
/*	Входные параметры:																*/
/*		state - текущее состояние процесса передачи									*/
/*		data - байт данных для отправки												*/
/*		buf - указатель на буфер данных для отправки								*/
/*	Выходные параметры:	количество записанных байт данных							*/
/*==================================================================================*/
static unsigned char WAKE_PutStuffChar (unsigned char state, unsigned char data, unsigned char* buf)
{
// Локальные переменные
	unsigned char	cnt = 0;		// количество записанных байт данных
// Программа
// Проверка состояния отправки
	if (state != SEND_SIGN)
	{
	// Проверка условия стаффинга
		if ((data == FEND) || (data == FESC))
		{
		// Выполнение стаффинга
			buf[cnt++] = FESC;
		// Переназначение байта данных
			switch (data)
			{
				case FEND:
					data = TFEND;
				break;
				case FESC:
					data = TFESC;
				break;
			}
		}
	}
// Запись данных в буфер
	buf[cnt++] = data;
// Возвращение количества записанных байт данных
	return cnt;
}

