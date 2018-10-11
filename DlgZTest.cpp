// DlgZTest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgZTest.h"
#include "afxdialogex.h"


// CDlgZTest �Ի���

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


// CDlgZTest ��Ϣ�������


void CDlgZTest::OnBnClickedButtonMove()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData())
		return;

	g_DAT.GoTo(0,0,m_dbZ);
}


BOOL CDlgZTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ctrSliderZ.SetRange(-12, 12, TRUE);
	m_ctrSliderZ.SetPos(0);

	m_bFixZ = (g_DAT.m_dbFixZ!=NOT_FIX_Z);

	m_dbZ = m_bFixZ ? g_DAT.m_dbFixZ : 0;
	m_ctrSliderZ.SetPos((int)m_dbZ);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgZTest::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdateData();

	g_DAT.m_dbFixZ = m_bFixZ ? m_dbZ : NOT_FIX_Z;

	CDialogEx::OnClose();
}
