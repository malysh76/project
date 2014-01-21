#include "common1.h"
#include "1986ve9x_clk.h"
#include "1986ve9x_gpio.h"
#include "1986ve9x_uart.h"
#include "uart.h"
// зависимости
#include "defports.h"

/*------------------------------------------------------------------------------
 * ИНИЦИАЛИЗАЦИЯ UART1
 *-----------------------------------------------------------------------------*/
void UART1_Init(unsigned long baud)
{
	PER_CLOCK_UART1_INIT;
//	PORT_UART1->DDR &= ~((1<<12)&(1<<13));
//	PORT_UART1_FUNC_INIT;
// 	PORT_UART1_DIGEN_INIT;
//	PORT_UART1_PWR_INIT;
	MDR_PORTB->OE = 0x0020;
	MDR_PORTB->FUNC = 0x00002800;
	MDR_PORTB->ANALOG = 0xFFFF;
	MDR_PORTB->PULL = 0x00000000;
	MDR_PORTB->PD = 0x0000;
	MDR_PORTB->PWR = 0xFFFFFFFF;
	MDR_PORTB->GFEN = 0x0000;	
	
// разрешение тактовой частоты
	UART_CLOCK |= (unsigned long)1<<24;
// делитель тактовой частоты
	UART_CLOCK &= 0xFFFFFF00; // UARTCLK = 1 * PLLCPUOUT = 80 МГц
	UART1->IBRD = PLLCPUOUT*1000000/16/baud;
//=ЦЕЛОЕ((B4-C4)*64+0,5)
	UART1->FBRD = PLLCPUOUT*1000000/16/baud*64 - UART1->IBRD*64 + 0.5;
// 8 бит, разрешение FIFO
	UART1->LCR = 0x3<<5 | 1<<4;
// Буфер заполнен на 1/8
	UART1->IFLS  = 0x00;
	UART1->CR    = 0x00;
// передача разрешена, прием разрешен, шлейф запрещен,
// Разрешение работы приемопередатчика
	UART1->CR   |= (0x06 << 7) | 1;
	// сброс всех флагов прерываний
	UART1->ICR |= 0x07FF;
	// установка маски прерывания по приемнику
	UART1->IMR |= (unsigned long)1<<4;

	// разрешение прерываний
//	NVIC_EnableIRQ(UART1_IRQn);
//	NVIC_SetPriority(UART1_IRQn,PRIO_UART1);
}


/*------------------------------------------------------------------------------
 * ИНИЦИАЛИЗАЦИЯ UART2
 *-----------------------------------------------------------------------------*/
void UART2_Init(unsigned long baud)
{
	PER_CLOCK_UART2_INIT;
	PORT_UART2_DDR_INIT;
	PORT_UART2_FUNC_INIT;
 	PORT_UART2_DIGEN_INIT;
	PORT_UART2_PWR_INIT;
// разрешение тактовой частоты
	UART_CLOCK |= (unsigned long)1<<25;
// делитель тактовой частоты
	UART_CLOCK &= 0xFFFF00FF; // UARTCLK = 1 * PLLCPUOUT = 80 МГц
	UART2->IBRD = PLLCPUOUT*1000000/16/baud;
//=ЦЕЛОЕ((B4-C4)*64+0,5)
	UART2->FBRD = PLLCPUOUT*1000000/16/baud*64 - UART2->IBRD*64 + 0.5;
// 8 бит, разрешение FIFO
	UART2->LCR = 0x3<<5 | 1<<4;
// Буфер заполнен на 1/8
	UART2->IFLS  = 0x00;
	UART2->CR    = 0x00;
// передача ращрешенак, прием разрешен, шлейф запрещен,
// Разрешение работы приемопередатчика
	UART2->CR   |= (0x06 << 7) | 1;
	// сброс всех флагов прерываний
	UART2->ICR |= 0x07FF;
	// установка маски прерывания по приемнику
	UART2->IMR |= (unsigned long)1<<4;

//	NVIC_EnableIRQ(UART2_IRQn);
//	NVIC_SetPriority(UART2_IRQn,PRIO_UART2);
}


/*--------------------------------------------------
 * Инициализация RS422
 *-------------------------------------------------*/
void init_RS422(void)
{
	PER_CLOCK_UART1_INIT;
	PORT_UART1->DDR &= ~((1<<12)&(1<<13));
	PORT_UART1_FUNC_INIT;
 	PORT_UART1_DIGEN_INIT;
	PORT_UART1_PWR_INIT;
// разрешение тактовой частоты
	UART_CLOCK |= (unsigned long)1<<24;
// делитель тактовой частоты
	UART_CLOCK &= 0xFFFFFF00; // UARTCLK = 1 * PLLCPUOUT = 80 МГц
	UART1->IBRD = 5;
//=ЦЕЛОЕ((B4-C4)*64+0,5)
	UART1->FBRD = 27;
// 8 бит, разрешение FIFO
	UART1->LCR = 0x3<<5 | 1<<4;
// Буфер заполнен на 1/8
	UART1->IFLS  = 0x00;
	UART1->CR    = 0x00;
// передача разрешена, прием разрешен, шлейф запрещен,
// Разрешение работы приемопередатчика
	UART1->CR   |= (0x06 << 7) | 1;
	// сброс всех флагов прерываний
	UART1->ICR |= 0x07FF;
	// установка маски прерывания по приемнику
	UART1->IMR |= (unsigned long)1<<4;

	// разрешение прерываний
//	NVIC_EnableIRQ(UART1_IRQn);
//	NVIC_SetPriority(UART1_IRQn,PRIO_UART1);
	// 921600
//	MDR_UART1->IBRD = 5;
//	MDR_UART1->FBRD = 27;


}










/*-----------------------------------------------------------------------------*/
/* Передача байта данных 													   */
/*-----------------------------------------------------------------------------*/
void UART_PutChar (unsigned char Data)
{
	while(1)
	{
		if (!((MDR_UART2->FR)&0x00000020)) // буфер не занят
		{
			MDR_UART2->DR = Data;
			break;
		}
	}
}


/*------------------------------------------------------------------------------
 * Отправка данных
 *----------------------------------------------------------------------------*/
/*
void UART_PutData (UART_TypeDef* PortUART, unsigned char* Data, unsigned short len)
{
	while(1)
	{
		if (!((PortUART->FR)&0x00000020)) // буфер не занят
		{
			PortUART->DR = *Data;
			Data++;
			if(!(--len))
				break;
		}
	}
}
*/
