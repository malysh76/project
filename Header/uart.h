#ifndef UART_H_
#define UART_H_
// �����������
#include "1986ve9x_uart.h"
#include "defports.h"
/*------------------------------------------------------------------------------
 * �������� ������
 *----------------------------------------------------------------------------*/
__forceinline void UART_PutData (unsigned char* Data, unsigned short len)
{
	while(1)
	{
		if (!((MDR_UART1->FR)&0x00000020)) // ����� �� �����
		{
			MDR_UART1->DR = *Data;
			Data++;
			if(!(--len))
				break;
		}
	}
}

/*------------------------------------------------------------------------------
 * �������� ������ � RS422
 *----------------------------------------------------------------------------*/
__forceinline void RS422_PutData (unsigned char* Data, unsigned short len)
{
	while(1)
	{
		if (!((MDR_UART1->FR)&0x00000020)) // ����� �� �����
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



// �����������

// ------------ ������������� ����� ��� UART1 ----------------------------------
#define PORT_UART1	 GPIOE
// ��������� �������� �������
#define PER_CLOCK_UART1_INIT {PER_CLOCK |= (unsigned long)1<<nPORTE;\
							  PER_CLOCK |= (unsigned long)1<<nUART1;}

//����� ������ ������ �����
//00 � ����
//01 � �������� �������
//10 � �������������� �������
//11 � ���������������� �������
#define PORT_UART1_FUNC_INIT {PORT_UART1->FUNC &= ~((unsigned long)3<<(12*2)|(unsigned long)3<<(13*2));\
							  PORT_UART1->FUNC |=  ((unsigned long)3<<(12*2)|(unsigned long)3<<(13*2));}

// ����� ������ �����������
//0 � ����������
//1 � ��������
#define	PORT_UART1_DIGEN_INIT PORT_UART1->DIGEN |=  (unsigned long)3<<12

//����� �������� �����������
//00 � ��������������� (���������� ��������)
//01 � ��������� �����
//10 � ������� �����
//11 � ����������� ������� �����
#define	PORT_UART1_PWR_INIT	{PORT_UART1->PWR &= ~(unsigned long)0x0F000000;\
							 PORT_UART1->PWR |=  (unsigned long)0x0A000000;}




// ------------ ������������� ����� ��� UART2 ----------------------------------
#define PORT_UART2	 GPIOF
// ��������� �������� �������
#define PER_CLOCK_UART2_INIT {PER_CLOCK |= (unsigned long)1<<nPORTF;\
							  PER_CLOCK |= (unsigned long)1<<nUART2;}

// ����������� �����	1 � ����� 0 - ����
// 0 ������ - RX
// 1 ������ - TX
#define	PORT_UART2_DDR_INIT	PORT_UART2->DDR &= ~(unsigned short)3;
//							 PORT_UART2->DDR |=  (unsigned short)0;}//2;}

//����� ������ ������ �����
//00 � ����
//01 � �������� �������
//10 � �������������� �������
//11 � ���������������� �������
#define PORT_UART2_FUNC_INIT {PORT_UART2->FUNC &= ~((unsigned long)3<<(0*2)|(unsigned long)3<<(1*2));\
							  PORT_UART2->FUNC |=  ((unsigned long)3<<(0*2)|(unsigned long)3<<(1*2));}

// ����� ������ �����������
//0 � ����������
//1 � ��������
#define	PORT_UART2_DIGEN_INIT PORT_UART2->DIGEN |=  (unsigned long)3

//����� �������� �����������
//00 � ��������������� (���������� ��������)
//01 � ��������� �����
//10 � ������� �����
//11 � ����������� ������� �����
#define	PORT_UART2_PWR_INIT	{PORT_UART2->PWR &= ~(unsigned long)0x0000000F;\
							 PORT_UART2->PWR |=  (unsigned long)0x0000000A;}
//------------------------------------------------------------------------------



// �������� ������� ������ � FIFO ������
//#define UART_EMPTY(uart)	((uart->FR) & 0x00000010)
// ���������� ������ �� ������
//#define UART_GetChar(uart) 	(u8)(uart->DR)

// �������� ������� ������ � FIFO ������
#define UART_EMPTY()	((MDR_UART1->FR) & 0x00000010)
// ���������� ������ �� ������
#define UART_GetChar() 	(unsigned char)(MDR_UART1->DR)



// �������� ������� ������ � FIFO ������
#define RS422_EMPTY()	((MDR_UART1->FR) & 0x00000010)
// ���������� ������ �� ������
#define RS422_GetChar() 	(unsigned char)(MDR_UART1->DR)



//------������� ������� --------------------------------------------------------


extern	void 			UART1_Init(unsigned long baud);
extern	void 			UART2_Init(unsigned long baud);
extern	void 			init_RS422		(void);
extern  void 			UART_PutChar (unsigned char Data);
extern	void 			UART_PutData (unsigned char* Data, unsigned short len);

#endif

