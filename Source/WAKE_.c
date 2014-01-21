/************************************************************************************/
/* 								���� ��������� WAKE									*/
/************************************************************************************/
#include "CRC.h"
//#include "UART.h"
#include "usb.h"
#include "WAKE.h"
// ��� �������
#include "defports.h"

/*==================================================================================*/
/* ���������� ����������															*/
/*==================================================================================*/
unsigned char TxMsg[256];
unsigned char RxMsg[256];

/*==================================================================================*/
/* ��������� ����������																*/
/*==================================================================================*/
static unsigned char	my_addr = 0;		// ����� �����������
static unsigned short	max_pack_size;		// ������������ ������ ������ ��� ����������
static TWAKE_STATUS		RcvStatus;		// ��������� �������� ������ ���������
static TWAKE_PACKET		RxPacket;		// ����������� ����� ������
static TWAKE_PACKET		TxPacket;		// ������������ ����� ������

/*==================================================================================*/
/* ��������� �������																*/
/*==================================================================================*/
void 					WAKE_Init			(unsigned short max_msg_size);
unsigned char 			WAKE_GetMsg 		(
											 unsigned char *pCmd,
											 unsigned char* pMsg,
											 unsigned char* pMsgLen);
void 					WAKE_PutMsg			(
											 unsigned char cmd,
											 unsigned char* pMsg,
											 unsigned char msg_len);
static void 			WAKE_PutErrMsg		(
											 unsigned char err);
static unsigned char 	WAKE_GetPacket 		(TWAKE_STATUS* pStatus,
											 TWAKE_PACKET* pPacket,
											 unsigned char data);
static void 			WAKE_PutPacket		(
											 TWAKE_PACKET* pPacket);
static unsigned char 	WAKE_PutStuffChar	(unsigned char state,
											 unsigned char data,
											 unsigned char* buf);

/*==================================================================================*/
/* ������������� ���������															*/
/*	������� ���������:																*/
/*		addr - ����� ���������� � ����, ��� ������ 2-� ��������� ����� 0			*/
/*	�������� ���������:	���															*/
/*==================================================================================*/
void WAKE_Init (unsigned short max_msg_size)
{
// ��������� ����������
// ���������

	max_pack_size = max_msg_size;	// ������������ ������ ������
}
/*==================================================================================*/
/* ����� ���������																	*/
/*	������� ���������:																*/
/*		port - ����� ����� ������ ������											*/
/*		pMsg - ��������� �� ����������� ���������									*/
/*		pMsgLen - ��������� �� ���������� ��� �������� ������� ��������� ���������	*/
/*	�������� ���������:																*/
/*		0x00 - ��������� �������													*/
/*		0xFF - �������� ������ ���������											*/
/*==================================================================================*/
unsigned char WAKE_GetMsg ( unsigned char *pCmd, unsigned char* pMsg, unsigned char* pMsgLen)
{
// ��������� ����������
	unsigned char 	data;			// ��������� ���� ������
// ���������
// ����������� ��������� �� ���� ���������
	RxPacket.Msg = pMsg;
// ��������� ������
	while (!PORT_EMPTY())
	{
		//WDT_Reset();
	// ������ ������ �� ������ �����
		data = PORT_GetChar();

	// ������������� ���������
		switch (WAKE_GetPacket(&RcvStatus, &RxPacket, data))
		{
			case 0:
			// �������� �������� ���������
				if(RxPacket.sign == 0x01)
					WAKE_PutPacket(&TxPacket); // ������ ������ � ������ ������
				else
				{
					*pMsgLen = RxPacket.len;
					*pCmd 	 = RxPacket.sign;
					return 0;
				}
			break;

			case 1:
			// ������ ���������
			// �������� ��������� �� ������ ������
				WAKE_PutErrMsg(ERR_STAFF);
			break;

			case 2:
			// ������ ������
			// �������� ��������� �� ������ ������
				WAKE_PutErrMsg(ERR_SIGN);
			break;

			case 3:
			// �������� ��������� �� ������ ������
				WAKE_PutErrMsg(ERR_LEN);
			break;
			case 4:
				// ������ crc
			// �������� ��������� �� ������ ������
				WAKE_PutErrMsg(ERR_CRC);
			break;

		}
	}
// ��� ���������
	return 0xFF;
}

/*==================================================================================*/
/* �������� ���������																*/
/*	������� ���������:																*/
/*		port - ����� ����� �������� ������											*/
/*		addr_dest - ����� ���������� ��������� ��� ������ � ����					*/
/*		pMsg - ��������� �� ������������ ���������									*/
/*		msg_len - ������� ������������� ���������									*/
/*	�������� ���������:	���															*/
/*==================================================================================*/
void WAKE_PutMsg (unsigned char cmd, unsigned char* pMsg, unsigned char msg_len)
{
// ��������� ����������
// ���������
// ������������� �������� ��������
	TxPacket.sign = cmd;
	TxPacket.len = msg_len;
	TxPacket.Msg = pMsg;
// �������� ������
	WAKE_PutPacket(&TxPacket);
}

/*==================================================================================*/
/* �������� ��������� � ��������� ������											*/
/*	������� ���������:																*/
/*		port - ����� ����� �������� ������											*/
/*		sign - ��� ��������� ������													*/
/*	�������� ���������:	���															*/
/*==================================================================================*/
static void WAKE_PutErrMsg (unsigned char err)
{
// ��������� ����������
// ���������
// ������������� �������� ��������
	TxPacket.sign = 0x01;
	TxPacket.len = 1;
	TxPacket.Msg = &err;
// �������� ������
	WAKE_PutPacket(&TxPacket);
}

/*==================================================================================*/
/* ����� ������ WAKE																*/
/*	������� ���������:																*/
/*		pStatus - ��������� �� ��������� ������										*/
/*		pPacket - ��������� �� �����												*/
/*		data - ��������� ���� ������												*/
/*	�������� ���������:																*/
/*		0x00 - ��������� �������													*/
/*		0x01 - ������ ���������														*/
/*		0x02 - ������ �������� ������												*/
/*		0x03 - ������ ������� ������												*/
/*		0x04 - ������ ����������� ����� ������										*/
/*		0xFF - �������� ������ ������												*/
/*==================================================================================*/
static unsigned char WAKE_GetPacket (TWAKE_STATUS* pStatus, TWAKE_PACKET* pPacket, unsigned char data)
{
// ��������� ����������
// ���������
// �������� �� ��������� ������
	if (data == FEND)
	{
	// ������������� ������ ���������
		pStatus->state = WAIT_ADDR;
	// ������������� �������� ������ ���������
		pStatus->stuff = 0;
		pStatus->nbt = 0;
		pStatus->crc = WAKE_CRC_INIT;

	}
	else if (pStatus->state != WAIT_FEND)
	{

	// ��������� ���������
		if (pStatus->stuff)
		{
		// ����� �������� ���������
			pStatus->stuff = 0;
		// ������ ����� ���������
			switch (data)
			{
				case TFEND:
					data = FEND;
				break;
				case TFESC:
					data = FESC;
				break;
				default:
					pStatus->state = WAIT_FEND;
				return 1;	// ������ ���������
			}
		}
		else if (data == FESC)
		{
		// ��������� �������� ���������
			pStatus->stuff = 1;
		// ��� ���������
			return 0xFF;
		}
	// ��������� ������
		switch (pStatus->state)
		{
			case WAIT_ADDR:

			// �������� ������� ������ � ���������
				if (data & 0x80)
				{
				// ���������� ������ �� ����� ������
					data &= 0x7F;
				// �������� ������ ����������
					if ((data == 0) || (data == my_addr))
					// ������� � ������ ��������
						pStatus->state = WAIT_SIGN;
					else
						pStatus->state = WAIT_FEND;
					break;
				}
			case WAIT_SIGN:
			// �������� �������� ��������� �� ������������
				if (data & 0x80)
				{
				// ������� � �������� ���������� ���������
					pStatus->state = WAIT_FEND;
				// ����������� ������ ���������
					return 2;	// ������ �������� ������
				}
				else
				{
				// ���������� �������� ���������
					pPacket->sign = data;
				// ������� � ������ ���������� ���� ������ � ���������
					pStatus->state = WAIT_LEN;
				}
			break;
			case WAIT_LEN:

			// ����� ������� ���������
				pPacket->len = data;

				// �������� ���������� ������� ���������
				if (pPacket->len > max_pack_size)
				{
				// ������� � �������� ���������� ���������
					pStatus->state = WAIT_FEND;
				// ����������� ������ ���������
					return 3;	// ������ ����� ������
				}
				else
				{
				// ������������� �������� ���� ������
					pStatus->nbt = 0;
				// ������� � ������ ������
					pStatus->state = WAIT_DATA;
				}

			break;
			case WAIT_DATA:
			// �������� ��������� ����� ���������
				if (pStatus->nbt == pPacket->len)
				{
				// ������� � �������� ���������� ���������
					pStatus->state = WAIT_FEND;
				// �������� ����������� ����� ���������
					if (pStatus->crc != data)
						return 4;	// ������ ����������� ����� ������
					else
						return 0;	// ��������� ��������
				}
				else
					pPacket->Msg[pStatus->nbt++] = data;
			break;
		}
	}
// ������� ����������� �����
	pStatus->crc = CalcByteCRC8(pStatus->crc, data);
// ��� ���������
	return 0xFF;
}

/*==================================================================================*/
/* �������� ������ WAKE 															*/
/*	������� ���������:																*/
/*		port - ����� ����� ��������													*/
/*		addr_dest - ����� ���������� ������											*/
/*		pPacket - ��������� �� ����� ��� ��������									*/
/*	�������� ���������:	���															*/
/*==================================================================================*/
static void WAKE_PutPacket (TWAKE_PACKET* pPacket)
{
// ��������� ����������
	TWAKE_STATUS	TxStatus = {SEND_FEND,0,0,0};	// ��������� �������� ��������
	unsigned char	data = 0;						// ������� ���� ������
	unsigned char	tran_sign = 1;					// ������� �������� �������� 
//
	unsigned char	tx_cnt = 0;						// ���������� ���� ������ � ������
	unsigned char	TxBuf[96];						// ������������� ����� �� ������ 
													// ���� ������ ������ �������� USB (100 ����)
													
// ���������
// �������� ���������
	while(tran_sign)
	{
	// ��������� ��������� �������� �������� ���������
		switch(TxStatus.state)
		{
			case SEND_FEND:
			// �������� �������� ������ ���������
				data = FEND;
			// ������������� �������� ��������
				TxStatus.nbt = 0;
				TxStatus.crc = WAKE_CRC_INIT;
			// ������� � �������� ������
				TxStatus.state = SEND_SIGN;
			break;
			case SEND_SIGN:
			// �������� �������� ������
				data = pPacket->sign & 0x7F;
			// ������� � �������� ������� ���������
				TxStatus.state = SEND_LEN;
			break;
			case SEND_LEN:
			// �������� ������� ���������
				data = pPacket->len;
				// ������� � ����� �������� ������
				TxStatus.state = SEND_DATA;
			break;
			case SEND_DATA:
			// �������� ��������� �������� ����� ���������
				if (TxStatus.nbt == pPacket->len)
				{
				// ����� �������� �������� ���������
					tran_sign = 0;
				// �������� ����������� ����� ���������
					data = TxStatus.crc;
				}
				else
					data = pPacket->Msg[TxStatus.nbt++];
			break;
			default:
			// ����� �������� �������� ���������
				tran_sign = 0;
		}
	// �������� ������ � ����
		tx_cnt += WAKE_PutStuffChar(TxStatus.state, data, &TxBuf[tx_cnt]);
	// �������� ������ � ����
		if (tx_cnt > (sizeof(TxBuf) - 2))
		{
			PORT_PutData(TxBuf, tx_cnt);
		// ����� �������� ������
			tx_cnt = 0;
		}	
	// ���������� ��������� �������� ������ ���������� ��� CRC
	//	if (TxStatus.state == SEND_SIGN)
	//		data &= 0x7F;
	// ������� ����������� ����� ���������
		TxStatus.crc = CalcByteCRC8(TxStatus.crc,data);
	}
// �������� ���������� ������ � ����
	if (tx_cnt)
	{
			PORT_PutData(TxBuf, tx_cnt);
	}
}

/*==================================================================================*/
/* ������ ����� ������ � ����� �� ����������										*/
/*	������� ���������:																*/
/*		state - ������� ��������� �������� ��������									*/
/*		data - ���� ������ ��� ��������												*/
/*		buf - ��������� �� ����� ������ ��� ��������								*/
/*	�������� ���������:	���������� ���������� ���� ������							*/
/*==================================================================================*/
static unsigned char WAKE_PutStuffChar (unsigned char state, unsigned char data, unsigned char* buf)
{
// ��������� ����������
	unsigned char	cnt = 0;		// ���������� ���������� ���� ������
// ���������
// �������� ��������� ��������
	if (state != SEND_SIGN)
	{
	// �������� ������� ���������
		if ((data == FEND) || (data == FESC))
		{
		// ���������� ���������
			buf[cnt++] = FESC;
		// �������������� ����� ������
			switch (data)
			{
				case FEND:
					data = TFEND;
				break;
				case FESC:
					data = TFESC;
				break;
			}
		}
	}
// ������ ������ � �����
	buf[cnt++] = data;
// ����������� ���������� ���������� ���� ������
	return cnt;
}

