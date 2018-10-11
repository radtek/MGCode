// MarkMulDocDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "HLDoc.h"
#include "MarkMulDocDlg.h"
#include "SetRunDlg.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLOCKFREQ_AND_TOTTICKS 100	//VXD
#define W32IF_PASS_EVENT 110

/////////////////////////////////////////////////////////////////////////////
// CMarkMulDocDlg dialog


CMarkMulDocDlg::CMarkMulDocDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkMulDocDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkMulDocDlg)
	m_bManual = FALSE;
	m_nFreq = 0;
	m_nMarkType = 0;
	m_bLoopMark= FALSE;
	//}}AFX_DATA_INIT
	m_nCount=-1;
	m_dwTotal=0;
	m_dwTotalStart=0;
	m_dwTotalEnd=0;
	m_dwSingle=0;
	m_nCur = 0;
	m_bPrevFrame = FALSE;
}

void CMarkMulDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkMulDocDlg)
	DDX_Control(pDX, IDC_PROGRESS_MARK, m_progressMark);
	DDX_Control(pDX, IDC_LIST_DOCNAME, m_listDocName);
	DDX_Check(pDX, IDC_CHECK_MANUAL, m_bManual);
	DDX_Text(pDX, IDC_EDIT_FREQ, m_nFreq);
	DDX_Radio(pDX, IDC_RADIO_LASER, m_nMarkType);
	DDX_Check(pDX, IDC_CHECK_LOOPMULMARK, m_bLoopMark);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarkMulDocDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkMulDocDlg)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MARK, OnButtonMark)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_CHECK_MANUAL, OnCheckManual)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MARK_END,OnMarkEnd)
	ON_MESSAGE(WM_MARK_START,OnMarkStart)
	ON_MESSAGE(WM_MARK_STOP,OnMarkStop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkMulDocDlg message handlers
BOOL CMarkMulDocDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//取得各打开文档的指针
	CHLApp *pApp = (CHLApp *)AfxGetApp();
	CMultiDocTemplate *pDocTem;
	CHLDoc *pDoc;

	POSITION posTem = pApp->GetFirstDocTemplatePosition();
	while(posTem != NULL)
	{
		pDocTem = (CMultiDocTemplate *)pApp->GetNextDocTemplate(posTem);
		POSITION posDoc = pDocTem->GetFirstDocPosition();
		while(posDoc != NULL)
		{
			pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
			pDoc->PreprocessMark();
			m_arrayDoc.Add(pDoc);
			m_listDocName.AddString(pDoc->GetTitle());
		}
	}
	m_listDocName.SetSel(0);
	if(m_listDocName.GetCount()==0){
			CDialog::OnCancel();
			return FALSE;
	}

	m_pMarkMgr = new CMarkMgr();
	m_pMarkMgr->RunThread((LPVOID)GetSafeHwnd());
	
	m_progressMark.SetRange32(0,10);
	m_progressMark.SetStep(1);

	m_bFirst=TRUE;
	m_bMark=m_bMarkExit=TRUE;
	m_bMarkStop=FALSE;
	UpdateView();

	UpdateTime();
	UpdateCount();
	//定义脚踏开关线程

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMarkMulDocDlg::OnButtonDown() 
{
	int nCount = m_listDocName.GetCount();
	if (0 == nCount)
	{
		return;
	}

	int nSel = m_listDocName.GetCurSel();
	m_listDocName.SetSel(nSel);

	m_listDocName.SelItemRange(FALSE,0,nCount);

	if(nSel==LB_ERR || nSel==(nCount-1)) return;

	CString str;
	m_listDocName.GetText(nSel,str);
	m_listDocName.DeleteString(nSel);
	m_listDocName.InsertString((nSel+1),str);
	m_listDocName.SetSel(nSel+1);
	UpdateData(FALSE);	

	CHLDoc *pDoc;
	pDoc=(CHLDoc *)m_arrayDoc.GetAt(nSel);
	m_arrayDoc.RemoveAt(nSel);
	m_arrayDoc.InsertAt(nSel+1,pDoc);
}

void CMarkMulDocDlg::OnButtonUp() 
{
	int nCount = m_listDocName.GetCount();
	if (0 == nCount)
	{
		return;
	}

	int nSel = m_listDocName.GetCurSel();
	m_listDocName.SetSel(nSel);
	m_listDocName.SelItemRange(FALSE,0,nCount);
	if(nSel==LB_ERR || nSel<1) return;

	CString str;
	m_listDocName.GetText(nSel,str);
	m_listDocName.DeleteString(nSel);
	m_listDocName.InsertString((nSel-1),str);
	m_listDocName.SetSel(nSel-1);
	UpdateData(FALSE);	

	CHLDoc *pDoc;
	pDoc=(CHLDoc *)m_arrayDoc.GetAt(nSel);
	m_arrayDoc.RemoveAt(nSel);
	m_arrayDoc.InsertAt(nSel-1,pDoc);
}

void CMarkMulDocDlg::OnButtonDel() 
{
	//multiple-document;
	int nCount = m_listDocName.GetSelCount();
	if (0 == nCount)
	{
		return;
	}
	
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_listDocName.GetSelItems(nCount, aryListBoxSel.GetData());

	int nSel;
	for(int i=0;i<nCount;i++)
	{
		nSel = aryListBoxSel.GetAt(i);
		if(nSel == LB_ERR) return;
		m_listDocName.DeleteString(nSel-i);
		m_arrayDoc.RemoveAt(nSel-i);
	}

	UpdateData(FALSE);
//	m_listDocName.SetCurSel(0);
}

void CMarkMulDocDlg::OnCancel() 
{
	m_pMarkMgr->SetChain(NULL);
	m_pMarkMgr->StopThread();
	delete m_pMarkMgr;
	
	CDialog::OnCancel();
}

void CMarkMulDocDlg::OnButtonMark() 
{
	UpdateData(TRUE);
	
	if (0 == m_arrayDoc.GetSize()) 
	{
		return;
	}

// 	CHLApp *pApp=(CHLApp *)AfxGetApp();
// 	pApp->WriteProfileDouble("RedLight","ReadScaleX",m_dbRedScaleX);
// 	pApp->WriteProfileDouble("RedLight","ReadScaleY",m_dbRedScaleY);
	Mark();
}

void CMarkMulDocDlg::OnButtonStop() 
{
	//设置停止事件，打标线程检测到该事件后退出打标
	m_nCur=0;
	m_bMark=m_bMarkExit=TRUE;
	m_bMarkStop=FALSE;
	m_pMarkMgr->StopMark();
	UpdateView();
}

void CMarkMulDocDlg::UpdateView()
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
			GetDlgItem(IDC_RADIO_LASER)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_RED)->ShowWindow(TRUE);
			GetDlgItem(IDC_RADIO_FRAME)->ShowWindow(TRUE);
		}
	}
	SetDlgItemText(IDC_BUTTON_MARK,str);
	
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(m_bMarkStop);
	GetDlgItem(IDCANCEL)->EnableWindow(m_bMarkExit);

	GetDlgItem(IDC_RADIO_LASER)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_RADIO_RED)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_RADIO_FRAME)->EnableWindow(m_bMarkExit);	

	GetDlgItem(IDC_BUTTON_DOWN)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_UP)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_SET)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_EDIT_FREQ)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(m_bMarkExit);
	GetDlgItem(IDC_CHECK_MANUAL)->EnableWindow(m_bMarkExit);
}

LRESULT CMarkMulDocDlg::OnMarkEnd(WPARAM wParam,LPARAM lParam)
{	
	switch(wParam)
	{
	case MARK_TEND:
		MarkEnd();
		break;
	case MARK_SEND:
//		MarkSEnd();
		break;
	case MARK_INFO:
		m_progressMark.SetPos(lParam);
		break;
	}
	return NULL;
}
void CMarkMulDocDlg::MarkEnd()
{
	MarkSEnd();
	m_bFirst=TRUE;
	m_bMark=m_bMarkExit=TRUE;
	m_bMarkStop=FALSE;
	
	Run();	//工作台移动
	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_MARK));

	int nCount = m_listDocName.GetCount();
	m_listDocName.SelItemRange(FALSE,0,nCount);
	m_listDocName.SetSel(m_nCur);

 	UpdateView();
	GotoDlgCtrl(GetDlgItem(IDC_BUTTON_MARK));
	
//	if(!m_pMarkMgr->GetRedPreview())
//		m_nCount++;
//	UpdateView();

	if(m_bManual)
	{
		if(m_nCur==0){
			m_dwTotalEnd=timeGetTime();
			m_dwTotal =m_dwTotalEnd -m_dwTotalStart;
			UpdateCount();
			m_dwTotalStart=0;
		}
	}
	else
	{
		if(m_nCur)
		{
			PostMessage(WM_MARK_START);
		}
		else
		{
			// 消息循环
			if (m_bLoopMark)
			{
				CString strValue("");
				GetDlgItem(IDC_EDIT_INTERVALTIME)->GetWindowText(strValue);
				int nValue = atoi(strValue);
				Sleep(nValue);
				PostMessage(WM_MARK_START);
			}
			else
			{
				m_dwTotalEnd=timeGetTime();
				m_dwTotal =m_dwTotalEnd -m_dwTotalStart;
				UpdateCount();
				m_dwTotalStart=0;
			}

			
		}
	}
}

void CMarkMulDocDlg::MarkSEnd()
{
	m_progressMark.SetPos(0);
	m_dwSingle=m_pMarkMgr->GetSingleMarkTime();
// 	m_nCount=m_nCount+1;
	UpdateTime();
}

LRESULT CMarkMulDocDlg::OnMarkStop(WPARAM wParam,LPARAM lParam)
{
	OnButtonStop() ;
	return NULL;
}

LRESULT CMarkMulDocDlg::OnMarkStart(WPARAM wParam,LPARAM lParam)
{
	Mark();
	return NULL;
}

void CMarkMulDocDlg::OnCheckManual() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

BOOL CMarkMulDocDlg::GetNext()
{
	m_nCur = m_listDocName.GetCurSel();

	m_pDoc=(CHLDoc *)m_arrayDoc.GetAt(m_nCur++);

	if(m_nCur>=m_arrayDoc.GetSize())
	{
		m_nCur=0;
	}

	//打标参数更新
	m_pMarkMgr->SetLay(m_pDoc->GetLay());
	DSF_TRACE("Set Lay OK ...");
	m_pMarkMgr->SetChain(m_pDoc);
	return TRUE;
}

void CMarkMulDocDlg::OnButtonSet() 
{
	int nCount = m_listDocName.GetSelCount();
	if (nCount==0)
	{
		return;
	}
	CArray<int,int> aryListBoxSel;

	aryListBoxSel.SetSize(nCount);
	m_listDocName.GetSelItems(nCount, aryListBoxSel.GetData());

	int nSel;
	nSel = aryListBoxSel.GetAt(0);
	if(nSel == LB_ERR) return;

	CHLDoc *pDoc;
	pDoc=(CHLDoc *)m_arrayDoc.GetAt(nSel);
	MOTOR motor=pDoc->m_motor;

	//如果MOTOR的值不等，则将MOTOR的值改为零；
	for(int i=0;i<nCount;i++)
	{
		nSel = aryListBoxSel.GetAt(i);
		if(nSel == LB_ERR) return;
		pDoc=(CHLDoc *)m_arrayDoc.GetAt(nSel);
		if(pDoc->m_motor.nDelay != motor.nDelay)
			motor.nDelay = 0;
		if(pDoc->m_motor.nMove != motor.nMove)
			motor.nMove = 0;
	}

	CSetRunDlg dlg;
	dlg.m_nDelay = motor.nDelay;
	dlg.m_nMove = motor.nMove;
	dlg.m_nDirection = motor.nDirection;
	if(dlg.DoModal() == IDOK)
	{
		motor.nDelay = dlg.m_nDelay;
		motor.nMove = dlg.m_nMove;
		motor.nDirection = dlg.m_nDirection;

		for(int i=0;i<nCount;i++)
		{
			nSel = aryListBoxSel.GetAt(i);
			if(nSel == LB_ERR) return;
			pDoc=(CHLDoc *)m_arrayDoc.GetAt(nSel);
			pDoc->m_motor = motor;
		}
	}
}

void CMarkMulDocDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CHLDoc *pDoc;
	CString str;
	int count=m_arrayDoc.GetSize();
	for(int i=0;i<count;i++)
	{
		pDoc=(CHLDoc *)m_arrayDoc.GetAt(i);
		if(pDoc->GetPathName()=="")
		{
			str.LoadString(IDS_MULDOC_NOPATH);
			AfxMessageBox(str,MB_ICONSTOP);
			return;
		}
	}

	//保存多文档数据
	static char szFilter[]="Muldoc File(*.mul)|*.mul||";
	CFileDialog dlg(FALSE,"mul",NULL,OFN_HIDEREADONLY,szFilter);
	
	str.LoadString(IDS_MULDOC_SAVE);
	dlg.m_ofn.lpstrTitle=str;
	if(dlg.DoModal()==IDOK)
	{
		str=dlg.GetPathName();
		BOOL bIsExit = IsFileExist(str);
		if (bIsExit) 
		{
			if (IDOK != AfxMessageBox(IDS_YH_DULNAME, MB_OKCANCEL)) 
			{
				return;
			}
		}

		CFile f;
		f.Open(str,CFile::modeWrite|CFile::modeCreate );
		CArchive ar(&f,CArchive::store);

		//贮存数据
		str="2009.03.15";//	str="2001.4.25";
		ar<<str;
		ar<<count;
		for(int i=0;i<count;i++)
		{
			pDoc=(CHLDoc *)m_arrayDoc.GetAt(i);
			ar<<pDoc->GetPathName();
			ar<<pDoc->m_motor.nMove;
			ar<<pDoc->m_motor.nDirection;
			ar<<pDoc->m_motor.nDelay;
			ar<<pDoc->m_motor.strIO;
		}
		ar<<m_nFreq;
		
		ar.Close();
		f.Close();
	}
}

void CMarkMulDocDlg::Run()
{
	//工作台转动
	MOTOR motor=m_pDoc->m_motor;
	RotateMoto(motor.nMove,motor.nDirection,m_nFreq);
	Sleep(motor.nDelay);
	Sleep(motor.nDelay);
}

void CMarkMulDocDlg::UpdateTime()
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
	SetDlgItemText(IDC_SINGLE_TIME,str);

	if(!m_pMarkMgr->GetRedPreview())
		m_nCount++;
	SetDlgItemInt(IDC_NUM,m_nCount);
}

void CMarkMulDocDlg::UpdateCount()
{
	CString str;
	int day,hour,min,ms;
	double sec;

	ms=m_dwTotal%1000;
	m_dwTotal/=1000;

	sec=m_dwTotal%60;
	m_dwTotal/=60;
	sec+=(ms/1000.0);

	min=m_dwTotal%60;
	m_dwTotal/=60;

	hour=m_dwTotal%24;
	day=m_dwTotal/24;

	str.Format("%d:%2d:%2d:%6.3f",day,hour,min,sec);
	SetDlgItemText(IDC_TOTAL_TIME,str);
}

void CMarkMulDocDlg::Mark() 
{
	UpdateData(TRUE);
 	m_pMarkMgr->SetMarkType(m_nMarkType);
	
	//得到本次要进行标记的文档
	if(m_bMark && m_bFirst)
	{
		if(!GetNext())
			return;
	}

	//界面改变
	m_bMark=!m_bMark;
	m_bMarkStop=TRUE;
	m_bMarkExit=FALSE;
	UpdateView();

	if(m_dwTotalStart==0)
		m_dwTotalStart=timeGetTime();

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
}

void CMarkMulDocDlg::RotateMoto(long nMove, int nDirection, int nFreq)
{
	BYTE nOutData;
	long nTotal=nMove;
	long nOuted=0;
	nOutData=_inp(0x37a);//取并行口的信号

	if(nDirection)  //设置方向信号
		nOutData=nOutData|0x01;
	else
		nOutData=nOutData&0xFE;

    _outp(0x37a,nOutData);//先发送方向信号

	int nPulseWidth; //脉冲脉宽
	nPulseWidth=1000000/(abs(nFreq)*2);
	while(nOuted<nTotal)
	{
        nOutData=nOutData|0x08;
		_outp(0x37a,nOutData);    //输出高电平
		Delay(nPulseWidth);
		nOutData=nOutData&0xF7;
		_outp(0x37a,nOutData);    //输出低电平
		Delay(nPulseWidth);
		nOuted++;
	}

}

void CMarkMulDocDlg::Delay(UINT nMicroSecond)
{
//延时函数，参数为要延时的微秒数
	LARGE_INTEGER f1,f2,f3;
	UINT k;
	if(!QueryPerformanceFrequency(&f1))
		return;
	QueryPerformanceCounter(&f2);
	while(1)
	{
		QueryPerformanceCounter(&f3);
		k=(int)((1000000.0/f1.QuadPart)*(f3.QuadPart-f2.QuadPart));
		if(k>nMicroSecond)
			break;
	}

}


BOOL CMarkMulDocDlg::IsFileExist(CString &strFileName)
{
	CFileFind ofilefind;
	if (!ofilefind.FindFile(strFileName))
	{
		ofilefind.Close();

		return FALSE;
	}

	ofilefind.Close();

	return TRUE;
}

// void CMarkMulDocDlg::OnRadioFrame() 
// {
// 	UpdateData(TRUE);
// 	UpdateView();
// }
// 
// void CMarkMulDocDlg::OnRadioLaser() 
// {
// 	UpdateData(TRUE);
// 	UpdateView();
// }
// 
// void CMarkMulDocDlg::OnRadioRed() 
// {
// 	UpdateData(TRUE);
// 	UpdateView();
// }
