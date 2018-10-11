// SelectCoordinary.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SelectCoordinary.h"
#include "HLDoc.h"
#include "HLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SelectCoordinary dialog


SelectCoordinary::SelectCoordinary(CWnd* pParent,double *pdbX,double *pdbY,double *pSizeX,double *pSizeY,CButton *pButt,int *pnIconType)
	: CDialog(SelectCoordinary::IDD, pParent)
{
	//{{AFX_DATA_INIT(SelectCoordinary)
	m_nBaseType = *pnIconType;
	//}}AFX_DATA_INIT
	m_pView=pParent;
	m_pdbX=pdbX;
	m_pdbY=pdbY;
	m_pSizeX=pSizeX;
	m_pSizeY=pSizeY;
	m_pButton=pButt;
	m_pIconType=pnIconType;
}


void SelectCoordinary::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SelectCoordinary)
	DDX_Radio(pDX, IDC_RADIO_BASE_TL, m_nBaseType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SelectCoordinary, CDialog)
	//{{AFX_MSG_MAP(SelectCoordinary)
	ON_BN_CLICKED(IDC_RADIO_BASE_BL, OnRadioBaseBl)
	ON_BN_CLICKED(IDC_RADIO_BASE_BM, OnRadioBaseBm)
	ON_BN_CLICKED(IDC_RADIO_BASE_BR, OnRadioBaseBr)
	ON_BN_CLICKED(IDC_RADIO_BASE_ML, OnRadioBaseMl)
	ON_BN_CLICKED(IDC_RADIO_BASE_MM, OnRadioBaseMm)
	ON_BN_CLICKED(IDC_RADIO_BASE_MR, OnRadioBaseMr)
	ON_BN_CLICKED(IDC_RADIO_BASE_TL, OnRadioBaseTl)
	ON_BN_CLICKED(IDC_RADIO_BASE_TM, OnRadioBaseTm)
	ON_BN_CLICKED(IDC_RADIO_BASE_TR, OnRadioBaseTr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SelectCoordinary message handlers

void SelectCoordinary::OnOK() 
{
	// TODO: Add extra validation here
	UpdateView();
	SetSelectIcon();
	CDialog::OnOK();
}

void SelectCoordinary::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void SelectCoordinary::OnRadioBaseBl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nBaseType=6;
}

void SelectCoordinary::OnRadioBaseBm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nBaseType=7;
}

void SelectCoordinary::OnRadioBaseBr() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nBaseType=8;
}

void SelectCoordinary::OnRadioBaseMl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nBaseType =3;
}

void SelectCoordinary::OnRadioBaseMm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nBaseType =4;
}

void SelectCoordinary::OnRadioBaseMr() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nBaseType=5;
}

void SelectCoordinary::OnRadioBaseTl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nBaseType = 0;
}

void SelectCoordinary::OnRadioBaseTm() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nBaseType=1;
}

void SelectCoordinary::OnRadioBaseTr() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nBaseType = 2;
}

void SelectCoordinary::UpdateView()
{
	//根据基本点位置确定XY坐标值
	CHLView *pHLView=(CHLView*)m_pView;
	CHLDoc *pDoc=(CHLDoc*)pHLView->GetDocument();
	CQuad quad=pDoc->GetLimit();
	CDot dot=quad.GetBaseDot(m_nBaseType);
	pDoc->m_nPosType=m_nBaseType;
	*m_pdbX=MyPrecision(dot.x);
	*m_pdbY=MyPrecision(dot.y);
	quad.Normal();
	*m_pSizeX=MyPrecision(quad.right-quad.left);
	*m_pSizeY=MyPrecision(quad.top-quad.bottom);
	UpdateData(FALSE);
}

void SelectCoordinary::SetSelectIcon()
{
	*m_pIconType=m_nBaseType;
	switch(m_nBaseType)
	{
	case BASE_TL:
		{
			HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON10);
			m_pButton->SetIcon(hIcon);
			break;
		}
	case BASE_TM:
		{
			HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON11);
			m_pButton->SetIcon(hIcon);
			break;
		}
	case BASE_TR:
		{
			HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON12);
			m_pButton->SetIcon(hIcon);
			break;
		}
	case BASE_ML:
		{
			HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON13);
			m_pButton->SetIcon(hIcon);
			break;
		}
	case BASE_MM:
		{
			HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON14);
			m_pButton->SetIcon(hIcon);
			break;
		}
	case BASE_MR:
		{
			HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON15);
			m_pButton->SetIcon(hIcon);
			break;
		}
	case BASE_BL:
		{
			HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON16);
			m_pButton->SetIcon(hIcon);
			break;
		}
	case BASE_BM:
		{
			HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON17);
			m_pButton->SetIcon(hIcon);
			break;
		}
	case BASE_BR:
		{
			HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON18);
			m_pButton->SetIcon(hIcon);
			break;
		}
	}
}