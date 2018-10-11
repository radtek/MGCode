// Chain.h: interface for the CChain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAIN_H__93671F63_E3AE_11D3_A07E_00105A615525__INCLUDED_)
#define AFX_CHAIN_H__93671F63_E3AE_11D3_A07E_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Quad.h"	// Added by ClassView
#include "Tree.h"
#include "Node.h"	// Added by ClassView
#include "XDC.h"
#include "MgrText.h"
#include "XTracker.h"
#include "DsfObj.h"
#include "StrokeList.h"
#include "GroupObj.h"

class CChain  
{
public:
	static int selectNull;
	static int selectSingle;
	static int selectMul;
	static int selectGroup;
	static int selectUnion;

	CChain();
	virtual ~CChain();
	void operator=(const CChain& src);

public:
	void MarkPreview(CDC *pDC, const int &nPreviewSpeed);
	int GetCount();
	void Array(CArray<OBJPOS, OBJPOS> &dtArray);
	void CalLimit(CDot dot,BOOL bAll = FALSE,double dbXScale=1.0,double dbYScale=1.0);
	CQuad SetSelectFlag(BOOL *pFlag);
	void GetSelectFlag(BOOL *pFlag);
	void GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray);
	int GetMenuIndex();
	CNode * GetNode();
	void SetProperty(int mode,CProperty pty);
	void Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY);
	//填充
	void GetFillStyle(FILLDATA& fill);
	void SetFillStyle(FILLDATA&	fill,XTracker *pTrack);
	void SelectDraw(CTree *pTree,XTracker *pTrack);
	//剪贴板使用
	void Clipboard(FILE* fp,BOOL bFlag);
	void AddRect(CQuad &quad,CWnd *pWnd);
	/*
		针对PreprocessMark进行改造。
		使之真正成为数据预处理之用而不是简单的用于得到个数用
		所以加入风格参数，定义如下：
		0x00	-	老用法，只是得到当前个数，这种用法只用于在不需要处理的地方，如首个
		0x01	-	对跳号数据进行实际的数据预处理
	*/
	BOOL PreprocessMark(DWORD nStyle=0x00);
	void Mark(HWND hWnd);
	void Copy();
	void Rotate(CDot dot,double angle);

	CQuad GetLimit();
	CQuad GetAllLimit();
	CQuad GetMarkLimit();
	CQuad GetReFurbishQuad();

	int GetStatus();		//返回当前选择集状态
	void Group();
	void UnGroup();
	void UnionGroup();
	void UnUnionGroup();

	void Delete();
	void Serialize(FILE* fp,BOOL bFlag);

	void SetSelect(BOOL bFlag=TRUE);

	CQuad Include(CQuad quad,BOOL bMulSelect);	//根据指定的框选范围进行点取识别

	void Scale(CDot dot,double ratioX,double ratioY);
	void Move(double moveX,double moveY);
	void Draw(XDC *pDC);
	void Add(CGroupObj *pGroupObj);
	void SetSelectedObjbeMarked(BOOL bFlag){m_bSelMark =bFlag;};
	
	void ArrayCopy(int iCnt);
	void SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor);

	void SetAllSelTreeDoubleOutParaAdd(double dbLeftXAdd,double dbLeftYAdd,double dbRightXAdd,double dbRightYAdd);
	void GetAllSelTreeProp(DOUBLEOUTPARA &DbPara, CQuad &QD);
	void GetSelTreeDoubleOutPara(DOUBLEOUTPARA &DoubleOutParaIns);
	void SetAllSelTreeDoubleOutPara(DOUBLEOUTPARA &DoubleOutParaIns);

	void SetDrawFill(CArray<SELQUAD ,SELQUAD> &QuadList, int nIndex);
	void GetQuadListInQuad(CArray<SELQUAD ,SELQUAD> &QuadList, CQuad &TrakerQD);
	BOOL IsInSelQuadList(CArray<SELQUAD ,SELQUAD> &QuadList, CQuad &QD);
	void SetSelInQuadListByDot(CArray<SELQUAD ,SELQUAD> &QuadList, CPoint &point, CWnd* pWnd );
	void SetUnSelInQuadListByDot(CArray<SELQUAD ,SELQUAD> &QuadList, CPoint &point, CWnd* pWnd );

	void OnMAlign(CArray<SELQUAD,SELQUAD> &quadList, int nType);
	void AjustTreeForQuad(CArray<SELQUAD,SELQUAD> &quadList);
	void RefreshSelectQuadList(CArray<SELQUAD,SELQUAD> &quadList);
	double GetLength(); //激光总路程

	void ClearALLMainSelect();

	CQuad& RetSelQuad();
	void   SetProStep(int nStep);
public:
	CTypedPtrList<CObList,CGroupObj *> m_list;	//所有树的头节点列表
	CQuad m_quadLimit;
	int m_nMarkUnit;
public:
	BOOL m_bSelMark;//cyaddfor 2004 被选中部分打标
	int m_nSelectMarkType; //０按对象，１按区域
public:
	void Convert(BOOL bPopMsg = TRUE);
	void ChangeQuad(CPoint &point,XTracker *pTrack,CWnd *pWnd, CArray<SELQUAD ,SELQUAD> &QuadList);
	void ReLoadStrokeList(StrokeList *pStrokeList,BOOL bFlag = FALSE);
	BOOL HitOtherObject(CPoint &point,CWnd *pWnd);
	BOOL HaveSelectObject();
	BOOL DragSelectTree(CDot &dot,double ratioX,double ratioY,double moveX,double moveY);
	void CaptureObject(CQuad &quad,XTracker *pTrack);
	BOOL SetStrokeList(StrokeList *pStrokeList,CPoint &point,CWnd *pWnd);
	BOOL ObjectSelected(CPoint &point,CWnd *pWnd);
	void DelSelectTree(CTree *pTree);
	BOOL MouseOnObject(CPoint &point,CWnd *pWnd);
	void PreTrack();
	CMgrText m_MgrText;
	CQuad GetMarkRange();
	void GetSelectedStrokeList(STROKELIST *pStrokeList);

	void GetColorByNum(int nNum,COLORREF &nColor);
	void GetLayColor(int &nLayer,int &nFillLayer);
	void SetLayColor(int nLayer,int nFillLayer=0);
	BOOL GetMarkSel(){return m_bSelMark;};//cyaddfor 2004 被选中部分打标

	BOOL GetFirstMarkCurrent(double & dbCurrent);
	BOOL ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);

	
    BOOL UnLockLayer(int nLayer);
    BOOL LockLayer(int nLayer);

	void FilterPoint(double dbPrecis, double dbAngle);
	void ExtendObj(double dbSpace,BOOL bIsKillOldObj, XTracker *pTrack, BOOL bOpt=TRUE);

	BOOL InitSort(CArray<SORTNODE,SORTNODE> &Sort, CWnd *pWnd);
	void SortAllObj(CArray<SORTNODE,SORTNODE> &Sort);
	void CalcMarkDot(CArray<SORTNODE,SORTNODE> &Sort);
	double Distance(CDot &sDot, CDot &eDot);

	BOOL InitASort(CArray<ASNODE,ASNODE> &Sort);
	void AutoSortAllObj(CArray<ASNODE,ASNODE> &Sort, DWORD dwStartTree = 0);

	BOOL InitGSort(CArray<GNODE,GNODE> &Sort, DWORD &pStroke);
	BOOL InitSelSort(CArray<GNODE,GNODE> &Sort);

	
	void GSortAllObj(CArray<GNODE,GNODE> &Sort);
	void DelTree(CTree *pTree);

	void SetGroupNum(int nNum);
	void GetLayNoByType(int Type ,int &LayNo);

	void ComToOneTreeCmd();

	void SelGroupByNum(int nNum, XTracker *pTrack,CWnd *pWnd);

	void InserObjToList(CGroupObj *pGroupObj);
	void DelObjInList(CGroupObj *pGroupObj);

	void DelOBj(DWORD dwGroupObj);

	void DelAllObjInList();
	void SetTreeSel(CGroupObj *pGroupObj, XTracker* pTracker);
	void InsertChainObjToList();

	void ObjListLoad();
	void ObjListUnLoad();

	BOOL IsHaveTextOrCodeBar();
	
	void ConvertToOneNodeDelete();
	void UpdateObjMarkSet(OBJMARKSET &ObjMarkSet);
	void GetObjMarkSet(OBJMARKSET &ObjMarkSet);

	void StoreChain(const CChain& chain);
	void ReStoreChain(const CChain& chain);


	void SelMark(HWND hWnd, HANDLE hStop, SUBOBJMARKSET &stSub);

	void FastMark(HWND hWnd);

	void AyCopy(CArray<OBJPOS, OBJPOS> &dtArray);

	void DeleteList();

	void AddList(const CChain &src);

	void JoinStroke(BOOL bPopMsg,double dbMinDist);

public:
	GROUPINFO m_GroupList[10];
//	void TransDsfObjPosToIndex(POSITION *pPos=NULL);
//	void TransDsfObjIndexToPos(void);
//	void TrimDsfObjList(void);


	//Wafer
	class CCopyObject {public: CQuad m_quadLimit; BOOL m_bSelect; BOOL m_bMarked;};
	CList<CCopyObject, CCopyObject&> m_listCopyObject;
	void ResetDivideFlag();
	void GetStrokeInQuad(CQuad &quad, StrokeList &strokeList, BOOL bDivedeByStroke);
	void SetNeedMarkFlag(BOOL arrNeedMark[600]);

	void GetStrokeFram(CQuad &quad, CTree &Tree);
	BOOL ChangeText(CString strTxt,CString ModelNo);


	void GetCopyObjectScale(CDot copyObjdot, double &dbcopyObjXScale , double & dbcopyObjYScale);
	int m_nXRow_mNum;
	int m_nYCol_nNum;
	double m_dbScaleRectX[4];//      CCD捕捉区域
	double m_dbScaleRectY[4];

	double m_dbTheoryRectX[4];//      图形理论区域
	double m_dbTheoryRectY[4];

	double m_dbXTopScale;
	double m_dbXBottomScale; 
	double m_dbYLeftScale;
	double m_dbYRightScale;


};

#endif // !defined(AFX_CHAIN_H__93671F63_E3AE_11D3_A07E_00105A615525__INCLUDED_)
