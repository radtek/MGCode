// DlgZTest.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgZTest.h"
#include "afxdialogex.h"


// CDlgZTest 对话框

IMPLEMENT_DYNAMIC(CDlgZTest, CDialogEx)

CDlgZTest::CDlgZTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgZTest::IDD, pParent)
{
	m_bFixZ = FALSE;
	m_dbZ = 0.0;
}

CDlgZTest::~CDlgZTest()
{
}

void CDlgZTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_FIX_Z, m_bFixZ);
	DDX_Text(pDX, IDC_EDIT_Z, m_dbZ);
	DDX_Control(pDX, IDC_SLIDER_Z, m_ctrSliderZ);
}


BEGIN_MESSAGE_MAP(CDlgZTest, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MOVE, &CDlgZTest::OnBnClickedButtonMove)
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgZTest 消息处理程序


void CDlgZTest::OnBnClickedButtonMove()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;

	g_DAT.GoTo(0,0,m_dbZ);
}


BOOL CDlgZTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ctrSliderZ.SetRange(-12, 12, TRUE);
	m_ctrSliderZ.SetPos(0);

	m_bFixZ = (g_DAT.m_dbFixZ!=NOT_FIX_Z);

	m_dbZ = m_bFixZ ? g_DAT.m_dbFixZ : 0;
	m_ctrSliderZ.SetPos((int)m_dbZ);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgZTest::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nCtrlID = pScrollBar->GetDlgCtrlID();
	if (nCtrlID==IDC_SLIDER_Z)
	{
		CSliderCtrl *pSliderCtrl = (CSliderCtrl *)pScrollBar;
		m_dbZ = pSliderCtrl->GetPos();
		UpdateData(FALSE);

		OnBnClickedButtonMove();
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlgZTest::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData();

	g_DAT.m_dbFixZ = m_bFixZ ? m_dbZ : NOT_FIX_Z;

	CDialogEx::OnClose();
}
