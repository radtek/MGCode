// NodeRect.h: interface for the CNodeRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODERECT_H__9E292CE2_E0D9_11D3_845D_444553540001__INCLUDED_)
#define AFX_NODERECT_H__9E292CE2_E0D9_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"

class CNodeRect : public CNode  
{
public:
	void AddCircleStroke(CStroke *pStroke,CDot CenterDot,double dbR, double dbStartAngle,double dbEndAngle,BOOL bPreview=FALSE);//zg add
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
//	virtual BOOL Create(CRect &rect,int nMode=0,double dbValue=0.0,BOOL bPreview=FALSE);//zg add
	virtual void Serialize(FILE* fp,BOOL bFlag);
	virtual BOOL Create(CDot pt1,CDot pt2);
	void TranslateDot(double &x,double &y,double a,double x0,double y0);
	CNodeRect();
	virtual ~CNodeRect();

};

#endif // !defined(AFX_NODERECT_H__9E292CE2_E0D9_11D3_845D_444553540001__INCLUDED_)
