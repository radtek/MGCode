// MonitorQuad.h: interface for the CMonitorQuad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORQUAD_H__ADD8D967_C2D4_4639_AE1C_0758BF95E5B2__INCLUDED_)
#define AFX_MONITORQUAD_H__ADD8D967_C2D4_4639_AE1C_0758BF95E5B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Chain.h"

class CMonitorQuad  
{
public:
	CMonitorQuad();
	CMonitorQuad::CMonitorQuad(CChain *pChain);
	virtual ~CMonitorQuad();
	void DoQuadCmd(int nType, CArray<SELQUAD, SELQUAD> &quadList);
	void SetChain(CChain *pChain);
protected:
	void EqualWidth(CArray<SELQUAD, SELQUAD> &quadList);
	void EqualHeight(CArray<SELQUAD, SELQUAD> &quadList);
	void EqualWh(CArray<SELQUAD, SELQUAD> &quadList);
	void DistanceX(CArray<SELQUAD, SELQUAD> &quadList);
	void DistanceY(CArray<SELQUAD, SELQUAD> &quadList);
	void AlignLeft(CArray<SELQUAD, SELQUAD> &quadList);
	void AlignRight(CArray<SELQUAD, SELQUAD> &quadList);
	void AlignTop(CArray<SELQUAD, SELQUAD> &quadList);
	void AlignBottom(CArray<SELQUAD, SELQUAD> &quadList);
private:
	CChain *m_pChain;
};

#endif // !defined(AFX_MONITORQUAD_H__ADD8D967_C2D4_4639_AE1C_0758BF95E5B2__INCLUDED_)
