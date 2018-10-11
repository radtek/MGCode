// CMarkTicps.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "CMarkTicps.h"
#include "afxdialogex.h"


// CCMarkTicps �Ի���

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


// CCMarkTicps ��Ϣ�������


HBRUSH CCMarkTicps::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	int nId = pWnd->GetDlgCtrlID();
	if (nId == IDC_STATIC_MARKING)
	{
		pDC->SetBkColor(RGB(255,0,0));
		pDC->SetTextColor(RGB(255,255,255));
		//pDC->SetBkMode(TRANSPARENT);
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CCMarkTicps::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	// �쳣: OCX ����ҳӦ���� FALSE
}
