// FillEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "FillEditDlg.h"
#include "HLView.h"
#include "FastMarkStopDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFillEditDlg dialog
extern CHLApp theApp;

CFillEditDlg::CFillEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFillEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFillEditDlg)
	m_nTabIndex = 0;
	//}}AFX_DATA_INIT
}


void CFillEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFillEditDlg)
	DDX_Control(pDX, IDC_TAB_OBJEDIT, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFillEditDlg, CDialog)
	//{{AFX_MSG_MAP(CFillEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLYFILL, OnButtonApplyfill)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_OBJEDIT, OnSelchangingTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OBJEDIT, OnSelchangeTab)
	ON_BN_CLICKED(IDC_BUTTON_FASTMARK, OnButtonFastmark)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFillEditDlg message handlers

void CFillEditDlg::OnButtonApplyfill() 
{
	if ((g_bIsSelMark) || (g_bIsRedPreview) || (g_bIsFastMark))
	{
		return;
	}
	
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	int nSel = m_tab.GetCurSel();
	if (0 == nSel) 
	{
		m_FillObjMarkSet.Apply();
	}
	else if (1 == nSel) 
	{
		FILLDATA fillNew;
		m_ObjFillEdit.GetNewFillData(fillNew);
		pDoc->UpdateFill(fillNew);
	}


	
	int nCodeId = 0;
	CDot QuadDot[1000];
	CGroupObj *pGroupObj = NULL;
	POSITION pos = pDoc->m_chain.m_list.GetHeadPosition();
	while( pos  != NULL )
	{	
		pGroupObj = (CGroupObj *)pDoc->m_chain.m_list.GetNext(pos);
		POSITION pos = pGroupObj->m_list.GetHeadPosition();
		CTree *pTree = NULL;
		while( pos != NULL )
		{	
			pTree = (CTree*)pGroupObj->m_list.GetNext(pos);
			if ( pTree )
			{
				pTree->GetTextBaseDot(pTree->m_pRoot,QuadDot,nCodeId);
			}
		}
	}
	
	m_BarEditDlg.Apply();
	m_TextEditDlg.Apply();


	nCodeId = 0;
	pos = pDoc->m_chain.m_list.GetHeadPosition();
	while( pos  != NULL )
	{	
		pGroupObj = (CGroupObj *)pDoc->m_chain.m_list.GetNext(pos);
		POSITION pos = pGroupObj->m_list.GetHeadPosition();
		CTree *pTree = NULL;
		while( pos != NULL )
		{	
			pTree = (CTree*)pGroupObj->m_list.GetNext(pos);
			if ( pTree )
			{
				pTree->MoveOldBaseDot(pTree->m_pRoot,QuadDot,nCodeId);
			}
		}
	}
	return;
}

BOOL CFillEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString str("");

	//生成标签
	TC_ITEM TabItem;
	TabItem.mask=TCIF_PARAM|TCIF_TEXT;
	TabItem.pszText=new TCHAR[255];

	TabItem.lParam=(LPARAM)&m_FillObjMarkSet;
	VERIFY(m_FillObjMarkSet.Create(CFillObjmMarkSetDlg::IDD,&m_tab));
	str.LoadString(IDS_MARKTYPESET);
	_tcscpy_s(TabItem.pszText,255,str);
	m_tab.InsertItem(0,&TabItem);
	m_FillObjMarkSet.SetWindowPos(NULL,4,25,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_FillObjMarkSet.ShowWindow(SW_HIDE);
	m_FillObjMarkSet.EnableWindow(FALSE);

	TabItem.lParam=(LPARAM)&m_ObjFillEdit;
	VERIFY(m_ObjFillEdit.Create(CObjecteditfill::IDD,&m_tab));
	str.LoadString(IDS_MARKFILLEDIT);
	_tcscpy_s(TabItem.pszText,255, str);
	m_tab.InsertItem(1,&TabItem);
	m_ObjFillEdit.SetWindowPos(NULL,4,25,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_ObjFillEdit.ShowWindow(SW_HIDE);
	m_ObjFillEdit.EnableWindow(FALSE);

/*
	TabItem.lParam=(LPARAM)&m_TextEditDlg;
	VERIFY(m_TextEditDlg.Create(CTextEditDlg::IDD, &m_tab));
	str.LoadString(IDS_TEXTTITLE);
	_tcscpy(TabItem.pszText,str);
	m_tab.InsertItem(2,&TabItem);
	m_TextEditDlg.SetWindowPos(NULL,0,25,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_TextEditDlg.ShowWindow(SW_HIDE);
	m_TextEditDlg.EnableWindow(FALSE);
*/

#ifdef _DOUBLE_HEAD_MARK
	TabItem.lParam=(LPARAM)&m_DBSpecialtyDlg;
	VERIFY(m_DBSpecialtyDlg.Create(CDbSpecialtyDlg::IDD, &m_tab));
	str.LoadString(IDS_DBSPECIAL);
	_tcscpy(TabItem.pszText,str);
	m_tab.InsertItem(3,&TabItem);
	m_DBSpecialtyDlg.SetWindowPos(NULL,0,25,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_DBSpecialtyDlg.ShowWindow(SW_HIDE);
	m_DBSpecialtyDlg.EnableWindow(FALSE);
#endif


    SetTable(0);
	m_nTabIndex = 0;
	delete TabItem.pszText;

	ShowSetTable(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFillEditDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab=m_tab.GetCurSel();
	if (-1 == iTab)
	{
		return;
	}
	
	m_nTabIndex = iTab;
	TC_ITEM tci;
	
	tci.mask=TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);

	CWnd *pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_HIDE);
	pWnd->EnableWindow(FALSE);

	*pResult = 0;
}

void CFillEditDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int iTab=m_tab.GetCurSel();
	if (-1 == iTab)
	{
		return;
	}

	m_nTabIndex = iTab;
	TC_ITEM tci;
	
	tci.mask=TCIF_PARAM;
	m_tab.GetItem(iTab,&tci);
	ASSERT(tci.lParam);

	CWnd *pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_SHOW);
	pWnd->EnableWindow(TRUE);

	*pResult = 0;
}

void CFillEditDlg::SetTable(int nItem)
{
	int nCnt = m_tab.GetItemCount();
	if (nItem >= nCnt) 
	{
		return;
	}

	int iTab = m_tab.GetCurSel();
	if (-1 == iTab)
	{
		return;
	}

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

	return;
}

void CFillEditDlg::ShowSetTable(BOOL bShow)
{
	if (bShow)
	{
		m_tab.ShowWindow(SW_SHOW);
		m_ObjFillEdit.ShowWindow(SW_HIDE);

#ifdef _DOUBLE_HEAD_MARK
		m_DBSpecialtyDlg.ShowWindow(SW_HIDE);
#endif
		m_FillObjMarkSet.ShowWindow(SW_SHOW);

		SetTable(m_nTabIndex);
	}
	else
	{
		m_ObjFillEdit.ShowWindow(SW_HIDE);
		m_FillObjMarkSet.ShowWindow(SW_HIDE);

#ifdef _DOUBLE_HEAD_MARK
		m_DBSpecialtyDlg.ShowWindow(SW_HIDE);
#endif
		m_tab.ShowWindow(SW_HIDE);
	}

	return;
}


void CFillEditDlg::CheckFastmark() 
{
	CFastMarkStopDlg FastMark;
	FastMark.DoModal();

	return;
}

void CFillEditDlg::OnButtonFastmark() 
{
	CFastMarkStopDlg FastMark;
	FastMark.DoModal();

	return;
}


void CFillEditDlg::ShowTextItem(CNode *pNode, FILLDATA &fill, BOOL bShow)
{
	if (TRUE == bShow)
	{
		CString str("");
		str.LoadString(IDS_TEXTTITLE);
		if (m_TextEditDlg.m_bIsShow) 
		{
			m_TextEditDlg.SetTextInfo(pNode, fill);
			return;
		}

		TC_ITEM TabItem;
		TabItem.mask=TCIF_PARAM|TCIF_TEXT;
		TabItem.pszText=new TCHAR[255];
		if (NULL == TabItem.pszText) 
		{
			return;
		}

		int nIndex = m_tab.GetItemCount();
		TabItem.lParam=(LPARAM)&m_TextEditDlg;
		
		if (!(m_TextEditDlg.IsCreateDlg()))
		{
			VERIFY(m_TextEditDlg.Create(CTextEditDlg::IDD, &m_tab));
		}

		_tcscpy_s(TabItem.pszText, 255, str);
		m_tab.InsertItem(nIndex,&TabItem);
		m_TextEditDlg.SetWindowPos(NULL,4,25,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_TextEditDlg.ShowWindow(SW_HIDE);
		m_TextEditDlg.EnableWindow(FALSE);

		SetTable(nIndex);
		m_nTabIndex = nIndex;

		if (TabItem.pszText)
		{
			delete TabItem.pszText;
		}

		m_TextEditDlg.SetShow(TRUE);

		m_TextEditDlg.SetTextInfo(pNode, fill);
	}
	else
	{
		TC_ITEM tci;
		memset(&tci, 0 , sizeof(TC_ITEM));
		tci.mask=TCIF_PARAM;
		int nCount = m_tab.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			// 隐藏原有的标签
			m_tab.GetItem(i,&tci);
			ASSERT(tci.lParam);
			if(tci.lParam == ((LPARAM)&m_TextEditDlg))
			{
				m_tab.DeleteItem(i);
				m_tab.SetCurSel(1);
				SetTable(0);
				m_nTabIndex = 0;
				m_TextEditDlg.ShowWindow(SW_HIDE);
	            m_TextEditDlg.EnableWindow(FALSE);
				break;
			}
			
		}//end for (int i = 0; i < nCount; i++)

		m_TextEditDlg.SetTextInfo(pNode, fill);
		m_TextEditDlg.SetShow(FALSE);
	}

	return;
}


void CFillEditDlg::ShowBarItem(CNode *pNode, FILLDATA &fill, BOOL bShow)
{
	if (TRUE == bShow)
	{
		CString str("");
		str.LoadString(IDS_BARTITLE);
		if (m_BarEditDlg.m_bIsShow) 
		{
			m_BarEditDlg.SetTextInfo(pNode, fill);
			return;
		}

		TC_ITEM TabItem;
		TabItem.mask=TCIF_PARAM|TCIF_TEXT;
		TabItem.pszText=new TCHAR[255];
		if (NULL == TabItem.pszText) 
		{
			return;
		}

		int nIndex = m_tab.GetItemCount();
		TabItem.lParam=(LPARAM)&m_BarEditDlg;
		
		if (!(m_BarEditDlg.IsCreateDlg()))
		{
			VERIFY(m_BarEditDlg.Create(CBarEditDlg::IDD, &m_tab));
		}

		_tcscpy_s(TabItem.pszText, 255, str);
		m_tab.InsertItem(nIndex,&TabItem);
		m_BarEditDlg.SetWindowPos(NULL,4,25,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_BarEditDlg.ShowWindow(SW_HIDE);
		m_BarEditDlg.EnableWindow(FALSE);

		SetTable(nIndex);
		m_nTabIndex = nIndex;

		if (TabItem.pszText)
		{
			delete TabItem.pszText;
		}

		m_BarEditDlg.SetShow(TRUE);

		m_BarEditDlg.SetTextInfo(pNode, fill);
	}
	else
	{
		TC_ITEM tci;
		memset(&tci, 0 , sizeof(TC_ITEM));
		tci.mask=TCIF_PARAM;
		int nCount = m_tab.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			// 隐藏原有的标签
			m_tab.GetItem(i,&tci);
			ASSERT(tci.lParam);
			if(tci.lParam == ((LPARAM)&m_BarEditDlg))
			{
				m_tab.DeleteItem(i);
				m_tab.SetCurSel(1);
				SetTable(0);
				m_nTabIndex = 0;
				m_BarEditDlg.ShowWindow(SW_HIDE);
	            m_BarEditDlg.EnableWindow(FALSE);
				break;
			}
			
		}//end for (int i = 0; i < nCount; i++)

		m_BarEditDlg.SetShow(FALSE);
		m_BarEditDlg.SetTextInfo(pNode, fill);
	}

	return;
}