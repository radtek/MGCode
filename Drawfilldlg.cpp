// DrawFillDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DrawFillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawFillDlg dialog


CDrawFillDlg::CDrawFillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawFillDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawFillDlg)
	m_bBack = FALSE;
	m_bCross = FALSE;
	m_bEven = FALSE;
	m_bFirstFill = FALSE;
	m_dbAngle1 = 0.0;
	m_dbAngle2 = 90.0;
	m_dbDistance = 0.0;
	m_dbSpace1 = 0.0;
	m_dbSpace2 = 0.0;
	m_bPtp = FALSE;
	m_dbPtp = 0.0;
	m_dbCircleLen = 0.0;
	m_bCircle = FALSE;
	m_bBetter = FALSE;
	m_dbHeadKillLen = 0.0;
	m_dbTailKillLen = 0.0;
	//}}AFX_DATA_INIT
}


void CDrawFillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawFillDlg)
	DDX_Check(pDX, IDC_CHECK_BACK, m_bBack);
	DDX_Check(pDX, IDC_CHECK_CROSS, m_bCross);
	DDX_Check(pDX, IDC_CHECK_EVEN, m_bEven);
	DDX_Check(pDX, IDC_CHECK_FIRSTFILL, m_bFirstFill);
	DDX_Text(pDX, IDC_EDIT_ANGLE1, m_dbAngle1);
	DDX_Text(pDX, IDC_EDIT_ANGLE2, m_dbAngle2);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, m_dbDistance);
	DDX_Text(pDX, IDC_EDIT_SPACE1, m_dbSpace1);
	DDX_Text(pDX, IDC_EDIT_SPACE2, m_dbSpace2);
	DDX_Check(pDX, IDC_CHECK_PTP, m_bPtp);
	DDX_Text(pDX, IDC_EDIT_PTP, m_dbPtp);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_LEN, m_dbCircleLen);
	DDX_Check(pDX, IDC_CHECK_CIRCLE, m_bCircle);
	DDX_Check(pDX, IDC_CHECK_BETTER, m_bBetter);
	DDX_Text(pDX, IDC_EDIT_HEAD_KILLLEN, m_dbHeadKillLen);
	DDX_Text(pDX, IDC_EDIT_TAIL_KILLLEN, m_dbTailKillLen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawFillDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawFillDlg)
	ON_BN_CLICKED(IDC_CHECK_CROSS, OnCheckCross)
	ON_BN_CLICKED(IDC_CHECK_CIRCLE, OnCheckCircle)
	ON_BN_CLICKED(IDC_CHECK_BACK, OnCheckBack)
	ON_BN_CLICKED(IDC_CHECK_BETTER, OnCheckBetter)
	ON_BN_CLICKED(IDC_CHECK_PTP, OnCheckPtp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawFillDlg message handlers

void CDrawFillDlg::OnOK() 
{
	UpdateData();
	if (m_dbSpace1<0.0001)
	{
		m_dbSpace1=0;
	}
	if (m_dbSpace2<0.0001)
	{
		m_dbSpace2=0;
	}
	CDialog::OnOK();
}

BOOL CDrawFillDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_bCircle) 
	{
		((CButton*)GetDlgItem(IDC_CHECK_BETTER))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_BETTER))->SetCheck(FALSE);
	}

	if (m_bBetter) 
	{
		((CButton*)GetDlgItem(IDC_CHECK_BETTER))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_BETTER))->SetCheck(FALSE);
	}

	if (m_bCircle || m_bBetter) 
	{
		((CButton*)GetDlgItem(IDC_EDIT_CIRCLE_LEN))->EnableWindow(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_EDIT_CIRCLE_LEN))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT_CIRCLE_LEN))->SetWindowText("0.0");
	}

	if (m_bBack) 
	{
		((CButton*)GetDlgItem(IDC_CHECK_BACK))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_CHECK_BACK))->SetCheck(FALSE);
	}

	if (m_bPtp)
	{
		((CButton*)GetDlgItem(IDC_EDIT_PTP))->EnableWindow(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_EDIT_PTP))->EnableWindow(FALSE);
	}

	UpdateDataView();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDrawFillDlg::OnCheckCross() 
{
	UpdateData();
	if (m_dbSpace1<0.0001)
	{
		m_dbSpace1=0;
	}
	if (m_dbSpace2<0.0001)
	{
		m_dbSpace2=0;
	}
	UpdateData(FALSE);
	UpdateDataView();
}

void CDrawFillDlg::UpdateDataView()
{
	GetDlgItem(IDC_EDIT_SPACE2)->EnableWindow(m_bCross);
	GetDlgItem(IDC_EDIT_ANGLE2)->EnableWindow(m_bCross);
}

void CDrawFillDlg::OnCheckCircle() 
{
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_CIRCLE))->GetCheck();
	if (bRet) 
	{
		GetDlgItem(IDC_EDIT_CIRCLE_LEN)->EnableWindow(TRUE);
	}
	else
	{
		BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_BETTER))->GetCheck();
		if (!bRet) 
		{
			GetDlgItem(IDC_EDIT_CIRCLE_LEN)->EnableWindow(FALSE);
			(GetDlgItem(IDC_EDIT_CIRCLE_LEN))->SetWindowText("0.0");
		}
		else
		{
			GetDlgItem(IDC_EDIT_CIRCLE_LEN)->EnableWindow(TRUE);
		}
	}
}

void CDrawFillDlg::OnCheckBack() 
{
//	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_BACK))->GetCheck();
//	if (bRet) 
//	{
//		((CButton*)GetDlgItem(IDC_CHECK_BETTER))->EnableWindow(TRUE);
//        ((CButton*)GetDlgItem(IDC_CHECK_CIRCLE))->EnableWindow(TRUE);
//        ((CButton*)GetDlgItem(IDC_EDIT_CIRCLE_LEN))->EnableWindow(TRUE);
//	}
//	else
//	{
//		((CButton*)GetDlgItem(IDC_CHECK_BETTER))->EnableWindow(FALSE);
//        ((CButton*)GetDlgItem(IDC_CHECK_CIRCLE))->EnableWindow(FALSE);
//        ((CButton*)GetDlgItem(IDC_EDIT_CIRCLE_LEN))->EnableWindow(FALSE);
//	}

	return;
}

void CDrawFillDlg::OnCheckBetter() 
{
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_BETTER))->GetCheck();
	if (bRet) 
	{
        ((CButton*)GetDlgItem(IDC_EDIT_CIRCLE_LEN))->EnableWindow(TRUE);
	}
	else
	{
		BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_CIRCLE))->GetCheck();
		if (!bRet) 
		{
			((CButton*)GetDlgItem(IDC_EDIT_CIRCLE_LEN))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_EDIT_CIRCLE_LEN))->SetWindowText("0.0");
		}
		else
		{
			((CButton*)GetDlgItem(IDC_EDIT_CIRCLE_LEN))->EnableWindow(TRUE);
		}
	}

	return;
}

void CDrawFillDlg::OnCheckPtp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_bPtp)
	{
		((CButton*)GetDlgItem(IDC_EDIT_PTP))->EnableWindow(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_EDIT_PTP))->EnableWindow(FALSE);
	}
}
