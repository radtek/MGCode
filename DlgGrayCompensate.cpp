// DlgGrayCompensate.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgGrayCompensate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGrayCompensate dialog


CDlgGrayCompensate::CDlgGrayCompensate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGrayCompensate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGrayCompensate)
	m_nDest0=m_nSource0=0;
	m_nDest1=m_nSource1=255*1/5;
	m_nDest2=m_nSource2=255*2/5;
	m_nDest3=m_nSource3=255*3/5;
	m_nDest4=m_nSource4=255*4/5;
	m_nDest5=m_nSource5=255;
	//}}AFX_DATA_INIT
}


void CDlgGrayCompensate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGrayCompensate)
	DDX_Control(pDX, IDC_SLIDER5, m_SliderCtrl5);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderCtrl4);
	DDX_Control(pDX, IDC_SLIDER3, m_SliderCtrl3);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderCtrl2);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderCtrl1);
	DDX_Control(pDX, IDC_SLIDER0, m_SliderCtrl0);
	DDX_Control(pDX, IDC_PIC_GrayResult, m_PicGrayResult);
	DDX_Control(pDX, IDC_PIC_GraySource, m_PicGraySource);
	DDX_Text(pDX, IDC_EDIT_Dest0, m_nDest0);
	DDV_MinMaxUInt(pDX, m_nDest0, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Dest1, m_nDest1);
	DDV_MinMaxUInt(pDX, m_nDest1, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Dest2, m_nDest2);
	DDV_MinMaxUInt(pDX, m_nDest2, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Dest3, m_nDest3);
	DDV_MinMaxUInt(pDX, m_nDest3, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Dest4, m_nDest4);
	DDV_MinMaxUInt(pDX, m_nDest4, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Dest5, m_nDest5);
	DDV_MinMaxUInt(pDX, m_nDest5, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Soure0, m_nSource0);
	DDV_MinMaxUInt(pDX, m_nSource0, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Soure1, m_nSource1);
	DDV_MinMaxUInt(pDX, m_nSource1, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Soure2, m_nSource2);
	DDV_MinMaxUInt(pDX, m_nSource2, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Soure3, m_nSource3);
	DDV_MinMaxUInt(pDX, m_nSource3, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Soure4, m_nSource4);
	DDV_MinMaxUInt(pDX, m_nSource4, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Soure5, m_nSource5);
	DDV_MinMaxUInt(pDX, m_nSource5, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGrayCompensate, CDialog)
	//{{AFX_MSG_MAP(CDlgGrayCompensate)
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_BTN_RESTORE, OnBtnRestore)
	ON_EN_KILLFOCUS(IDC_EDIT_Dest0, OnKillfocusEDITDest0)
	ON_EN_KILLFOCUS(IDC_EDIT_Dest1, OnKillfocusEDITDest1)
	ON_EN_KILLFOCUS(IDC_EDIT_Dest2, OnKillfocusEDITDest2)
	ON_EN_KILLFOCUS(IDC_EDIT_Dest3, OnKillfocusEDITDest3)
	ON_EN_KILLFOCUS(IDC_EDIT_Dest4, OnKillfocusEDITDest4)
	ON_EN_KILLFOCUS(IDC_EDIT_Dest5, OnKillfocusEDITDest5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGrayCompensate message handlers

void CDlgGrayCompensate::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
// 	switch(nSBCode)   
// 	{   
// 		
// 	case   SB_THUMBTRACK: //拖动滚动滑块时   
		if(pScrollBar==(CScrollBar*)&m_SliderCtrl0)     //根据pScrollBar确定是那个滚动条   
		{     
			m_nDest0=m_SliderCtrl0.GetPos();
			CRect rect(0,0,50,30);
			UpdateGrayResultRect(rect,m_nDest0,m_nDest1,COLUMN_COUNT);	
		} 
		else if(pScrollBar==(CScrollBar*)&m_SliderCtrl1)     //根据pScrollBar确定是那个滚动条   
		{
			m_nDest1=m_SliderCtrl1.GetPos();
			CRect rect(0,0,50,30);
			UpdateGrayResultRect(rect,m_nDest0,m_nDest1,COLUMN_COUNT);	
			rect.left+=50;
			rect.right+=50;
			UpdateGrayResultRect(rect,m_nDest1,m_nDest2,COLUMN_COUNT);
		}
		else if(pScrollBar==(CScrollBar*)&m_SliderCtrl2)     //根据pScrollBar确定是那个滚动条   
		{
			m_nDest2=m_SliderCtrl2.GetPos();
			CRect rect(50,0,100,30);
			UpdateGrayResultRect(rect,m_nDest1,m_nDest2,COLUMN_COUNT);	
			rect.left=rect.right;
			rect.right+=50;
			UpdateGrayResultRect(rect,m_nDest2,m_nDest3,COLUMN_COUNT);
		}
		else if(pScrollBar==(CScrollBar*)&m_SliderCtrl3)     //根据pScrollBar确定是那个滚动条   
		{
			m_nDest3=m_SliderCtrl3.GetPos();
			CRect rect(100,0,150,30);
			UpdateGrayResultRect(rect,m_nDest2,m_nDest3,COLUMN_COUNT);	
			rect.left=rect.right;
			rect.right+=50;
			UpdateGrayResultRect(rect,m_nDest3,m_nDest4,COLUMN_COUNT);
		}
		else if(pScrollBar==(CScrollBar*)&m_SliderCtrl4)     //根据pScrollBar确定是那个滚动条   
		{
			m_nDest4=m_SliderCtrl4.GetPos();
			CRect rect(150,0,200,30);
			UpdateGrayResultRect(rect,m_nDest3,m_nDest4,COLUMN_COUNT);	
			rect.left=rect.right;
			rect.right+=50;
			UpdateGrayResultRect(rect,m_nDest4,m_nDest5,COLUMN_COUNT);
		}
		else if(pScrollBar==(CScrollBar*)&m_SliderCtrl5)     //根据pScrollBar确定是那个滚动条   
		{
			m_nDest5=m_SliderCtrl5.GetPos();
			CRect rect(200,0,250,30);
			UpdateGrayResultRect(rect,m_nDest4,m_nDest5,COLUMN_COUNT);	
		}
	/*	break;*/
/*	}    */
	UpdateData(FALSE);   
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDlgGrayCompensate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_SliderCtrl0.SetRange(0,255);
	m_SliderCtrl1.SetRange(0,255);
	m_SliderCtrl2.SetRange(0,255);
	m_SliderCtrl3.SetRange(0,255);
	m_SliderCtrl4.SetRange(0,255);
	m_SliderCtrl5.SetRange(0,255);
	
	

	m_SliderCtrl0.SetPos(m_nDest0);
	m_SliderCtrl1.SetPos(m_nDest1);
	m_SliderCtrl2.SetPos(m_nDest2);
	m_SliderCtrl3.SetPos(m_nDest3);
	m_SliderCtrl4.SetPos(m_nDest4);
	m_SliderCtrl5.SetPos(m_nDest5);

	m_PicGrayResult.SetWindowPos(NULL,18,18,254,33,SWP_DRAWFRAME|SWP_SHOWWINDOW);
	m_PicGraySource.SetWindowPos(NULL,18,208,254,33,SWP_DRAWFRAME|SWP_SHOWWINDOW);

	UpdateData(FALSE);
	return TRUE;  
}


void CDlgGrayCompensate::UpdateGrayResultRect(CRect rect,UINT nDestStart,UINT nDestEnd,int iCnt)
{
	CRect SlipRect;
	CBrush brush;
	CClientDC dc(&m_PicGrayResult);
	double dbStep=(double(nDestEnd)-nDestStart)/iCnt;
	double dbCurrent=double(nDestStart)+dbStep/2;
	double dbWidth=(double(rect.right)-rect.left)/iCnt;
	double dbCurrentLeft=double(rect.left);
	brush.CreateSolidBrush(RGB(int(dbCurrent),int(dbCurrent),int(dbCurrent)));
	SlipRect=rect;
	SlipRect.right=LONG(dbCurrentLeft+dbWidth);
	dc.FillRect(&SlipRect,&brush); 
	for (int i=1;i<iCnt;i++)
	{
		brush.DeleteObject();
		dbCurrent+=dbStep;
		dbCurrentLeft+=dbWidth;
		SlipRect.left=LONG(dbCurrentLeft);
		if (i==(iCnt-1))
		{
			SlipRect.right=rect.right;
		} 
		else
		{
			SlipRect.right=LONG(dbCurrentLeft+dbWidth);
		}
		brush.CreateSolidBrush(RGB(int(dbCurrent),int(dbCurrent),int(dbCurrent)));
		dc.FillRect(&SlipRect,&brush); 
	}
	return;
}

void CDlgGrayCompensate::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//draw(m_PicGrayResult)
	{     
		CRect rect(0,0,50,30);
		UpdateGrayResultRect(rect,m_nDest0,m_nDest1,COLUMN_COUNT);	
		rect.left+=50;
		rect.right+=50;
		UpdateGrayResultRect(rect,m_nDest1,m_nDest2,COLUMN_COUNT);	
		rect.left=rect.right;
		rect.right+=50;
		UpdateGrayResultRect(rect,m_nDest2,m_nDest3,COLUMN_COUNT);	
		rect.left=rect.right;
		rect.right+=50;
		UpdateGrayResultRect(rect,m_nDest3,m_nDest4,COLUMN_COUNT);	
		rect.left=rect.right;
		rect.right+=50;
		UpdateGrayResultRect(rect,m_nDest4,m_nDest5,COLUMN_COUNT);
	}
	//draw(m_PicGraySource)
	{     
		CRect rect(0,0,250,30);
		UpdateGraySourceRect(rect,0,255,COLUMN_COUNT*5);	
	}
	// Do not call CDialog::OnPaint() for painting messages
}


void CDlgGrayCompensate::UpdateGraySourceRect(CRect rect,UINT nDestStart,UINT nDestEnd,int iCnt)
{
	CRect SlipRect;
	CBrush brush;
	CClientDC dc(&m_PicGraySource);
	double dbStep=(double(nDestEnd)-nDestStart)/iCnt;
	double dbCurrent=double(nDestStart)+dbStep/2;
	double dbWidth=(double(rect.right)-rect.left)/iCnt;
	double dbCurrentLeft=double(rect.left);
	brush.CreateSolidBrush(RGB(int(dbCurrent),int(dbCurrent),int(dbCurrent)));
	SlipRect=rect;
	SlipRect.right=LONG(dbCurrentLeft+dbWidth);
	dc.FillRect(&SlipRect,&brush); 
	for (int i=1;i<iCnt;i++)
	{
		brush.DeleteObject();
		dbCurrent+=dbStep;
		dbCurrentLeft+=dbWidth;
		SlipRect.left=LONG(dbCurrentLeft);
		if (i==(iCnt-1))
		{
			SlipRect.right=rect.right;
		} 
		else
		{
			SlipRect.right=LONG(dbCurrentLeft+dbWidth);
		}
		brush.CreateSolidBrush(RGB(int(dbCurrent),int(dbCurrent),int(dbCurrent)));
		dc.FillRect(&SlipRect,&brush); 
	}
	return;
}

void CDlgGrayCompensate::OnBtnRestore() 
{
	// TODO: Add your control notification handler code here
	m_nDest0=m_nSource0=0;
	m_nDest1=m_nSource1=255*1/5;
	m_nDest2=m_nSource2=255*2/5;
	m_nDest3=m_nSource3=255*3/5;
	m_nDest4=m_nSource4=255*4/5;
	m_nDest5=m_nSource5=255;
	
	m_SliderCtrl0.SetPos(m_nSource0);
	m_SliderCtrl1.SetPos(m_nSource1);
	m_SliderCtrl2.SetPos(m_nSource2);
	m_SliderCtrl3.SetPos(m_nSource3);
	m_SliderCtrl4.SetPos(m_nSource4);
	m_SliderCtrl5.SetPos(m_nSource5);
	UpdateData(FALSE);
	OnPaint();
}

void CDlgGrayCompensate::OnKillfocusEDITDest0() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_SliderCtrl0.SetPos(m_nDest0);
	CRect rect(0,0,50,30);
	UpdateGrayResultRect(rect,m_nDest0,m_nDest1,COLUMN_COUNT);
	UpdateData(FALSE);
}

void CDlgGrayCompensate::OnKillfocusEDITDest1() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_SliderCtrl1.SetPos(m_nDest1);
	CRect rect(0,0,50,30);
	UpdateGrayResultRect(rect,m_nDest0,m_nDest1,COLUMN_COUNT);
	rect.left=rect.right;
	rect.right+=50;
	UpdateGrayResultRect(rect,m_nDest1,m_nDest2,COLUMN_COUNT);
	UpdateData(FALSE);
}

void CDlgGrayCompensate::OnKillfocusEDITDest2() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_SliderCtrl2.SetPos(m_nDest2);
	CRect rect(50,0,100,30);
	UpdateGrayResultRect(rect,m_nDest1,m_nDest2,COLUMN_COUNT);
	rect.left=rect.right;
	rect.right+=50;
	UpdateGrayResultRect(rect,m_nDest2,m_nDest3,COLUMN_COUNT);
	UpdateData(FALSE);
}

void CDlgGrayCompensate::OnKillfocusEDITDest3() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_SliderCtrl3.SetPos(m_nDest3);
	CRect rect(100,0,150,30);
	UpdateGrayResultRect(rect,m_nDest2,m_nDest3,COLUMN_COUNT);
	rect.left=rect.right;
	rect.right+=50;
	UpdateGrayResultRect(rect,m_nDest3,m_nDest4,COLUMN_COUNT);
	UpdateData(FALSE);
}

void CDlgGrayCompensate::OnKillfocusEDITDest4() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_SliderCtrl4.SetPos(m_nDest4);
	CRect rect(150,0,200,30);
	UpdateGrayResultRect(rect,m_nDest3,m_nDest4,COLUMN_COUNT);
	rect.left=rect.right;
	rect.right+=50;
	UpdateGrayResultRect(rect,m_nDest4,m_nDest5,COLUMN_COUNT);
	UpdateData(FALSE);
}

void CDlgGrayCompensate::OnKillfocusEDITDest5() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_SliderCtrl5.SetPos(m_nDest5);
	CRect rect(200,0,250,30);
	UpdateGrayResultRect(rect,m_nDest4,m_nDest5,COLUMN_COUNT);
	UpdateData(FALSE);
}
