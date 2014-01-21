#ifndef __FIFO_BUF_H__
	#define __FIFO_BUF_H__
	
// ���� ������
	typedef struct
	{
		volatile unsigned short	wr;		// ��������� �� ������
		volatile unsigned short	rd;		// ��������� �� ������
		unsigned short			size;	// ������ ������ (2^n, ��� n = 1..16)
		unsigned short			mask;	// ����� ������� ������
		volatile unsigned char	*data;	// ��������� �� ����� ������
	} TFIFO_BUF;
	
// ����������������
/************************	������� ������	*************************************/
/*	������� ���������:															*/
/*		fifo - ��������� �� FIFO �����											*/
/*	�������� ���������:	���														*/
/********************************************************************************/	
	#define FIFO_CLEAR(fifo)	{(fifo)->wr = 0; (fifo)->rd = 0;}
/*************	����������� ���������� ������ � ������	*************************/
/*	������� ���������:															*/
/*		fifo - ��������� �� FIFO �����											*/
/*	�������� ���������:	���														*/
/********************************************************************************/	
	#define FIFO_AVAIL(fifo)	((unsigned short)((fifo)->size + (fifo)->wr - (fifo)->rd) & (fifo)->mask)
/*************	����������� ���������� ����� � ������	*************************/
/*	������� ���������:															*/
/*		fifo - ��������� �� FIFO �����											*/
/*	�������� ���������:	���														*/
/********************************************************************************/	
	#define FIFO_FREE(fifo)		((unsigned short)((fifo)->mask + (fifo)->rd - (fifo)->wr) & (fifo)->mask)
	
// �������������� �������
	extern void 			FIFO_Init 	(TFIFO_BUF* fifo, volatile unsigned char* buf, unsigned short fifo_size);
	extern unsigned short	FIFO_Put 	(TFIFO_BUF* fifo, volatile unsigned char* data, unsigned short len);
	extern unsigned short	FIFO_Get	(TFIFO_BUF* fifo, volatile unsigned char* data, unsigned short len);
	
#endif
