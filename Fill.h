/* 
�����:����רΪ��ʸ��ͼ�����
	��    ��:�ܷ�
	����ʱ��:20060902
	�� �� ��:1.1
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
#define FILL_MORD	8		//���˳���ȴ�������ٴ��߿�
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
	double dbPtp;			// �Ե㴦��ʱ�ĵ���
	double dbCircleLen;		// ���߼��
    double dbHeadKillLen;	// ������׵���������
    double dbTailKillLen;	// �����β����������
}FILLDATA;

class CFill
{
public:
	CFill(double space,double dbBorderSpace,double dbAngle
		,UINT style,CProperty property,double dbPtp, double dbCircle,double dbHeadKillLen,double dbTailKillLen);
	virtual ~CFill();

public:
	void Proc(StrokeList* plistData,StrokeList& listFillData);
	StrokeList* m_plist;		//ֻ�Ǳ���ָ������ݣ���������ʵ��
	double m_dbSpace;
	double m_dbBorderSpace;
	double m_dbPtp;				// ��������
    double m_dbCircle;			// ���߼��

    double m_dbHeadKillLen;		// ��������
    double m_dbTailKillLen;		// ���߼��
	//m_nStyle����
	//BIT0 �������
	UINT m_nStyle;
	double m_dbAngle;			//���Ƕ�

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
