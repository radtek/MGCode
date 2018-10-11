// Quad.cpp: implementation of the CQuad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "Quad.h"
#include "Stroke.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuad::CQuad()
{
	left	= 0.0;
	right	= 0.0;
	top		= 0.0;
	bottom	= 0.0;
}

CQuad::~CQuad()
{

}

CQuad::CQuad(RECT rect)
{
	left=rect.left;
	right=rect.right;
	top=rect.top;
	bottom=rect.bottom;
}

CQuad::CQuad(double l,double t,double r,double b)
{
   	left  = l;
	right = r;
	top   = t;
	bottom= b;

	return;
}

CQuad::CQuad(CDot center,double width,double height)
{
	left	=center.x - width / 2;
	right	=center.x + width / 2;
	top		=center.y + height/ 2;
	bottom	=center.y - height/ 2;

	return;
}

CQuad::CQuad(CDot dot1,CDot dot2)
{
	left	= min(dot1.x,dot2.x);
	right	= max(dot1.x,dot2.x);
	bottom	= min(dot1.y,dot2.y);
	top		= max(dot1.y,dot2.y);

	return;
}

CQuad::CQuad(int base,CDot dot,double width,double height)
{
	CDot center;
	switch(base)
	{
	case BASE_BL:
		{
			center.x = dot.x + width  / 2;
		    center.y = dot.y + height / 2;
		    break;
		}
	case BASE_BM:
		{
			center.x = dot.x;
		    center.y = dot.y + height / 2;
		    break;
		}
	case BASE_BR:
		{
			center.x = dot.x - width / 2;
		    center.y = dot.y + height/ 2;
		    break;
		}
	case BASE_ML:
		{
			center.x = dot.x + width / 2;
		    center.y = dot.y;
		    break;
		}
	case BASE_MM:
		{
			center.x = dot.x;
		    center.y = dot.y;
		    break;
		}
	case BASE_MR:
		{
			center.x = dot.x - width / 2;
		    center.y = dot.y;
		    break;
		}
	case BASE_TL:
		{
			center.x = dot.x + width / 2;
		    center.y = dot.y - height/ 2;
		    break;
		}
	case BASE_TM:
		{
			center.x = dot.x;
		    center.y = dot.y - height / 2;
		    break;
		}
	case BASE_TR:
		{
			center.x = dot.x - width/2;
		    center.y = dot.y - height/2;
		    break;
		}
	default:
		{
			break;
		}
	}

	left	= center.x - width  / 2;
	right	= center.x + width  / 2;
	top		= center.y + height / 2;
	bottom	= center.y - height / 2;

	return;
}

CQuad::operator CRect()
{
	CRect rect;
	rect.left	= (int)left;
	rect.right	= (int)right;
	rect.bottom	= (int)bottom;
	rect.top	= (int)top;

	return rect;
}

void CQuad::operator=(const CRect& src)
{  	
	left	= src.left;
	right	= src.right;
	top		= src.top;
	bottom	= src.bottom;

	return;
}

void CQuad::operator=(const CQuad& src)
{  	
	left	= src.left;
	right	= src.right;
	top		= src.top;
	bottom	= src.bottom;

	return;
}

void CQuad::operator*=(double ratio)
{  	
	CDot dot;
	dot		 = GetBaseDot();

	double width  = ratio * Width();
	double height = ratio * Height();

	left	= dot.x - width  / 2;
	right	= dot.x + width  / 2;
	top		= dot.y + height / 2;
	bottom	= dot.y - height / 2;

	return;
}	


void CQuad::Compose(CQuad &quad)
{
	if(    (fabs(left)   < EPSINON)
		&& (fabs(right)  < EPSINON)
		&& (fabs(top)    < EPSINON)
		&& (fabs(bottom) < EPSINON)
	  )
	{
		left  = quad.left;
		right = quad.right;
		top   = quad.top;
		bottom= quad.bottom;
	}
	else 
	{
		if(!quad.IsNull())
		{
			left  = left  < quad.left ?   left:quad.left;
			right = right > quad.right ?  right :quad.right;
			top   = top   > quad.top ?    top :quad.top;
			bottom= bottom< quad.bottom ? bottom:quad.bottom;
		}
	}

	return;
}


BOOL CQuad::Equal(CQuad quad)
{
	Normal();
	quad.Normal();

	if(  (fabs(left - quad.left)    < EPSINON)
	  && (fabs(right - quad.right)  < EPSINON)
	  && (fabs(bottom - quad.bottom)< EPSINON)
	  && (fabs(top - quad.top)      < EPSINON)
	  )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}


BOOL CQuad::Union(CQuad quad)
{
	if(IsEmpty())
	{
		left	= quad.left;
		right	= quad.right;
		top		= quad.top;
		bottom	= quad.bottom;
	}
	else
	{
		if(!quad.IsEmpty())
		{
			left	=min(left	,quad.left);
			right	=max(right	,quad.right);
			top		=max(top	,quad.top);
			bottom	=min(bottom	,quad.bottom);
		}
	}

	return !IsEmpty();
}


BOOL CQuad::Include(CPoint &pt,CWnd *pWnd)
{
	if (!pWnd)
	{
		return FALSE;
	}

	CHLView *pView = (CHLView*)pWnd;
	double dx = pt.x;
	double dy = pt.y;
	
	pView->LogicToPhysic(dx,dy);
	double x = 6;
	double y = 6;
	pView->ActualLen(x);
	pView->ActualLen(y);

	CQuad quad;
	quad.left  = left;
	quad.right = right;
	quad.top   = top;
	quad.bottom= bottom;
	quad.Inflate(x,y);

	if(    (dx >= quad.left   )
		&& (dx <= quad.right  )
		&& (dy >= quad.bottom )
		&& (dy <= quad.top    )
	  )
	{
		return TRUE;
	}

	return FALSE;
}

void CQuad::LogToTrue()
{
	CDot dot;
	dot.x = left;
	dot.y = top;
	dot.LogToTrue();
	
	left = dot.x;
	top  = dot.y;
	dot.x= right;
	dot.y= bottom;
	dot.LogToTrue();
	right =dot.x;
	bottom=dot.y;
	
	return;
}

void CQuad::TrueToLog()
{
	CDot dot;
	dot.x = left;
	dot.y = top;
	dot.TrueToLog();
	
	left  = dot.x;
	top   = dot.y;
	
	dot.x = right;
	dot.y = bottom;
	dot.TrueToLog();
	
	right = dot.x;
	bottom= dot.y;
	
	return;
}

void CQuad::Move(double &MoveX, double &MoveY)
{
	left += MoveX;
	right+= MoveY;
	
	return;
}

void CQuad::SetOffset(double x,double y)
{
	left   += x;
	right  += x;
	
	top    += y;
	bottom += y;
	
	return;
}

void CQuad::Scale(CDot &dot, double &ratioX, double &ratioY)
{
	double dx = left - dot.x;
	left = ratioX * dx + dot.x;
	
	double dy = right - dot.y;
	right = ratioY * dy + dot.x;
	
	return;
}

double CQuad::GetWid()
{
	return fabs(right-left);
}

double CQuad::GetLen()
{
	return fabs(right-left);
}

void CQuad::Initial()
{
	left = right = top = bottom = 0.0;
}

BOOL CQuad::Click(CDot dot,double dbR)
{
	//在设定精度下点取
	int x= (int)dbR;
	
	CQuad quad;
	quad.left  = left;
	quad.right = right;
	quad.top   = top;
	quad.bottom= bottom;
	quad.Normal();
	
	quad.left  = left - x;
	quad.right = right+ x;
	quad.top   = top  + x;
	quad.bottom= bottom-x;
	
	return quad.DotIn(dot);
}

BOOL CQuad::DotIn(CDot dot)
{
	Normal();
	
	if(    dot.x >= left 
		&& dot.x <= right 
		&& dot.y <= top 
		&& dot.y >= bottom
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CQuad::Inflate(double x, double y)
{
	left  -= x;
	right += x;
	top   += y;
	bottom-= y;
	
	return;
}

CDot CQuad::Center()
{
	CDot dot;
	dot.x = (left + right)  / 2;
	dot.y = (top  + bottom) / 2;
	return dot;
}

CDot CQuad::GetBaseDot(int base)
{
	CDot dot;
	switch(base)
	{
	case BASE_BL:
		{
			dot.x = left;
			dot.y = bottom;
			break;
		}
	case BASE_BM:
		{
			dot.x = (left + right) / 2;
			dot.y = bottom;
			break;
		}
	case BASE_BR:
		{
			dot.x = right;
			dot.y = bottom;
			break;
		}
	case BASE_ML:
		{
			dot.x = left;
			dot.y = (top + bottom) / 2;
			break;
		}
	case BASE_MM:
		{
			dot.x =(left + right) / 2;
			dot.y =(top  + bottom)/ 2;
			break;
		}
	case BASE_MR:
		{
			dot.x = right;
			dot.y = (top + bottom) / 2;
			break;
		}
	case BASE_TL:
		{
			dot.x = left;
			dot.y = top;
			break;
		}
	case BASE_TM:
		{
			dot.x = (left + right) / 2;
			dot.y = top;
			break;
		}
	case BASE_TR:
		{
			dot.x = right;
			dot.y = top;
			break;
		}
	}
	return dot;
}

BOOL CQuad::Include(CDot &dt)
{
	if (
		dt.x >= left 
		&& dt.x <= right 
		&& dt.y >= bottom 
		&& dt.y <= top
		)
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL CQuad::Include(CQuad quad)
{
	Normal();
	quad.Normal();
	
	if(    (left <= quad.left) 
		&& (right>=quad.right) 
		&& (bottom<=quad.bottom) 
		&& (top>=quad.top)
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

double CQuad::Height()
{
	return top - bottom;
}

double CQuad::Width()
{
	return right - left;
}

void CQuad::Normal()
{
	if(right < left)
	{
		double t = left;
		left  = right;
		right = t;
	}
	
	if(top < bottom)
	{
		double t = bottom;
		bottom   = top;
		top      = t;
	}
	
	return;
}

BOOL CQuad::IsEmpty()
{
	if(    ((fabs(left - right) < EPSINON)   && (fabs(top - bottom) < EPSINON)) 
		&& ((fabs(right - CENTER) < EPSINON) || (fabs(top - CENTER) < EPSINON)) 
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CQuad::IsNull()
{
	if(    (fabs(left)   < EPSINON) 
		&& (fabs(right)  < EPSINON) 
		&& (fabs(top)    < EPSINON) 
		&& (fabs(bottom) < EPSINON) 
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CQuad::SetEmpty()
{
	left=right=top=bottom=CENTER;
}