// DibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DibDlg.h"
#include "LPSetINF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void g_TESTCUR(double);
extern void g_TESTALL(double*,int);
extern void g_STOPTEST(void);
extern UINT g_GETCARDTYPE(void);


#define MAXDOTTIME 65535 //最大单点时间

/////////////////////////////////////////////////////////////////////////////
// CDibDlg dialog

CDibDlg::CDibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDibDlg::IDD, pParent)
	, m_dbMarkSpeed(0)
{
	//{{AFX_DATA_INIT(CDibDlg)
	m_strBmpName = _T("");
	m_bOppsitioncol = FALSE;
	m_dbSpaceX = 0.0;
	m_dbSpaceY = 0.0;
	m_dbDotTime = MAXDOTTIME;
	m_bLoop = FALSE;
	m_nDibType =0;
	m_dbQFre = 0.0;
	m_dbLoopCompate = 0.0f;
	m_nAccTime = 0;
	m_dbBrightness = 0.0;
	m_dbContrast = 0.0;
	m_nComSel = 0;
	m_dbQFre=0.0;
	//}}AFX_DATA_INIT
}

void CDibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDibDlg)
	DDX_Text(pDX, IDC_FILE_NAME, m_strBmpName);
	DDX_Check(pDX, IDC_OPPOSITION_COL, m_bOppsitioncol);
	DDX_Text(pDX, IDC_SPACE_X, m_dbSpaceX);
	DDV_MinMaxDouble(pDX, m_dbSpaceX,1.e-003,1.);
	DDX_Text(pDX, IDC_SPACE_Y, m_dbSpaceY);	
	DDV_MinMaxDouble(pDX, m_dbSpaceY,1.e-003,1.);
	DDX_Text(pDX, IDC_EDIT_DOTTIME, m_dbDotTime);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_bLoop);
	DDX_Radio(pDX, IDC_RADIO_GRAY, m_nDibType);
	DDV_MinMaxInt(pDX, m_nDibType, 0, 1);
	DDX_Text(pDX, IDC_EDIT_QFre, m_dbQFre);
	DDX_Text(pDX, IDC_EDIT_LoopCompate, m_dbLoopCompate);
	DDV_MinMaxDouble(pDX, m_dbLoopCompate, -50., 50.);
	DDX_Text(pDX, IDC_EDIT_ACCTime, m_nAccTime);
	DDV_MinMaxInt(pDX, m_nAccTime, 0, 50);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_dbBrightness);
	DDV_MinMaxDouble(pDX, m_dbBrightness, -1., 1.);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_dbContrast);
	DDV_MinMaxDouble(pDX, m_dbContrast, -1., 1.);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_MARKSPEED, m_dbMarkSpeed);
}

BEGIN_MESSAGE_MAP(CDibDlg, CDialog)
	//{{AFX_MSG_MAP(CDibDlg)
	ON_EN_KILLFOCUS(IDC_SPACE, OnKillfocusSpace)
	ON_BN_CLICKED(IDC_CHECK_LOOP, OnCheckLoop)
	ON_EN_KILLFOCUS(IDC_EDIT_DOTTIME, OnKillfocusEditDottime)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_GRAY, OnBnClickedRadioGray)
	ON_BN_CLICKED(IDC_RADIO_DIFF, OnBnClickedRadioDiff)
	ON_BN_CLICKED(IDC_BUTTON_GRAYCURRENT,OnBnClickedButtonGraycurrent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDibDlg message handlers

void CDibDlg::OnBnClickedRadioGray()
{
	UpdateData();
	GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(m_nDibType==1?TRUE:FALSE);
	GetDlgItem(IDC_BUTTON_GRAYCURRENT)->EnableWindow(m_nDibType==1?FALSE:TRUE);
	if (m_nDibType==1)
	{
		GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(FALSE);
	}	
	else
	{
		GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(m_bLoop);
		GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(TRUE);
	}
}

void CDibDlg::OnBnClickedRadioDiff()
{
	UpdateData();
	GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(m_nDibType==1?TRUE:FALSE);
	GetDlgItem(IDC_BUTTON_GRAYCURRENT)->EnableWindow(m_nDibType==1?FALSE:TRUE);
	if (m_nDibType==1)
	{
		GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(FALSE);
	}	
	else
	{
		GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(m_bLoop);
		GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(TRUE);
	}
}

void CDibDlg::OnBnClickedButtonGraycurrent()
{
#ifdef DEBUG
	HMODULE hLib = LoadLibrary("LPSet_D.dll");
#else
	HMODULE hLib = LoadLibrary("LPSet.dll");
#endif
	if(hLib)
	{
		LPLPSet pLPSet = (LPLPSet)GetProcAddress(hLib,"LPSet");
		LPInitLP pInitLP = (LPInitLP)GetProcAddress(hLib,"InitLP");
		
		double paGreyCurrent[256];
		for (int i=0;i<256;i++)
		{
			paGreyCurrent[i]=0.2*i;
		}
		(pInitLP)( paGreyCurrent, 256 );
		/*BOOL bRtn=(pLPSet) (paGreyCurrent, 
			256, 
			g_TESTCUR,
			g_TESTALL,
			g_STOPTEST,
			g_GETCARDTYPE,
			NULL
			);*/
		double dTest[]={0,25,0,30};
		BOOL bRtn=(pLPSet) (paGreyCurrent, 
			256, 
			g_TESTCUR,
			g_TESTALL,
			g_STOPTEST,
			g_GETCARDTYPE,
			dTest
			);
	}
}

BOOL CDibDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_nDibType==1)
	{
		GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(FALSE);
	}	
	else
	{
		GetDlgItem(IDC_EDIT_DOTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(m_bLoop);
		GetDlgItem(IDC_EDIT_ACCTime)->EnableWindow(TRUE);
	}
	
	
	m_dbQFre=(m_dbMarkSpeed/(1e6)) / m_dbSpaceX * 1000;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDibDlg::OnKillfocusSpace() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_dbQFre=(m_dbMarkSpeed/(1e6)) / m_dbSpaceX * 1000;
	UpdateData(FALSE);
}

void CDibDlg::OnCheckLoop() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nDibType==0)
	{
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(m_bLoop?TRUE:FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_LoopCompate)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
	
}

void CDibDlg::OnOK()
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

void CDibDlg::OnKillfocusEditDottime() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_dbDotTime > MAXDOTTIME) 
	{
		m_dbDotTime = MAXDOTTIME;
	}
	UpdateData(FALSE);
	
}
