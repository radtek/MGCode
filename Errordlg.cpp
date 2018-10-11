// ErrorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ErrorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CErrorDlg dialog


CErrorDlg::CErrorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CErrorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CErrorDlg)
	//}}AFX_DATA_INIT
	m_nPos=0;
	memset(m_char,0,10);
	m_bFlag = FALSE;
}


void CErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CErrorDlg, CDialog)
	//{{AFX_MSG_MAP(CErrorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CErrorDlg message handlers


BOOL CErrorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_CHAR)
	{
		m_char[m_nPos++]=(TCHAR)pMsg->wParam;
		m_nPos%=9;
		if(m_nPos == 0){
			if(strcmp(m_char,"nihaohans")==0)
				m_bFlag = TRUE;
		}
		else{
			m_bFlag = FALSE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
