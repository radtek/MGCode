// GroupNum.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "GroupNum.h"
#include "afxdialogex.h"


// CGroupNum 对话框

IMPLEMENT_DYNAMIC(CGroupNum, CDialogEx)

CGroupNum::CGroupNum(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGroupNum::IDD, pParent)
	, m_nGroupNum(0)
{

}

CGroupNum::~CGroupNum()
{
}

void CGroupNum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GROUPNUM, m_nGroupNum);
	DDV_MinMaxInt(pDX, m_nGroupNum, 0, 300);
}


BEGIN_MESSAGE_MAP(CGroupNum, CDialogEx)
END_MESSAGE_MAP()


// CGroupNum 消息处理程序
