// OffsetReg.h: interface for the COffsetReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OFFSETREG_H__BDAAFC28_67EB_422D_80A7_5C402DB8DC0A__INCLUDED_)
#define AFX_OFFSETREG_H__BDAAFC28_67EB_422D_80A7_5C402DB8DC0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Quad.h"
#include "Stroke.h"
#include "Property.h"

class COffsetReg  
{
public:
	void Proc(StrokeList* plistData,StrokeList& listOffSetData);
	void DsfProc(StrokeList *plistData, StrokeList &listOffSetData, BOOL bOpt);
	COffsetReg(double dbOffset);
	virtual ~COffsetReg();

private:
	double m_dbOffset;		//偏移量
	StrokeList* m_pList;	//原始数据
};

#endif // !defined(AFX_OFFSETREG_H__BDAAFC28_67EB_422D_80A7_5C402DB8DC0A__INCLUDED_)
