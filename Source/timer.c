#include "common1.h"
#include "1986ve9x_gpio.h"
#include "1986ve9x_clk.h"
#include "1986ve9x_tim.h"
#include "1986ve9x_irq.h"
#include "1986ve9x_systick.h"
#include "MDR32F9Qx_wwdg.h"
#include "defports.h"
#include "timer.h"
#include "spi.h"
#include "sin.h"
#include "can.h"
#include "CtrlPwm.h"
//#include "p7e.h"
//зависимости
#include "1986ve9x_adc.h"
#include "adc.h"
#include "uart.h"
#include "wake.h"
//#include "encoder.h"


// для отладки
#include "dac.h"
#include "uart.h"
//#include "exchange.h"

#include "MDR32Fx.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_board.h"
#include "MDR32F9Qx_can.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "position.h"
#include "device.h"
#include "protocol.h"


typedef __packed struct
{
	signed i:15;	// ток
	unsigned ia_ib:1; // ia = 0, ib = 1;
}TSpi;
// переменные
volatile unsigned long SoftTimer;
volatile unsigned long workTime = 0;		// счетчик времени работы блока

volatile unsigned short Count_Contrl;	//Счетчик циклов без команды управления
											//Включается после подачи команды "Управление"
											//Обнуляется с приходом команды "Управление"
											//Если команда не приходит в течение 100мс - Авария

volatile unsigned short Count_OFF_VIP= 0;	//Счетчик циклов без сигнала на запуск ВИП

volatile unsigned long StatusFlg= 0;		//Флаги статусной переменной

volatile unsigned short Count_Nastroika= 0;		//счетчик для проверки, настройки блока

volatile signed short OCT=0;
volatile signed short OCC=0; 

//volatile TCTRL			Ctrl;
//volatile char pwm_off = 0;
unsigned long 			i;
int		cnt_can = 0;
//long					intdIq = 0;						// интегратор
long					intdId = 0;						// интегратор
volatile long			CtrlPwm[SIZE_CTRL_PWM];			// переменная управления Pwm
volatile unsigned long* pICHR = PICHR;					// указатель на регистр1 сравнения PWM
volatile unsigned long* pICHR1 = PICHR1;				// указатель на регистр2 сравнения PWM
#define intdIq  CtrlPwm[CPWM_INTIQ]
#define intdId  CtrlPwm[CPWM_INTID]
CAN_RxMsgTypeDef RxMsg_CAN;

//Переменные для CAN
//__IO uint32_t rx_buf = 0;
//__IO uint32_t tx_buf = 1;
//volatile TestStatus TestRx;
//PORT_InitTypeDef PORT_InitStructure;
//PORT_InitTypeDef PORTC_InitStructure;
#define sign_long(x)	(x&0x10000000)



// переменные для осцилографа
unsigned char oscil_en = 0;								// передача данных для осцилографа: 0 - запрет, 1 - разрешение
unsigned char oscil_ind[4];								// индексы данных для передачи осцилографу
TCtrlBuf      ctrlbuf = {0,0,0,0};                      // структура для внешнего управления переменными



// функции

void __irq 			Timer1_IRQHandler (void);
void __irq  		Timer2_IRQHandler (void);
void __irq  		Timer3_IRQHandler (void);

//__forceinline static void 			Get_gamma_speed	   (void);
void 			Get_IA_IB		   (void);
__forceinline static void 			PWM_Calc 		   (void);
__forceinline static unsigned char	Possibility_PWM_ON (void);
__forceinline static void 			PWMs_ON 		   (void);
__forceinline static void 			PWMs_OFF 		   (void);

#ifndef CCR1_EN
__forceinline static void 			PWM_Ctrl		(void);
#else
__forceinline static void 			PWM_Ctrl_CCR1	(void);
#endif

//#ifdef	 CONTROL_ANALOG
//#warning CONTROL_ANALOG
//#else
//#warning CONTROL_DIGITAL
//#endif

//-----------------------------------------------------------------------------------
// Инициализация таймера в режиме ШИМ
//-----------------------------------------------------------------------------------
void TIM_pwmInit(unsigned char ntimer)
{
	TIMER_TypeDef *pTimer = TIMER1;
	unsigned char perClockTim = nTIMER1;
//	IRQn_Type timIRQn = Timer1_IRQn;
	unsigned char timClkn = 24;

	switch(ntimer)
	{
	case 1:
		pTimer = TIMER1;
		perClockTim = nTIMER1;
//		timIRQn = Timer1_IRQn;
		timClkn = 24;
		break;

	case 2:
		pTimer = TIMER2;
		perClockTim = nTIMER2;
//		timIRQn = Timer2_IRQn;
		timClkn = 25;
		break;

	case 3:
		pTimer = TIMER3;
		perClockTim = nTIMER3;
//		timIRQn = Timer3_IRQn;
		timClkn = 26;
		break;

	default:
		return;

	}

	PER_CLOCK |= (unsigned long)1<<perClockTim; 	//Разрешение тактовой частоты таймеров
	TIM_CLOCK |= (unsigned long)1<<timClkn;			//Включение тактовой частоты таймеров
	pTimer->CTRL = 0x0000;

//	Настраиваем работу основного счетчика
	
	pTimer->CNT = 0x00000000;					//Начальное значение счетчика

	pTimer->PSC = 0x0000;						//Предделитель частоты (15:0) CLK = TIMx_CLK/(TIM1_PSC+1)

#if ((PLLCPUOUT*500/FPWM)>65535)
	#warning FPWM is too low
#endif
	pTimer->ARR = PLLCPUOUT*500/FPWM;			//Основание счета (15:0) CNT = [0…ARR]
												// 80 000 kHz / 10 kHz / 2 = 4000 такта
											

//Таблица 279 – Описание битов регистра TIMx_IE 
// №           Функциональное       Расшифровка функционального имени бита, краткое описание  
//             имя бита             назначения и принимаемых значений. 
// 31..17      -                    Зарезервировано 
// 16..13      CCR                  Флаг разрешения прерывания по событию переднего фронта на  
//             CAP1                 выходе CAP1 каналов таймера 
//             EVENT                0 – нет прерывания 
//             IE [3:0]             1 – прерывание разрешено 
//                                  Бит 0 – первый канал 
//                                  Бит 3 – четвертый канал 

// 12…9        CCR                  Флаг разрешения прерывания по событию переднего фронта на  
//             REF                  выходе REF каналов таймера 
//             EVENT                0 – нет прерывания 
//             IE[3:0]              1 – прерывание разрешено 
//                                  Бит 0 – первый канал 
//                                  Бит 3 – четвертый канал 

// 8…5         CCR                  Флаг разрешения прерывания по событию переднего фронта на  
//             CAP                  выходе CAP каналов таймера 
//             EVENT                0 – нет прерывания 
//             IE [3:0]             1 – прерывание разрешено 
//                                  Бит 0 – первый канал 
//                                  Бит 3 – четвертый канал 

// 4           BRK                  Флаг разрешения по триггерированному по PCLK состоянию  
//             EVENT                входа BRK, 
//             IE                   0 – нет прерывания 
//                                  1 – прерывание разрешено 

// 3           ETR                  Флаг разрешения прерывания по заднему фронту на входе ETR 
//             FE                   0 – нет прерывания 
//             EVENT                1 – прерывание разрешено 
//             IE 

// 2           ETR                  Флаг разрешения прерывания по переднему фронту на входе  
//             RE                   ETR 
//             EVENT                0 – нет прерывания 
//             IE                   1 – прерывание разрешено 

// 1           CNT                  Флаг разрешения прерывания по событию совпадения CNT и  
//             ARR                  ARR  
//             EVENT                0 – нет прерывания 
//             IE                   1 – прерывание разрешено 

// 0           CNT                  Флаг разрешения прерывания по событию совпадения CNT и  
//             ZERO                 нуля 
//             EVENT                0 – нет прерывания 
//             IE                   1 – прерывание разрешено 
	pTimer->INTE = 0x00000003;						//Разрешение генерировать прерывание
//	                       						//при совпадении CNT = 0

//	Режим работы каналов - ШИМ
//Таблица 267 – Описание битов регистра TIMx_CHy_CNTRL 
// №           Функциональное       Расшифровка функционального имени бита, краткое описание  
//             имя бита             назначения и принимаемых значений. 
// 31..16      -                    Зарезервировано 
// 15                        CAP    Режим работы канала Захват или ШИМ 
//                         nPWM      1 – канал работает в режиме Захват 
//                                   0 – канал работает в режиме ШИМ 
// 14                         WR    Флаг окончания записи, при задании нового значения регистра  
//                         CMPL     CCR 
//                                   1 – данные не записаны и идет запись 
//                                   0 – новые данные можно записывать 
// 13                     ETREN     Разрешения сброса по выводу ETR 
//                                   0 – запрещен сброс 
//                                   1 – разрешен 
// 12                    BRKEN      Разрешение сброса по выводу BRK 
//                                   0 – запрещен сброс 
//                                   1 – разрешен 
// 11…9               OCCM[2:0]     Формат выработки сигнала REF в режиме ШИМ 
//                                  Если CCR1_EN = 0: 
//                                  000 – всегда 0 
//                                  001 – 1, если CNT = CCR  
//                                  010 – 0, если CNT = CCR 
//                                  011 – переключение REF, если CNT =CCR 
//                                  100 – всегда 0 
//                                  101 – всегда 1 
//                                  110 – 1, если DIR= 0 (счет вверх), CNT<CCR, иначе 0 
//                                            0, если DIR= 1 (счет вниз), CNT<CCR, иначе 1 
//                                  111 – 0, если DIR= 0 (счет вверх), CNT<CCR, иначе 1 
//                                            1, если DIR= 1 (счет вниз), CNT<CCR, иначе 0 
//                                  Если CCR1_EN = 1: 
//                                  000 – всегда 0 
//                                  001 – 1, если CNT = CCR или CNT = CCR1 
//                                  010 – 0, если CNT = CCR или CNT = CCR1 
//                                  011 – переключение REF, если CNT =CCR или CNT =CCR1 
//                                  100 – всегда 0 
//                                  101 – всегда 1 
//                                  110 – 1, если DIR = 0 (счет вверх),  CCR1 < CNT < CCR, иначе 0 
//                                            0, если DIR= 1 (счет вниз), CCR < CNT < CCR1, иначе 1 
//                                  111 – 0, если DIR = 0 (счет вверх), CCR1 < CNT < CCR, иначе 1 
//                                            1, если DIR = 1 (счет вниз), CCR< CNT< CCR1, иначе 0 
// 8                        OCCE      Разрешение работы  ETR 
//                                    0 – запрет ETR 
//                                    1 – разрешение ETR 
// 7…6                CHPSC[1:0]      Предварительный делитель входного канала 
//                                    00 – нет деления 
//                                    01 – /2 
//                                    10 – /4 
//                                    11 – /8 
// 5…4                CHSEL[1:0]      Выбор события по входному каналу 
//                                    00 – положительный фронт  
//                                    01 – отрицательный фронт 
//                                    10 – положительный фронт от других каналов 
//                                            Для первого канала от 2 канала 
//                                            Для второго канала от 3 канала 
//                                            Для третьего канала от 4 канала 
//                                            Для четвертого канала от 1 канала 
//                                    11 – положительный фронт от других каналов 
//                                            Для первого канала от 3 канала 
//                                            Для второго канала от 4 канала 
//                                            Для третьего канала от 1 канала 
//                                            Для четвертого канала от 2 канала 
// 3…0              CHFLTR[3:0]       Сигнал зафиксирован: 
//                                    0000 – в 1 триггере на частоте TIM_CLK 
//                                    0001 – в 2 триггерах на частоте TIM_CLK 
//                                    0010 – в 4 триггерах на частоте TIM_CLK 
//                                    0011 – в 8 триггерах на частоте TIM_CLK 
//                                    0100 – в 6 триггерах на частоте FDTS/2 
//                                    0101 – в 8 триггерах на частоте FDTS/2 
//                                    0110 – в 6 триггерах на частоте FDTS/4 
//                                    0111 – в 8 триггерах на частоте FDTS/4 
//                                    1000 – в 6 триггерах на частоте FDTS/8 
//                                    1001 – в 8 триггерах на частоте FDTS/8 
//                                    1010 – в 5 триггерах на частоте FDTS/16 
//                                    1011 – в 6 триггерах на частоте FDTS/16 
//                                    1100 – в 8 триггерах на частоте FDTS/16 
//                                    1101 – в 5 триггерах на частоте FDTS/32 
//                                    1110 – в 6 триггерах на частоте FDTS/32 
//                                    1111 – в 8 триггерах на частоте FDTS/32 

	pTimer->CH_CTRL[0] = INIT_TIMx_CHx_CTRL;	// переключение REF если CNT = CCR
	pTimer->CH_CTRL[1] = INIT_TIMx_CHx_CTRL;	// переключение REF если CNT = CCR
	pTimer->CH_CTRL[2] = INIT_TIMx_CHx_CTRL;	// переключение REF если CNT = CCR
	pTimer->CH_CTRL[3] = INIT_TIMx_CHx_CTRL;	// переключение REF если CNT = CCR

	
	pTimer->CH_CTRL1[0]= INIT_TIMx_CHx_CTRL1;
	pTimer->CH_CTRL1[1]= INIT_TIMx_CHx_CTRL1;
	pTimer->CH_CTRL1[2]= INIT_TIMx_CHx_CTRL1;
	pTimer->CH_CTRL1[3]= INIT_TIMx_CHx_CTRL1;

#ifndef CCR1_EN
	pTimer->CH_CTRL2[0]= INIT_TIMx_CHx_CTRL2;
	pTimer->CH_CTRL2[1]= INIT_TIMx_CHx_CTRL2;
	pTimer->CH_CTRL2[2]= INIT_TIMx_CHx_CTRL2;
	pTimer->CH_CTRL2[3]= INIT_TIMx_CHx_CTRL2;
#else
	pTimer->CH_CTRL2[0]= INIT_TIMx_CHx_CTRL2_CCR1;
	pTimer->CH_CTRL2[1]= INIT_TIMx_CHx_CTRL2_CCR1;
	pTimer->CH_CTRL2[2]= INIT_TIMx_CHx_CTRL2_CCR1;
	pTimer->CH_CTRL2[3]= INIT_TIMx_CHx_CTRL2_CCR1;
#endif

	// инициализация DTG
	pTimer->CH_DTG[0] = INIT_TIMx_CH_DTG;
	pTimer->CH_DTG[1] = INIT_TIMx_CH_DTG;
	pTimer->CH_DTG[2] = INIT_TIMx_CH_DTG;
	pTimer->CH_DTG[3] = INIT_TIMx_CH_DTG;

	//Разрешение работы таймера.
	pTimer->CTRL |= (unsigned long)(1<<6);		//Счет вверх/вниз по TIM_CLK.
	pTimer->CTRL |= 1;							//Разрешение работы таймера
//	NVIC_EnableIRQ(timIRQn);					// Разрешение прерывания по таймеру

}

//-----------------------------------------------------------------------------------
// Инициализация таймера в режиме прямого счета для периодичной
// обработки энкодера
//-----------------------------------------------------------------------------------
void TIM_Init(unsigned char ntimer)
{
	TIMER_TypeDef *pTimer = TIMER1;
	unsigned char perClockTim = nTIMER1;
	IRQn_Type timIRQn = Timer1_IRQn;
	unsigned char timClkn = 24;

	switch(ntimer)
	{
	case 1:
		pTimer = TIMER1;
		perClockTim = nTIMER1;
		timIRQn = Timer1_IRQn;
		timClkn = 24;
		break;

	case 2:
		pTimer = TIMER2;
		perClockTim = nTIMER2;
		timIRQn = Timer2_IRQn;
		timClkn = 25;
		break;

	case 3:
		pTimer = TIMER3;
		perClockTim = nTIMER3;
		timIRQn = Timer3_IRQn;
		timClkn = 26;
		break;

	default:
		return;

	}

	PER_CLOCK |= (unsigned long)1<<perClockTim; 	//Разрешение тактовой частоты таймеров
	TIM_CLOCK |= (unsigned long)1<<timClkn;			//Включение тактовой частоты таймеров
	pTimer->CTRL = 0x0000;

//	Настраиваем работу основного счетчика

	pTimer->CNT = 0x00000000;					//Начальное значение счетчика

	pTimer->PSC = 0x0000;						//Предделитель частоты (15:0) CLK = TIMx_CLK/(TIM1_PSC+1)

//	pTimer->ARR = 65535;						//Основание счета (15:0) CNT = [0…ARR]
	pTimer->ARR = 3000;// 75 мкс //2400 - 60 мкс;//PLLCPUOUT*500/17/2;			//Основание счета (15:0) CNT = [0…ARR]
												// 80 000 kHz / 30 kHz / 2 = 1333 такта
												// устанавливаем 1330 тк 1333 работает со сбоем

//Таблица 279 – Описание битов регистра TIMx_IE
// №           Функциональное       Расшифровка функционального имени бита, краткое описание
//             имя бита             назначения и принимаемых значений.
// 31..17      -                    Зарезервировано
// 16..13      CCR                  Флаг разрешения прерывания по событию переднего фронта на
//             CAP1                 выходе CAP1 каналов таймера
//             EVENT                0 – нет прерывания
//             IE [3:0]             1 – прерывание разрешено
//                                  Бит 0 – первый канал
//                                  Бит 3 – четвертый канал

// 12…9        CCR                  Флаг разрешения прерывания по событию переднего фронта на
//             REF                  выходе REF каналов таймера
//             EVENT                0 – нет прерывания
//             IE[3:0]              1 – прерывание разрешено
//                                  Бит 0 – первый канал
//                                  Бит 3 – четвертый канал

// 8…5         CCR                  Флаг разрешения прерывания по событию переднего фронта на
//             CAP                  выходе CAP каналов таймера
//             EVENT                0 – нет прерывания
//             IE [3:0]             1 – прерывание разрешено
//                                  Бит 0 – первый канал
//                                  Бит 3 – четвертый канал

// 4           BRK                  Флаг разрешения по триггерированному по PCLK состоянию
//             EVENT                входа BRK,
//             IE                   0 – нет прерывания
//                                  1 – прерывание разрешено

// 3           ETR                  Флаг разрешения прерывания по заднему фронту на входе ETR
//             FE                   0 – нет прерывания
//             EVENT                1 – прерывание разрешено
//             IE

// 2           ETR                  Флаг разрешения прерывания по переднему фронту на входе
//             RE                   ETR
//             EVENT                0 – нет прерывания
//             IE                   1 – прерывание разрешено

// 1           CNT                  Флаг разрешения прерывания по событию совпадения CNT и
//             ARR                  ARR
//             EVENT                0 – нет прерывания
//             IE                   1 – прерывание разрешено

// 0           CNT                  Флаг разрешения прерывания по событию совпадения CNT и
//             ZERO                 нуля
//             EVENT                0 – нет прерывания
//             IE                   1 – прерывание разрешено
	pTimer->INTE = 0x00000001;						//Разрешение генерировать прерывание
//	                       						//при совпадении CNT = 0

	//Разрешение работы таймера.
	pTimer->CTRL |= (unsigned long)(1<<6);
	pTimer->CTRL |= 1;							//Разрешение работы таймера
	NVIC_EnableIRQ(timIRQn);					// Разрешение прерывания по таймеру

}
//------------------------------------------------------------------------------
// инициализация таймера SysTick
//------------------------------------------------------------------------------
void TIM_SysTickInit(void)
{

	SysTick->CTRL = 4;
//16 бит COUNTFLAG
//Возвращает 1, если таймер досчитал до нуля с последнего момента чтения.
//
//2 бит CLKSOURCE
//Указывает источник синхросигнала:
//0=LSI
//1=HCLK
//
//1 бит TCKINT
//Разрешает запрос на прерывание от  системного таймера:
//0=если таймер досчитал до нуля, то прерывание не возникает.
//1=если таймер досчитал до нуля, то возникает запрос на прерывание.
//Программное обеспечение может использовать бит COUNTFLAG, чтобы определить досчитал
//таймер до нуля или нет.
//
//0 бит ENABLE
//Разрешает работу таймера:
//0=таймер запрещён.
//1=таймер разрешён.
//Когда ENABLE установлен в единицу, таймер загружает значение RELOAD из регистра LOAD и
//затем начинает декрементироваться. По достижению значения 0, устанавливается бит
//COUNTFLAG и в зависимости от TCKINT генерируется запрос на прерывание. Затем загружается
//значение RELOAD, и продолжается счёт.



//	SysTick->LOAD =
//Регистр перегружаемого значения LOAD (23...0 биты)
//Регистр LOAD устанавливает стартовое значение, загружаемое в регистр VAL.

}
__forceinline void switchCommand(void)
{
	switch(CANMsgRx.Data.ubyte[0])
	{
		case PRM_CMD_RST:// сброс
			
		break;
		
		case PRM_CMD_ERR:// аварийный код
			
		break;
		
		case PRM_CMD_INF_RD:// чтение данных об изготовлении
			//МКРН.468741.101
			// 1 сообщение
			CANMsgTx.Data.ubyte[0] = PRM_CMD_INF_RD;
			CANMsgTx.Data.ubyte[1] = 0x01;
			CANMsgTx.Data.ubyte[2] = 'М';
			CANMsgTx.Data.ubyte[3] = 'К';
			CANMsgTx.Data.ubyte[4] = 'Р';
			CANMsgTx.Data.ubyte[5] = 'Н';
			CANMsgTx.nbyte = 6;
			CAN_Put(&CANMsgTx);

			// 2 сообщение
			CANMsgTx.Data.ubyte[0] = PRM_CMD_INF_RD;
			CANMsgTx.Data.ubyte[1] = 0x02;
			CANMsgTx.Data.ubyte[2] = '4';
			CANMsgTx.Data.ubyte[3] = '6';
			CANMsgTx.Data.ubyte[4] = '8';
			CANMsgTx.Data.ubyte[5] = '7';
			CANMsgTx.Data.ubyte[6] = '4';
			CANMsgTx.Data.ubyte[7] = '1';		
			CANMsgTx.nbyte = 8;
			CAN_Put(&CANMsgTx);
		
			// 3 сообщение
			CANMsgTx.Data.ubyte[0] = PRM_CMD_INF_RD;
			CANMsgTx.Data.ubyte[1] = 0x03;
			CANMsgTx.Data.ubyte[2] = '1';
			CANMsgTx.Data.ubyte[3] = '0';
			CANMsgTx.Data.ubyte[4] = '1';
			CANMsgTx.Data.ubyte[5] = '-';
			CANMsgTx.Data.ubyte[6] = '0';
			CANMsgTx.Data.ubyte[7] = '0';		
			CANMsgTx.nbyte = 8;
			CAN_Put(&CANMsgTx);

			// 4 сообщение
			CANMsgTx.Data.ubyte[0] = PRM_CMD_INF_RD;
			CANMsgTx.Data.ubyte[1] = 0x04;
			CANMsgTx.Data.ubyte[2] = 'Я';
			CANMsgTx.Data.ubyte[3] = 'Д';
			CANMsgTx.Data.ubyte[4] = '2';
			CANMsgTx.Data.ubyte[5] = '7';
			CANMsgTx.Data.ubyte[6] = '0';
			CANMsgTx.Data.ubyte[7] = '1';		
			CANMsgTx.nbyte = 8;
			CAN_Put(&CANMsgTx);
			
			// 5 сообщение
			CANMsgTx.Data.ubyte[0] = PRM_CMD_INF_RD;
			CANMsgTx.Data.ubyte[1] = 0x05;
			CANMsgTx.Data.ubyte[2] = 29;
			CANMsgTx.Data.ubyte[3] = 7;
			*(unsigned short*)&CANMsgTx.Data.ubyte[4] = 2013;
			CANMsgTx.Data.ubyte[6] = 16;
			CANMsgTx.Data.ubyte[7] = 29;		
			CANMsgTx.nbyte = 8;
			CAN_Put(&CANMsgTx);
		
		break;
		
		case PRM_CMD_UID:// уникальный ID
			
		break;
		
		case PRM_CMD_INF_WR:// запись данных об изготовлении
			
		break;
		
		case PRM_CMD_INF_RST:// Сброс флага запрета  перезаписи данных об изготовлении
			
		break;

	}
	
}



__forceinline void ProcessCAN2(void)
{
//	unsigned char res = 0;
//	res |= CAN_GetBuffer(&CANMsgRx,BUF_CMD_UM_CTRL);
//
	//	if(!res)
//	res |= CAN_GetBuffer(&CANMsgRx,BUF_CMD_UM_CTRL2);
	long iq,vlc;
	if(MDR_CANx->RX & (1<<BUF_CMD_UM_CTRL))
	{
		CAN_GetBuffer(&CANMsgRx,BUF_CMD_UM_CTRL);
		//CtrlPwm[CPWM_CAN_COUNT]++;
		cnt_can++;
		CtrlPwm[CPWM_CAN_STATUS_IN] = CANMsgRx.Data.UM_CTRL3.status;
		if(CtrlPwm[CPWM_CAN_STATUS_IN]&0x04)
		{
			CtrlPwm[CPWM_IQIN_] = CANMsgRx.Data.UM_CTRL3.ctrl/KVLC_IN;
		}
		else
		{
			CtrlPwm[CPWM_IQIN_] = 0;
		}
		// 0-й бит - признак удачной инициализации
		CtrlPwm[CPWM_CAN_STATUS_UM] = CtrlPwm[CPWM_CAN_STATUS_IN]|0x01;	
		// подготовка данных к отправке
		CANMsgTx.nbyte = 0x05;
		iq = CtrlPwm[CPWM_IQ]*KIQ;
		SATURAT(iq,-32767,32767);
		CANMsgTx.Data.UM_INF5.I = iq;	// 800 - 32767
		
		vlc = CtrlPwm[CPWM_VLC]*KVLC_OUT;
		SATURAT(vlc,-32767,32767);
		CANMsgTx.Data.UM_INF5.V = vlc;		// 2978 - 32767
		
		CANMsgTx.Data.UM_INF5.status = CtrlPwm[CPWM_CAN_STATUS_UM];
		Count_Contrl=1000;
		CAN_Put(&CANMsgTx);		
	}
}

/*----------------------------------------------------------------
 * Фильтр скорости
 * К0=Т/(Т+Тф)  0,02	  для частоты 8 Гц
 * К1=ТФ/(Т+Тф) 0,98
 * К1+К2=1
 * Т - период дискрет. 100мкс
 * Тф=1/(2*пи*F)
 *-------------------------------------------------------------*/
__forceinline long filterVLC(void)
{
	static long y = 0;	// y на предыдущей операции
	y = (2*CtrlPwm[CPWM_VLC]+98*y)/100;
	return y;
}
//----------------------------------------------------------------------------------
// Чтение из буфера SPI1 значения угла положения ротора gamma и скорости вращения,
// пока буфер не будет пустой.
// если бит 15 (самый старший)
// "0" - gamma
// "1" - скорость
//----------------------------------------------------------------------------------
__forceinline void Get_gamma_speed(void)
{
 	u16 data;		// данные из SPI буфера
 	s16 temp;		//временная переменная, используются для расширения знакового разряда
	 //считываем данные пока буфер не будет пустой
	while(SPI1_SR_RNE)
	{
	  // считывание из буфера
	  data = SPI1_RD;
	  //CtrlPwm[CPWM_TEMP] = data;
      // сохранение результата
	  if (data&0x8000)
	  {
	   temp = (data&0x7FFF)<<1;
	   CtrlPwm[CPWM_VLC] = -temp;   			//скорость
	   CtrlPwm[CPWM_VLC_FLTR] = filterVLC();		//фильтрованная скорость
	  }
	  else //CtrlPwm[CPWM_ANGL] =	data<<1;	//gamma
	  	   CtrlPwm[CPWM_ANGL] =	(data<<1)+CtrlPwm[CPWM_DANGLE];	//gamma	
	} 
}

/*-------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------*/
/*
__forceinline long filterUB(void)
{
	static long y = 0;	// y на предыдущей операции
	y = (CtrlPwm[CPWM_ADCUB]+9*y)/10;
	return y;
}
*/
//void CheckCAN(void);
__forceinline void CheckCAN(void)
{

	static unsigned long start = 10000;
	
	if((start > 5)&&(start < 10)&&(!Count_Contrl))
	{
	
		
		__disable_irq();
		MDR_BKP->REG_00++;
		RST_CLK_PCLKcmd(RST_CLK_PCLK_WWDG,ENABLE);
		
		MDR_WWDG->CFR = 0x00;
		MDR_WWDG->CR = 0xFF;
		MDR_WWDG->CR = 0xFF;
		WWDG_Enable(0x7F);
		
		while (1);
//		__nop();
	
	}
	
	
	if(start)	start--;
	
	
	if(!Count_Contrl)
	{
		__disable_irq();
		MDR_BKP->REG_01++;
		RST_CLK_PCLKcmd(RST_CLK_PCLK_WWDG,ENABLE);
		
		MDR_WWDG->CFR = 0x00;
		MDR_WWDG->CR = 0xFF;
		MDR_WWDG->CR = 0xFF;
		WWDG_Enable(0x7F);
		
		while (1);
//		__nop();
		
	}		
	/*
	if(!Count_Contrl)
	{
		CtrlPwm[CPWM_DEBUG1]++;
		
//		CAN_Cmd(MDR_CANx, DISABLE);
		PORT_CAN2->DDR |= 1<<CAN2_TX;
		PORT_CAN2->RXTX &= ~(1<<CAN2_TX);
		PORT_CAN2->FUNC &= ~(3<<(CAN2_TX*2));
		for(i = 0; i<500; i++);
		PORT_CAN2->FUNC |= (MODE_WORK_CAN2<<(CAN2_TX*2));
//		CAN_Cmd(MDR_CANx, ENABLE);
		
		
	}	
	*/
	
	/*
	if((!Count_Contrl)&&(!start))
	{
		CtrlPwm[CPWM_DEBUG1]++;
		
		CAN_Cmd(MDR_CANx, DISABLE);
		CAN2_init();
		CAN_Cmd(MDR_CANx, ENABLE);
		
		Count_Contrl = 1000;
		
	}
	*/
	
	
}

unsigned char id_oscil;

__forceinline  void ExtrCtrl(void)
{
    enum
    {
        FRM_NO =  0,          // нет
        FRM_SIN =  1,         // синус
        FRM_MNDR = 2,         // меандр
        FRM_SAW  = 3,         // пила
        FRM_FRM1  = 4         // пауза 5 сек(50000 т), импульс положит 0,36 с(3600 т), отриц 0,33 с(3300 т)
    };
    static unsigned short ang;
    unsigned short delta;
    static unsigned short delay;
    static unsigned char state = 0;
    switch(ctrlbuf.frm)
    {
        case FRM_SIN:

            delta = 6553/ctrlbuf.prd;
            ang += delta;
            CtrlPwm[ctrlbuf.var] = (ctrlbuf.amp * PWM_sin(ang))/32768; 
        break;
        
        case FRM_MNDR:
            
        break;
        
        case FRM_SAW:
            
        break;
        
        case FRM_FRM1:
            switch (state)
            {
            case 0: // ожидание запуска
                state = 1;
                delay = 30000;
            break;
            
            case 1: // пауза 5 сек
                if(delay)
                {
                    delay--;
                }
                else
                {
                    delay = 3600;
                    CtrlPwm[ctrlbuf.var] = ctrlbuf.amp;
                    state = 2;
                }
                break;

            case 2: // положительный импульс
                if(delay)
                {
                    delay--;
                    CtrlPwm[ctrlbuf.var] = ctrlbuf.amp;
                }
                else
                {
                    delay = 3300;
                    CtrlPwm[ctrlbuf.var] = -ctrlbuf.amp;
                    state = 3;
                }                
            break;

            case 3:  // отрицательный импульс
                if(delay)
                {
                    delay--;
                    CtrlPwm[ctrlbuf.var] = -ctrlbuf.amp;
                }
                else
                {
                    CtrlPwm[ctrlbuf.var] = 0;
                    state = 0;
                    ctrlbuf.frm = FRM_NO;
                }                 
                break;
            }
          
        break;        
    }
    
}
//------------------------------------------------------------------------------
// обработка прерывания по таймеру1 каждые 33 мкс.
//------------------------------------------------------------------------------
void __irq  Timer1_IRQHandler(void)
{	
//	static unsigned long CAN_vkl_UM = 0;
//	static unsigned char flag;
//	signed short contrl_dv=0;
	static unsigned long cnt_takt = 20;
	unsigned char data_oscil[9];
	
//	if(flag)
//	{
//		
//		TIMER1->INTF = 0;
//		return;
//	}
	
	
//	flag = 1;
	// прерывание по CNT == 0
	if(TIMER1->INTF&0x01)
	{
		TIMER1->INTF = 0;
		
		
/*		
		data_oscil[7] = data_oscil[6];
		data_oscil[6] = data_oscil[5];
		data_oscil[5] = data_oscil[4];
		data_oscil[4] = data_oscil[3];
		data_oscil[3] = data_oscil[2];
		data_oscil[2] = data_oscil[1];
		data_oscil[1] = data_oscil[0];
		data_oscil[0]++;
		
		if(!cnt_takt)
		{
			WAKE_PutMsg(UART1,5,data_oscil,sizeof(data_oscil));
			cnt_takt = 20;
		}
		
		*/
		
		
		
		
		
		
		
		
		
		// период потока данных для осцилографа - 2 мс
		// если таймер 100 мкс, отправляем каждые 20 тактов
		if(oscil_en)
		{
			cnt_takt--;
			if(!cnt_takt)
			{
				// это тестовое запонение
				CtrlPwm[CPWM_MGN]+=2000;	
				CtrlPwm[CPWM_ANGL]+=3000;	
				CtrlPwm[CPWM_BETTA]-=4000;	
				CtrlPwm[CPWM_IQIN]-=5000;	
				
				
				
				
				
				
				
				
				
				// преобразовывание в двухбайтную переменную и запись в буфер для отправки
				*((signed short*)&data_oscil[0]) = CtrlPwm[oscil_ind[0]];
				*((signed short*)&data_oscil[2]) = CtrlPwm[oscil_ind[1]];
				*((signed short*)&data_oscil[4]) = CtrlPwm[oscil_ind[2]];
				*((signed short*)&data_oscil[6]) = CtrlPwm[oscil_ind[3]];
				data_oscil[8] = id_oscil++;
				WAKE_PutMsg(UART1,APC_OSC_DATA,data_oscil,sizeof(data_oscil));
				cnt_takt = 20;
			}	
		}		
    
        ExtrCtrl();
        




















		
/*		
		PORT_SET(GPIOC,10);
			
		
		PORT_SET(PORT_EXT_INT1,BIT_EXT_INT1);
		Get_IA_IB();
		
		//PORT_SET(PORT_dig18,BIT_dig18);
		//Формирование меандра 20 кГц на DIG19	(такая же функция в прерывании по CNT == ARR)
		PORT_FLP(PORT_DIG19,BIT_DIG19);

		
		
		
		
		
		
		
		PORT_SET(GPIOC,11);
		ProcessCAN2();
		CheckCAN();
		PORT_CLR(GPIOC,11);
		
		
		
		
		
		
		
		
		//-----------------------------------------------------------------
		if (CtrlPwm[CPWM_U27] > LEVEL_BS_RELAY) 
		{
			PORT_CLR(PORT_POWER_RELAY,BIT_POWER_RELAY); //Включить сил. реле
		}		
		
		
		
		//-----------------------------------------------------------------
		// проверка наличия команд управления в течении 100 мс
		if(Count_Contrl) 
		{
			Count_Contrl--;
		}
		else 
		{
			CtrlPwm[CPWM_IQIN_] = 0;
		}
		
		
		
		//-----------------------------------------------------------------
		//Проверка условий включения ШИМ	
//		if((CtrlPwm[CPWM_CAN_STATUS_IN]&0x04))//&&(!CAN_vkl_UM)&&(CtrlPwm[CPWM_U27] > LEVEL_BS_PWM)) 	
//		{
//			CAN_vkl_UM = 1;
////			CtrlPwm[CPWM_INTVLC] = 0;
//			CtrlPwm[CPWM_IQIN_] = 0;
//			CtrlPwm[CPWM_INTIQ] = 0;
//			CtrlPwm[CPWM_INTID] = 0;
//			PWMs_ON ();
//		}
//		if(!(CtrlPwm[CPWM_CAN_STATUS_IN]&0x04)) 	
//		else
//		{
//			PWMs_OFF ();
//			CAN_vkl_UM = 0;
//		}
		
		
		
		//-----------------------------------------------------------------
		if(Possibility_PWM_ON())	  
		{		   		  
//			PORT_CLR(PORT_UM_VKL,BIT_UM_VKL); //Сигнал "УМ ВКЛ" - включен//
			PWMs_ON ();			
		}
		else
		{
			//снятие сигнала "УМ ВКЛ.",
//			PORT_SET(PORT_UM_VKL,BIT_UM_VKL); //Сигнал "УМ ВКЛ" - выключен
			PWMs_OFF ();
		}	
		
		
		
		
		
		
		
		//-----------------------------------------------------------------
		//Проверка аппаратной защиты по току
		//Триггер аппаратной защиты по току установлен в "0" (Сработала защита по току) 
		if(ALARM_TRIG_IN)
		{
		  //Выключение ШИМ выходов
		  PWMs_OFF (); // 
		  PORT_CLR(PORT_RESET_TRIG,BIT_RESET_TRIG);	//Cброс триггера аппаратной защиты
		}		
		
		
		
		
		
		

		
		//-----------------------------------------------------------------
		// включение ШИМ для отладки
		//PWMs_ON ();
		//-----------------------------------------------------------------
		
		
		
		
		// Чтение из буфера SPI1 значения угла положения ротора gamma и скорости вращения
		Get_gamma_speed();

		//Снятие высокого уровня с порта запроса
		//на выдачу IA и IB по SPI2
		//PORT_CLR(PORT_EXT_INT1,BIT_EXT_INT1);	 
		// счетчик программного таймера
		if(SoftTimer)	SoftTimer--;
		workTime++;		// счетчик времени работы






		ADC_GO_ON_U27;	 //Запуск АЦП U27



		// проверка бита WR_CMPL
		if(!((TIMER1->CH_CTRL[0]&0x4000)||(TIMER1->CH_CTRL[1]&0x4000)||(TIMER1->CH_CTRL[2]&0x4000))) 
		{
			PWM_Calc();
		}
		POS_OutDac();
		
		PORT_CLR(GPIOC,10);
*/		
		
		CtrlPwm[CPWM_TIMCNT] = TIMER1->CNT;
		
		
		
		
		
	}//конец прерывания по CNT == 0

	// прерывание по ARR == CNT
	if(TIMER1->INTF&0x02)
	{ 
		
		
		
		TIMER1->INTF = 0;
		
		PORT_CLR(PORT_EXT_INT1,BIT_EXT_INT1);
		//Запрос для контроллера обработки фазных токов
		//на выдачу IA и IB по SPI2
		//PORT_SET(PORT_EXT_INT1,BIT_EXT_INT1);
		//Формирование меандра 20 кГц на DIG19	(такая же функция в прерывании по CNT == 0)
		PORT_FLP(PORT_DIG19,BIT_DIG19);
		PORT_SET(PORT_RESET_TRIG,BIT_RESET_TRIG);		  //Снятие сброса с триггера аппаратной защиты
		
		

		
	}
//	TIMER1->INTF = 0;
//	flag = 0;
}

//------------------------------------------------------------------------------
// обработка прерывания по таймеру2 
//------------------------------------------------------------------------------

void __irq  Timer2_IRQHandler (void)
{
	TIMER2->INTF = 0;
}

//------------------------------------------------------------------------------
// обработка прерывания по таймеру3 
//------------------------------------------------------------------------------
void __irq  Timer3_IRQHandler (void)
{
	TIMER3->INTF = 0;
}









//-----------------------------------------------------------------------------------------------------
//Чтение фазных токов (Ia и Ib) из SPI2 буфера.
//Принимаем на SPI2 пакет данных из трех 16-разр. слов
//первое слово
//13...0 биты - 14 мл.разрядов 17 битного Ia
//15,14 биты - индекс слова "00"
//второе слово
//13...0 биты - 14 мл.разрядов 17 битного Ib
//15,14 биты - индекс слова "01"
//третье слово
//2...0 биты - 3 ст.разряда 17 битного Ia
//10...8 биты - 3 ст.разряда 17 битного Ib
//15,14 биты - индекс слова "10"
//-------------------------------------------------------------------------------------------------------
//__forceinline static void 			Get_IA_IB	(void)
//{
//	  
//	  u32 data;		// данные из SPI буфера
//	  static unsigned short word1, word2, word3;  //полученные по SPI 16разр. слова (пакет из трех слов)	  
//	  vs32 ia, ib; //временные переменные фазных токов, используются для расширения знакового разряда
//	  u8  status =0;   
//	 	
//	 while(SPI2_SR_RNE)
//	 {
//	  // считывание из буфера
//	  data = SPI2_RD;
//	  CtrlPwm[CPWM_DEBUG7] = data;
//      // сохранение результата
//	  if (data&0xC000)
//	  {
//	   if (data&0x4000) 
//	   {
//	    word2 = data;  //второе слово пакета
//	   	//CtrlPwm[CPWM_WORD2]=word2;
//		CtrlPwm[CPWM_DEBUG8] = data;
//		status |= 0x02; 
//	   }
//	   else
//	   {
//		word3 = data; //третье слово пакета
//		CtrlPwm[CPWM_DEBUG9] = data;
//		//CtrlPwm[CPWM_WORD3]=word3;
//		if (status&0x03)
//		{	
//		ia = ((0x00000007&word3)<<29) | ((u32)(word1&0x3FFF)<<15);
//		ib = ((0x00000700&word3)<<21) | ((u32)(word2&0x3FFF)<<15);

//		CtrlPwm[CPWM_IA] = (-(ia/32768))/240;
//		CtrlPwm[CPWM_IB] = (-(ib/32768))/240;
//	   	}
//	   }
//	  }
//	  else 
//	  {
//	  word1 = data; //первое слово пакета
//	  //CtrlPwm[CPWM_WORD1]=word1;
//	  status |= 0x01; 
//	  }
//	  
//	  
//	  
//	 }	
//}
//static volatile unsigned short data;

__forceinline  void Get_IA_IB(void)
{
//	register short data;
	TSpi spidata;
//	static volatile short data;
	while(SPI2_SR_RNE) // пока буфер FIFO не пуст
	 {
		 //data = SPI2_RD;
		 *(short*)&spidata = SPI2_RD;
		 //if(data&0x8000)
		 /*
		 if(data&0x0001)
		 {
			 //data = data/2;
			 // ток Ib
			 //CtrlPwm[CPWM_IB] = ((data&0x7FFF)-16384)/KIx;  // 800 дискр - 12 А
			 
			 if(data<0)
			 {
				CtrlPwm[CPWM_IB] = data/(KIx*2)-1;  // 800 дискр - 12 А
			 }
			 else
			 {
				 CtrlPwm[CPWM_IB] = data/(KIx*2);  // 800 дискр - 12 А
			 }
		 }
		 else
		 {
			 // ток Ia
			 CtrlPwm[CPWM_IA] = data/(KIx*2); 
		 }
		 */
		 if(spidata.ia_ib)
		 {
			 CtrlPwm[CPWM_IB] = spidata.i/(KIx*2);
		 }
		 else
		 {
			 CtrlPwm[CPWM_IA] = spidata.i/(KIx*2);
		 }
	 }
}


/*------------------------------------------------------------------------------------
 * ОГРАНИЧЕНИЕ СКОРОСТИ
 * параметры:
 * dg_ext - цифровая измеренная скорость
 * POR - значение тока из характеристики обороты-ток
 *-----------------------------------------------------------------------------------*/
__forceinline long velogr(long dg_ext, long POR)
{
//	#define MAX_VLC	5600		// максимальная скорость в цифре  3050 об/мин
	#define MAX_VLC	2800		// максимальная скорость в цифре  1500 об/мин
//	#define MAX_VLC	3000
//	#define POR		100			// значение тока из характеристики обороты-ток
//	register long temp;
	register long temp;
	temp = abs(dg_ext)-MAX_VLC;
//	CtrlPwm[CPWM_DEBUG1]=temp;
	if (temp<0) return -1;

	temp = (temp<0)?0:temp;
//	CtrlPwm[CPWM_DEBUG2]=temp;
	temp = POR-temp/2;
//	CtrlPwm[CPWM_DEBUG9]=temp;
	temp = (temp<0)?0:temp;
//	CtrlPwm[CPWM_DEBUG10]=temp;
	
	return temp;
	//SATURAT(Iqin,-temp,temp);
	//SATURATE(*Iqin,temp;-temp);
}




//__forceinline long PI_OSS (long * vlcin, long * )



//-----------------------------------------------------------------------------------------
// расчет шим
// вх: 	ia, ib - ток
// 		gamma - угол положения ротора
// 		vlcIn - задание скорости
// 		vlcReal - реальная скорость
// ----------------------------------------------------------------------------------------
__forceinline static void PWM_Calc (void)
{
	long 			ialfa, ibeta;
//	register long	npor,ppor;
//	register short	por;
//	static signed long intVLC = 0;
	#define intVLC	CtrlPwm[CPWM_INTVLC]
	long deltaVLC;
	// перевод угла в электрический

	unsigned short	angle_el;
	angle_el = CtrlPwm[CPWM_ANGL]*NPOL;
	
// 1. Ограничение по скорости нарастания входного воздействия
	if(abs(CtrlPwm[CPWM_IQIN_]-CtrlPwm[CPWM_IQINN])<LIMIT_IN_IQIN)
	{
		CtrlPwm[CPWM_IQINN] = CtrlPwm[CPWM_IQIN_];
	}
	else
	{
		CtrlPwm[CPWM_IQINN] +=	((CtrlPwm[CPWM_IQIN_]-CtrlPwm[CPWM_IQINN])<0? (-LIMIT_IN_IQIN):LIMIT_IN_IQIN);
	}
	
	
 
// --------------- ПИ ОСС ------------------------------------------
	deltaVLC = CtrlPwm[CPWM_IQINN] - CtrlPwm[CPWM_VLC];
	intVLC += deltaVLC;
	SATURAT(intVLC,-LIMINTV,+LIMINTV);
	CtrlPwm[CPWM_IQIN] = deltaVLC*CtrlPwm[CPWM_KV]/100 + 
		intVLC*CtrlPwm[CPWM_KINTV]/10000;
	SATURAT(CtrlPwm[CPWM_IQIN],-POR_IQIN, POR_IQIN);
// -----------------------------------------------------------------

	



// 11. Вычисление преобразования кларка
	ialfa = CtrlPwm[CPWM_IA];
	ibeta = ((2 * CtrlPwm[CPWM_IB] + CtrlPwm[CPWM_IA])*SQRT3INV)/4096 ;
	
//	CtrlPwm[CPWM_IALFA] = ialfa;
//	CtrlPwm[CPWM_IBETTA] = ibeta;
	
// 12. Park transformation (alfa, beta)->(d,q)         *
 	CtrlPwm[CPWM_ID] = (ialfa * PWM_cos(angle_el) + ibeta * PWM_sin(angle_el))/32768;
 	CtrlPwm[CPWM_IQ] = (ibeta * PWM_cos(angle_el) - ialfa * PWM_sin(angle_el))/32768;

// 16. Вычисление ошибки по току Iq
 	CtrlPwm[CPWM_DIQ] = (CtrlPwm[CPWM_IQIN] - CtrlPwm[CPWM_IQ]);
	SATURAT(CtrlPwm[CPWM_DIQ],INTMIN, INTMAX);

// 17. Вычисление ошибки по току Id
 	CtrlPwm[CPWM_DID] =   -CtrlPwm[CPWM_ID];
	SATURAT(CtrlPwm[CPWM_DID],INTMIN, INTMAX);

// 18. Усиление dIq в ПИ звене
	intdIq += CtrlPwm[CPWM_DIQ];
	SATURAT(intdIq,INTMIN, INTMAX);

	//	CtrlPwm[CPWM_UQ] = KPI1 * CtrlPwm[CPWM_DIQ] + intdIq * KPI2;
	CtrlPwm[CPWM_UQ] = CtrlPwm[CPWM_K1]	* CtrlPwm[CPWM_DIQ] + (intdIq * CtrlPwm[CPWM_K2])/32767;
	
	
//	CtrlPwm[CPWM_UQ] = KPI1 * CtrlPwm[CPWM_DIQ] + intdIq * KPI2;
//	CtrlPwm[CPWM_DEBUG4] = (Adc[ADC_UIN]-3250)*6;
//	CtrlPwm[CPWM_UQ] = CtrlPwm[CPWM_DEBUG4];
	SATURAT(CtrlPwm[CPWM_UQ],-32767,32767);//INTMIN, INTMAX);

// 19. Усиление dId в ПИ звене
	intdId += CtrlPwm[CPWM_DID];
	SATURAT(intdId,INTMIN, INTMAX);

	CtrlPwm[CPWM_UD] = CtrlPwm[CPWM_K1] *CtrlPwm[CPWM_DID] + (CtrlPwm[CPWM_K2]*intdId)/32767;
//	CtrlPwm[CPWM_UD] = KPI1 *CtrlPwm[CPWM_DID] + KPI2*intdId;
//	CtrlPwm[CPWM_UD] = 0;
	SATURAT(CtrlPwm[CPWM_UD],-32767,32767);//INTMIN, INTMAX);
//	21.	Вычисление углового смещения вектора напряжения
	CtrlPwm[CPWM_TETTA] = PWM_atan(CtrlPwm[CPWM_UD],CtrlPwm[CPWM_UQ]);
//	22.	Вычисление модуля вектора напряжения
	CtrlPwm[CPWM_MGN] = (
						CtrlPwm[CPWM_UD]*PWM_cos(CtrlPwm[CPWM_TETTA])+
						CtrlPwm[CPWM_UQ]*PWM_sin(CtrlPwm[CPWM_TETTA])
						)/32767;
 
	SATURAT(CtrlPwm[CPWM_MGN],0, 32000);
//	24.	Вычисление углового положения вектора напряжения в координатах статора
	CtrlPwm[CPWM_BETTA] = (unsigned short)(CtrlPwm[CPWM_TETTA] + angle_el);
//	26. Разложение вектора на базовые вектора. Загрузка PWM автомата

 
//	CtrlPwm[CPWM_MGN] = CtrlPwm[CPWM_DEBUG1];
//	CtrlPwm[CPWM_BETTA] = (unsigned short)((CtrlPwm[CPWM_DEBUG2]++));
//	CtrlPwm[CPWM_BETTA] = (unsigned short)(CtrlPwm[CPWM_DEBUG2]);
//	CtrlPwm[CPWM_DEBUG3] = CtrlPwm[CPWM_BETTA]*360/65536;


/* ПРОВЕРКА СИНУСА
	CtrlPwm[CPWM_DEBUG3]++;
	CtrlPwm[CPWM_DEBUG3]&=0xFFFF;
	CtrlPwm[CPWM_DEBUG1] = (PWM_sin(CtrlPwm[CPWM_DEBUG3]))/10;
	CtrlPwm[CPWM_DEBUG2] = (PWM_cos(CtrlPwm[CPWM_DEBUG3]))/10;
	*/
	

	PWM_Ctrl();
	return;
}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Инициализация PWM машины
//------------------------------------------------------------------------------
void PWM_Init(void)
{
	CtrlPwm[CPWM_KPI1] = KPI1;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
__forceinline static void PWM_Ctrl(void)
{
	unsigned char sector;			// сектор в котором находится вектор
	signed long theta1, theta2;	// угол
	signed long	tau1, tau2;		//
	unsigned short	angle = (unsigned short)(CtrlPwm[CPWM_BETTA]+ANGLE_240);


#define FASE0
#ifdef FASE0
#define CH_0	1
#define CH_1	2
#define CH_2	0
#endif

#ifdef FASE1
#define CH_0	1
#define CH_1	0
#define CH_2	2
#endif

#ifdef FASE2
#define CH_0	0
#define CH_1	2
#define CH_2	1
#endif

#ifdef FASE3
#define CH_0	0
#define CH_1	1
#define CH_2	2
#endif

#ifdef FASE4
#define CH_0	2
#define CH_1	1
#define CH_2	0
#endif

#ifdef FASE5
#define CH_0	2
#define CH_1	0
#define CH_2	1
#endif

	register unsigned long cmpr1;
	register unsigned long cmpr2;
	register unsigned long cmpr3;
	// определение сектора
	sector = /*CtrlPwm[CPWM_BETTA]*/angle / (ANGLE_60+1) + 1;
	sector = (workMode == INIT? 7: sector);

	// нахождение углов между базовыми векторами
	theta1 = ANGLE_60 * sector - angle/*CtrlPwm[CPWM_BETTA]*/ ;
	theta2 = /*CtrlPwm[CPWM_BETTA]*/angle - (sector - 1) * ANGLE_60;

//расчет скважностей в относительных единицах (функция от угла)	
	tau1 = (CtrlPwm[CPWM_MGN] * PWM_sin(theta1)) / 32767; 
    tau2 = (CtrlPwm[CPWM_MGN] * PWM_sin(theta2)) / 32767; 

//пересчет в коды для загрузки CMPR


	tau1 = (tau1*TPWM/32767)/4;
	tau2 = (tau2*TPWM/32767)/4;

        switch (sector)
        {

	 	case 1 :
	   		cmpr1 = (TPWM/4 - tau1 - tau2) ;
	  		cmpr2 = (TPWM/4 + tau1 - tau2) ;
	  		cmpr3 = (TPWM/4 + tau1 + tau2) ; break ;
		case 2 :
			cmpr1 = (TPWM/4 - tau1 + tau2) ;
			cmpr2 = (TPWM/4 - tau1 - tau2) ;
			cmpr3 = (TPWM/4 + tau1 + tau2) ; break ;
		case 3 :
			cmpr1= (TPWM/4 + tau1 + tau2) ;
			cmpr2 = (TPWM/4 - tau1 - tau2) ;
			cmpr3 = (TPWM/4 + tau1 - tau2) ; break ;
		case 4 :
			cmpr1 = (TPWM/4 + tau1 + tau2) ;
			cmpr2 = (TPWM/4 - tau1 + tau2) ;
			cmpr3 = (TPWM/4 - tau1 - tau2) ; break ;
		case 5 :
			cmpr1 = (TPWM/4 + tau1 - tau2) ;
			cmpr2 = (TPWM/4 + tau1 + tau2) ;
			cmpr3 = (TPWM/4 - tau1 - tau2) ; break ;
		case 6 :
			cmpr1 = (TPWM/4 - tau1 - tau2) ;
			cmpr2 = (TPWM/4 + tau1 + tau2) ;
			cmpr3 = (TPWM/4 - tau1 + tau2) ; break ;
		default :
			cmpr1 = (TPWM/4) ;
			cmpr2 = (TPWM/4) ;
			cmpr3 = (TPWM/4) ; break ;
      	}
		pICHR[CH_0] = cmpr1;
		pICHR[CH_1] = cmpr2;
		pICHR[CH_2] = cmpr3;
/*
		CtrlPwm[CPWM_DEBUG8] = cmpr1;
		CtrlPwm[CPWM_DEBUG9] = cmpr2;
		CtrlPwm[CPWM_DEBUG10] = cmpr3;
*/		
// вывод расчитанного значения в цап для отладки
//    	DAC0_WR(pICHR[CH_0],0);
//}
}


//-----------------------------------------------------------------------------
// вычисление синуса угла 0 <= (gamma/65536) <= 360
// результат: -1 <= res*32768 <= 1
// ----------------------------------------------------------------------------
__forceinline short PWM_sin(unsigned short gamma)
{
	

	if(gamma < ANGLE_180)
	{
		if(gamma > ANGLE_90)
			gamma = ANGLE_180 - gamma;
		return sin_tbl[gamma];
	}
	else
	{
		if(gamma < ANGLE_270)
			gamma = gamma - ANGLE_180;
		else
			gamma = ANGLE_360 - gamma;
		return -sin_tbl[gamma];

	}

}


//-----------------------------------------------------------------------------
// вычисление косинуса угла 0 <= (gamma/65536) <= 360
// результат: -1 <= res*32768 <= 1
//-----------------------------------------------------------------------------
__forceinline short PWM_cos(unsigned short gamma)
{
	/*

	if(gamma < ANGLE_90) // 16384
		return sin_tbl[ANGLE_90-gamma];
	else
		if(gamma < ANGLE_180) // 32768
			return -sin_tbl[gamma - ANGLE_90];
		else
			if(gamma < ANGLE_270) // 49152
				return -sin_tbl[ANGLE_270 - gamma];
	return sin_tbl[gamma - ANGLE_270];
	*/
	return 0;
}


//-----------------------------------------------------------------------------
// вычисление угла по проекциям Uq и Ud
// -32767 <= Uq <= 32767
// -32767 <= Ud <= 32767
// результат: 0 <= gamma*65536/360 <= 360
//                   ^Uq
//                   |
//                   |
//            -------+------->Ud
//                   |
//                   |
//-----------------------------------------------------------------------------
__forceinline unsigned short PWM_atan(short Ud, short Uq)
{
	/*
	unsigned long absUd, absUq;
	absUd = abs(Ud);
	absUq = abs(Uq);
	if( absUd > absUq)
	{
		absUq *=8192;
		if(Ud > 0)
			if(Uq >= 0)				// 0...45
				return atan_tbl[absUq/absUd];
			else					// 315...360
				return -atan_tbl[absUq/absUd];
		else
			if(Uq > 0)				// 135...180
				return 0x8000 - atan_tbl[absUq/absUd];
			else					// 180...225
				return 0x8000 + atan_tbl[absUq/absUd];
	}
	else
	{
		absUd *=8192;
		if(Ud > 0)
			if(Uq > 0)				// 45...90
				return 0x4000 - atan_tbl[absUd/absUq];
			else					// 270...315
				return 0xC000 + atan_tbl[absUd/absUq];
		else
			if(Uq > 0)				// 90...135
				return  0x4000 + atan_tbl[absUd/absUq];
			else					// 225...270
				return  0xC000 - atan_tbl[absUd/absUq];
	}
	*/
	return 0;
}

//--------------------------------------------------------------------------------
//Проверка условий на возможность включения управления электродвигателем 
//Возвращает:
//"0" - электродвигатель включать нельзя
//"1" - можно включать электродвигатель
__forceinline static unsigned char 		Possibility_PWM_ON (void)
{
	static unsigned long delayPwmOn = 5000;	// 0.5 секунды задержка вкл ШИМ
	if(delayPwmOn)
	{
		delayPwmOn--;
//		CtrlPwm[CPWM_INTVLC] = 0;
		CtrlPwm[CPWM_IQIN_] = 0;
		CtrlPwm[CPWM_IQINN] = 0;
		CtrlPwm[CPWM_VLC] = 0;
		CtrlPwm[CPWM_INTIQ] = 0;
		CtrlPwm[CPWM_INTID] = 0;		
		return 0;
	}
	if(!(CtrlPwm[CPWM_CAN_STATUS_IN]&0x04))
	{
		CtrlPwm[CPWM_IQIN_] = 0;
		CtrlPwm[CPWM_IQINN] = 0;
		CtrlPwm[CPWM_VLC] = 0;
		CtrlPwm[CPWM_INTIQ] = 0;
		CtrlPwm[CPWM_INTID] = 0;			
		return 0;
	}
		
	if(CtrlPwm[CPWM_U27] > LEVEL_BS_PWM)
	{
//		CtrlPwm[CPWM_IQIN_] = 0;
//		CtrlPwm[CPWM_INTIQ] = 0;
//		CtrlPwm[CPWM_INTID] = 0;		
		return 1;
	}
//		if(CtrlPwm[CPWM_CAN_STATUS_IN]&0x04)
			
//	CtrlPwm[CPWM_INTVLC] = 0;

	return 0;
/*	
    if
	(	
	  //Установлен флаг "Команда с CAN включить усилитель мощности пришла" 
		(CtrlPwm[CPWM_CAN_STATUS_IN]&(1UL<<2))&&
	 // (StatusFlg & (1<<StatusFlg_VKL_UM))&&		   
	    (CtrlPwm[CPWM_U27] > LEVEL_BS_PWM)	    //Уровень БС в норме
	 // &&(StatusFlg &= (1<<StatusFlg_UM_VKL))	//Включен один из двигателей 
	)   
	return 1;
	else
	return 0;
	*/
}

//--------------------------------------------------------------------------------
//Включение ШИМ выходов
__forceinline static void 			PWMs_ON 		   (void)
{
 //Включить ШИМ
 PWM_ON(TIMER1,0);
 PWM_ON(TIMER1,1);
 PWM_ON(TIMER1,2);
 PWM_ON(TIMER1,3);
}

//--------------------------------------------------------------------------------
//Выключение ШИМ выходов
__forceinline static void 			PWMs_OFF 		   (void)
{
 PWM_OFF(TIMER1,0);
 PWM_OFF(TIMER1,1);
 PWM_OFF(TIMER1,2);
 PWM_OFF(TIMER1,3);
}
//---------------------------------------------------------------------------------

///TestStatus CAN_Polling(void)
///{
///  CAN_InitTypeDef  sCAN;
///  CAN_TxMsgTypeDef TxMsg;
//  CAN_RxMsgTypeDef RxMsg;
//  uint32_t i = 0;

  /* Set the HCLK division factor = 1 for CAN1*/
///  CAN_BRGInit(MDR_CAN1,CAN_HCLKdiv1);

  /* CAN register init */
///  CAN_DeInit(MDR_CAN1);

  /* CAN cell init */
///  CAN_StructInit (&sCAN);

//  sCAN.CAN_ROP  = ENABLE;
//  sCAN.CAN_SAP  = ENABLE;
//  sCAN.CAN_STM  = ENABLE;
///  sCAN.CAN_ROM  = DISABLE;
///  sCAN.CAN_PSEG = CAN_PSEG_Mul_2TQ;
///  sCAN.CAN_SEG1 = CAN_SEG1_Mul_5TQ;
///  sCAN.CAN_SEG2 = CAN_SEG2_Mul_5TQ;
///  sCAN.CAN_SJW  = CAN_SJW_Mul_4TQ;
///  sCAN.CAN_SB   = CAN_SB_1_SAMPLE;
///  sCAN.CAN_BRP  = 4;

///  CAN_Init (MDR_CAN1,&sCAN);

///  CAN_Cmd(MDR_CAN1, ENABLE);

  /* Disable all CAN1 interrupt */
///  CAN_ITConfig( MDR_CAN1, CAN_IT_GLBINTEN | CAN_IT_RXINTEN | CAN_IT_TXINTEN |
///                CAN_IT_ERRINTEN | CAN_IT_ERROVERINTEN, DISABLE);

  /* Enable CAN1 interrupt from receive buffer */
///  CAN_RxITConfig( MDR_CAN1 ,(1<<rx_buf), ENABLE);
  /* Enable CAN1 interrupt from transmit buffer */
///  CAN_TxITConfig( MDR_CAN1 ,(1<<tx_buf), ENABLE);

  /* receive buffer enable */
///  CAN_Receive(MDR_CAN1, rx_buf, ENABLE);

  /* transmit */
///  TxMsg.IDE     = CAN_ID_STD;
///  TxMsg.DLC     = 0x02;
///  TxMsg.PRIOR_0 = DISABLE;
///  TxMsg.ID      = 0x11111111;
///  TxMsg.Data[1] = 0x22222222;
///  TxMsg.Data[0] = 0x44444444;

///  CAN_Transmit(MDR_CAN1, tx_buf, &TxMsg);
/*
  i = 0;
  while(((CAN_GetStatus(MDR_CAN1) & CAN_STATUS_TX_READY) != RESET) && (i != 0xFFF))
  {
    i++;
  }
  CAN_ITClearRxTxPendingBit(MDR_CAN1, tx_buf, CAN_STATUS_TX_READY);

  i = 0;
  while(((CAN_GetStatus(MDR_CAN1) & CAN_STATUS_RX_READY) == RESET) && (i != 0xFFF))
  {
    i++;
  }
*/
  /* receive */

/*
  CAN_GetRawReceivedData(MDR_CAN1, rx_buf, &RxMsg);

  CAN_ITClearRxTxPendingBit(MDR_CAN1, rx_buf, CAN_STATUS_RX_READY);
*/
//  CAN_Cmd(MDR_CAN1, DISABLE);		  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
  if(RxMsg.Rx_Header.IDE != TxMsg.IDE)
  {
    return FAILED;
  }

  if(RxMsg.Rx_Header.DLC != TxMsg.DLC)
  {
    return FAILED;
  }

  if(RxMsg.Rx_Header.ID != TxMsg.ID)
  {
    return FAILED;
  }

  if(RxMsg.Data[1] != TxMsg.Data[1])
  {
    return FAILED;
  }

  if(RxMsg.Data[0] != TxMsg.Data[0])
  {
    return FAILED;
  }

  else
  {
    return PASSED;  // Test Passed
  }
*/
///return PASSED; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
///}
//-----------------------------------------------------------------------------------------




