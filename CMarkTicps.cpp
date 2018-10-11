// CMarkTicps.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "CMarkTicps.h"
#include "afxdialogex.h"


// CCMarkTicps 对话框

IMPLEMENT_DYNAMIC(CCMarkTicps, CDialogEx)

CCMarkTicps::CCMarkTicps(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCMarkTicps::IDD, pParent)
	, m_strMarkTicps(_T(""))
{
	m_bMarkTicps = TRUE;
}

CCMarkTicps::~CCMarkTicps()
{
}

void CCMarkTicps::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_MARKING, m_strMarkTicps);
}


BEGIN_MESSAGE_MAP(CCMarkTicps, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCMarkTicps 消息处理程序


HBRUSH CCMarkTicps::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	int nId = pWnd->GetDlgCtrlID();
	if (nId == IDC_STATIC_MARKING)
	{
		pDC->SetBkColor(RGB(255,0,0));
		pDC->SetTextColor(RGB(255,255,255));
		//pDC->SetBkMode(TRANSPARENT);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CCMarkTicps::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_bMarkTicps)
	{
		m_strMarkTicps.LoadString(IDS_STRINGMARKING);
	}
	else
	{
		m_strMarkTicps = "AutoOffseting";
	}
	m_font_.CreatePointFont(1500, "MS Shell Dlg");
	GetDlgItem(IDC_STATIC_MARKING)->SetFont(&m_font_);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
