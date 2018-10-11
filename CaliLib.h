#ifndef CALILIB_HEAD
#define CALILIB_HEAD

#include "stdAfx.h"

//校正类型
#define CALITYPE_TRA 0 //传统校正
#define CALITYPE_GRID 1 //网格校正
#define CALITYPE_SUPER 2 //高精校正
//UINT nCaliType

#define VER_LIMIT 0.001 //版本判断
typedef struct CALIDOT{
	CALIDOT()
	{
		x=y=0.0;
	}
	void Serial(FILE* fp,BOOL bSave)
	{
		if (bSave)
		{
			fwrite(&x,sizeof(double), 1, fp);
			fwrite(&y,sizeof(double), 1, fp);
		}
		else
		{
			fread(&x,sizeof(double),1,fp);
			fread(&y,sizeof(double), 1, fp);
		}
	}
	double x;
	double y;
}CALIDOT;

typedef struct  TRADITIONPARA
{
	TRADITIONPARA()
	{
		memset(this,0,sizeof(TRADITIONPARA));
		dbBox=100.;
		dbXOrg=100.;
		dbYOrg=100.;
		dbXBox=100.;
		dbYBox=100.;
	}
	void operator=(const TRADITIONPARA& src)
	{
		dbBox=src.dbBox;
		dbXOrg=src.dbXOrg;
		dbYOrg=src.dbYOrg;
		dbXAdj=src.dbXAdj;
		dbYAdj=src.dbYAdj;
		dbXBox=src.dbXBox;
		dbYBox=src.dbYBox;
	}
	void Serial(FILE* fp,BOOL bSave)
	{
		if (bSave)
		{
			fwrite(&dbBox,sizeof(double), 1, fp);
			fwrite(&dbXOrg,sizeof(double), 1, fp);
			fwrite(&dbYOrg,sizeof(double), 1, fp);
			fwrite(&dbXAdj,sizeof(double), 1, fp);
			fwrite(&dbYAdj,sizeof(double), 1, fp);
			fwrite(&dbXBox,sizeof(double), 1, fp);
			fwrite(&dbYBox,sizeof(double), 1, fp);
		}
		else
		{
			fread(&dbBox,sizeof(double), 1, fp);
			fread(&dbXOrg,sizeof(double),1,fp);
			fread(&dbYOrg,sizeof(double), 1, fp);
			fread(&dbXAdj,sizeof(double), 1, fp);
			fread(&dbYAdj,sizeof(double), 1, fp);
			fread(&dbXBox,sizeof(double), 1, fp);
			fread(&dbYBox,sizeof(double), 1, fp);
		}
	}
	//传统校正方式
	double dbBox; //BOX大小，新加参数,仅为支持校正软件画BOX用
	double dbXOrg; //未校正大小
	double dbYOrg;
	double dbXAdj; //校正量
	double dbYAdj;
	double dbXBox;	//实际大小
	double dbYBox;
}TRADITIONPARA;

typedef struct  GRIDPARA
{
	GRIDPARA()
	{
		memset(this,0,sizeof(GRIDPARA));
		dbBox=100.;
		dbScaleX=1.;
		dbScaleY=1.;
	}
	void operator=(const GRIDPARA& src)
	{
		dbBox=src.dbBox;
		dbOffsetX=src.dbOffsetX;
		dbOffsetY=src.dbOffsetY;
		//dbKFactor=src.dbKFactor;
		dbOrthoX=src.dbOrthoX;
		dbOrthoY=src.dbOrthoY;
		dbTrapezoidX=src.dbTrapezoidX;
		dbTrapezoidY=src.dbTrapezoidY;
		dbDistortionX=src.dbDistortionX;
		dbDistortionY=src.dbDistortionY;
		dbScaleX=src.dbScaleX;
		dbScaleY=src.dbScaleY;
	}
	void Serial(FILE* fp,BOOL bSave)
	{
		if (bSave)
		{
			fwrite(&dbBox,sizeof(double), 1, fp);
			fwrite(&dbOffsetX,sizeof(double), 1, fp);
			fwrite(&dbOffsetY,sizeof(double), 1, fp);
			fwrite(&dbOrthoX,sizeof(double), 1, fp);
			fwrite(&dbOrthoY,sizeof(double), 1, fp);
			fwrite(&dbTrapezoidX,sizeof(double), 1, fp);
			fwrite(&dbTrapezoidY,sizeof(double), 1, fp);
			fwrite(&dbDistortionX,sizeof(double), 1, fp);
			fwrite(&dbDistortionY,sizeof(double), 1, fp);
			fwrite(&dbScaleX,sizeof(double), 1, fp);
			fwrite(&dbScaleY,sizeof(double), 1, fp);
		}
		else
		{
			fread(&dbBox,sizeof(double), 1, fp);
			fread(&dbOffsetX,sizeof(double), 1, fp);
			fread(&dbOffsetY,sizeof(double), 1, fp);
			fread(&dbOrthoX,sizeof(double), 1, fp);
			fread(&dbOrthoY,sizeof(double), 1, fp);
			fread(&dbTrapezoidX,sizeof(double), 1, fp);
			fread(&dbTrapezoidY,sizeof(double), 1, fp);
			fread(&dbDistortionX,sizeof(double), 1, fp);
			fread(&dbDistortionY,sizeof(double), 1, fp);
			fread(&dbScaleX,sizeof(double), 1, fp);
			fread(&dbScaleY,sizeof(double), 1, fp);
		}
	};
	double	dbBox;//BOX的大小
	double	dbOffsetX;//中心点偏移量X方向
	double	dbOffsetY;//中心点偏移量Y方向
//	double	dbKFactor;//lsb/mm 程序内部计算，外部不可见
	double	dbOrthoX;//X方向上的平行四边形校正
	double	dbOrthoY;//Y方向上的平行四边形校正
	double	dbTrapezoidX;//X方向上的梯形校正
	double	dbTrapezoidY;//Y方向上的梯形校正
	double	dbDistortionX;//X方向上的凹凸形校正
	double	dbDistortionY;//Y方向上的凹凸形校正
	double	dbScaleX;//0~1 缩放比例
	double	dbScaleY;//0~1
}GRIDPARA;


typedef struct SuperCaliPara 
{
	SuperCaliPara()
	{
		nRow=nColumn=0;
		dbXRange=dbYRange=0.0;
		pFindPoint=NULL;
	};
	~SuperCaliPara()
	{
		if (pFindPoint!=NULL)
		{
			delete[] pFindPoint;
			pFindPoint=NULL;
		}
	}
	void operator=(const SuperCaliPara& src)
	{
		nRow=src.nRow;
		nColumn=src.nColumn;
		dbXRange=src.dbXRange;
		dbYRange=src.dbYRange;
		if (pFindPoint!=NULL)
		{
			delete[] pFindPoint;
			pFindPoint=NULL;
		}
		if (src.pFindPoint!=NULL)
		{
			if (nRow*nColumn>0)
			{
				pFindPoint=new CALIDOT[nRow*nColumn];
				memcpy(pFindPoint,src.pFindPoint,sizeof(CALIDOT)*nRow*nColumn);//v1.2更改为内存COPY
			}
		}
		
	}
	void Serial(FILE* fp,BOOL bSave)
	{
		if (bSave)
		{
			fwrite(&nRow,sizeof(int), 1, fp);
			fwrite(&nColumn,sizeof(int), 1, fp);
			fwrite(&dbXRange,sizeof(double), 1, fp);
			fwrite(&dbYRange,sizeof(double), 1, fp);			
		}
		else
		{
			fread(&nRow,sizeof(int), 1, fp);
			fread(&nColumn,sizeof(int), 1, fp);
			fread(&dbXRange,sizeof(double), 1, fp);
			fread(&dbYRange,sizeof(double), 1, fp);
			if (pFindPoint!=NULL)  //注意，加载数据可能会重新分配内存
			{
				delete[] pFindPoint;
				pFindPoint=NULL;
			}
			if (nRow*nColumn>0)
			{
				pFindPoint=new CALIDOT[nRow*nColumn];
			}			
		}
		for (int i=0;i<nRow;i++)
		{
			for (int j=0;j<nColumn;j++)
			{
				pFindPoint[i*nColumn+j].Serial(fp,bSave);
			}
		}
	}
	int nRow;
	int nColumn;
	double dbXRange;
	double dbYRange;
	CALIDOT *pFindPoint; //高精校正用插值阵列指针  行索引*列数+列索引
}SuperCaliPara;

typedef struct  SUPERPARA
{
	SUPERPARA(){
		memset(this,0,sizeof(SUPERPARA));
		dbVer=1.1; 
		dbBox=100.;
//ver1.0
		//dbScaleX=1.;
		//dbScaleY=1.;
//ver1.1
		dbScaleL1=1.;
		dbScaleL2=1.;
		dbScaleL3=1.;
		dbScaleL4=1.;
	}
	void operator=(const SUPERPARA& src)
	{
		dbVer=src.dbVer;
		dbBox=src.dbBox;
		dbScaleL1=src.dbScaleL1;
		dbScaleL2=src.dbScaleL2;
		dbScaleL3=src.dbScaleL3;
		dbScaleL4=src.dbScaleL4;
		superCaliPara=src.superCaliPara;
	}
	void Serial(FILE* fp,BOOL bSave)
	{
		if (bSave)
		{
			fwrite(&dbVer,sizeof(double), 1, fp);
			fwrite(&dbBox,sizeof(double), 1, fp);
			fwrite(&dbScaleL1,sizeof(double), 1, fp);
			fwrite(&dbScaleL2,sizeof(double), 1, fp);	
			fwrite(&dbScaleL3,sizeof(double), 1, fp);
			fwrite(&dbScaleL4,sizeof(double), 1, fp);
		}
		else
		{
			fread(&dbVer,sizeof(double), 1, fp);
			if (fabs(dbVer-1.0)<VER_LIMIT)
			{
				dbVer=1.1;
				fread(&dbBox,sizeof(double), 1, fp);
				fread(&dbScaleL1,sizeof(double), 1, fp);
				fread(&dbScaleL2,sizeof(double), 1, fp);
				dbScaleL3=dbScaleL1;
				dbScaleL4=dbScaleL2;
// 				fread(&dbScaleL3,sizeof(double), 1, fp);
// 				fread(&dbScaleL4,sizeof(double), 1, fp);
			}
			else
			{
				fread(&dbBox,sizeof(double), 1, fp);
				fread(&dbScaleL1,sizeof(double), 1, fp);
				fread(&dbScaleL2,sizeof(double), 1, fp);
				fread(&dbScaleL3,sizeof(double), 1, fp);
				fread(&dbScaleL4,sizeof(double), 1, fp);
			}
		}
		superCaliPara.Serial(fp,bSave);
	}
	double dbVer;//版本信息
	double dbBox;//BOX大小
	double	dbScaleL1;//0~1 缩放系数	***L3****
	double	dbScaleL2;//0~1				*		*
	double	dbScaleL3;//				L4		L2
	double	dbScaleL4;//				*		*
	SuperCaliPara superCaliPara;//		***L1****		
}SUPERPARA;

typedef struct REDPARA 
{
	REDPARA(){
		memset(this,0,sizeof(REDPARA));
		dbVer=1.0;
		dbScaleX=1.0143;
		dbScaleY=1.0143;
	}
	void operator=(const REDPARA& src)
	{
		dbVer=src.dbVer;
		dbScaleX=src.dbScaleX;
		dbScaleY=src.dbScaleY;
	}
	void Serial(FILE* fp,BOOL bSave)
	{
		if (bSave)
		{
			fwrite(&dbVer,sizeof(double), 1, fp);
			fwrite(&dbScaleX,sizeof(double), 1, fp);
			fwrite(&dbScaleY,sizeof(double), 1, fp);			
		}
		else
		{
			fread(&dbVer,sizeof(double), 1, fp);
			fread(&dbScaleX,sizeof(double), 1, fp);
			fread(&dbScaleY,sizeof(double), 1, fp);
		}
	}
	double dbVer;//版本信息
	double	dbScaleX;//0~1
	double	dbScaleY;//0~1
}REDPARA;

typedef struct DiffCorr 
{
	DiffCorr(){
		memset(this,0,sizeof(DiffCorr));
	}
	void Serial(FILE* fp,BOOL bSave)
	{
		if (bSave)
		{
			fwrite(&dbBoxSize,sizeof(double), 1, fp);
			fwrite(&dbXPosDiff,sizeof(double), 1, fp);
			fwrite(&dbXMinusDiff,sizeof(double), 1, fp);
			fwrite(&dbYPosDiff,sizeof(double), 1, fp);
			fwrite(&dbYMinusDiff,sizeof(double), 1, fp);
		}
		else
		{
			fread(&dbBoxSize,sizeof(double), 1, fp);
			fread(&dbXPosDiff,sizeof(double), 1, fp);
			fread(&dbXMinusDiff,sizeof(double), 1, fp);
			fread(&dbYPosDiff,sizeof(double), 1, fp);
			fread(&dbYMinusDiff,sizeof(double), 1, fp);
		}
	}
	double dbBoxSize; //级别校正Box大小
	double dbXPosDiff;//X轴正向偏差
	double dbXMinusDiff;//X轴负向偏差
	double dbYPosDiff;//Y轴正向偏差
	double dbYMinusDiff;//Y轴负向偏差 
}DiffCorr;

typedef DiffCorr* pDIFFCORR;

typedef struct DIFFCORRPARA 
{
	DIFFCORRPARA(){
		memset(this,0,sizeof(DIFFCORRPARA));
		dbVer=1.0;
	}
	~DIFFCORRPARA()
	{
		if (pDiffCorrData!=NULL)
		{
			delete[] pDiffCorrData;
			pDiffCorrData=NULL;
		}
	}
	void operator=(const DIFFCORRPARA& src)
	{
		dbVer=src.dbVer;
		bEnableDiffCorr=src.bEnableDiffCorr;
		iDiffGrade=src.iDiffGrade;
		if (pDiffCorrData!=NULL)
		{
			delete[] pDiffCorrData;
			pDiffCorrData=NULL;
		}
		if (src.pDiffCorrData!=NULL)
		{
			if (iDiffGrade>0)
			{
				pDiffCorrData=new DiffCorr[iDiffGrade];
				memcpy(pDiffCorrData,src.pDiffCorrData,sizeof(DiffCorr)*iDiffGrade);//v1.2更改为内存COPY
			}
		}
		
	}
	void Serial(FILE* fp,BOOL bSave)
	{
		if (bSave)
		{
			fwrite(&dbVer,sizeof(double), 1, fp);
			fwrite(&bEnableDiffCorr,sizeof(BOOL), 1, fp);
			fwrite(&iDiffGrade,sizeof(int), 1, fp);
		}
		else
		{
			fread(&dbVer,sizeof(double), 1, fp);
			fread(&bEnableDiffCorr,sizeof(BOOL), 1, fp);
			fread(&iDiffGrade,sizeof(int), 1, fp);
			if (pDiffCorrData!=NULL)
			{
				delete[] pDiffCorrData;
				pDiffCorrData=NULL;
			}
			if (iDiffGrade>0)
			{
				pDiffCorrData=new DiffCorr[iDiffGrade];
			}
		}
		for (int i=0;i<iDiffGrade;i++)
		{
			pDiffCorrData[i].Serial(fp,bSave);
		}
	}
	double dbVer; //版本号
	BOOL bEnableDiffCorr; //是否启用多级校正
	int	iDiffGrade;//级别数
	pDIFFCORR pDiffCorrData; //级别校正数据
}DIFFCORRPARA;

//以下函数返回值约定 0表成功，其它表错误，
//返回值定义
#define FUN_SUCCESS 0
#define FUN_PARAERR 1 //参数错误
#define FUN_INIERR 2 //初始化失败


//初始化函数
UINT WINAPI Initial(UINT nHeadCnt);
UINT WINAPI Create(UINT nHead,UINT nCaliType,void* pCaliPara,void * pRedPara);
UINT WINAPI SetCaliPara(UINT nHead,UINT nCaliType,void* pCaliPara);
UINT WINAPI SetRedPara(UINT nHead,void * pRedPara);

//必须在硬件坐标系下的，
//返回：0-成功 1-表维数不正确
//启用多极校正返回值定义
#define INI_DIFFCORR_SUCCESS 0
#define INI_DIFFCORR_FAIL	1
UINT WINAPI EnableDiffCorr(UINT nHead,double dbBox,int iDiffGrade,void* pDiffCorr); //dbBox为最外层BOX大小
UINT WINAPI DisableDiffCorr(UINT nHead);
//退出时调用清除操作
UINT WINAPI Del(UINT nHead);
UINT WINAPI DelAll();  //库退出时必须调用此函数，否则会有内存泄漏
// 相对中心点偏移量函数	,是否放在还是单独处理？
// UINT WINAPI SetCenter(double dbOffsetX,double dbOffsetY);
//DA校正接口
UINT WINAPI DoCali(UINT nHead,double sx,double sy,int& ux,int& uy,BOOL bRed=FALSE);
//网格校正误差表,HAN'S Cam用
// UINT WINAPI GetTable(UINT nHead,,,,);

#endif