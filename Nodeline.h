// NodeLine.h: interface for the CNodeLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODELINE_H__9E292CE1_E0D9_11D3_845D_444553540001__INCLUDED_)
#define AFX_NODELINE_H__9E292CE1_E0D9_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Dot.h"
#include "Stroke.h"

class CNodeLine : public CNode
{
friend class CPlt;
public:	
	virtual void DelRect(CWnd *pWnd);
	virtual void AddRect(CWnd *pWnd);
	void Adjust(MSG msg, CPoint pointOld,CPoint &pointNew);
	virtual void Adjust(MSG &msg,const CPoint &ptOrg);
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Serialize(FILE* fp,BOOL bFlag);
	CNodeLine();
	virtual ~CNodeLine();
	CNodeLine(CStroke *pStroke);
};

#endif // !defined(AFX_NODELINE_H__9E292CE1_E0D9_11D3_845D_444553540001__INCLUDED_)
