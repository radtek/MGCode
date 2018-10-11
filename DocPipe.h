#ifndef FENCER_2006_0627_1136
#define FENCER_2006_0627_1136

#include "Chain.h"
#include "Quad.h"
#include "mfccmd.h"
#include "Lay.h"

typedef struct Motor_Para
{
	long nMove;			//�綯��ת��������
	int nDirection;		//���ת������
	int nDelay;			//ת�������ʱ
	CString strIO;
	Motor_Para(){
		nMove = 0;
		nDelay = 0;
		nDirection = 0;
		strIO.Empty();
	};
}MOTOR;

#endif