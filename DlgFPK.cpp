// DlgFPK.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgFPK.h"
#include "Mainfrm.h"
#include "FpkSetglg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFPK dialog


CDlgFPK::CDlgFPK(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFPK::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFPK)
	m_bApplyAll = FALSE;
	m_nNum = 0;
	m_dbFpkStartPrec = 0.0;
	m_dbFpkEndPrec = 0.0;
	m_nFpkTime = 0;
	m_bIsLine = FALSE;
	//}}AFX_DATA_INIT
	m_pFPKFrame = NULL;
	m_pItem = NULL;
	m_lbSel = 0;
	m_bShowIsLine = TRUE;
}


void CDlgFPK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFPK)
	DDX_Check(pDX, IDC_CHECK_APPLY_ALL, m_bApplyAll);
	DDX_Text(pDX, IDC_EDIT_NUM, m_nNum);
	DDV_MinMaxUInt(pDX, m_nNum, 0, MAX_QSOFT_CNT);
	DDX_Text(pDX, IDC_EDIT_FPKSTARTLEVE, m_dbFpkStartPrec);
	DDX_Text(pDX, IDC_EDIT_FPKENDLEVE, m_dbFpkEndPrec);
	DDX_Text(pDX, IDC_EDIT_FPKTIME, m_nFpkTime);
	DDX_Check(pDX, IDC_CHECK_ISLINE, m_bIsLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFPK, CDialog)
	//{{AFX_MSG_MAP(CDlgFPK)
	ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
	ON_BN_CLICKED(IDC_BTN_MARK, OnBtnMark)
	ON_EN_CHANGE(IDC_EDIT_NUM, OnChangeEditNum)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_ISLINE, OnCheckIsline)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFPK message handlers

BOOL CDlgFPK::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(!m_bShowIsLine){
		GetDlgItem(IDC_CHECK_ISLINE)->ShowWindow(SW_HIDE);
		m_bIsLine = FALSE;
	}
	
	CreateFrame();

	InitData(NULL);

	CHLApp* pApp = (CHLApp *)AfxGetApp();
	if (pApp)
	{
		CString str("");
		switch(m_nFpkType) 
		{
		case 0:
			{
				str = pApp->GetProfileString("FPKSET","IsLineF", "0");
				m_bIsLine = (BOOL)atof(str);
				break;
			}
		case 1:
			{
				str = pApp->GetProfileString("FPKSET","IsLineT", "0");
				m_bIsLine = (BOOL)atof(str);
				break;
			}
		default:
			{
				break;
			}
		}
	}// end if (pApp)

	UpdateData(FALSE);
	UpdateView();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgFPK::CreateFrame()
{
	m_pFPKFrame = new CFPKFrame;
	if (NULL == m_pFPKFrame) 
	{
		return;
	}

	CRect rect;
	this->GetDlgItem(IDC_STATIC_PANEL)->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	m_pFPKFrame->CreateFrame(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, this, NULL, 0);
}

void CDlgFPK::OnBtnModify() 
{
	if(!UpdateData(TRUE)) return;
	int nCount = m_pFPKFrame->m_pFpkDoc->m_array.GetSize();
	while ((UINT)nCount>m_nNum)
	{
		m_pFPKFrame->m_pFpkDoc->m_array.RemoveAt(nCount-1);
		nCount = m_pFPKFrame->m_pFpkDoc->m_array.GetSize();
	}
	while ((UINT)nCount<m_nNum)
	{
		m_pFPKFrame->m_pFpkDoc->m_array.Add(0);
		nCount = m_pFPKFrame->m_pFpkDoc->m_array.GetSize();
	}
	m_pFPKFrame->m_pFpkDoc->UpdateAllViews(NULL);
}

CFPKDoc* CDlgFPK::GetDocument()
{
	return m_pFPKFrame->m_pFpkDoc;
}

BOOL CDlgFPK::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)
		{
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgFPK::InitData(CPropertyItem* pItem)
{
	CMainFrame *pMainWnd = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainWnd)
	{
		return;
	}
		
	int nCount = pMainWnd->m_PropDlg.m_listLayer.GetItemCount();
	CHLDoc *pDoc = pMainWnd->m_PropDlg.GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	int nCurSel = pMainWnd->m_PropDlg.m_listLayer.GetCurSel();
	if ((nCurSel < 0) || (nCurSel >= nCount))
	{
		return;
	}

	CLayer ly = pDoc->GetLayer(nCurSel);

	DispData(ly.m_para);

	return;
}

void CDlgFPK::SetPropertyItem(CPropertyItem* pItem, int lbSel, int nFpkType)
{
	m_pItem     = pItem;
	m_lbSel    = lbSel;
	m_nFpkType = nFpkType;
}

void CDlgFPK::OnOK() 
{
	UpdateData(TRUE);

	CFPKDoc* pFpkDoc = GetDocument();
	if (NULL == pFpkDoc) 
	{
		return;
	}

	if (m_nNum < 1)
	{
		return;
	}

	pFpkDoc->m_array.RemoveAll();
	
	double dbDistanc = m_dbFpkEndPrec - m_dbFpkStartPrec;
	int nStpC = (m_nNum - 1);
	if (nStpC < 0) 
	{
		nStpC = 1;
	}

	double nStep     = (dbDistanc / nStpC);

	for (UINT i = 0; i < m_nNum; i++)
	{
		if (i == (m_nNum - 1))
		{
			pFpkDoc->m_array.Add((float)m_dbFpkEndPrec);
		}
		else
		{
			pFpkDoc->m_array.Add((float)(m_dbFpkStartPrec + i * nStep));
		}
	}

	pFpkDoc->UpdateAllViews(NULL);

	
	return;
}

void CDlgFPK::OnApply(BOOL bApplyAll, int nQSwitchdelay, double dbFreq, double dbQRelease, double dbCurret, double dbSpeed)
{
	UpdateData(TRUE);

	CFPKDoc* pFpkDoc = GetDocument();
	if (NULL == pFpkDoc)
	{
		return;
	}

	CMainFrame *pMainWnd = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainWnd)
	{
		return;
	}
		
	int nCount = pMainWnd->m_PropDlg.m_listLayer.GetItemCount();
	CHLDoc *pDoc = pMainWnd->m_PropDlg.GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	int nCurSel = pMainWnd->m_PropDlg.m_listLayer.GetCurSel();
	if ((nCurSel < 0) || (nCurSel >= nCount))
	{
		return;
	}

	CLayer ly = pDoc->GetLayer(nCurSel);
	switch (m_nFpkType)
	{
	case 0:
		{
			if (0 == m_nNum) 
			{
				ly.m_para.bQSoftStart    = FALSE;
			}
			else
			{
				ly.m_para.bQSoftStart = TRUE;
			}
			ly.m_para.nQSoftStartCnt = m_nNum;
			for (UINT j = 0; j < m_nNum; j++)
			{
				ly.m_para.dbQSoftStartVal[j] = pFpkDoc->m_array.GetAt(j);
			}
			ly.m_para.nQSwitchDly = nQSwitchdelay;

//			ly.m_para.dbMarkSpeed   = dbSpeed;
			ly.m_para.dbQFreq   = dbFreq;
			ly.m_para.dbQRelease   = dbQRelease;
			ly.m_para.dbCurrent = dbCurret;
	
			ly.m_para.qSoftStartType = m_bIsLine?QA_LINE:QA_DOT;
			ly.m_para.nQLineTime = m_nFpkTime;
			ly.m_para.dbQLineVal = (float)m_dbFpkStartPrec;			break;
		}
	case 1:
		{
			if (0 == m_nNum)
			{
				ly.m_para.bQSoftEnd    = FALSE;
			}
			else
			{
				ly.m_para.bQSoftEnd = TRUE;
			}
			ly.m_para.nQSoftEndCnt = m_nNum;
			for (UINT j = 0; j < m_nNum; j++)
			{
				ly.m_para.dbQSoftEndVal[j] = pFpkDoc->m_array.GetAt(j);
			}
			ly.m_para.nQSoftEndDly = nQSwitchdelay;

//			ly.m_para.dbMarkSpeed   = dbSpeed;
			ly.m_para.dbQFreq   = dbFreq;
			ly.m_para.dbQRelease   = dbQRelease;
			ly.m_para.dbCurrent = dbCurret;
			break;
		}
	default:
		break;
	}

	pDoc->SetLayer(ly);

	if(bApplyAll)
	{
		for(int i = 0; i < nCount; i++)
		{
			CLayer lyC = pDoc->GetLayer(i);
			switch (m_nFpkType)
			{
			case 0:
				{
					if (0 == m_nNum) 
					{
						lyC.m_para.bQSoftStart    = FALSE;
					}
					else
					{
						lyC.m_para.bQSoftStart = TRUE;
					}
					lyC.m_para.nQSoftStartCnt = m_nNum;
					lyC.m_para.nQSwitchDly     = nQSwitchdelay;

//					lyC.m_para.dbMarkSpeed   = dbSpeed;
					lyC.m_para.dbQFreq   = dbFreq;
					lyC.m_para.dbQRelease   = dbQRelease;
			        lyC.m_para.dbCurrent = dbCurret;
					ly.m_para.qSoftStartType = m_bIsLine?QA_LINE:QA_DOT;
					ly.m_para.nQLineTime = m_nFpkTime;
					ly.m_para.dbQLineVal = (float)m_dbFpkStartPrec;
					memcpy(lyC.m_para.dbQSoftStartVal, ly.m_para.dbQSoftStartVal, MAX_QSOFT_CNT * sizeof(float));
					break;
				}
			case 1:
				{
					if (0 == m_nNum)
					{
						lyC.m_para.bQSoftEnd    = FALSE;
					}
					else
					{
						lyC.m_para.bQSoftEnd = TRUE;
					}
					lyC.m_para.nQSoftEndDly   = nQSwitchdelay;
					lyC.m_para.nQSoftEndCnt   = m_nNum;

//					lyC.m_para.dbMarkSpeed   = dbSpeed;
					lyC.m_para.dbQFreq   = dbFreq;
					lyC.m_para.dbQRelease   = dbQRelease;
			        lyC.m_para.dbCurrent = dbCurret;
					memcpy(lyC.m_para.dbQSoftEndVal, ly.m_para.dbQSoftEndVal, MAX_QSOFT_CNT * sizeof(float));
					break;
				}
			default:
				break;
			}
			pDoc->SetLayer(lyC);
		}// end for(int i=0; i<nCount; i++)
	}// end if(m_bApplyAll)
	else if (0 == nCurSel)
	{
		CLayer ly0 = pDoc->GetLayer(0);
		for(int i = 1; i < nCount; i++)
		{
			CLayer lyC = pDoc->GetLayer(i);
			if (lyC.m_bDefault)
			{
				lyC.m_para = ly0.m_para;
				pDoc->SetLayer(lyC);
			}
			
		}// end for(int i=0; i<nCount; i++)
	}
	
	return;
}

void CDlgFPK::GetCurPara(CLayer &ly, int nQSwitchdelay)
{
	UpdateData(TRUE);

	CFPKDoc* pFpkDoc = GetDocument();
	if (NULL == pFpkDoc)
	{
		return;
	}

	switch (m_nFpkType)
	{
	case 0:
		{
			if (0 == m_nNum) 
			{
				ly.m_para.bQSoftStart    = FALSE;
			}
			else
			{
				ly.m_para.bQSoftStart = TRUE;
			}
			ly.m_para.nQSoftStartCnt = m_nNum;
			for (UINT j = 0; j < m_nNum; j++)
			{
				ly.m_para.dbQSoftStartVal[j] = pFpkDoc->m_array.GetAt(j);
			}
			ly.m_para.nQSwitchDly = nQSwitchdelay;
			ly.m_para.qSoftStartType = m_bIsLine?QA_LINE:QA_DOT;
			ly.m_para.nQLineTime = m_nFpkTime;
			ly.m_para.dbQLineVal = (float)m_dbFpkStartPrec;
			break;
		}
	case 1:
		{
			if (0 == m_nNum)
			{
				ly.m_para.bQSoftEnd    = FALSE;
			}
			else
			{
				ly.m_para.bQSoftEnd = TRUE;
			}
			ly.m_para.nQSoftEndCnt = m_nNum;
			for (UINT j = 0; j < m_nNum; j++)
			{
				ly.m_para.dbQSoftEndVal[j] = pFpkDoc->m_array.GetAt(j);
			}
			ly.m_para.nQSoftEndDly = nQSwitchdelay;
			break;
		}
	default:
		break;
	}
	
	return;
}





void CDlgFPK::OnBtnMark() 
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
	CHLDoc *pDoc = (CHLDoc*)pMain->GetActiveDocument();
	//调用pDoc的Mark函数
}

void CDlgFPK::OnChangeEditNum() 
{
	
}

void CDlgFPK::OnSelectMod(int nMod) 
{
	if (0 == nMod) 
	{
		GetDlgItem(IDC_EDIT_NUM)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_APPLY_ALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MODIFY)->EnableWindow(FALSE);
        GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else if (1 == nMod) 
	{
		GetDlgItem(IDC_EDIT_NUM)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_APPLY_ALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_MODIFY)->EnableWindow(TRUE);
        GetDlgItem(IDOK)->EnableWindow(TRUE);
	}
	else
	{
	}

	if (m_pFPKFrame)
	{
		m_pFPKFrame->OnSelectMod(nMod);
	}

	return;
}

void CDlgFPK::OnDestroy() 
{
	CDialog::OnDestroy();
}


void CDlgFPK::DispData(PARA &para)
{
	CFPKDoc* pFpkDoc = GetDocument();
	if (NULL == pFpkDoc)
	{
		return;
	}

	pFpkDoc->m_array.RemoveAll();

	switch (m_nFpkType)
	{
	case 0:
		{
			m_nNum = para.nQSoftStartCnt;
			for (UINT j = 0; j < m_nNum; j++)
			{
				pFpkDoc->m_array.Add(para.dbQSoftStartVal[j]);
			}

			if(para.qSoftStartType==QA_DOT)
			{
				m_dbFpkStartPrec = para.dbQSoftStartVal[0];
				m_dbFpkEndPrec   = para.dbQSoftStartVal[m_nNum - 1];
				m_nFpkTime = para.nQLineTime;
				m_bIsLine = FALSE;
			}
			else{
				m_dbFpkStartPrec = para.dbQLineVal;
				m_dbFpkEndPrec   = 0;
				m_nFpkTime = para.nQLineTime;
				m_bIsLine = TRUE;
			}
			break;
		}
	case 1:
		{
			m_nNum = para.nQSoftEndCnt;
			for (UINT j = 0; j < m_nNum; j++)
			{
				pFpkDoc->m_array.Add(para.dbQSoftEndVal[j]);
			}

			m_dbFpkStartPrec = para.dbQSoftEndVal[0];
			m_dbFpkEndPrec   = para.dbQSoftEndVal[m_nNum - 1];
			break;
		}
	default:
		break;
	}

	//
	UpdateView();
	UpdateData(FALSE);
	GetDocument()->UpdateAllViews(NULL);
}

void CDlgFPK::OnCheckIsline() 
{
	UpdateData();
	UpdateView();

	CHLApp* pApp = (CHLApp *)AfxGetApp();
	if (pApp)
	{
		char szMsg[32];
	    sprintf_s(szMsg, 32, "%d", m_bIsLine);
		switch(m_nFpkType) 
		{
		case 0:
			{
				pApp->WriteProfileString("FPKSET","IsLineF", szMsg);
				break;
			}
		case 1:
			{
				pApp->WriteProfileString("FPKSET","IsLineT", szMsg);
				break;
			}
		default:
			{
				break;
			}
		}
	}
	
	return;
}

void CDlgFPK::UpdateView()
{
	GetDlgItem(IDC_BTN_MODIFY)->ShowWindow(m_bIsLine?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_EDIT_NUM)->ShowWindow(m_bIsLine?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_STATIC_PNUM)->ShowWindow(m_bIsLine?SW_HIDE:SW_SHOW);
	m_pFPKFrame->ShowWindow(m_bIsLine?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_EDIT_FPKTIME)->ShowWindow(!m_bIsLine?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_STATIC_FPKTIME)->ShowWindow(!m_bIsLine?SW_HIDE:SW_SHOW);
}
