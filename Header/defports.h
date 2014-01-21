#ifndef DEFPORTS_H_
	#define DEFPORTS_H_
// зависимости
#include "1986ve9x_gpio.h"

// Режим работы контроллера
//#define MODE_CONTROL	//настройка, контроль
#define MODE_WORK		//нормальный рабочий

//Режим работы усилителя мощности
//#define CONTROL_ANALOG  //аналоговое управление
//#define CONTROL_DIGITAL //цифровое управление по CAN

//#define FEEDBACK_SPEED_ON  //обратная связь по скорости включена

#define	K_FEEDBACK_SPEED  1	   //коэффициент обратной связи по скорости


// ЛОКАЛЬНЫЕ ОПРЕДЕЛЕНИЯ

// НАСТРОЙКИ ТАКТОВОГО ГЕНЕРАТОРА

// частота кварцевого резонатора в МГц
#define HSE			8

// выходная частота PLLCPUOUT в Мгц
#define PLLCPUOUT	80


// частота ШИМ в кГц
#define FPWM		10

// задержка мертвого времени, в нс ( 1...3180)
#define DTGDELAY	2000

// источник опорного напряжения
//#define ADC_REF_INT
#define ADC_REF_EXT

//-----------------------------------------------
//**********************
//Выбор номера CANа
//#define	CAN_1
#define	CAN_2
//**********************
#ifdef CAN_1
#define MDR_CANx  MDR_CAN1
#define RST_CLK_PCLK_CANx  RST_CLK_PCLK_CAN1

#else
//CAN2
#define MDR_CANx  MDR_CAN2
#define RST_CLK_PCLK_CANx  RST_CLK_PCLK_CAN2
#endif

//Задержка выключения ШИМ и ВИПов после снятия сигнала ВКЛ.ВИП
//#define  ZADERGKA_OFF_VIP1			20000  //Задержка выключения ШИМ
//#define  ZADERGKA_OFF_VIP2			30000  //Задержка выключения ВИП

//Ограничение по входу нарастания Iqin_
#define  LIMIT_IN_IQIN				20	   //Максимальное значение, на которое может изменится
										   //Iqin_ за один цикл таймера

//Уровень напряжения на конденсаторах
//при котором срабатывает силовое реле
#define LEVEL_BS_RELAY			2000

//Уровень напряжения БС
//при котором включают ШИМ
#define LEVEL_BS_PWM			2100 	//20 Вольт



//Задание Iqin_ при аварийном повороте
//#define  ALARM_IQIN_				100	   // Поворот башни влево 18 - 24 град/сек.

#define SWITCH_1_RS				1		   //ДВ-30 (ВН)
#define SWITCH_2_RS				2		   //ДВ-ПУЛ
#define SWITCH_3_RS				4		   //ДВ-ПУП


//------------------------------------------------
// ПОРТЫ ВВОДА-ВЫВОДА

//Входы:
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Аппаратная защита по току с триггера
#define PORT_ALARM_TRIG				GPIOA
#define BIT_ALARM_TRIG				10
#define ALARM_TRIG_IN				PORT_IN(PORT_ALARM_TRIG,BIT_ALARM_TRIG)

/*
//Команда включения УМ "Вкл. Привода ГН"
//Х1/14 +27В
//dig2_in
#define PORT_VKL_UM				GPIOA
#define BIT_VKL_UM				7
#define VKL_UM_IN				PORT_IN(PORT_VKL_UM,BIT_VKL_UM)
*/


//Управление Коммутатором - канал 1
//ДВ-30 (ВН)   активный +27В
//dig1_in
#define PORT_SWITH_1			GPIOA
#define BIT_SWITH_1				6
#define SWITH_1_IN				PORT_IN(PORT_SWITH_1,BIT_SWITH_1)



//Управление Коммутатором - канал 3
//ДВ-ПУП   активный Корпус
//dig4_in
#define PORT_SWITH_3				GPIOA
#define BIT_SWITH_3				11
#define SWITH_3_IN				PORT_IN(PORT_SWITH_3,BIT_SWITH_3)



//Управление Коммутатором - канал 2
//ДВ-ПУЛ   активный Корпус
//dig3_in
#define PORT_SWITH_2				GPIOA
#define BIT_SWITH_2				9
#define SWITH_2_IN				PORT_IN(PORT_SWITH_2,BIT_SWITH_2)


/*
//Корпус "Люки шасси закрыты"
//X5/19 -27В   вкл. ММПБ и управление ЭДГН
//dig7
#define PORT_LYUKI_SCH			GPIOA
#define BIT_LYUKI_SCH			14
#define LYUKI_SCH_IN			PORT_IN(PORT_LYUKI_SCH,BIT_LYUKI_SCH)
*/

/*
//+15В от концевика "РСВП"
//X5/15 +15В   вкл. ММПБ и управление ЭДГН
//dig5
#define PORT_RSVP				GPIOA
#define BIT_RSVP				12
#define RSVP_IN					PORT_IN(PORT_RSVP,BIT_RSVP)
*/

/*
//Команда "Аварийный поворот"
//X5/8 +27В
//dig6
#define PORT_ALARM_ROTATE		GPIOA
#define BIT_ALARM_ROTATE		13
#define ALARM_ROTATE_IN			PORT_IN(PORT_ALARM_ROTATE,BIT_ALARM_ROTATE)
*/

/*
//Корпус "ММПБ Вкл."
//X5/20 -27В  Тормоз. муфта вкл.
//dig8 
#define PORT_BRAKE_MUFF			GPIOA
#define BIT_BRAKE_MUFF			15
#define BRAKE_MUFF_IN			PORT_IN(PORT_BRAKE_MUFF,BIT_BRAKE_MUFF)
*/

/*
//Готовность преобразователя напряжения
//Б.Э.-060 X2/12 +3,3В
//dig20_in
//"0" - преобр. готов 
#define PORT_GOTOV_PREOBR		GPIOD
#define BIT_GOTOV_PREOBR		7
#define GOTOV_PREOBR_IN			PORT_IN(PORT_GOTOV_PREOBR,BIT_GOTOV_PREOBR)
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Выходы:
//--------------------------------------------------------------
//Сброс триггера аппаратной защиты
#define PORT_RESET_TRIG			GPIOC
#define BIT_RESET_TRIG			8

//Запрос на выдачу по SPI IA и IB
#define PORT_EXT_INT1			GPIOF
#define BIT_EXT_INT1			10

//Выключение ШИМ выходов на регистре 1594АП5, "1" - выкл.
#define PORT_REGISTR_PWM_OUT	GPIOA
#define BIT_REGISTR_PWM_OUT		0


//Сигнал на вкл. силовых реле
//dig12_out
//"0" - ВИП включены
#define PORT_POWER_RELAY		GPIOC
#define BIT_POWER_RELAY			9


//Сигнал включения силовой части "УМ.ВКЛ"
//X1/8 +15В
//dig21_out
//"0" - +15В
#define PORT_UM_VKL				GPIOD
#define BIT_UM_VKL				8



//Выход на канал 1 коммутатора
//ДВ-30 (ВН)
//dig13_out
#define PORT_OUT_SWITH1			GPIOC
#define BIT_OUT_SWITH1			10				   

//Выход на канал 2 коммутатора
//ДВ-ПУЛ 
//dig14_out
#define PORT_OUT_SWITH2			GPIOC
#define BIT_OUT_SWITH2			11			

//Выход на канал 3 коммутатора
//ДВ-ПУП
//dig15_out
#define PORT_OUT_SWITH3			GPIOC
#define BIT_OUT_SWITH3			12	

//Меандр 20 кГц
//Выход с регистра 1594АП5 +5В
//dig19
#define PORT_DIG19				GPIOF
#define BIT_DIG19				9

//Сигнал включения передатчика
//dig10
//"0" - выходы передатчика в высокоимпедансном состоянии
#define PORT_RSTX_DE			GPIOC
#define BIT_RSTX_DE				5

#define PORT_dig18				GPIOC
#define BIT_dig18				15

//--------------------------------------------------------------

//Спец.функции
//ШИМ выходы
#define PORT_PWM_OUT			GPIOA
#define BIT_PWM_OUT1			1
#define BIT_PWM_OUT2			2
#define BIT_PWM_OUT3			3
#define BIT_PWM_OUT4			4
#define BIT_PWM_OUT5			5
#define BIT_PWM_OUT6			8





//----------------------------------------------------------------
//Флаги статусной переменной StatusFlg
#define StatusFlg_CNTRL         0  //Устан.в "1" когда приходит первая команда "Управление"
#define StatusFlg_VIP_OFF       1  //Отсутствие сигнала вкл. ВИП или авар. поворот более ZADERGKA_OFF_VIP1
#define StatusFlg_VKL_UM        2  //Команда с CAN "Включить усилитель мощности" "1" - ВКЛ 
#define StatusFlg_RS_GET          3  //Принято сообщение по RS-422
#define StatusFlg_UM_VKL          4  //Один из двигателей включен

// ПОРТЫ ВВОДА-ВЫВОДА
/*
//------------------------------------------------------------------------------
// ИНИЦИАЛИЗАЦИЯ ПОРТА ОПРЕДЕЛЕНИЯ ТИПА ПЛАТЫ
//------------------------------------------------------------------------------
#define PORT_TYPEBOARD	 		GPIOF		// порт
#define DIGIT_TYPEBOARD 		2			// разряд

// чтение состояния порта
#define TYPEBOARD				PORT_IN(PORT_TYPEBOARD,DIGIT_TYPEBOARD)
*/

/*
//----------------------------------------------------------------------------------------------------
// УПРАВЛЕНИЕ ТАКТИРОВАНИЕМ ИНТЕГРАТОРОВ
//----------------------------------------------------------------------------------------------------
#define PORT_INTEGR				GPIOD
#define BIT_INTEGR1				11
#define BIT_INTEGR2				12

#define INTGR1_FLP				PORT_FLP(PORT_INTEGR,BIT_INTEGR1)
#define INTGR2_FLP				PORT_FLP(PORT_INTEGR,BIT_INTEGR2)

*/

//------------------------------------------------------------------------------
//	ОБРАБОТКА ЛИНИЙ УПРАВЛЕНИЯ CTRL, RDY, ADD, CHARGE
//------------------------------------------------------------------------------
/*
#define PORT_POSITION			GPIOB
#define BIT_POS_0				11
#define BIT_POS_1				12
#define BIT_POS_2				13
#define BIT_POS_3				14
#define BIT_POS_4				15


#define PORT_CTRL				GPIOD
#define BIT_CTRL				10

#define PORT_RDY				GPIOD
#define BIT_RDY					9

#define PORT_ADD				GPIOF
#define BIT_ADD					7

#define PORT_CHARGE				GPIOF
#define BIT_CHARGE				3

#define PORT_DRVERR				GPIOD
#define BIT_DRVERR				13

#define PORT_BRKERR				GPIOD
#define BIT_BRKERR				14


#define CTRL_IN					PORT_IN(PORT_CTRL,BIT_CTRL)
#define DRVERR_IN				PORT_IN(PORT_DRVERR,BIT_DRVERR)
#define BRKERR_IN				PORT_IN(PORT_BRKERR,BIT_BRKERR)

#define POS_IN0					PORT_IN(PORT_POSITION,BIT_POS_0)
#define POS_IN1					PORT_IN(PORT_POSITION,BIT_POS_1)
#define POS_IN2					PORT_IN(PORT_POSITION,BIT_POS_2)
#define POS_IN3					PORT_IN(PORT_POSITION,BIT_POS_3)
#define POS_IN4					PORT_IN(PORT_POSITION,BIT_POS_4)
#define POS_IN					~((PORT_POSITION->RXTX)>>11)&0x1F

#define RDY_SET					PORT_SET(PORT_RDY,BIT_RDY)
#define RDY_CLR					PORT_CLR(PORT_RDY,BIT_RDY)
#define RDY_FLP					PORT_FLP(PORT_RDY,BIT_RDY)
#define RDY_IN					PORT_IN(PORT_RDY,BIT_RDY)


#define ADD_SET					PORT_SET(PORT_ADD,BIT_ADD)
#define ADD_CLR					PORT_CLR(PORT_ADD,BIT_ADD)
#define ADD_FLP					PORT_FLP(PORT_ADD,BIT_ADD)

#define CHARGE_SET				PORT_SET(PORT_CHARGE,BIT_CHARGE)
#define CHARGE_CLR				PORT_CLR(PORT_CHARGE,BIT_CHARGE)
#define CHARGE_FLP				PORT_FLP(PORT_CHARGE,BIT_CHARGE)

//-----------------------------------------------------------------------------
// НАЗНАЧЕНИЕ ПОРТОВ ДЛЯ ПАРАЛЛЕЛЬНОГО ИНТЕРФЕЙСА
//-----------------------------------------------------------------------------
#define PORT_EX_WORD_LO  GPIOA		// младшее слово данных
#define PORT_EX_WORD_ME  GPIOC		// среднее слово данных
#define PORT_EX_BYTE_HI  GPIOE		// старший байт данных
#define PORT_EX_CTRL	 GPIOE		// регистр линий управления

#define BIT_MODE		 13			// бит линии режима 0 - прошивка 1 - энкодер
#define BIT_BUSY		 14			// бит линии занятости 0 - занят, 1 - свободен
#define BIT_EXT		 	 15			// бит запроса данных

*/
#endif
