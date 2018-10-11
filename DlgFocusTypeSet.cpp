// DlgFocusTypeSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgFocusTypeSet.h"
#include "afxdialogex.h"


// CDlgFocusTypeSet �Ի���

IMPLEMENT_DYNAMIC(CDlgFocusTypeSet, CDialogEx)

CDlgFocusTypeSet::CDlgFocusTypeSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgFocusTypeSet::IDD, pParent)
{

}

CDlgFocusTypeSet::~CDlgFocusTypeSet()
{
}

void CDlgFocusTypeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FOCUS_TYPE, m_listCtrlFocusType);
	DDX_Text(pDX, IDC_EDIT_FOCUS, m_strFocus);
	DDX_Text(pDX, IDC_EDIT_MOVE_DIS, m_strMoveDis);
	DDX_Text(pDX, IDC_EDIT_MOVE_NUM, m_strMoveNum);
}


BEGIN_MESSAGE_MAP(CDlgFocusTypeSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgFocusTypeSet::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgFocusTypeSet::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgFocusTypeSet::OnBnClickedButtonEdit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FOCUS_TYPE, &CDlgFocusTypeSet::OnNMClickListFocusType)
END_MESSAGE_MAP()


// CDlgFocusTypeSet ��Ϣ�������


BOOL CDlgFocusTypeSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listCtrlFocusType.ModifyStyle(0, LVS_REPORT&LVS_SINGLESEL);
	m_listCtrlFocusType.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	m_listCtrlFocusType.InsertColumn(0, "", 0, 0);
	m_listCtrlFocusType.InsertColumn(1, "Type", 0, 50);
	m_listCtrlFocusType.InsertColumn(2, "Pos", 0, 50);
	m_listCtrlFocusType.InsertColumn(3, "MoveTimes", 0, 100);
	m_listCtrlFocusType.InsertColumn(4, "Move", 0, 50);

	m_listCtrlFocusType.InsertItem(0, "");
	m_listCtrlFocusType.SetItemText(0, 1, "1");
	m_listCtrlFocusType.SetItemText(0, 2, "9.8");
	m_listCtrlFocusType.SetItemText(0, 3, "0");
	m_listCtrlFocusType.SetItemText(0, 4, "1");

	m_listCtrlFocusType.InsertItem(1, "");
	m_listCtrlFocusType.SetItemText(1, 1, "1");
	m_listCtrlFocusType.SetItemText(1, 2, "9.8");
	m_listCtrlFocusType.SetItemText(1, 3, "0");
	m_listCtrlFocusType.SetItemText(1, 4, "1");

	RefreshFocusID();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgFocusTypeSet::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex = m_listCtrlFocusType.InsertItem(100, "");
	m_listCtrlFocusType.SetItemText(nIndex, 1, "0");
	m_listCtrlFocusType.SetItemText(nIndex, 2, "0");
	m_listCtrlFocusType.SetItemText(nIndex, 3, "0");
	m_listCtrlFocusType.SetItemText(nIndex, 4, "0");

	RefreshFocusID();
}


void CDlgFocusTypeSet::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION posSel = m_listCtrlFocusType.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_listCtrlFocusType.GetNextSelectedItem(posSel);
		m_listCtrlFocusType.DeleteItem(nCurSel);
	}


	RefreshFocusID();
}


void CDlgFocusTypeSet::OnBnClickedButtonEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData())
		return;

	POSITION posSel = m_listCtrlFocusType.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_listCtrlFocusType.GetNextSelectedItem(posSel);

		m_listCtrlFocusType.SetItemText(nCurSel, 2, m_strFocus);
		m_listCtrlFocusType.SetItemText(nCurSel, 3, m_strMoveNum);
		m_listCtrlFocusType.SetItemText(nCurSel, 4, m_strMoveDis);
	}
}


void CDlgFocusTypeSet::OnNMClickListFocusType(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	POSITION posSel = m_listCtrlFocusType.GetFirstSelectedItemPosition();
	if (posSel)
	{
		int nCurSel = m_listCtrlFocusType.GetNextSelectedItem(posSel);

		m_strFocus   = m_listCtrlFocusType.GetItemText(nCurSel, 2);
		m_strMoveNum = m_listCtrlFocusType.GetItemText(nCurSel, 3);
		m_strMoveDis = m_listCtrlFocusType.GetItemText(nCurSel, 4);
		UpdateData(FALSE);
	}

	*pResult = 0;
}

void CDlgFocusTypeSet::RefreshFocusID()
{
	CString strID;
	int nCount = m_listCtrlFocusType.GetItemCount();
	for (int i=0; i<nCount; i++)
	{
		strID.Format("%d", i+1);
		m_listCtrlFocusType.SetItemText(i, 1, strID);
	}
}