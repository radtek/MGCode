// NetSet.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "NetSet.h"
#include "afxdialogex.h"


// CNetSet 对话框

IMPLEMENT_DYNAMIC(CNetSet, CDialogEx)

CNetSet::CNetSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetSet::IDD, pParent)
	, m_strIPAddress(_T("127.0.0.1"))
	, m_nPort(9001)
	, m_strSend_NET(_T(""))
	, m_strRec_NET(_T(""))
	,m_bConnected(FALSE)
	, m_bIsNet(TRUE)
	, m_strLotNo(_T(""))
	, m_strEqNo(_T(""))
	, m_strSendShow(_T(""))
	, m_strOperatorcode(_T(""))
	, m_bDebug(FALSE)
	, m_strReverMatriCode(_T(""))
{

}

CNetSet::~CNetSet()
{
}

void CNetSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IPNETADDR, m_strIPAddress);
	DDX_Text(pDX, IDC_EDIT_NETPORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT_SEND, m_strSend_NET);
	DDX_Text(pDX, IDC_EDIT_RECV, m_strRec_NET);
	DDX_Check(pDX, IDC_CHECK_ISNET, m_bIsNet);
	DDX_Text(pDX, IDC_EDIT_LOTNO, m_strLotNo);
	DDX_Text(pDX, IDC_EDIT_EQNO, m_strEqNo);
	DDX_Text(pDX, IDC_EDIT_SENDSHOW, m_strSendShow);
	DDX_Text(pDX, IDC_EDIT_OPERATORCODE, m_strOperatorcode);
	DDX_Check(pDX, IDC_CHECK_DEBUG, m_bDebug);
	DDX_Control(pDX, IDC_COMBO_SEND, m_ComBoSend);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_ComBoStyle);
	DDX_Text(pDX, IDC_EDIT_REMATRICODE, m_strReverMatriCode);
}


BEGIN_MESSAGE_MAP(CNetSet, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_OPENNET, &CNetSet::OnBnClickedBtnOpennet)
	ON_BN_CLICKED(IDC_BTN_CLOSENET, &CNetSet::OnBnClickedBtnClosenet)
	ON_BN_CLICKED(IDC_BTN_SEND, &CNetSet::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDOK, &CNetSet::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_ISNET, &CNetSet::OnBnClickedCheckIsnet)
END_MESSAGE_MAP()


// CNetSet 消息处理程序
void CNetSet::OnBnClickedBtnOpennet()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_ClientSock.SetSocket(1,m_strIPAddress,m_nPort);
	m_bConnected = m_ClientSock.Connet();

	m_strRec_NET = "";
	m_strSendShow = "";

	UpdateNetView();
	UpdateData(FALSE);
}


void CNetSet::UpdateNetView()
{	
	GetDlgItem(IDC_BTN_OPENNET)->EnableWindow(!m_bConnected);
	GetDlgItem(IDC_BTN_CLOSENET)->EnableWindow(m_bConnected);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(m_bConnected);
}

void CNetSet::OnBnClickedBtnClosenet()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_ClientSock.IsConnected())
	{
		m_ClientSock.Close();
	}
	m_bConnected = FALSE;

	UpdateNetView();
	m_strRec_NET = "";
	m_strSendShow = "";
}


BOOL CNetSet::NetSendRecvOpenDoc()
{
	UpdateData(TRUE);

	CString strSend     = "TP02\r\n!COMMAND_START\r\nID 510008\r\n";
	CString strLotNo	 = "ARG NOC0027 " +  m_strLotNo + "\r\n";
	CString strPanleID   = "ARG IDC6017 " + m_strSend_NET + "\r\n";
	CString strEqNo		 = "ARG CDC0010 " + m_strEqNo + "\r\n";
	CString strOperatorCode = "ARG CDC0145 " + m_strOperatorcode + "\r\n";
	CString strProcess;
	if (m_ComBoStyle.GetCurSel())
	{
		strProcess = "ARG DHC0013 CUT\r\n";
	}
	else
	{
		strProcess = "ARG DHC0013 TRENCH\r\n";
	}


	strSend = strSend + strPanleID + strProcess + strEqNo + strOperatorCode  + "!COMMAND_END\r\nEND\r\n";
	m_strSendShow = strSend;
	m_ClientSock.ClearBuffer();
	m_ClientSock.Send(strSend);
	WriteRunLog(strSend);
	CString strFirst = m_ClientSock.Receive();
	CString str;
	str = "One Receive: " + strFirst;
	WriteRunLog(str);
	CString strEnd = m_ClientSock.Receive();
	str = "Two Receive: " + strEnd;
	WriteRunLog(str);
	m_strRec_NET = strFirst + strEnd;
	UpdateData(FALSE);
	return TRUE;
}


BOOL CNetSet::NetSendRecvAfterData()
{

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (pFWnd==NULL) 
		return FALSE;
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return FALSE;
	}
	CHLDoc *pDoc = pView->GetDocument();
	if (NULL == pDoc)
	{
		return FALSE;
	}
	CString strSend     = "TP02\r\n!COMMAND_START\r\nID 510024\r\n";
	CString strEqNo		 = "ARG CDC0010 " + m_strEqNo + "\r\n";
	CString strPanleID   = "ARG IDC6017 " + m_strSend_NET + "\r\n";
	CString strName	 =		"ARG DHC0937 " + pDoc->GetPathName() + "\r\n";


	PARA curMarkPara =  pDoc->GetLay().GetPara(0);
	CString strSpeed;
	strSpeed.Format(_T("0000%d"),(int)curMarkPara.dbMarkSpeed);
	strSpeed = "/" + strSpeed.Right(4);

	CString strQFreq;
	strQFreq.Format(_T("000%d"),(int)curMarkPara.dbQFreq);
	strQFreq =  "/" + strQFreq.Right(3);

	CString strQRelease;
	strQRelease.Format(_T("000%d"),(int)curMarkPara.dbQRelease);
	strQRelease =  "/" + strQRelease.Right(3);

	CString strCurrent;
	strCurrent.Format(_T("00%d"),(int)curMarkPara.dbCurrent);
	strCurrent =  "/" + strCurrent.Right(2);

	CString strPara = strSpeed + strQFreq + strQRelease + strCurrent + "\r\n";
	strPara = "ARG DHC6280 " + strPara;

	strSend = strSend + strPanleID + strEqNo + strName + strPara + "!COMMAND_END\r\nEND\r\n";

	m_strSendShow = strSend;
	m_ClientSock.ClearBuffer();
	m_ClientSock.Send(strSend);
	WriteRunLog(strSend);
	CString strFirst = m_ClientSock.Receive();
	CString str;
	str = "One Receive: " + strFirst;
	WriteRunLog(str);
	CString strEnd = m_ClientSock.Receive();
	str = "Two Receive: " + strEnd;
	WriteRunLog(str);
	m_strRec_NET = strFirst + strEnd;

	UpdateData(FALSE);

	return TRUE;
}

BOOL CNetSet::NetSendRecv()
{
	CString strSend     = "TP02\r\n!COMMAND_START\r\nID 510023\r\n";
	CString strLotNo	 = "ARG NOC0027 " +  m_strLotNo + "\r\n";
	CString strPanleID   = "ARG IDC6017 " + m_strSend_NET + "\r\n";
	CString strEqNo		 = "ARG CDC0010 " + m_strEqNo + "\r\n";
	CString strOperatorCode = "ARG CDC0145 " + m_strOperatorcode + "\r\n";

	strSend = strSend + strPanleID + strEqNo + "!COMMAND_END\r\nEND\r\n";
	m_strSendShow = strSend;
	m_ClientSock.ClearBuffer();
	m_ClientSock.Send(strSend);
	WriteRunLog(strSend);
	CString strFirst = m_ClientSock.Receive();
	CString str;
	str = "One Receive: " + strFirst;
	WriteRunLog(str);
	CString strEnd = m_ClientSock.Receive();
	str = "Two Receive: " + strEnd;
	WriteRunLog(str);
	m_strRec_NET = strFirst + strEnd;

	UpdateData(FALSE);
	return TRUE;

}


void CNetSet::OnBnClickedBtnSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (pFWnd==NULL) 
		return ;
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return ;
	}
	CHLDoc *pDoc = pView->GetDocument();
	if (NULL == pDoc)
	{
		return ;
	}

	UpdateData(TRUE); 
	if (m_ClientSock.IsConnected())
	{
		int nSend = m_ComBoSend.GetCurSel();
		switch(nSend)
		{
			case 0:
				NetSendRecvOpenDoc();
				break;
			/*case 1:
				NetSendRecv();
				break;*/
			case 1:
				NetSendRecvAfterData();
				break;
		}
	}
	else
	{
		OnBnClickedBtnClosenet();
	}
}


BOOL CNetSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_bDebug = g_ParaIni.ReadInt(_T("ISDEBUG"),_T("IsDebug"),0);
	m_bIsNet = g_ParaIni.ReadInt(_T("ISNET"),_T("IsNet"),1);
	m_strIPAddress = g_ParaIni.ReadString(_T("IPADDR"),_T("IpAddr"),_T("127.0.0.1"));
	m_nPort = g_ParaIni.ReadInt(_T("NETPORT"),_T("NetPort"),9001);

	m_strLotNo =  g_ParaIni.ReadString(_T("LOTNO"),_T("LotNo"),_T("M460613080"));
	m_strEqNo =  g_ParaIni.ReadString(_T("EQNO"),_T("EqNo"),_T("HANS-100"));
	m_strSend_NET = g_ParaIni.ReadString(_T("MATRICODE"),_T("MatriCode"),_T(""));

	m_strReverMatriCode = g_ParaIni.ReadString(_T("REVERSEMATRICODE"),_T("ReverseMatriCode"),_T(""));

	m_ComBoSend.InsertString(0,"revOpenFile");
	//m_ComBoSend.InsertString(1,"PCB2D");
	m_ComBoSend.InsertString(1,"AfterData");

	m_ComBoSend.SetCurSel(g_ParaIni.ReadInt(_T("COMBOSEND"),_T("ComBoSend"),0));


	m_ComBoStyle.InsertString(0,"TRENCH");
	m_ComBoStyle.InsertString(1,"CUT");
	m_ComBoStyle.SetCurSel(g_ParaIni.ReadInt(_T("COMBOSTYLE"),_T("ComBoStyle"),0));


	m_strRec_NET = "";
	UpdateData(FALSE);

	UpdateNetView();

	OnBnClickedCheckIsnet();

	if (m_bIsNet)
	{
		OnBnClickedBtnOpennet();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CNetSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	g_ParaIni.WriteString(_T("IPADDR"),_T("IpAddr"),m_strIPAddress);
	g_ParaIni.WriteInt(_T("NETPORT"),_T("NetPort"),m_nPort);
	g_ParaIni.WriteInt(_T("ISNET"),_T("IsNet"),m_bIsNet);
	g_ParaIni.WriteInt(_T("ISDEBUG"),_T("IsDebug"),m_bDebug);

	g_ParaIni.WriteString(_T("LOTNO"),_T("LotNo"),m_strLotNo);
	g_ParaIni.WriteString(_T("EQNO"),_T("EqNo"),m_strEqNo);
	g_ParaIni.WriteString(_T("MATRICODE"),_T("MatriCode"),m_strSend_NET);
	g_ParaIni.WriteString(_T("REVERSEMATRICODE"),_T("ReverseMatriCode"),m_strReverMatriCode);

	g_ParaIni.WriteInt(_T("COMBOSEND"),_T("ComBoSend"),m_ComBoSend.GetCurSel());
	g_ParaIni.WriteInt(_T("COMBOSTYLE"),_T("ComBoStyle"),m_ComBoStyle.GetCurSel());

	CDialogEx::OnOK();
}


void CNetSet::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedBtnClosenet();

	
	CDialogEx::OnClose();
}


void CNetSet::OnBnClickedCheckIsnet()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	GetDlgItem(IDC_BTN_OPENNET)->EnableWindow(m_bIsNet);
	GetDlgItem(IDC_BTN_CLOSENET)->EnableWindow(m_bIsNet);
	GetDlgItem(IDC_BTN_SEND)->EnableWindow(m_bIsNet);
	GetDlgItem(IDC_EDIT_RECV)->EnableWindow(m_bIsNet);
	GetDlgItem(IDC_EDIT_SEND)->EnableWindow(m_bIsNet);
	GetDlgItem(IDC_EDIT_IPNETADDR)->EnableWindow(m_bIsNet);
	GetDlgItem(IDC_EDIT_NETPORT)->EnableWindow(m_bIsNet);
	if (m_bIsNet)
	{
		UpdateNetView();
	}
}


BOOL CNetSet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN && pMsg->wParam!=VK_TAB)
	{
		CWnd *pWnd1 = GetDlgItem(IDC_EDIT_LOTNO);
		CWnd *pWnd2 = GetDlgItem(IDC_EDIT_OPERATORCODE);
		CWnd *pWnd3 = GetDlgItem(IDC_EDIT_SEND);
		CWnd *pWnd4 = GetDlgItem(IDC_EDIT_EQNO);
		CWnd *pWnd5 = GetDlgItem(IDC_EDIT_REMATRICODE);
		CEdit*pEdit = NULL;

		//if (GetFocus()!= pWnd1 && GetFocus()!= pWnd2 && GetFocus()!=pWnd3 && GetFocus()!=pWnd4)
		//{
		//	pEdit = (CEdit*)pWnd1;
		//	pEdit->SetFocus();
		//	pEdit->SetSel(0,-1,TRUE);
		//	UpdateData(FALSE);
		//}

		if (pMsg->wParam==VK_RETURN)  ////回车键按键消息
		{
			HWND hWnd = GetFocus()->GetSafeHwnd();
			if (pMsg->hwnd==hWnd ) //只捕获指定窗口的消息
			{
				UpdateData(TRUE);
				////回车后自动以下一个输入窗口作为焦点
				if (GetFocus()==pWnd1)
					pEdit = (CEdit*)pWnd2;
				else if (GetFocus()==pWnd2)
					pEdit = (CEdit*)pWnd3;
				else if (GetFocus()==pWnd3)
					pEdit = (CEdit*)pWnd4;
				else if (GetFocus()==pWnd4)
					pEdit = (CEdit*)pWnd5;
				else
					pEdit = (CEdit*)pWnd1;
				pEdit->SetFocus();
				pEdit->SetSel(0,-1,TRUE);
				UpdateData(FALSE);
			}
		}
		return 0;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

