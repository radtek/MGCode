// OptStrokeList.h: interface for the COptStrokeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTSTROKELIST_H__4EDC974A_D229_4945_9E83_71547B898772__INCLUDED_)
#define AFX_OPTSTROKELIST_H__4EDC974A_D229_4945_9E83_71547B898772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stroke.h"

class COptStrokeList  
{
public:
	////部分优化函数
	void PartOptInsertStroke(CStroke* pStroke);
	void PartOptEmptyStrokeList(BOOL bDestroy=FALSE);

	////完全优化函数
	void FullOpt();

	////
	void Detach();
	void Attach(StrokeList* pListSrc,double dbOptDist=0.001,BOOL bFull=TRUE);
	COptStrokeList();
	virtual ~COptStrokeList();

private:
	void InsertStrokeIntoList(CStroke* pStroke);
	void InsertStrokeIntoList2(CStroke* pStroke);

	////
	BOOL m_bOptFull;//表示是否完全优化(完全优化，则表示对m_pListSrc中线条进行优化处理)
	double m_dbOptDist;//优化间距
	StrokeList* m_pListSrc;//待优化的线条链
};

#endif // !defined(AFX_OPTSTROKELIST_H__4EDC974A_D229_4945_9E83_71547B898772__INCLUDED_)
