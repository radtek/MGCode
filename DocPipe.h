#ifndef FENCER_2006_0627_1136
#define FENCER_2006_0627_1136

#include "Chain.h"
#include "Quad.h"
#include "mfccmd.h"
#include "Lay.h"

typedef struct Motor_Para
{
	long nMove;			//电动机转动的脉冲
	int nDirection;		//电机转动方向
	int nDelay;			//转动后的延时
	CString strIO;
	Motor_Para(){
		nMove = 0;
		nDelay = 0;
		nDirection = 0;
		strIO.Empty();
	};
}MOTOR;

#endif