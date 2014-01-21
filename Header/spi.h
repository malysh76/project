/*
   spi.h
   Created on: 26.06.2012
*/
#ifndef SPI_H_
#define SPI_H_
#include "1986ve9x_ssp.h"
#include "ports.h"

// ------------ ������������� ����� ��� SPI ----------------------------------

//SPI1
//**********************************
#define PORT_SPI1	 GPIOD			 //B	 //D	 //E	  //F
#define SSP1_FSS	 9				 //12	 //9	 //13	  //2
#define SSP1_CLK	 10				 //13	 //10	 //-	  //1
#define SSP1_RXD	 11				 //14	 //11	 //12	  //3
#define SSP1_TXD	 12				 //15	 //12	 //-	  //0
#define	PERNUM_nPORT1	 	 nPORTD	 //B	 //D	 //E	  //F
#define	PERNUM_nSPI1	 	 nSPI1	 //		 //		 //		  //
//����� ������ ������ �����			 //		 //		 //		  //
//0 � ����							 //		 //		 //		  //
//1 � �������� �������				 //		 //		 //		  //
//2 � �������������� �������		 //		 //		 //		  //
//3 � ���������������� �������		 //		 //		 //		  //
#define	MODE_WORK1	 3				 //2	 //3	 //2	  //2

//**********************************


//SPI2
//**********************************
#define PORT_SPI2	 GPIOC           //B       //C    //C    //F      //D
#define SSP2_FSS	 0               //12???   //0    //14   //12     //3
#define SSP2_CLK	 1               //13      //1    //-    //13     //5
#define SSP2_RXD	 2               //14???   //2    //15   //14???  //2
#define SSP2_TXD	 3               //15      //3    //-    //15     //6
#define	PERNUM_nPORT2	 	 nPORTC  //B       //C    //C    //F      //D
#define	PERNUM_nSPI2	 	 nSPI2	 //        //     //     //       //
//����� ������ ������ �����			 //        //     //     //       //
//0 � ����							 //        //     //     //       //
//1 � �������� �������				 //        //     //     //       //
//2 � �������������� �������		 //        //     //     //       //
//3 � ���������������� �������		 //        //     //     //       //
#define	MODE_WORK2	 3               //3       //3    //2    //3      //2

//**********************************


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ����� ������������ ���������� ������
#define MASK_PORT_SPI1	(((unsigned long)1<<SSP1_TXD)|\
						 ((unsigned long)1<<SSP1_RXD)|\
						 ((unsigned long)1<<SSP1_CLK)|\
						 ((unsigned long)1<<SSP1_FSS))
#define MASK_PORT_SPI2	(((unsigned long)1<<SSP2_TXD)|\
						 ((unsigned long)1<<SSP2_RXD)|\
						 ((unsigned long)1<<SSP2_CLK)|\
						 ((unsigned long)1<<SSP2_FSS))
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ��������� �������� �������
#define PER_CLOCK_SPI1_INIT {PER_CLOCK |= (unsigned long)1<<PERNUM_nPORT1;\
							PER_CLOCK |= (unsigned long)1<<PERNUM_nSPI1;}
#define PER_CLOCK_SPI2_INIT {PER_CLOCK |= (unsigned long)1<<PERNUM_nPORT2;\
							PER_CLOCK |= (unsigned long)1<<PERNUM_nSPI2;}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//����� ������ ������ �����												 
#define PORT_SPI1_FUNC_INIT {PORT_SPI1->FUNC &= ~(((unsigned long)3<<(SSP1_TXD*2))|\
					      						((unsigned long)3<<(SSP1_RXD*2))|\
                                                ((unsigned long)3<<(SSP1_CLK*2))|\
												((unsigned long)3<<(SSP1_FSS*2)));\
							PORT_SPI1->FUNC |=   ((unsigned long)MODE_WORK1<<(SSP1_TXD*2))|\
						  						((unsigned long)MODE_WORK1<<(SSP1_RXD*2))|\
                                                ((unsigned long)MODE_WORK1<<(SSP1_CLK*2))|\
												((unsigned long)MODE_WORK1<<(SSP1_FSS*2));}
#define PORT_SPI2_FUNC_INIT {PORT_SPI2->FUNC &= ~(((unsigned long)3<<(SSP2_TXD*2))|\
					      						((unsigned long)3<<(SSP2_RXD*2))|\
                                                ((unsigned long)3<<(SSP2_CLK*2))|\
												((unsigned long)3<<(SSP2_FSS*2)));\
							PORT_SPI2->FUNC |=   ((unsigned long)MODE_WORK2<<(SSP2_TXD*2))|\
						  						((unsigned long)MODE_WORK2<<(SSP2_RXD*2))|\
                                                ((unsigned long)MODE_WORK2<<(SSP2_CLK*2))|\
												((unsigned long)MODE_WORK2<<(SSP2_FSS*2));}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++													


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
// ����� ������ �����������
//0 � ����������
//1 � ��������
#define	PORT_SPI1_DIGEN_INIT PORT_SPI1->DIGEN |= MASK_PORT_SPI1
#define	PORT_SPI2_DIGEN_INIT PORT_SPI2->DIGEN |= MASK_PORT_SPI2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ������� ���������� �������� ������ � GND (100K)
//0 � �������� � ������� ���������
//1 � �������� � ������� �������� (���� �������� ~100�)
#define	PORT_SPI1_RGND_INIT PORT_SPI1->RGND &= ~MASK_PORT_SPI1
#define	PORT_SPI2_RGND_INIT PORT_SPI2->RGND &= ~MASK_PORT_SPI2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ������� ���������� �������� ������ � VCC (100K)
//0 � �������� � ������� ���������
//1 � �������� � ������� �������� (���� �������� ~100�)
#define	PORT_SPI1_RVCC_INIT PORT_SPI1->RVCC &= ~MASK_PORT_SPI1
#define	PORT_SPI2_RVCC_INIT PORT_SPI2->RVCC &= ~MASK_PORT_SPI2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++	


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define	PORT_SPI1_DDR_INIT PORT_SPI1->DDR |= MASK_PORT_SPI1
#define	PORT_SPI2_DDR_INIT PORT_SPI2->DDR |= MASK_PORT_SPI2
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define	PORT_SPI1_PWR_INIT {PORT_SPI1->PWR &= ~(((unsigned long)3<<(SSP1_FSS*2))|\
											  ((unsigned long)3<<(SSP1_CLK*2))|\
											  ((unsigned long)3<<(SSP1_RXD*2))|\
											  ((unsigned long)3<<(SSP1_TXD*2)));\
						   PORT_SPI1->PWR |=  (((unsigned long)2<<(SSP1_FSS*2))|\
						   					  ((unsigned long)2<<(SSP1_CLK*2))|\
											  ((unsigned long)2<<(SSP1_RXD*2))|\
						   					  ((unsigned long)2<<(SSP1_TXD*2)));}
#define	PORT_SPI2_PWR_INIT {PORT_SPI2->PWR &= ~(((unsigned long)3<<(SSP2_FSS*2))|\
											  ((unsigned long)3<<(SSP2_CLK*2))|\
											  ((unsigned long)3<<(SSP2_RXD*2))|\
											  ((unsigned long)3<<(SSP2_TXD*2)));\
						   PORT_SPI2->PWR |=  (((unsigned long)2<<(SSP2_FSS*2))|\
						   					  ((unsigned long)2<<(SSP2_CLK*2))|\
											  ((unsigned long)2<<(SSP2_RXD*2))|\
						   					  ((unsigned long)2<<(SSP2_TXD*2)));}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
				

//-----------------������������� SPI-----------------------------------
//�������������� ��������
//F_SSPCLK / ( CPSDVR * (1 + SCR)   
//---------------------------------
//200 ����/��� (��� ����� �� 16 ��� -  160��� < 250��� (������ ����� �������)) 
//80���/ (4 * (1 + 19) = 1 ����/���

//---------------- ������� CR0------------------------------
//������� ���� SCR 15...8  �� 0 �� 255
#define CR0_SCR_1	 99	   //SPI1
#define CR0_SCR_2	 9	   //SPI2

//��� SPH 7 - ���� ������� CLKOUT (������������ ������ � ������ SPI Motorolla)
//"0" - �� ��������� ������ R\W, "1" - �� ������� ������. 
#define CR0_SPH_1	 1	   //SPI1
#define CR0_SPH_2	 1	   //SPI2

//��� SPO 6 - ���������� ������� CLKOUT (������������ ������ � ������ SPI Motorolla)
//"0" - � ������ �������� "0", "1" - � ������ �������� "1".
#define CR0_SPO_1	 0	   //SPI1
#define CR0_SPO_2	 0	   //SPI2

//������� ���� FRF 5...4 - ������ ��������������� �����
//00 � �������� SPI ����� Motorola;
//01 � �������� SSI ����� Texas Instruments;
//10 � �������� Microwire ����� National Semiconductor;
//11 � ������
#define CR0_FRF_1	 0	  //SPI1
#define CR0_FRF_2	 0	  //SPI2

//������� ���� DSS 3...0 - ������ ����� ������
//0000 � ������
//0001 � ������
//0010 � ������
//0011 � 4 ����
//|||| - ||||||
//1111 � 16 ���
#define CR0_DSS_1	 15	  //SPI1
#define CR0_DSS_2	 15	  //SPI2

#define SPI1_CR0_INIT (unsigned long)CR0_SCR_1<<8|\
					  (unsigned long)CR0_SPH_1<<7|\
					  (unsigned long)CR0_SPO_1<<6|\
					  (unsigned long)CR0_FRF_1<<4|\
					  (unsigned long)CR0_DSS_1
#define SPI2_CR0_INIT (unsigned long)CR0_SCR_2<<8|\
					 (unsigned long)CR0_SPH_2<<7|\
					 (unsigned long)CR0_SPO_2<<6|\
					 (unsigned long)CR0_FRF_2<<4|\
					 (unsigned long)CR0_DSS_2
//------------------------------------------------------------

//---------------������� CR1----------------------------------
//��� SOD 3 - ������ �������� ����� � ������ �������� ����������
//"0" � ���������� ������ SSP_TXD � ������� ������ ���������
//"1" � ���������� ������ SSP_TXD � ������� ������ ���������
#define CR1_SOD_1	 0		//SPI1
#define CR1_SOD_2	 0		//SPI2

//��� MS 2 - ����� �������� ��� �������� ������ ������
//"0" � ������� ������ (��������������� �� ���������)
//"1" � ������� ������
#define CR1_MS_1	 1		//SPI1
#define CR1_MS_2	 1		//SPI2

//��� SSE 1 - ���������� ������ �����������������
//"0" � ������ ���������, "1" � ������ ���������
#define CR1_SSE_1	 1	    //SPI1
#define CR1_SSE_2	 1	    //SPI2

//��� LBM 0 - ������������ �� ������
//"0" � ���������� ����� ������ �����������������
//"1" � ����� �������� ������ ����������� �������� �� ������ �������� ������ ���������
#define CR1_LBM_1	 0	    //SPI1
#define CR1_LBM_2	 0	    //SPI2

#define SPI1_CR1_INIT (unsigned long)CR1_SOD_1<<3|\
					 (unsigned long)CR1_MS_1<<2|\
					 (unsigned long)CR1_SSE_1<<1|\
					 (unsigned long)CR1_LBM_1
#define SPI2_CR1_INIT (unsigned long)CR1_SOD_2<<3|\
					 (unsigned long)CR1_MS_2<<2|\
					 (unsigned long)CR1_SSE_2<<1|\
					 (unsigned long)CR1_LBM_2
//---------------------------------------------------------------

//----------------������� CPSR-----------------------------------
//������� ���� CPSDVSR 7...0 �� 2 �� 254
//����������� ������� �������� �������
//����� ������ ���� ������ (������� ��� ��������� ��������������� � "0"
#define CPSR_CPSDVSR_1	 4		//SPI1
#define CPSR_CPSDVSR_2	 4		//SPI2

#define SPI1_CPSR_INIT (unsigned long)CPSR_CPSDVSR_1   //SPI1
#define SPI2_CPSR_INIT (unsigned long)CPSR_CPSDVSR_2   //SPI2
//----------------------------------------------------------------

//----------------������� IMSC------------------------------------
//������� ����� ����������
//"1" � �� �����������
//"0" � �����������

//��� TXIM 3 - ����� ���������� �� ���������� �� 50% � ����� ������ FIFO �����������
#define IMSC_TXIM_1	0	 //SPI1
#define IMSC_TXIM_2	0	 //SPI2

//��� RXIM 2 - ����� ���������� �� ���������� �� 50% � ����� ������ FIFO ���������
#define IMSC_RXIM_1	0	//SPI1
#define IMSC_RXIM_2	0	//SPI2

//��� RTIM 1 - ����� ���������� �� �������� ��������� 
//(����� FIFO ��������� �� ���� � �� ���� ������� ��� ������ � ������� ������� ��������)
#define IMSC_RTIM_1	0	//SPI1
#define IMSC_RTIM_2	0	//SPI2

//��� RORIM 0 -	����� ���������� �� ������������ ������ ���������
#define IMSC_RORIM_1	0	//SPI1
#define IMSC_RORIM_2	0	//SPI2

#define SPI1_IMSC_INIT (unsigned long)IMSC_TXIM_1<<3|\
					  (unsigned long)IMSC_RXIM_1<<2|\
					  (unsigned long)IMSC_RTIM_1<<1|\
					  (unsigned long)IMSC_RORIM_1
#define SPI2_IMSC_INIT (unsigned long)IMSC_TXIM_2<<3|\
					  (unsigned long)IMSC_RXIM_2<<2|\
					  (unsigned long)IMSC_RTIM_2<<1|\
					  (unsigned long)IMSC_RORIM_2
//-----------------------------------------------------------------

//-----------------������� DMACR-----------------------------------
//������� ���������� ������ �������� � ������
//"1" - ���������, "0" - ���������
//��� TXDMAE 1 - ������������� DMA ��� ��������
#define DMACR_TXDMAE_1	0	 //SPI1
#define DMACR_TXDMAE_2	0	 //SPI2

//��� RXDMAE 0 - ������������� DMA ��� ������
#define DMACR_RXDMAE_1	0 	 //SPI1
#define DMACR_RXDMAE_2	0 	 //SPI2

#define SPI1_DMACR_INIT (unsigned long)DMACR_TXDMAE_1<<1|\
					   (unsigned long)DMACR_RXDMAE_1
#define SPI2_DMACR_INIT (unsigned long)DMACR_TXDMAE_2<<1|\
					   (unsigned long)DMACR_RXDMAE_2					   				
//------------------------------------------------------------------

//������ � ����� SPI
#define			SPI1_WR(data)		SSP1->DR = ((unsigned long)data)	//SPI1
#define			SPI2_WR(data)		SSP2->DR = ((unsigned long)data)	//SPI2

//������ �� ������ SPI
#define			SPI1_RD				SSP1->DR   //SPI1
#define			SPI2_RD				SSP2->DR   //SPI2

//---------------------------------------------
//������ ����� ������� ������ FIFO ���������
//"0" - ����� FIFO ��������� ����;
//"1" - ����� FIFO ��������� �� ����.
//������� SR
//��� 2 - RNE
#define			SPI1_SR_RNE				(SSP1->SR & 0x04)  //SPI1
#define			SPI2_SR_RNE				(SSP2->SR & 0x04)  //SPI2
//---------------------------------------------

// ������� �������
extern void SPI1_Init (void);
extern void SPI2_Init (void);

#endif /* SPI_H_ */
