// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__FF08A521_DF0A_11D3_845D_444553540001__INCLUDED_)
#define AFX_NODE_H__FF08A521_DF0A_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Quad.h"
#include "Stroke.h"
#include "Property.h"
#include "XDC.h"
class CNode : public CObject  
{
	DECLARE_SERIAL(CNode)

friend class CTree;

public:
	static	CNode * CreateFromType(INT type);
	CNode();
	virtual ~CNode();

	CNode *m_pLeft;
	CNode *m_pRight;
	CLay  *m_play;

	BOOL	m_bSelect;//选中标志 TRUE: 选中; FALSE: 不选中
	UINT    m_nXPos;  //cyaddfor2004 0 head;1 left,2 right,3 tail，4parity bit

	virtual void SetSelect(BOOL bSelect){m_bSelect = bSelect;};
	virtual void InvertSelect();
	virtual BOOL IsSelect(){return m_bSelect;};

	virtual BOOL Include(CQuad quad);

	int	m_type;
	CProperty m_property;

public:
	int  m_nObjID;
	int  m_nObjType;

	BOOL m_bIsCurSortTree;

	int m_nMarkMode;

public:
	int  GetObjID();
	void SetObjID(int nObjID);
    int  GetObjType();
	void SetObjType(int nObjType);

	double GetLength();

	CStroke* GetGoLineStroke();
	
	void SetMarkMode(int nMarkMode);

public:
	StrokeList m_list;

protected:
	CQuad m_quadLimit;
	CTree *m_pParent;		//本节点所处在的树

public:
	virtual void AddToPlt(CNode *pNodeplt);
	virtual void SetStrokeList(StrokeList *pStrokeList);
	BOOL IsEmpty();
	void SetFlag(BOOL bflag);
	BOOL HasDot(CQuad &quad);
	virtual BOOL IsOnDot(CPoint &point,CWnd *pWnd);
	void DelRect(CWnd *pWnd);
	void Serialize_list(FILE* fp,BOOL bFlag);
	virtual void GetStrokeList(STROKELIST *pStrokeList);
	virtual void MarkPreview(CDC *pDC, const int &nPreviewSpeed);
	CTree * GetParent(){return m_pParent;};
	void SetParent(CTree *p){m_pParent=p;};

	virtual int GetBaseType();
	virtual void SetProperty(int mode,CProperty pty, XDC *pDc = NULL);
	void ChangeTo(CQuad quad);
	int GetType();

	virtual void Mark(HWND hWnd);
    virtual void SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsResMark);
	virtual BOOL PreprocessMark(DWORD nStyle);

	virtual CQuad CalLimit();
	virtual CQuad GetLimit(BOOL bRef = FALSE){return m_quadLimit;};

	virtual CQuad GetMarkRange(){return m_quadLimit;};//zg add 2005/08/26 for v4.3

	virtual BOOL Click(CPoint &point,CWnd *pWnd);

	virtual void Draw(XDC *pDC);
	virtual void Adjust(MSG &msg,const CPoint &ptOrg);
	virtual BOOL Track(CWnd* pWnd,CPoint ptOrg);
	virtual BOOL Create(CRect &rect);

	virtual CNode * Copy(UINT nStyle=0x00);
	virtual void Delete();
	virtual BOOL Move(double moveX,double moveY);
	virtual void Scale(CDot dot,double ratioX,double ratioY);
	virtual void Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY);
	virtual void Rotate(CDot dot,double angle);
	virtual void Scan(StrokeList *pList);
	virtual CTree * UnGroup();
	virtual BOOL CanUnGroup();
	virtual BOOL CanGroup();

	virtual void Serialize(FILE* fp,BOOL bFlag){};

	virtual void GetLayColor(int &nLayer,int &nFillLayer);
	virtual void SetLayColor(int nLayer,COLORREF color,int nLayerFill=0,COLORREF colorFill=0);
	virtual BOOL GetBarcode();

	//yaddforhl3.0
	virtual void DeleteFill();
	virtual void GetFillStyle(double &spaceHor,double &spaceVer,int &style,BOOL &bFillBack,double &dbBorderSpace); //zg add
	virtual void SetFillStyle(double spaceHor,double spaceVer,int style,BOOL bRedo=FALSE,BOOL bFillBack=FALSE,double dbBorderSpace=0);//zg add

	BOOL m_bGroup;//表示条码是否为组合对象。
	virtual CNode *ReDefineCopy();
	void SetGroupFlag(BOOL bFlag){m_bGroup = bFlag;};
	virtual void AddRect(CWnd *pWnd);
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	StrokeList &GetList(){return m_list;};

	virtual void LockLayer(int nLayer);
	virtual void UnLockLayer(int nLayer);
	void FilterPoint(double dbPrecis, double dbAngle);
	virtual BOOL GetNodeFirstDot(CDot &dt);
	virtual BOOL GetNodeLastDot(CDot &dt);

	virtual void DrawExtern(XDC *pDC);

	virtual void JoinStroke(double dbMinDistance);

	public:
		virtual void GetStrokeInQuad(CQuad &quad, StrokeList &strokeList);
		virtual int  GetStrokeNum(int nFocusType);

		void SetResinRect(int nRect);

		void GetResinRect(int aResinRect[],int &nId);

		int m_nResinRect;

};

#endif // !defined(AFX_NODE_H__FF08A521_DF0A_11D3_845D_444553540001__INCLUDED_)
