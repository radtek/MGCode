// NodeEllipse.h: interface for the CNodeEllipse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEELLIPSE_H__7C4826C1_3F94_11D4_B789_FAB90A0E980F__INCLUDED_)
#define AFX_NODEELLIPSE_H__7C4826C1_3F94_11D4_B789_FAB90A0E980F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Dot.h"

class CNodeEllipse : public CNode  
{
public:
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	void Serialize(FILE* fp,BOOL bFlag);
	void SerializePoint(FILE *fp, BOOL bFlag);
//	virtual void MarkPreview(CDC *pDC, const int &nPreviewSpeed);
	BOOL Create(CDot dt1,CDot dt2,DOT Center,MSG msg);
	CNodeEllipse();
	virtual ~CNodeEllipse();
	int m_nCount;
private:
	CDot m_dotArray[13];
	CStroke *m_pStrkTemp;
public:
	void CtrlPointToPoint();
	void PointToCtrPoint();
	void Mark(HWND hWnd, int nFocusType);
	virtual void SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsResMark);
	virtual void SetLayColor(int nLayer,COLORREF color,int nLayerFill=0,COLORREF colorFill=0);
};

#endif // !defined(AFX_NODEELLIPSE_H__7C4826C1_3F94_11D4_B789_FAB90A0E980F__INCLUDED_)
