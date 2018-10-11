// SortListObj.h: interface for the CSortListObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTLISTOBJ_H__704F20F8_39FD_4623_BB1D_2FE38E9EC588__INCLUDED_)
#define AFX_SORTLISTOBJ_H__704F20F8_39FD_4623_BB1D_2FE38E9EC588__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSortListObj  
{
public:
	CSortListObj();
	virtual ~CSortListObj();

public:
	BOOL Track(CWnd* pView);
	BOOL InitSortPro();
private:
	BOOL IsDotInSortList(CPoint &point, CWnd *pWnd, CArray<SORTNODE,SORTNODE>  &slist, int &nRetIndex);
	BOOL SortCurList(CArray<SORTNODE,SORTNODE> &slist, int nIndex);

	BOOL DrawSRect(CArray<SORTNODE,SORTNODE> &slist, int nIndex, BOOL bDraw);

private:
	int m_nCurIndex;
};

#endif // !defined(AFX_SORTLISTOBJ_H__704F20F8_39FD_4623_BB1D_2FE38E9EC588__INCLUDED_)
