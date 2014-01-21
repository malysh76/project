/*
 * protocol.h
 *
 *  Created on: 19.04.2011
 *      Author: 129
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// команды протокола wake
#define	APC_NOP				0x00	//	Нет операции
#define	APC_ERROR			0x01	//	Передача кода ошибки
#define	APC_ECHO			0x02	//	Запрос возврата переданного пакета
#define	APC_INFO			0x03	//	Запрос информации об ЭОК
#define	APC_POS				0x04	//	Позиционирование на заданный угол.
#define	APC_BUSY			0x05	//	Проверка  готовности ЭОК
#define	APC_WRITE			0x06	//	Запись значения углов соответствующих позиций во внутреннюю память ЭОК
#define	APC_READ			0x07	//	Чтение значения углов соответствующих позиций из внутренней памяти ЭОК
#define	APC_VAR_READ		0x08	//	Чтение сервисных переменных системой верхнего уровня
#define	APC_VAR_WRITE		0x0F	//	запись сервисных переменных системой верхнего уровня
#define APC_STORE			0x09	//  сохранения указанных данных из ОЗУ в энергонезависимой памяти
#define APC_RESTORE			0x0A	//  восстановления ука-занных данных из энергонезависимой памяти в ОЗУ.
#define APC_BOOT            0x0B    //  переход в загрузчик
#define APC_FWR_PREP        0x0C    //  подготовка перед загрузкой прошивки
#define APC_FWR_LOAD        0x0D    //  загрузка данных прошивки.
#define APC_FWR_RUN         0x0E    //  запуск основной программы после обновления
#define APC_MODE	        0x10    //  установка режима
#define APC_DEBUG			0x11	//  отладочный режим
#define APC_LOG				0x12	//  передача лога
#define APC_OSC_START		0x14	//  запрос на запуск потока данных для осцилоографа
#define APC_OSC_STOP		0x15	//  запрос на останов потока данных для осцилоографа
#define APC_OSC_DATA		0x16	//  потоковые данные для осцилографа
#define APC_OSC_CTRL		0x17	//  внешнее управление переменной
#define APC_LOT 	        0x18	//  считать/записать заводской номер



// коды устройств
#define DEV_CODE_BOOT		0x0000	// загрузчик
#define DEV_CODE_UM100		0x0100	// усилитель мощности 100
#define DEV_CODE_ENC		0x1000	// угловая плата с энкодером

typedef __packed struct
		{
			unsigned short DevCode;	 //	0-й…1-й байт код устройства
			unsigned short Version;	 //	2-й…3-й байт  - вер-сия прошивки.
			unsigned char  date;	 //	4-й байт число
			unsigned char  month;	 //	5-й байт месяц
			unsigned char  year;	 //	6-й байт год
			unsigned char  State[4]; //	7-й...10-й байты – код со-стояния устройства после инициализа-ции
		}TInfo;



#endif /* PROTOCOL_H_ */
