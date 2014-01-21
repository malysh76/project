/*
 * protocol.h
 *
 *  Created on: 19.04.2011
 *      Author: 129
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

// ������� ��������� wake
#define	APC_NOP				0x00	//	��� ��������
#define	APC_ERROR			0x01	//	�������� ���� ������
#define	APC_ECHO			0x02	//	������ �������� ����������� ������
#define	APC_INFO			0x03	//	������ ���������� �� ���
#define	APC_POS				0x04	//	���������������� �� �������� ����.
#define	APC_BUSY			0x05	//	��������  ���������� ���
#define	APC_WRITE			0x06	//	������ �������� ����� ��������������� ������� �� ���������� ������ ���
#define	APC_READ			0x07	//	������ �������� ����� ��������������� ������� �� ���������� ������ ���
#define	APC_VAR_READ		0x08	//	������ ��������� ���������� �������� �������� ������
#define	APC_VAR_WRITE		0x0F	//	������ ��������� ���������� �������� �������� ������
#define APC_STORE			0x09	//  ���������� ��������� ������ �� ��� � ����������������� ������
#define APC_RESTORE			0x0A	//  �������������� ���-������ ������ �� ����������������� ������ � ���.
#define APC_BOOT            0x0B    //  ������� � ���������
#define APC_FWR_PREP        0x0C    //  ���������� ����� ��������� ��������
#define APC_FWR_LOAD        0x0D    //  �������� ������ ��������.
#define APC_FWR_RUN         0x0E    //  ������ �������� ��������� ����� ����������
#define APC_MODE	        0x10    //  ��������� ������
#define APC_DEBUG			0x11	//  ���������� �����
#define APC_LOG				0x12	//  �������� ����
#define APC_OSC_START		0x14	//  ������ �� ������ ������ ������ ��� ������������
#define APC_OSC_STOP		0x15	//  ������ �� ������� ������ ������ ��� ������������
#define APC_OSC_DATA		0x16	//  ��������� ������ ��� �����������
#define APC_OSC_CTRL		0x17	//  ������� ���������� ����������
#define APC_LOT 	        0x18	//  �������/�������� ��������� �����



// ���� ���������
#define DEV_CODE_BOOT		0x0000	// ���������
#define DEV_CODE_UM100		0x0100	// ��������� �������� 100
#define DEV_CODE_ENC		0x1000	// ������� ����� � ���������

typedef __packed struct
		{
			unsigned short DevCode;	 //	0-�1-� ���� ��� ����������
			unsigned short Version;	 //	2-�3-� ����  - ���-��� ��������.
			unsigned char  date;	 //	4-� ���� �����
			unsigned char  month;	 //	5-� ���� �����
			unsigned char  year;	 //	6-� ���� ���
			unsigned char  State[4]; //	7-�...10-� ����� � ��� ��-������� ���������� ����� ����������-���
		}TInfo;



#endif /* PROTOCOL_H_ */
