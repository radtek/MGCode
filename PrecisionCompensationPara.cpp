#include "stdafx.h"
#include "PrecisionCompensationPara.h"


CPrecisionCompensationPara::CPrecisionCompensationPara(void)
{
	dotBase.x = 0;
	dotBase.y = 0;
	dotBase.z = 0;

	dbScaleX  = 1;
	dbScaleY  = 1;
	dbOffsetX = 0;
	dbOffsetY = 0;
	dbOffsetCCW = 0;
}


CPrecisionCompensationPara::~CPrecisionCompensationPara(void)
{

}
