// NodeGrid.h: interface for the CNodeGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEGRID_H__0911EE61_50C0_11D4_9563_00105A6155CA__INCLUDED_)
#define AFX_NODEGRID_H__0911EE61_50C0_11D4_9563_00105A6155CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"

class CNodeGrid : public CNode  
{
public:
	virtual BOOL Create(CQuad &quad);
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Serialize(FILE* fp,BOOL bFlag);
	CNodeGrid();
	virtual ~CNodeGrid();

private:
	int m_nCol;
	int m_nRow;
};

#endif // !defined(AFX_NODEGRID_H__0911EE61_50C0_11D4_9563_00105A6155CA__INCLUDED_)
