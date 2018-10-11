// Stroke.cpp: implementation of the CStroke class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "Stroke.h"
#include "HLDoc.h"
#include "HLView.h"
#include "Node.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL CStroke::m_bRedColorDraw = FALSE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CStroke,CObject,1)
CStroke::CStroke(CProperty pty)
{
	m_property  = pty;
	m_nFocusType = 0;
	m_nCountDealy = 0;
	m_bTag      = FALSE;
	m_pParent   = NULL;
	m_bStyle    = 0;
	m_bLockFlag = pty.m_bLockFlag;

	m_nResinRect = 0;

	m_dbQFre = 0.0;
}

CStroke::CStroke()
{
	m_bStyle = 0;
	m_bTag   = FALSE;
	m_pParent= NULL;
	m_bLockFlag = FALSE;
	m_nFocusType = 0;
	m_nCountDealy = 0;

	m_nResinRect = 0;
	m_dbQFre = 0.0;
}

CStroke::~CStroke()
{
}


void CStroke::AddRect(CWnd *pWnd)//������Ľ����Ϻ��
{
	if (TRUE == m_bStyle) 
	{
		return;
	}
	int nCount = m_dotArray.GetSize();
	if(nCount<1)
	{
		return;
	}

	CHLView *pView = (CHLView*)pWnd;
	CDC     *pDC   = pView->GetDC();
	CQuad  quad;
	DOT    dot;
	CPen   pen(PS_SOLID,1,RGB(255,0,0));
	CPen  *OldPen = pDC->SelectObject(&pen);
	double dbdata = 3;
	
	pView->ActualLen(dbdata);
	CRect rect;
	for(int i = 0; i < nCount; i++)
	{
		dot        = m_dotArray.GetAt(i);
		quad.left  = dot.x-dbdata;
		quad.top   = dot.y+dbdata;
		quad.right = dot.x+dbdata;
		quad.bottom=dot.y-dbdata;

		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);

		rect=(CRect)quad;

		pDC->MoveTo(rect.left,rect.top);
		pDC->LineTo(rect.right,rect.top);
		pDC->LineTo(rect.right,rect.bottom);
		pDC->LineTo(rect.left,rect.bottom);
		pDC->LineTo(rect.left,rect.top);

		if (0 == i) 
		{
			pDC->FillSolidRect(&rect, RED);
		}
		if (1 == i) 
		{
			pDC->FillSolidRect(&rect, BLUE);
		}
	}
	
	pDC->SelectObject(OldPen);
	pView->ReleaseDC(pDC);

	return;
}

BOOL CStroke::Click(CPoint &point,CWnd *pWnd)//�ж�����Ƿ���ж���
{
	int nCount=m_dotArray.GetSize();
	if(nCount<1)
	{
		return FALSE;
	}

	m_dbR= g_SysCfg.m_dbClick;

	double x1,y1,x2,y2;
	double x,y;
	CDot mid,dot;
	CQuad quad;
	dot.x=point.x;
	dot.y=point.y;
	CHLView *pView=(CHLView*)pWnd;
	pView->LogicToPhysic(dot.x,dot.y);
	pView->ActualLen(m_dbR);
	for(int i=0;i<nCount-1;i++)
	{
		x=dot.x;
		y=dot.y;
		x1=m_dotArray[i].x;
		x2=m_dotArray[i+1].x;
		y1=m_dotArray[i].y;
		y2=m_dotArray[i+1].y;

		// a----p----b
		double dbap,dbpb,dbab,dx,dy;
		dx = fabs(x1-dot.x);
		dy = fabs(y1-dot.y);
		dbap = sqrt(dx*dx+dy*dy);
		dx = fabs(x2-dot.x);
		dy = fabs(y2-dot.y);
		dbpb = sqrt(dx*dx+dy*dy);
		dx = fabs(x1-x2);
		dy = fabs(y1-y2);
		dbab = sqrt(dx*dx+dy*dy);

		if( (dbap+dbpb)<(dbab+1.5*m_dbR))
			return TRUE;
	}

	return FALSE;
}

void CStroke::DelRect(CWnd *pWnd)//ȥ�����ĺ��
{
	int nCount = m_dotArray.GetSize();
	if(nCount<1)
	{
		return;
	}

	CHLView *pView = (CHLView*)pWnd;
	CDC     *pDC  = pView->GetDC();
	CQuad   quad;
	DOT     dot;
	CPen    pen(PS_SOLID,1,RGB(255,0,0));
	CPen   *OldPen  = pDC->SelectObject(&pen);
	double  dbdata  = 3;

	pView->ActualLen(dbdata);
	int mod = pDC->SetROP2(R2_NOTXORPEN);
	CRect rect;
	
	for(int i = 0; i < nCount; i++)
	{
		dot=m_dotArray.GetAt(i);
		quad.left=dot.x-dbdata;
		quad.top=dot.y+dbdata;
		quad.right=dot.x+dbdata;
		quad.bottom=dot.y-dbdata;
		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);
		rect=(CRect)quad;
	//	pDC->Ellipse(&rect);
		pDC->MoveTo(rect.left,rect.top);
		pDC->LineTo(rect.right,rect.top);
		pDC->LineTo(rect.right,rect.bottom);
		pDC->LineTo(rect.left,rect.bottom);
		pDC->LineTo(rect.left,rect.top);
	}
	pDC->SetROP2(mod);
	pDC->SelectObject(OldPen);
	pView->ReleaseDC(pDC);

	return;
}

BOOL CStroke::IsOnDot(CPoint &point, CWnd *pWnd)//����Ƿ���н��ĺ��
{
	int nCount=m_dotArray.GetSize();
	if(nCount<1)
	{
		return FALSE;
	}

	CHLView *pView=(CHLView*)pWnd;
	CQuad quad;
	DOT   dot;
	double dbdata = 3;
	pView->ActualLen(dbdata);
	CRect rect;
	for(int i=0;i<nCount;i++)
	{
		dot         = m_dotArray.GetAt(i);
		quad.left   = dot.x-dbdata;
		quad.top    = dot.y+dbdata;
		quad.right  = dot.x+dbdata;
		quad.bottom = dot.y-dbdata;
		pView->PhysicToLogic(quad.left,quad.top);
		pView->PhysicToLogic(quad.right,quad.bottom);
		rect = (CRect)quad;

		if(rect.left<=point.x && point.x<=rect.right && rect.top<=point.y && point.y<=rect.bottom)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CStroke::AddPoint(CPoint &point,CWnd *pWnd)//��ӽ��
{
	int nCount=m_dotArray.GetSize();
	if(nCount<1)
	{
		return FALSE;
	}

	m_dbR = g_SysCfg.m_dbClick;

	double x1,y1,x2,y2;
	double x,y;
	CDot   mid;
	CQuad  quad;
	CDot   dot;
	dot.x = point.x;
	dot.y = point.y;

	CHLView *pView = (CHLView*)pWnd;
	pView->LogicToPhysic(dot.x,dot.y);
	pView->ActualLen(m_dbR);

	for(int i=0; i<nCount-1; i++)
	{
		x  = dot.x;
		y  = dot.y;
		x1 = m_dotArray[i].x;
		x2 = m_dotArray[i+1].x;
		y1 = m_dotArray[i].y;
		y2 = m_dotArray[i+1].y;

		double dbap,dbpb,dbab,dx,dy;
		BOOL bLenX;	//�����Ƿ�ΪX��

		dx = fabs(x1-dot.x);
		dy = fabs(y1-dot.y);
		dbap = sqrt(dx*dx+dy*dy);
		dx = fabs(x2-dot.x);
		dy = fabs(y2-dot.y);
		dbpb = sqrt(dx*dx+dy*dy);
		dx = fabs(x1-x2);
		dy = fabs(y1-y2);
		dbab = sqrt(dx*dx+dy*dy);

		bLenX = (dx>dy);

		if( (dbap+dbpb)<(dbab+1.5*m_dbR)){
			CDot AddDot;
			dx = x2-x1;
			dy = y2-y1;
			if(bLenX){
				AddDot.x = dot.x;
				AddDot.y = (y2-y1)*(dot.x-x1)/(x2-x1)+y1;
			}
			else{
				AddDot.y = dot.y;
				AddDot.x = (x2-x1)*(dot.y-y1)/(y2-y1)+x1;
			}
			m_dotArray.InsertAt(i+1,AddDot);
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CStroke::DotRight(CDot &dot,CWnd *pWnd)
{
	int nCount     = m_dotArray.GetSize();
	CDot dotIndex;
	CHLView *pView = (CHLView*)pWnd;
	CQuad quad;
	double dbdata  = 3;
	pView->ActualLen(dbdata);
	
	for(int i=0;i<nCount;i++)
	{
		dotIndex    = m_dotArray.GetAt(i);
		quad.left   = dotIndex.x-dbdata;
		quad.right  = dotIndex.x+dbdata;
		quad.top    = dotIndex.y+dbdata;
		quad.bottom = dotIndex.y-dbdata;

		if(quad.DotIn(dot))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CStroke::SplitDot(int nBegin,int nEnd,CDot &dt1,CDot &dt2)//ֻ����ɾ
{
	CStroke *pStroke = new CStroke;
	if (NULL == pStroke)
	{
		return;
	}

	pStroke->SetProperty(PTY_INIT, m_property);
	pStroke->m_nFocusType = m_nFocusType;
	pStroke->m_bLockFlag = m_bLockFlag;
	pStroke->m_bTag      =  m_bTag;
	pStroke->m_bStyle    =  m_bStyle;
	pStroke->m_nResinRect = m_nResinRect;
	pStroke->m_dbQFre	= m_dbQFre;

	CNode   *pNode   = (CNode *)GetParent();
	if (NULL == pNode)
	{
		return;
	}

	int i = 0;

	if(dt1==dt2)
	{
		pStroke->m_dotArray.Add(dt1);
		for(i = nBegin; i <= nEnd; i++)
		{
			pStroke->m_dotArray.Add(m_dotArray[i]);
		}
	}
	else
	{
		pStroke->m_dotArray.Add(dt1);
		for(i = nBegin; i <= nEnd; i++)
		{
			pStroke->m_dotArray.Add(m_dotArray[i]);
		}
		pStroke->m_dotArray.Add(dt2);
	}
	pNode->GetList().AddTail(pStroke);

	return;
}



// ��β����
void CStroke::AddStroke(CStroke *pStroke)
{
	int nCount=pStroke->m_dotArray.GetSize();
	int i;
	for(i = 0; i < nCount; i++)
	{
		m_dotArray.Add(pStroke->m_dotArray[i]);
	}

	CNode *pNode=(CNode*)m_pParent;
	POSITION pos=pNode->GetList().GetHeadPosition();
	POSITION pre;
	CStroke *pSearch;
	while(pos)
	{
		pre=pos;
		pSearch=pNode->GetList().GetNext(pos);
		if(pSearch==pStroke)
		{
			pNode->GetList().RemoveAt(pre);
			delete pStroke;
			break;
		}
	}

	return;
}

// �����׽ڵ�����
void CStroke::ConnectHeadToHead(CStroke *pStroke)
{
	if (NULL == pStroke)
	{
		return;
	}
	
	Reverse();
	int nCount=pStroke->m_dotArray.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		m_dotArray.Add(pStroke->m_dotArray[i]);
	}

	return;
}

// ����β�ڵ�����
void CStroke::ConnectTailToTail(CStroke *pStroke)
{
	if (NULL == pStroke)
	{
		return;
	}
	
	pStroke->Reverse();
	int nCount=pStroke->m_dotArray.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		m_dotArray.Add(pStroke->m_dotArray[i]);
	}


	return;
}

void CStroke::Draw(XDC *pDC,int nFont, BOOL bSort)
{
	if(m_dotArray.GetSize()<1)
		return;
	
	if(nFont == FONT_MATRIX)
	{
		for(int i=0;i<m_dotArray.GetSize();i++)
		{
			pDC->SetPixel((int)m_dotArray[i].x, (int)m_dotArray[i].y, RGB(0, 0, 0));
			pDC->SetPixel((int)m_dotArray[i].x, (int)m_dotArray[i].y, m_property.m_color);
		}

		double x, y, r;
		int x1,y1,x2,y2;
		CBrush brush;
		brush.CreateSolidBrush(m_property.m_color);
		CBrush* pOldBrush=(CBrush*) pDC->m_pDC->SelectObject(&brush);
		r = 3;
		for(int i=0;i<m_dotArray.GetSize();i++)
		{
			x = m_dotArray[i].x;
			y = m_dotArray[i].y;
			pDC->PhysicToLogic(x, y);

			x1 = (int)( x - r*0.5 ); 
			y1 = (int)( y - r*0.5 ); 
			x2 = (int)( x + r*0.5 ); 
			y2 = (int)( y + r*0.5 ); 
			pDC->m_pDC->Ellipse(x1,y1,x2,y2);
		}
		pDC->m_pDC->SelectObject(pOldBrush);

		return;
	}
	
	COLORREF drawColor = m_property.m_color;
	if (m_bRedColorDraw)
		drawColor = RED;	//ѡ�ж���ʱ��ʾ��ɫ����ͼ�����غ�
	//	drawColor = RGB(184,134,11);
	if (g_bIsSort && !bSort)
		drawColor = RGB(200, 200, 200);
		
	CPen pen(PS_SOLID, 1, drawColor);
	CPen *oldPen=pDC->SelectObject(&pen);
	CDot Dot;
	Dot = m_dotArray[0];

	double x = Dot.x;
	double y = Dot.y;

	pDC->MoveTo(x,y);
	int size=m_dotArray.GetSize();
	for(int i=1;i<size;i++)	
	{
		Dot = m_dotArray[i];
		x   = Dot.x;
		y   = Dot.y;
		pDC->LineTo(x,y);
	} 

	pDC->SelectObject(oldPen);
	pen.DeleteObject();

	return;
}

int CStroke::GetMarkUnit()
{
	//dsf 2010.04.06
	//PARA para=g_DAT.GetPara(m_property);
	//return para.nCount;
	return 0;
}


void CStroke::MarkPreview(CDC *pDC, const int &nPreviewSpeed, int nFont)
{
	CPen* oldPen;
	CPen pen(PS_SOLID,1,m_property.m_color);
	CPen pencur(PS_SOLID,1,RGB(255,0,0));

	oldPen    = pDC->SelectObject(&pencur);
	//PARA para = g_DAT.GetPara(m_property);

	//dsf 2010.04.06
	/*if(para.nCount == 0)
	{
		return; 
	}*/

	double cX,cY,ucX,ucY;
	g_DAT.GetMarkRange(cX,cY);
	cX/=2;
	cY/=2;

	ucX = cX * (-DPMM) + LCENTER;
	ucY = LCENTER - cY * (-DPMM);
	cX  = cX * DPMM + LCENTER;
	cY  = LCENTER - cY*DPMM;

	if(m_dotArray.GetSize()<1)
	{
		return;
	}

	//������
	if(nFont == FONT_MATRIX)
	{
		if((m_dotArray[0].x>ucX)&&(m_dotArray[0].x<cX)&&(m_dotArray[0].y>cY)&&(m_dotArray[0].y<ucY))
		{
			pDC->SetPixel((int)m_dotArray[0].x, (int)m_dotArray[0].y, RGB(0, 0, 0));
		}

		Sleep(MARKPREVIEW_SPEEDMAX*5/nPreviewSpeed);
		if(g_sys.m_bStopPreview)
		{
			AfxEndThread(0);
		}
		return;
	}

	pDC->MoveTo(m_dotArray[0]);
	
	CDot   dotStart,dotEnd;			//���,�յ�
	double count;					//΢ʸ�����ִ���
	double distanceX,distanceY;		//X,Y�����
	double distance;				//ֱ�߾���
	double angle;					//�Ƕ�
	double x,y;						//΢ʸ��λ��
	double stepX,stepY;				//X,Y�򲽳�
	
	//��Чʸ��
	int i=0;
	for(i=0; i < m_dotArray.GetSize()-1; i++)
	{
		if(i>0)
		{
			pDC->SelectObject(&pen);
			pDC->MoveTo(m_dotArray[i-1]);
			pDC->LineTo(m_dotArray[i]);
		}
		
		pDC->SelectObject(&pencur);
		dotStart= m_dotArray[i];	
		dotEnd  = m_dotArray[i+1];	

		distanceX = dotEnd.x-dotStart.x;
		distanceY = dotEnd.y-dotStart.y;
		distance  = sqrt(distanceX*distanceX+distanceY*distanceY);

		angle = atan2(distanceY,distanceX);
		count = distance/nPreviewSpeed;
		
		stepX = nPreviewSpeed*cos(angle);
		stepY = nPreviewSpeed*sin(angle);
				
		x=dotStart.x;
		y=dotStart.y;

		for(int j = 0; j < count-1; j++)
		{	
			x += stepX;
			y += stepY;

			if((x>ucX)&&(x<cX)&&(y>cY)&&(y<ucY))
			{
				pDC->LineTo((int)x,(int)y);
			}
			else
			{
				pDC->MoveTo((int)x,(int)y);
			}

			Sleep(1);
			if(g_sys.m_bStopPreview)
			{
				AfxEndThread(0);
			}
		}		
		pDC->LineTo((int)dotEnd.x,(int)dotEnd.y);
		Sleep(1);
	}

	pDC->SelectObject(&pen);
	pDC->MoveTo(m_dotArray[i-1]);
	pDC->LineTo(m_dotArray[i]);

	pDC->SelectObject(oldPen);
	pen.DeleteObject();	
	pencur.DeleteObject();

	return;
}

void CStroke::MarkPointPreview(CDC *pDC, const int &nPreviewSpeed)
{
	int size = m_dotArray.GetSize();
	CDot TmpDot;
	for(int i=0;i<size;i++)
	{
		TmpDot = m_dotArray[i];
		TmpDot.TrueToLog();
		
		pDC->SetPixelV((int)TmpDot.x,(int)TmpDot.y,m_property.m_color);
		Sleep(nPreviewSpeed);

		if(g_sys.m_bStopPreview)
		{
			AfxEndThread(0);
		}
	}

	return;
}

void CStroke::FilterExcursion(StrokeList& listData)
{
	//��Ҫ�Ե���е�Բ��ֱ�Ǵ����Ұѷ����ɾ��
	int nSize = m_dotArray.GetSize();
	CDot adot,bdot,cdot,ddot,dottmp;
	//	BOOL bValid;
	DotExList dotExList/*,dotTmp*/;
//	DOTEX tmpDotex;
	
	for(int i=0;i<nSize;i++)
	{
		CDot dsrc= m_dotArray.GetAt(i);
		DOTEX dt;
		DotToDotEx(dsrc,dt);
		dotExList.Add(dt);
	}
	/*
	//������Ч��
	for(int i=0;i<nSize/2;i++)
	{
	adot = m_dotArray[i*2];
	bdot = m_dotArray[i*2+1];
	if(i<nSize/2-1){
	cdot = m_dotArray[i*2+2];
	ddot = m_dotArray[i*2+3];
	}
	else{
	cdot = m_dotArray[0];
	ddot = m_dotArray[1];
	}
	UINT rtn,nL1Dir,nL2Dir;
	CDot dot2;
	rtn=CalCrossEx(adot,bdot,cdot,ddot,dottmp,dot2,nL1Dir,nL2Dir);
	if(rtn==1){
	DOTEX tmp;
	DotToDotEx(dottmp,tmp);
	dotExList.Add(tmp);
	}
	else{
	DOTEX tmp;
	DotToDotEx(bdot,tmp,1);
	dotExList.Add(tmp);
	DotToDotEx(cdot,tmp,1);
	dotExList.Add(tmp);
	}
	}
	//���ĵ����һ������ͬ
	int nsize = dotExList.GetSize();
	if (nSize<1)
	{
	return;
	}
	tmpDotex = dotExList.GetAt(nsize-1);
	dotExList.InsertAt(0,tmpDotex);
	*/
	int nCount = 0;
	//#define FILTERDOT
	// #ifdef FILTERDOT
	FilterUnAvlityDot(dotExList,listData,nCount);
	// #endif
	
	// #ifndef FILTERDOT
	// 	{
	// 	CStrokeB* pStroke;
	// 	pStroke = new CStrokeB(m_property);
	// 	
	// 	nsize = dotExList.GetSize();
	// 
	// 	for(int m=0;m<nSize;m++)
	// 	{
	// 		DOTEX tmp0;
	// 		CDot dot;
	// 		
	// 		tmp0 = dotExList.GetAt(m);
	// 		DotExToDot(tmp0,dot);
	// 		pStroke->Add(dot);
	// 	}
	// 	
	// 	listData.AddTail(pStroke);
	// 	}
	// #endif
	
	
}


//���˶�����ӵ���Ϣ���󽻵㣬�н���������޽����˳�,
//Ϊ�˷�ֹ�����������ӵ���������������޲���100��
void CStroke::FilterUnAvlityDot(DotExList& dotlist,StrokeList& listdata,int& nCount)
{
	//��������
	if(++nCount==100){
		//����ĵ���Ϣת��listdata�󷵻�
		CStroke* pStroke;
		pStroke = new CStroke(m_property);
		
		int n = dotlist.GetSize();
		for(int m=0;m<n;m++)
		{
			DOTEX tmp0;
			CDot dot;
			
			tmp0 = dotlist.GetAt(m);
			DotExToDot(tmp0,dot);
			pStroke->Add(dot);
		}

		listdata.AddTail(pStroke);
		return;
	}

	//�����ཻ��
	CDot dot1,dot2;		//����1�뽻��2��һ�������ֻ������1����
	CDot adot,bdot,cdot,ddot;
	int i,j=0;
	BOOL bFind = FALSE;	//���޽���
//	BOOL bAvlityData = TRUE;	//�Ƿ�����Ч����
	int nSize = dotlist.GetSize();

	if(nSize<3)
		return;

	for(i=0;i<nSize-1;i++)
	{
		DOTEX tmp;
		tmp = dotlist.GetAt(i);
// 		if(tmp.nFlag == 1)
// 			bAvlityData = FALSE;
		DotExToDot(tmp,adot);
		tmp = dotlist.GetAt(i+1);
		DotExToDot(tmp,bdot);

		for(j=i+2;j<nSize-1;j++)
		{
			DOTEX tmp0;
			UINT rtn;
			UINT ndir1,ndir2;

			tmp0 = dotlist.GetAt(j);
			DotExToDot(tmp0,cdot);
			tmp0 = dotlist.GetAt(j+1);
			DotExToDot(tmp0,ddot);
			rtn = CalCrossEx(adot,bdot,cdot,ddot,dot1,dot2,ndir1,ndir2);

			if((ndir1==1)&&(ndir2==1)&&(rtn==1)&&(!(adot==dot1)))
			{
				bFind = TRUE;
				break;
			}
		}

		if(bFind)
			break;
	}
	
	if(bFind){
		//�ֽ������DotExList������һ������
		DotExList dotList1,dotList2;
		DOTEX tmp,dtmp;

		DotToDotEx(dot1,tmp);
		//�齨DOTLIST1
		int m=0;
		for(m=0;m<i+1;m++)
		{
			dtmp = dotlist.GetAt(m);
			dotList1.Add(dtmp);
		}
		dotList1.Add(tmp);
		for(m=j+1;m<nSize;m++)
		{
			dtmp = dotlist.GetAt(m);
			dotList1.Add(dtmp);
		}

		//�齨DOTLIST2
		dotList2.Add(tmp);
		for(m=i+1;m<j+1;m++)
		{
			dtmp = dotlist.GetAt(m);
			dotList2.Add(dtmp);
		}
		dotList2.Add(tmp);

		//����dotlist������һ������
		FilterUnAvlityDot(dotList1,listdata,nCount);
		FilterUnAvlityDot(dotList2,listdata,nCount);
	}
	else{
		//�޽��㣬ȫΪ��Ч����ʱ�Ž���ת������
		//BOOL bPOLYCCW=FALSE;
		// if (bPOLYCCW)//˳ʱ�Ӳ���Ч
		{
			//��Ч���ݲżӵ㵽listdata��
			CStroke* pStroke;
			pStroke = new CStroke(m_property);
			
			nSize = dotlist.GetSize();
			if (nSize>1)
			{
				for(int m=0;m<nSize;m++)
				{
					DOTEX tmp0;
					CDot dot;
					
					tmp0 = dotlist.GetAt(m);
					DotExToDot(tmp0,dot);
					pStroke->Add(dot);
				}
//				BOOL bCw=pStroke->ISClockWise(); // ��ֲ�޸�
				BOOL bCw=FALSE;//pStroke->ISClockWise();
				if (!bCw)//˳ʱ�Ӳ���Ч
					listdata.AddTail(pStroke);
				else
					delete pStroke;
			}
			else
			{
				delete pStroke;
			}
		}
	}
	
	return;
}

CStroke& CStroke::operator =(const CStroke &stroke)
{
	CopyStroke(&stroke);
	
	return *this;
}

//Stroke����һ�������е���һˮƽ����ɨ���ߣ���ɨpListData���ж��Ƿ�Ϊ������
BOOL CStroke::ISOutLineStroke(StrokeList *plistData)
{
	//��ˮƽ�ߵ��е�
	if (!IsRegion())
	{
		DSF_TRACE("\nCstroke_ISOutLineStroke Err:Stroke isn't closed ");
		return FALSE;
	}
	int size=m_dotArray.GetSize();
	double angle=0;
	BOOL bFind=FALSE;
	CDot aDot,bDot;//�߶ε�
	CDot cDot; //�߶��м�
	//������һ������
	int i=0;
	for (i=0;i<size-1;i++)
	{
		aDot=m_dotArray.GetAt(i);
		bDot=m_dotArray.GetAt(i+1);
		angle=CalLineAngle(aDot,bDot);
		if (!(SAMEVALUE(angle,PAI)||SAMEVALUE(angle,0)))
		{
			bFind=TRUE;
			break;
		}
	}
	if (!bFind)
	{
		DSF_TRACE("\nCStroke_ISOutLineStroke Err:Can't Find not horizonal line");
		return FALSE;
	}
	cDot.x=(aDot.x+bDot.x)/2; //
	cDot.y=(aDot.y+bDot.y)/2;

	CArray<HotDot,HotDot> arrayCross;
	HotDot tmpHotDot;

	POSITION pos=plistData->GetHeadPosition();
	CStroke* pStroke;
	CDot dDot,eDot; //���㽻���߶�
	CDot dHotDot;//���ɵ�(�߽��ϵĵ�)
	double dbHotDotDir;//(�߽��ǰһ�㷽��)
	int nDotSize=0;
	BOOL bEndPointCross=FALSE; //�Ƿ񽻵���������δ��
	i=-1;
	while (pos!=NULL)
	{
		pStroke=plistData->GetNext(pos);
		i++;
		nDotSize=pStroke->GetSize();
		bEndPointCross=FALSE;
		int j=0;
		for (j=0;j<nDotSize-1;j++) //ѭ���󽻵�
		{
			dDot=pStroke->GetAt(j);
			eDot=pStroke->GetAt(j+1);
			while (dDot==eDot)
			{
				j++;
				if (j>=nDotSize-1)
				{
					break;
				}
				eDot=pStroke->GetAt(j+1);
			}
			if (j>=nDotSize-1)
			{
				break;
			}
			//������ˮƽ��y=cDot.y�Ľ���
			if (((dDot.y-cDot.y)*(eDot.y-cDot.y)<=0)||bEndPointCross||SAMEVALUE(cDot.y,eDot.y))
			{
				if (SAMEVALUE(cDot.y,eDot.y))
				{
					if (!bEndPointCross)
					{
						dHotDot=eDot;//���±߽��
						dbHotDotDir=dDot.y;//���±߽��ǰһ���Xֵ
						bEndPointCross=TRUE;
					}					
				}
				else //δ��δ����Ϣ�㴦
				{
					if (bEndPointCross) //��һ����δ����
					{
						//�жϷ����Ƿ�ͬ��
						if ((dbHotDotDir-dHotDot.y)*(eDot.y-dHotDot.y)<0)
						{
							//����ͬ��Ϊ��Ч��
							tmpHotDot.m_CrossDot=dHotDot;
							tmpHotDot.m_iStrokeIndex=i;
							arrayCross.Add(tmpHotDot);
						}
					}
					else //��һ��δ����Ϣ�㴦
					{
						dHotDot.x=eDot.x-(eDot.x-dDot.x)*(eDot.y-cDot.y)/(eDot.y-dDot.y);
						dHotDot.y=cDot.y;
						tmpHotDot.m_CrossDot=dHotDot;
						tmpHotDot.m_iStrokeIndex=i;
						arrayCross.Add(tmpHotDot);
					}
					bEndPointCross=FALSE;
				}
			}
		}
		if (bEndPointCross && arrayCross.GetSize()%2==1)
		{
			dDot=pStroke->GetAt((j>=nDotSize-1)?j-(nDotSize-1):j);
			eDot=pStroke->GetAt((j+1>=nDotSize-1)?(j+1)-(nDotSize-1):(j+1));
			while (dDot==eDot)
			{
				j++;
				eDot=pStroke->GetAt((j+1>=nDotSize-1)?(j+1)-(nDotSize-1):(j+1));
			}
			//�жϷ����Ƿ�ͬ��
			if ((dbHotDotDir-dHotDot.y)*(eDot.y-dHotDot.y)<0)
			{
				//����ͬ��Ϊ��Ч��
				tmpHotDot.m_CrossDot=dHotDot;
				tmpHotDot.m_iStrokeIndex=i;
				arrayCross.Add(tmpHotDot);
			}
		}
	}
	//arrayCross:������y=cDot.y�����н���
	//cDot:�����߽���;
	//�ж��Ƿ�Ϊ������
	//a.�ȶ�arrayCross��С��������
	size=arrayCross.GetSize();
	CDot fDot;
	int jindex;
	HotDot tmpHotDot2;
	for (i=0;i<size-1;i++)
	{
		tmpHotDot=arrayCross.GetAt(i);
		jindex=i;
		for (int j=i+1;j<size;j++)
		{
			tmpHotDot2=arrayCross.GetAt(j);
			if (tmpHotDot2.m_CrossDot.x<tmpHotDot.m_CrossDot.x)
			{	
				//switch
				tmpHotDot=tmpHotDot2;
				jindex=j;
			}			
		}
		if (i!=jindex)
		{
			arrayCross.SetAt(jindex,arrayCross.GetAt(i));
			arrayCross.SetAt(i,tmpHotDot);
		}
	}
	//b.�ҳ�cDot��arrayCross�е�����
	int nOutLine = -1;
	jindex=-1;
	for (i=0;i<size;i++)
	{
		dDot=arrayCross.GetAt(i).m_CrossDot;
		{
			if (dDot==cDot)
			{
				nOutLine = arrayCross.GetAt(i).m_iStrokeIndex;
				jindex=i;
				break;
			}
		}
	}
	if (jindex==(-1))
	{
		return FALSE; //���������ܣ���Ϊ�ݴ�
	}
	//c.�ж�(ע����������0)
	tmpHotDot=arrayCross.GetAt(0);
	tmpHotDot.m_bOutLine=TRUE;
	arrayCross.SetAt(0,tmpHotDot);

	/*
	HotDot bhdot,ehdot;
	bhdot = arrayCross.GetAt(0);
	bhdot.m_bOutLine = TRUE;
	arrayCross.SetAt(0,bhdot);

	for(i=1;i<size;i++)
	{
		ehdot = arrayCross.GetAt(i);
		if(ehdot.m_iStrokeIndex==bhdot.m_iStrokeIndex){
			ehdot.m_bOutLine = TRUE;
			arrayCross.SetAt(i,ehdot);

			if(i<size-1){
				bhdot = arrayCross.GetAt(i+1);
				bhdot.m_bOutLine = TRUE;
				arrayCross.SetAt(i+1,bhdot);
				i++;
				continue;
			}
		}
		else{
			if(bhdot.m_bOutLine){
				ehdot.m_bOutLine = FALSE;
				arrayCross.SetAt(i,ehdot);
			}
		}
	}

	HotDot tmp;
	tmp.m_bOutLine = TRUE;

	for(i=0;i<size;i++)
	{
		tmp = arrayCross.GetAt(i);
		if(tmp.m_iStrokeIndex == nOutLine)
		{
			break;
		}
	}*/

	HotDot ehdot;
	HotDot tmp;
	tmp.m_bOutLine = TRUE;
	CArray<HotDot, HotDot> arrayStack;
	for (i=0; i<size; i++)
	{
		ehdot = arrayCross.GetAt(i);
		int size = arrayStack.GetSize();
		if (size==0)
		{
			arrayStack.Add(ehdot);
		}
		else
		{
			HotDot tmpHDot = arrayStack.GetAt(size-1);
			if (ehdot.m_iStrokeIndex != tmpHDot.m_iStrokeIndex)
			{
				arrayStack.Add(ehdot);
			}
			else
			{
				arrayStack.RemoveAt(size-1);
				tmpHDot.m_bOutLine = (arrayStack.GetSize()%2==0)?TRUE:FALSE;
				if(tmpHDot.m_iStrokeIndex==nOutLine) 
				{
					tmp.m_bOutLine = tmpHDot.m_bOutLine;
					break;
				}
			}
		}
	}

	return tmp.m_bOutLine;

// 	tmpHotDot2=tmpHotDot;
// 	for( i=1;i<=jindex;i++)
// 	{
// 		tmpHotDot2=arrayCross.GetAt(i);
// 		for(int j=0;j<i;j++)
// 		{
// 			tmpHotDot=arrayCross.GetAt(j);
// 			if (tmpHotDot.m_iStrokeIndex==tmpHotDot2.m_iStrokeIndex)
// 			{
// 				tmpHotDot2.m_bOutLine=tmpHotDot.m_bOutLine;
// 				arrayCross.SetAt(i,tmpHotDot2);
// 				break;
// 			}
// 		}
// 		if (j==i)
// 		{
// 			//֮����֮ǰ���ֹ���iSotrkeIndex����ǰһ�����࣬����ΪTrue;
// 			HotDot tmpHotDot3;
// 			HotDot tmpHotDot4;
// 			BOOL bInclude=FALSE;
// 			for (int k=0;k<j;k++)
// 			{
// 				tmpHotDot3=arrayCross.GetAt(k);
// 				for (int l=j+1;l<size;l++)
// 				{
// 					tmpHotDot4=arrayCross.GetAt(l);
// 					if (tmpHotDot3.m_iStrokeIndex==tmpHotDot4.m_iStrokeIndex)
// 					{
// 						bInclude=TRUE;
// 						break;
// 					}
// 				}
// 				if (bInclude)
// 				{
// 					break;
// 				}
// 			}
// 			if (bInclude)
// 			{
// 				tmpHotDot2.m_bOutLine=!tmpHotDot.m_bOutLine;
// 				arrayCross.SetAt(i,tmpHotDot2);
// 			}
// 			else
// 			{
// 				tmpHotDot2.m_bOutLine=TRUE;
// 				arrayCross.SetAt(i,tmpHotDot2);
// 			}
// 			
// 		}
// 	}

// 	return tmpHotDot2.m_bOutLine;
} 

// ����ָ����λ�����ߵ����
void CStroke::SetDotToStart(CDot &dt)
{
	int nCount = m_dotArray.GetSize();
	if (nCount < 2)
	{
		return;
	}

	CDot sDot = m_dotArray[0];
	CDot eDot = m_dotArray[nCount - 1];
	if(sDot==eDot)
	{
		CArray<CDot,CDot> dotArrayTemp;
		dotArrayTemp.RemoveAll();

		for (int i = 0; i < nCount; i++)
		{
			CDot dtTemp = m_dotArray[i];
			if (!(dt == dtTemp))// ��ָ����
			{
				dotArrayTemp.Add(dtTemp);
			}
			else
			{
				dotArrayTemp.Add(dt);
				CArray<CDot,CDot> dotArray;
		        dotArray.RemoveAll();
				for (int j = i; j < (nCount - 1); j++)
				{
					CDot Dot = m_dotArray[j];
					dotArray.Add(Dot);
				}
				m_dotArray.RemoveAll();
				m_dotArray.Append(dotArray);
				m_dotArray.Append(dotArrayTemp);
				dotArray.RemoveAll();
				dotArrayTemp.RemoveAll();

				return;
			}
		}// end for i
		dotArrayTemp.RemoveAll();
	}// end if
	else
	{
		CString str("");
		str.LoadString(IDS_THELINEISNOTCIRCL);
		AfxMessageBox(str);
	}

	return;
}

CStroke * CStroke::Copy()
{
	CStroke *tmp = new CStroke(m_property);
	if (NULL == tmp)
	{
		return NULL;
	}
	tmp->m_dotArray.Append(m_dotArray);
	tmp->m_nFocusType = m_nFocusType;
	tmp->m_nResinRect = m_nResinRect;
	tmp->m_dbQFre	= m_dbQFre;
	return tmp;
}


CQuad CStroke::CalLimit()
{
	int nCount=m_dotArray.GetSize();
	if(nCount<=0)
	{
		m_quadLimit.Initial();
	}
	else
	{
		CDot dot;
		int i;
		dot = m_dotArray[0];
		m_quadLimit.left = m_quadLimit.right =dot.x;
		m_quadLimit.top  = m_quadLimit.bottom=dot.y;
		for(i=1;i<nCount;i++)
		{
			dot=m_dotArray[i];
			m_quadLimit.left  = min(m_quadLimit.left,dot.x);
			m_quadLimit.right = max(m_quadLimit.right,dot.x);
			m_quadLimit.top   = max(m_quadLimit.top,dot.y);
			m_quadLimit.bottom= min(m_quadLimit.bottom,dot.y);
		}
	}
	return m_quadLimit;
}


CQuad CStroke::CalLimit(CDot dot,double dbXScale,double dbYScale)
{
	if(m_dotArray.GetSize()==0)
	{
		m_quadLimit.SetEmpty();
	}
	else
	{
		CDot Dot=m_dotArray[0];
		Dot.Scale(dot,dbXScale,dbYScale);
		m_quadLimit=CQuad(Dot.x,Dot.y,Dot.x,Dot.y);
		for(int i=1;i<m_dotArray.GetSize();i++)
		{
			Dot=m_dotArray[i];
			Dot.Scale(dot,dbXScale,dbYScale);
			m_quadLimit.left	=min(m_quadLimit.left	, Dot.x);
			m_quadLimit.right	=max(m_quadLimit.right	, Dot.x);
			m_quadLimit.top		=max(m_quadLimit.top	, Dot.y);
			m_quadLimit.bottom	=min(m_quadLimit.bottom	, Dot.y);
		}
	}
	return m_quadLimit;
}

void CStroke::CopyStroke(const CStroke *pStroke)
{
	if(this==pStroke)
	{
		return;
	}
	else
	{
		m_dotArray.RemoveAll();
		m_dotArray.Append(pStroke->m_dotArray);
	}

	m_property = pStroke->m_property;
	m_nFocusType = pStroke->m_nFocusType;
	m_nResinRect = pStroke->m_nResinRect;
	m_dbQFre	= pStroke->m_dbQFre;
	CalLimit();

	return;
}

void CStroke::CopyStrokeB(const CStroke *pStroke)
{
	if(this==pStroke)
	{
		return;
	}
	else
	{
		m_dotArray.RemoveAll();
		m_dotArray.Append(pStroke->m_dotArray);
	}

	m_property = pStroke->m_property;
	m_nFocusType = pStroke->m_nFocusType;
	m_nResinRect = pStroke->m_nResinRect;
	m_dbQFre	= pStroke->m_dbQFre;
	CalLimit();

	return;
}

void CStroke::SetProperty(int mode,CProperty &pty)
{
	m_property.Set(mode,pty);
	
	return;
}

void CStroke::SetLayColor(int nLayer,COLORREF color)
{
	m_property.m_color  = color;
	m_property.m_nLayer = nLayer;
}

long CStroke::IsInDots()
{
	return 0;
}

long CStroke::GetOptLayer()
{
	return m_property.m_nLayer;
}

void CStroke::TranslateDot(double &x,double &y,double a,double x0,double y0)//����任
{
	double dbx,dby;
	dbx = x-x0;
	dby = y-y0;
	x   = dbx*cos(a) + dby*sin(a);
	y   = dby*cos(a) - dbx*sin(a);
	
	return;
}

void CStroke::SetFlag(BOOL bflag)
{
	m_bTag = bflag;
}

void CStroke::SetParent(LPVOID *pParent)
{
	m_pParent=pParent;
}

void CStroke::SetStyle(BOOL bflag)
{
	m_bStyle=bflag;
}

void CStroke::SetLockFlag(BOOL bFlag)
{
	m_bLockFlag = bFlag;
	m_property.m_bLockFlag = bFlag;
}

BOOL CStroke::GetLockFlag()
{
	return m_property.m_bLockFlag;
}

// ������ľ���
double CStroke::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return dbD;
}

//�������󽻵�
//�������ֱ�ߵĽ��㣬���жϽ��㷽��
//ԭ�� a b c d ,b c�ཻ
//nL1Dir,nL2Dir���ڷ���ֵΪ1,3ʱ��Ч
//nL1Dir =0 a b e
//		 =1 a e b
//		 =2 e a b
//nL2Dir =0 e c d
//		 =1 c e d
//		 =2 c d e

//����ֵ �߶��ڽ�����Ϣ 0:����(�ӳ���Ҳ�޽���) 1:һ������ 2:�������,������ 3:�������,������ 4:������㣬ab��bc�����Է�
UINT CStroke::CalCrossEx(CDot aDot,CDot bDot,CDot cDot,CDot dDot,CDot& dotCross,CDot& dotCross2, UINT& nL1Dir,UINT& nL2Dir)
{
	double k0,b0,k1,b1;
	UINT nL1Type=0; 	
	UINT nL2Type=0;
	BOOL bAInCD=FALSE;
	BOOL bBInCD=FALSE;
	BOOL bCInAB=FALSE;
	BOOL bDInAB=FALSE;

	if (!(CalLine(aDot,bDot,k0,b0,nL1Type)))
	{
		return 0;
	}
	if (!(CalLine(cDot,dDot,k1,b1,nL2Type)))
	{
		return 0;
	}
	if ((1==nL1Type)&&(1==nL2Type))
	{
		if(SAMEVALUE(k0,k1))
		{
			DSF_TRACE("\nCalCross Err:Same K,Same X!");
			bAInCD=(cDot.y-aDot.y)*(dDot.y-aDot.y)<=0;
			bBInCD=(cDot.y-bDot.y)*(dDot.y-bDot.y)<=0;
			bCInAB=(aDot.y-cDot.y)*(bDot.y-cDot.y)<=0;
			bDInAB=(aDot.y-dDot.y)*(bDot.y-dDot.y)<=0;
			if ((!bAInCD)&&(!bBInCD)&&(!bCInAB)&&(!bDInAB))//c,d������ab��
			{
				return 2;
			}
			if ((bAInCD&&bBInCD)||(bCInAB&&bDInAB))
			{
				return 4;
			}
			else
			{
				dotCross=cDot;
				dotCross2=bDot;
				nL1Dir=1;
				nL2Dir=1;
				return 3;
			}
		}
		else
		{
			DSF_TRACE("\nCalCross Err:Same K,Diff X!");
			return 0;
		}		
	}
	else if ((0==nL1Type)&&(0==nL2Type))
	{
		if (SAMEVALUE(k0,k1))
		{
			//SAME K
			if(SAMEVALUE(b0,b1))
			{
				DSF_TRACE("\nCalCross Err:Same K,Same X!");
				bAInCD=(cDot.x-aDot.x)*(dDot.x-aDot.x)<=0;
				bBInCD=(cDot.x-bDot.x)*(dDot.x-bDot.x)<=0;
				bCInAB=(aDot.x-cDot.x)*(bDot.x-cDot.x)<=0;
				bDInAB=(aDot.x-dDot.x)*(bDot.x-dDot.x)<=0;
				if ((!bAInCD)&&(!bBInCD)&&(!bCInAB)&&(!bDInAB))//c,d������ab��
				{
					return 2;
				}
				if ((bAInCD&&bBInCD)||(bCInAB&&bDInAB))
				{
					return 4;
				}
				else
				{
					dotCross=cDot;
					dotCross2=bDot;
					nL1Dir=1;
					nL2Dir=1;
					return 3;
				}
				
			}
			else
			{
				DSF_TRACE("\nCalCross Err:Same K,Diff X!");
				return 0;
			}			
			
		}
		else
		{
			dotCross.x=(b1-b0)/(k0-k1);
			dotCross.y=k0*dotCross.x+b0;
			//�ж�nL1Dir����
			if ((aDot.x-dotCross.x)*(bDot.x-dotCross.x)<=0)
				nL1Dir=1;
			else if(fabs(aDot.x-dotCross.x)>fabs(bDot.x-dotCross.x))
				nL1Dir=0;			
			else
				nL1Dir=2;
			//�ж�nL2Dir����
			if ((cDot.x-dotCross.x)*(dDot.x-dotCross.x)<=0)
				nL2Dir=1;
			else if(fabs(cDot.x-dotCross.x)>fabs(dDot.x-dotCross.x))
				nL2Dir=2;			
			else
				nL2Dir=0;
			return 1;
		}
	}
	else if ((1==nL1Type)&&(0==nL2Type))
	{
		dotCross.x=k0;
		dotCross.y=k1*dotCross.x+b1;

		//�ж�nL1Dir����
		if ((aDot.y-dotCross.y)*(bDot.y-dotCross.y)<=0)
			nL1Dir=1;
		else if(fabs(aDot.y-dotCross.y)>fabs(bDot.y-dotCross.y))
			nL1Dir=0;			
		else
			nL1Dir=2;
		//�ж�nL2Dir����
		if ((cDot.x-dotCross.x)*(dDot.x-dotCross.x)<=0)
			nL2Dir=1;
		else if(fabs(cDot.x-dotCross.x)>fabs(dDot.x-dotCross.x))
			nL2Dir=2;			
		else
			nL2Dir=0;
		return 1;
	}
	else //((0==nL1Type)&&(1==nL2Type))
	{
		dotCross.x=k1;
		dotCross.y=k0*dotCross.x+b0;
		//�ж�nL1Dir����
		if ((aDot.x-dotCross.x)*(bDot.x-dotCross.x)<=0)
			nL1Dir=1;
		else if(fabs(aDot.x-dotCross.x)>fabs(bDot.x-dotCross.x))
			nL1Dir=0;			
		else
			nL1Dir=2;
		//�ж�nL2Dir����
		if ((cDot.y-dotCross.y)*(dDot.y-dotCross.y)<=0)
			nL2Dir=1;
		else if(fabs(cDot.y-dotCross.y)>fabs(dDot.y-dotCross.y))
			nL2Dir=2;			
		else
			nL2Dir=0;
		return 1;
	}
	return 0;
}

//����ʸ��a->b�ĽǶ�(-pi,pi]
double CStroke::CalLineAngle(CDot aDot,CDot bDot)
{
	if (aDot==bDot)
	{
		DSF_TRACE("CalLineAngle Err::Same Dot");
		return 0;
	}
	if(SAMEVALUE(aDot.y,bDot.y)) //ˮƽ��
	{
		if (aDot.x>bDot.x)
		{
			return PAI;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		double angle;
		angle=acos((bDot.x-aDot.x)/(CalDistance(aDot,bDot)));
		if ((bDot.y-aDot.y)<0)
		{
			angle*=(-1.0);
		}
		return angle;
	}
}

double CStroke::GetAngle(CDot &p1, CDot &p2, CDot &p3) 
{
	double angle=0.0;	
	double a2,b2,c2;
	a2=((double(p2.x-p1.x))*(p2.x-p1.x)+(double(p2.y-p1.y))*(p2.y-p1.y));
	b2=((double(p3.x-p2.x))*(p3.x-p2.x)+(double(p3.y-p2.y))*(p3.y-p2.y));
	c2=((double(p3.x-p1.x))*(p3.x-p1.x)+(double(p3.y-p1.y))*(p3.y-p1.y));
	if(fabs(a2)< EPSINON && fabs(b2) > EPSINON)
	{
		angle = atan2(p3.y-p2.y,p3.x-p2.x);
		angle = fabs(angle);
		angle = angle*180/PAI;
		angle = 180 - angle;
		return angle;
	}
	if (fabs(b2) < EPSINON)
	{
		return 0.0;
	}
	angle=double(a2+b2-c2)/2.0/sqrt(double (a2))/sqrt(double (b2));
	
	if((angle+1.0) < EPSINON )
	{
		angle=-1.0;
	}
	
	if((angle-1.0) > - EPSINON)
	{
		angle=1.0;
	}
	angle=acos(angle);
	angle=PAI-angle;
	
	angle = angle*180/PAI;
	angle = 180 - angle;
	return angle;
}

//�������ֱ�ߵĽ��㣬���жϽ��㷽��
//����ֵ��TRUE���ýڵ�
//        FALSE�����ýڵ�
BOOL CStroke::CalCross(CDot aDot,CDot bDot,CDot cDot,CDot dDot,CDot& dotCross,BOOL& bValid)
{
	bValid=FALSE;
	double k0,b0,k1,b1;
	UINT nL1Type=0; 	
	UINT nL2Type=0;
	
	if (!(CalLine(aDot,bDot,k0,b0,nL1Type)))
	{
		return FALSE;
	}
	if (!(CalLine(cDot,dDot,k1,b1,nL2Type)))
	{
		return FALSE;
	}
	if ((1==nL1Type)&&(1==nL2Type))
	{
		DSF_TRACE("\nCalCross Err:Same K!");
		return FALSE;
	}
	else if ((0==nL1Type)&&(0==nL2Type))
	{
		if (SAMEVALUE(k0,k1))
		{
			//SAME K
			DSF_TRACE("\nCalCross Err:Same K!");
			return FALSE;
		}
		else
		{
			dotCross.x=(b1-b0)/(k0-k1);
			dotCross.y=k0*dotCross.x+b0;			
		}
	}
	else if ((1==nL1Type)&&(0==nL2Type))
	{
		dotCross.x=k0;
		dotCross.y=k1*dotCross.x+b1;
	}
	else //((0==nL1Type)&&(1==nL2Type))
	{
		dotCross.x=k1;
		dotCross.y=k0*dotCross.x+b0;
	}
	//�жϽ����Ƿ�Ϸ�
	
	if (CalDistance(cDot,dotCross)>CalDistance(dDot,dotCross))
	{
		//Err Dir
		bValid=FALSE;
		DSF_TRACE("\nCalCross Err:dotCross inValid!");
		return FALSE;
	}
	bValid=TRUE;
	return TRUE;
}

double CStroke::CalDistance(CDot dot1,CDot dot2)
{
	double dx=dot1.x-dot2.x;
	double dy=dot1.y-dot2.y;
	return sqrt(dx*dx+dy*dy);
}

//��ֱ�߷��� //nLType 0:y=bx+b 1:x=k;
BOOL CStroke::CalLine(CDot aDot,CDot bDot,double& k,double& b,UINT& nLType)
{
	if (SAMEVALUE(aDot.x,bDot.x))
	{
		if (SAMEVALUE(aDot.y,bDot.y))
		{
			//SAME Point
			DSF_TRACE("\nCalLine Err:Same Point!");
			return FALSE;
		}
		else
		{
			nLType=1;
			k=aDot.x;
			return TRUE;
		}
	}
	else
	{
		k=(bDot.y-aDot.y)/(bDot.x-aDot.x);
		b=aDot.y-k*aDot.x;
		nLType=0;
		return TRUE;
	}
}

// ��ƽ��x0,y0 ����תangle
//bRev=TRUE��������ϵ�任Ϊԭ����ϵ
//	  =FALSEԭ����ϵ->������ϵ
//x,y��Ҫ�任����ֵ
void CStroke::TransCoord(double x0,double y0,double angle,BOOL bRev,double& x,double &y)
{
	double xSource=x;
	double ySource=y;
	double dbCos=cos(angle);
	double dbSin=sin(angle);
	if (!bRev)
	{
		xSource=x-x0;
		ySource=y-y0;
		x=dbCos*xSource+dbSin*ySource;
		y=dbCos*ySource-dbSin*xSource;
	}
	else
	{
		xSource=dbCos*x-dbSin*y;
		ySource=dbCos*y+dbSin*x;
		x=xSource+x0;
		y=ySource+y0;
	}
}

void CStroke::DrawBmp(CDC *pDC)
{
	CPen pen(PS_SOLID,1,m_property.m_color);
	CPen *oldPen=pDC->SelectObject(&pen);
	
	CDot Dot ;
	for(int i=0;i<m_dotArray.GetSize();i++)
	{
		Dot= m_dotArray[i];
		pDC->MoveTo(Dot);
		pDC->LineTo((int)Dot.x+5,(int)Dot.y);
	}
	
	pDC->SelectObject(oldPen);
	pen.DeleteObject();
	
	return;
}

void CStroke::TrueToLog()
{
	for(int i=0;i<m_dotArray.GetSize();i++)
	{
		m_dotArray[i].TrueToLog();
	}
	
	return;
}

CDot CStroke::GetAt(int index)
{
	return m_dotArray.GetAt(index);
}

CDot CStroke::CalCrossPoint(CDot dotS,CDot dotE,double scan)
{
	CDot dot;
	
	dot.y = scan;
	dot.x = dotE.x-(dotE.y-scan)*(dotE.x-dotS.x)/(dotE.y-dotS.y);
	
	return dot;
}

/*
listInfo[0]��׼����
listInfo[1]��׼�����ϵ���
listInfo[2]��׼�����µ���
*/
void CStroke::ScanFill(DotList* listInfo, double scan,double str,double end)
{
	//ֻ�迼��Y����
	CDot   dot,dot1,dot2;
	double dis;
	
	int max=m_dotArray.GetSize()-1;
	if(max<1)
	{
		return;
	}
	
	//ȷ���Ƿ����ɨ����
	BOOL bFlag = FALSE;
	
	for(int i = 0; i < max; i++)
	{
		dot1 = m_dotArray[i];
		dot2 = m_dotArray[i+1];
		
		//����ɨ����Ч�������Ҽ��뵽����LISTINFO��
		if(fabs(dot1.y-scan)<1E-6)
		{
			dot1.y+=0.0001;
		}
		
		if(fabs(dot2.y-scan)<1E-6)
		{
			dot2.y+=0.0001;
		}
		
		dis = fabs(dot1.y-scan)+fabs(dot2.y-scan)-fabs(dot1.y-dot2.y);
		if(dis<1E-6)
		{
			dot = CalCrossPoint(dot1,dot2,scan);
			listInfo[0].Add(dot);
		}
		
		//������˵������Ҽ��뵽����LIST��
		if(fabs(dot1.y-str) < 1E-6)
		{
			dot1.y+=0.0001;
		}
		if(fabs(dot2.y-str) < 1E-6)
		{
			dot2.y+=0.0001;
		}
		
		dis = fabs(dot1.y-str)+fabs(dot2.y-str)-fabs(dot1.y-dot2.y);
		if(dis<1E-6)
		{
			dot = CalCrossPoint(dot1,dot2,str);
			listInfo[1].Add(dot);
		}
		
		if(fabs(dot1.y-end)<1E-6)
		{
			dot1.y+=0.0001;
		}
		if(fabs(dot2.y-end)<1E-6)
		{
			dot2.y+=0.0001;
		}
		
		dis = fabs(dot1.y-end)+fabs(dot2.y-end)-fabs(dot1.y-dot2.y);
		if(dis < 1E-6)
		{
			dot = CalCrossPoint(dot1,dot2,end);
			listInfo[2].Add(dot);
		}
	}
	
	return;
}

void CStroke::Add(CDot dot)
{
	m_dotArray.Add(dot);
	
	return;
}

void CStroke::Scale(CDot dot, double ratioX, double ratioY)
{
	for(int i=0;i<m_dotArray.GetSize();i++)
	{
		m_dotArray[i].Scale(dot,ratioX,ratioY);
	}
	
	return;
}

void CStroke::Rotate(CDot dot, double angle)
{
	for(int i=0;i<m_dotArray.GetSize();i++)
	{
		m_dotArray[i].Rotate(dot,angle);
	}
	
	return;
}

void CStroke::Move(double moveX, double moveY)
{
	for(int i=0;i<m_dotArray.GetSize();i++)
	{
		m_dotArray[i].Move(moveX,moveY);
	}
	
	return;
}

void CStroke::Effect(double r,double start,BOOL bCCW,BOOL bInside,CQuad quad)
{
	double x,y; 
	double angle;
	
	for(int i = 0; i < m_dotArray.GetSize(); i++)
	{
		//�õ���ǰ���ƫ�ƽ�
		x = m_dotArray[i].x-quad.left;
		if(bCCW)
		{
			angle = start+x/r;
		}
		else
		{
			angle = start-x/r;
		}
		
		//�õ���ǰ��İ뾶
		if(bInside)
		{
			y = quad.top - m_dotArray[i].y + r;
		}
		else
		{
			y = m_dotArray[i].y - quad.bottom + r;
		}
		
		m_dotArray[i].x = y * cos(angle);
		m_dotArray[i].y = y * sin(angle);
	}
}

void CStroke::Serialize(FILE* fp,BOOL bFlag)
{	
	//����׼�����ı���ʽ��������¼��
	CMySerialize fs(fp);
	CString str;
	
	if (bFlag)
	{
		fs<<"S_B";
		int nSize = m_dotArray.GetSize();
		CDot dt;
		for(int i = 0; i < nSize; i++)
		{
			dt=m_dotArray.GetAt(i);
			dt.TrueToLog();

			fs<<100;
			fs<<dt.x;
			fs<<110;
			fs<<dt.y;

			fs<<111;
			fs<<m_nResinRect;
		}
		//д��β��Ϣ
		fs<<"S_E";

		m_property.Serialize(fp,bFlag);	
	}
	else
	{
		CDot dot(CENTER,CENTER);
		
		while(fs>>str)
		{
			if(str == "100")
			{
				fs>>str;
				dot.x = atof(str);
				continue;
			}
			else if(str == "110")
			{
				fs>>str;
				dot.y = atof(str);
				dot.LogToTrue();
				m_dotArray.Add(dot);
				continue;
			}
			else if(str == "111")
			{
				fs>>str;
				m_nResinRect = atoi(str);
				continue;
			}
			if(str == "S_E")
				break;
		}
					
		m_property.Serialize(fp,bFlag);
	}
	
	return;
}

int CStroke::GetSize()
{
	return m_dotArray.GetSize();
}

void CStroke::AddHead(CDot dot)
{
	m_dotArray.InsertAt(0,dot);
	return;
}

void CStroke::DrawExtern(XDC *pDC)
{
	int size = m_dotArray.GetSize();
	if (size == 0)
	{
		return;
	}
	CDot Dot;
	Dot = m_dotArray[0];
	
	double x = Dot.x;
	double y = Dot.y;
	
	pDC->MoveTo(x,y);
	for(int i=1;i<size;i++)	
	{
		Dot = m_dotArray[i];
		x   = Dot.x;
		y   = Dot.y;
		pDC->LineTo(x,y);
	}
}

//�Ƿ�Ϊ˳ʱ�ӷ���
BOOL CStroke::ISClockWise() 
{
	//??????�������Y�����ϵĵ�ʱ��ʱ�е�����	
	BOOL bXPOS=FALSE;	//X�Ƿ���������
	int n = 0;
	int nSize = m_dotArray.GetSize();
	double x;
	double y;
	CDot dots,dote;
	double ab,bc,ac;
	
	//������Ȥ��
	int j=0;
	for(j=0;j<nSize-3;j++)
	{
		dots = m_dotArray[j];
		dote = m_dotArray[j+1];
		
		if(fabs(dots.x-dote.x)>0.1)
		{
			bXPOS = (dote.x-dots.x)>0;
			x = dots.x;
			y = dots.y;
			break;
		}
	}
	
	if(j==nSize-3)
		return TRUE;//?????
	
	
	for(int i=1;i<nSize-3;i++)
	{
		dots = m_dotArray[i];
		dote = m_dotArray[i+1];
		ac = fabs(dots.x-x);
		bc = fabs(dote.x-x);
		ab = fabs(dots.x-dote.x);
		if(fabs(ac+bc-ab)<0.00001){
			double ty = (dote.y-dots.y)*(x-dots.x)/(dote.x-dots.x)+dots.y;
			if(ty>y){
				n++;
			}
		}
	}
	
	if(n%2){
		//������,X������Ϊ˳ʱ��
		return bXPOS;
	}
	else{
		//ż����,X������Ϊ˳ʱ��
		return !bXPOS;
	}
	
}

//������ߣ���ֵ��������ֵ����
//ע�⣺ԭ���߱����Ǳպ�����
void CStroke::Excursion(CStroke* pStroke,double dbOffset)
{
	//�����ص�
	CDot dot1,dot2;
	int size = m_dotArray.GetSize();
	while(--size)
	{
		dot1 = m_dotArray[size];
		dot2 = m_dotArray[size-1];
		if(dot1==dot2){
			m_dotArray.RemoveAt(size);
		}
	}

	//�ҷ�ˮƽ��
	size=m_dotArray.GetSize();
	double angle=0;
	BOOL bFind=FALSE;
	CDot aDot,bDot;//�߶ε�
	CDot cDot; //�߶��м�
	int i=0;
	for (i=0;i<size-1;i++)
	{
		aDot=m_dotArray.GetAt(i);
		bDot=m_dotArray.GetAt(i+1);
		angle=CalLineAngle(aDot,bDot);
		if (!(SAMEVALUE(angle,PAI)||SAMEVALUE(angle,0)))
		{
			bFind=TRUE;
			break;
		}
	}
	if (!bFind)
	{
		DSF_TRACE("\nCStroke_ISOutLineStroke Err:Can't Find not horizonal line");
		return ;
	}
	cDot.x=(aDot.x+bDot.x)/2; //
	cDot.y=(aDot.y+bDot.y)/2;
	//�������߽���
	CArray<CDot,CDot> arrayCross;	
	CDot dDot,eDot; //���㽻���߶�
	CDot dHotDot;//���ɵ�(�߽��ϵĵ�)
	double dbHotDotDir;//(�߽��ǰһ�㷽��)
	size=m_dotArray.GetSize();
	BOOL bEndPointCross=FALSE; //�Ƿ񽻵���������δ��
	bEndPointCross=FALSE;
	for (int j=0;j<size-1;j++) //ѭ���󽻵�
	{
		dDot=GetAt(j);
		eDot=GetAt(j+1);
		while (dDot==eDot)
		{
			j++;
			if (j>=size-1)
			{
				break;
			}
			eDot=GetAt(j+1);
		}
		if (j>=size-1)
		{
			break;
		}
		//������ˮƽ��y=cDot.y�Ľ���
		if ((dDot.y-cDot.y)*(eDot.y-cDot.y)<=0)
		{
			if (SAMEVALUE(cDot.y,eDot.y))
			{
				if (!bEndPointCross)
				{
					dHotDot=eDot;//���±߽��
					dbHotDotDir=dDot.y;//���±߽��ǰһ���Xֵ
					bEndPointCross=TRUE;
				}					
			}
			else //δ��δ����Ϣ�㴦
			{
				if (bEndPointCross) //��һ����δ����
				{
					//�жϷ����Ƿ�ͬ��
					if ((dbHotDotDir-dHotDot.y)*(eDot.y-dHotDot.y)<0)
					{
						//����ͬ��Ϊ��Ч��
						arrayCross.Add(dHotDot);
					}
				}
				else //��һ��δ����Ϣ�㴦
				{
					dHotDot.x=eDot.x-(eDot.x-dDot.x)*(eDot.y-cDot.y)/(eDot.y-dDot.y);
					dHotDot.y=cDot.y;
					arrayCross.Add(dHotDot);
				}
				bEndPointCross=FALSE;
			}
		}
	}		

	//arrayCross:������y=cDot.y�����н���
	//cDot:�����߽���;
	//�ж��Ƿ�Ϊ������
	//a.�ȶ�arrayCross��С��������
	size=arrayCross.GetSize();
	CDot fDot;
	int jindex;
	for (i=0;i<size-1;i++)
	{
		dDot=arrayCross.GetAt(i);
		jindex=i;
		for (int j=i+1;j<size;j++)
		{
			eDot=arrayCross.GetAt(j);
			if (eDot.x<dDot.x)
			{	
				//switch
				dDot=eDot;
				jindex=j;
			}			
		}
		if (i!=jindex)
		{
			arrayCross.SetAt(jindex,arrayCross.GetAt(i));
			arrayCross.SetAt(i,dDot);
		}
	}
	//b.�ҳ�cDot��arrayCross�е�����
	jindex=-1;
	for (i=0;i<size;i++)
	{
		dDot=arrayCross.GetAt(i);
		{
			if (dDot==cDot)
			{
				jindex=i;
				break;
			}
		}
	}
	if (jindex==(-1))
	{
		return ; //���������ܣ���Ϊ�ݴ�
	}

	//
	angle=CalLineAngle(aDot,bDot);
	//c.�ж�(ע����������0)
	double RotateAngle=0;//������ת�Ƕ�
	if (jindex%2)
	{
		//��ŵ�
		if (angle<0)
		{
			RotateAngle=PAI/2;
		}
		else
		{
			RotateAngle=PAI/(-2);
		}
	}
	else //ż�ŵ�
	{
		if (angle<0)
		{
			RotateAngle=PAI/(-2);
		}
		else
		{
			RotateAngle=PAI/2;
		}
	}
	


	//����(����)��
	size = m_dotArray.GetSize();

	for(i=0;i<size-1;i++)
	{
		dDot = m_dotArray[i];
		eDot = m_dotArray[i+1];
		angle=CalLineAngle(dDot,eDot);
		fDot.y = dbOffset*sin(angle+RotateAngle)+dDot.y;
		fDot.x = dbOffset*cos(angle+RotateAngle)+dDot.x;
		pStroke->Add(fDot);
		fDot.y = dbOffset*sin(angle+RotateAngle)+eDot.y;
		fDot.x = dbOffset*cos(angle+RotateAngle)+eDot.x;
		pStroke->Add(fDot);
	}

	fDot = pStroke->GetAt(0);
	pStroke->Add(fDot);
}

BOOL CStroke::IsRegion()
{
	//�Ƿ��Ƿ������
	int n = m_dotArray.GetSize();
	
	if(n==0)
		return FALSE;
	
	CDot f=m_dotArray[0];
	CDot e=m_dotArray[n-1];
	
	return (fabs(f.x-e.x)<ESP&&fabs(f.y-e.y)<ESP);
}

void CStroke::MoveDot(int nNum, double MoveX, double MoveY)
{
	m_dotArray[nNum].Move(MoveX,MoveY);
}

CDot CStroke::GetDot(CDot &dot1, CDot &dot2,double dbPoint)
{
	CDot dotNew;
	double k,dbx,dby;
	dbx = dot2.x-dot1.x;
	dby = dot2.y-dot1.y;
	double dbLen;
	if(fabs(dbx)>1e-10)
	{
		k = dby/dbx;
		k = atan(k);
		
		if(dbx<0)
		{
			k+=PAI;
		}
		
		dbLen=sqrt(dbx*dbx+dby*dby);
		dbLen=dbLen-dbPoint;
		dotNew.x=dbLen*cos(k)+dot1.x;
		dotNew.y=dbLen*sin(k)+dot1.y;
		
		return dotNew;
	}
	else
	{
		k=PAI/2;
		
		if(dby<0)
		{
			k=-PAI/2;
		}
		
		dbLen=sqrt(dbx*dbx+dby*dby);
		dbLen-=dbPoint;
		dotNew.x=dbLen*cos(k)+dot1.x;
		dotNew.y=dbLen*sin(k)+dot1.y;
		
		return dotNew;
	}
}

void CStroke::RemoveDot(int nIndex)
{
	int nCount=m_dotArray.GetSize();
	if(nIndex>=0 && nIndex<nCount)
	{
		int i;
		CArray<CDot,CDot>NewArray;
		NewArray.SetSize(nCount-1);
		
		for(i=0;i<nIndex;i++)
		{
			NewArray[i]=m_dotArray[i];
		}
		
		for(i=nIndex+1;i<nCount;i++)
		{
			NewArray[i-1]=m_dotArray[i];
		}
		
		m_dotArray.RemoveAll();
		m_dotArray.SetSize(nCount-1);
		for(i=0;i<nCount-1;i++)
		{
			m_dotArray[i]=NewArray[i];
		}
		
		NewArray.RemoveAll();
	}
	
	return;
}

BOOL CStroke::IsEmpty()
{
	int nCount = m_dotArray.GetSize();
	if(nCount  == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CStroke::DotInQuad(CQuad &quad)
{
	int nCount = m_dotArray.GetSize();
	CDot dot;
	
	for(int i=0;i<nCount;i++)
	{
		dot = m_dotArray.GetAt(i);
		if(quad.DotIn(dot))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CStroke::HasDot(CQuad &quad)//�ж��Ƿ�����˵�
{
	int nCount=m_dotArray.GetSize();
	if(nCount<1)
	{
		return FALSE;
	}
	
	DOT dot;
	BOOL bFlag=FALSE;
	for(int i=0;i<nCount;i++)
	{
		dot=m_dotArray.GetAt(i);
		if(quad.left<=dot.x && dot.x<=quad.right && quad.bottom<=dot.y && dot.y<=quad.top)
		{
			bFlag=TRUE;
			break;
		}
	}
	
	return bFlag;
}

void CStroke::LogToTrue()
{
	for(int i=0;i<m_dotArray.GetSize();i++)
	{
		m_dotArray[i].LogToTrue();
	}
	
	return;
}

void CStroke::SetDot(unsigned int index, CDot dot)
{
	ASSERT(index <= (unsigned int)(m_dotArray.GetSize()));
	m_dotArray.SetAt(index,dot);
	
	return;
}

void CStroke::Append(const CStroke &stroke)
{
    ASSERT(this != &stroke);
	
	m_dotArray.Append(stroke.m_dotArray);
	
	return;
}

void CStroke::RemoveAllPoint()
{
	m_dotArray.RemoveAll();
	
	return;
}

void CStroke::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScale100)
{
	if(m_dotArray.GetSize()<1)
	{
		return;
	}
	
	//����
	CString str;
	str.Format("SP%d;",m_property.m_nLayer);
	file.WriteString(str);
	
	//���
	CDot Dot ;
	Dot    = m_dotArray[0];
	Dot.x -= CENTER;
	Dot.y -= CENTER;
	
	if (bScale100)
	{
		Dot.x=4000.0*Dot.x;
		Dot.y=4000.0*Dot.y;
	}
	else
	{
		Dot.x=40*Dot.x;
		Dot.y=40*Dot.y;
	}
	
	//str.Format("PU%.0f,%.0f;",Dot.x,-1.*Dot.y);
	str.Format("PU%.0f,%.0f;",Dot.x, Dot.y);
	file.WriteString(str);
	
	//����
	for(int i=1;i<m_dotArray.GetSize();i++)	
	{
		Dot = m_dotArray[i];
		Dot.x-=CENTER;
		Dot.y-=CENTER;
		
		if (bScale100)
		{
			Dot.x=4000.0*Dot.x;
			Dot.y=4000.0*Dot.y;
		}
		else
		{
			Dot.x=40*Dot.x;
			Dot.y=40*Dot.y;
		}
		//	str.Format("PD%.0f,%.0f;",Dot.x,-1.*Dot.y);
		str.Format("PD%.0f,%.0f;",Dot.x,Dot.y);
		file.WriteString(str);
	}
	
	return;
}

BOOL CStroke::DotIsInStroke(CDot &dot)
{
	int nCount=m_dotArray.GetSize();
	if(nCount<1)
	{
		return FALSE;
	}
	
	DOT   dt;
	for(int i=0; i < nCount; i++)
	{
		dt  = m_dotArray.GetAt(i);
		if ((fabs(dot.x - dt.x) < EPSINON) && (fabs(dot.y - dt.y) < EPSINON))
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

// �˵�
void CStroke::FilterPoint(double dbPresic, double dbAngle)
{
	int nCount = m_dotArray.GetSize();
	if (nCount < 2)
	{
		return;
	}
	
	int nStart = 0;
	int nEnd   = 1;
	int nNext  = 2;
	// Ϊ�˱�������֮��ıպ��ԣ��׵���β�㲻��ȥ��
	while (nNext < m_dotArray.GetSize())
	{
		if (nNext > (m_dotArray.GetSize() - 1))
		{
			continue;
		}
		
		CDot sDot = m_dotArray[nStart];
		CDot eDot = m_dotArray[nEnd];
		CDot nDot = m_dotArray[nNext];
		double dbDs = Distance(sDot, eDot);
		if (dbDs > dbPresic)
		{
			nStart++;
			nEnd++;
			nNext++;
		}
		else
		{
			if (nEnd < (m_dotArray.GetSize() - 1))
			{
				double dbA = GetAngle(sDot, eDot, nDot);
				if (dbA > dbAngle) 
				{
					m_dotArray.RemoveAt(nEnd);
				}
				else
				{
					nStart++;
					nEnd++;
					nNext++;
				}
			}
			else
			{
				m_dotArray.RemoveAt(nStart);
			}
		}// end if
	}// end while
	
	return;
	
}

// ����
void CStroke::Reverse()
{
	int nCount = m_dotArray.GetSize();
	if (nCount < 0)
	{
		return;
	}
	
	CArray<CDot,CDot> dotArrayTemp;
	dotArrayTemp.RemoveAll();
	
	for (int i = (nCount - 1); i >= 0; i--) 
	{
		dotArrayTemp.Add(m_dotArray[i]);
	}
	
	m_dotArray.RemoveAll();
	m_dotArray.Append(dotArrayTemp);
	dotArrayTemp.RemoveAll();
	
	return;
}

// ����ָ�����߷���
void CStroke::SetStrokeReverse(CDot &dt)
{
	int nCount = m_dotArray.GetSize();
	if (nCount < 1)
	{
		return;
	}
	
	Reverse();
	
	return;
}

BOOL CStroke::KillAngle(int nNum, double dbSpace)
{
	DSF_TRACE("\nCStroke::MoveDot");
	int size=m_dotArray.GetSize();
	if (size<3)
	{
		return FALSE;
	}
	
	CDot aDot,bDot,cDot;
	BOOL bStartIndex=(nNum==0);
	BOOL bEndIndex=(nNum==(size-1));
	BOOL bClose=IsRegion();
	int aDotIndex=bStartIndex?((bClose?(size-2):(size-1))):(nNum-1);
	int bDotIndex=nNum;
	int cDotIndex=bEndIndex?(bClose?(1):(0)):(nNum+1);
	
	aDot=m_dotArray[aDotIndex];
	bDot=m_dotArray[bDotIndex];
	cDot=m_dotArray[cDotIndex];
	
	CStroke* pRtnStroke=new CStroke(m_property);
	if (NULL == pRtnStroke)
	{
		return FALSE;
	}
	
	if(CalArcJoint(aDot, bDot, cDot, dbSpace, pRtnStroke))
	{
		if (bStartIndex&&bClose)
		{
			m_dotArray.RemoveAt(size-1);			
		}
		
		int iRtnSize=pRtnStroke->GetSize();
		m_dotArray.SetAt(aDotIndex,pRtnStroke->GetAt(0));
		m_dotArray.SetAt(cDotIndex,pRtnStroke->GetAt(iRtnSize-1));
		m_dotArray.RemoveAt(bDotIndex);
		
		for (int i=1;i<iRtnSize-1;i++)
		{
			m_dotArray.InsertAt(bDotIndex+i-1,pRtnStroke->GetAt(i));
		}
		
		if (bStartIndex&&bClose)
		{
			m_dotArray.InsertAt(0,pRtnStroke->GetAt(0));
		}
		
		if (bEndIndex&&bClose)
		{
			m_dotArray.SetAt(0,pRtnStroke->GetAt(0));
		}
	}
	else
	{
	}
	
	if (pRtnStroke)
	{
		delete pRtnStroke;
		pRtnStroke = NULL;
	}
	
	return TRUE;
	
}

double CStroke::GetLength()
{
	int count = m_dotArray.GetSize();
	if(count < 2) return 0.0;
	double total = 0.0;
	for (int i=0; i<count-1; i++)
	{
		double x = m_dotArray[i].x - m_dotArray[i+1].x;
		double y = m_dotArray[i].y - m_dotArray[i+1].y;
		total += sqrt(x*x + y*y);
	}
	return total;
}

//���ܣ������ཻ�߶ε�Բ������
BOOL CStroke::CalArcJoint(CDot aDot,CDot bDot,CDot cDot,double dbPrecision,CStroke* pRtnStroke)
{
	//CDot a1Dot,b1Dot,c1Dot; //����ϵ�任���λ������
	//��������ϵƽ�Ƶ���Ϣ
	double dbbDotX=bDot.x;
	double dbbDotY=bDot.y;
	double dbAngleXBA=CalLineAngle(bDot,aDot);
	//����ƽ��
	TransCoord(dbbDotX,dbbDotY,dbAngleXBA,FALSE,aDot.x,aDot.y);
	TransCoord(dbbDotX,dbbDotY,dbAngleXBA,FALSE,bDot.x,bDot.y);
	TransCoord(dbbDotX,dbbDotY,dbAngleXBA,FALSE,cDot.x,cDot.y);
	//��������ϵ����Բ�����ɵ�Բ��
	double dbAngleABC=CalLineAngle(bDot,cDot);
	if (SAMEVALUE(dbAngleABC,PAI)||SAMEVALUE(dbAngleABC,0))
	{
		return	FALSE;
	}
	double dbLAB=CalDistance(aDot,bDot);
	double dbLBC=CalDistance(bDot,cDot);
	BOOL bANearPt=dbLAB<dbLBC; //A���Ƿ�Ϊ�����(��A��B)
	double dbL=bANearPt?dbLAB:dbLBC;
	dbL=dbL/cos(dbAngleABC/2);
	double dbCircleX=dbL*cos(dbAngleABC/2);
	double dbCircleY=dbL*sin(dbAngleABC/2);
	double dbCircleR=fabs(dbCircleY);
	//Բ����Ϊ�ȼ��ֱ�߶�
	double dbStepAngle=fabs(dbPrecision/dbCircleR);
	int nStepCnt=int((PAI-fabs(dbAngleABC))/dbStepAngle+0.5);
	if (nStepCnt==0)
	{
		nStepCnt=1;
	}
	dbStepAngle=(PAI-fabs(dbAngleABC))/nStepCnt;
	double dbStartAngle=0;//ARC��ʼ�Ƕ�
	
	if (dbAngleABC>=0)
	{
		dbStartAngle=PAI/(-2.0);
		dbStepAngle*=(-1.0);
	}
	else
	{
		dbStartAngle=PAI/2;
		//dbStepAngle*=(1.0);
	}
	
	CDot dotTmp;
	double dbAngleTmp=0;
	pRtnStroke->RemoveAllPoint();	
	for(int i=0;i<=nStepCnt;i++)
	{
		dbAngleTmp=dbStepAngle*i+dbStartAngle;
		dotTmp.x=dbCircleR*cos(dbAngleTmp)+dbCircleX;
		dotTmp.y=dbCircleR*sin(dbAngleTmp)+dbCircleY;
		//�任��ԭ����ϵ
		TransCoord(dbbDotX,dbbDotY,dbAngleXBA,TRUE,dotTmp.x,dotTmp.y);
		pRtnStroke->Add(dotTmp);
	}
	//��������
	if (bANearPt)
	{
		TransCoord(dbbDotX,dbbDotY,dbAngleXBA,TRUE,cDot.x,cDot.y);
		pRtnStroke->Add(cDot);
	}
	else
	{
		TransCoord(dbbDotX,dbbDotY,dbAngleXBA,TRUE,aDot.x,aDot.y);
		pRtnStroke->AddHead(aDot);
	}
	return TRUE;
}

CDot CStroke::GetFirstDot()
{
	return m_dotArray[0];

}

CDot CStroke::GetLastDot()
{
	int n = m_dotArray.GetSize();
	return m_dotArray[n-1];
}

void CStroke::GetStrokeInQuad(CQuad &quad, StrokeList &strokeList)
{
	if(m_dotArray.GetSize()==0)
		return;

	if (m_dotArray.GetSize()==1)
	{
		strokeList.AddTail(Copy());
		return;
	}

	CDot dotStart, dotEnd, dotPrev, dotLast, dotTemp;
	dotPrev = m_dotArray.GetAt(0);
	dotPrev.Round(3);
	
	CStroke *pStroke=NULL;
	CDot *pdotMin, *pdotMax;
	for(int i=1;i<m_dotArray.GetSize();i++)
	{
		dotStart = dotPrev;
		dotEnd	= m_dotArray.GetAt(i);
		dotEnd.Round(3);
		dotPrev = dotEnd;

		//�ڲ������ϡ��¹�ϵ������£��ȶ��߶ν���X����ض�
		if(dotStart.x > dotEnd.x) //����ֵ����
		{	
			pdotMax = &dotStart;
			pdotMin = &dotEnd;
		}
		else
		{
			pdotMax = &dotEnd;
			pdotMin = &dotStart;
		}

		if (pdotMax->x < quad.left || pdotMin->x >= quad.right)		//�ų����ҷ�Χ������
			continue;
		else if(pdotMax->x == quad.left && pdotMin->x != quad.left)		//�ų���ˮƽ�����һ��������ཻ�����
			continue;

		if( pdotMin->x < quad.left)	//��߽ض�
		{
			dotTemp.x = quad.left;		//�����ܴ��� dotEnd.x==dotStart.x
			dotTemp.y = dotEnd.y - (dotEnd.x - dotTemp.x)*(dotEnd.y - dotStart.y)/(dotEnd.x - dotStart.x);
			*pdotMin = dotTemp;
		}
		if(pdotMax->x > quad.right)	//�ұ߽ض�
		{
			dotTemp.x = quad.right;
			dotTemp.y = dotEnd.y - (dotEnd.x - dotTemp.x)*(dotEnd.y - dotStart.y)/(dotEnd.x - dotStart.x);
			*pdotMax = dotTemp;
		}

		//���߶ν���Y����ض�
		if(dotStart.y > dotEnd.y)
		{
			pdotMax = &dotStart;
			pdotMin = &dotEnd;
		}
		else
		{
			pdotMax = &dotEnd;
			pdotMin = &dotStart;
		}

		if(pdotMin->y > quad.top  || pdotMax->y <= quad.bottom)
			continue;
		else if(pdotMin->y == quad.top && pdotMax->y != quad.top) 
			continue;

		if(pdotMin->y < quad.bottom)
		{
			dotTemp.y = quad.bottom;
			dotTemp.x = dotEnd.x - (dotEnd.x - dotStart.x)*(dotEnd.y - dotTemp.y)/(dotEnd.y-dotStart.y);
			*pdotMin = dotTemp;
		}
		if (pdotMax->y > quad.top)
		{
			dotTemp.y = quad.top;
			dotTemp.x = dotEnd.x - (dotEnd.x - dotStart.x)*(dotEnd.y - dotTemp.y)/(dotEnd.y-dotStart.y);
			*pdotMax = dotTemp;
		}

		if (pStroke == NULL)
		{
			pStroke = new CStroke(m_property);
			pStroke->m_nFocusType = m_nFocusType;
			pStroke->m_nResinRect = m_nResinRect;
			m_dbQFre	= pStroke->m_dbQFre;
			pStroke->Add(dotStart);
			pStroke->Add(dotEnd);
		}
		else
		{
			//����Ƿ���������
			dotLast = pStroke->GetLastDot();
			if ( dotLast==dotStart)
			{
				pStroke->Add(dotEnd);
			}
			else if (dotLast==dotEnd)	//����ĳ�ߺ����������ĳ���ۻ�������Ϊ�����Ч�ʣ�����ȡ��
			{
				pStroke->Add(dotStart);
			}
			else
			{
				strokeList.AddTail(pStroke);
				pStroke = NULL;
				pStroke = new CStroke(m_property);
				pStroke->m_nFocusType = m_nFocusType;
				pStroke->m_nResinRect = m_nResinRect;
				pStroke->m_dbQFre = m_dbQFre;
				pStroke->Add(dotStart);
				pStroke->Add(dotEnd);
			}
		}		
	}

	if (pStroke != NULL)
	{
		strokeList.AddTail(pStroke);
		pStroke = NULL;
	}	
}

void CStroke::GetStrokeInQuadFromStrokeList(StrokeList &srcList, StrokeList &desList, CQuad &quad, CProperty &property, int nResinRect)
{
	StrokeList tempList;
	BOOL bWholeStroke = quad.IsEmpty();

	CLay* play = g_DAT.GetCurLay();
	int nRepeat =  play->GetRepeat(property.m_nLayer);
	int nFocusType = play->GetFocusType(property.m_nLayer);
	int nCountDelay = play->GetCountDelay(property.m_nLayer);
	if (nRepeat <= 0)
		return;
	
	CStroke *pStroke=NULL;
	POSITION pos = srcList.GetHeadPosition();
	while(pos)
	{
		pStroke = srcList.GetNext(pos);
		pStroke->m_nFocusType = nFocusType;
		pStroke->m_nResinRect = nResinRect;

		if (bWholeStroke)
			tempList.AddTail(pStroke->Copy());
		else
			pStroke->GetStrokeInQuad(quad, tempList);
	}

	if (tempList.GetCount()>0)
	{
		pos = tempList.GetHeadPosition();
		while(pos)
		{
			pStroke = tempList.GetNext(pos);
			desList.AddTail(pStroke);
		}
		
		for (int n=1; n<nRepeat; n++)
		{	
			//��ʱ����
			pStroke = new CStroke(property);
			pStroke->m_nFocusType = nFocusType;
			pStroke->m_nCountDealy = nCountDelay;
			pStroke->m_nResinRect = nResinRect;
			desList.AddTail(pStroke);

			//��������
			pos = tempList.GetHeadPosition();
			while(pos)
			{
				pStroke = tempList.GetNext(pos);
				desList.AddTail(pStroke->Copy());
			}
		}
	}
}