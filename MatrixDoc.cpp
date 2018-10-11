// MatrixDoc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "MatrixDoc.h"
#include "afxdialogex.h"


// CMatrixDoc �Ի���

IMPLEMENT_DYNAMIC(CMatrixDoc, CDialogEx)

CMatrixDoc::CMatrixDoc(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMatrixDoc::IDD, pParent)
	, m_strPath(_T(""))
	, m_strMatrix(_T(""))
{

}

CMatrixDoc::~CMatrixDoc()
{
}

void CMatrixDoc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MATRIXDOC, m_ListMatrixDoc);
	DDX_Text(pDX, IDC_EDIT_DOCPATH, m_strPath);
	DDX_Text(pDX, IDC_EDIT_MATRIX, m_strMatrix);
}


BEGIN_MESSAGE_MAP(CMatrixDoc, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMatrixDoc::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMatrixDoc::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMatrixDoc::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CMatrixDoc::OnBnClickedButtonChange)
	ON_BN_CLICKED(IDOK, &CMatrixDoc::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MATRIXDOC, &CMatrixDoc::OnNMClickListMatrixdoc)
END_MESSAGE_MAP()


// CMatrixDoc ��Ϣ�������


BOOL CMatrixDoc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LONG lStyle;
	lStyle = GetWindowLong(m_ListMatrixDoc, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT; 
	SetWindowLong(m_ListMatrixDoc, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ListMatrixDoc.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_ListMatrixDoc.SetExtendedStyle(dwStyle); //������չ���

	m_ListMatrixDoc.InsertColumn(0,"Matrix",LVCFMT_CENTER,100);
	m_ListMatrixDoc.InsertColumn(1,"Path",LVCFMT_CENTER,470);

	ReadFileData();


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMatrixDoc::OnBnClickedButtonLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strFilter = "HS File(*.hs)|*.hs|All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE,NULL,m_strPath,OFN_HIDEREADONLY,strFilter);
	if(dlg.DoModal()==IDOK)
	{
		m_strPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CMatrixDoc::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);

	if (m_strPath == "" || m_strMatrix == "")
	{
		AfxMessageBox("Path or Matrix is Null");
		return;
	}
	if (!FileFind())
	{
		AfxMessageBox("Path isnot exist!");
		return ;
	}

	int nIndex = m_ListMatrixDoc.InsertItem(100, "");
	m_ListMatrixDoc.SetItemText(nIndex, 0, m_strMatrix);
	m_ListMatrixDoc.SetItemText(nIndex, 1, m_strPath);

	CString strLog = "���Ӷ�ά�룺" + m_strMatrix;
	strLog = strLog + "   ��Ӧ·��Ϊ��" + m_strPath + "\n";

	WriteRunLog(strLog);
}


void CMatrixDoc::OnBnClickedButtonDelete()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION posSel = m_ListMatrixDoc.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_ListMatrixDoc.GetNextSelectedItem(posSel);

		m_strMatrix = m_ListMatrixDoc.GetItemText(nCurSel, 0);
		m_strPath = m_ListMatrixDoc.GetItemText(nCurSel, 1);

		CString strLog = "ɾ����ά�룺" + m_strMatrix;
		strLog = strLog + "   ��Ӧ·��Ϊ��" + m_strPath + "\n";

		WriteRunLog(strLog);


		m_ListMatrixDoc.DeleteItem(nCurSel);
	}
}

BOOL CMatrixDoc::FileFind()
{
	UpdateData(TRUE);
	CFileFind finder;
	return finder.FindFile(m_strPath);
}


void CMatrixDoc::OnBnClickedButtonChange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData())
		return;

	POSITION posSel = m_ListMatrixDoc.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_ListMatrixDoc.GetNextSelectedItem(posSel);

		if (m_strPath == "" || m_strMatrix == "")
		{
			AfxMessageBox("Path or Matrix is Null");
			return;
		}
		if (!FileFind())
		{
			AfxMessageBox("Path isnot exist!");
			return ;
		}

		CString strMatrix = m_ListMatrixDoc.GetItemText(nCurSel, 0);
		CString strPath = m_ListMatrixDoc.GetItemText(nCurSel, 1);

		CString strLog = "�޸�ǰ��ά�룺" + strMatrix;
		strLog = strLog + "   ��Ӧ·��Ϊ��" + strPath + "\n";
		WriteRunLog(strLog);
	

		m_ListMatrixDoc.SetItemText(nCurSel, 0, m_strMatrix);
		m_ListMatrixDoc.SetItemText(nCurSel, 1, m_strPath);

		strLog = "�޸ĺ��ά�룺" + m_strMatrix;
		strLog = strLog + "   ��Ӧ·��Ϊ��" + m_strPath + "\n";
		WriteRunLog(strLog);


	}
}


void CMatrixDoc::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	WriteFileData();

	CString strLog = "�����ά��Ͷ�Ӧ��·��\n";
	WriteRunLog(strLog);

	CDialogEx::OnOK();
}



BOOL CMatrixDoc::ReadFileData(void)
{

	CString strPath = "d:\\MatrixDoc.dat";
	CStdioFile fReadtxt;
	BOOL bReadOpen = fReadtxt.Open(strPath,CFile::modeRead|CFile::modeNoTruncate);
	if (bReadOpen)
	{
		int nLength = static_cast<int>(fReadtxt.GetLength());
		CString strData;
		for (int i = 0; i < nLength; i++)
		{
			fReadtxt.ReadString(strData);
			strData.Trim();
			if (strData == "")
			{
				break;
			}
			int nRet = strData.Find("*");
			strData.Mid(nRet + 1);
			m_strMatrix = strData.Left(nRet);
			m_strPath = strData.Mid(nRet + 1);
			int nIndex = m_ListMatrixDoc.InsertItem(100, "");
			m_ListMatrixDoc.SetItemText(nIndex, 0, m_strMatrix);
			m_ListMatrixDoc.SetItemText(nIndex, 1, m_strPath);

		}
	}
	fReadtxt.Close();
	return TRUE;
}


BOOL CMatrixDoc::WriteFileData(void)
{
	UpdateData(TRUE);


	
	CString strPath = "d:\\MatrixDoc.dat";
	CStdioFile fWritetxt;
	CMatrixMarkDoc MatrixMarkDoc;
	BOOL bWriteOpen = fWritetxt.Open(strPath,CFile::modeCreate | CFile::modeWrite);
	if ( bWriteOpen)
	{
		g_pDlgMarkNormalPane->m_arrMatrixMarkDoc.RemoveAll();
		int nCount = m_ListMatrixDoc.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			MatrixMarkDoc.strMatrix = m_ListMatrixDoc.GetItemText(i, 0);
			MatrixMarkDoc.strPath = m_ListMatrixDoc.GetItemText(i, 1);
			g_pDlgMarkNormalPane->m_arrMatrixMarkDoc.Add(MatrixMarkDoc);

			CString strItem = m_ListMatrixDoc.GetItemText(i, 0) + "*";
			strItem += m_ListMatrixDoc.GetItemText(i, 1);
			strItem += "\n";
			fWritetxt.WriteString(strItem);
		}
		fWritetxt.Close();
	}
	return TRUE;
}

void CMatrixDoc::OnNMClickListMatrixdoc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;


	UpdateData(TRUE);
	POSITION posSel = m_ListMatrixDoc.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_ListMatrixDoc.GetNextSelectedItem(posSel);

		m_strMatrix = m_ListMatrixDoc.GetItemText(nCurSel, 0);
		m_strPath = m_ListMatrixDoc.GetItemText(nCurSel, 1);
		UpdateData(FALSE);
	}


}


BOOL CMatrixDoc::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam!=VK_TAB)
	{
		CWnd *pWnd1 = GetDlgItem(IDC_EDIT_MATRIX);
		CWnd *pWnd2 = GetDlgItem(IDC_EDIT_DOCPATH);
		CEdit*pEdit = NULL;

		if (pMsg->wParam==VK_RETURN)  ////�س���������Ϣ
		{
			HWND hWnd = GetFocus()->GetSafeHwnd();
			if (pMsg->hwnd==hWnd ) //ֻ����ָ�����ڵ���Ϣ
			{
				UpdateData(TRUE);
				////�س����Զ�����һ�����봰����Ϊ����
				if (GetFocus()==pWnd1)
					pEdit = (CEdit*)pWnd2;
				else
					pEdit = (CEdit*)pWnd1;
				pEdit->SetFocus();
				pEdit->SetSel(0,-1,TRUE);
				UpdateData(FALSE);
			}
		}
		return 0;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
