// Objecteditfill.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "Objecteditfill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjecteditfill dialog


CObjecteditfill::CObjecteditfill(CWnd* pParent /*=NULL*/)
	: CDialog(CObjecteditfill::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjecteditfill)
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


void CObjecteditfill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjecteditfill)
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


BEGIN_MESSAGE_MAP(CObjecteditfill, CDialog)
	//{{AFX_MSG_MAP(CObjecteditfill)
	ON_BN_CLICKED(IDC_CHECK_CROSS, OnCheckCross)
	ON_BN_CLICKED(IDC_CHECK_CIRCLE, OnCheckCircle)
	ON_BN_CLICKED(IDC_CHECK_BACK, OnCheckBack)
	ON_BN_CLICKED(IDC_CHECK_BETTER, OnCheckBetter)
	ON_BN_CLICKED(IDC_CHECK_PTP, OnCheckPtp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjecteditfill message handlers

BOOL CObjecteditfill::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateUI();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CObjecteditfill::OnCheckCross() 
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

void CObjecteditfill::UpdateDataView()
{
	GetDlgItem(IDC_EDIT_SPACE2)->EnableWindow(m_bCross);
	GetDlgItem(IDC_EDIT_ANGLE2)->EnableWindow(m_bCross);
}

void CObjecteditfill::OnCheckCircle() 
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

void CObjecteditfill::OnCheckBack() 
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

void CObjecteditfill::OnCheckBetter() 
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

void CObjecteditfill::OnCheckPtp() 
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

void CObjecteditfill::DisplyFillProp(FILLDATA &fillOld) 
{
	m_dbAngle1 = fillOld.dbAngle1;
	m_dbAngle2 = fillOld.dbAngle2;
	m_dbSpace1 = fillOld.dbSpace1;
	m_dbSpace2 = fillOld.dbSpace2;
	m_dbDistance = fillOld.dbDistance;
	m_dbPtp = fillOld.dbPtp;
	m_dbCircleLen = fillOld.dbCircleLen;
	m_dbHeadKillLen = fillOld.dbHeadKillLen;
	m_dbTailKillLen = fillOld.dbTailKillLen;

	m_bBack  = (fillOld.nStyle&FILL_BACK)>0?1:0;
	m_bCross = (fillOld.nStyle&FILL_CROSS)>0?1:0;
	m_bEven  = (fillOld.nStyle&FILL_EVEN)>0?1:0;
	m_bFirstFill = (fillOld.nStyle&FILL_MORD)>0?1:0;
	m_bPtp    = (fillOld.nStyle&FILL_PTP)>0?1:0;
	m_bCircle = (fillOld.nStyle&FILL_CIRCLE)>0?1:0;
	m_bBetter = (fillOld.nStyle&FILL_BETTER)>0?1:0;

	UpdateUI();
	UpdateData(FALSE);

	return;
}

void CObjecteditfill::UpdateUI() 
{
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

	GetDlgItem(IDC_EDIT_SPACE2)->EnableWindow(m_bCross);
	GetDlgItem(IDC_EDIT_ANGLE2)->EnableWindow(m_bCross);

	return;
}

void CObjecteditfill::GetNewFillData(FILLDATA &fillNew) 
{
	UpdateData(TRUE);

	fillNew.dbAngle1 = m_dbAngle1;
	fillNew.dbAngle2 = m_dbAngle2;
	fillNew.dbSpace1 = m_dbSpace1;
	fillNew.dbSpace2 = m_dbSpace2;
	fillNew.dbDistance = m_dbDistance;
	fillNew.dbPtp      = m_dbPtp;
	fillNew.dbCircleLen   = m_dbCircleLen;
	fillNew.dbHeadKillLen = m_dbHeadKillLen;
	fillNew.dbTailKillLen = m_dbTailKillLen;
	fillNew.nStyle = 0;
	fillNew.nStyle = m_bBack?fillNew.nStyle|FILL_BACK:fillNew.nStyle;
	fillNew.nStyle = m_bCross?fillNew.nStyle|FILL_CROSS:fillNew.nStyle;
	fillNew.nStyle = m_bEven?fillNew.nStyle|FILL_EVEN:fillNew.nStyle;
	fillNew.nStyle = m_bFirstFill?fillNew.nStyle|FILL_MORD:fillNew.nStyle;
	fillNew.nStyle = m_bPtp?fillNew.nStyle|FILL_PTP:fillNew.nStyle;
	fillNew.nStyle = m_bCircle?fillNew.nStyle|FILL_CIRCLE:fillNew.nStyle;
    fillNew.nStyle = m_bBetter?fillNew.nStyle|FILL_BETTER:fillNew.nStyle;

	return;
}

BOOL CObjecteditfill::PreTranslateMessage(MSG* pMsg) 
{
	/*
	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F10) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_MARK_NORMAL , NULL);
		return TRUE;
	}

	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F7) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FAST_MARK , NULL);
		return TRUE;
	}
	*/


	return CDialog::PreTranslateMessage(pMsg);
}
