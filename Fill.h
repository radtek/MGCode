/* 
填充类:本类专为给矢量图填充用
	作    者:曹锋
	更新时间:20060902
	版 本 号:1.1
//*/

#ifndef FENCER_FILL_H_20060902
#define FENCER_FILL_H_20060902

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Quad.h"
#include "Stroke.h"
#include "Property.h"

#define FILL_BACK	1
#define FILL_CROSS	2
#define FILL_EVEN	4
#define FILL_MORD	8		//打标顺序，先打填充线再打标边框
#define FILL_PTP	16
#define FILL_CIRCLE	32
#define FILL_BETTER	64

typedef struct FILLDATA 
{
	FILLDATA()
	{
		memset(this, 0, sizeof(FILLDATA));
	}
	double dbSpace1;
	double dbSpace2;
	double dbAngle1;
	double dbAngle2;
	double dbDistance;
	UINT nStyle;
	double dbPtp;			// 对点处理时的点间距
	double dbCircleLen;		// 绕线间距
    double dbHeadKillLen;	// 填充线首点缩进长度
    double dbTailKillLen;	// 填充线尾点缩进长度
}FILLDATA;

class CFill
{
public:
	CFill(double space,double dbBorderSpace,double dbAngle
		,UINT style,CProperty property,double dbPtp, double dbCircle,double dbHeadKillLen,double dbTailKillLen);
	virtual ~CFill();

public:
	void Proc(StrokeList* plistData,StrokeList& listFillData);
	StrokeList* m_plist;		//只是保存指针的数据，并不产生实体
	double m_dbSpace;
	double m_dbBorderSpace;
	double m_dbPtp;				// 点间距设置
    double m_dbCircle;			// 绕线间距

    double m_dbHeadKillLen;		// 点间距设置
    double m_dbTailKillLen;		// 绕线间距
	//m_nStyle属性
	//BIT0 反复填充
	UINT m_nStyle;
	double m_dbAngle;			//填充角度

	CQuad m_quadLimit;
	CProperty m_property;

private:
	BOOL IsValide(double x1,double x2,double &ox1,double &ox2);
	void sort_bubble(double *plist, int size,BOOL bFlag);
	void CreateDotArray(DotList* pList,DotList& dotArray);
	void ScanFill(DotList* listInfo,double scan,double str,double end);
	void Rotate(StrokeList* plist,CDot dot, double angle);
	CQuad CalLimit();
	void Sortlist(DotList& dotArray,BOOL bFlag);
	void DeleteTmpList(StrokeList *list);
	void CirclProc(StrokeList *pList, StrokeList *pRetList, double dbLen);
	void BetterProc(CArray<StrokeList*, StrokeList*> &FillList
		            , CArray<StrokeList*, StrokeList*> &RetList);
	double Distance(CDot &sDot, CDot &eDot);
	BOOL   DeletStrokeList(StrokeList* pList);
	void CirclLinePro(CArray<StrokeList*, StrokeList*> &RetList, StrokeList& listFillData);
	void NobetterPro(CArray<StrokeList*, StrokeList*> &FillList, StrokeList& listFillData);
	BOOL StrokeProc(CStroke *pStrk, CArray<StrokeList*, StrokeList*> &RetList);
	void ListSortProc(CArray<StrokeList*, StrokeList*> &FillList
					   , CArray<StrokeList*, StrokeList*> &RetList);
	void ReverStrkList(StrokeList *pStrkList);
	void ReleaseFillList(CArray<StrokeList*, StrokeList*> &FillList);
	void AddToOneList(CArray<StrokeList*, StrokeList*> &FillList, CArray<StrokeList*, StrokeList*> &RetList);

	BOOL ZoomStroke(CStroke* pStroke);
protected:
	void PtpProc(StrokeList* pList);
};

#endif
