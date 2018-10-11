// TextPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextPageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextPageDlg property page
IMPLEMENT_DYNCREATE(CTextPageDlg, CPropertyPage)

CTextPageDlg::CTextPageDlg() : CPropertyPage(CTextPageDlg::IDD)
{
	//{{AFX_DATA_INIT(CTextPageDlg)
	m_bGroup = TRUE;
	//}}AFX_DATA_INIT
 	m_pCurTextSet = new CTextSet();
	m_bCreate = TRUE;
}

CTextPageDlg::~CTextPageDlg()
{
	m_pCurTextSet->Delete();
 	delete m_pCurTextSet;
}

void CTextPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextPageDlg)
	DDX_Control(pDX, IDC_LIST_DATA, m_ListData);
	DDX_Control(pDX, IDC_TAB_TEXT, m_tab);
	DDX_Check(pDX, IDC_CHECK_GROUP, m_bGroup);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTextPageDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CTextPageDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TEXT, OnSelchangeTabText)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_TEXT, OnSelchangingTabText)
	ON_BN_CLICKED(IDC_BUT_ADD, OnButAdd)
	ON_BN_CLICKED(IDC_BUT_DEL, OnButDel)
	ON_BN_CLICKED(IDC_BUT_MODIFY, OnButModify)
	ON_LBN_SELCHANGE(IDC_LIST_DATA, OnSelchangeListData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextPageDlg message handlers

BOOL CTextPageDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString str;
	//生成标签
	TC_ITEM TabItem;
	TabItem.mask=TCIF_PARAM|TCIF_TEXT;
	TabItem.pszText=new TCHAR[255];
	
	TabItem.lParam=(LPARAM)&m_PageText;
	VERIFY(m_PageText.Create(CTextPageTextDlg::IDD,&m_tab));
	str.LoadString(IDS_TEXTTITLE);
	_tcscpy(TabItem.pszText,str);
	m_tab.InsertItem(0,&TabItem);
	m_PageText.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PageText.ShowWindow(SW_SHOW);
	m_PageText.EnableWindow(TRUE);
	
	TabItem.lParam=(LPARAM)&m_PageFnt;
	VERIFY(m_PageFnt.Create(CTextPageFntDlg::IDD,&m_tab));
	str.LoadString(IDS_FNTTITLE);
	_tcscpy(TabItem.pszText,str);
	m_tab.InsertItem(1,&TabItem);
	m_PageFnt.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PageFnt.ShowWindow(SW_HIDE);
	m_PageFnt.EnableWindow(FALSE);
		
	TabItem.lParam=(LPARAM)&m_PageRound;
	VERIFY(m_PageRound.Create(CTextPageRoundDlg::IDD,&m_tab));
	str.LoadString(IDS_ROUNDTITLE);
	_tcscpy(TabItem.pszText,str);
	m_tab.InsertItem(2,&TabItem);
	m_PageRound.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_PageRound.ShowWindow(SW_HIDE);
	m_PageRound.EnableWindow(FALSE);

	delete TabItem.pszText;

 	UpdateDataView(FALSE);

	if(!m_bCreate)
		GetDlgItem(IDC_CHECK_GROUP)->ShowWindow(SW_HIDE);
	else
		GetDlgItem(IDC_CHECK_GROUP)->ShowWindow(SW_SHOW);

	ListFlash();
	if(m_ListData.GetCount()>0)
	{
		m_ListData.SetCurSel(0);
		CTextSet* pSet = m_pTextObject->GetItem(0);
		*m_pCurTextSet = *pSet;
		UpdateDataView(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextPageDlg::OnSelchangeTabText(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CTextPageDlg::OnSelchangingTabText(NMHDR* pNMHDR, LRESULT* pResult) 
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

void CTextPageDlg::OnButAdd()
{
	UpdateDataView(TRUE);

	m_pCurTextSet->m_pText->m_nTextID = 1;
	CTextSet* pTextSet = new CTextSet();
	*pTextSet = *m_pCurTextSet;
	m_pTextObject->AddText(pTextSet);
	ListFlash();
	int nListCount = m_ListData.GetCount();
	if (nListCount>0)
	{
		m_ListData.SetCurSel(nListCount-1);
		OnSelchangeListData();
	}
}

void CTextPageDlg::ListFlash()
{
	CString str;
	int nCount = m_ListData.GetCount();
	while(nCount--)
	{
		m_ListData.DeleteString(nCount);
	}

	CTextSet* pText;
	POSITION pos=m_pTextObject->m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pText = m_pTextObject->m_list.GetNext(pos);
		str = pText->m_pText->GetInfo();
		m_ListData.AddString(str);
	}
}

void CTextPageDlg::OnButDel()
{
	UINT nIndex = m_ListData.GetCurSel();
	m_pTextObject->DeleteItem(nIndex);
	ListFlash();
	if (m_ListData.GetCount()>0)
	{
		if (nIndex>=1)
		{
			m_ListData.SetCurSel(nIndex-1);
		}
		else
		{
			m_ListData.SetCurSel(0);
		}
		OnSelchangeListData();
	}
}

void CTextPageDlg::OnButModify()
{
	Modify();
}

void CTextPageDlg::UpdateDataView(BOOL bFlag)
{
	if(bFlag){
		//文本设置部分
		m_PageText.UpdateData(TRUE);
		m_pCurTextSet->m_pText->m_nText			= m_PageText.m_nText;
		m_pCurTextSet->m_pText->m_strFix		= m_PageText.m_strFix;
		m_pCurTextSet->m_pText->m_strDate		= m_PageText.m_strDate;
		m_pCurTextSet->m_pText->m_nJumpStart	= m_PageText.m_nJumpStart;
		m_pCurTextSet->m_pText->m_nJumpEnd		= m_PageText.m_nJumpEnd;
		m_pCurTextSet->m_pText->m_strLeft		= m_PageText.m_strLeft;
		m_pCurTextSet->m_pText->m_strRight		= m_PageText.m_strRight;
		m_pCurTextSet->m_pText->m_nJumpLen		= m_PageText.m_nJumpLen;
		m_pCurTextSet->m_pText->m_strPrefix		= m_PageText.m_strPrefix;
		m_pCurTextSet->m_pText->m_nJumpStep		= m_PageText.m_nJumpStep;
		m_pCurTextSet->m_pText->m_nJumpDis		= m_PageText.m_nJumpInterval;
		m_pCurTextSet->m_pText->m_bFill			= m_PageText.m_bFill;
		m_pCurTextSet->m_pText->m_nFromFile		= m_PageText.m_nFromFile;
		m_pCurTextSet->m_pText->m_strFromFile	= m_PageText.m_strFromFile;
		m_pCurTextSet->m_pText->m_strBinText	= m_PageText.m_strBinText;

		//字体设置部分
		m_PageFnt.UpdateData(TRUE);
 		m_pCurTextSet->m_pNodeText->m_nFont			= m_PageFnt.m_nFont;
		m_pCurTextSet->m_pNodeText->m_dbCharSpace	= m_PageFnt.m_dbCharSpace;
		m_pCurTextSet->m_pNodeText->m_dbWidthFactor = m_PageFnt.m_dbWidthFactor/100;
		m_pCurTextSet->m_pNodeText->m_dbHeight		= m_PageFnt.m_dbHeight;
		m_pCurTextSet->m_pNodeText->m_dbLineSpace	= m_PageFnt.m_dbLineSpace;
		m_pCurTextSet->m_pNodeText->m_strFontCHS	= m_PageFnt.m_strFontCHS;		
		m_pCurTextSet->m_pNodeText->m_strFontEN		= m_PageFnt.m_strFontEN;	
		m_pCurTextSet->m_pNodeText->m_strFontNUM	= m_PageFnt.m_strFontNUM;
		m_pCurTextSet->m_pNodeText->m_lf			= m_PageFnt.m_lf;
		m_pCurTextSet->m_pNodeText->m_bCustomFormat	= m_PageFnt.m_bCustomFormat;
		m_pCurTextSet->m_pNodeText->m_strFormat		= m_PageFnt.m_strFormat;
        m_pCurTextSet->m_pNodeText->m_strPrecisK	= m_PageFnt.m_strPrecisK;
		//排布设置部分
		m_PageRound.UpdateData(TRUE);
		m_pCurTextSet->m_pNodeText->m_nPath			= m_PageRound.m_nPath		;
		m_pCurTextSet->m_pNodeText->m_dbRadius		= m_PageRound.m_dbRadius	;
		m_pCurTextSet->m_pNodeText->m_dbStart		= m_PageRound.m_dbStart		;
		m_pCurTextSet->m_pNodeText->m_bCCW			= m_PageRound.m_bCCW		;
		m_pCurTextSet->m_pNodeText->m_bInside		= m_PageRound.m_bInside		;
		m_pCurTextSet->m_pNodeText->m_nAlign		= m_PageRound.m_nAlign		;
		m_pCurTextSet->m_pNodeText->m_dbDirection	= m_PageRound.m_dbDirection	;
		m_pCurTextSet->m_pNodeText->m_bFixSpace		= m_PageRound.m_bFixSpace;

		m_pCurTextSet->m_pText->m_bModel		= m_PageText.m_bModel;
		m_pCurTextSet->m_pText->m_strModelNo	= m_PageText.m_strModelNo;
	}
	else{
		//文本设置部分
		m_PageText.m_nText	 = m_pCurTextSet->m_pText->m_nText	;
		m_PageText.m_strFix	 = m_pCurTextSet->m_pText->m_strFix	;
		m_PageText.m_strDate = m_pCurTextSet->m_pText->m_strDate ;

		m_PageText.m_nText		= m_pCurTextSet->m_pText->m_nText				;
		m_PageText.m_strFix		= m_pCurTextSet->m_pText->m_strFix			;
		m_PageText.m_strDate	= m_pCurTextSet->m_pText->m_strDate				;
		m_PageText.m_nJumpStart	= m_pCurTextSet->m_pText->m_nJumpStart		;
		m_PageText.m_nJumpEnd	= m_pCurTextSet->m_pText->m_nJumpEnd				;
		m_PageText.m_strLeft	= m_pCurTextSet->m_pText->m_strLeft				;
		m_PageText.m_strRight	= m_pCurTextSet->m_pText->m_strRight				;
		m_PageText.m_nJumpLen	= m_pCurTextSet->m_pText->m_nJumpLen				;
		m_PageText.m_strPrefix	= m_pCurTextSet->m_pText->m_strPrefix			;
		m_PageText.m_nJumpStep	= m_pCurTextSet->m_pText->m_nJumpStep			;
		m_PageText.m_nJumpInterval=m_pCurTextSet->m_pText->m_nJumpDis;
		m_PageText.m_bFill		= m_pCurTextSet->m_pText->m_bFill				;
		m_PageText.m_nFromFile	= m_pCurTextSet->m_pText->m_nFromFile			;
		m_PageText.m_strFromFile= m_pCurTextSet->m_pText->m_strFromFile			;
		m_PageText.m_pSerial	= &(m_pCurTextSet->m_pText->m_saSerial);
		m_PageText.m_pCur		= &(m_pCurTextSet->m_pText->m_nMarkTimes);
		m_PageText.m_strBinText	= m_pCurTextSet->m_pText->m_strBinText			;

		m_PageText.m_bModel		= m_pCurTextSet->m_pText->m_bModel;
		m_PageText.m_strModelNo	= m_pCurTextSet->m_pText->m_strModelNo;

		
		m_PageText.UpdateData(FALSE);
		m_PageText.UpdateDataView();
		//字体设置部分
		m_PageFnt.m_nFont			= m_pCurTextSet->m_pNodeText->m_nFont				;
		m_PageFnt.m_dbCharSpace		= m_pCurTextSet->m_pNodeText->m_dbCharSpace		;
		m_PageFnt.m_dbWidthFactor	= m_pCurTextSet->m_pNodeText->m_dbWidthFactor*100 	;
		m_PageFnt.m_dbHeight		= m_pCurTextSet->m_pNodeText->m_dbHeight			;
		m_PageFnt.m_dbLineSpace		= m_pCurTextSet->m_pNodeText->m_dbLineSpace		;
		m_PageFnt.m_strFontCHS		= m_pCurTextSet->m_pNodeText->m_strFontCHS		;		
		m_PageFnt.m_strFontEN		= m_pCurTextSet->m_pNodeText->m_strFontEN			;	
		m_PageFnt.m_strFontNUM		= m_pCurTextSet->m_pNodeText->m_strFontNUM		;
		m_PageFnt.m_lf				= m_pCurTextSet->m_pNodeText->m_lf				;
		m_PageFnt.m_bCustomFormat	= m_pCurTextSet->m_pNodeText->m_bCustomFormat		;
		m_PageFnt.m_strFormat		= m_pCurTextSet->m_pNodeText->m_strFormat			;
        m_PageFnt.m_strPrecisK		= m_pCurTextSet->m_pNodeText->m_strPrecisK			;
		m_PageFnt.UpdateData(FALSE);
		m_PageFnt.UpdateDataView();
		//排布设置部分
		m_PageRound.m_nPath			= m_pCurTextSet->m_pNodeText->m_nPath		;
		m_PageRound.m_dbRadius		= m_pCurTextSet->m_pNodeText->m_dbRadius	;
		m_PageRound.m_dbStart		= m_pCurTextSet->m_pNodeText->m_dbStart		;
		m_PageRound.m_bCCW			= m_pCurTextSet->m_pNodeText->m_bCCW		;
		m_PageRound.m_bInside		= m_pCurTextSet->m_pNodeText->m_bInside		;
		m_PageRound.m_nAlign		= m_pCurTextSet->m_pNodeText->m_nAlign		;
		m_PageRound.m_dbDirection	= m_pCurTextSet->m_pNodeText->m_dbDirection	;
		m_PageRound.m_bFixSpace		= m_pCurTextSet->m_pNodeText->m_bFixSpace;
		m_PageRound.UpdateData(FALSE);
		m_PageRound.UpdateDataView();
	}
}

void CTextPageDlg::OnSelchangeListData() 
{
	UpdateData();
	int nCount = m_ListData.GetCurSel();
	CTextSet* pSet = m_pTextObject->GetItem(nCount);
	*m_pCurTextSet = *pSet;

	UpdateDataView(FALSE);
}

void CTextPageDlg::Modify()
{
	UpdateDataView(TRUE);
	
	int nCount = m_ListData.GetCurSel();
	if(nCount>=0){
		CTextSet* pSet = m_pTextObject->GetItem(nCount);
		*pSet = *m_pCurTextSet;
		ListFlash();
		m_ListData.SetCurSel(nCount);
		OnSelchangeListData();
	}	
}
