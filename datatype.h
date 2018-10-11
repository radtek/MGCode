#ifndef DiffCorr_DEFINE
#define DiffCorr_DEFINE

#include "CaliLib.h"


typedef struct CALI_PARA{
	CALI_PARA(){
		Clear();
	}
	void Clear(){
		memset(this,0,sizeof(CALI_PARA));
	}
	
	void operator=(const CALI_PARA& src)
	{
		nCaliType=src.nCaliType;
		triPara=src.triPara;
		gridPara=src.gridPara;
		superPara=src.superPara;
		redPara=src.redPara;
		bEnableCoord=src.bEnableCoord;
		nCoord=src.nCoord;
		dbOffsetX=src.dbOffsetX;
		dbOffsetY=src.dbOffsetY;
		dbRotateAngle=src.dbRotateAngle;
	}
	
	UINT nCaliType;	
	//校正类型 #define CALITYPE_TRA 0
	//  #define CALITYPE_GRID 1 
	// #define CALITYPE_SUPER 2
	
	//传统校正方式参数
	TRADITIONPARA triPara;
	
	//网格校正参数
	GRIDPARA	gridPara;
	
	//高精校正参数
	SUPERPARA superPara;
	
	//红光校正参数
	REDPARA redPara;
	
	//读出用户座标系定义
	BOOL bEnableCoord;	//是否启用用户坐标系 0-不启用 1-启用
	UINT nCoord;
	double dbOffsetX;
	double dbOffsetY;
	double dbRotateAngle;
}CALI_PARA;
#endif //DiffCorr_DEFINE