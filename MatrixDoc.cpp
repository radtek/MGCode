// MatrixDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "MatrixDoc.h"
#include "afxdialogex.h"


// CMatrixDoc 对话框

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


// CMatrixDoc 消息处理程序


BOOL CMatrixDoc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LONG lStyle;
	lStyle = GetWindowLong(m_ListMatrixDoc, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT; 
	SetWindowLong(m_ListMatrixDoc, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ListMatrixDoc.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	dwStyle |= LVS_EX_SUBITEMIMAGES;
	m_ListMatrixDoc.SetExtendedStyle(dwStyle); //设置扩展风格

	m_ListMatrixDoc.InsertColumn(0,"Matrix",LVCFMT_CENTER,100);
	m_ListMatrixDoc.InsertColumn(1,"Path",LVCFMT_CENTER,470);

	ReadFileData();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CMatrixDoc::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码

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

	CString strLog = "增加二维码：" + m_strMatrix;
	strLog = strLog + "   对应路径为：" + m_strPath + "\n";

	WriteRunLog(strLog);
}


void CMatrixDoc::OnBnClickedButtonDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION posSel = m_ListMatrixDoc.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_ListMatrixDoc.GetNextSelectedItem(posSel);

		m_strMatrix = m_ListMatrixDoc.GetItemText(nCurSel, 0);
		m_strPath = m_ListMatrixDoc.GetItemText(nCurSel, 1);

		CString strLog = "删除二维码：" + m_strMatrix;
		strLog = strLog + "   对应路径为：" + m_strPath + "\n";

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
	// TODO: 在此添加控件通知处理程序代码
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

		CString strLog = "修改前二维码：" + strMatrix;
		strLog = strLog + "   对应路径为：" + strPath + "\n";
		WriteRunLog(strLog);
	

		m_ListMatrixDoc.SetItemText(nCurSel, 0, m_strMatrix);
		m_ListMatrixDoc.SetItemText(nCurSel, 1, m_strPath);

		strLog = "修改后二维码：" + m_strMatrix;
		strLog = strLog + "   对应路径为：" + m_strPath + "\n";
		WriteRunLog(strLog);


	}
}


void CMatrixDoc::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	WriteFileData();

	CString strLog = "保存二维码和对应的路径\n";
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam!=VK_TAB)
	{
		CWnd *pWnd1 = GetDlgItem(IDC_EDIT_MATRIX);
		CWnd *pWnd2 = GetDlgItem(IDC_EDIT_DOCPATH);
		CEdit*pEdit = NULL;

		if (pMsg->wParam==VK_RETURN)  ////回车键按键消息
		{
			HWND hWnd = GetFocus()->GetSafeHwnd();
			if (pMsg->hwnd==hWnd ) //只捕获指定窗口的消息
			{
				UpdateData(TRUE);
				////回车后自动以下一个输入窗口作为焦点
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
