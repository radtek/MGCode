// DlgReplaceText.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgReplaceText.h"
#include "afxdialogex.h"


// CDlgReplaceText �Ի���

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


// CDlgReplaceText ��Ϣ�������


BOOL CDlgReplaceText::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}
