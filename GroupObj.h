// GroupObj.h: interface for the CGroupObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUPOBJ_H__84771DF0_22D0_44EC_81B5_1D6E9DDB904D__INCLUDED_)
#define AFX_GROUPOBJ_H__84771DF0_22D0_44EC_81B5_1D6E9DDB904D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tree.h"
#include "Nodeplt.h"


class CGroupObj: public CObject
{
public:
	CGroupObj();
	CGroupObj(const CGroupObj *src, UINT nStyle=0x00, BOOL bFill=TRUE);
	virtual ~CGroupObj();
	
public:
	DWORD    m_dwObjId;
	CString  m_strObjName;

	int m_nFlag;
	
	OBJMARKSET m_MarkSet;
	BOOL			m_bMainSelect;

	CQuad GetLimit();
	BOOL Mark(HWND hWnd);
	BOOL CopyToMe( CGroupObj *pSrc, DWORD dwStyle,BOOL bGenFill = TRUE );
	void InsertHead(CTree *pTree);
	void Rotate(CDot &dot, double angle);
	void DeleteAll();
	void Serialize(FILE* fp,BOOL bFlag);
	void SetSelect(BOOL bFlag = TRUE);
	BOOL GetSelect(){return m_bSelect;};
	void Scale(CDot dot,double ratioX,double ratioY);
	void Drag(CDot dot, double ratioX, double ratioY, double moveX, double moveY);
	void Move(double moveX,double moveY);
	void Draw(XDC *pDC);
	BOOL Add(CTree *pTree);
	BOOL IsSelect();
	BOOL Include(CQuad quad);
	void InvertSelect();
	BOOL GetLockFlg();
	void UnionGroup(CGroupObj *pGroupObj);
	void UnUnionGroup( CTypedPtrList<CObList,CGroupObj *> &listGroupObj);
	void Group(CGroupObj *pGroupObj);
	void UnGroup(CTypedPtrList<CObList,CGroupObj *> &listGroupObj);
	BOOL CheckBarCode(CUIntArray &arrayTextID);
	void SelectBarCode(CUIntArray& arrayTextID);
	void SetFillStyle(FILLDATA& fill);
	void GetFillStyle(FILLDATA& fill);
	BOOL IsMainSelect();
	void SetMainSelect(BOOL bSelect);
	void SetProperty(int mode,CProperty pty);
	BOOL PreprocessMark(DWORD nStyle);
	CNode * GetNode();
	void GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray);
	void CalLimit(CDot dot,BOOL bAll,double dbXScale,double dbYScale );

	void SetLayColor(int nLayer, int nFillLayer);
	void SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor);
	void GetLayColor(int &nLayer,int &nFillLayer);

	CQuad GetReFurbishQuad();
	void GetSelectedStrokeList(StrokeList *pStrokeList);
	CQuad GetMarkRange();
	BOOL GetFirstMarkCurrent(double & dbCurrent);
	BOOL ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	void AddRect(CWnd *pWnd);
	BOOL MouseOnObject(CPoint &point,CWnd *pWnd);
	void CaptureObject(CQuad &quad,XTracker *pTrack);
	BOOL Click(CPoint &point,CWnd *pWnd);

	// add by zhq
	int   GetType();
	void RefreshBarText(CWnd* pWnd,UINT nTextID);
	void SetPathOptimizeState(BOOL bState);
	BOOL GetSelObjLockFlg();
	void SetLayerSelect(int nItem,BOOL bFlag);
	void DrawExtern(XDC *pDC);
	int   GetSelObjLayerNo();
	CGroupObj * ExtendObj(double dbSpace, BOOL bOpt);
	void UpdateObjMarkSet(OBJMARKSET &ObjMarkSet);
	BOOL IsBmpNode();
	BOOL IsHaveTextOrCodeBar();
	double GetLength();
	void SetGroupNum(int nNum);
	int   GetGroupNum();
	BOOL ComToOneGroupObj(CNode *pNode);
	BOOL GetFirstDot(CDot &dt);
	BOOL GetLastDot(CDot &dt);
	void FilterPoint(double dbPrecis, double dbAngle);
	void JoinStroke(double dbMinDistance);
	void  Convert();
	UINT CanConvert();
	CGroupObj * ConvertPLT();
	void  GetStrokeList(StrokeList *pStrokeList);
	BOOL IsHaveTxtOrCodeBar();
	void  SetSortStatus(BOOL bIsSort);
	BOOL HaveFunctionNode();
	UINT  GetTreeCount(){return m_list.GetCount();};
	
	virtual void GetTextValue(CString& strTxt);
	void   SetTextData(CString &strTxt);
	BOOL  GetRulerPos(CDot &dot, int nAlignType);


	void SetStrokeList(StrokeList *pStrokeList);


public:
	BOOL m_bNeedMark;
	BOOL m_bMarked;	//是否标记过,按对象切分时用
	void GetStrokeInQuad(CQuad &quad, StrokeList &strokeList);

public:
	CTypedPtrList<CObList,CTree *>	m_list;			//Tree链表

	BOOL			m_bSelect;		//是否被选中
	int				m_nGroupNum;
	int				m_nGroupGraphicType;	//0:No Set, 1:Other, 2:Fullcut, 3:Dum, 4:SmallX, 5:BigX
};

#endif // !defined(AFX_GROUPOBJ_H__84771DF0_22D0_44EC_81B5_1D6E9DDB904D__INCLUDED_)
