// UserLogin.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "UserLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserLogin dialog
extern CArray<USERINFO, USERINFO> g_RightInfoList;

CUserLogin::CUserLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CUserLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserLogin)
	m_strUserName = _T("");
	m_strPassword = _T("");
	m_bIsOK       = FALSE;
	m_nLoginCount = 0;
	m_dwDispMsg   = 0;
	//}}AFX_DATA_INIT
}


void CUserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserLogin)
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserLogin, CDialog)
	//{{AFX_MSG_MAP(CUserLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUserLogin message handlers

BOOL CUserLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strmsg("");
	switch(m_dwDispMsg) 
	{
	case 0:
		{
			break;
		}
	case 1:
		{
			strmsg.LoadString(IDS_NODEFAULTUSER);
			break;
		}
	case 2:
		{
			strmsg.LoadString(IDS_USERINFOISNOOK);
			break;
		}
	default:
		{
			break;
		}
	}

	GetDlgItem(IDC_STATIC_DISPMSG)->SetWindowText(strmsg);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserLogin::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	if (0 == m_strUserName.CompareNoCase("softcenter")) 
	{
		if (0 != m_strPassword.CompareNoCase(g_CurUserInfo.szPassWord)) 
		{
			AfxMessageBox(IDS_USERORPASSWORDNOOK);
			m_bIsOK = FALSE;
			return;
		}

		strcpy(g_CurUserInfo.szUserName, "softcenter");
		g_CurUserInfo.dwRight1 |= 0xFFFFFFFF;
		g_CurUserInfo.dwRight2 |= 0xFFFFFFFF;
		USERINFO stUserInfo;
		if (!g_sys.GetUserInfoByName(m_strUserName, stUserInfo)) 
		{
			g_sys.AddUser(g_CurUserInfo);
		}
		else
		{
			g_sys.EditUserInfo(g_CurUserInfo);
		}
		m_bIsOK = TRUE;
		CDialog::OnOK();
		return;
	}
	
	BOOL bRet = g_sys.IsAvailUser(m_strUserName, m_strPassword);
	if (!bRet)
	{
		AfxMessageBox(IDS_USERORPASSWORDNOOK);
	}
	else
	{
		m_bIsOK = TRUE;
		CDialog::OnOK();
		return;
	}
	
	m_nLoginCount++;
	if (m_nLoginCount >= 3)
	{
		CDialog::OnOK();
	}

	return;
}
