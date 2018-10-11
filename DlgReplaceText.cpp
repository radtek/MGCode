// DlgReplaceText.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgReplaceText.h"
#include "afxdialogex.h"


// CDlgReplaceText 对话框

IMPLEMENT_DYNAMIC(CDlgReplaceText, CDialogEx)

CDlgReplaceText::CDlgReplaceText(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgReplaceText::IDD, pParent)
{

	m_strText = _T("");
}

CDlgReplaceText::~CDlgReplaceText()
{
}

void CDlgReplaceText::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
}


BEGIN_MESSAGE_MAP(CDlgReplaceText, CDialogEx)
END_MESSAGE_MAP()


// CDlgReplaceText 消息处理程序


BOOL CDlgReplaceText::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}
