/*
 * p7e.h
 *
 *  Created on: 28.06.2013
 *      Author: 129
 */

#ifndef P7E_H_
#define P7E_H_
#include "MDR32Fx.h"
#include "uart.h"
#include "defports.h"


/*=========================== ������� ��������� 7E ===========================*/
		//�������	���������	��������
#define P7E_CMD_VN_CTRL_R    0x91	//�����, �������� 16 ���	������� ���� ��
#define P7E_CMD_VN_CTRL_A    0x11	//2 ����� �������� 16���
									//1-� ��� ����������������
									//2-� �������� ����������������	����� �� �.1
#define P7E_CMD_VN_STAT_R    0xA1	//���	�������� ������ ��
#define P7E_CMD_VN_STAT_A    0x21	//1 ����, ����� ��� ����������� �������	����� �� �.3
#define P7E_CMD_VN_LOT_R     0xB1	//���	�������� �������� ����� ��
#define P7E_CMD_VN_LOT_A     0x31	//4 �����, 2 ����� � 2-� ����� ��������� ������ � ��������� ANSI, 2 ����� � ����� (����������� �����) 	����� �� �.5
#define P7E_CMD_GN_CTRL_R    0x92	//���������� �.1	������� ���� ��
#define P7E_CMD_GN_CTRL_A    0x12	//���������� �.2	����� �� �.7
#define P7E_CMD_GN_STAT_R    0xA2	//���������� �.3	�������� ������ ��
#define P7E_CMD_GN_STAT_A    0x22	//���������� �.4	����� �� �.9
#define P7E_CMD_GN_LOT_R     0xB2	//���������� �.5	�������� �������� ����� ��
#define P7E_CMD_GN_LOT_A     0x32	//���������� �.6	����� �� �.11
#define P7E_CMD_VKL_R        0xC1	//1 ����, � 3� ������� ����� ��������  �����������, ��� ����� ��������:
									//01h � ��-30
									//02h � ��-���
									//04h � ��-���	��������� ��-30, ��-���, ��-���
#define P7E_CMD_VKL_A        0x41	//1 ����, � 3� ������� ����� ��������  ����������� ������ ����, ��� ��������:
									//01h � ��-30
									//02h � ��-���
									//04h � ��-���	����� �� �.13
#define P7E_CMD_ECHO        0xFF	// ���

/*=========================== ������ ��������� 7E ===========================*/
#define P7E_ADDR_VN			6<<3|1
#define P7E_ADDR_GN			6<<3|2





#define P7E_START	0x7E

// ��������� ��� ������������ �������
typedef struct
{
	unsigned char adr;		// �����
	unsigned char nb;		// ���������� ���� ������������ ������ � ���� DATA
	unsigned char cmd;		// �������
	union
	{
		unsigned char byte[4];	// ������ � ������
		signed short  int16[2]; // ������ � 16 ������ ����� ��������
	};
}TP7E;



extern TP7E P7E_TxMsg;
extern TP7E P7E_RxMsg;
extern unsigned char P7E_WorkAddr;


/*------------------------------------------------------------------------------
 * ��������� ���������
 *----------------------------------------------------------------------------*/
__forceinline void P7E_PutMsg(TP7E * Msg)
{
	unsigned char packet[16];
	unsigned char i = 0;
	unsigned char id = 0;
	unsigned char cs = P7E_START;


	packet[i++] = P7E_START;		// �����
	packet[i++] = Msg->adr;			// �����
	cs += Msg->adr;
	packet[i++] = Msg->nb+5;		// ���������� ���� � ������ ���� ��������� ����
	if((packet[2]) == P7E_START)
		packet[i++] = 0x00;
	cs += packet[2];
	packet[i++] = Msg->cmd;
	cs += Msg->cmd;
	// �������� ������ � ������ ���������
	while(id<Msg->nb)
	{
		packet[i++] = Msg->byte[id];
		cs += Msg->byte[id];
		if(Msg->byte[id]==P7E_START)
			packet[i++] = 0x00;
		id++;
	}
	packet[i++] = cs;
	if(cs == P7E_START)
		packet[i++] = 0;
	RS422_PutData(packet,i);
}


/*------------------------------------------------------------------------------
 * �������� ���������
 * 0 - �������� ���������
 * 1 - ��� ���������
 * 2 - ��� ����� ���������
 *----------------------------------------------------------------------------*/
//extern unsigned char P7E_GetMsg(TP7E * Msg);
//unsigned char P7E_GetMsg(TP7E * Msg)


__forceinline unsigned char P7E_GetMsg(TP7E * Msg)
{
	unsigned char data;
	static unsigned char nbyte = 0;
	static unsigned char staff = 0;	// ������� �������� ���������
	static unsigned char packet_cs = 0; // ����������� ����� ������
	static unsigned char intern_cs = 0; // ����������� ����� ���������

	static enum{
		WAIT_START,
		WAIT_ADR,
		WAIT_NB,
		WAIT_CMD,
		WAIT_DATA,
		WAIT_CS
	}P7E_State = WAIT_START;
	 while(!RS422_EMPTY())
	 //if(RS422_EMPTY())
	//	return 1;
	{
	data = RS422_GetChar();

	switch(P7E_State)
	{
	case WAIT_START:
		if(!staff)
		{
			if(data == P7E_START)
			{
				staff = 1;
				return 2;
			}
			else
				return 1;
		}
		else
		{
			staff = 0;
			if(data != 0x00)
			{
				nbyte = 0;
				intern_cs = P7E_START;
				P7E_State = WAIT_ADR;
				// ����� ������� �� WAIT_ADR return ��� break �� �������
			}
			else
				return 1;
		}

	case WAIT_ADR:
		if(data != P7E_WorkAddr)
		{
			// ��������� �� ���
			//������ �� ������ ��������
			P7E_State = WAIT_START;
			PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //���������� RS ��������
			return 1;
		}
		Msg->adr = data;
		intern_cs += data;
		P7E_State = WAIT_NB;
		PORT_SET(PORT_RSTX_DE,BIT_RSTX_DE); //���������� RS �������
		return 2;


	case WAIT_NB:
		if(!staff)
		{
			intern_cs += data;
			Msg->nb = data-5;	// ��������� ���������� ������ � ���� data
			if(data == P7E_START)
				staff = 1;
			else
				P7E_State = WAIT_CMD;
			return 2;
		}
		else
		{
			staff = 0;
			if(data == 0x00)
				P7E_State = WAIT_CMD;
			else
			{
				P7E_State = WAIT_START;
				PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //���������� RS ��������
				return 1;
			}
		}

	case WAIT_CMD:
		intern_cs += data;
		Msg->cmd = data;
		P7E_State = WAIT_DATA;
		return 2;


	case WAIT_DATA:
		if(nbyte < Msg->nb)
		{
			if(!staff)
			{
				intern_cs += data;
				Msg->byte[nbyte] = data;
				if(data == P7E_START)
				{
					staff = 1;
				}
				else
				{
					nbyte++;
				}
				return 2;
			}
			else
			{
				staff = 0;
				if(data == 0x00)
				{
					nbyte++;
					return 2; // ���������� �������� ���������
				}
				else
				{
					P7E_State = WAIT_START;	// ������ ��������
					PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //���������� RS ��������
					return 1;
				}
			}
		}
		else
		{
			P7E_State = WAIT_CS;
		}


	case WAIT_CS:
		if(!staff)
		{
			packet_cs = data;
			if(data == P7E_START)
			{
				staff = 1;
				return 2;
			}
			P7E_State = WAIT_START;
			if(packet_cs == intern_cs)
				return 0; // ����� ������
			else
			 {
				PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //���������� RS ��������
				return 1; // ������ cs
			 }
		}
		else
		{
			staff = 0;
			P7E_State = WAIT_START;
			if(data == 0x00)
			{
				if(packet_cs == intern_cs)
					return 0; // ����� ������
				else
				{
					PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //���������� RS ��������
					return 1; // ������ cs
				}
			}
			else
			{
				PORT_CLR(PORT_RSTX_DE,BIT_RSTX_DE); //���������� RS ��������
				return 1;	  // ������ ��������
			}
		}
	}
  }
	return 1;
}


#endif /* P7E_H_ */
