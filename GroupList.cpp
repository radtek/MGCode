// GroupList.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "GroupList.h"
#include "afxdialogex.h"


// CGroupList 对话框

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


// CGroupList 消息处理程序


BOOL CGroupList::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	LONG lStyle;
	lStyle = GetWindowLong(m_GroupList.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT; 
	SetWindowLong(m_GroupList.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_GroupList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_GroupList.SetExtendedStyle(dwStyle); //设置扩展风格


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
	// 异常: OCX 属性页应返回 FALSE
}

void CGroupList::OnNMDblclkListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	CString tem;
	m_edit.GetWindowText(tem);    
	int nGroupNum = _ttoi(tem);
	if ((nGroupNum == 0 && tem != "0") || nGroupNum > 100)
	{
		AfxMessageBox(_T("Input Num Error！"));
		return;
	}
	m_GroupList.SetItemText(m_Row,m_Col,tem); 
	m_edit.ShowWindow(SW_HIDE); 

	m_pGroupObj->SetGroupNum(nGroupNum);

}


void CGroupList::OnNMClickListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
