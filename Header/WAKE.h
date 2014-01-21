#ifndef __WAKE_H__
	#define __WAKE_H__
	
// Определения макросов
	#define WAKE_TIMEOUT	1500	// мс - таймаут получения ответа
	#define WAKE_CRC_INIT 	0xDE 	// Начальное значение контрольной суммы

// Значения управляющих байтов
	#define FEND  			0xC0    // Frame END
	#define FESC  			0xDB    // Frame ESCape
	#define TFEND 			0xDC    // Transposed Frame END
	#define TFESC 			0xDD    // Transposed Frame ESCape

// Признаки сообщений
	#define SIGN_DATA  		0x00	// валидный пакет
	#define ERR_STAFF 		0x01	// ошибка стаффинга
	#define ERR_SIGN		0x02	// ошибка формирования признака
	#define ERR_LEN 		0x03	// ошибка длины сообщения
	#define ERR_CRC			0x04	// ошибка контрольной суммы пакета

// Состояния процесса приема сообщения
	enum
	{
		WAIT_FEND,		//ожидание приема FEND
		WAIT_ADDR,		//ожидание приема адреса назначения
		WAIT_SIGN,		//ожидание приема признака сообщения
		WAIT_LEN,		//ожидание приема количества байт в пакете
		WAIT_DATA,		//прием данных
	};
	
// Состояние процесса передачи сообщения	
	enum
	{
		SEND_FEND,		//передача начала сообщения
		SEND_ADDR,		//передача адреса назначения
		SEND_SIGN,		//передача признака сообщения
		SEND_LEN,		//передача количества байт в пакете
		SEND_DATA,		//передача данных
		SEND_CRC,		//передача CRC
	};
	
// ***** Типы данных протокола *****
// Структура процесса приема сообщения
	typedef struct
	{
		unsigned char	state;	// текущее состояние процесса
		unsigned char	stuff;	// состояние стафинга(подмены управляющих символов)
		//unsigned short	nbt;	// количество полученных байт
		unsigned char	nbt;	// количество полученных байт
		unsigned char	crc;	// контрольная сумма сообщения
	} TWAKE_STATUS;

// Общий заголовок сообщения WAKE при запросе и ответе
	typedef __packed struct
	{
		unsigned char	sign;
		unsigned char	len;
		unsigned char*	Msg;
		unsigned char	crc;
	} TWAKE_PACKET;

// Экспортируемые переменные
	extern unsigned char TxMsg[255];
	extern unsigned char RxMsg[255];
// Экспортируемые функции
	extern void 			WAKE_Init	(unsigned short max_msg_size);
	extern unsigned char 	WAKE_GetMsg (UART_TypeDef * pUART,
										 unsigned char *pCmd,
										 unsigned char* pMsg,
										 unsigned char* pMsgLen);
	extern void 			WAKE_PutMsg	(UART_TypeDef * pUART,
										 unsigned char cmd,
										 unsigned char* pMsg,
										 unsigned char msg_len);



#endif
