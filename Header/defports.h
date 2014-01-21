#ifndef DEFPORTS_H_
	#define DEFPORTS_H_
// �����������
#include "1986ve9x_gpio.h"

// ����� ������ �����������
//#define MODE_CONTROL	//���������, ��������
#define MODE_WORK		//���������� �������

//����� ������ ��������� ��������
//#define CONTROL_ANALOG  //���������� ����������
//#define CONTROL_DIGITAL //�������� ���������� �� CAN

//#define FEEDBACK_SPEED_ON  //�������� ����� �� �������� ��������

#define	K_FEEDBACK_SPEED  1	   //����������� �������� ����� �� ��������


// ��������� �����������

// ��������� ��������� ����������

// ������� ���������� ���������� � ���
#define HSE			8

// �������� ������� PLLCPUOUT � ���
#define PLLCPUOUT	80


// ������� ��� � ���
#define FPWM		10

// �������� �������� �������, � �� ( 1...3180)
#define DTGDELAY	2000

// �������� �������� ����������
//#define ADC_REF_INT
#define ADC_REF_EXT

//-----------------------------------------------
//**********************
//����� ������ CAN�
//#define	CAN_1
#define	CAN_2
//**********************
#ifdef CAN_1
#define MDR_CANx  MDR_CAN1
#define RST_CLK_PCLK_CANx  RST_CLK_PCLK_CAN1

#else
//CAN2
#define MDR_CANx  MDR_CAN2
#define RST_CLK_PCLK_CANx  RST_CLK_PCLK_CAN2
#endif

//�������� ���������� ��� � ����� ����� ������ ������� ���.���
//#define  ZADERGKA_OFF_VIP1			20000  //�������� ���������� ���
//#define  ZADERGKA_OFF_VIP2			30000  //�������� ���������� ���

//����������� �� ����� ���������� Iqin_
#define  LIMIT_IN_IQIN				20	   //������������ ��������, �� ������� ����� ���������
										   //Iqin_ �� ���� ���� �������

//������� ���������� �� �������������
//��� ������� ����������� ������� ����
#define LEVEL_BS_RELAY			2000

//������� ���������� ��
//��� ������� �������� ���
#define LEVEL_BS_PWM			2100 	//20 �����



//������� Iqin_ ��� ��������� ��������
//#define  ALARM_IQIN_				100	   // ������� ����� ����� 18 - 24 ����/���.

#define SWITCH_1_RS				1		   //��-30 (��)
#define SWITCH_2_RS				2		   //��-���
#define SWITCH_3_RS				4		   //��-���


//------------------------------------------------
// ����� �����-������

//�����:
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//���������� ������ �� ���� � ��������
#define PORT_ALARM_TRIG				GPIOA
#define BIT_ALARM_TRIG				10
#define ALARM_TRIG_IN				PORT_IN(PORT_ALARM_TRIG,BIT_ALARM_TRIG)

/*
//������� ��������� �� "���. ������� ��"
//�1/14 +27�
//dig2_in
#define PORT_VKL_UM				GPIOA
#define BIT_VKL_UM				7
#define VKL_UM_IN				PORT_IN(PORT_VKL_UM,BIT_VKL_UM)
*/


//���������� ������������ - ����� 1
//��-30 (��)   �������� +27�
//dig1_in
#define PORT_SWITH_1			GPIOA
#define BIT_SWITH_1				6
#define SWITH_1_IN				PORT_IN(PORT_SWITH_1,BIT_SWITH_1)



//���������� ������������ - ����� 3
//��-���   �������� ������
//dig4_in
#define PORT_SWITH_3				GPIOA
#define BIT_SWITH_3				11
#define SWITH_3_IN				PORT_IN(PORT_SWITH_3,BIT_SWITH_3)



//���������� ������������ - ����� 2
//��-���   �������� ������
//dig3_in
#define PORT_SWITH_2				GPIOA
#define BIT_SWITH_2				9
#define SWITH_2_IN				PORT_IN(PORT_SWITH_2,BIT_SWITH_2)


/*
//������ "���� ����� �������"
//X5/19 -27�   ���. ���� � ���������� ����
//dig7
#define PORT_LYUKI_SCH			GPIOA
#define BIT_LYUKI_SCH			14
#define LYUKI_SCH_IN			PORT_IN(PORT_LYUKI_SCH,BIT_LYUKI_SCH)
*/

/*
//+15� �� ��������� "����"
//X5/15 +15�   ���. ���� � ���������� ����
//dig5
#define PORT_RSVP				GPIOA
#define BIT_RSVP				12
#define RSVP_IN					PORT_IN(PORT_RSVP,BIT_RSVP)
*/

/*
//������� "��������� �������"
//X5/8 +27�
//dig6
#define PORT_ALARM_ROTATE		GPIOA
#define BIT_ALARM_ROTATE		13
#define ALARM_ROTATE_IN			PORT_IN(PORT_ALARM_ROTATE,BIT_ALARM_ROTATE)
*/

/*
//������ "���� ���."
//X5/20 -27�  ������. ����� ���.
//dig8 
#define PORT_BRAKE_MUFF			GPIOA
#define BIT_BRAKE_MUFF			15
#define BRAKE_MUFF_IN			PORT_IN(PORT_BRAKE_MUFF,BIT_BRAKE_MUFF)
*/

/*
//���������� ��������������� ����������
//�.�.-060 X2/12 +3,3�
//dig20_in
//"0" - ������. ����� 
#define PORT_GOTOV_PREOBR		GPIOD
#define BIT_GOTOV_PREOBR		7
#define GOTOV_PREOBR_IN			PORT_IN(PORT_GOTOV_PREOBR,BIT_GOTOV_PREOBR)
*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//������:
//--------------------------------------------------------------
//����� �������� ���������� ������
#define PORT_RESET_TRIG			GPIOC
#define BIT_RESET_TRIG			8

//������ �� ������ �� SPI IA � IB
#define PORT_EXT_INT1			GPIOF
#define BIT_EXT_INT1			10

//���������� ��� ������� �� �������� 1594��5, "1" - ����.
#define PORT_REGISTR_PWM_OUT	GPIOA
#define BIT_REGISTR_PWM_OUT		0


//������ �� ���. ������� ����
//dig12_out
//"0" - ��� ��������
#define PORT_POWER_RELAY		GPIOC
#define BIT_POWER_RELAY			9


//������ ��������� ������� ����� "��.���"
//X1/8 +15�
//dig21_out
//"0" - +15�
#define PORT_UM_VKL				GPIOD
#define BIT_UM_VKL				8



//����� �� ����� 1 �����������
//��-30 (��)
//dig13_out
#define PORT_OUT_SWITH1			GPIOC
#define BIT_OUT_SWITH1			10				   

//����� �� ����� 2 �����������
//��-��� 
//dig14_out
#define PORT_OUT_SWITH2			GPIOC
#define BIT_OUT_SWITH2			11			

//����� �� ����� 3 �����������
//��-���
//dig15_out
#define PORT_OUT_SWITH3			GPIOC
#define BIT_OUT_SWITH3			12	

//������ 20 ���
//����� � �������� 1594��5 +5�
//dig19
#define PORT_DIG19				GPIOF
#define BIT_DIG19				9

//������ ��������� �����������
//dig10
//"0" - ������ ����������� � ����������������� ���������
#define PORT_RSTX_DE			GPIOC
#define BIT_RSTX_DE				5

#define PORT_dig18				GPIOC
#define BIT_dig18				15

//--------------------------------------------------------------

//����.�������
//��� ������
#define PORT_PWM_OUT			GPIOA
#define BIT_PWM_OUT1			1
#define BIT_PWM_OUT2			2
#define BIT_PWM_OUT3			3
#define BIT_PWM_OUT4			4
#define BIT_PWM_OUT5			5
#define BIT_PWM_OUT6			8





//----------------------------------------------------------------
//����� ��������� ���������� StatusFlg
#define StatusFlg_CNTRL         0  //�����.� "1" ����� �������� ������ ������� "����������"
#define StatusFlg_VIP_OFF       1  //���������� ������� ���. ��� ��� ����. ������� ����� ZADERGKA_OFF_VIP1
#define StatusFlg_VKL_UM        2  //������� � CAN "�������� ��������� ��������" "1" - ��� 
#define StatusFlg_RS_GET          3  //������� ��������� �� RS-422
#define StatusFlg_UM_VKL          4  //���� �� ���������� �������

// ����� �����-������
/*
//------------------------------------------------------------------------------
// ������������� ����� ����������� ���� �����
//------------------------------------------------------------------------------
#define PORT_TYPEBOARD	 		GPIOF		// ����
#define DIGIT_TYPEBOARD 		2			// ������

// ������ ��������� �����
#define TYPEBOARD				PORT_IN(PORT_TYPEBOARD,DIGIT_TYPEBOARD)
*/

/*
//----------------------------------------------------------------------------------------------------
// ���������� ������������� ������������
//----------------------------------------------------------------------------------------------------
#define PORT_INTEGR				GPIOD
#define BIT_INTEGR1				11
#define BIT_INTEGR2				12

#define INTGR1_FLP				PORT_FLP(PORT_INTEGR,BIT_INTEGR1)
#define INTGR2_FLP				PORT_FLP(PORT_INTEGR,BIT_INTEGR2)

*/

//------------------------------------------------------------------------------
//	��������� ����� ���������� CTRL, RDY, ADD, CHARGE
//------------------------------------------------------------------------------
/*
#define PORT_POSITION			GPIOB
#define BIT_POS_0				11
#define BIT_POS_1				12
#define BIT_POS_2				13
#define BIT_POS_3				14
#define BIT_POS_4				15


#define PORT_CTRL				GPIOD
#define BIT_CTRL				10

#define PORT_RDY				GPIOD
#define BIT_RDY					9

#define PORT_ADD				GPIOF
#define BIT_ADD					7

#define PORT_CHARGE				GPIOF
#define BIT_CHARGE				3

#define PORT_DRVERR				GPIOD
#define BIT_DRVERR				13

#define PORT_BRKERR				GPIOD
#define BIT_BRKERR				14


#define CTRL_IN					PORT_IN(PORT_CTRL,BIT_CTRL)
#define DRVERR_IN				PORT_IN(PORT_DRVERR,BIT_DRVERR)
#define BRKERR_IN				PORT_IN(PORT_BRKERR,BIT_BRKERR)

#define POS_IN0					PORT_IN(PORT_POSITION,BIT_POS_0)
#define POS_IN1					PORT_IN(PORT_POSITION,BIT_POS_1)
#define POS_IN2					PORT_IN(PORT_POSITION,BIT_POS_2)
#define POS_IN3					PORT_IN(PORT_POSITION,BIT_POS_3)
#define POS_IN4					PORT_IN(PORT_POSITION,BIT_POS_4)
#define POS_IN					~((PORT_POSITION->RXTX)>>11)&0x1F

#define RDY_SET					PORT_SET(PORT_RDY,BIT_RDY)
#define RDY_CLR					PORT_CLR(PORT_RDY,BIT_RDY)
#define RDY_FLP					PORT_FLP(PORT_RDY,BIT_RDY)
#define RDY_IN					PORT_IN(PORT_RDY,BIT_RDY)


#define ADD_SET					PORT_SET(PORT_ADD,BIT_ADD)
#define ADD_CLR					PORT_CLR(PORT_ADD,BIT_ADD)
#define ADD_FLP					PORT_FLP(PORT_ADD,BIT_ADD)

#define CHARGE_SET				PORT_SET(PORT_CHARGE,BIT_CHARGE)
#define CHARGE_CLR				PORT_CLR(PORT_CHARGE,BIT_CHARGE)
#define CHARGE_FLP				PORT_FLP(PORT_CHARGE,BIT_CHARGE)

//-----------------------------------------------------------------------------
// ���������� ������ ��� ������������� ����������
//-----------------------------------------------------------------------------
#define PORT_EX_WORD_LO  GPIOA		// ������� ����� ������
#define PORT_EX_WORD_ME  GPIOC		// ������� ����� ������
#define PORT_EX_BYTE_HI  GPIOE		// ������� ���� ������
#define PORT_EX_CTRL	 GPIOE		// ������� ����� ����������

#define BIT_MODE		 13			// ��� ����� ������ 0 - �������� 1 - �������
#define BIT_BUSY		 14			// ��� ����� ��������� 0 - �����, 1 - ��������
#define BIT_EXT		 	 15			// ��� ������� ������

*/
#endif
