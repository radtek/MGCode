// FileLoadData.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "FileLoadData.h"
#include "afxdialogex.h"


// CFileLoadData 对话框

IMPLEMENT_DYNAMIC(CFileLoadData, CDialogEx)

CFileLoadData::CFileLoadData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileLoadData::IDD, pParent)
	, m_bUseFile(FALSE)
	, m_strPath(_T(""))
	, m_strNGMapPath(_T(""))
{

}

CFileLoadData::~CFileLoadData()
{
}

void CFileLoadData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_USEFILE, m_bUseFile);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_strPath);
	DDX_Text(pDX, IDC_EDIT_NGMAP, m_strNGMapPath);
}


BEGIN_MESSAGE_MAP(CFileLoadData, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOAD, &CFileLoadData::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_CHECK_USEFILE, &CFileLoadData::OnBnClickedCheckUsefile)
	ON_BN_CLICKED(IDOK, &CFileLoadData::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_NGMAP, &CFileLoadData::OnBnClickedButtonNgmap)
END_MESSAGE_MAP()


// CFileLoadData 消息处理程序


void CFileLoadData::OnBnClickedBtnLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter = "txt File(*.txt)|*.txt|All Files(*.*)|*.*||";

	CFileDialog dlg(TRUE,NULL,m_strPath,OFN_HIDEREADONLY,strFilter);
	if(dlg.DoModal()==IDOK)
	{
		m_strPath = dlg.GetPathName();

		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		if (NULL == pFWnd)
		{
			return ;
		}
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{	
			return ;
		}
		CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
		if (NULL == pDoc)
		{
			return ;
		}
		if(!pDoc->LoadFileDataChange(m_strPath))
		{
			m_strPath = "";
		}
		UpdateData(FALSE);

	//	ChangeText();
	}
}


void CFileLoadData::ChangeText(void)
{
	CStdioFile ftxt;
	BOOL bcanOpen = ftxt.Open(m_strPath,CFile::modeRead|CFile::modeNoTruncate);
	if (bcanOpen)
	{
		UpdateData(FALSE);
	}
	else
	{
		AfxMessageBox("无法打开");
		return;
	}
	
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (NULL == pFWnd)
	{
		return ;
	}
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{	
		return ;
	}
	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return ;
	}

	CString strData;
	int nCount = 0;
	for (int i = 0; i < 100; i++)
	{
		ftxt.ReadString(strData);
		strData.Trim();
		if (strData == "")
		{
			break;
		}
		int nRet = strData.Find(_T("|"));
		if (-1 != nRet)
		{
			CString strText = strData.Left(nRet);
			CString strMatri = strData.Right(strData.GetLength() - nRet - 1);
			CString strTextID,strMatriID;
			strTextID.Format(_T("%d"),nCount);
			strMatriID.Format(_T("%d"),nCount + 1000);
			pDoc->m_chain.ChangeText(strText,strTextID);
			pDoc->m_chain.ChangeText(strMatri,strMatriID);
			nCount++;
		}
	}
	pDoc->m_chain.PreprocessMark();
	pDoc->UpdateAllViews(NULL);
}


BOOL CFileLoadData::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CHLApp *pApp = (CHLApp *)AfxGetApp();
	 m_strNGMapPath = pApp->GetProfileString("NGMAPPATH", "NGMapPath","C:\\Users\\lenovo\\Desktop\\ChangeDoc\\Release\\ChangeDoc.exe");
	
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (NULL == pFWnd)
	{
		return FALSE;
	}
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{	
		return FALSE;
	}
	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return FALSE;
	}
	m_strPath = pDoc->m_strFileDataPath;
	m_bUseFile = pDoc->m_bUseFile;
	GetDlgItem(IDC_BTN_LOAD)->EnableWindow(m_bUseFile);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFileLoadData::OnBnClickedCheckUsefile()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_LOAD)->EnableWindow(m_bUseFile);
}


void CFileLoadData::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (NULL == pFWnd)
	{
		return ;
	}
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{	
		return ;
	}
	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return ;
	}
	pDoc->m_bUseFile = m_bUseFile;
	pDoc->OnSaveDocument(pDoc->GetPathName());
	CDialogEx::OnOK();
}


void CFileLoadData::OnBnClickedButtonNgmap()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter = "exe File(*.exe)|*.exe|All Files(*.*)|*.*||";

	CFileDialog dlg(TRUE,NULL,m_strNGMapPath,OFN_HIDEREADONLY,strFilter);
	if(dlg.DoModal()==IDOK)
	{
		m_strNGMapPath = dlg.GetPathName();
		UpdateData(FALSE);
		CHLApp *pApp = (CHLApp *)AfxGetApp();
		pApp->WriteProfileString("NGMAPPATH", "NGMapPath", m_strNGMapPath);
	}
}
