#ifndef __WAKE_H__
	#define __WAKE_H__
	
// ����������� ��������
	#define WAKE_TIMEOUT	1500	// �� - ������� ��������� ������
	#define WAKE_CRC_INIT 	0xDE 	// ��������� �������� ����������� �����

// �������� ����������� ������
	#define FEND  			0xC0    // Frame END
	#define FESC  			0xDB    // Frame ESCape
	#define TFEND 			0xDC    // Transposed Frame END
	#define TFESC 			0xDD    // Transposed Frame ESCape

// �������� ���������
	#define SIGN_DATA  		0x00	// �������� �����
	#define ERR_STAFF 		0x01	// ������ ���������
	#define ERR_SIGN		0x02	// ������ ������������ ��������
	#define ERR_LEN 		0x03	// ������ ����� ���������
	#define ERR_CRC			0x04	// ������ ����������� ����� ������

// ��������� �������� ������ ���������
	enum
	{
		WAIT_FEND,		//�������� ������ FEND
		WAIT_ADDR,		//�������� ������ ������ ����������
		WAIT_SIGN,		//�������� ������ �������� ���������
		WAIT_LEN,		//�������� ������ ���������� ���� � ������
		WAIT_DATA,		//����� ������
	};
	
// ��������� �������� �������� ���������	
	enum
	{
		SEND_FEND,		//�������� ������ ���������
		SEND_ADDR,		//�������� ������ ����������
		SEND_SIGN,		//�������� �������� ���������
		SEND_LEN,		//�������� ���������� ���� � ������
		SEND_DATA,		//�������� ������
		SEND_CRC,		//�������� CRC
	};
	
// ***** ���� ������ ��������� *****
// ��������� �������� ������ ���������
	typedef struct
	{
		unsigned char	state;	// ������� ��������� ��������
		unsigned char	stuff;	// ��������� ��������(������� ����������� ��������)
		//unsigned short	nbt;	// ���������� ���������� ����
		unsigned char	nbt;	// ���������� ���������� ����
		unsigned char	crc;	// ����������� ����� ���������
	} TWAKE_STATUS;

// ����� ��������� ��������� WAKE ��� ������� � ������
	typedef __packed struct
	{
		unsigned char	sign;
		unsigned char	len;
		unsigned char*	Msg;
		unsigned char	crc;
	} TWAKE_PACKET;

// �������������� ����������
	extern unsigned char TxMsg[255];
	extern unsigned char RxMsg[255];
// �������������� �������
	extern void 			WAKE_Init	(unsigned short max_msg_size);
	extern unsigned char 	WAKE_GetMsg (UART_TypeDef * pUART,
										 unsigned char *pCmd,
										 unsigned char* pMsg,
										 unsigned char* pMsgLen);
	extern void 			WAKE_PutMsg	(UART_TypeDef * pUART,
										 unsigned char cmd,
										 unsigned char* pMsg,
										 unsigned char msg_len);



#endif
