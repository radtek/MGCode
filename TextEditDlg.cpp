// TextEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextEditDlg.h"
#include "TextSerialDlg.h"
#include "TextServiceDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextEditDlg dialog


CTextEditDlg::CTextEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	m_strDate = _T("");
	m_strFix  = _T("");
	m_nText   = 0;
	m_pNodeText   = NULL;
	m_pCurSelText = NULL;
	m_bIsShow     = FALSE;
	//}}AFX_DATA_INIT
}


void CTextEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextEditDlg)
	DDX_Control(pDX, IDC_LIST_EDITDATA, m_ListData);
	DDX_Text(pDX, IDC_DATE_EDIT, m_strDate);
	DDX_Text(pDX, IDC_FIX_EDIT, m_strFix);
	DDX_Radio(pDX, IDC_RADIO_FIX, m_nText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextEditDlg, CDialog)
	//{{AFX_MSG_MAP(CTextEditDlg)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL, OnButtonSerial)
	ON_BN_CLICKED(IDC_RADIO_FIX, OnRadioFix)
	ON_BN_CLICKED(IDC_RADIO_SERIAL, OnRadioSerial)
	ON_BN_CLICKED(IDC_RADIO_TIME, OnRadioTime)
    ON_WM_TIMER()
	ON_EN_CHANGE(IDC_DATE_EDIT, OnChangeDateEdit)
	ON_BN_CLICKED(IDC_BUTTON_TEXTADD, OnButtonTextadd)
	ON_BN_CLICKED(IDC_BUTTON_TEXTDEL, OnButtonTextdel)
	ON_BN_CLICKED(IDC_BUTTON_TEXTEDIT, OnButtonTextedit)
	ON_LBN_SELCHANGE(IDC_LIST_EDITDATA, OnSelchangeListEditdata)
	ON_WM_DESTROY()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BUTTON_ADVANCE, OnButtonAdvance)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTextEditDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextEditDlg message handlers

BOOL CTextEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateDataView();

	m_pCurSelText = new CText(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTextEditDlg::OnButtonSerial() 
{
	CTextSerialDlg dlg;
	dlg.m_nJumpStart	= m_pCurSelText->m_nJumpStart;
	dlg.m_nJumpEnd  	= m_pCurSelText->m_nJumpEnd  ;
	dlg.m_strLeft	    = m_pCurSelText->m_strLeft	  ;
	dlg.m_strRight	    = m_pCurSelText->m_strRight  ;
	dlg.m_nJumpLen	    = m_pCurSelText->m_nJumpLen  ;
	dlg.m_strPrefix		= m_pCurSelText->m_strPrefix ;
	dlg.m_nJumpStep	    = m_pCurSelText->m_nJumpStep ;
	dlg.m_nJumpInterval = m_pCurSelText->m_nJumpDis  ;
	dlg.m_bFill		    = m_pCurSelText->m_bFill     ;
	dlg.m_nFromFile	    = m_pCurSelText->m_nFromFile ;
	dlg.m_strFromFile   = m_pCurSelText->m_strFromFile;
	dlg.m_pSerial	    = &(m_pCurSelText->m_saSerial);
	dlg.m_pCur		    = &(m_pCurSelText->m_nMarkTimes);
	dlg.m_strBinText	= m_pCurSelText->m_strBinText;
	
	if(dlg.DoModal()==IDOK)
	{
		m_pCurSelText->m_nJumpStart	= dlg.m_nJumpStart;
		m_pCurSelText->m_nJumpEnd		= dlg.m_nJumpEnd;
		m_pCurSelText->m_strLeft		= dlg.m_strLeft;
		m_pCurSelText->m_strRight		= dlg.m_strRight;
		m_pCurSelText->m_strPrefix		= dlg.m_strPrefix;
		m_pCurSelText->m_nJumpLen		= dlg.m_nJumpLen;
		m_pCurSelText->m_nJumpStep		= dlg.m_nJumpStep;
		m_pCurSelText->m_bFill			= dlg.m_bFill;
		m_pCurSelText->m_nFromFile		= dlg.m_nFromFile;
		m_pCurSelText->m_strFromFile	= dlg.m_strFromFile;
		m_pCurSelText->m_strBinText	= dlg.m_strBinText;
		m_pCurSelText->m_nJumpDis      = dlg.m_nJumpInterval;
		m_pCurSelText->m_nMarkTimes    = *(dlg.m_pCur);
		
		CString strSerial;
		strSerial = GetSerialString();
		SetDlgItemText(IDC_SERIAL_EDIT, strSerial);
	}
	
}


//用于配合RADIO刷新界面的
void CTextEditDlg::UpdateDataView()
{
	BOOL bFlag;
	switch(m_nText) {
	case 0:
		//固定文本
		bFlag = (m_nText == 0);
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(bFlag);
		GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(!bFlag);
		GetDlgItem(IDC_DATE_EDIT)->EnableWindow(!bFlag);
		break;
	case 1:
		//跳号部分
		bFlag = (m_nText == 1);
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(!bFlag);
		GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(bFlag);
		GetDlgItem(IDC_DATE_EDIT)->EnableWindow(!bFlag);
		break;
	case 2:
		//时间日期
		bFlag = (m_nText == 2);
		GetDlgItem(IDC_FIX_EDIT)->EnableWindow(!bFlag);
		GetDlgItem(IDC_BUTTON_SERIAL)->EnableWindow(!bFlag);
		GetDlgItem(IDC_DATE_EDIT)->EnableWindow(bFlag);
		break;
	}
}

void CTextEditDlg::OnRadioFix() 
{
	SetDlgItemText(IDC_TIME_INFO,"");
	KillTimer(1);
	UpdateData();
	UpdateDataView();
}

void CTextEditDlg::OnRadioSerial() 
{
	SetDlgItemText(IDC_TIME_INFO,"");
	KillTimer(1);
	UpdateData();
	UpdateDataView();
}

void CTextEditDlg::OnRadioTime() 
{
	UpdateData();
	SetTimer(1,1000,NULL);
	UpdateDataView();
}

CString CTextEditDlg::GetSerialString()
{
	if (NULL == m_pCurSelText)
	{
		return "";
	}
	
	CString str="";
	int i = m_pCurSelText->m_nMarkTimes;
	if(m_nText==TEXT_SERIAL)
	{
		if(m_pCurSelText->m_nFromFile)
		{
			//取于文件
			str=m_pCurSelText->m_saSerial.GetAt(i);
		}
		else
		{
			//自定义跳号
			int nItems=(m_pCurSelText->m_nJumpEnd - m_pCurSelText->m_nJumpStart)/m_pCurSelText->m_nJumpStep+1;
			if(i<nItems)
			{
				int cur=m_pCurSelText->m_nJumpStart+m_pCurSelText->m_nJumpStep*i;
				str.Format("%d",cur);
				
				if(m_pCurSelText->m_bFill)
				{	
					int n;
					
					n=m_pCurSelText->m_nJumpLen-str.GetLength();
					for(int j=0;j<n;j++)
						str=m_pCurSelText->m_strPrefix+str;
				}
				
				str=m_pCurSelText->m_strLeft+str+m_pCurSelText->m_strRight;
			}
		}
	}
	return str;
}

void CTextEditDlg::OnTimer(UINT nIDEvent) 
{
	if (NULL == m_pCurSelText) 
	{
		return;
	}

	CTime t;
	t=CTime::GetCurrentTime();
	CString strText=t.Format(m_pCurSelText->m_strDate);
	SetDlgItemText(IDC_TIME_INFO,strText);

	return;
}

void CTextEditDlg::OnChangeDateEdit() 
{
	GetDlgItemText(IDC_DATE_EDIT,m_strDate);
	
}

void CTextEditDlg::SetTextInfo(CNode *pNode, FILLDATA &fill) 
{
	if (!m_bIsShow)
	{
		return;
	}

	if (NULL == pNode) 
	{
		return;
	}

	if (NODE_TEXT_DSF != pNode->GetType())
	{
		return;
	}

	m_pNodeText = (CNodeText*)pNode;
	m_fill      = fill;

	m_strFix = "";

	int nCount = m_ListData.GetCount();
	while(nCount--)
	{
		m_ListData.DeleteString(nCount);
	}
	m_ListData.ResetContent();
	m_TextList.RemoveAll();

	if (NULL == m_pNodeText)
	{
		KillTimer(1);
		UpdateData(FALSE);
		return;
	} 

	CString str;
	nCount = m_pNodeText->m_ArrayTextID.GetSize();
	UINT nTextID;
	
	for(int i = 0; i < nCount; i++)
	{
		nTextID = m_pNodeText->m_ArrayTextID.GetAt(i);
		g_MgrText->FlashText(nTextID,0x00);
		g_MgrText->GetString(nTextID,str);
		CText *pText = g_MgrText->GetAt(nTextID);
		////
		if (NULL == pText)
		{
			continue;
		}
		
		if(pText->m_bBarCode)
		{
			GetDlgItem(IDC_BUTTON_TEXTDEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_TEXTADD)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_TEXTDEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_TEXTADD)->EnableWindow(TRUE);
		}
		////
		m_ListData.InsertString(i, str);

		TEXTINFO TextInfo;
		TextInfo.nTexID = nTextID;
		if (pText) 
		{
			TextInfo.nType = pText->m_nText;
		}

		m_TextList.Add(TextInfo);

		if (0 == i) 
		{
			*m_pCurSelText = *pText;
		}
	}

	if (m_pCurSelText != NULL)
	{
		m_ListData.SetCurSel(0);
		m_nText = m_pCurSelText->m_nText;
		if (0 == m_nText)
		{
			m_strFix = m_pCurSelText->m_strFix;
		}
		m_strDate = m_pCurSelText->m_strDate;
	}

	UpdateData(FALSE);

	UpdateDataView();

	return;
}

BOOL CTextEditDlg::FindTextInfo(int nIndex, TEXTINFO &TextInfo) 
{
	int nCount = m_TextList.GetSize();
	if (nIndex >= nCount)
	{
		return FALSE;
	}
	TextInfo = m_TextList[nIndex];

	return TRUE;
}

void CTextEditDlg::OnButtonTextadd() 
{
	CHLDoc*pDoc = GetDocument();
	if (NULL == pDoc) 
	{
		return;
	}

	if (!pDoc->IsHaveSelTxetNode()) 
	{
		return;
	}

	if (NULL == m_pNodeText)
	{
		return;
	}

	UpdateData(TRUE);

	if ((m_strFix.IsEmpty()))
	{
		return;
	}

	int nCount = m_pNodeText->m_ArrayTextID.GetSize();
	if (nCount <= 0)
	{
		return;
	}

	UINT nTextID = m_pNodeText->m_ArrayTextID.GetAt(0);
	CText* pTextOL = g_MgrText->GetAt(nTextID);
	if (NULL == pTextOL)
	{
		return;
	}

	CText* pText = new CText(1);
	if (NULL == pText) 
	{
		return;
	}
	*pText = *pTextOL;

	nTextID = g_MgrText->GetNextID();
	pText->m_nTextID = nTextID;
	pText->m_nText   = m_nText;
	switch(m_nText) 
	{
	case 0:
		{
			pText->m_strFix  = m_strFix;
		    break;
		}
	case 1:
		{
			pText->m_nJumpStart   = m_pCurSelText->m_nJumpStart       ;
			pText->m_nJumpEnd     = m_pCurSelText->m_nJumpEnd         ;
			pText->m_strLeft      = m_pCurSelText->m_strLeft	   ;
			pText->m_strRight     = m_pCurSelText->m_strRight	   ;
			pText->m_nJumpLen     = m_pCurSelText->m_nJumpLen	   ;
			pText->m_strPrefix    = m_pCurSelText->m_strPrefix	   ;
			pText->m_nJumpStep    = m_pCurSelText->m_nJumpStep	   ;
			pText->m_nJumpDis     = m_pCurSelText->m_nJumpDis    ;
			pText->m_bFill        = m_pCurSelText->m_bFill	           ;
			pText->m_nFromFile    = m_pCurSelText->m_nFromFile	   ;
			pText->m_strFromFile  = m_pCurSelText->m_strFromFile      ;

			int nCount = m_pCurSelText->m_saSerial.GetSize();
			CString strInfo;
			for(int i=0; i<nCount; i++)
			{
				strInfo = m_pCurSelText->m_saSerial.GetAt(i);
				pText->m_saSerial.Add(strInfo);
			}

			pText->m_nMarkTimes  = m_pCurSelText->m_nMarkTimes	           ;
			pText->m_strBinText   = m_pCurSelText->m_strBinText       ;
		    break;
		}
	default:
		break;
	}

	g_MgrText->AddText(pText);
	m_pNodeText->m_ArrayTextID.Add(nTextID);
	TEXTINFO textInfo;
	textInfo.nTexID = nTextID;
	textInfo.nType  = m_nText;
	m_TextList.Add(textInfo);

	m_pNodeText->CreateString(FALSE);

	ListFlash();
	int nListCount = m_ListData.GetCount();
	if (nListCount>0)
	{
		m_ListData.SetCurSel(nListCount-1);
		OnSelchangeListEditdata();
	}

	pDoc->UpdateSelText(m_fill);

	return;
}

void CTextEditDlg::OnButtonTextdel() 
{
	if (NULL == m_pNodeText)
	{
		return;
	}
	
	CHLDoc*pDoc = GetDocument();
	if (NULL == pDoc) 
	{
		return;
	}

	if (!pDoc->IsHaveSelTxetNode()) 
	{
		return;
	}

	int nCurSel = m_ListData.GetCurSel();
	if (-1 == nCurSel) 
	{
		return;
	}

	TEXTINFO TextInfo;
	BOOL bRet = FindTextInfo(nCurSel, TextInfo);
	if (!bRet)
	{
		return;
	}

	if (m_pNodeText != NULL)
	{
		int nCount = m_TextList.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			UINT nTextID = m_TextList[i].nTexID;
			if (nTextID == TextInfo.nTexID) 
			{
				m_TextList.RemoveAt(i);
				break;
			}
		}

		m_pNodeText->DelTextByID(TextInfo.nTexID);
		nCount = m_pNodeText->m_ArrayTextID.GetSize();
		if (0 < nCount) 
		{
			m_pNodeText->CreateString(FALSE);
		}
		
		pDoc->UpdateSelText(m_fill);
		nCount = m_TextList.GetSize();
		if (0 == nCount) 
		{
			pDoc->Delete();
		}
	}

	ListFlash();
	if (m_ListData.GetCount()>0)
	{
		if (nCurSel-1>=0)
		{
			m_ListData.SetCurSel(nCurSel-1);
		}
		else
		{
			m_ListData.SetCurSel(0);
		}
		OnSelchangeListEditdata();
	}

	return;
}

void CTextEditDlg::OnButtonTextedit() 
{
	if (NULL == m_pNodeText)
	{
		return;
	}

	CHLDoc*pDoc = GetDocument();
	if (NULL == pDoc) 
	{
		return;
	}

	if (!pDoc->IsHaveSelTxetNode()) 
	{
		return;
	}

	int nCurSel = m_ListData.GetCurSel();
	if (-1 == nCurSel) 
	{
		return;
	}

	TEXTINFO TextInfo;
	BOOL bRet = FindTextInfo(nCurSel, TextInfo);
	if (!bRet)
	{
		return;
	}

	CText* pText = g_MgrText->GetAt(TextInfo.nTexID);
	if (NULL == pText)
	{
		return;
	}

	UpdateData(TRUE);

	switch(m_nText) 
	{
	case 0:
		{
			pText->m_nText  = m_nText;
			pText->m_strFix = m_strFix;
			break;
		}
	case 1:
		{
			*pText = *m_pCurSelText;
			pText->m_nText  = m_nText;
			break;
		}
	case 2:
		{
			pText->m_nText   = m_nText;
			break;
		}
	default:
		{
			break;
		}
	}

	if (m_pNodeText != NULL)
	{
		m_pNodeText->CreateString(FALSE);
	}

	pDoc->UpdateSelText(m_fill);

	ListFlash();
	m_ListData.SetCurSel(nCurSel);
	OnSelchangeListEditdata();

	return;
	
}


void CTextEditDlg::ListFlash()
{
	CString str;
	int nCount = m_ListData.GetCount();
	while(nCount--)
	{
		m_ListData.DeleteString(nCount);
	}

	m_ListData.ResetContent();

	nCount = m_TextList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		UINT nTextID = m_TextList[i].nTexID;
		CText* pText = g_MgrText->GetAt(nTextID);
		if (NULL != pText)
		{
			m_ListData.InsertString(i, pText->m_strText);
		}
	}

	return;
}

CHLDoc *CTextEditDlg::GetDocument() 
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (NULL == pFWnd)
	{
		return NULL;
	}

	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{	
		return NULL;
	}

	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return NULL;
	}

	return pDoc;
}

void CTextEditDlg::OnSelchangeListEditdata() 
{
	int nCurSel = m_ListData.GetCurSel();
	int nCount = m_ListData.GetCount();
	if (nCurSel >= nCount)
	{
		return;
	}
	
	UINT nTextID = m_TextList[nCurSel].nTexID;
	CText* pText = g_MgrText->GetAt(nTextID);
	*m_pCurSelText = *pText;
	m_nText = m_TextList[nCurSel].nType;
	switch(m_nText) 
	{
	case 0:
		{
			m_strFix = pText->m_strText;
			break;
		}
	default:
		{
			break;
		}
	}
	
	UpdateData(FALSE);
	UpdateDataView();

	return;
}

BOOL CTextEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	/*
	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F10) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_MARK_NORMAL , NULL);
		return TRUE;
	}

	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F7) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FAST_MARK , NULL);
		return TRUE;
	}
	*/
	return CDialog::PreTranslateMessage(pMsg);
}

void CTextEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_pCurSelText)
	{
		delete m_pCurSelText;
		m_pCurSelText = NULL;
	}

	return;
}

void CTextEditDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
}

void CTextEditDlg::SetShow(BOOL bIsShow) 
{
	m_bIsShow = bIsShow;
}

void CTextEditDlg::OnNewDocment() 
{
	if (!m_bIsShow)
	{
		return;
	}

	m_pNodeText = NULL;
    KillTimer(1);
	
	int nCount = m_ListData.GetCount();
	while(nCount--)
	{
		m_ListData.DeleteString(nCount);
	}
	m_ListData.ResetContent();
	m_TextList.RemoveAll();

	m_strFix = "";
	
	UpdateData(FALSE);
}

BOOL CTextEditDlg::IsCreateDlg() 
{
	return (NULL != m_pCurSelText);
}

void CTextEditDlg::OnButtonAdvance() 
{
	if (NULL == m_pNodeText)
	{
		return;
	}

	CHLDoc*pDoc = GetDocument();
	if (NULL == pDoc) 
	{
		return;
	}

	pDoc->EditTextNode(m_pNodeText);

	return;
}

void CTextEditDlg::Apply()
{
	OnButtonTextedit();
}

void CTextEditDlg::OnBnClickedButton1()
{
	if (m_pNodeText == nullptr) return;

	TextServiceDialog text_service;
	text_service.selected_node=m_pNodeText;
	text_service.DoModal();
	if(text_service.Get_exit_window()==1){
		m_strFix=text_service.selected_node->m_strText;
		UpdateData(FALSE);
	}
}