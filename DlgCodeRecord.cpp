// DlgCodeRecord.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgCodeRecord.h"

#include "HlParaIni.h"
extern CHlParaIni g_ParaIni;


// CDlgCodeRecord 对话框

IMPLEMENT_DYNAMIC(CDlgCodeRecord, CDialog)

CDlgCodeRecord::CDlgCodeRecord(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCodeRecord::IDD, pParent)
{

}

CDlgCodeRecord::~CDlgCodeRecord()
{
}

void CDlgCodeRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CODE_RECORD, m_ListCodeRecord);
}


BEGIN_MESSAGE_MAP(CDlgCodeRecord, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CDlgCodeRecord::OnBnClickedButtonFind)
END_MESSAGE_MAP()


// CDlgCodeRecord 消息处理程序

void CDlgCodeRecord::OnBnClickedButtonFind()
{
	// TODO: 在此添加控件通知处理程序代码


	if (!m_adoDB.IsDBConnected())
	{
		AfxMessageBox(_T("打开数据库失败，请重设参数!"));
		return ;
	}


	CString strCode;
	GetDlgItemText(IDC_EDIT_HAND_CODE,strCode);

	CString strSQL;

	strCode.Trim();
	if (strCode == "")
	{
		AfxMessageBox(_T("查询数据未空，将显示所有数据"));
		strSQL.Format(_T("select * from tb_MarkData"),strCode);
	}
	else
	{
		strSQL.Format(_T("select * from tb_MarkData where VinCode =\'%s\'"),strCode);
	}

	if (!m_adoDB.OpenSQLQuery((_bstr_t)strSQL,m_recordset))
	{
		AfxMessageBox(_T("数据库表文件不存在，请重新选择数据库"));
		return ;
	}


	m_ListCodeRecord.DeleteAllItems();

	int nIndex = 0;
	CStringArray strVinCode ,strTime;
	while (!m_recordset.IsEOF())
	{
		m_recordset.GetRecordValue(strVinCode,1);
		m_recordset.GetRecordValue(strTime,2);
		m_recordset.MoveNext();

		m_ListCodeRecord.InsertItem(100,_T(""));
		m_ListCodeRecord.SetItemText(nIndex, 0, (LPTSTR)(LPCTSTR)(strVinCode.GetAt(0)));
		m_ListCodeRecord.SetItemText(nIndex, 1, (LPTSTR)(LPCTSTR)(strTime.GetAt(0)));
		nIndex++;

	}
	UpdateData(FALSE);


}

BOOL CDlgCodeRecord::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	DBPARA  para;
	CHlParaIni ini;
	para._Database = _bstr_t(ini.GetDbDatabase());
	para._Server = _bstr_t(ini.GetDbServer());
	para._User  = _bstr_t(ini.GetDbLoginUser());
	para._Pwd   = _bstr_t(ini.GetDbLoginPwd());
	para.nDriverType = ini.GetDbDataType();
	if (!m_adoDB.IsDBConnected()) //未连接，则连接数据库
	{
		if (!m_adoDB.ConnectToDB(para,TRUE))
		{
			AfxMessageBox(_T("打开数据库失败，请重设参数!"));
			return FALSE;
		}
	}



	LONG lStyle;
	lStyle = GetWindowLong(m_ListCodeRecord, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT; 
	SetWindowLong(m_ListCodeRecord, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ListCodeRecord.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_ListCodeRecord.SetExtendedStyle(dwStyle); //设置扩展风格

	m_ListCodeRecord.InsertColumn(0,_T("Matrix"),LVCFMT_CENTER,200);
	m_ListCodeRecord.InsertColumn(1,_T("Time"),LVCFMT_CENTER,300);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgCodeRecord::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message==WM_KEYDOWN && pMsg->wParam!=VK_TAB)
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HAND_CODE);
			if (pMsg->hwnd==pEdit->GetSafeHwnd())
			{
				CString strText = "";
				GetDlgItemText(IDC_EDIT_HAND_CODE,strText); 	
				strText.TrimRight();
				strText.TrimLeft();

				pEdit->SetFocus();
				pEdit->SetSel(0,-1);
				pEdit->SetWindowText(_T(""));

				UpdateData(TRUE);
				//提前处理
				OnBnClickedButtonFind();
				return FALSE;
			}
			return FALSE;
		}
	}



	return CDialog::PreTranslateMessage(pMsg);
}
