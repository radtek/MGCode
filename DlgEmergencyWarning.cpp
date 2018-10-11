// DlgEmergencyWarning.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgEmergencyWarning.h"
#include "afxdialogex.h"
#include "EmergencyWatcher.h"
#include "DlgMarkNormalPane.h"

extern CEmergencyWatcher g_EmergencyWatcher;

// CDlgEmergencyWarning �Ի���

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


// CDlgEmergencyWarning ��Ϣ�������
BOOL CDlgEmergencyWarning::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	BOOL bShowHomeButton = g_IO.INPUT_WORKBENCH_DISABLE();
	GetDlgItem(IDC_BUTTON_HOME)->ShowWindow(bShowHomeButton);
	GetDlgItem(IDC_BTN_CLOSESOFT)->ShowWindow(bShowHomeButton);

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	g_IO.OUTPUT_SOFT_OK(FALSE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	OutputDebugString("CDlgEmergencyWarning OnInitDialog");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgEmergencyWarning::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!g_IO.INPUT_SAFEDOOR())
	{
		g_pMotion->StartHome(eMaskXYZ);
		GetDlgItem(IDC_BUTTON_HOME)->EnableWindow(FALSE);
		SetTimer(110,100,NULL);
		m_bSetTime = TRUE;
	}
	else
	{
		AfxMessageBox("���ȹرհ�ȫ��!");
	}

}


void CDlgEmergencyWarning::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (NULL != pFWnd)
	{
		::ResetEvent(g_hEventMainThreadEmergency);
		pFWnd->SendMessage(WM_CLOSE);
		OutputDebugString("CloseWindow");
	}
		
}
