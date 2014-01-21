/*
 * boot.c
 *
 *  Created on: 18.05.2011
 *      Author: 129
 */
#include "common1.h"
#include "1986ve9x_wdg.h"
#include "1986ve9x_clk.h"
#include "pll.h"
#include "eeprom.h"
#include "uart.h"
#include "wake.h"
#include "boot.h"
#include "protocol.h"
#include "defports.h"
#include "adr.h"
#include "crc.h"

TInfo	Info;

// ����������
// �������������� ������ ��� �������� ����������� ����� ����������
//const unsigned long 	boot_crc __attribute__((at(ADDR_CRC_BOOT)));
// �������������� � ������������� �������� ��������������������
const unsigned long 	sign_reprog __attribute__((at(ADDR_SIGN_REPROG)))= SIGN_REPROG;
	  unsigned long * psign = (unsigned long*)ADDR_SIGN_REPROG;	// ��������� �� ������� ��������������������



// ������ �������
void(*work_entry)(void);


// �������
void boot(void);




// �������� �������
int main(void)
{
	unsigned long adr;
	unsigned long delay;
	adr = *((unsigned long*)ADDR_WORK_ENTRY);
	work_entry = (void (*)(void))adr;
	PLL_Init();
	// ����� 5 ������ ��� �� � ������ ������ jtag ����� ���� ��������
	for(delay = 0; delay<1000;delay++) EEPROM_Sleep(300);
	// ��������� ����������� �����������
	NVIC_SetPriorityGrouping(3); //
//	��������	��������� �����	���� ������	���� ���������	�����	��������
//	b011 		bxxxx.0000 			[7:4] 		None 		16 			1
//	b100 		bxxx.y0000 			[7:5] 		[4] 		8 			2
//	b101 		bxx.yy0000 			[7:6] 		[5:4] 		4 			4
//	b110 		bx.yyy0000 			[7] 		[6:4] 		2 			8
//	b111 		b.yyyy0000 			None 		[7:4] 		1 			16
	UART2_Init(115200);
	WAKE_Init(255);
	EEPROM_Init();
	// �������� �������� ��������������������
	if(*psign == SIGN_REPROG)

		work_entry();
	else
		boot();
	return 0;
}
//------------------------------------------------------------------------------
//	���������
//------------------------------------------------------------------------------
void boot(void)
{

	unsigned char cmd;
	unsigned char RxMsgLen;
	unsigned long adr = ADDR_WORK_ENTRY;
	unsigned long sign;
	unsigned long npage;
	unsigned char npacket=0;
	unsigned char crc;



	// ��������� wake
	while(1)
	{
		if(!WAKE_GetMsg (UART2,&cmd, RxMsg, &RxMsgLen))
		{
			switch(cmd)
			{
			case APC_INFO: //������ ����
				Info.DevCode = DEV_CODE_BOOT;
				Info.Version = VERBOOT;
				WAKE_PutMsg(UART2,APC_INFO,(__packed unsigned char*)&Info,sizeof(TInfo));
				break;

			case APC_FWR_PREP: // �������� ����������� ������� ����������������
				// ���������� ��������� �������
				npage = (*(unsigned long*)RxMsg+SIZE_OF_PAGE-1)/SIZE_OF_PAGE;
				do
				{
					npage--;
					EEPROM_ErasePage(MEM_BAS,npage*SIZE_OF_PAGE+ADDR_WORK_ENTRY);
				}
				while(npage);
				// �������� ����� � crc � ��� ������������ ��������
				EEPROM_WriteData(MEM_BAS,ADDR_LEN_FWR,RxMsg,8);
				TxMsg[0] = 0x00;
				WAKE_PutMsg(UART2,APC_FWR_PREP,TxMsg,1);
				// ���������� ���������� ��� ������
				npacket = 0;
				adr = ADDR_WORK_ENTRY;
				break;
			
	
			case APC_FWR_LOAD:
				if(RxMsg[0] != npacket)
				{
					TxMsg[0] = 1;
				}
				else
				{

					EEPROM_WriteData(MEM_BAS,adr,&RxMsg[1],RxMsgLen-1);
					adr += RxMsgLen-1;
					npacket++;
					TxMsg[0] = 0;
				}
				WAKE_PutMsg(UART2,APC_FWR_LOAD,TxMsg,1);
				break;
	
			case APC_FWR_RUN:
				// �������� ����������� ����� ����������� ��������
				crc = CalcArrayCRC8(0,(unsigned char*)ADDR_WORK_ENTRY,*(unsigned short*)ADDR_LEN_FWR);
				if(crc == *(unsigned char*)ADDR_CRC_FWR)
				{
				// ��������� �������� ��������������������.
					sign = SIGN_REPROG;
					EEPROM_WriteData(MEM_BAS,ADDR_SIGN_REPROG,(unsigned char*)&sign,4);
					TxMsg[0] = 0;	// �������
				}
				else
				{
					TxMsg[0] = 1; 	// ������ ����������� �����
				}
				WAKE_PutMsg(UART2,APC_FWR_RUN,TxMsg,1);
				// ���� �� ������������
				IWDG_INIT();
				while(1);

			case APC_BOOT:	//	������� � ���������
				// ���������� �����
				TxMsg[0] = 0;	// �������
				WAKE_PutMsg(UART2,APC_BOOT,TxMsg,1);
				break;
			}
	
		}
	}
}

