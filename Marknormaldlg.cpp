// MarkNormalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "MarkNormalDlg.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkNormalDlg dialog

CMarkNormalDlg::CMarkNormalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkNormalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkNormalDlg)
	m_bUpdate = FALSE;
	m_nMarkType = 0;
	m_bLoopMark = FALSE;
	m_nIntervalTime = 0;
	//}}AFX_DATA_INIT
	m_pMarkMgr = NULL;
}

void CMarkNormalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkNormalDlg)
	DDX_Control(pDX, IDC_PROGRESS_MARK, m_progressMark);
	DDX_Check(pDX, IDC_CHECK_UPDATE, m_bUpdate);
	DDX_Radio(pDX, IDC_RADIO_LASER, m_nMarkType);
	DDX_Check(pDX, IDC_CHECK_LOOPMARK, m_bLoopMark);
	DDX_Text(pDX, IDC_EDIT_INTERVALTIME, m_nIntervalTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarkNormalDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkNormalDlg)
	ON_BN_CLICKED(IDC_BUTTON_MARK, OnButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_UPDATE, OnCheckUpdate)
	ON_BN_CLICKED(IDC_RADIO_LASER, OnRadioLaser)
	ON_BN_CLICKED(IDC_RADIO_RED, OnRadioRed)
	ON_BN_CLICKED(IDC_RADIO_FRAME, OnRadioFrame)
//	ON_BN_CLICKED(IDC_BUTTON_RedCorrSetting, OnBUTTONRedCorrSetting)
	ON_BN_CLICKED(IDC_CHECK_LOOPMARK, OnCheckLoopmark)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MARK_END,OnMarkEnd)
	ON_MESSAGE(WM_MARK_START,OnMarkStart)
	ON_MESSAGE(WM_MARK_STOP,OnMarkStop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkNormalDlg message handlers

void CMarkNormalDlg::OnButtonMark() 
{
	Mark() ;
}

void CMarkNormalDlg::OnButtonStop() 
{
	if(WaitForSingleObject(hMarkSts, 0) != WAIT_OBJECT_0)
	{
		return;
	}

	m_bLoopMark = FALSE;
	m_pMarkMgr->SetLoopMarkInfo(m_bLoopMark, m_nIntervalTime);
//	Sleep(100);
	m_pMarkMgr->StopMark();

	for(int i = 0; i < 3; i++)
	{
		if(WaitForSingleObject(hMarkSts,0) != WAIT_OBJECT_0)
		{
			break;
		}
		Sleep(20);
		DSF_TRACE("Wait Mark Stop ... \n");
	}

	m_bMark=m_bMarkExit=TRUE;
	m_bMarkStop=FALSE;

	UpdateView();
}

BOOL CMarkNormalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	DSF_TRACE("Open Normal Dlg ...");
	//控件浮动
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_MARK), IDC_BUTTON_MARK); 

	m_pMarkMgr = new CMarkMgr();
	m_pMarkMgr->RunThread((LPVOID)GetSafeHwnd());
	m_pMarkMgr->SetLay(m_pDoc->GetLay());
	DSF_TRACE("Set Lay OK ...");
	m_pMarkMgr->SetChain(m_pDoc);
	DSF_TRACE("Set Chain OK ...");

	m_progressMark.SetRange32(0,10);
	m_progressMark.SetStep(1);

	DSF_TRACE("Set Button Sts ...");
	m_bFirst=TRUE;
	m_bMark=m_bMarkExit=TRUE;
	m_bMarkStop=FALSE;
	UpdateView();
	
	//定义计时器
	if(!SetTimer(0,1000,NULL))
	{
		CString strError;
		strError.LoadString(IDS_ERROR_TIMER);
		MessageBox(strError);
	}
	m_timeStart=CTime::GetCurrentTime();
	m_tsWork=0;
	m_dwSingle=0;
	m_dwWork=0;
	m_nCount=0;
	m_nTotal=0;
	UpdateTime(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMarkNormalDlg::UpdateView()
{
	CString str;
	if(m_bMark){
		str.LoadString(IDS_MARK_ON);
		if(m_bFirst)
		{
			GetDlgItem(IDC_RADIO_LASER)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_RED)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_FRAME)->ShowWindow(TRUE);
		}
	}
	else{
		str.LoadString(IDS_MARK_PAUSE);
		if(m_bFirst)
		{
			GetDlgItem(IDC_RADIO_LASER)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_RED)->ShowWindow(FALSE);
			GetDlgItem(IDC_RADIO_FRAME)->ShowWindow(FALSE);
		}
	}
	SetDlgItemText(IDC_BUTTON_MARK,str);

	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bMarkStop);
	GetDlgItem(IDCANCEL)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_RADIO_LASER)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_RADIO_RED)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_RADIO_FRAME)->EnableWindow(m_bMarkExit);

// 	GetDlgItem(IDC_BUTTON_RedCorrSetting)->ShowWindow((m_nMarkType!=MARK_LASER));
// 	GetDlgItem(IDC_STATIC_RED)->ShowWindow((m_nMarkType!=MARK_LASER)&&m_bShowRedCorr);
// 	GetDlgItem(IDC_EDIT_REDSCALEX)->ShowWindow((m_nMarkType!=MARK_LASER)&&m_bShowRedCorr);
// 	GetDlgItem(IDC_EDIT_REDSCALEY)->ShowWindow((m_nMarkType!=MARK_LASER)&&m_bShowRedCorr);
// 	GetDlgItem(IDC_STATIC_REDX)->ShowWindow((m_nMarkType!=MARK_LASER)&&m_bShowRedCorr);
// 	GetDlgItem(IDC_STATIC_REDY)->ShowWindow((m_nMarkType!=MARK_LASER)&&m_bShowRedCorr);
}

LRESULT CMarkNormalDlg::OnMarkEnd(WPARAM wParam,LPARAM lParam)
{
	DSF_TRACE("OnMarkEnd ____ !!!");
	switch(wParam)
	{
	case MARK_TEND:
		MarkEnd();
		break;
	case MARK_SEND:
		MarkSEnd();
		break;
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}

	DSF_TRACE("OnMarkEnd ____  OUT !!!");
	return NULL;
}

void CMarkNormalDlg::OnMarkEndL(WPARAM wParam,LPARAM lParam)
{
	DSF_TRACE("OnMarkEnd ____ !!!");
	switch(wParam)
	{
	case MARK_TEND:
		MarkEnd();
		break;
	case MARK_SEND:
		MarkSEnd();
		break;
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}

	DSF_TRACE("OnMarkEnd ____  OUT !!!");
}

LRESULT CMarkNormalDlg::OnMarkStart(WPARAM wParam,LPARAM lParam)
{
	Mark() ;
	return NULL;
}

LRESULT CMarkNormalDlg::OnMarkStop(WPARAM wParam,LPARAM lParam)
{
	OnButtonStop() ;
	return NULL;
}

void CMarkNormalDlg::UpdateTime(BOOL flag)
{
	m_dwSingle=m_pMarkMgr->GetSingleMarkTime();

	m_timeEnd=CTime::GetCurrentTime();
	m_tsTotal=m_timeEnd-m_timeStart;
	if(flag)
	{
		m_dwWork+=m_dwSingle;
		m_tsWork=(m_dwWork/1000);
	}
	m_tsIdle=m_tsTotal-m_tsWork;
	if (m_tsTotal < m_tsWork)
	{
		m_tsIdle = 0;
	}
	
	SetDlgItemInt(IDC_STATIC_COUNT,m_nCount);
	SetDlgItemInt(IDC_STATIC_NUM,m_nTotal);
	SetDlgItemText(IDC_STATIC_TOTAL,m_tsTotal.Format("%D:%H:%M:%S"));
	SetDlgItemText(IDC_STATIC_WORK,m_tsWork.Format("%D:%H:%M:%S"));
	SetDlgItemText(IDC_STATIC_IDLE,m_tsIdle.Format("%D:%H:%M:%S"));

	//单个打标时间显示
	if(flag)
	{
		CString str;
		int day,hour,min,ms;
		double sec;
		
		ms=m_dwSingle%1000;
		m_dwSingle/=1000;

		sec=m_dwSingle%60;
		m_dwSingle/=60;
		sec+=(ms/1000.0);

		min=m_dwSingle%60;
		m_dwSingle/=60;

		hour=m_dwSingle%24;
		day=m_dwSingle/24;

		str.Format("%d:%2d:%2d:%6.3f",day,hour,min,sec);
		SetDlgItemText(IDC_STATIC_SINGLE,str);
	}
}

void CMarkNormalDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent==0)
	{
		UpdateTime();
	}
	CDialog::OnTimer(nIDEvent);
}

void CMarkNormalDlg::OnCancel() 
{
	if (NULL == m_pMarkMgr)
	{
		return;
	}

	m_pMarkMgr->SetLoopMarkInfo(FALSE, m_nIntervalTime);
	m_pMarkMgr->SetChain(NULL);
	m_pMarkMgr->StopThread();

	delete m_pMarkMgr;

	//统计打标参数
	CHLApp* pApp = (CHLApp *)AfxGetApp();
	DWORD dwMarkNum  = pApp->GetProfileInt("MARKNUMBER", "MARKSTAT", 0);
	DWORD dwMarkTime = pApp->GetProfileInt("MARKTIME", "MARKSTAT", 0);
	dwMarkNum  +=  m_nCount;
	dwMarkTime += (m_dwWork+500)/1000;
	pApp->WriteProfileInt("MARKNUMBER", "MARKSTAT", dwMarkNum);
	pApp->WriteProfileInt("MARKTIME", "MARKSTAT", dwMarkTime);
	
	CDialog::OnCancel();
}

BOOL CMarkNormalDlg::PreTranslateMessage(MSG* pMsg) 
{
//	m_tooltip.RelayEvent(pMsg);
    if(!m_bMarkExit)
	{
		if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_RETURN) & 0x8000))
		{
			return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CMarkNormalDlg::OnCheckUpdate() 
{
	UpdateData(TRUE);
}

void CMarkNormalDlg::Mark() 
{
	UpdateData(TRUE);
	m_pMarkMgr->SetMarkType(m_nMarkType);
	m_pMarkMgr->SetLoopMarkInfo(m_bLoopMark, m_nIntervalTime);

	if(m_bMark&&m_bFirst){
		//处于不打标状态时,即下一个状态是打标状态时
		if(m_pMarkMgr->MarkIsOver()){
			return;
		}
	}
	m_bMark=!m_bMark;
	m_bMarkStop=TRUE;
	m_bMarkExit=FALSE;

 	if(!m_bMark){
		if(m_bFirst){
			//使缺省按钮失效
			m_bFirst=FALSE;
			m_pMarkMgr->StartMark();
		}
		else{
			m_pMarkMgr->ReMark();
		}
	}
	else{
		m_pMarkMgr->PauseMark();
	}
	UpdateView();
}

void CMarkNormalDlg::MarkEnd()
{
	DSF_TRACE("end ____ !!!");
	MarkSEnd();
	m_bFirst=TRUE;
	m_bMark=m_bMarkExit=TRUE;
	m_bMarkStop=FALSE;
	
	UpdateView();
	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_MARK));
	if(m_bUpdate)
	{
		if (m_pDoc) 
		{
			m_pDoc->UpdateAllViews(NULL);
			m_pDoc->SetTrackerQuad(m_pChain->GetLimit());
		}
	}

	if(!m_pMarkMgr->GetRedPreview())
		m_nCount++;
	UpdateTime(TRUE);
	UpdateView();
}

void CMarkNormalDlg::MarkSEnd()
{
	m_progressMark.SetPos(0);
	/*m_dwSingle=m_pMarkMgr->GetSingleMarkTime();*/
	m_nTotal=m_nTotal+1;
	UpdateTime();
}

void CMarkNormalDlg::OnRadioLaser() 
{
	UpdateData(TRUE);
	UpdateView();
}

void CMarkNormalDlg::OnRadioRed() 
{
	UpdateData(TRUE);
	UpdateView();
}

void CMarkNormalDlg::OnRadioFrame() 
{
	UpdateData(TRUE);
	UpdateView();
}
//
//void CMarkNormalDlg::OnBUTTONRedCorrSetting() 
//{
//	// TODO: Add your control notification handler code here
//	CDlgRedLight dlg;
//	dlg.DoModal();
//}

void CMarkNormalDlg::OnCheckLoopmark() 
{
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	m_pMarkMgr->SetLoopMarkInfo(m_bLoopMark, m_nIntervalTime);
}
