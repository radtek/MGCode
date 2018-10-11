// XDC.cpp: implementation of the XDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XDC.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_SCALE	1024.0/20000
#define MIN_SCALE	1024.0/0.004

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XDC::XDC()
{
	m_dbScale=1.0;
	m_dbPhysicX=m_dbPhysicY=0.0;
	m_dx=m_dy=0.0;
}

XDC::~XDC()
{

}

void XDC::LogicToPhysic(double &x,double &y)
{
	x=(x-m_ptCenter.x)/m_dbScale+m_dbPhysicX;
	y=m_dbPhysicY-(y-m_ptCenter.y)/m_dbScale;
}

void XDC::PhysicToLogic(double &x,double &y)
{
	x=m_ptCenter.x+m_dbScale*(x-m_dbPhysicX);
	y=m_ptCenter.y-m_dbScale*(y-m_dbPhysicY);
}

double XDC::LenLogicToPhysic(double& dbVal)
{
	double t;
	t=dbVal/m_dbScale;
	dbVal = t;
	return t;
}

double XDC::LenPhysicToLogic(double& dbVal)
{
	double t;
	t=dbVal*m_dbScale;
	dbVal = t;
	return t;
}

void XDC::MoveTo(double x,double y)
{
	m_dbPreX = x;
	m_dbPreY = y;
}

void XDC::LineTo(double x,double y)
{
	double x1,y1,x2,y2;
	x1 = m_dbPreX;
	y1 = m_dbPreY;
	x2 = x;
	y2 = y;

	if(IsDraw(x1,y1,x2,y2))
	{
		PhysicToLogic(x1,y1);
		PhysicToLogic(x2,y2);
		m_pDC->MoveTo((int)x1,(int)y1);
		m_pDC->LineTo((int)x2,(int)y2);
	}

	m_dbPreX = x;
	m_dbPreY = y;
}

BOOL XDC::IsPrinting()
{
	return m_pDC->IsPrinting();
}

int XDC::GetClipBox( LPRECT lpRect )
{
	return m_pDC->GetClipBox(lpRect);
}

COLORREF XDC::SetPixel( int x, int y, COLORREF crColor )
{
	return m_pDC->SetPixel(x,y,crColor);
}

CPen* XDC::SelectObject( CPen* pPen )
{
	return m_pDC->SelectObject(pPen);
}

CBrush* XDC::SelectObject( CBrush* pBrush )
{
	return m_pDC->SelectObject(pBrush);
}

int XDC::SetROP2( int nDrawMode )
{
	return m_pDC->SetROP2(nDrawMode);
}

HGDIOBJ XDC::SelectObject(HGDIOBJ hObject)
{
	return m_pDC->SelectObject(hObject);
}

BOOL XDC::IsInRect(double x, double y)
{
	return ((x<=m_dbMaxX) && (x>=m_dbMinX) && (y>=m_dbMinY) &&(y<=m_dbMaxY));
}

void XDC::SetViewRect(double dbViewWid,double dbViewHei)
{
	m_dbViewX = dbViewWid;
	m_dbViewY = dbViewHei;
}

void XDC::CalPhyRect(double dbViewWid,double dbViewHei)
{
	m_dbMaxX= dbViewWid+100;
	m_dbMaxY= -100;
	m_dbMinX= -100;
	m_dbMinY= dbViewHei+100;

	LogicToPhysic(m_dbMaxX,m_dbMaxY);
	LogicToPhysic(m_dbMinX,m_dbMinY);
}

BOOL XDC::IsDraw(double& x1,double &y1,double& x2,double& y2)
{
	CalPhyRect(m_dbViewX,m_dbViewY);

	//两点都在VIEW实际坐标矩形内
	if(IsInRect(x1,y1)&& IsInRect(x2,y2))
		return TRUE;
	//求交点个数
	if(FindInterSectNum(x1,y1,x2,y2))
	{
		if((fabs(x1-x2)<1e-12)&&(fabs(y1-y2)<1e-12))
			return FALSE;
		return TRUE;
	}
	else
		return FALSE;
}

int XDC::FindInterSectNum(double &x1,double &y1,double &x2,double &y2)
{
	//一个点在矩形内，一定有个交点
	if(IsInRect(x1,y1)||IsInRect(x2,y2))
	{	
		if(IsInRect(x1,y1))
		{
			//判断斜率是否为无穷大
			if(fabs(x1-x2)<1e-12)
			{
				if((m_dbMaxY<=y2) && (m_dbMaxY>=y1))
					y2=m_dbMaxY;
				else if((m_dbMinY<=y2) && (m_dbMinY>=y1))
					y2=m_dbMinY;
			}
			else if(fabs(y1-y2)<1e-12)//判断斜率是否为0
			{
				if((m_dbMaxX<=x2) && (m_dbMaxX>=x1))
					x2=m_dbMaxX;
				else if((m_dbMinX<=x2) && (m_dbMinX>=x1))
					x2=m_dbMinY;
			}
			else 
			{
				double dx1,dy1,dx2,dy2,dx3,dy3,dx4,dy4;
				int nPoint=0;
				
				dx1=m_dbMinX;
				dy1=y1+(y2-y1)*(dx1-x1)/(x2-x1);
				dx2=m_dbMaxX;
				dy2=y1+(y2-y1)*(dx2-x1)/(x2-x1);
				
				dy3=m_dbMinY;
				dx3=x1+(dy3-y1)*(x2-x1)/(y2-y1);
				dy4=m_dbMaxY;
				dx4=x1+(dy4-y1)*(x2-x1)/(y2-y1);
				if(IsInRect(dx1,dy1))
				{
					if(IsLine(x1,y1,x2,y2,dx1,dy1))
					{
						x2=dx1;
						y2=dy1;
					}
				}
				else if(IsInRect(dx2,dy2))
				{
					if(IsLine(x1,y1,x2,y2,dx2,dy2))
					{
						x2=dx2;
						y2=dy2;
					}
				}
				else if(IsInRect(dx3,dy3))
				{
					if(IsLine(x1,y1,x2,y2,dx3,dy3))
					{
						x2=dx3;
						y2=dy3;
					}
				}
				else if(IsInRect(dx4,dy4))
				{
					if(IsLine(x1,y1,x2,y2,dx4,dy4))
					{
						x2=dx4;
						y2=dy4;
					}
				}
			}			
		}
		else if(IsInRect(x2,y2))
		{
			double t1,t2;
			t1 = x1;
			t2 = y1;
			x1 = x2;
			y1 = y2;
			x2 = t1;
			y2 = t2;

			//判断斜率是否为无穷大
			if(fabs(x1-x2)<1e-12)
			{
				if((m_dbMaxY<=y2) && (m_dbMaxY>=y1))
					y2=m_dbMaxY;
				else if((m_dbMinY<=y2) && (m_dbMinY>=y1))
					y2=m_dbMinY;
			}
			else if(fabs(y1-y2)<1e-12)//判断斜率是否为0
			{
				if((m_dbMaxX<=x2) && (m_dbMaxX>=x1))
					x2=m_dbMaxX;
				else if((m_dbMinX<=x2) && (m_dbMinX>=x1))
					x2=m_dbMinY;
			}
			else 
			{
				double dx1,dy1,dx2,dy2,dx3,dy3,dx4,dy4;
				int nPoint=0;
				
				dx1=m_dbMinX;
				dy1=y1+(y2-y1)*(dx1-x1)/(x2-x1);
				dx2=m_dbMaxX;
				dy2=y1+(y2-y1)*(dx2-x1)/(x2-x1);
				
				dy3=m_dbMinY;
				dx3=x1+(dy3-y1)*(x2-x1)/(y2-y1);
				dy4=m_dbMaxY;
				dx4=x1+(dy4-y1)*(x2-x1)/(y2-y1);
				if(IsInRect(dx1,dy1))
				{
					if(IsLine(x1,y1,x2,y2,dx1,dy1))
					{
						x2=dx1;
						y2=dy1;
					}
				}
				else if(IsInRect(dx2,dy2))
				{
					if(IsLine(x1,y1,x2,y2,dx2,dy2))
					{
						x2=dx2;
						y2=dy2;
					}
				}
				else if(IsInRect(dx3,dy3))
				{
					if(IsLine(x1,y1,x2,y2,dx3,dy3))
					{
						x2=dx3;
						y2=dy3;
					}
				}
				else if(IsInRect(dx4,dy4))
				{
					if(IsLine(x1,y1,x2,y2,dx4,dy4))
					{
						x2=dx4;
						y2=dy4;
					}
				}
			}
			t1 = x1;
			t2 = y1;
			x1 = x2;
			y1 = y2;
			x2 = t1;
			y2 = t2;
		}
		return 1;
	}

	if(fabs(x1-x2)<1e-12)
	{
		//判断斜率是否为无穷大
		double minY,maxY;
		minY=min(y1,y2);
		maxY=max(y1,y2);
		if((x1<m_dbMaxX)&&(x1>m_dbMinX)&&((m_dbMinY>minY)&&(m_dbMinY<maxY)))
		{
			y1 = m_dbMaxY;
			y2 = m_dbMinY;
			return 2;
		}
	}
	else if(fabs(y1-y2)<1e-12)
	{
		//判断斜率是否为0
		double min,max;
		min=min(x1,x2);
		max=max(x1,x2);
		if((y1<m_dbMaxY)&&(y1>m_dbMinY)&&((m_dbMinX>min)&&(m_dbMinX<max)))
		{
			x1 = m_dbMinX;
			x2 = m_dbMaxX;
			return 2;
		}
	}
	else 
	{
		//斜率为非无穷大时计算两直线的交点
		double dx1,dy1,dx2,dy2,dx3,dy3,dx4,dy4;
		double px[4],py[4];
		int nPoint=0;

		dx1=m_dbMinX;
		dy1=y1+(y2-y1)*(dx1-x1)/(x2-x1);
		dx2=m_dbMaxX;
		dy2=y1+(y2-y1)*(dx2-x1)/(x2-x1);

		dy3=m_dbMinY;
		dx3=x1+(dy3-y1)*(x2-x1)/(y2-y1);
		dy4=m_dbMaxY;
		dx4=x1+(dy4-y1)*(x2-x1)/(y2-y1);

		if(IsInRect(dx1,dy1))
		{
			if(IsLine(x1,y1,x2,y2,dx1,dy1))
			{
				px[nPoint]=dx1;
				py[nPoint]=dy1;
				nPoint++;
			}
		}
		if(IsInRect(dx2,dy2))
		{
			if(IsLine(x1,y1,x2,y2,dx2,dy2))
			{
				px[nPoint]=dx2;
				py[nPoint]=dy2;
				nPoint++;
			}
		}
		if(IsInRect(dx3,dy3))
		{
			if(IsLine(x1,y1,x2,y2,dx3,dy3))
			{
				px[nPoint]=dx3;
				py[nPoint]=dy3;
				nPoint++;
			}
		}
		if(IsInRect(dx4,dy4))
		{
			if(IsLine(x1,y1,x2,y2,dx4,dy4))
			{
				px[nPoint]=dx4;
				py[nPoint]=dy4;
				nPoint++;
			}
		}
		if(nPoint==1)
		{
			AfxMessageBox("Here");
		}
		if(nPoint==3)
		{
			AfxMessageBox("Here3");
		}
		if(nPoint==2)
		{
			x1=px[0];
			x2=px[1];
			y1=py[0];
			y2=py[1];
			return nPoint;
		}
	}
	return 0;
}


void XDC::SetScale(double dbVal)
{
	if(dbVal>MIN_SCALE)
		dbVal = MIN_SCALE;
	if(dbVal<MAX_SCALE)
		dbVal = MAX_SCALE;
	m_dbScale=dbVal;
}

BOOL XDC::IsLine(double x1, double y1, double x2, double y2, double x, double y)
{
	double minX=min(x1,x2);
	double minY=min(y1,y2);
	double maxX=max(x1,x2);
	double maxY=max(y1,y2);
	return ((x>minX)&&(x<maxX)&&(y>minY)&&(y<maxY));
}

void XDC::FillSolidQuad(CQuad &quad, COLORREF cor)
{
	CQuad quadTemp=quad;
	PhysicToLogic(quadTemp.left,quadTemp.top);
	PhysicToLogic(quadTemp.right,quadTemp.bottom);
	CRect rect=(CRect)quadTemp;
	m_pDC->FillSolidRect(&rect,cor);
}

void XDC::Ellipse(CQuad &quad)
{
	quad.Normal();
	double dbLen=(quad.right-quad.left)/2;
	double dbWid=(quad.top-quad.bottom)/2;
	double dbAraf,dbR;
	CDot dtCenter,dtNew,dtOld;
	dtCenter.x=(quad.right+quad.left)/2;
	dtCenter.y=(quad.top+quad.bottom)/2;
	int nCount =100;
	dbAraf=0;
	dbR=dbLen*dbWid*sqrt(1.0/(dbLen*dbLen*cos(dbAraf)*cos(dbAraf)+dbWid*dbWid*sin(dbAraf)*sin(dbAraf)));
	dtOld.x=dbR*cos(dbAraf);
	dtOld.y=dbR*sin(dbAraf);
	dtOld.x+=dtCenter.x;
	dtOld.y+=dtCenter.y;
	PhysicToLogic(dtOld.x,dtOld.y);
	for(int i=1;i<=nCount;i++)
	{
		dbAraf=2*PAI*i/nCount;
		dbR=dbLen*dbWid*sqrt(1.0/(dbLen*dbLen*cos(dbAraf)*cos(dbAraf)+dbWid*dbWid*sin(dbAraf)*sin(dbAraf)));
		dtNew.x=dbR*cos(dbAraf);
		dtNew.y=dbR*sin(dbAraf);
		dtNew.x+=dtCenter.x;
		dtNew.y+=dtCenter.y;
		PhysicToLogic(dtNew.x,dtNew.y);
		m_pDC->MoveTo((int)dtOld.x,(int)dtOld.y);
		m_pDC->LineTo((int)dtNew.x,(int)dtNew.y);
		dtOld=dtNew;
	}
}

void XDC::SetPixelV(int x, int y, COLORREF crColor)
{
	m_pDC->SetPixelV(x, y, crColor);
}
