// CCDSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "CCDSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCCDSettingDlg dialog


CCCDSettingDlg::CCCDSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCCDSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCCDSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCCDSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCCDSettingDlg)
	DDX_Control(pDX, IDC_TAB, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCCDSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CCCDSettingDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCDSettingDlg message handlers

void CCCDSettingDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iTab=m_tab.GetCurSel();
	TC_ITEM tci;
	
	tci.mask=TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_SHOW);
	pWnd->EnableWindow(TRUE);
	
	*pResult = 0;
}

void CCCDSettingDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iTab=m_tab.GetCurSel();
	TC_ITEM tci;
	
	tci.mask=TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_HIDE);
	pWnd->EnableWindow(FALSE);
	
	*pResult = 0;
}

BOOL CCCDSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rectTab;
//	m_tab.GetItemRect(rectTab);
	

	//生成标签
	TC_ITEM TabItem;
	CString str;
	TabItem.pszText=new TCHAR[255];

	
	TabItem.mask=TCIF_PARAM|TCIF_TEXT;
	TabItem.lParam=(LPARAM)&m_page1;
	VERIFY(m_page1.Create(CCCDArrayCom::IDD,&m_tab));
	str=("CCDSetting");
	_tcscpy(TabItem.pszText,str);
	m_tab.InsertItem(0,&TabItem);
	m_page1.SetWindowPos(NULL,3,23,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_page1.ShowWindow(SW_SHOW);
	m_page1.EnableWindow(TRUE);
	
	
	TabItem.mask=TCIF_PARAM|TCIF_TEXT;
	TabItem.lParam=(LPARAM)&m_page2;
	VERIFY(m_page2.Create(CCCDAdjustDlg::IDD,&m_tab));
	str=("CCDOffset");
	_tcscpy(TabItem.pszText,str);
	m_tab.InsertItem(1,&TabItem);
	m_page2.SetWindowPos(NULL,3,23,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_page2.ShowWindow(SW_HIDE);
	m_page2.EnableWindow(FALSE);
	
	TabItem.mask=TCIF_PARAM|TCIF_TEXT;
	TabItem.lParam=(LPARAM)&m_page3;
	VERIFY(m_page3.Create(CCCDWorkDlg::IDD,&m_tab));
	str=("CCDWork");
	_tcscpy(TabItem.pszText,str);
	m_tab.InsertItem(2,&TabItem);
	m_page3.SetWindowPos(NULL,3,23,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_page3.ShowWindow(SW_HIDE);
	m_page3.EnableWindow(FALSE);

	delete TabItem.pszText;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCCDSettingDlg::SetTable(int nItem)
{
	int iTab=m_tab.GetCurSel();
	TC_ITEM tci;
	tci.mask=TCIF_PARAM;
	
	//隐藏原有的标签
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_HIDE);
	pWnd->EnableWindow(FALSE);
	
	//显示选定的标签
	m_tab.SetCurSel(nItem);
	
	m_tab.GetItem(nItem,&tci);
	ASSERT(tci.lParam);
	
	pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_SHOW);
	pWnd->EnableWindow(TRUE);

}

