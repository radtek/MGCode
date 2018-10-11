// DlgCodeRecord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgCodeRecord.h"

#include "HlParaIni.h"
extern CHlParaIni g_ParaIni;


// CDlgCodeRecord �Ի���

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


// CDlgCodeRecord ��Ϣ�������

void CDlgCodeRecord::OnBnClickedButtonFind()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	if (!m_adoDB.IsDBConnected())
	{
		AfxMessageBox(_T("�����ݿ�ʧ�ܣ����������!"));
		return ;
	}


	CString strCode;
	GetDlgItemText(IDC_EDIT_HAND_CODE,strCode);

	CString strSQL;

	strCode.Trim();
	if (strCode == "")
	{
		AfxMessageBox(_T("��ѯ����δ�գ�����ʾ��������"));
		strSQL.Format(_T("select * from tb_MarkData"),strCode);
	}
	else
	{
		strSQL.Format(_T("select * from tb_MarkData where VinCode =\'%s\'"),strCode);
	}

	if (!m_adoDB.OpenSQLQuery((_bstr_t)strSQL,m_recordset))
	{
		AfxMessageBox(_T("���ݿ���ļ������ڣ�������ѡ�����ݿ�"));
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	DBPARA  para;
	CHlParaIni ini;
	para._Database = _bstr_t(ini.GetDbDatabase());
	para._Server = _bstr_t(ini.GetDbServer());
	para._User  = _bstr_t(ini.GetDbLoginUser());
	para._Pwd   = _bstr_t(ini.GetDbLoginPwd());
	para.nDriverType = ini.GetDbDataType();
	if (!m_adoDB.IsDBConnected()) //δ���ӣ����������ݿ�
	{
		if (!m_adoDB.ConnectToDB(para,TRUE))
		{
			AfxMessageBox(_T("�����ݿ�ʧ�ܣ����������!"));
			return FALSE;
		}
	}



	LONG lStyle;
	lStyle = GetWindowLong(m_ListCodeRecord, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT; 
	SetWindowLong(m_ListCodeRecord, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ListCodeRecord.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_ListCodeRecord.SetExtendedStyle(dwStyle); //������չ���

	m_ListCodeRecord.InsertColumn(0,_T("Matrix"),LVCFMT_CENTER,200);
	m_ListCodeRecord.InsertColumn(1,_T("Time"),LVCFMT_CENTER,300);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CDlgCodeRecord::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

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
				//��ǰ����
				OnBnClickedButtonFind();
				return FALSE;
			}
			return FALSE;
		}
	}



	return CDialog::PreTranslateMessage(pMsg);
}
