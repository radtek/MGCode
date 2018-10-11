// DlgSetDxfPrecision.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgSetDxfPrecision.h"
#include "afxdialogex.h"


// CDlgSetDxfPrecision 对话框

IMPLEMENT_DYNAMIC(CDlgSetDxfPrecision, CDialogEx)

CDlgSetDxfPrecision::CDlgSetDxfPrecision(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetDxfPrecision::IDD, pParent)
	, m_dbDxfPrecision(0)
	, m_dbGerberOffset(0)
{

}

CDlgSetDxfPrecision::~CDlgSetDxfPrecision()
{
}

void CDlgSetDxfPrecision::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PRECISION, m_dbDxfPrecision);
	DDX_Text(pDX, IDC_EDIT_GBX_OFFSET, m_dbGerberOffset);
}


BEGIN_MESSAGE_MAP(CDlgSetDxfPrecision, CDialogEx)
END_MESSAGE_MAP()


// CDlgSetDxfPrecision 消息处理程序

void CDlgSetDxfPrecision::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (!UpdateData())
		return;

	CDialogEx::OnOK();
}
