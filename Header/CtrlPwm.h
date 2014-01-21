/*
 * CtrlPwm.h
 *
 *  Created on: 18.04.2011
 *      Author: 129
 */

#ifndef CTRLPWM_H_
#define CTRLPWM_H_

#define SIZE_CTRL_PWM		100	// ������ ������� ���������� CtrlPwm

// �������� ���������� � ������� CtrlPwm.
#define CPWM_MGN			0	// ���������
#define CPWM_ANGL			1	// ������� ���� ��� PWM ������
#define CPWM_BETTA			2	// ���� ������� ���������� � ����������� �������
#define	CPWM_IQIN			3	// �������
#define CPWM_IQ				4	// ������������ ���� q
#define CPWM_ID				5	// ������������ ���� d
#define CPWM_DIQ			6	// ������ �� ���� Iq
#define CPWM_DID			7	// ������ �� ���� Id
#define CPWM_UD				8	// ����������
#define CPWM_UQ				9	// ����������
#define	CPWM_TETTA			10	// ���� ������� ����������
#define CPWM_IA				11	// ��� � ���� A � ������ �������� 0
#define CPWM_IB				12	// ��� � ���� B � ������ �������� 0
#define CPWM_IC				13	// ��� � ���� � � ������ �������� 0
#define CPWM_OFST_IA		14	// �������� ���� ���� A
#define CPWM_OFST_IB		15	// �������� ���� ���� B
#define CPWM_OFST_IC		16	// �������� ���� ���� C
#define CPWM_VLC			17	// ��������
#define CPWM_ACL			18	// ���������
#define CPWM_DEBUG1			19	// �������1
#define CPWM_DEBUG2			20	// ������� 2
#define CPWM_DANGLE			21	// �������� ������������� ���� � ��������������
#define CPWM_ANGLINIT		22	// ������� ���� ���������� �������
#define CPWM_DEBUG3			23	// ������� 2
#define CPWM_DEBUG4			24	// ������� 2
#define CPWM_KPI1			25	// ���������� ������� � �������� ����� �� ����
#define CPWM_KP				26	// ������� ����������� ������������������
#define CPWM_KU				27	// ������� ����������� �����������
#define CPWM_KOSS			28	// ������� ����������� �� ��������
#define CPWM_KOSU			29	// ������� ����������� �� ���������
#define CPWM_IQINN			30	// ������� ����������
#define CPWM_KA				31	// ���������� ��� �������� ��������
#define CPWM_ANGLFUL		32	// ������ ���� 21 ������
#define CPWM_DAC0			33	// ��� ���������� ��� ������ � DAC0
#define CPWM_DAC1			34	// ��� ���������� ��� ������ � DAC1
#define CPWM_SCALE_DAC0		35	// ���������� ��� ������ � DAC0
#define CPWM_SCALE_DAC1		36	// ���������� ��� ������ � DAC1
#define CPWM_KTP			37	//
#define CPWM_KOST			38	//
#define CPWM_DELTA			39	// �������� ������
#define CPWM_KV				40	// ���������� �������� �����
#define CPWM_K1				41	//K1
#define CPWM_K2				42	//K2
#define CPWM_DEBUG7			43	//
#define CPWM_DEBUG8			44	//
#define CPWM_DEBUG9			45	//
#define CPWM_DEBUG10		46	//
#define CPWM_POINT			47	// �������� ����� ����������������
#define CPWM_MODE			48	// ����� ������ �����������
#define CPWM_ANGLTASK		49	// ������� �������� ���� �� ���� �������� � CPWM_POINT
#define CPWM_VERSOFT		50  // ������ ��������
#define CPWM_WORD1			51	//word1
#define CPWM_WORD2			52	//word2
#define CPWM_WORD3			53	//word3
#define CPWM_INCR			54	//increm
#define CPWM_COUNT			55	//count
#define CPWM_U27			56	//+27�
#define CPWM_U310			57	//+310�
#define CPWM_IQIN_			58	//�������� ����������
#define CPWM_CAN_COUNT		59	//CAN Count
#define CPWM_CAN_KOMAND		60	//�������
#define CPWM_CAN_ID			61	//CAN ID
#define CPWM_CAN_DLC		62	//CAN DLC
#define CPWM_CAN_DATAH		63	//CAN DATAH
#define CPWM_CAN_DATAL		64	//CAN DATAL
#define CPWM_RS_DATA3		65	//������ ���� 3
#define CPWM_SWITCH			66	//��������� - ����� ���������
#define CPWM_TEMP3			67	//temp3
#define CPWM_POR_IQIN		68	//����������� Iqin �� ��������
#define CPWM_IALFA			69	//ialfa
#define CPWM_IBETTA			70	//ibeta
#define CPWM_INTIQ			71	//���������� Iq
#define CPWM_INTID			72	//���������� Iq
#define CPWM_VLC_FLTR		73  // ������������� ��������
#define CPWM_POR2_IQIN		74	// ����� ����������� IQIN
#define CPWM_KINTV			75	// ���������� �� �� �������� ��� �����������
#define CPWM_INTVLC			76	// ���������� ������ �� ��������
#define CPWM_CAN_STATUS_IN	77  // ������� ������ �������� �� CAN
#define CPWM_CAN_STATUS_UM	78  // �������� ������ ��
#define CPWM_TIMCNT			79	// �������� �������� CNT ������� ��������� �����




#endif /* CTRLPWM_H_ */
