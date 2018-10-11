// DataSourceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DataSourceDlg.h"
#include "resource.h"

// CDataSourceDlg 对话框

IMPLEMENT_DYNAMIC(CDataSourceDlg, CDialog)

CDataSourceDlg::CDataSourceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataSourceDlg::IDD, pParent)
	, m_nType(1)
	, m_strDsn(_T(""))
	, m_strMdb(_T(""))
	, m_strUserName(_T(""))
	, m_strPwd(_T(""))
	, m_strServer(_T(""))
{


}

CDataSourceDlg::~CDataSourceDlg()
{
}

void CDataSourceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_DATA, m_nType);
	DDX_Text(pDX, IDC_EDIT_DSN, m_strDsn);
	DDX_Text(pDX, IDC_EDIT_MDB, m_strMdb);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PSW, m_strPwd);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServer);

}


BEGIN_MESSAGE_MAP(CDataSourceDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA, &CDataSourceDlg::OnCbnSelchangeComboData)
	ON_BN_CLICKED(IDC_BUTTON_MDB, &CDataSourceDlg::OnBnClickedButtonMdb)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDataSourceDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CDataSourceDlg::OnBnClickedOk)
	ON_CBN_EDITCHANGE(IDC_COMBO_DATA, &CDataSourceDlg::OnCbnEditchangeComboData)
END_MESSAGE_MAP()


// CDataSourceDlg 消息处理程序

void CDataSourceDlg::OnCbnSelchangeComboData()
{
	UpdateData();
	UpdateFrame();
}

void CDataSourceDlg::OnBnClickedButtonMdb()
{
	CFileDialog fileDlg(TRUE, _T("MDB"), _T("*.mdb"), NULL, _T("Access Files(*.mdb)|*.mdb | Access Files(*.accdb)|*.accdb||"), NULL);
	//unicode版本下_T("Access Files(*.mdb)")后面需加上||或\0\0，否则选择文件类型时第一行正常,第二行是乱码
	if (fileDlg.DoModal ()==IDOK)
	{
		m_strMdb = fileDlg.GetPathName();
		UpdateData(FALSE);
	}	
}

void CDataSourceDlg::OnBnClickedButtonTest()
{
	UpdateData(TRUE);

	m_Para.nDriverType = m_nType;
	if (m_nType==driverODBC)
		m_Para._Database=_bstr_t(m_strDsn);
	else
		m_Para._Database=_bstr_t(m_strMdb);
	m_Para._Pwd  = _bstr_t(m_strPwd);
	m_Para._User = _bstr_t(m_strUserName);
	m_Para._Server = _bstr_t(m_strServer);

	BeginWaitCursor();
	CAdoDB adoDb;
	if (adoDb.ConnectToDB(m_Para))
		AfxMessageBox(_T("Connect Test Ok!"),MB_OK|MB_ICONQUESTION);
	else
		AfxMessageBox(_T("Connect Test fail!"),MB_OK|MB_ICONQUESTION);

	EndWaitCursor();
}

void CDataSourceDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_Para.nDriverType = m_nType;
	if (m_nType==driverODBC)
		m_Para._Database=_bstr_t(m_strDsn);
	else
		m_Para._Database=_bstr_t(m_strMdb);
	m_Para._Pwd  = _bstr_t(m_strPwd);
	m_Para._User = _bstr_t(m_strUserName);
	m_Para._Server = _bstr_t(m_strServer);	

	CDialog::OnOK();
	//OnOK();
}

BOOL CDataSourceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nType = m_Para.nDriverType;
	if (m_nType==driverODBC)
		m_strDsn = (LPCTSTR)m_Para._Database;
	else
		m_strMdb = (LPCTSTR)m_Para._Database;
	m_strPwd = (LPCTSTR)m_Para._Pwd;
	m_strUserName = (LPCTSTR)m_Para._User;
	m_strServer = (LPCTSTR)m_Para._Server;

	UpdateFrame();

	return TRUE; 
}

void CDataSourceDlg::UpdateFrame()
{
	GetDlgItem(IDC_EDIT_DSN)->EnableWindow(m_nType==driverODBC);
	GetDlgItem(IDC_EDIT_SERVER)->EnableWindow(m_nType==driverSQLServer);
	GetDlgItem(IDC_EDIT_MDB)->EnableWindow(m_nType!=driverODBC);
	GetDlgItem(IDC_BUTTON_MDB)->EnableWindow(m_nType>=driverACCESS97);
	GetDlgItem(IDC_BUTTON_TEST)->EnableWindow(m_nType>=0);
	UpdateData(FALSE);
}
void CDataSourceDlg::OnCbnEditchangeComboData()
{
	UpdateData();
	UpdateFrame();
}
