// Stroke.h: interface for the CStroke class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STROKE_H__61FEE585_4852_11D4_A483_00105A615525__INCLUDED_)
#define AFX_STROKE_H__61FEE585_4852_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Quad.h"	// Added by ClassView
#include "Dot.h"	// Added by ClassView
#include "Property.h"
#include "Layer.h"
#include "XDC.h"

#define ESP 0.00001

class CStroke;
typedef CTypedPtrList<CObList,CStroke *> StrokeList;
typedef CArray<CDot,CDot> DotList;

class HotDot
{
public:
	int m_iStrokeIndex; //所属线条索引
	CDot m_CrossDot;	//交点信息
	BOOL m_bOutLine;		//是否为外轮廓
	void operator =(const HotDot& src)
	{
		m_iStrokeIndex=src.m_iStrokeIndex;
		m_CrossDot=src.m_CrossDot;
		m_bOutLine=src.m_bOutLine;
	}
};

//DOTEX结构体主要用于进行缩过处理
typedef struct
{
	double x;
	double y;
	UINT nFlag;		//做为标志做用
}DOTEX;
typedef CArray<DOTEX,DOTEX> DotExList;

class CStroke : public CObject  
{
	DECLARE_SERIAL(CStroke)

public:
	CStroke(CProperty pty);	
	CStroke();
	virtual ~CStroke();
	
    CStroke& operator=(const CStroke &stroke);

	void AddRect(CWnd *pWnd);
	BOOL Click(CPoint &point,CWnd *pWnd);
	void DelRect(CWnd *pWnd);
	virtual BOOL IsOnDot(CPoint &point,CWnd*pWnd);
	BOOL AddPoint(CPoint &point,CWnd *pWnd);
	BOOL DotRight(CDot &dot,CWnd *pWnd);
	void SplitDot(int nBegin,int nEnd,CDot &dt1,CDot &dt2);
	void AddStroke(CStroke *pStroke);
	void ConnectHeadToHead(CStroke *pStroke);
	void ConnectTailToTail(CStroke *pStroke);

	void Draw(XDC *pDC,int nFont=0, BOOL bSort = FALSE);
	int  GetMarkUnit();

	void MarkPreview(CDC *pDC, const int &nPreviewSpeed, int nFont=FONT_TTF);
	void MarkPointPreview(CDC *pDC, const int &nPreviewSpeed);

	void FilterExcursion(StrokeList& listData);

	BOOL ISOutLineStroke(StrokeList *plistData);
	void FilterUnAvlityDot(DotExList& dotlist,StrokeList& listdata,int& nCount);
	void SetDotToStart(CDot &dt);
	CStroke * Copy();
	CQuad CalLimit();
	CQuad CalLimit(CDot dot,double dbXScale=1,double dbYScale=1);
	void CopyStroke(const CStroke *pStroke);
	void CopyStrokeB(const CStroke *pStroke);

public:
	void SetProperty(int mode,CProperty &pty);
	void SetLayColor(int nLayer,COLORREF color);
	long IsInDots();
	long GetOptLayer();
	void TranslateDot(double &x,double &y,double a,double x0,double y0);
	void SetFlag(BOOL bflag);
	void SetParent(LPVOID *pParent);
	void SetStyle(BOOL bflag);
	void SetLockFlag(BOOL bFlag);
	BOOL GetLockFlag();

	static double Distance(CDot &sDot, CDot &eDot);
	static UINT CalCrossEx(CDot aDot,CDot bDot,CDot cDot,CDot dDot,CDot& dotCross,
		CDot& dotCross2, UINT& nL1Dir,UINT& nL2Dir);
	//计算矢量a->b的角度(-pi,pi]
	static double CalLineAngle(CDot aDot,CDot bDot);
	static double GetAngle(CDot &p1, CDot &p2, CDot &p3);
	
	static BOOL CalCross(CDot aDot,CDot bDot,CDot cDot,CDot dDot,CDot& dotCross,BOOL& bValid);
	static double CalDistance(CDot dot1,CDot dot2);
	static BOOL CalLine(CDot aDot,CDot bDot,double& k,double& b,UINT& nLType);
	static void TransCoord(double x0,double y0,double angle,BOOL bRev,double& x,double &y);
	
	void DrawBmp(CDC *pDC);
	
	void TrueToLog();
	CDot GetAt(int index);
	
	CDot CalCrossPoint(CDot dotS,CDot dotE,double scan);
	void ScanFill(DotList* listInfo, double scan,double str,double end);
	
	
	void Add(CDot dot);
	void Scale(CDot dot,double ratioX,double ratioY);
	void Rotate(CDot dot,double angle);
	void Move(double moveX,double moveY);
	void Effect(double r,double start,BOOL bCCW,BOOL bInside,CQuad quad);
	
	virtual void Serialize(FILE* fp,BOOL bFlag);
	
	
	int GetSize();
	void AddHead(CDot dot);
	void DrawExtern(XDC *pDC);
public:
	BOOL ISClockWise(); 
	//	void ClockWise(BOOL bFlag);
	void Excursion(CStroke* pStroke,double dbOffset);
	BOOL IsRegion();
	void MoveDot(int nNum,double MoveX,double MoveY);
	CDot GetDot(CDot &dot1,CDot &dot2,double dbPoint);
	void RemoveDot(int nIndex);
	BOOL IsEmpty();
	BOOL DotInQuad(CQuad &quad);
	BOOL HasDot(CQuad &quad);
	void LogToTrue();
	void SetDot(unsigned int index,CDot dot);
	void Append(const CStroke &stroke);
	void RemoveAllPoint();
	void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScale100);
	LPVOID GetParent(){return m_pParent;};
	BOOL DotIsInStroke(CDot &dot);
	void FilterPoint(double dbPresic, double dbAngle);
	void Reverse();
	void SetStrokeReverse(CDot &dt);
	BOOL KillAngle(int nNum, double dbSpace);
	double GetLength();
	
public:
	
	BOOL CalArcJoint(CDot aDot,CDot bDot,CDot cDot,double dbPrecision,CStroke* pRtnStroke);
	void DotToDotEx(CDot dot,DOTEX& dt,UINT nFlag=0){dt.x = dot.x;dt.y = dot.y;dt.nFlag = nFlag;}
	void DotExToDot(DOTEX dot,CDot& dt){dt.x = dot.x;dt.y = dot.y;}

public:
	CProperty           m_property;
	CArray<CDot,CDot> m_dotArray;
	
	BOOL m_bLockFlag; // 锁定标记 TRUE：锁定(不能加载到XTracker中), FALSE ：非锁定
	BOOL m_bTag;      // 选定标记 TRUE：已经选定(可编辑)， FALSE:没有选定(不可编辑)
	BOOL m_bStyle;    // 操作标记     0-普通Stroke，1-填充线
	double m_dbR;
	LPVOID *m_pParent;

	CQuad m_quadLimit;

public:
	CDot GetFirstDot();
	CDot GetLastDot();
	void GetStrokeInQuad(CQuad &quad, StrokeList &strokeList);
	static void GetStrokeInQuadFromStrokeList(StrokeList &srcList, StrokeList &desList, CQuad &quad, CProperty &property, int nResinRect);
	static BOOL m_bRedColorDraw;
	int m_nFocusType;
	int m_nCountDealy;

	int m_nResinRect;

	double  m_dbQFre;			//Q频(KHz)
};

#endif // !defined(AFX_STROKE_H__61FEE585_4852_11D4_A483_00105A615525__INCLUDED_)
