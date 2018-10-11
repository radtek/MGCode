// DsfDlgEditTime.cpp : implementation file
//

#include "stdafx.h"
#include "HL.h"
#include "DsfDlgEditTime.h"
#include "afxdialogex.h"


// CDsfDlgEditTime dialog

IMPLEMENT_DYNAMIC(CDsfDlgEditTime, CDialogEx)

CDsfDlgEditTime::CDsfDlgEditTime(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDsfDlgEditTime::IDD, pParent)
	, m_nTime(0)
{

}

CDsfDlgEditTime::~CDsfDlgEditTime()
{
}

void CDsfDlgEditTime::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIME, m_nTime);
}


BEGIN_MESSAGE_MAP(CDsfDlgEditTime, CDialogEx)
END_MESSAGE_MAP()


// CDsfDlgEditTime message handlers
