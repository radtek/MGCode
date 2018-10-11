#pragma once

#include "Dot.h"

class CPrecisionCompensationPara
{
public:
	CPrecisionCompensationPara(void);
	~CPrecisionCompensationPara(void);

	CDot dotBase;
	double dbScaleX;
	double dbScaleY;
	double dbOffsetX;
	double dbOffsetY;
	double dbOffsetCCW;
};

