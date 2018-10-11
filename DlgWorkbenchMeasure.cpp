// DlgWorkbenchMeasure.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgWorkbenchMeasure.h"
#include "afxdialogex.h"


// CDlgWorkbenchMeasure �Ի���

IMPLEMENT_DYNAMIC(CDlgWorkbenchMeasure, CDialogEx)

CDlgWorkbenchMeasure::CDlgWorkbenchMeasure(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgWorkbenchMeasure::IDD, pParent)
{

	m_dbX1 = 0.0;
	m_dbX2 = 0.0;
	//  m_dbY1 = _T("");
	m_dbY1 = 0.0;
	m_dbY2 = 0.0;
	m_dbPointsDis = 0.0;
}

CDlgWorkbenchMeasure::~CDlgWorkbenchMeasure()
{
}

void CDlgWorkbenchMeasure::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X1, m_dbX1);
	DDX_Text(pDX, IDC_EDIT_X2, m_dbX2);
	//  DDX_Text(pDX, IDC_EDIT_Y1, m_dbY1);
	DDX_Text(pDX, IDC_EDIT_Y1, m_dbY1);
	DDX_Text(pDX, IDC_EDIT_Y2, m_dbY2);
	DDX_Text(pDX, IDC_EDIT_POINTS_DIS, m_dbPointsDis);
}


BEGIN_MESSAGE_MAP(CDlgWorkbenchMeasure, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GET_POS1, &CDlgWorkbenchMeasure::OnBnClickedButtonGetPos1)
	ON_BN_CLICKED(IDC_BUTTON_GET_POS2, &CDlgWorkbenchMeasure::OnBnClickedButtonGetPos2)
	ON_BN_CLICKED(IDC_BUTTON_CALCULATE, &CDlgWorkbenchMeasure::OnBnClickedButtonCalculate)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlgWorkbenchMeasure ��Ϣ�������


void CDlgWorkbenchMeasure::OnBnClickedButtonGetPos1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData())
		return;

	m_dbX1 = g_pMotion->GetPos(eMaskX);
	m_dbY1 = g_pMotion->GetPos(eMaskY);
	UpdateData(FALSE);
}


void CDlgWorkbenchMeasure::OnBnClickedButtonGetPos2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData())
		return;

	m_dbX2 = g_pMotion->GetPos(eMaskX);
	m_dbY2 = g_pMotion->GetPos(eMaskY);
	UpdateData(FALSE);
}


void CDlgWorkbenchMeasure::OnBnClickedButtonCalculate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData())
		return;

	m_dbPointsDis = sqrt((m_dbX1-m_dbX2)*(m_dbX1-m_dbX2) + (m_dbY1-m_dbY2)*(m_dbY1-m_dbY2));
	UpdateData(FALSE);
}


void CDlgWorkbenchMeasure::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	ShowWindow(SW_HIDE);
	return;	
}
