// TextBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextBaseDlg dialog
CTextBaseDlg::CTextBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextBaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextBaseDlg)
	//}}AFX_DATA_INIT
	m_bGroup = TRUE;
	m_bCreate = FALSE;
	m_bText = TRUE;
}

void CTextBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextBaseDlg)
	DDX_Control(pDX, IDC_TAB_BASE, m_ctrlTabBase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextBaseDlg, CDialog)
	//{{AFX_MSG_MAP(CTextBaseDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BASE, OnSelchangeTabBase)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_BASE, OnSelchangingTabBase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextBaseDlg message handlers

BOOL CTextBaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString str;
	
	Update_Data(FALSE);
	m_TextPage.m_pTextObject = &m_TextObject;
	
	//生成标签
	TC_ITEM TabItem;
	TabItem.mask=TCIF_PARAM|TCIF_TEXT;
	TabItem.pszText=new TCHAR[255];
	if(!m_bCreate){
		m_TextPage.m_bGroup = TRUE;
		m_TextPage.m_bCreate = FALSE;
	}
	
	TabItem.lParam=(LPARAM)&m_TextPage;
	VERIFY(m_TextPage.Create(CTextPageDlg::IDD,&m_ctrlTabBase));
	str.LoadString(IDS_TEXTTITLE);
	_tcscpy(TabItem.pszText,str);
	m_ctrlTabBase.InsertItem(0,&TabItem);
	m_TextPage.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_TextPage.ShowWindow(SW_SHOW);
	m_TextPage.EnableWindow(TRUE);
	m_TextPage.UpdateData(FALSE);
	
	if(!m_bText)
	{
		TabItem.lParam=(LPARAM)&m_BarPage;
		VERIFY(m_BarPage.Create(CTextBarCodePageDlg::IDD,&m_ctrlTabBase));
		str.LoadString(IDS_BARTITLE);
		_tcscpy(TabItem.pszText,str);
		m_ctrlTabBase.InsertItem(1,&TabItem);
		m_BarPage.SetWindowPos(NULL,2,20,0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_BarPage.ShowWindow(SW_HIDE);
		m_BarPage.EnableWindow(FALSE);
		m_BarPage.UpdateData(FALSE);
	}
	
	delete TabItem.pszText;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextBaseDlg::OnSelchangeTabBase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iTab=m_ctrlTabBase.GetCurSel();
	TC_ITEM tci;
	
	tci.mask=TCIF_PARAM;
	m_ctrlTabBase.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_SHOW);
	pWnd->EnableWindow(TRUE);
	
	*pResult = 0;
}

void CTextBaseDlg::OnSelchangingTabBase(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iTab=m_ctrlTabBase.GetCurSel();
	TC_ITEM tci;
	
	tci.mask=TCIF_PARAM;
	m_ctrlTabBase.GetItem(iTab,&tci);
	ASSERT(tci.lParam);
	
	CWnd *pWnd=(CWnd *)tci.lParam;
	pWnd->ShowWindow(SW_HIDE);
	pWnd->EnableWindow(FALSE);
	
	*pResult = 0;
}

void CTextBaseDlg::OnCancel() 
{
	m_TextObject.Delete();
	CDialog::OnCancel();
}

void CTextBaseDlg::OnOK()
{
	m_TextPage.Modify();
	Update_Data(TRUE);
	CDialog::OnOK();
}

void CTextBaseDlg::Update_Data(BOOL bFlag)
{
	if(bFlag)
	{
		m_TextPage.UpdateData();
		if(!m_bText)
			m_BarPage.UpdateData();
		m_bGroup = m_TextPage.m_bGroup;
		//数据更新到指定实际存储的数据中去
		if(m_bCreate)
		{
			//创建新的文本或条码内容时才会用到此处代码
			CTextSet* pTextSet;
			POSITION pos;
			CText* pText;
			CNodeText* pNodeText;

			if(m_bGroup)
			{
				pos = m_TextObject.m_list.GetHeadPosition();
				CUIntArray arrayTmp;
				while(pos != NULL)
				{
					pTextSet = m_TextObject.m_list.GetNext(pos);
					UINT nTextID = g_MgrText->GetNextID();
					arrayTmp.Add(nTextID);
					pTextSet->m_pText->m_nTextID = nTextID;
					pText = new CText(1);
					*pText = *(pTextSet->m_pText);
					m_Textlist.AddTail(pText);
					g_MgrText->AddText(pText);
				}
				pos = m_TextObject.m_list.GetHeadPosition();
				BOOL bFirst=TRUE;
				while(pos != NULL)
				{
					pTextSet = m_TextObject.m_list.GetNext(pos);
					if(bFirst)
					{
						bFirst = FALSE;
						int nCount = arrayTmp.GetSize();
						for(int i=0;i<nCount;i++)
						{
							pTextSet->m_pNodeText->m_ArrayTextID.Add(arrayTmp.GetAt(i));
						}
						pNodeText = (CNodeText*)(pTextSet->m_pNodeText->Copy(0x01));
						pNodeText->CreateString(FALSE);
						m_Nodelist.AddTail(pNodeText);
						break;
					}
				}
			}
			else{
				pos = m_TextObject.m_list.GetHeadPosition();
				CDot dotBase(0,0);
				CQuad quadLmt;

				while(pos != NULL)
				{
					pTextSet = m_TextObject.m_list.GetNext(pos);
					UINT nTextID = g_MgrText->GetNextID();
					pTextSet->m_pNodeText->m_ArrayTextID.Add(nTextID);
					pTextSet->m_pText->m_nTextID = nTextID;
					pText = new CText(1);
					*pText = *(pTextSet->m_pText);
					m_Textlist.AddTail(pText);
					if (g_MgrText != NULL)
					{
						g_MgrText->AddText(pText);
					} 
					
					pNodeText = (CNodeText*)(pTextSet->m_pNodeText->Copy(0x01));
 					pNodeText->m_dotBase = dotBase;
					pNodeText->CreateString(FALSE);
 					quadLmt = pNodeText->GetLimit();
 					dotBase = quadLmt.GetBaseDot(BASE_TR);
 					dotBase.LogToTrue();
 					dotBase.x+=0.6;
					m_Nodelist.AddTail(pNodeText);
				}
			}
		}
		else{
			//只有在重新设置时才会用到此处代码
			//由于在重新设置时，有几个限定条件
			//不能修改组合文本，只能修改相关文档的属性
			UINT nTextID;
			CUIntArray arrayTmp;
			CText* pText;
			CNodeText* pNodeText    = NULL;
			CNodeText* pNodeTextBak = NULL;
			CTextSet* pTextSet      = NULL;
			POSITION pos;
			BOOL bFirst=TRUE;
			//读出所有的TEXTID并写入数组中
			pos = m_TextObject.m_list.GetHeadPosition();
			while(pos != NULL)
			{
				pTextSet = m_TextObject.m_list.GetNext(pos);
				nTextID = pTextSet->m_pText->m_nTextID;
				if(g_MgrText->IsExist(nTextID))
				{
					//如果ID存在则不加入只是设置相关属性
					if (g_MgrText != NULL)
					{
						pText = g_MgrText->GetAt(nTextID);
					    *pText = *(pTextSet->m_pText);
					}
					
				}
				else{
					//如果ID不存在则加入
					if (g_MgrText != NULL)
					{
						nTextID = g_MgrText->GetNextID();
					}
					
					pTextSet->m_pText->m_nTextID = nTextID;
					pText = new CText(1);
					*pText = *(pTextSet->m_pText);
					if (g_MgrText != NULL)
					{
						g_MgrText->AddText(pText);
					}
					m_Textlist.AddTail(pText);
				}
				arrayTmp.Add(nTextID);
				if(bFirst)
				{
					bFirst=FALSE;
					pNodeTextBak = pTextSet->m_pNodeText;
				}
			}
			//将NODETEXT内容COPY到第一个节点中，
			//因为在这种情况下只可能会有一个NODETEXT
			bFirst = TRUE;
			pos = m_Nodelist.GetHeadPosition();
			while(pos!=NULL)
			{
				if(bFirst)
				{
					bFirst = FALSE;
					//这种情况下只有一个NODETEXT
					pNodeText = m_Nodelist.GetNext(pos);
					if (NULL == pNodeTextBak)
					{
						break;
					}

					pNodeText->CopyToMe(pNodeTextBak);
					int nCount = arrayTmp.GetSize();
					pNodeText->m_ArrayTextID.RemoveAll();
					
					for(int i=0;i<nCount;i++)
					{
						pNodeText->m_ArrayTextID.Add(arrayTmp.GetAt(i));
					}
					pNodeText->CreateString(FALSE);
					break;
				}
			}
		}
		m_TextObject.Delete();
	}
	else{
		//根据实际存储的数据组成现有的设置数据
		if(!m_bCreate)
		{
			//只有在重新设置时才会用到此处代码
			POSITION pos;
			CText* pText;
			CTextSet* pTextSet;
			CNodeText* pNodeText;
			CNodeText* pNodeTextBak;

			pos = m_Nodelist.GetHeadPosition();
			while(pos!=NULL)
			{
				pNodeText = m_Nodelist.GetNext(pos);
				int nCount = pNodeText->m_ArrayTextID.GetSize();
				UINT nTextID;

				for(int i=0;i<nCount;i++)
				{
					nTextID = pNodeText->m_ArrayTextID.GetAt(i);
					if (g_MgrText != NULL)
					{
						pText = g_MgrText->GetAt(nTextID);
					}
					pNodeTextBak = (CNodeText*)(pNodeText->Copy(0x01));
					//指定到指定的ID
					pNodeTextBak->m_ArrayTextID.RemoveAll();
					pNodeTextBak->m_ArrayTextID.Add(nTextID);

					pTextSet = new CTextSet();
					pTextSet->DeleteNodeText();
					pTextSet->m_pNodeText = pNodeTextBak;
					*(pTextSet->m_pText) = *pText;

					m_TextObject.AddText(pTextSet);
					//本操作用于确定反向操作时是否需要重新设置ID
					m_Textlist.AddTail(pText);
				}
			}
			m_TextPage.m_bGroup = TRUE;
		}
	}
}
