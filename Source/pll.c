#include "common1.h"
#include "1986ve9x_clk.h"
#include "1986ve9x_tim.h"
#include "1986ve9x_irq.h"
//#include "1986ve9x_gpio.h"
//#include "1986ve9x_i2c.h"
//#include "1986ve9x_ssp.h"
//#include "1986ve9x_uart.h"
//#include "1986ve9x_ext.h"
#include "defports.h"
#include "pll.h"
//------------------------------------------------------------------------------
//		������������� PLL
//------------------------------------------------------------------------------
void PLL_Init(void)
{
	
	RST_CLOCK.rPLL_CONTROL = 0x00;
//������� 82 � �������� ����� �������� PLL_CONTROL 
//�         ������������     ����������� ��������������� ����� ����, ������� ��������  
//           �� ��� ����      ���������� � ����������� ��������. 
// 31..12    -                ��������������� 
// 11...8    PLL              ����������� ��������� ��� CPU PLL: 
//           CPU              PLLCPUo = PLLCPUi x (PLLCPUMUL+1) 
//           MUL[3:0] 
// 7..4      PLL              ����������� ��������� ��� USB PLL: 
//           USB              PLLUSBo = PLLUSBi x (PLLUSBMUL+1) 
//           MUL[3:0] 
// 3         PLL              ��� ����������� PLL 
//           CPU              ��� ����� ������������ ��������� � ������� ������  
//           PLD              ���������� ������ ������ 1 
// 2         PLL              ��� ��������� PLL 
//           CPU               0 � PLL ��������� 
//           ON                1 � PLL �������� 
// 1         PLL               ��� ����������� PLL 
//           USB               ��� ����� ������������ ��������� � ������� ������  
//           RLD               ���������� ������ ������ 1 
// 0         PLL               ��� ��������� PLL 
//           USB               0 � PLL ��������� 
//           ON                1 � PLL �������� 


// ��������� HSE ����������
	RST_CLOCK.rHS_CONTROL = 0x01;
//	������� 84 � �������� ����� �������� HS_CONTROL
//	1	HSE_BYP		��� ���������� HSE ������������
//		0 � ����� �����������
//		1 � ����� �������� ����������
//	0	HSE_ON	��� ���������� HSE ������������
//		0 � ��������
//		1 � �������


// �������� ���������� ����������
	while ( !RST_CLOCK.rCLOCK_STATUS&0x04 ) {}
//	������� 80 � �������� ����� �������� CLOCK_STATUS
//	 �         ������������     ����������� ��������������� ����� ����, ������� ��������
//	           �� ��� ����      ���������� � ����������� ��������.
//	 31..3     -                ���������������
//	 2         HSE_RDY          ���� ������ � ������� ����� ����������� HSE
//	                            0 � ���������� �� ������� ��� �� ��������
//	                            1 � ���������� ������� � ��������
//	 1         PLL_CPU_RDY      ���� ������ � ������� ����� CPU PLL
//	                            0 � PLL �� �������� ��� �� ���������
//	                            1 � PLL �������� � ���������
//	 0         PLL_USB_RDY      ���� ������ � ������� ����� USB PLL
//	                            0 � PLL �� �������� ��� �� ���������
//	                            1 � PLL �������� � ���������

// ����� ����������� ���������
	#if ((HSE<2)||(HSE>16))
		#warning HSE should be in the range 2...16 MHz
		#elif (PLLCPUOUT>80)
				#warning PLLCPUOUT should be no more 80 �Hz
			#elif ((PLLCPUOUT/HSE)>16)
				#warning PLLCPUOUT/HSE should be no more 16
	#endif
	RST_CLOCK.rPLL_CONTROL |= (unsigned long)(PLLCPUOUT/HSE-1)<<8;

// ��������� PLL
	RST_CLOCK.rPLL_CONTROL |= 0x04;

//  �������� ���������� PLL
	while ( !RST_CLOCK.rCLOCK_STATUS&0x02 ) {}

// ����� ��������� � ���������� �������� �������
// CPU_C1 = HSE, CPU_C2 = CPUPLL, CPU_C3 = CPU_C2, HCLK = CPU_C3
	RST_CLOCK.rCPU_CLOCK = CPU_C1(2)|CPU_C2(1)|CPU_C3(0)|HCLK_SEL(1);
}
