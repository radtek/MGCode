// FntTTF.cpp: implementation of the CFntTTF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "FntTTF.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFntTTF,CObject,2000)

CFntTTF::CFntTTF()
{

}

CFntTTF::~CFntTTF()
{

}

BOOL CFntTTF::MakeStroke()
{	
	if(m_bFixSpace)
	{
		return MakeFixStroke();
	}
	else
	{
		return MakeOldStroke();
	}
}

/*
BOOL CFntTTF::MakeOldStroke()
{
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	if(dc.GetMapMode()!=MM_TEXT)
		return FALSE;

	//////////////////
	//以pes个逻辑单位为基准画文本
	int pes=100;
	int h=-MulDiv(pes, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 72);

	//缺省双字节字体，也即系统字体
	CFont tmp;
	tmp.CreateStockObject( DEFAULT_GUI_FONT);
	LOGFONT lf2;
	tmp.GetLogFont(&lf2);
	tmp.DeleteObject();

	lf2.lfHeight=h;
	CFont font2;
	font2.CreateFontIndirect(&lf2);

	//////////////////////////
	LOGFONT lf=m_lf;
	lf.lfHeight = h;
	CFont font;
	font.CreateFontIndirect(&lf);
	CFont *pOldFont=dc.SelectObject(&font);

	UINT flag=dc.GetTextAlign();
	flag=TA_UPDATECP|TA_TOP;
	flag=dc.SetTextAlign(flag);
		 
	int mode=dc.SetBkMode(TRANSPARENT);
	dc.BeginPath();
	
	//字间距，行间距
	CPoint pt;	
	CSize sz=dc.GetTextExtent(_T("1")) ;
	int cs=(int)(pes*m_dbCharSpace/m_dbWidthFactor/m_dbHeight);
	int ls=(int)(pes*m_dbLineSpace/m_dbHeight+sz.cy);
	int count=m_strText.GetLength();
	dc.MoveTo(0,-sz.cy);
	for(int i=0;i<count;i++)
	{	
		if(m_strText[i]==0x0D)
		{
			//回车换行处理
			pt=dc.GetCurrentPosition();
			pt.x=0;
			pt.y=pt.y+(int)ls;
			dc.MoveTo(pt);
			i++;
		}
		else
		{
			if(IsDBCSLeadByte(m_strText[i]))
			{
				//双字节字符
				if(lf.lfCharSet!=lf2.lfCharSet)
				{
					CFont *p=dc.SelectObject(&font2);
					dc.TextOut(0,0,LPCTSTR(m_strText)+i,2);
					i++;
					dc.SelectObject(p);
				}
				else
				{
					dc.TextOut(0,0,LPCTSTR(m_strText)+i,2);
					i++;
				}
			}
			else
				dc.TextOut(0,0,LPCTSTR(m_strText)+i,1);

			pt=dc.GetCurrentPosition();
			pt.x=pt.x+(int)cs;
			dc.MoveTo(pt);
		}
	}
	dc.EndPath();
	dc.SetBkMode(mode);

	dc.SelectObject(pOldFont);
	font.DeleteObject();
	font2.DeleteObject();
	dc.SetTextAlign(flag);

	//检取路径数据
	dc.FlattenPath();
	int nCount=dc.GetPath(NULL,NULL,0);
	CPoint* points=new CPoint[nCount];
	BYTE* bytes=new BYTE[nCount];
	dc.GetPath(points,bytes,nCount);

	CStroke *pStroke=NULL;
	CPoint ptStart;
	BOOL bFirst=FALSE;
	for(i=0;i<nCount;i++)
	{
		switch(bytes[i])
		{
		//移动当前点位置
		case PT_MOVETO:
			bFirst=TRUE;
			ptStart=points[i];
			break;
		//画直线
		case PT_LINETO:
			if(bFirst)
			{
				pStroke=new CStroke(m_property);
				pStroke->Add(ptStart);
				m_list.AddTail(pStroke);
				bFirst=FALSE;
			}
			pStroke->Add(points[i]);
			break;
		//画直线并封闭图形
		case PT_LINETO|PT_CLOSEFIGURE:
			pStroke->Add(points[i]);
			pStroke->Add(ptStart);
			break;
		}
	}
	delete []points;
	delete []bytes;

	dc.DeleteDC();

	//座标反转
	//还原为设定高度,宽度系数
	double ratioY=-m_dbHeight/pes;
	double ratioX=-ratioY*m_dbWidthFactor;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->Scale(CDot(0,0),ratioX,ratioY);
	}

	return TRUE;
}
*/


BOOL CFntTTF::MakeOldStroke()
{
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	if(dc.GetMapMode()!=MM_TEXT)
	{
		return FALSE;
	} 

	//////////////////
	//以pes个逻辑单位为基准画文本
	int pes=100;
	int nK = atoi(m_strPrecisK);

	int h=-MulDiv(pes,nK,20);
	/************************* Begin *****************************************/
	// 日期：2015/7/29  时间：11:36:36 by xief     
	h=-MulDiv(pes, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 72);
	/************************* End   *****************************************/
	

	//缺省双字节字体，也即系统字体
	CFont tmp;
	tmp.CreateStockObject( DEFAULT_GUI_FONT);
	LOGFONT lf2;
	tmp.GetLogFont(&lf2);
	tmp.DeleteObject();

	lf2.lfHeight=h;
	CFont font2;
	font2.CreateFontIndirect(&lf2);

	//////////////////////////
	LOGFONT lf=m_lf;
	lf.lfHeight = h;
	CFont font;
	font.CreateFontIndirect(&lf);
	CFont *pOldFont=dc.SelectObject(&font);

	UINT flag=dc.GetTextAlign();
	flag=TA_UPDATECP|TA_TOP;
	flag=dc.SetTextAlign(flag);
		 
	int mode=dc.SetBkMode(TRANSPARENT);
	dc.BeginPath();
	
	//字间距，行间距
	CPoint pt;	
	CSize sz=dc.GetTextExtent(_T("1")) ;
	int cs=(int)(pes*m_dbCharSpace/m_dbWidthFactor/m_dbHeight);
	int ls=(int)(pes*m_dbLineSpace/m_dbHeight+sz.cy);
	int count=m_strText.GetLength();
	dc.MoveTo(0,-sz.cy);
	for(int i=0;i<count;i++)
	{	
		if(m_strText[i]==0x0D)
		{
			//回车换行处理
			pt=dc.GetCurrentPosition();
			pt.x=0;
			pt.y=pt.y+(int)ls;
			dc.MoveTo(pt);
			i++;
		}
		else
		{
			if(IsDBCSLeadByte(m_strText[i]))
			{
				//双字节字符
				if(lf.lfCharSet!=lf2.lfCharSet)
				{
					CFont *p=dc.SelectObject(&font2);
					dc.TextOut(0,0,LPCTSTR(m_strText)+i,2);
					i++;
					dc.SelectObject(p);
				}
				else
				{
					dc.TextOut(0,0,LPCTSTR(m_strText)+i,2);
					i++;
				}
			}
			else
				dc.TextOut(0,0,LPCTSTR(m_strText)+i,1);

			pt=dc.GetCurrentPosition();
			pt.x=pt.x+(int)cs;
			dc.MoveTo(pt);
		}
	}
	dc.EndPath();
	dc.SetBkMode(mode);

	dc.SelectObject(pOldFont);
	font.DeleteObject();
	font2.DeleteObject();
	dc.SetTextAlign(flag);

	//检取路径数据
	dc.FlattenPath();
	int nCount=dc.GetPath(NULL,NULL,0);
	CPoint* points=new CPoint[nCount];
	BYTE* bytes=new BYTE[nCount];
	dc.GetPath(points,bytes,nCount);

	CStroke *pStroke=NULL;
	CPoint ptStart;
	BOOL bFirst=FALSE;
	for(int i=0;i<nCount;i++)
	{
		switch(bytes[i])
		{
		//移动当前点位置
		case PT_MOVETO:
			bFirst=TRUE;
			ptStart=points[i];
			break;
		//画直线
		case PT_LINETO:
			if(bFirst)
			{
				pStroke=new CStroke(m_property);
				pStroke->Add(ptStart);
				m_list.AddTail(pStroke);
				bFirst=FALSE;
			}
			pStroke->Add(points[i]);
			break;
		//画直线并封闭图形
		case PT_LINETO|PT_CLOSEFIGURE:
			pStroke->Add(points[i]);
			pStroke->Add(ptStart);
			break;
		}
	}
	delete []points;
	delete []bytes;

	dc.DeleteDC();

	//座标反转
	//还原为设定高度,宽度系数
	double ratioY=-m_dbHeight/(pes*abs(h)*0.007418);
	double ratioX=-ratioY*m_dbWidthFactor;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->Scale(CDot(0,0),ratioX,ratioY);
	}

	return TRUE;
}


BOOL CFntTTF::MakeFixStroke()
{
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	if(dc.GetMapMode()!=MM_TEXT)
	{
		return FALSE;
	}

	//////////////////
	//以pes个逻辑单位为基准画文本
	int pes=100;
	int nK = atoi(m_strPrecisK);
	int h=-MulDiv(pes,nK,20);
	/************************* Begin *****************************************/
	// 日期：2015/7/29  时间：11:29:35 by xief     
	h=-MulDiv(pes, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 72);
	/************************* End   *****************************************/
	

	//缺省双字节字体，也即系统字体
	CFont tmp;
	tmp.CreateStockObject( DEFAULT_GUI_FONT);
	LOGFONT lf2;
	tmp.GetLogFont(&lf2);
	tmp.DeleteObject();

	lf2.lfHeight=h;
	CFont font2;
	font2.CreateFontIndirect(&lf2);

	//////////////////////////
	LOGFONT lf=m_lf;
	lf.lfHeight = h;
	CFont font;
	font.CreateFontIndirect(&lf);
	CFont *pOldFont=dc.SelectObject(&font);

	UINT flag=dc.GetTextAlign();
	flag=TA_UPDATECP|TA_TOP;
	flag=dc.SetTextAlign(flag);
		 
	int mode=dc.SetBkMode(TRANSPARENT);
	dc.BeginPath();
	
	//字间距，行间距
	CPoint pt;	
	CSize sz=dc.GetTextExtent(_T("H")) ;
	int cs=(int)(pes*m_dbCharSpace/m_dbWidthFactor/m_dbHeight);
	int ls=(int)(pes*m_dbLineSpace/m_dbHeight+sz.cy);
	int count=m_strText.GetLength();
	dc.MoveTo(0,-sz.cy);
	pt = dc.GetCurrentPosition();
	for(int i=0;i<count;i++)
	{	
		if(m_strText[i]==0x0D)
		{
			//回车换行处理
			pt=dc.GetCurrentPosition();
			pt.x=0;
			pt.y=pt.y+(int)ls;
			dc.MoveTo(pt);
			i++;
		}
		else
		{
			if(IsDBCSLeadByte(m_strText[i]))
			{
				//双字节字符
				if(lf.lfCharSet!=lf2.lfCharSet)
				{
					CFont *p=dc.SelectObject(&font2);
					dc.TextOut(0,0,LPCTSTR(m_strText)+i,2);
					i++;
					dc.SelectObject(p);
				}
				else
				{
					dc.TextOut(0,0,LPCTSTR(m_strText)+i,2);
					i++;
				}
			}
			else
				dc.TextOut(0,0,LPCTSTR(m_strText)+i,1);

			pt.x=pt.x+sz.cx+(int)cs;
			dc.MoveTo(pt);
		}
	}
	dc.EndPath();
	dc.SetBkMode(mode);

	dc.SelectObject(pOldFont);
	font.DeleteObject();
	font2.DeleteObject();
	dc.SetTextAlign(flag);

	//检取路径数据
	dc.FlattenPath();
	int nCount=dc.GetPath(NULL,NULL,0);
	CPoint* points=new CPoint[nCount];
	BYTE* bytes=new BYTE[nCount];
	dc.GetPath(points,bytes,nCount);

	CStroke *pStroke=NULL;
	CPoint ptStart;
	BOOL bFirst=FALSE;
	for(int i=0;i<nCount;i++)
	{
		switch(bytes[i])
		{
		//移动当前点位置
		case PT_MOVETO:
			bFirst=TRUE;
			ptStart=points[i];
			break;
		//画直线
		case PT_LINETO:
			if(bFirst)
			{
				pStroke=new CStroke(m_property);
				pStroke->Add(ptStart);
				m_list.AddTail(pStroke);
				bFirst=FALSE;
			}
			pStroke->Add(points[i]);
			break;
		//画直线并封闭图形
		case PT_LINETO|PT_CLOSEFIGURE:
			pStroke->Add(points[i]);
			pStroke->Add(ptStart);
			break;
		}
	}
	delete []points;
	delete []bytes;

	dc.DeleteDC();

	//座标反转
	//还原为设定高度,宽度系数
	double ratioY=-m_dbHeight/(pes*abs(h)*0.007418);
	double ratioX=-ratioY*m_dbWidthFactor;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->Scale(CDot(0,0),ratioX,ratioY);
	}

	return TRUE;
}


void CFntTTF::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		//ar<<m_lf;
	}
	else
	{
		// TODO: add loading code here
		//ar>>m_lf;
	}
}
