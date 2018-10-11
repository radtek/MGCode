// DrawPolygonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DrawPolygonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawPolygonDlg dialog


CDrawPolygonDlg::CDrawPolygonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawPolygonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawPolygonDlg)
	//}}AFX_DATA_INIT
	m_bFlag=TRUE;
}


void CDrawPolygonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawPolygonDlg)
	DDX_Control(pDX, IDC_SPIN_NUM, m_spinNum);
	DDX_Control(pDX, IDC_SLIDER_OPTION, m_sliderOption);
	DDX_Text(pDX, IDC_EDIT_NUM, m_nNum);
	DDX_Radio(pDX, IDC_RADIO_POLYGON_LINE, m_nPolygon);
	DDX_Text(pDX, IDC_EDIT_OPTION, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawPolygonDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawPolygonDlg)
	ON_BN_CLICKED(IDC_RADIO_POLYGON_CROSS, OnRadioPolygonCross)
	ON_BN_CLICKED(IDC_RADIO_POLYGON_LINE, OnRadioPolygonLine)
	ON_BN_CLICKED(IDC_RADIO_POLYGON_STAR, OnRadioPolygonStar)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_OPTION, OnKillfocusEditOption)
	ON_EN_CHANGE(IDC_EDIT_NUM, OnChangeEditNum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawPolygonDlg message handlers


void CDrawPolygonDlg::UpdateView()
{
	if(m_nNum>500)
		m_nNum=500;

	switch(m_nPolygon)
	{
	case POLYGON_LINE:
		if(m_nNum<3)
			m_nNum=3;
		m_sliderOption.ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_OPTION)->ShowWindow(FALSE);
		break;
	case POLYGON_STAR:
		if(m_nNum<3)
			m_nNum=3;
		m_sliderOption.ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_OPTION)->ShowWindow(TRUE);
		m_sliderOption.SetTicFreq(10);
		m_sliderOption.SetRange(0,100,TRUE);
		m_sliderOption.SetPageSize(10);

		break;
	case POLYGON_CROSS:
		if(m_nNum<5)
			m_nNum=5;
		int max;
		max=(m_nNum+1)/2-2;

		m_sliderOption.ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_OPTION)->ShowWindow(TRUE);
		m_sliderOption.SetTicFreq(1);
		m_sliderOption.SetRange(1,max,TRUE);
		m_sliderOption.SetPageSize(1);
		break;
	default:
		return;
	}

	int min=m_sliderOption.GetRangeMin();
	int max=m_sliderOption.GetRangeMax();
	if(m_nOption<min)
		m_nOption=min;
	if(m_nOption>max)
		m_nOption=max;
	m_sliderOption.SetPos(m_nOption);


	UpdateData(FALSE);
}

void CDrawPolygonDlg::OnRadioPolygonCross() 
{
	// TODO: Add your control notification handler code here
	int flag;
	flag=m_nPolygon;
	UpdateData(TRUE);

	if(flag!=m_nPolygon)
	{
		m_nStar=m_nOption;
		m_nOption=m_nCross;
		UpdateView();
	}
}

void CDrawPolygonDlg::OnRadioPolygonLine() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateView();
}

void CDrawPolygonDlg::OnRadioPolygonStar() 
{
	// TODO: Add your control notification handler code here
	int flag;
	flag=m_nPolygon;
	UpdateData(TRUE);

	if(flag!=m_nPolygon)
	{
		m_nCross=m_nOption;
		m_nOption=m_nStar;
		UpdateView();
	}
	
}

BOOL CDrawPolygonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_spinNum.SetRange(3,500);
	if(m_nPolygon==POLYGON_STAR)
		m_nOption=m_nStar;
	if(m_nPolygon==POLYGON_CROSS)
		m_nOption=m_nCross;

	UpdateView();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CDrawPolygonDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl *pSlider=(CSliderCtrl *)pScrollBar;
	if(pSlider!=&m_sliderOption)
		return;

	int page=pSlider->GetPageSize();
	int min=pSlider->GetRangeMin();
	int max=pSlider->GetRangeMax();

	switch(nSBCode)
	{
	case TB_LINEDOWN:
		m_nOption++;
		break;
	case TB_LINEUP:
		m_nOption--;
		break;
	case TB_PAGEDOWN:
		m_nOption+=page;
		break;
	case TB_PAGEUP:
		m_nOption-=page;
		break;
	case TB_THUMBPOSITION:
		m_nOption=nPos;
		break;
	}

	if(m_nOption<min)
		m_nOption=min;
	if(m_nOption>max)
		m_nOption=max;

	UpdateData(FALSE);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}



void CDrawPolygonDlg::OnKillfocusEditOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	int min=m_sliderOption.GetRangeMin();
	int max=m_sliderOption.GetRangeMax();
	if(m_nOption<min)
		m_nOption=min;
	if(m_nOption>max)
		m_nOption=max;
	m_sliderOption.SetPos(m_nOption);
	UpdateData(FALSE);
}

void CDrawPolygonDlg::OnChangeEditNum() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if(m_bFlag)
	{
		m_bFlag=FALSE;
		return;
	}

	UpdateData(TRUE);
	UpdateView();
}

void CDrawPolygonDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(m_nPolygon==POLYGON_STAR)
		m_nStar=m_nOption;
	if(m_nPolygon==POLYGON_CROSS)
		m_nCross=m_nOption;

	CDialog::OnOK();
}
