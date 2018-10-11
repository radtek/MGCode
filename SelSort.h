// SelSort.h: interface for the CSelSort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SELSORT_H__6079E525_9C6F_47BB_946A_DB1FE9C0D494__INCLUDED_)
#define AFX_SELSORT_H__6079E525_9C6F_47BB_946A_DB1FE9C0D494__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSelSort  
{
public:
	CSelSort();
	virtual ~CSelSort();

public:
	DWORD  GetSort(CArray<GNODE,GNODE> &ObjList, CStroke *pStroke);
	BOOL   GetRectCrossDot(GNODE &gNode, CDot &dtS, CDot &dtE, CArray<GNODE,GNODE> &ObjList);
	BOOL   GetCrossDot(CDot &p1, CDot &p2, CDot &q1, CDot &q2, CDot &CrosDot);
	BOOL   IsCross(CDot&p1 , CDot& p2 , CDot& p3 , CDot& p4 );
	double Distance(CDot &sDot, CDot &eDot);


public:
	BOOL Track(CWnd* pWnd, CArray<GNODE,GNODE> &ObjList);
	void Adjust(MSG msg, CPoint pointOld,CPoint &pointNew);

};

#endif // !defined(AFX_SELSORT_H__6079E525_9C6F_47BB_946A_DB1FE9C0D494__INCLUDED_)
