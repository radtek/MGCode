// BarcodeSetLen.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "BarcodeSetLen.h"
#include "afxdialogex.h"


// CBarcodeSetLen 对话框

IMPLEMENT_DYNAMIC(CBarcodeSetLen, CDialogEx)

CBarcodeSetLen::CBarcodeSetLen(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBarcodeSetLen::IDD, pParent)
	, m_dbBarcodeLen(0)
	, m_dbBarcodeWid(0)
	, m_bSameLen(TRUE)
{

}

CBarcodeSetLen::~CBarcodeSetLen()
{
}

void CBarcodeSetLen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_dbBarcodeLen);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_dbBarcodeWid);
	DDX_Check(pDX, IDC_CHECK_SAMELEN, m_bSameLen);
}


BEGIN_MESSAGE_MAP(CBarcodeSetLen, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBarcodeSetLen::OnBnClickedOk)
END_MESSAGE_MAP()


// CBarcodeSetLen 消息处理程序


void CBarcodeSetLen::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
		
	CDialogEx::OnOK();
}


BOOL CBarcodeSetLen::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
