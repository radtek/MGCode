// DlgArrayRename.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgArrayRename.h"
#include "afxdialogex.h"


// CDlgArrayRename �Ի���

IMPLEMENT_DYNAMIC(CDlgArrayRename, CDialogEx)

CDlgArrayRename::CDlgArrayRename(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgArrayRename::IDD, pParent)
	, m_strTextNum(_T("1"))
	, m_strMatriNum(_T("1001"))
{

}

CDlgArrayRename::~CDlgArrayRename()
{
}

void CDlgArrayRename::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXTNUM, m_strTextNum);
	DDX_Text(pDX, IDC_EDIT_MATRINUM, m_strMatriNum);
}


BEGIN_MESSAGE_MAP(CDlgArrayRename, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgArrayRename::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgArrayRename ��Ϣ�������


void CDlgArrayRename::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������





	CDialogEx::OnOK();
}
