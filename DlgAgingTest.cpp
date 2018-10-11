// DlgAgingTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgAgingTest.h"
#include "afxdialogex.h"
#include "DlgMarkNormalPane.h"

#define TIMER_AGING 1
// CDlgAgingTest �Ի���

IMPLEMENT_DYNAMIC(CDlgAgingTest, CDialogEx)

CDlgAgingTest::CDlgAgingTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAgingTest::IDD, pParent)
{

}

CDlgAgingTest::~CDlgAgingTest()
{
}

void CDlgAgingTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAgingTest, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_START, &CDlgAgingTest::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDlgAgingTest::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CDlgAgingTest ��Ϣ�������


BOOL CDlgAgingTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LOGFONT logfont;
	GetFont()->GetLogFont(&logfont);

	logfont.lfHeight = -200;
	m_font.CreateFontIndirect(&logfont);
	GetDlgItem(IDC_STATIC_TIP)->SetFont(&m_font);
	GetDlgItem(IDC_STATIC_TIP)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

	g_pDlgMarkNormalPane->m_bDisableOtherWindowInputOnMarking = FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgAgingTest::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!GetDlgItem(IDC_BUTTON_START)->IsWindowEnabled())
		return;

	g_pDlgMarkNormalPane->m_bDisableOtherWindowInputOnMarking = TRUE;
	CDialogEx::OnClose();
}

void CDlgAgingTest::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent==TIMER_AGING)
	{
		if (!g_pDlgMarkNormalPane->m_bMarking)
		{
			BOOL bMoving = (g_pMotion->GetStatus(eMaskX)&eStatusBitMoving) ||(g_pMotion->GetStatus(eMaskY)&eStatusBitMoving) ||	(g_pMotion->GetStatus(eMaskZ)&eStatusBitMoving);
			if (!bMoving)
			{
				g_pDlgMarkNormalPane->m_bMarking = TRUE;
				g_pDlgMarkNormalPane->PostMessage(WM_MARK_START);
			}
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CDlgAgingTest::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_STATIC_TIP)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
	SetTimer(TIMER_AGING, 1000, NULL);
}


void CDlgAgingTest::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_STATIC_TIP)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	KillTimer(TIMER_AGING);
	g_pDlgMarkNormalPane->OnBnClickedButtonStop();
}
