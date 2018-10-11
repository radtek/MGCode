// CHandPos.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "CHandPos.h"
#include "afxdialogex.h"

// CCHandPos �Ի���

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


// CCHandPos ��Ϣ�������


BOOL CCHandPos::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_nOldMoveModel = CFStatic::m_nMoveModel;
	m_dbOldHandMoveDis = CFStatic::m_dbMoveDis;

	CFStatic::m_nMoveModel = 2;
	CheckDlgButton(IDC_RADIO_MOVE_MODEL2, TRUE);
	CFStatic::m_dbMoveDis = m_dbHandMoveDis = 1.0;
	
	UpdateData(FALSE);
//	::SetWindowPos(GetSafeHwnd(), HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CCHandPos::OnBnClickedRadioMoveModel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFStatic::m_nMoveModel = 0;
}


void CCHandPos::OnBnClickedRadioMoveModel1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFStatic::m_nMoveModel = 1;
}


void CCHandPos::OnBnClickedRadioMoveModel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFStatic::m_nMoveModel = 2;
}


void CCHandPos::OnBnClickedRadioMoveModel3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFStatic::m_nMoveModel = 3;
}


void CCHandPos::OnEnChangeEditHandMoveDis()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString strMoveDis;
	GetDlgItemText(IDC_EDIT_HAND_MOVE_DIS, strMoveDis);
	CFStatic::m_dbMoveDis = atof(strMoveDis);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}



void CCHandPos::OnDestroy()
{
	CDialogEx::OnDestroy();

	CFStatic::m_nMoveModel = m_nOldMoveModel;
	CFStatic::m_dbMoveDis = m_dbOldHandMoveDis;
}
