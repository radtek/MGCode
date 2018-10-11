// GroupList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "GroupList.h"
#include "afxdialogex.h"


// CGroupList �Ի���

IMPLEMENT_DYNAMIC(CGroupList, CDialogEx)

CGroupList::CGroupList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGroupList::IDD, pParent)
	, m_Row(0)
	, m_Col(0)
{
	m_pGroupObj = NULL;
}

CGroupList::~CGroupList()
{
}

void CGroupList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GROUP, m_GroupList);
	DDX_Control(pDX, IDC_EDIT_GROUPNUM, m_edit);
}


BEGIN_MESSAGE_MAP(CGroupList, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GROUP, &CGroupList::OnNMDblclkListGroup)
	ON_EN_KILLFOCUS(IDC_EDIT_GROUPNUM, &CGroupList::OnEnKillfocusEditGroupnum)
	ON_NOTIFY(NM_CLICK, IDC_LIST_GROUP, &CGroupList::OnNMClickListGroup)
END_MESSAGE_MAP()


// CGroupList ��Ϣ�������


BOOL CGroupList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


	LONG lStyle;
	lStyle = GetWindowLong(m_GroupList.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT; 
	SetWindowLong(m_GroupList.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_GroupList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	//dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	m_GroupList.SetExtendedStyle(dwStyle); //������չ���


	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (NULL == pFWnd)
	{
		return FALSE;
	}
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{	
		return FALSE;
	}
	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return FALSE;
	}

	m_GroupList.InsertColumn( 0, "Num",LVCFMT_CENTER, 150 );
	m_GroupList.InsertColumn( 1, "GroupNum", LVCFMT_CENTER, 150);
	CGroupObj *pGroupObj = NULL;
	POSITION pos = pDoc->m_chain.m_list.GetHeadPosition();
	int nCount = 0;
	while(pos != NULL)
	{
		pGroupObj=pDoc->m_chain.m_list.GetNext(pos);
		if (pGroupObj)
		{
			CString strId;
			strId.Format(_T("%d"),nCount);
			m_GroupList.InsertItem(nCount,strId);

			CString strGroup;
			strGroup.Format(_T("%d"),pGroupObj->GetGroupNum());
			m_GroupList.SetItemText(nCount++,1,strGroup);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGroupList::OnNMDblclkListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;
	m_Row = pNMListView->iItem;
	m_Col = pNMListView->iSubItem;
	if   (m_Col != 0 && m_Row > -1) 
	{
		m_GroupList.GetSubItemRect(m_Row,m_Col,LVIR_LABEL,rc);
		m_edit.SetParent(&m_GroupList);
		m_edit.MoveWindow(rc);
		m_edit.SetWindowText(m_GroupList.GetItemText(m_Row,m_Col));
		m_edit.ShowWindow(SW_SHOW);
		m_edit.SetFocus();
		m_edit.ShowCaret();
		m_edit.SetSel(-1);


		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
		CGroupObj *pGroupObj = NULL;
		POSITION pos = pDoc->m_chain.m_list.GetHeadPosition();
		int nCount = 0;
		while(pos != NULL)
		{
			pGroupObj=pDoc->m_chain.m_list.GetNext(pos);
			if (pGroupObj)
			{
				if (nCount == m_Row)
				{
					m_pGroupObj = pGroupObj;
				}
				nCount++;
			}
		}

	}
	*pResult = 0;
}


void CGroupList::OnEnKillfocusEditGroupnum()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tem;
	m_edit.GetWindowText(tem);    
	int nGroupNum = _ttoi(tem);
	if ((nGroupNum == 0 && tem != "0") || nGroupNum > 100)
	{
		AfxMessageBox(_T("Input Num Error��"));
		return;
	}
	m_GroupList.SetItemText(m_Row,m_Col,tem); 
	m_edit.ShowWindow(SW_HIDE); 

	m_pGroupObj->SetGroupNum(nGroupNum);

}


void CGroupList::OnNMClickListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
