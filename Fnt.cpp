// Fnt.cpp: implementation of the CFnt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "Fnt.h"
#include "XDC.h"
#include "HLView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFnt,CObject,2000)
CFnt::CFnt()
{	
	//初始化值
	m_dbHeight=5;
	m_dbLineSpace=0;
	m_dbCharSpace=0;
	m_dbWidthFactor=1;

	m_nPath=PATH_LINE;
	m_dbRadius=20;
	m_dbStart=0;
	m_dbDirection=0;
	m_bCCW=FALSE;
	m_dotBase=CDot(0,0);

	m_nAlign=ALIGN_LEFT;
	m_bInside=FALSE;
}

CFnt::~CFnt()
{
	Delete();
}

BOOL CFnt::CreateStrokeList()
{		
	Delete();
	
	//生成具体的STROKELIST
	if(MakeStroke()==FALSE)
		return FALSE;

	//得到初始时边框大小
	m_quadInit.SetEmpty();
	POSITION pos;	
	pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		CStroke *pStroke;
		pStroke=m_list.GetNext(pos);
		m_quadInit.Union(pStroke->CalLimit());
	}

	//变形成所需的形状
	Effect();
	
	//变为LOG方式
	pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		CStroke *pStroke;
		pStroke=m_list.GetNext(pos);
		pStroke->Move(m_dotBase.x,m_dotBase.y);
	}

	return TRUE;
}

void CFnt::Delete()
{
	POSITION prev = NULL;
	POSITION pos  = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos != NULL)
	{	
		prev=pos;
		pStroke=m_list.GetNext(pos);

		m_list.RemoveAt(prev);
		delete pStroke;
		pStroke = NULL;
	}

	return;
}

void CFnt::Rotate(CDot dot, double angle)
{
	m_dotBase.Rotate(dot,angle);
	
	m_dbDirection += angle;
	m_dbDirection  = fmod(m_dbDirection, 360);
	m_dbStart     += angle;
	m_dbStart      = fmod(m_dbStart, 360);

	return;
}
void CFnt::Move(double moveX, double moveY)
{
	m_dotBase.Move(moveX,moveY);
}

void CFnt::MoveStroke(double moveX, double moveY)
{
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		CStroke *pStroke;
		pStroke=m_list.GetNext(pos);
		pStroke->Move(moveX, moveY);
	}
}

void CFnt::Scale(CDot dot, double ratioX, double ratioY)
{
	if(m_nPath==PATH_ROUND)
	{
		if(ratioX==ratioY)
		{
			double r=fabs(ratioX);
			m_dbRadius*=r;
			m_dbHeight*=r;
		}
	}
	else
	{	
		int n=(int)(m_dbDirection/90);
		if(m_dbDirection==n*90)
		{
			m_dbHeight*=ratioY;
			m_dbWidthFactor*=(ratioX/ratioY);
		}
	}

	if ((ratioX > 0) && (ratioY > 0))
	{
		m_dotBase.Scale(dot,ratioX,ratioY);
	}
	
	m_dbHeight=MyPrecision(m_dbHeight);
	m_dbWidthFactor=MyPrecision(m_dbWidthFactor,5);
	m_dbRadius=MyPrecision(m_dbRadius);
}

void CFnt::Draw(XDC *pDC, BOOL bSort)
{	
	HBRUSH hBrsh,hBrshOld;
	hBrsh=(HBRUSH)GetStockObject(NULL_BRUSH);
	hBrshOld=(HBRUSH)pDC->SelectObject(hBrsh);

	CPen pen(PS_DOT,1,BLACK);
	CPen *oldPen=pDC->SelectObject(&pen);

	if(m_nPath==PATH_ROUND)
	{
		//圆弧方式
		double d1,d2;
		m_quadInit.Normal();
		d2=2*m_dbRadius;
		d1=2*(m_dbRadius+m_quadInit.Height());

		CQuad quad1(m_dotBase,d1,d1);
		CQuad quad2(m_dotBase,d2,d2);
		pDC->Ellipse(quad1);
		pDC->Ellipse(quad2);
		
		CDot tmp;
		tmp = quad1.GetBaseDot(BASE_ML);
		pDC->MoveTo(tmp.x,tmp.y);
		tmp = quad1.GetBaseDot(BASE_MR);
		pDC->LineTo(tmp.x,tmp.y);
		tmp = quad1.GetBaseDot(BASE_TM);
		pDC->MoveTo(tmp.x,tmp.y);
		tmp = quad1.GetBaseDot(BASE_BM);
		pDC->LineTo(tmp.x,tmp.y);
	}
	
	pDC->SelectObject(hBrshOld);
	pDC->SelectObject(oldPen);
	pen.DeleteObject();
	DeleteObject(hBrsh);

	//画STROKE
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->Draw(pDC, 0, bSort);
	}
}

CQuad CFnt::CalLimit()
{
	if(m_nPath==PATH_ROUND)
	{
		//圆弧方式

		double d;
		m_quadInit.Normal();
		d=2*(m_dbRadius+m_quadInit.Height());
		m_quadLimit=CQuad(m_dotBase,d,d);
	}
	else
	{
		m_quadLimit.SetEmpty();
		POSITION pos=m_list.GetHeadPosition();
		CStroke *pStroke;
		while(pos!=NULL)
		{	
			pStroke=m_list.GetNext(pos);
			m_quadLimit.Union(pStroke->CalLimit());
		}
	}

	return m_quadLimit;
}

CQuad CFnt::GetMarkRange()
{
	CQuad quad;
	quad.SetEmpty();
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		quad.Union(pStroke->CalLimit());
	}
	return quad;
}
UINT CFnt::Mark(HWND hWnd)
{
	UINT rtn;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		rtn = g_DAT.MarkStroke(*pStroke);
	}
	return rtn;
}

UINT CFnt::SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsResMark)
{
	UINT rtn;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos!=NULL)
	{	
		if(::WaitForSingleObject(hSelStop,0)==WAIT_OBJECT_0)
		{
			return 1;
		}
		pStroke=m_list.GetNext(pos);
		rtn    = g_DAT.MarkStroke(*pStroke, nLayer, bIsResMark);
	}
	return rtn;
}

void CFnt::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{
		pStroke=m_list.GetNext(pos);
		pStroke->ExportPlt(file, bUnExportFill, bScall100);
	}
	return ;
}

void CFnt::SetProperty(int mode, CProperty pty)
{
	m_property.Set(mode,pty);
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->SetProperty(mode,pty);
	}	
}

void CFnt::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		
		ar<<m_dbCharSpace;	
		ar<<m_dbHeight;		
		ar<<m_dbWidthFactor;

		ar<<m_nPath;		
		ar<<m_dotBase.x;	
		ar<<m_dotBase.y;	
		ar<<m_dbDirection;	
		ar<<m_dbRadius;	
		ar<<m_dbStart;		
		ar<<m_bCCW;			
	}
	else
	{
		// TODO: add loading code here
		
		ar>>m_dbCharSpace;	
		ar>>m_dbHeight;		
		ar>>m_dbWidthFactor;

		ar>>m_nPath;		
		ar>>m_dotBase.x;	
		ar>>m_dotBase.y;	
		ar>>m_dbDirection;	
		ar>>m_dbRadius;	
		ar>>m_dbStart;		
		ar>>m_bCCW;			
	}

// 	m_property.Serialize(ar);
}

int CFnt::GetMarkUnit()
{
	int count=0;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		count+=pStroke->GetMarkUnit();
	}
	return count;
}

void CFnt::Scan(StrokeList *pList)
{
	//扫描所有线条数据的指针到FILL中进行填充处理
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pList->AddTail(pStroke);
	}
}

void CFnt::Effect()
{
	if(m_nPath==PATH_ROUND)
	{
		//圆弧方式变形
		m_quadInit.Normal();
		double start=m_dbStart*PAI/180;

		POSITION pos=m_list.GetHeadPosition();
		CStroke *pStroke;
		while(pos!=NULL)
		{	
			pStroke=m_list.GetNext(pos);
			pStroke->Effect(m_dbRadius,start,m_bCCW,m_bInside,m_quadInit);
		}
	}
	else
	{
		double moveX,moveY;
		//对齐方式
		switch(m_nAlign)
		{
		case ALIGN_LEFT:
			moveX=-m_quadInit.left;
			break;
		case ALIGN_RIGHT:
			moveX=-m_quadInit.right;
			break;
		case ALIGN_MID:
			moveX=-(m_quadInit.left+m_quadInit.right)/2;
			break;
		}
		//位置变形
		moveY = -m_quadInit.bottom;		//下边对齐的方式
		POSITION pos=m_list.GetHeadPosition();
		CStroke *pStroke;
		while(pos!=NULL)
		{	
			pStroke=m_list.GetNext(pos);
			pStroke->Move(moveX,moveY);
		}

		double angle;
//		angle=m_dbDirection*PAI/180;
		angle=m_dbDirection;

		//方向变形
		CStroke *pStrokeR = NULL;
		POSITION posR=m_list.GetHeadPosition();
		while(posR!=NULL)
		{	
			pStrokeR=m_list.GetNext(posR);
			pStrokeR->Rotate(CDot(0,0),angle);
		}
	}
}

void CFnt::MarkPreview(CDC *pDC, const int &nPreviewSpeed)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->MarkPreview(pDC, nPreviewSpeed);
	}
}

CQuad CFnt::CalLimit(CDot dot,double dbXScale,double dbYScale)
{
	if(m_nPath==PATH_ROUND)
	{
		//圆弧方式

		double d;
		m_quadInit.Normal();
		d=2*(m_dbRadius+m_quadInit.Height()*dbXScale);
		m_quadLimit=CQuad(m_dotBase,d,d);
		m_quadLimit.TrueToLog();
	}
	else
	{
		m_quadLimit.SetEmpty();
		POSITION pos=m_list.GetHeadPosition();
		CStroke *pStroke;
		while(pos!=NULL)
		{	
			pStroke=m_list.GetNext(pos);
			m_quadLimit.Union(pStroke->CalLimit( dot, dbXScale, dbYScale));
		}
	}

	return m_quadLimit;
}

