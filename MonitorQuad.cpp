// MonitorQuad.cpp: implementation of the CMonitorQuad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "MonitorQuad.h"
#include "Chain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMonitorQuad::CMonitorQuad()
{

}

CMonitorQuad::CMonitorQuad(CChain *pChain)
{
	m_pChain = pChain;
}

CMonitorQuad::~CMonitorQuad()
{

}

void CMonitorQuad::DoQuadCmd(int nType, CArray<SELQUAD, SELQUAD> &quadList)
{
	switch(nType)
	{
	case CmdEqualWidth:
		EqualWidth(quadList);
		break;
	case CmdEqualHeight:
		EqualHeight(quadList);
		break;
	case CmdEqualWh:
		EqualWh(quadList);
		break;
	case CmdDistanceX:
		DistanceX(quadList);
		break;
	case CmdDistanceY:
		DistanceY(quadList);
		break;
	case CmdAlignLeft:
		AlignLeft(quadList);
		break;
	case CmdAlignRight:
		AlignRight(quadList);
		break;
	case CmdAlignTop:
		AlignTop(quadList);
		break;
	case CmdAlignBottom:
		AlignBottom(quadList);
		break;
	default:
		return;
	}
	m_pChain->AjustTreeForQuad(quadList);
}

void CMonitorQuad::EqualWidth(CArray<SELQUAD, SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	if (count == 0) return;
	SELQUAD selQuad;
	int j=0;
	for (j=0; j<count; j++)
	{
		selQuad = quadList.GetAt(j);
		if (selQuad.m_bFill) break;
	}
	double width = selQuad.m_QD.Width();
	for (int i=0; i<count; i++)
	{
		if (i==j) continue;
		SELQUAD& quad = quadList.ElementAt(i);
		double midX = (quad.m_QD.left + quad.m_QD.right)/2;
		quad.m_QD.left = midX - width/2;
		quad.m_QD.right = midX + width/2;
		quad.m_QD.Normal();
	}
}

void CMonitorQuad::EqualHeight(CArray<SELQUAD, SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	if (count == 0) return;
	SELQUAD selQuad;
	int j=0;
	for (j=0; j<count; j++)
	{
		selQuad = quadList.GetAt(j);
		if (selQuad.m_bFill) break;
	}
	double height = selQuad.m_QD.Height();
	for (int i=0; i<count; i++)
	{
		if (i==j) continue;
		SELQUAD& quad = quadList.ElementAt(i);
		double midY = (quad.m_QD.top + quad.m_QD.bottom)/2;
		quad.m_QD.bottom = midY - height/2;
		quad.m_QD.top = midY + height/2;
		quad.m_QD.Normal();
	}
}

void CMonitorQuad::EqualWh(CArray<SELQUAD, SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	if (count == 0) return;
	SELQUAD selQuad;
	int j=0;
	for (j=0; j<count; j++)
	{
		selQuad = quadList.GetAt(j);
		if (selQuad.m_bFill) break;
	}
	double width = selQuad.m_QD.Width();
	double height = selQuad.m_QD.Height();
	for (int i=0; i<count; i++)
	{
		if (i==j) continue;
		SELQUAD& quad = quadList.ElementAt(i);
		double midX = (quad.m_QD.left + quad.m_QD.right)/2;
		quad.m_QD.left = midX - width/2;
		quad.m_QD.right = midX + width/2;
		double midY = (quad.m_QD.top + quad.m_QD.bottom)/2;
		quad.m_QD.bottom = midY - height/2;
		quad.m_QD.top = midY + height/2;
		quad.m_QD.Normal();
	}
}

void CMonitorQuad::DistanceX(CArray<SELQUAD, SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	if (count < 3) return;

	for (int j=count-1; j>0; j--)
	{
		for (int i=0; i<j; i++)
		{
			SELQUAD &quad1 = quadList.ElementAt(i);
			SELQUAD &quad2 = quadList.ElementAt(i+1);
			double mid1 = (quad1.m_QD.left+quad1.m_QD.right)/2;
			double mid2 = (quad2.m_QD.left+quad2.m_QD.right)/2;
			if (mid1 > mid2)
			{
				SELQUAD selQuad = quad1;
				quad1 = quad2;
				quad2 = selQuad;
			}
		}
	}
	SELQUAD &minQuad = quadList.ElementAt(0);
	SELQUAD &maxQuad = quadList.ElementAt(count-1);
	double minTemp = (minQuad.m_QD.left+minQuad.m_QD.right)/2;
	double maxTemp = (maxQuad.m_QD.left+maxQuad.m_QD.right)/2;
	double dist = (maxTemp - minTemp)/(count-1);
	for (int i=1; i<count-1; i++)
	{
		SELQUAD &quad = quadList.ElementAt(i);
		double moveX = minTemp + i*dist - (quad.m_QD.left+quad.m_QD.right)/2;
		quad.m_QD.left += moveX;
		quad.m_QD.right += moveX;
	}
}

void CMonitorQuad::DistanceY(CArray<SELQUAD, SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	if (count < 3) return;
	
	for (int j=count-1; j>0; j--)
	{
		for (int i=0; i<j; i++)
		{
			SELQUAD &quad1 = quadList.ElementAt(i);
			SELQUAD &quad2 = quadList.ElementAt(i+1);
			double mid1 = (quad1.m_QD.top+quad1.m_QD.bottom)/2;
			double mid2 = (quad2.m_QD.top+quad2.m_QD.bottom)/2;
			if (mid1 > mid2)
			{
				SELQUAD selQuad = quad1;
				quad1 = quad2;
				quad2 = selQuad;
			}
		}
	}
	SELQUAD &minQuad = quadList.ElementAt(0);
	SELQUAD &maxQuad = quadList.ElementAt(count-1);
	double minTemp = (minQuad.m_QD.top+minQuad.m_QD.bottom)/2;
	double maxTemp = (maxQuad.m_QD.top+maxQuad.m_QD.bottom)/2;
	double dist = (maxTemp - minTemp)/(count-1);
	for (int i=1; i<count-1; i++)
	{
		SELQUAD &quad = quadList.ElementAt(i);
		double moveY = minTemp + i*dist - (quad.m_QD.top+quad.m_QD.bottom)/2;
		quad.m_QD.top += moveY;
		quad.m_QD.bottom += moveY;
	}
}

void CMonitorQuad::AlignLeft(CArray<SELQUAD, SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	if (count == 0) return;
	SELQUAD selQuad;
	int j=0;
	for (j=0; j<count; j++)
	{
		selQuad = quadList.GetAt(j);
		if (selQuad.m_bFill) break;
	}
	for (int i=0; i<count; i++)
	{
		if (i==j) continue;
		SELQUAD& quad = quadList.ElementAt(i);
		double moveX = selQuad.m_QD.left - quad.m_QD.left;
		quad.m_QD.left += moveX;
		quad.m_QD.right += moveX;
	}
}

void CMonitorQuad::AlignRight(CArray<SELQUAD, SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	if (count == 0) return;
	SELQUAD selQuad;
	int j=0;
	for (j=0; j<count; j++)
	{
		selQuad = quadList.GetAt(j);
		if (selQuad.m_bFill) break;
	}
	for (int i=0; i<count; i++)
	{
		if (i==j) continue;
		SELQUAD& quad = quadList.ElementAt(i);
		double moveX = selQuad.m_QD.right - quad.m_QD.right;
		quad.m_QD.left += moveX;
		quad.m_QD.right += moveX;
	}
}

void CMonitorQuad::AlignTop(CArray<SELQUAD, SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	if (count == 0) return;
	SELQUAD selQuad;
	int j=0;
	for (j=0; j<count; j++)
	{
		selQuad = quadList.GetAt(j);
		if (selQuad.m_bFill) break;
	}
	for (int i=0; i<count; i++)
	{
		if (i==j) continue;
		SELQUAD& quad = quadList.ElementAt(i);
		double moveY = selQuad.m_QD.top - quad.m_QD.top;
		quad.m_QD.top += moveY;
		quad.m_QD.bottom += moveY;
	}
}

void CMonitorQuad::AlignBottom(CArray<SELQUAD, SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	if (count == 0) return;
	SELQUAD selQuad;
	int j=0;
	for (j=0; j<count; j++)
	{
		selQuad = quadList.GetAt(j);
		if (selQuad.m_bFill) break;
	}
	for (int i=0; i<count; i++)
	{
		if (i==j) continue;
		SELQUAD& quad = quadList.ElementAt(i);
		double moveY = selQuad.m_QD.bottom - quad.m_QD.bottom;
		quad.m_QD.top += moveY;
		quad.m_QD.bottom += moveY;
	}
}

void CMonitorQuad::SetChain(CChain *pChain)
{
	m_pChain = pChain;
}