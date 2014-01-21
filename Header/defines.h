#ifndef DEFINES_H
#define DEFINES_H


// Частота платы 
#define BOARD_MGZ 80
  
// для таймерного прерывания
#define SYS_TIMER
#ifdef SYS_TIMER
  // Частота основного таймера
//  #define TIMER_HZ 200	
  #define TIMER_HZ 10000	
  #define TIMER_MKS 100	
#endif

//#define CYCLETIME_SEC 0.0001//Для классов р-фильтров 


// Варианты режимов 
#define RS_MODE_422_DUPLEX      1
#define RS_MODE_422_HALFDUPLEX  2
#define RS_MODE_485_HALFDUPLEX  4

//#define UART_1
#ifdef UART_1
  // режим работы
  #define UART_1_MODE RS_MODE_422_DUPLEX
  // скорость сервисного UART-а (UART_1)
  #define UART_1_BAUD 921600
  // размер буфера для UART_1
  #define UART_1_BUFF_LEN 100
  // Порт
  #define UART_1_PORT_ADDR 0x400A8000
  // Ножка
  #define UART_1_RX_PORT_PIN 0
  // адрес устройства в сети RS422/485
  #define UART_1_PROT_7E_ADDR 0x02
#endif


#define UART_2
#ifdef UART_2
  // режим работы
  #define UART_2_MODE RS_MODE_485_HALFDUPLEX//RS_MODE_422_DUPLEX
  // скорость сервисного UART-а (UART_2)
  #define UART_2_BAUD 921600
  // размер буфера для UART_2
  #define UART_2_BUFF_LEN 100
  // Порт
  #define UART_2_PORT_ADDR MDR_PORTA_BASE//0x400A8000 //Порт вывода DE передатчика
  // Ножка
  #define UART_2_RX_PORT_PIN 0  // вывод порта DE передатчика
  // адрес устройства в сети RS422/485
  #define UART_2_PROT_7E_ADDR 0x20  
#endif


#define WATCHDOG


// здесь анализируем все, что сидит на DMA
//#if (defined UART_1)  || (defined UART_2)
//  #define _DMA_
//#endif

// здесь настройки для DMA
#ifdef DMA
  #define DMA_MAX_CHANNEL 9
  #define DMA_PRIMARY_CONTROL_DATA_ADDRESS 0x20005000
#endif

// здесь настройки для DAC
//#define DAC_CH0
#ifdef DAC_CH0
  #define DAC_CH0_MIN_VAL -20
  #define DAC_CH0_MAX_VAL  20
#endif

//#define DAC_CH1
#ifdef DAC_CH1
  #define DAC_CH1_MIN_VAL -40
  #define DAC_CH1_MAX_VAL  40
#endif


// Варианты работы с CAN
#define CAN_IRQ_MODE       1
#define CAN_REGISTRY_MODE  2

#define CAN_0
//===========================================================================
//          Установка  скорости CAN  и таймингов
//
// SJW - подстройка для SEQ1 и SEQ2. Не должна превышать минимального из них.
//
//=========================================================================== 						 
//=                      Частота_платы_Гц									=
//= BRP = -------------------------------------------------------			=
//=		   Скорость_передачи_Бод * (1 + PROP_SEG + SEG1 + SEG2)				=
//===========================================================================

#ifdef CAN_0
  #define CAN_0_MODE            CAN_IRQ_MODE
  #define CAN_0_IN_BUFFER_CNT	  9
  #define CAN_0_BRP             5
  #define CAN_0_PSEG            4
  #define CAN_0_SEG1      	    7
  #define CAN_0_SEG2		      	4
  #define CAN_0_SJW             3   
  #define CAN_0_SPEED_KHZ    1000
  #define CAN_0_PROT_ADDR    0x20
#endif

#if (defined CAN_0)  || (defined CAN_1)
  #define CAN_FIX_0002
  #define CAN_FIX_0010
#endif  

#ifdef CAN_FIX_0010
  #define CAN_FIX_0010_DELAY 2
  #define CAN_0_TX_PORT_ADDRESS 0x400B8000
  #define CAN_0_TX_PORT_PIN 8
  #define CAN_0_TX_PIN_MODE 1
#endif


// область памяти
//#define FLASH_MEM_ADR 0x0801f000
#define FLASH_MEM_ADR 0x0801e000

//Глобальные маски
#define SET_P0   0x00000001
#define SET_P1   0x00000002
#define SET_P2   0x00000004
#define SET_P3   0x00000008
#define SET_P4   0x00000010
#define SET_P5   0x00000020
#define SET_P6   0x00000040
#define SET_P7   0x00000080
#define SET_P8   0x00000100
#define SET_P9   0x00000200
#define SET_P10  0x00000400
#define SET_P11  0x00000800
#define SET_P12  0x00001000
#define SET_P13  0x00002000
#define SET_P14  0x00004000
#define SET_P15  0x00008000

#define CLR_P0   0xFFFFFFFE
#define CLR_P1   0xFFFFFFFD
#define CLR_P2   0xFFFFFFFB
#define CLR_P3   0xFFFFFFF7
#define CLR_P4   0xFFFFFFEF
#define CLR_P5   0xFFFFFFDF
#define CLR_P6   0xFFFFFFBF
#define CLR_P7   0xFFFFFF7F
#define CLR_P8   0xFFFFFEFF
#define CLR_P9   0xFFFFFDFF
#define CLR_P10  0xFFFFFBFF
#define CLR_P11  0xFFFFF7FF
#define CLR_P12  0xFFFFEFFF
#define CLR_P13  0xFFFFDFFF
#define CLR_P14  0xFFFFBFFF
#define CLR_P15  0xFFFF7FFF


#endif //DEFINES_H
