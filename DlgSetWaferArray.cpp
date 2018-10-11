// DlgSetWaferArray.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgSetWaferArray.h"
#include "afxdialogex.h"


// CDlgSetWaferArray 对话框

IMPLEMENT_DYNAMIC(CDlgSetWaferArray, CDialogEx)

CDlgSetWaferArray::CDlgSetWaferArray(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSetWaferArray::IDD, pParent)
{

	m_dbRowDist = 0.0;
	m_dbColDist = 0.0;
	m_dbShrinkRow = 0.0;
	m_dbShrinkYCol = 0.0;
}

CDlgSetWaferArray::~CDlgSetWaferArray()
{
}

void CDlgSetWaferArray::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROW_DISTANCE, m_dbRowDist);
	DDX_Text(pDX, IDC_EDIT_COLUMN_DISTANCE, m_dbColDist);
	DDX_Text(pDX, IDC_EDIT_SHRINK, m_dbShrinkRow);
	DDX_Text(pDX, IDC_EDIT_SHRINK_Y, m_dbShrinkYCol);
}


BEGIN_MESSAGE_MAP(CDlgSetWaferArray, CDialogEx)
END_MESSAGE_MAP()


// CDlgSetWaferArray 消息处理程序
