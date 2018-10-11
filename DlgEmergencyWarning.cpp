// DlgEmergencyWarning.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "DlgEmergencyWarning.h"
#include "afxdialogex.h"
#include "EmergencyWatcher.h"
#include "DlgMarkNormalPane.h"

extern CEmergencyWatcher g_EmergencyWatcher;

// CDlgEmergencyWarning 对话框

IMPLEMENT_DYNAMIC(CDlgEmergencyWarning, CDialogEx)

CDlgEmergencyWarning::CDlgEmergencyWarning(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgEmergencyWarning::IDD, pParent)
{
	m_bSetTime = FALSE;
}

CDlgEmergencyWarning::~CDlgEmergencyWarning()
{
}

void CDlgEmergencyWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgEmergencyWarning, CDialogEx)
//	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_HOME, &CDlgEmergencyWarning::OnBnClickedButtonHome)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLOSESOFT, &CDlgEmergencyWarning::OnBnClickedBtnClosesoft)
END_MESSAGE_MAP()


// CDlgEmergencyWarning 消息处理程序
BOOL CDlgEmergencyWarning::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	BOOL bShowHomeButton = g_IO.INPUT_WORKBENCH_DISABLE();
	GetDlgItem(IDC_BUTTON_HOME)->ShowWindow(bShowHomeButton);
	GetDlgItem(IDC_BTN_CLOSESOFT)->ShowWindow(bShowHomeButton);

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	g_IO.OUTPUT_SOFT_OK(FALSE);
	// TODO:  在此添加额外的初始化
	OutputDebugString("CDlgEmergencyWarning OnInitDialog");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgEmergencyWarning::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (g_IO.INPUT_SAFEDOOR())
		return;

	if (!g_pMotion->IsHomed(eMaskXYZ))
	{
		if (m_bSetTime)
		{
			GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(TRUE);
			m_bSetTime = FALSE;
		}
		return;
	}
	::ResetEvent(g_hEventMainThreadEmergency);
	g_IO.OUTPUT_SOFT_OK(TRUE);
	CDialogEx::OnClose();
}


void CDlgEmergencyWarning::OnBnClickedButtonHome()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_IO.INPUT_SAFEDOOR())
	{
		g_pMotion->StartHome(eMaskXYZ);
		GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(FALSE);
		SetTimer(110,100,NULL);
		m_bSetTime = TRUE;
	}
	else
	{
		AfxMessageBox("请先关闭安全门!");
	}

}


void CDlgEmergencyWarning::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 110)
	{
		if (g_pMotion->WaitHomeFinish(0))
		{
			KillTimer(nIDEvent);
			PostMessage(WM_CLOSE);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDlgEmergencyWarning::OnBnClickedBtnClosesoft()
{
	// TODO: 在此添加控件通知处理程序代码
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (NULL != pFWnd)
	{
		::ResetEvent(g_hEventMainThreadEmergency);
		pFWnd->SendMessage(WM_CLOSE);
		OutputDebugString("CloseWindow");
	}
		
}
