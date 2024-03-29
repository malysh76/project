/************************************************************************************/
/*					������ ��� ������ � FIFO �������								*/
/*					(������ ������ 2^n (n = 1..16))									*/
/************************************************************************************/
#include "FIFO_BUF.h"

/*==================================================================================*/
/* ���������																		*/
/*==================================================================================*/
/*==================================================================================*/
/* ���������� ����������															*/
/*==================================================================================*/
/*==================================================================================*/
/* ���������																		*/
/*==================================================================================*/
void			FIFO_Init 	(TFIFO_BUF *fifo, volatile unsigned char *buf, unsigned short fifo_size);
unsigned short	FIFO_Put 	(TFIFO_BUF *fifo, volatile unsigned char* data, unsigned short len);
unsigned short	FIFO_Get	(TFIFO_BUF *fifo, volatile unsigned char* data, unsigned short len);

/*==================================================================================*/
/* ������������� ������																*/
/*	������� ���������:																*/
/*		fifo - ��������� �� �������� FIFO ������ 									*/
/*		buf - ��������� �� ����� ��� ������											*/
/*		fifo_size - ������ ������ ��� ������ (������ ���� 2^n)						*/
/*	�������� ���������:	���															*/
/*==================================================================================*/
void FIFO_Init (TFIFO_BUF* fifo, volatile unsigned char *buf, unsigned short fifo_size)
{
// ��������� ����������
// ���������
// ������������� ���������� FIFO ������
	fifo->size = fifo_size;
	fifo->mask = fifo_size - 1;
	fifo->data = buf;
// ������������� ����������
	FIFO_CLEAR(fifo);
}

/*==================================================================================*/
/* ��������� ������ � �����															*/
/*	������� ���������:																*/
/*		fifo - ��������� �� �������� FIFO ������ 									*/
/*		data - ��������� �� ������													*/
/*		len  - ���������� ���� ������ ��� ��������� � �����							*/
/*	�������� ���������:	���������� ���� ������, ���������� � �����					*/
/*==================================================================================*/
unsigned short FIFO_Put (TFIFO_BUF* fifo, volatile unsigned char* data, unsigned short len)
{
// ��������� ����������
	unsigned short	free_cnt;	// ���������� ��������� ������� � ������
	unsigned short	wr_cnt = 0;	// ���������� ������ ���������� � �����
// ���������
// ��������� ������� ���������� ����� � ������
	free_cnt = FIFO_FREE(fifo);
// �������� ������������� ���������� �����
	if (free_cnt < len)
		len = free_cnt;
// ��������� ���� ������ � �����
	while (len--)
	{
	// ��������� ����� ������ � �����
		fifo->data[fifo->wr++] = *data++;
	// �������� �� ���������� ������� ������� ������
		fifo->wr &= fifo->mask;
	// ���������� ���������� ���������� ���� ������
		wr_cnt++;
	}
// ����������� ���������� ������ ���������� � �����
	return wr_cnt;
}

/*==================================================================================*/
/* ���������� ������ �� ������														*/
/*	������� ���������:																*/
/*		fifo - ��������� �� �������� FIFO ������ 									*/
/*		data - ��������� �� ������													*/
/*		len  - ���������� ���� ������ ��� ��������� � �����							*/
/*	�������� ���������:	���������� ���� ������, ���������� � �����					*/
/*==================================================================================*/
unsigned short FIFO_Get (TFIFO_BUF* fifo, volatile unsigned char* data, unsigned short len)
{
// ��������� ����������
	unsigned short	avail_cnt;	// ���������� ���� ������ ��������� � ������
	unsigned short	rd_cnt = 0;	// ���������� ������ ��������� �� ������
// ���������
// ��������� ������� ��������� ������
	avail_cnt = FIFO_AVAIL(fifo);
// �������� ������� ������ � ������
	if (avail_cnt < len)
		len = avail_cnt;
// ���������� ������ �� ������
	while (len--)
	{
	// ���������� ���������� ����� ������
		*data++ = fifo->data[fifo->rd++];
	// �������� �� ���������� ������� ������� ������
		fifo->rd &= fifo->mask;
	// ���������� ���������� ����������� ���� ������
		rd_cnt++;
	}
// ���������� ����������� ���� ������
	return rd_cnt;
}

/*==================================================================================*/
