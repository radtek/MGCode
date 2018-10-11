// MarkdataStatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "MarkdataStatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkdataStatDlg dialog


CMarkdataStatDlg::CMarkdataStatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkdataStatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkdataStatDlg)
	m_strDate = _T("");
	m_strMarknum = _T("");
	m_strMarktime = _T("");
	//}}AFX_DATA_INIT

	m_chPassWord[8] = 0;
	m_nWordLength = 0;
	UpdateView();
}


void CMarkdataStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkdataStatDlg)
	DDX_Text(pDX, IDC_STATIC_DATE, m_strDate);
	DDX_Text(pDX, IDC_STATIC_MARKNUM, m_strMarknum);
	DDX_Text(pDX, IDC_STATIC_MARKTIME, m_strMarktime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarkdataStatDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkdataStatDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkdataStatDlg message handlers

void CMarkdataStatDlg::UpdateView()
{
	CHLApp *pApp = (CHLApp *)AfxGetApp();
	
	//起始日期
	if((m_strDate=pApp->GetProfileString("STARTDATE", "MARKSTAT", ""))=="")
	{
		SYSTEMTIME systemtime;
		::GetSystemTime(&systemtime);
		CString strFormat;
		strFormat.LoadString(IDS_MARKSTAT_DATEFORMAT);
		m_strDate.Format(strFormat, systemtime.wYear, systemtime.wMonth, systemtime.wDay);
		pApp->WriteProfileString("STARTDATE", "MARKSTAT", m_strDate);
	}
	//打标次数
	DWORD dwMarknum = pApp->GetProfileInt("MARKNUMBER", "MARKSTAT", 0);
	char buf[32];
	_ltoa(dwMarknum, buf, 10);
	m_strMarknum = buf;
	//打标时间
	DWORD dwMarktime = pApp->GetProfileInt("MARKTIME", "MARKSTAT", 0);
	CTimeSpan timespan = dwMarktime;
	m_strMarktime = timespan.Format(IDS_MARKSTAT_TIMEFORMAT);

}


BOOL CMarkdataStatDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_CHAR)
	{
		m_chPassWord[m_nWordLength++]=(TCHAR)pMsg->wParam;
		m_nWordLength%=8;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CMarkdataStatDlg::OnOK() 
{
	CString str = m_chPassWord;
	if(str == "markstat")
	{
		CHLApp *pApp = (CHLApp *)AfxGetApp();

		//写统计开始日期
		SYSTEMTIME systemtime;
		::GetSystemTime(&systemtime);
		CString strFormat;
		strFormat.LoadString(IDS_MARKSTAT_DATEFORMAT);
		m_strDate.Format(strFormat, systemtime.wYear, systemtime.wMonth, systemtime.wDay);
		pApp->WriteProfileString("STARTDATE", "MARKSTAT", m_strDate);

		pApp->WriteProfileInt("MARKNUMBER", "MARKSTAT", 0);
		pApp->WriteProfileInt("MARKTIME", "MARKSTAT", 0);
	}

	CDialog::OnOK();
}
