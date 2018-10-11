// DotB.cpp: implementation of the CDot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dot.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CDot::CDot()
{
	m_bScale = FALSE;
	x = y = z = 0;
}


CDot::~CDot()
{

}


CDot::CDot(CPoint &pt)
{
	x=pt.x;
	y=pt.y;
	z=0;
}

CDot::CDot(POINT pt)
{
	x=pt.x;
	y=pt.y;
	z=0;
}

CDot::CDot(double xValue, double yValue)
{
	x=xValue;
	y=yValue;
	z=0;
}

void CDot::operator =(const DOT & src)
{
	x=src.x;
	y=src.y;
	z=src.z;
}

void CDot::operator =(const CDot & src)
{
	x=src.x;
	y=src.y;
	z=src.z;
}

BOOL CDot::operator ==(const CDot & src)
{
	if((fabs(x - src.x) < EPSINON) && (fabs(y - src.y) < EPSINON))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CDot::operator *=(double mul)
{
	x*=mul;
	y*=mul;
}

CDot::operator CPoint()
{
	CPoint pt;
	pt.x=(int)x;
	pt.y=(int)y;
	return pt;
}
void CDot::XChangeY()
{
	double tx;
	tx = x;
	x = y;
	y = tx;
}
void CDot::Scale(CDot center,double ratioX, double ratioY)
{
	if (fabs(ratioX) < EPSINON) ratioX = 0.000000001;
	if (fabs(ratioY) < EPSINON) ratioY = 0.000000001;
	
	double tx(0),ty(0);
	tx=x-center.x;
	x=ratioX*tx+center.x;

	ty=y-center.y;
	y=ratioY*ty+center.y;

	return;
}

void CDot::SetOffset(double xf,double yf)
{
	x+=xf;
	y+=yf;
}

void CDot::Move(double moveX, double moveY)
{
	x+=moveX;
	y+=moveY;
}
void CDot::LogToTrue()
{
	x=(x-LCENTER)/DPMM;
	y=(LCENTER-y)/DPMM;
}
void CDot::TrueToLog()
{
	x=x*DPMM+LCENTER;
	y=LCENTER-y*DPMM;

}

void CDot::Rotate(CDot center, double angle)
{
	double dbx,dby,r,Araf;
	dbx=x-center.x;
	dby=y-center.y;
	r=sqrt(dbx*dbx+dby*dby);
	if(dbx!=0)
	{
		Araf=atan(dby/dbx);
		if(dbx<0)
			Araf+=PAI;
		
	}
	else
	{
		Araf=PAI/2;
		if(dby<0)
			Araf=-PAI/2;
	}
	Araf+=angle*PAI/180;
	x=center.x+r*cos(Araf);
	y=center.y+r*sin(Araf);
}


/*****************************************************************************
  描述: 判断指定点是否在线段上
  参数: 　p1 --- 线段起点　p2 --- 线段终点
  返回值  在线段上 返回TRUE 否则返回FALSE
******************************************************************************/
BOOL CDot::IsDotInLine(CDot& p1 , CDot& p2 )
{
	 // 判断点是否在外接矩形范围内 
	 if ( x < min(p1.x ,p2.x) || x > max(p1.x ,p2.x)
	  || y <  min(p1.y ,p2.y) || y > max(p1.y ,p2.y))
	 {
		 return FALSE;
	 }

     //计算叉积  
	 double dblMulti = (double)((x - p1.x) * (p2.y - p1.y) - (p2.x - p1.x) * (y - p1.y));	 
     if ( fabs(dblMulti) < EPSINON )
	 {
		 return TRUE;
	 }
	 else
	 {
		 return FALSE;
	 }

}  

//点→点 距离
double CDot::DistanceFromDot(CDot dot)
{
	return pow(pow(x-dot.x,2)+pow(y-dot.y,2),0.5);

}

void CDot::Round(int digit)
{
	double temp1 = pow(double(10), digit);
	
	if (x>=0)
		x = int((x*temp1+0.5))/temp1;
	else
		x = -int((-x*temp1+0.5))/temp1;

	if (y>=0)
		y = int((y*temp1+0.5))/temp1;
	else
		y = -int((-y*temp1+0.5))/temp1;
}

CDot CDot::Center(const CDot &dot1, const CDot &dot2)
{
	CDot dotCenter;
	dotCenter.x = (dot1.x + dot2.x)/2;
	dotCenter.y = (dot1.y + dot2.y)/2;
	return dotCenter;
}