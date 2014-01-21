#ifndef TIMER_H_
	#define TIMER_H_
// �����������
#include "common1.h"
#include "1986ve9x_systick.h"
#include "1986ve9x_tim.h"
#include "CtrlPwm.h"
#include "defports.h"

//------------ �������� ------------------------------------------------
typedef	struct
		{
			unsigned char int0;	// ������� �������� �������
			unsigned char int1; // ������� �������� �������
		}TCTRL;
//--------------- ����������� ------------------------------------------
#define FLTRDEG					2			// ������� ����������
// ------------- ������������� �������  --------------------------------
//#define CCR1_EN					// ���������� ������ �������� CCR1
#define INIT_TIMx_CHx_CTRL		(unsigned long)3<<9
#define INIT_TIMx_CHx_CTRL1		((unsigned long)3<<10)|((unsigned long)3<<2)|((unsigned long)1<<8)|((unsigned long)1<<0)
#define INIT_TIMx_CHx_CTRL2	      (unsigned long)1<<3 /* ���������� ����� ������������ ������ ��� CNT = 0*/
#define INIT_TIMx_CHx_CTRL2_CCR1 ((unsigned long)1<<3 /* ���������� ����� ������������ ������ ��� CNT = 0*/\
								|(unsigned long)1<<2) /* ��������� CCR1 */
// DTG
#define GEN_DTG_PSC				(unsigned long)(PLLCPUOUT * DTGDELAY/1000)	// �������� �������� ������� DTG
#define PRE_DTG_PSC				(unsigned long)0							// ��������������� �������� ������� DTG
#define INIT_TIMx_CH_DTG		(GEN_DTG_PSC<<8)|PRE_DTG_PSC				

#define StartSoftTimer(ms) 		SoftTimer = ms * FPWM
#define DelayMs(ms)				{StartSoftTimer(ms);	while(SoftTimer){}}

#define SYSTICKSTART			SysTick->CTRL = 0x05
#define SYSTICKSTOP				SysTick->CTRL = 0x04

#define PWM_OFF(timer, x)		timer->CH_CTRL1[x] &= ~(((unsigned long)3<<10)|((unsigned long)3<<2));
#define PWM_ON(timer,x)			timer->CH_CTRL1[x] |= ((unsigned long)3<<10)|((unsigned long)3<<2);

		// ���������� PWM �������
#define PWM_OUTS_OFF(timer) {\
		PWM_OFF(timer,0);\
		PWM_OFF(timer,1);\
		PWM_OFF(timer,2);\
		PWM_OFF(timer,3);\
		}

		// ��������� PWM �������
#define PWM_OUTS_ON(timer) {\
		PWM_ON(timer,0);\
		PWM_ON(timer,1);\
		PWM_ON(timer,2);\
		PWM_ON(timer,3);\
		}

// �������� �� 200 ns �� 65000 ns
#define DelayNs(ns) \
								{\
									SysTick->LOAD = (((ns*2)/25)-16);\
									SysTick->VAL = 0;\
									SYSTICKSTART;\
									while(!(SysTick->CTRL&(1<<16)));\
									SYSTICKSTOP;\
								}


//----------------------------------------------
// ������������ 1 ������� ��������� 1 ������
#define PICHR 	&TIMER1->ICHR[0]
// ������������ 2 ������� ��������� 1 ������
#define PICHR1 	&TIMER1->ICHRn1[1]



// �����������

#define SATURAT(val, min, max)\
{\
	if(val < min)	val = min;\
	else if (val>max) val = max;\
}

//----- ��������� PWM--------------------------------------------------------------
#define SQRT3INV	0x093C					// SQRT3inv = (1 / sqrt(3))*4096 = 0x093C � ����� 4:12
#define INVSQRT3	0x49E6					// invSQRT3 = (1 / sqrt(3))*32767 = 0x49E6 � ����� 1:15
#define TPWM		(PLLCPUOUT*1000/FPWM)	// ������ PWM (80 000 ��� / 10 ��� = 8000)

#define MAXPWM		(TPWM/2)				// ������������ �������� �������� ���������
// ������������ ��� Iqin
#define IQINMAX 	18*KIQIN	// ��� 100 ��




//#define IQINMAX 	4*KIQIN	// ��� 50 ��

// ������������ ��� AddTorque
#define TORQMIN		-37
#define TORQMAX		37
// ������������ ��� IqinRes
#define IQINRESMIN	-40
#define IQINRESMAX	40
// ������������ ������������
#define INTMIN		-32000//-32000
#define INTMAX		+32000//+32000
// ������������ ���������� ���������
#define MGN_MAX		900
// ���������� ��������� ����
#define K_GAMMA		(65536/360)
#define ANGLE_60		10923
#define ANGLE_90		16384
#define ANGLE_180		16384*2
#define ANGLE_240		43690
#define ANGLE_270		16384*3
#define ANGLE_360		16384*4
// ������������ ���� � ������� K_IQIN = Ix(���)/Ix(������) = [��/�]
#define K_IQIN		16						// ��� ������� �� ���� 066 ��

//----------------------------------------------
// ��������� ������ ��� �������� ���������� �����������        
typedef  __packed struct
    {
        unsigned char  var;           // ������ ����������
        unsigned char  frm;           // ����� �������
          signed short amp;           // ���������
        unsigned long  prd;           // ������ � ��

    }TCtrlBuf;  

// ������� ����������
extern volatile unsigned long 	SoftTimer;
extern volatile TCTRL		  	Ctrl;
extern volatile char 			pwm_off;
extern volatile unsigned long   workTime;
extern	volatile unsigned long* pICHR;					// ��������� �� ������� ��������� PWM
extern 	volatile long			CtrlPwm[SIZE_CTRL_PWM];	// ���������� ���������� Pwm
extern  volatile long			last_ang;	            //��������� ��������� ����
extern  volatile unsigned short Count_Contrl;
// ���������� ��� �����������
extern unsigned char oscil_en;							// �������� ������ ��� �����������: 0 - ������, 1 - ����������
extern unsigned char oscil_ind[];						// ������� ������ ��� �������� �����������
extern TCtrlBuf      ctrlbuf;                           // ��������� ��� �������� ���������� �����������





// ������� �������
extern	void 			TIM_pwmInit			(unsigned char ntimer);
extern	void 			TIM_Init			(unsigned char ntimer);
extern	void 			TIM_SysTickInit		(void);
extern 		void 			PWM_Init		(void);
__forceinline extern	 	short 			PWM_sin			(unsigned short gamma);
__forceinline extern	 	short 			PWM_cos			(unsigned short gamma);
__forceinline extern	 	unsigned short 	PWM_atan		(short Ud, short Uq);

#endif
