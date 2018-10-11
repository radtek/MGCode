// CHandPos.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "CHandPos.h"
#include "afxdialogex.h"

// CCHandPos 对话框

IMPLEMENT_DYNAMIC(CCHandPos, CDialogEx)

CCHandPos::CCHandPos(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCHandPos::IDD, pParent)
{
	m_dbHandMoveDis = 1.0;

	m_bInit = FALSE;
	m_left.SetOper(MOVE_LEFT);
	m_right.SetOper(MOVE_RIGHT);
	m_top.SetOper(MOVE_TOP);
	m_bottom.SetOper(MOVE_BOTTOM);
}

CCHandPos::~CCHandPos()
{
}

void CCHandPos::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_LEFT, m_left);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_right);
	DDX_Control(pDX, IDC_STATIC_TOP, m_top);
	DDX_Control(pDX, IDC_STATIC_BOTTOM, m_bottom);
	DDX_Text(pDX, IDC_EDIT_HAND_MOVE_DIS, m_dbHandMoveDis);
}


BEGIN_MESSAGE_MAP(CCHandPos, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL, &CCHandPos::OnBnClickedRadioMoveModel)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL1, &CCHandPos::OnBnClickedRadioMoveModel1)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL2, &CCHandPos::OnBnClickedRadioMoveModel2)
	ON_BN_CLICKED(IDC_RADIO_MOVE_MODEL3, &CCHandPos::OnBnClickedRadioMoveModel3)
	ON_EN_CHANGE(IDC_EDIT_HAND_MOVE_DIS, &CCHandPos::OnEnChangeEditHandMoveDis)
//	ON_WM_DESTROY()
ON_WM_DESTROY()
END_MESSAGE_MAP()


// CCHandPos 消息处理程序


BOOL CCHandPos::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_nOldMoveModel = CFStatic::m_nMoveModel;
	m_dbOldHandMoveDis = CFStatic::m_dbMoveDis;

	CFStatic::m_nMoveModel = 2;
	CheckDlgButton(IDC_RADIO_MOVE_MODEL2, TRUE);
	CFStatic::m_dbMoveDis = m_dbHandMoveDis = 1.0;
	
	UpdateData(FALSE);
//	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCHandPos::OnBnClickedRadioMoveModel()
{
	// TODO: 在此添加控件通知处理程序代码
	CFStatic::m_nMoveModel = 0;
}


void CCHandPos::OnBnClickedRadioMoveModel1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFStatic::m_nMoveModel = 1;
}


void CCHandPos::OnBnClickedRadioMoveModel2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFStatic::m_nMoveModel = 2;
}


void CCHandPos::OnBnClickedRadioMoveModel3()
{
	// TODO: 在此添加控件通知处理程序代码
	CFStatic::m_nMoveModel = 3;
}


void CCHandPos::OnEnChangeEditHandMoveDis()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString strMoveDis;
	GetDlgItemText(IDC_EDIT_HAND_MOVE_DIS, strMoveDis);
	CFStatic::m_dbMoveDis = atof(strMoveDis);
	// TODO:  在此添加控件通知处理程序代码
}



void CCHandPos::OnDestroy()
{
	CDialogEx::OnDestroy();

	CFStatic::m_nMoveModel = m_nOldMoveModel;
	CFStatic::m_dbMoveDis = m_dbOldHandMoveDis;
}
