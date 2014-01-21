#ifndef UART_H_
#define UART_H_
// зависимости
#include "1986ve9x_uart.h"
#include "defports.h"
/*------------------------------------------------------------------------------
 * ќтправка данных
 *----------------------------------------------------------------------------*/
__forceinline void UART_PutData (unsigned char* Data, unsigned short len)
{
	while(1)
	{
		if (!((MDR_UART1->FR)&0x00000020)) // буфер не зан€т
		{
			MDR_UART1->DR = *Data;
			Data++;
			if(!(--len))
				break;
		}
	}
}

/*------------------------------------------------------------------------------
 * ќтправка данных в RS422
 *----------------------------------------------------------------------------*/
__forceinline void RS422_PutData (unsigned char* Data, unsigned short len)
{
	while(1)
	{
		if (!((MDR_UART1->FR)&0x00000020)) // буфер не зан€т
		{
			

			MDR_UART1->DR = *Data;
			Data++;
			if(!(--len))
			{
				
				break;
			}
			
		}
	}
}



// ќѕ–≈ƒ≈Ћ≈Ќ»я

// ------------ »Ќ»÷»јЋ»«ј÷»я ѕќ–“ј ƒЋя UART1 ----------------------------------
#define PORT_UART1	 GPIOE
// включение тактовой частоты
#define PER_CLOCK_UART1_INIT {PER_CLOCK |= (unsigned long)1<<nPORTE;\
							  PER_CLOCK |= (unsigned long)1<<nUART1;}

//–ежим работы вывода порта
//00 Ц порт
//01 Ц основна€ функци€
//10 Ц альтернативна€ функци€
//11 Ц переопределенна€ функци€
#define PORT_UART1_FUNC_INIT {PORT_UART1->FUNC &= ~((unsigned long)3<<(12*2)|(unsigned long)3<<(13*2));\
							  PORT_UART1->FUNC |=  ((unsigned long)3<<(12*2)|(unsigned long)3<<(13*2));}

// –ежим работы контроллера
//0 Ц аналоговый
//1 Ц цифровой
#define	PORT_UART1_DIGEN_INIT PORT_UART1->DIGEN |=  (unsigned long)3<<12

//–ежим мощности передатчика
//00 Ц зарезервировано (передатчик отключен)
//01 Ц медленный фронт
//10 Ц быстрый фронт
//11 Ц максимально быстрый фронт
#define	PORT_UART1_PWR_INIT	{PORT_UART1->PWR &= ~(unsigned long)0x0F000000;\
							 PORT_UART1->PWR |=  (unsigned long)0x0A000000;}




// ------------ »Ќ»÷»јЋ»«ј÷»я ѕќ–“ј ƒЋя UART2 ----------------------------------
#define PORT_UART2	 GPIOF
// включение тактовой частоты
#define PER_CLOCK_UART2_INIT {PER_CLOCK |= (unsigned long)1<<nPORTF;\
							  PER_CLOCK |= (unsigned long)1<<nUART2;}

// Ќаправление порта	1 Ц выход 0 - вход
// 0 разр€д - RX
// 1 разр€д - TX
#define	PORT_UART2_DDR_INIT	PORT_UART2->DDR &= ~(unsigned short)3;
//							 PORT_UART2->DDR |=  (unsigned short)0;}//2;}

//–ежим работы вывода порта
//00 Ц порт
//01 Ц основна€ функци€
//10 Ц альтернативна€ функци€
//11 Ц переопределенна€ функци€
#define PORT_UART2_FUNC_INIT {PORT_UART2->FUNC &= ~((unsigned long)3<<(0*2)|(unsigned long)3<<(1*2));\
							  PORT_UART2->FUNC |=  ((unsigned long)3<<(0*2)|(unsigned long)3<<(1*2));}

// –ежим работы контроллера
//0 Ц аналоговый
//1 Ц цифровой
#define	PORT_UART2_DIGEN_INIT PORT_UART2->DIGEN |=  (unsigned long)3

//–ежим мощности передатчика
//00 Ц зарезервировано (передатчик отключен)
//01 Ц медленный фронт
//10 Ц быстрый фронт
//11 Ц максимально быстрый фронт
#define	PORT_UART2_PWR_INIT	{PORT_UART2->PWR &= ~(unsigned long)0x0000000F;\
							 PORT_UART2->PWR |=  (unsigned long)0x0000000A;}
//------------------------------------------------------------------------------



// проверка наличи€ данных в FIFO буфере
//#define UART_EMPTY(uart)	((uart->FR) & 0x00000010)
// считывание данных из буфера
//#define UART_GetChar(uart) 	(u8)(uart->DR)

// ѕроверка наличи€ данных в FIFO буфере
#define UART_EMPTY()	((MDR_UART1->FR) & 0x00000010)
// считывание данных из буфера
#define UART_GetChar() 	(unsigned char)(MDR_UART1->DR)



// ѕроверка наличи€ данных в FIFO буфере
#define RS422_EMPTY()	((MDR_UART1->FR) & 0x00000010)
// считывание данных из буфера
#define RS422_GetChar() 	(unsigned char)(MDR_UART1->DR)



//------Ё —ѕќ–“ ‘”Ќ ÷»… --------------------------------------------------------


extern	void 			UART1_Init(unsigned long baud);
extern	void 			UART2_Init(unsigned long baud);
extern	void 			init_RS422		(void);
extern  void 			UART_PutChar (unsigned char Data);
extern	void 			UART_PutData (unsigned char* Data, unsigned short len);

#endif

