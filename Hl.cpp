// HL.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "HL.h"
#include "ErrorDlg.h"


#include "MainFrm.h"
#include "ChildFrm.h"
#include "HLDoc.h"
#include "HLView.h"
#include "SetTrackerDlg.h"
#include "OptionDlg.h"
#include "MarkMulDocDlg.h"
#include "MarkdataStatDlg.h"
#include "DlgGrayCompensate.h"
#include "LPSetINF.h"
#include "UserLogin.h"
#include "RedMarkThread.h"
#include "DlgSplash.h"
#include "DlgMarkNormalPane.h"

#include "CCDCalibrate.h"
#include "WBGridCali.h"
#include "CCDSettingDlg.h"
#include "DlgSetWorkbench.h"
#include "DlgCaliSet.h"
#include "DogCheck.h"
#include "DlgPowerTest.h"

#include "MatrixDoc.h"
#include "AdoDb.h"
#include "DataSourceDlg.h"
#include "DlgAgingTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HANDLE g_hEventMainThreadEmergency = ::CreateEvent(NULL,TRUE,FALSE,NULL);

CStringArray g_arrMulDoc;

CHlParaIni g_ParaIni;
CHLMotion *g_pMotion = NULL;

DBPARA		g_para;

BOOL g_bLaserReady=FALSE; 
COpcLaser* g_OPCLaser = NULL;
BOOL g_bReady = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CHLApp

BEGIN_MESSAGE_MAP(CHLApp, CWinAppEx)
	//{{AFX_MSG_MAP(CHLApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_SET_HARDWARE, OnSetHardware)
	ON_COMMAND(ID_OPTION, OnOption)
	ON_COMMAND(ID_SET_TRACKER, OnSetTracker)
	ON_COMMAND(ID_FILE_MULDOC, OnFileMuldoc)
	ON_COMMAND(ID_MARK_STAT, OnMarkStat)
	ON_COMMAND(ID_CALIBRATE_NET, OnCalibrate)
	ON_COMMAND(ID_CALIBRATE, OnCalibrate)
	ON_COMMAND(IDM_GRAYCURRENT,OnGrayCurrent)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_MENU_FONT_FILE_NEW, &CHLApp::OnMenuFontFileNew)
	ON_UPDATE_COMMAND_UI(ID_MENU_FONT_FILE_NEW, &CHLApp::OnUpdateMenuFontFileNew)
	ON_COMMAND(ID_MENU_FONT_FILE_OPEN, &CHLApp::OnMenuFontFileOpen)
	ON_UPDATE_COMMAND_UI(ID_MENU_FONT_FILE_OPEN, &CHLApp::OnUpdateMenuFontFileOpen)
	ON_COMMAND(ID_MENU_FONT_FILE_SAVE, &CHLApp::OnMenuFontFileSave)
	ON_UPDATE_COMMAND_UI(ID_MENU_FONT_FILE_SAVE, &CHLApp::OnUpdateMenuFontFileSave)
	ON_COMMAND(ID_MENU_FONT_FILE_SAVEAS, &CHLApp::OnMenuFontFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_MENU_FONT_FILE_SAVEAS, &CHLApp::OnUpdateMenuFontFileSaveAs)
	ON_COMMAND(ID_SET_WORKBENCH, &CHLApp::OnSetWorkbench)
	ON_COMMAND(ID_GRID_CALI, &CHLApp::OnGridCali)
	ON_COMMAND(ID_CCD_OPERATION, &CHLApp::OnCcdOperation)
	ON_COMMAND(ID_CCD_VIEW, &CHLApp::OnCcdView)
	ON_COMMAND(ID_FILE_MULDOC_OPEN, &CHLApp::OnFileMulDocOpen)
	ON_COMMAND(ID_FILE_MULDOC_SAVE, &CHLApp::OnFileMulDocSave)
	ON_COMMAND(ID_CHANGEDOC, &CHLApp::OnChangedoc)
	ON_COMMAND(ID_POWER_TEST, &CHLApp::OnPowerTest)
	ON_COMMAND(ID_MATRIX_DOC_SET, &CHLApp::OnMatrixDocSet)
	ON_COMMAND(ID_SET_DATABASE, &CHLApp::OnSetDatabase)
	ON_COMMAND(ID_AGING_TEST, &CHLApp::OnAgingTest)
	ON_UPDATE_COMMAND_UI(ID_AGING_TEST, &CHLApp::OnUpdateAgingTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHLApp construction

CHLApp::CHLApp()
{
	// 支持重新启动管理器
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MG.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_hLib       = NULL;
	m_hLangDLL   = NULL;
	m_hMutexNew = NULL;
	char pszTemp[512];
	GetExepath(pszTemp);
	m_strAppIni = pszTemp;
	m_strAppIni += "app.ini";
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHLApp object
extern CArray<USERINFO, USERINFO> g_RightInfoList;

CHLApp theApp;

int g_nLeftDay = 999;

/////////////////////////////////////////////////////////////////////////////
// CHLApp initialization
CONST CString G_HL5VER = "FPC_LASER_C19B4089-F327-448d-A33C-E09F5705E908";

BOOL CHLApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	GdiplusStartup(&m_GdiplusToken,& m_Gdistart,NULL);

	CWinAppEx::InitInstance();

	InitHLExceptionManager();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);
	m_bSaveState = FALSE;	//关闭自动写注册表记录界面状态

	//SetRegistryKey("Super Precision Laser Marking System 1.0");
	SetRegistryKey("PCB Laser Cutting System V1");
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
	
	g_sys.ReadSysCfg();       // 取系统配置信息
	g_sys.ReadSysPara();  // 取系统打标配置信息
	g_sys.WriteSysCfg();

	//确定语言版本
 	CString str;
	str.Format("mg%x.DLL",g_sys.m_langID);
	g_LangID = g_sys.m_langID;

 	m_hLangDLL=AfxLoadLibrary(str);
 	if(m_hLangDLL)
 	{
 		AfxSetResourceHandle(m_hLangDLL); //设定资源文件
 	}

	//1.0以后软件采用的互斥体串
	m_hMutexNew = ::OpenMutex(MUTEX_ALL_ACCESS, TRUE, G_HL5VER);
	if(NULL == m_hMutexNew)
	{
		m_hMutexNew = ::CreateMutex(NULL, TRUE, G_HL5VER);
	}
	else
	{
		AfxMessageBox(IDS_ONLY_ONE_COPY);
		return FALSE;
	}

#ifndef _DEBUG
	//屏蔽此处加密狗代码以使软件启动	周欢	2018.06.22
	//if (!g_DogCheck.__Dog())
	//{
	//	return FALSE;
	//}
#endif

	if (g_sys.m_nLaserCurrentMode && g_sys.m_nLaserControllerPort==0)
	{
		AfxMessageBox("启用了串口调电流，但未配置串口号!");
		return FALSE;
	}
	
	//Splash s1
	CDlgSplash *pDlgSplash = new CDlgSplash;
	pDlgSplash->Create(IDD_SPLASH, NULL);
	pDlgSplash->ShowWindow(SW_SHOW);
	pDlgSplash->UpdateWindow();
	m_pMainWnd = pDlgSplash;

	CString strWelcome;
	strWelcome.LoadString(IDS_WELCOME_CHECKDOG);
	pDlgSplash->SetPaneText(strWelcome);
	strWelcome.LoadString(IDS_WELCOME_CHECKUSER);
	pDlgSplash->SetPaneText(strWelcome);
	// 读取用户信息
	CString strFileName("");
	g_sys.ReadRightInfo(strFileName);

	int nCount = g_RightInfoList.GetSize();
	if (0 == nCount) 
	{
		if (IDOK != AfxMessageBox(IDS_NOUSERINFO, MB_OKCANCEL)) 
		{
			delete pDlgSplash;
			return FALSE;
		}
	}

	USERINFO stUserInfo;
	BOOL bRetUser = g_sys.GetDefaultUserInfo(stUserInfo);
	if (bRetUser)
	{
		g_CurUserInfo = stUserInfo;
	}
	else
	{
		CUserLogin Login;
		if (0 == nCount) 
		{
			Login.m_dwDispMsg = 2;
		}
		else
		{
			Login.m_dwDispMsg = 1;
		}

		if (IDOK == Login.DoModal())
		{
			if (!(Login.m_bIsOK))
			{
				delete pDlgSplash;
				return FALSE;
			}
		}
		else
		{
			delete pDlgSplash;
			return FALSE;
		}
	}


	strWelcome.LoadString(IDS_WELCOME_INITCARD);
	pDlgSplash->SetPaneText(strWelcome);
	if(!g_sys.Create())	
	{
//     	return FALSE; 
	}

	char buffer[1024];
	g_pMotion = CHLMotion::CreateMotion();
	if (!g_pMotion->Init(buffer))
	{
		AfxMessageBox(buffer);
		return FALSE;
	}
	g_IO.SetOnline(g_sys.m_bPLCOnline);
	
	if (g_sys.m_bPLCOnline && !g_IO.INPUT_MACHINE_READY())
	{
		AfxMessageBox(_T("the table is not ready"));
	}

	g_ccd.InitCCD();

	/*if (!g_TrumpfLaser.IsConnect())
	{
		int nRtn = 0;
		nRtn = g_TrumpfLaser.Connect();
		if (nRtn)
			OutputDebugString(_T("OPC Server connect ok!"));
		else
			AfxMessageBox(_T("OPC Server connect fail!"));

		nRtn = g_TrumpfLaser.InitLaserPara();
		if (!nRtn)
			OutputDebugString(_T("Init laser para ok!"));
		else
			AfxMessageBox(_T("Init laser para fail!"));
	}
*/

	if (g_sys.m_bUseOpcLaser)
	{
		g_OPCLaser = COpcLaser::CreateObject();
		g_OPCLaser->SetWriteDelay(g_sys.m_nWriteDelay);
		if (!g_OPCLaser->Connect(g_sys.m_szOPCIP))
		{
			AfxMessageBox(_T("Connect OPC Server failed!"));
			return FALSE;
		}

		if (!g_OPCLaser->InitLaserPara())
		{
			AfxMessageBox(_T("Init OPC laser parameter failed!"));
			return FALSE;
		}
	}

	
	m_pMainWnd = NULL;
	delete pDlgSplash;
	pDlgSplash = NULL;

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_HLTYPE,
		RUNTIME_CLASS(CHLDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CHLView));
	AddDocTemplate(pDocTemplate);
	// create main MDI Frame window

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
 	CCommandLineInfo cmdInfo;
 	ParseCommandLine(cmdInfo);
	cmdInfo.m_nShellCommand=CCommandLineInfo::FileNew;
		

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
	{
		delete pDlgSplash;
		return FALSE;
	}

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	pMainFrame->ActivateFrame( SW_SHOW );

	if (-1 == LoadImageProcLib()) 
	{
		return FALSE;
	}

	//工作台回零
	if (!g_sys.m_bPLCOnline || g_IO.INPUT_MACHINE_READY())
		PostMessage(g_pDlgMarkNormalPane->m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_HOME, NULL);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	public:
		
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_strCardHdVer;
	CString m_strCardSwVer;
//	CString m_strReleaseDay;
//	CString m_strVersion;
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
	, m_strCardHdVer(_T(""))
	, m_strCardSwVer(_T(""))
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STATIC_CARDHDVER, m_strCardHdVer);
	DDX_Text(pDX, IDC_STATIC_CARDSWVER, m_strCardSwVer);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CHLApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CHLApp message handlers


CString CHLApp::GetInstallPath()
{
	return g_sys.GetInstallPath();
}

void CHLApp::OnSetTracker()
{
	return;
	CSetTrackerDlg dlg;
	int style;
 	dlg.m_nHandleSize	=GetProfileInt("TRACKER","HANDLESIZE",8);
 	style				=GetProfileInt("TRACKER","STYLE",18);

	if(style & CRectTracker::dottedLine)
		dlg.m_nLine=0;
	else if(style & CRectTracker::solidLine)
		dlg.m_nLine=1;
	else
		dlg.m_nLine=2;

	if(style & CRectTracker::resizeInside)
		dlg.m_nResize=0;
	else 
		dlg.m_nResize=1;

 	if(dlg.DoModal()==IDOK)
 	{
		style=0;
		switch(dlg.m_nLine)
		{
		case 0:
			style^=CRectTracker::dottedLine;
			break;
		case 1:
			style^=CRectTracker::solidLine;
			break;
		}

		if(dlg.m_nResize)
			style^=CRectTracker::resizeOutside;
		else
			style^=CRectTracker::resizeInside;

  		WriteProfileInt("TRACKER","HANDLESIZE",dlg.m_nHandleSize);
 		WriteProfileInt("TRACKER","STYLE",style);

 		//更新所有的视
 		POSITION pos1=GetFirstDocTemplatePosition();
		while(pos1!=NULL)
		{
			CDocTemplate* pDT=GetNextDocTemplate(pos1);
			POSITION pos2=pDT->GetFirstDocPosition();
			while(pos2!=NULL)
			{
				CDocument *pDoc=pDT->GetNextDoc(pos2);
				pDoc->UpdateAllViews(NULL,UPDATE_TRACKER_VIEW);
				pDoc->UpdateAllViews(NULL,UPDATE_TRACKER_PROPERTY);
				pDoc->UpdateAllViews(NULL);
			}
		}
 	}
}


void CHLApp::OnSetHardware() 
{
	return;
	if (!(g_CurUserInfo.dwRight1 & 32))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
//		GetMainWnd()->UpdateWindow();
//		UpdateAllViews();
	}
	
	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
//	if (g_bIsFastMark) 
//	{
//		theApp.m_FastMark.StopFastMark();
//	}

	SHELLEXECUTEINFO  ShExecInfo  =  {0};  
	ShExecInfo.cbSize  =  sizeof(SHELLEXECUTEINFO);  
	ShExecInfo.fMask   =  SEE_MASK_NOCLOSEPROCESS;  
	ShExecInfo.hwnd    =  NULL;  
	ShExecInfo.lpVerb  =  _T("open");
	CString str;
	str.Format("%s%s%s", GetInstallPath(), g_sys.m_strCardName,"\\SystemSet.exe");
	ShExecInfo.lpFile        =  LPCSTR(str); 
	ShExecInfo.lpParameters  = "SystemSet.ini";
	CString str1=GetInstallPath();
	str1 += g_sys.m_strCardName;
	ShExecInfo.lpDirectory  =LPCSTR(str1);   
	ShExecInfo.nShow        =  SW_NORMAL;  
	ShExecInfo.hInstApp     =  NULL;              
	
	BOOL bRunOK=ShellExecuteEx(&ShExecInfo); 
	if (bRunOK)
	{
		DWORD dwExitCode;
		MSG msg;
		while(WaitForSingleObject(ShExecInfo.hProcess,0)!=WAIT_OBJECT_0)
		{
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				if (( WM_PAINT == msg.message ) || (WM_PRINTCLIENT == msg.message))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				WaitMessage();
			}
		}
		GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
		CloseHandle(ShExecInfo.hProcess);
	}
}

BOOL CHLApp::WriteProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dbValue)
{
	CString str;
	str.Format("%f",dbValue);
	return WriteProfileString(lpszSection,lpszEntry,str);
}

double CHLApp::GetProfileDouble(LPCTSTR lpszSection, LPCTSTR lpszEntry, double dbDefault)
{
	CString str;
	str.Format("%f",dbDefault);
	str=GetProfileString(lpszSection,lpszEntry,str);
	return atof(str);
}

void CHLApp::OnOption() 
{
	if (!(g_CurUserInfo.dwRight1 & 32))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	CString strPath = GetInstallPath()+"sysOption\\";
	SetDllDirectory(strPath);
	strPath += "sysOption.dll";

	HMODULE hModule = LoadLibrary(strPath);
	SetDllDirectory(NULL);
	if (hModule == NULL)
	{
		CString strTemp;
		strTemp.Format("Load %s failed!", strPath);
		AfxMessageBox(strTemp);
		return;
	}
	typedef UINT (WINAPI *OPTIONDLGSET)();
	OPTIONDLGSET pFunc = (OPTIONDLGSET)GetProcAddress(hModule, LPSTR(1));
	if (pFunc == NULL)
	{
		CString strTemp;
		strTemp.Format("Load function failed!");
		AfxMessageBox(strTemp);
		FreeLibrary(hModule);
		return;
	}

	if (pFunc()==IDCANCEL)
	{
		FreeLibrary(hModule);
		return;
	}

	
	//g_sys.m_langID = nLangId;
	//g_sys.WriteLang();

	g_sys.ReadSysCfg();
	g_sys.ReadSysPara();

	

	CMDIChildWnd *pChildWnd=((CMDIFrameWnd *)m_pMainWnd)->MDIGetActive();
	if(pChildWnd)
	{
		CHLDoc *pDoc=(CHLDoc *)pChildWnd->GetActiveDocument();
		{
			pDoc->UpdatePerUnit();
		}

		if(pDoc)
		{
			pDoc->UpdateAllViews(NULL);
		}
	}// if (pChildWnd)
	FreeLibrary(hModule);
	return;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	TCHAR filename[_MAX_PATH];
	GetModuleFileName(theApp.m_hInstance, filename,_MAX_PATH);
	DWORD size,dwHandle;
	size=GetFileVersionInfoSize(filename, &dwHandle);
	if(size)
	{
		LPVOID versionData=new char[size];
		GetFileVersionInfo(filename,dwHandle,size,versionData);

		LPVOID textData;
		UINT textSize=0;
		VerQueryValue(versionData, _T("\\StringFileInfo\\080404b0\\FileDescription"), &textData, &textSize);
		SetDlgItemText(IDC_STATIC_SOFTWARE_VERSION, (LPCTSTR)textData);
		delete[] versionData;
	}

	UINT nMainVer=0,nSubVer=0,nSSubVer=0,nCustomVer=0;
	g_DAT.GetHdVer(nMainVer, nSubVer, nSSubVer, nCustomVer);
	m_strCardHdVer.Format("V%d.%d.%d.%d", nMainVer, nSubVer, nSSubVer, nCustomVer);
	
	g_DAT.GetSwVer(nMainVer, nSubVer, nSSubVer, nCustomVer);
	m_strCardSwVer.Format("V%d.%d.%d.%d", nMainVer, nSubVer, nSSubVer, nCustomVer);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CHLApp::ExitInstance() 
{
	AfxOleTerm(FALSE);

	//m_dlgCCD.m_ctrCVX1.Disconnect();
	//m_CcdViewDlg.m_cvXTRL.Disconnect();
	HWND hwnd = FindWindow(NULL,"CCDVIEW");
	if (NULL != hwnd)
	{
		SendMessage( hwnd, WM_CLOSE,0,0);
	}

	if (g_pMotion && g_pMotion->IsInit())
	{
		g_pMotion->WaitMoveFinish(eMaskXYZ);
		// IO_OUTPUT_LOADPT				0
		//	IO_OUTPUT_MARKBLOW			6
		for (int i = 0; i < 7; i ++)
		{
			g_pMotion->OutBit(i, TRUE);
		}
	}

	if (g_sys.m_bUseOpcLaser)
	{
		BOOL bRet = g_OPCLaser->CloseLaser();
		if (!bRet)
		{
			OutputDebugString(_T("写激光触发数据项失败！"));
		}
		g_OPCLaser->ReleaseObject();
		g_OPCLaser = NULL;
	}
	
	CloseHandle(hMark);
	CloseHandle(hStop);
	CloseHandle(hMarkSts);
	CloseHandle(hThread);

	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	if (m_hLib)
	{
		FreeLibrary(m_hLib);
		m_hLib=NULL;
	}

 	if (m_hLangDLL)
 	{
 		AfxFreeLibrary(m_hLangDLL);
 	}

	if (m_hMutexNew != NULL)
	{
		CloseHandle(m_hMutexNew);
		m_hMutexNew = NULL;
	}

	GdiplusShutdown(m_GdiplusToken); 


	return CWinAppEx::ExitInstance();
}

void CHLApp::OnMarkMul() 
{
	if (!(g_CurUserInfo.dwRight1 & 4))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
//		GetMainWnd()->UpdateWindow();
//		UpdateAllViews();
		return;
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
		return;
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
		return;
	}
	
	CHLApp *pApp=(CHLApp *)AfxGetApp();
	CMultiDocTemplate *pDocTem;
	CHLDoc *pDoc;
	POSITION posTem = GetFirstDocTemplatePosition();
	while(posTem != NULL)
	{
		pDocTem = (CMultiDocTemplate *)GetNextDocTemplate(posTem);
		POSITION posDoc = pDocTem->GetFirstDocPosition();
		while(posDoc != NULL)
		{
			pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
			pDoc->PreprocessMark();
			if(!pDoc->OutOfLimit())
			{
				AfxMessageBox(IDS_ERROT_OUTAREA);
				return;
			}
		}
	}

	CMarkMulDocDlg dlg;
	dlg.m_nFreq=GetProfileInt("MULDOC","FREQ",1000);
	
	g_DAT.EnterMarkEnv();
	dlg.DoModal();
	g_DAT.LeaveMarkEnv();

	WriteProfileInt("MULDOC","FREQ",dlg.m_nFreq);
}


void CHLApp::OnFileMuldoc() 
{
	// TODO: Add your command handler code here
	OnFileMulDocOpen();
	return;
	//打开多文档数据
	CString str;
	CString strVer;
	static char szFilter[]="Muldoc File(*.mul)|*.mul||";
	CFileDialog dlg(TRUE,"mul",NULL,OFN_HIDEREADONLY,szFilter);
	
	str.LoadString(IDS_MULDOC_LOAD);
	dlg.m_ofn.lpstrTitle=str;
	if(dlg.DoModal()==IDOK)
	{
		str=dlg.GetPathName();
		CFile f;
		f.Open(str,CFile::modeRead );
		CArchive ar(&f,CArchive::load);

		//恢复数据
		int count;
		ar>>strVer;
		ar>>count;
		for(int i=0;i<count;i++)
		{
			ar>>str;
			CHLDoc *pDoc=(CHLDoc *)OpenDocumentFile(str);
			ar>>pDoc->m_motor.nMove;
			ar>>pDoc->m_motor.nDirection;
			ar>>pDoc->m_motor.nDelay;
			if (strVer.CompareNoCase("2009.03.15")==0)
			{
				ar>>pDoc->m_motor.strIO;
			}
		}

		int freq;
		ar>>freq;
		WriteProfileInt("MULDOC","FREQ",freq);

		ar.Close();
		f.Close(); 
	}
}

void CHLApp::OnMarkStat() 
{
	CMarkdataStatDlg dlg;
	dlg.DoModal();
}

void CHLApp::OnCalibrate() 
{
	/*
	SetDATCard();

	//更新所有的文档及视图
	POSITION pos=GetFirstDocTemplatePosition();
	CDocTemplate *pDocTemplate=GetNextDocTemplate(pos);
	pos=pDocTemplate->GetFirstDocPosition();
	while(pos!=NULL)
	{
		CHLDoc *pDoc=(CHLDoc *)(pDocTemplate->GetNextDoc(pos));
		pDoc->UpdateAllViews(NULL);
	}
	*/

	if (!(g_CurUserInfo.dwRight1 & 64))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return ;
	}

	CDlgCaliSet dlg;
	dlg.DoModal();
}

void CHLApp::SetDATCard()
{
	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
		return;
//		Sleep(1000);
//		GetMainWnd()->UpdateWindow();
//		UpdateAllViews();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	SHELLEXECUTEINFO  ShExecInfo  =  {0};  
	ShExecInfo.cbSize  =  sizeof(SHELLEXECUTEINFO);  
	ShExecInfo.fMask  =  SEE_MASK_NOCLOSEPROCESS;  
	ShExecInfo.hwnd  =  NULL;  
	ShExecInfo.lpVerb  =  _T("open");
	CString strCaliExe;
	strCaliExe.Format("%s%s%s",GetInstallPath(), g_sys.m_strCardName, "\\CaliSoft.exe");
	
	ShExecInfo.lpFile  =  LPCSTR(strCaliExe);
	ShExecInfo.lpParameters  = NULL;
	ShExecInfo.lpDirectory  =  GetInstallPath() + g_sys.m_strCardName;
	ShExecInfo.nShow  =  SW_NORMAL;
	ShExecInfo.hInstApp  =  NULL;
	
//	GetMainWnd()->ShowWindow(SW_HIDE);	
	g_DAT.ExitCard();
//	Sleep(1000);
	BOOL bRunOK=ShellExecuteEx(&ShExecInfo); 
	if (bRunOK)
	{
		DWORD dwExitCode;
		MSG msg;
		while(WaitForSingleObject(ShExecInfo.hProcess,0)!=WAIT_OBJECT_0)
		{
			//还未退出
			if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
			{
				if ( msg.message == WM_PAINT){
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
				WaitMessage();
		}
		GetExitCodeProcess(ShExecInfo.hProcess, &dwExitCode);
		CloseHandle(ShExecInfo.hProcess);
	}
	//Sleep(500);
	g_DAT.InitCard();
}

void g_TESTCUR(double dbCurrent){return;}
// test all grey-current pairs
void g_TESTALL(double* paGreyCurrent, 
			   int nSize)
{
	return;
}
void (g_STOPTEST)()
{
	return;
}
// ctrl card type: 0 - DAT2000; 1 - PCI2000; 2 - PCI3000
UINT g_GETCARDTYPE()
{
	return 2;
}

BOOL CHLApp::PreTranslateMessage(MSG* pMsg) 
{
	if (g_pDlgMarkNormalPane)
	{
		if (g_pDlgMarkNormalPane->m_bDisableOtherWindowInputOnMarking && g_pDlgMarkNormalPane->m_bMarking)
		{
			if (pMsg->message==WM_KEYDOWN || pMsg->message==WM_KEYUP ||	pMsg->message==WM_SYSKEYDOWN || pMsg->message==WM_SYSKEYUP)
				return TRUE;

			if (pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONDBLCLK ||	pMsg->message==WM_RBUTTONDOWN)
			{
				if (pMsg->hwnd!=g_pDlgMarkNormalPane->m_hWnd && GetParent(pMsg->hwnd)!=g_pDlgMarkNormalPane->m_hWnd)
					return TRUE;
			}
		}
	}

	return CWinAppEx::PreTranslateMessage(pMsg);
}

int CHLApp::Run() 
{
	return CWinAppEx::Run();
}

void CHLApp::OnGrayCurrent()
{/*
	TCHAR pExePath[MAX_PATH];
	memset(pExePath, 0, MAX_PATH);
	GetExepath(pExePath);
	
	CString strDllName;
	strDllName.Format("%s",pExePath);
	
#ifdef _DEBUG
	strDllName+=_T("LPSet_D.dll"); 
#else
	LANGID id=GetProfileInt("LANGUAGE","LANG",0x804); //typedef  word LANDID
	if (id==0x804)
	{
		strDllName+=_T("LPSet.dll");
	} 
	else
	{
		strDllName+=_T("LPSet_E.dll");
	}
	
#endif

	HMODULE hLib = LoadLibrary(strDllName);
	
	if(hLib)
	{
		LPLPSet pLPSet = (LPLPSet)GetProcAddress(hLib,"LPSet");
		LPInitLP pInitLP = (LPInitLP)GetProcAddress(hLib,"InitLP");
		
		double paGreyCurrent[256];
		//读文件
		CStdioFile fp;
		CHLApp *pApp=(CHLApp *)AfxGetApp();
		CString strFileName(""),str(""),tmp("");
		strFileName.Format("%s%s", pExePath, "GrayCurrent.dat");
		int nCount = 0;
		int nGray  = 0;
		double nValue(0.0);
		
		BOOL bReadOK=FALSE;
		if(fp.Open(strFileName,CFile::modeRead))
		{
			//数组的度数是由小到大排列
			//高16位为角度值，低16位为具体值的百分比%
			int i=0;
			BOOL bWhileOK=TRUE;
			while(fp.ReadString(str))
			{
				nCount = str.Find('@',0);
				if(nCount==-1)
					continue;
				tmp = str.Left(nCount);
				nGray = atoi(tmp);
				if (nGray>=256||nGray<0)
				{
					fp.Close();
					bWhileOK=FALSE;
					break;
				}
				tmp = str.Mid(nCount+1);
				nValue = atof(tmp);
				paGreyCurrent[i]=nValue;
				if (i>=255)
				{
					break;
				}
				i++;
			}
			if (bWhileOK)
			{
				for (;i<256;i++)
				{
					paGreyCurrent[i]=0.2*i;
				}
				bReadOK=TRUE;
				fp.Close();
			}
			
		}
		else
		{
			AfxMessageBox(IDS_PROMPT_OPENGRAYSCALE_FAIL);
		}
		if (!bReadOK)
		{
			for (int i=0;i<256;i++)
			{
				paGreyCurrent[i]=0.2*i;
			}
		}
		
		double dTest[]={g_sys.m_dbMinCurrent,g_sys.m_dbMaxCurrent,g_sys.m_dbMinCurrent,g_sys.m_dbMaxCurrent};
		BOOL bRtn=(pLPSet) (paGreyCurrent, 
			256, 
			g_TESTCUR,
			g_TESTALL,
			g_STOPTEST,
			g_GETCARDTYPE,
			dTest
			);
		if (bRtn)
		{
			//写文件
			if (fp.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
			{
				for (int i=0;i<256;i++)
				{
					str.Format("%i@%.2f\n",i,paGreyCurrent[i]);
					fp.WriteString(str);
				}
				fp.Close();
				g_DAT.FlashGrayCurrent();
			}
			else
			{
				AfxMessageBox(IDS_PROMPT_SAVEGRAYSCALE_FAIL);
			}
		}
		FreeLibrary(hLib);
		return;
	}
*/
	return;
}

UINT CHLApp::LoadImageProcLib()
{
	TCHAR pExePath[MAX_PATH];
	memset(pExePath, 0, MAX_PATH);
	GetExepath(pExePath);

	CString strDllName("");
	strDllName.Format("%s",pExePath);
#ifdef _DEBUG
		strDllName += _T("ImageProc_D.DLL"); 
#else
		strDllName += _T("ImageProc.DLL");
#endif

	m_hLib = LoadLibrary(strDllName);
	if (NULL == m_hLib)
	{
		return -1;
	}

	LoadDib    = (LPLOADDIB)GetProcAddress(m_hLib,"LoadDib");
	ProcDib    = (LPPROCDIB)GetProcAddress(m_hLib,"ProcDib");
	ProcGrayDib= (LPPROCGRAYDIB)GetProcAddress(m_hLib,"ProcDib_Gray");
	DelDib     = (LPDELDIB)GetProcAddress(m_hLib,"DelDib");

	if (LoadDib&&ProcDib&&ProcGrayDib&&DelDib)
	{
		return 0; //成功
	}
	else
	{
		return -1; 
	}
}

UINT CHLApp::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry,
	int nDefault)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	return ::GetPrivateProfileInt(lpszSection, lpszEntry, nDefault,
		m_strAppIni);
}

CString CHLApp::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
	LPCTSTR lpszDefault)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	if (lpszDefault == NULL)
		lpszDefault = _T("");	// don't pass in NULL
	TCHAR szT[4096];
	DWORD dw = ::GetPrivateProfileString(lpszSection, lpszEntry,
		lpszDefault, szT, _countof(szT), m_strAppIni);
	ASSERT(dw < 4095);
	return szT;
}

BOOL CHLApp::GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry,
	BYTE** ppData, UINT* pBytes)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	ASSERT(ppData != NULL);
	ASSERT(pBytes != NULL);
	*ppData = NULL;
	*pBytes = 0;

	CString str = GetProfileString(lpszSection, lpszEntry, NULL);
	if (str.IsEmpty())
		return FALSE;
	ASSERT(str.GetLength()%2 == 0);
	INT_PTR nLen = str.GetLength();
	*pBytes = UINT(nLen)/2;
	*ppData = new BYTE[*pBytes];
	for (int i=0;i<nLen;i+=2)
	{
		(*ppData)[i/2] = (BYTE)
			(((str[i+1] - 'A') << 4) + (str[i] - 'A'));
	}
	return TRUE;
}

BOOL CHLApp::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry,
	int nValue)
{
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);

	TCHAR szT[16];
	_stprintf_s(szT, _countof(szT), _T("%d"), nValue);
	return ::WritePrivateProfileString(lpszSection, lpszEntry, szT,
		m_strAppIni);
}

BOOL CHLApp::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
	LPCTSTR lpszValue)
{
	ASSERT(lpszSection != NULL);
	return ::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue,
		m_strAppIni);
}

BOOL CHLApp::WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry,
	LPBYTE pData, UINT nBytes)
{
	ASSERT(lpszSection != NULL);
	// convert to string and write out
	LPTSTR lpsz = new TCHAR[nBytes*2+1];
	UINT i;
	for (i = 0; i < nBytes; i++)
	{
		lpsz[i*2] = (TCHAR)((pData[i] & 0x0F) + 'A'); //low nibble
		lpsz[i*2+1] = (TCHAR)(((pData[i] >> 4) & 0x0F) + 'A'); //high nibble
	}
	lpsz[i*2] = 0;

	BOOL bResult = WriteProfileString(lpszSection, lpszEntry, lpsz);
	delete[] lpsz;
	return bResult;
}

void CHLApp::OnMenuFontFileNew()
{
	// TODO: Add your command handler code here
}


void CHLApp::OnUpdateMenuFontFileNew(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CHLApp::OnMenuFontFileOpen()
{
	// TODO: Add your command handler code here
}


void CHLApp::OnUpdateMenuFontFileOpen(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CHLApp::OnMenuFontFileSave()
{
	// TODO: Add your command handler code here
}


void CHLApp::OnUpdateMenuFontFileSave(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CHLApp::OnMenuFontFileSaveAs()
{
	// TODO: Add your command handler code here
}


void CHLApp::OnUpdateMenuFontFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CHLApp::OnUpdateAgingTest(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(g_CurUserInfo.dwRight1 & 1024);
}

#define INI_ITEM_SAFE_DOOR _T("HARDWARE"), _T("USE_SAFE_DOOR")

void CHLApp::OnSetWorkbench()
{
	// TODO: 在此添加命令处理程序代码

	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}

	CHLApp *pApp = (CHLApp*)AfxGetApp();
//	if (!pApp->CurRight(RT_WB_PARA))
//		return;

	CDlgSetWorkbench dlg;
	g_pMotion->GetPara("AXIS_0", "FastVel", dlg.m_dbFastVel);
	g_pMotion->GetPara("AXIS_0", "SlowVel", dlg.m_dbSlowVel);
	g_pMotion->GetPara("XY", "LoadPositionX", dlg.m_dbLoadPosX);
	g_pMotion->GetPara("XY", "LoadPositionY", dlg.m_dbLoadPosY);
	g_pMotion->GetPara("XY", "UnloadPositionX", dlg.m_dbUnloadPosX);
	g_pMotion->GetPara("XY", "UnloadPositionY", dlg.m_dbUnloadPosY);
	g_pMotion->GetPara("AXIS_0", "Min", dlg.m_dbLeftPtX);
	g_pMotion->GetPara("AXIS_1", "Min", dlg.m_dbLeftPtY);
	g_pMotion->GetPara("AXIS_0", "Max", dlg.m_dbRightPtX);
	g_pMotion->GetPara("AXIS_1", "Max", dlg.m_dbRightPtY);
	g_pMotion->GetPara("XY", "LoadDis", dlg.m_dbLoadDis);
	g_pMotion->GetPara("XY", "UnLoadDis", dlg.m_dbUnLoadDis);

	double dbScan,dbMove,dbScanId,dbPosTicps;
	g_pMotion->GetPara("XY", "Scan", dbScan);
	g_pMotion->GetPara("XY", "bMove", dbMove);
	g_pMotion->GetPara("XY", "ScanId", dbScanId);
	g_pMotion->GetPara("XY", "PosTicps", dbPosTicps);

	dlg.m_dbCloseDoorTime = g_ParaIni.m_nCloseDoorTime/1000;

	double bDocProGress;
	g_pMotion->GetPara("XY", "bDocProGress", bDocProGress);
	dlg.m_bDocProGress = static_cast<BOOL>(bDocProGress);

	g_sys.m_bUseSafeDoor = g_sys.GetProfileInt(INI_ITEM_SAFE_DOOR, 1);
	dlg.m_bUseSafeDoor  =g_sys.m_bUseSafeDoor;	

	dlg.m_bPosTicps = static_cast<BOOL>(dbPosTicps);
	dlg.m_bMove = static_cast<BOOL>(dbMove);


	dlg.m_bUseCutBlow = g_ParaIni.m_bUseCutBlow;

	dlg.m_nMotionDelayForCCD  = g_ParaIni.m_nMotionDelayForCCD;
	dlg.m_nMotionDelayForMark = g_ParaIni.m_nMotionDelayForMark;
	dlg.m_dbRefPointX   = g_ParaIni.m_dbRefPointX;
	dlg.m_dbRefPointY   = g_ParaIni.m_dbRefPointY;
	dlg.m_dbMarkToCCD1X = g_ParaIni.m_dbMarkToCCD1X;
	dlg.m_dbMarkToCCD1Y = g_ParaIni.m_dbMarkToCCD1Y;
	dlg.m_dbMarkToCCD2X = g_ParaIni.m_dbMarkToCCD2X;
	dlg.m_dbMarkToCCD2Y = g_ParaIni.m_dbMarkToCCD2Y;


	dlg.m_dbPowerCheckX = g_ParaIni.m_dbPowerCheckX;
	dlg.m_dbPowerCheckY = g_ParaIni.m_dbPowerCheckY;
	dlg.m_dbPowerCheckZ = g_ParaIni.m_dbPowerCheckZ;
	dlg.m_dbAutoOffsetDeviationMax = g_ParaIni.m_dbAutoOffsetDeviationMax;
	dlg.m_dbAutoOffsetDeviationMin = g_ParaIni.m_dbAutoOffsetDeviationMin;
	dlg.m_dbWorkbenchFixtureAngle = g_ParaIni.m_dbWorkbenchFixtureAngle;


	dlg.m_dbCrossInCCDPosX = g_pMotion->GetPos(eMaskX);
	dlg.m_dbCrossInCCDPosY = g_pMotion->GetPos(eMaskY);

	dlg.m_dbWorkPosX = g_ParaIni.m_dbWorkPosX;
	dlg.m_dbWorkPosY = g_ParaIni.m_dbWorkPosY;

	if (dlg.DoModal()==IDOK)
	{
		g_pMotion->WritePara("AXIS_0", "FastVel", dlg.m_dbFastVel);
		g_pMotion->WritePara("AXIS_0", "SlowVel", dlg.m_dbSlowVel);
		g_pMotion->WritePara("AXIS_1", "FastVel", dlg.m_dbFastVel);
		g_pMotion->WritePara("AXIS_1", "SlowVel", dlg.m_dbSlowVel);
		g_pMotion->WritePara("XY", "LoadPositionX", dlg.m_dbLoadPosX);
		g_pMotion->WritePara("XY", "LoadPositionY", dlg.m_dbLoadPosY);
		g_pMotion->WritePara("XY", "UnloadPositionX", dlg.m_dbUnloadPosX);
		g_pMotion->WritePara("XY", "UnloadPositionY", dlg.m_dbUnloadPosY);
		g_pMotion->WritePara("XY", "LoadDis", dlg.m_dbLoadDis);
		g_pMotion->WritePara("XY", "UnLoadDis", dlg.m_dbUnLoadDis);

		g_pMotion->WritePara("AXIS_0", "Min", dlg.m_dbLeftPtX);
		g_pMotion->WritePara("AXIS_1", "Min", dlg.m_dbLeftPtY);
		g_pMotion->WritePara("AXIS_0", "Max", dlg.m_dbRightPtX);
		g_pMotion->WritePara("AXIS_1", "Max", dlg.m_dbRightPtY);
		double dbMove = dlg.m_bMove;
		g_pMotion->WritePara("XY", "bMove",dbMove);


		double bDocProGress = dlg.m_bDocProGress;
		g_pMotion->WritePara("XY", "bDocProGress", bDocProGress);

		g_ParaIni.m_bUseCutBlow = dlg.m_bUseCutBlow;

		g_sys.m_bUseSafeDoor = dlg.m_bUseSafeDoor;
		g_sys.WriteProfileInt(INI_ITEM_SAFE_DOOR, g_sys.m_bUseSafeDoor);

		double dbPosTicps = dlg.m_bPosTicps;
		g_pMotion->WritePara("XY", "PosTicps", dbPosTicps);

		g_pMotion->LoadPara();


		g_ParaIni.m_nCloseDoorTime = (int) dlg.m_dbCloseDoorTime;
		if (g_ParaIni.m_nCloseDoorTime > 60)
			g_ParaIni.m_nCloseDoorTime = 60;
		g_ParaIni.m_nCloseDoorTime *= 1000;
		
		g_ParaIni.m_nMotionDelayForCCD = dlg.m_nMotionDelayForCCD;
		g_ParaIni.m_nMotionDelayForMark = dlg.m_nMotionDelayForMark;
		g_ParaIni.m_dbRefPointX   = dlg.m_dbRefPointX;
		g_ParaIni.m_dbRefPointY   = dlg.m_dbRefPointY;
		g_ParaIni.m_dbMarkToCCD1X = dlg.m_dbMarkToCCD1X;
		g_ParaIni.m_dbMarkToCCD1Y = dlg.m_dbMarkToCCD1Y;
		g_ParaIni.m_dbMarkToCCD2X = dlg.m_dbMarkToCCD2X;
		g_ParaIni.m_dbMarkToCCD2Y = dlg.m_dbMarkToCCD2Y;		
		g_ParaIni.m_dbPowerCheckX = dlg.m_dbPowerCheckX;
		g_ParaIni.m_dbPowerCheckY = dlg.m_dbPowerCheckY;
		g_ParaIni.m_dbPowerCheckZ = dlg.m_dbPowerCheckZ;
		g_ParaIni.m_dbAutoOffsetDeviationMax = dlg.m_dbAutoOffsetDeviationMax;
		g_ParaIni.m_dbAutoOffsetDeviationMin	= dlg.m_dbAutoOffsetDeviationMin;
		g_ParaIni.m_dbWorkbenchFixtureAngle = dlg.m_dbWorkbenchFixtureAngle;
		g_ParaIni.m_dbWorkPosX = dlg.m_dbWorkPosX;
		g_ParaIni.m_dbWorkPosY = dlg.m_dbWorkPosY;
		g_ParaIni.SavePara();

		g_pDlgMarkNormalPane->ResetProductDocPara(0x08);
		g_pDlgMarkNormalPane->m_pDoc->UpdateAllViews(NULL);
	}
}


void CHLApp::OnCcdOperation()
{
	// TODO: 在此添加命令处理程序代码

	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}
	if (!g_pMotion->IsHomed())
	{
		AfxMessageBox("The workbench is not home");
		return;
	}
	if ((g_pMotion->GetStatus(eMaskX)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskY)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskZ)&eStatusBitMoving))
	{
		AfxMessageBox("The workbench is Moving");
		return;
	}

	CHLApp *pApp = (CHLApp*)AfxGetApp();
//	if (!pApp->CurRight(RT_CCD))
//		return;

	static CCCDSettingDlg *pCCDSetDlg=NULL;

	if(pCCDSetDlg && ::IsWindow(pCCDSetDlg->m_hWnd))
	{
		pCCDSetDlg->SetActiveWindow();
	}
	else
	{	
		pCCDSetDlg=new CCCDSettingDlg();
		pCCDSetDlg->Create(CCCDSettingDlg::IDD);
		pCCDSetDlg->CenterWindow();
	}
	pCCDSetDlg->SetTable(2);
	pCCDSetDlg->ShowWindow(SW_RESTORE);
}

void HL_PosCallback(double &dbX, double &dbY)
{
	dbX = g_pMotion->GetPos(eMaskX);
	dbY = g_pMotion->GetPos(eMaskY);
}

BOOL HL_MoveCallback(double dbX, double dbY)
{
	g_pMotion->LnXY(dbX, dbY);
	g_pMotion->MovedDelay();
	return TRUE;
}

BOOL HL_CCDCallback(double &dbX, double &dbY, BOOL &bPrecisionOK)
{
	if (g_ccd.CCDLocate())
	{
		double dbCCDX=0.0, dbCCDY=0.0, dbAngle=0.0;
		g_ccd.GetTrigMM(TRUE, dbCCDX, dbCCDY, dbAngle);
		bPrecisionOK = (g_ccd.GetCaptPrecision(TRUE)<g_ParaIni.m_dbCCD1Precision);

		dbX -= dbCCDX;
		dbY -= dbCCDY;
		return TRUE;
	}
	return FALSE;
}

void CHLApp::OnGridCali()
{
	// TODO: 在此添加命令处理程序代码

	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}


	if (!g_pMotion->IsHomed())
	{
		AfxMessageBox("The workbench is not home");
		return;
	}
	if ((g_pMotion->GetStatus(eMaskX)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskY)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskZ)&eStatusBitMoving))
	{
		AfxMessageBox("The workbench is Moving");
		return;
	}
	CHLApp *pApp = (CHLApp*)AfxGetApp();
//	if (!pApp->CurRight(RT_GRID))
//		return;

	WBGC_ShowGridDialog(HL_PosCallback, HL_MoveCallback, HL_CCDCallback);

}




void CHLApp::OnCcdView()
{
	// TODO: 在此添加命令处理程序代码
	HWND hwnd = FindWindow(NULL,"CCDVIEW");
	if (NULL != hwnd)
	{
		::SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
	}
	else
	{
		TCHAR szCommandLine[MAX_PATH] = _T("CCDVIEW.exe");
		LPCTSTR strErrorTip =  _T("CV-X100SeriesActiveXControl No install Or CCDVIEW Not Exist.");
		if (g_sys.m_bDEV_3030B)
		{
			_tcscpy_s(szCommandLine, "CCDVIEW_F.exe");
			strErrorTip = "CCDVIEW_F.exe does not exist or was unable to start correctly.";
		}

		STARTUPINFO si;
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		si.dwFlags=STARTF_USESHOWWINDOW;
		si.wShowWindow=SW_HIDE;
		PROCESS_INFORMATION pi;
		ZeroMemory( &pi, sizeof(pi) );
		if( !CreateProcess( NULL,   // No module name (use command line). 
			szCommandLine, // Command line. 
			NULL,             // Process handle not inheritable. 
			NULL,             // Thread handle not inheritable. 
			FALSE,            // Set handle inheritance to FALSE. 
			0,                // No creation flags. 
			NULL,             // Use parent's environment block. 
			NULL,             // Use parent's starting directory. 
			&si,              // Pointer to STARTUPINFO structure.
			&pi)             // Pointer to PROCESS_INFORMATION structure.
			) 
		{
			AfxMessageBox(strErrorTip);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}

		LONG lWaitTime = 0;
		while(TRUE)
		{
			Sleep(10);

			hwnd = FindWindow(NULL,"CCDVIEW");
			if (hwnd != NULL)
			{
				::SetWindowPos(hwnd,HWND_TOPMOST,0,0,0,0,SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
				break;
			}
			if (++lWaitTime > 600)
				break;
		}

		if (NULL == hwnd)
			AfxMessageBox(strErrorTip);
	}
}

void CHLApp::OnFileMulDocOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString str, strDirectory, strPath;
	static char szFilter[]="Muldoc File(*.mul)|*.mul||";
	CFileDialog dlg(TRUE,"mul",NULL,OFN_HIDEREADONLY,szFilter,0,FALSE);

	str.LoadString(IDS_MULDOC_LOAD);
	dlg.m_ofn.lpstrTitle=str;
	if(dlg.DoModal()==IDOK)
	{
		str=dlg.GetPathName();
		strDirectory = str.Left(str.ReverseFind('\\')+1);

		CStringArray arrDoc;
		FILE* file = fopen(str, "r");
		CMySerialize ar(file);		
		int count;
		ar>>str;
		ar>>count;
		for(int i=0;i<count;i++)
		{
			ar>>str;
			strPath = strDirectory + str;	
			arrDoc.Add(strPath);
		}
		fclose(file);

		CFileStatus status;
		for (int i=0; i<count; i++)
		{
			if (!CFile::GetStatus(arrDoc.GetAt(i), status))
			{
				str.Format("找不到文件!\n%s", arrDoc.GetAt(i));
				AfxMessageBox(str);	
				return;
			}
		}
		
		CloseAllDocuments(FALSE);
		for (int i=0; i<count; i++)
			OpenDocumentFile(arrDoc.GetAt(i));

		g_arrMulDoc.Copy(arrDoc);
	}
}


void CHLApp::OnFileMulDocSave()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	CArray<CHLDoc*> arrDoc;

	CHLDoc *pDoc;
	CMultiDocTemplate *pDocTem;
	POSITION posTem = GetFirstDocTemplatePosition();
	while(posTem != NULL)
	{
		pDocTem = (CMultiDocTemplate *)GetNextDocTemplate(posTem);
		POSITION posDoc = pDocTem->GetFirstDocPosition();
		while(posDoc != NULL)
		{
			pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
			if(pDoc->GetPathName().IsEmpty())
			{
				str.LoadString(IDS_MULDOC_NOPATH);
				AfxMessageBox(str,MB_ICONSTOP);
				return;
			}

			arrDoc.Add(pDoc);
		}
	}

	int count=arrDoc.GetSize();
	if (count==0)
		return;
	

	//保存多文档数据
	static char szFilter[]="Muldoc File(*.mul)|*.mul||";
	CFileDialog dlg(FALSE,"mul",NULL,OFN_HIDEREADONLY,szFilter, 0, FALSE);
	
	str.LoadString(IDS_MULDOC_SAVE);
	dlg.m_ofn.lpstrTitle=str;
	if(dlg.DoModal()==IDOK)
	{
		str=dlg.GetPathName();
		g_arrMulDoc.RemoveAll();

		FILE* file = fopen(str, "w");
		CMySerialize ar(file);

		str="V1";
		ar<<str;
		ar<<count;
		for(int i=0;i<count;i++)
		{
			pDoc=(CHLDoc *)arrDoc.GetAt(i);
			ar<<pDoc->GetTitle();
			g_arrMulDoc.Add(pDoc->GetPathName());
		}

		fclose(file);
	}
}


void CHLApp::OnChangedoc()
{
	// TODO: 在此添加命令处理程序代码
	CString strPath;
	CString strFilter = "HS File(*.hs)|*.hs|All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE,NULL,strPath,OFN_HIDEREADONLY,strFilter);
	if(dlg.DoModal()==IDOK)
	{
		strPath = dlg.GetPathName();
		CString strNewfile = "temp.hs";
		if (!CopyFile(strPath,strNewfile,TRUE))
		{
			AfxMessageBox("Error");
		}
		CStdioFile fReadtxt,fWritetxt;
		BOOL bReadOpen = fReadtxt.Open(strNewfile,CFile::modeRead|CFile::modeNoTruncate);
		BOOL bWriteOpen = fWritetxt.Open(strPath,CFile::modeWrite|CFile::modeNoTruncate);
		if (bReadOpen  && bWriteOpen)
		{
			int nLength = static_cast<int>(fReadtxt.GetLength());
			CString strData;
			for (int i = 0; i < nLength; i++)
			{
				fReadtxt.ReadString(strData);
				if (strData == "T_B")
				{
					fWritetxt.WriteString((LPTSTR)("GROUP_B\r\n"));
					fWritetxt.WriteString((LPTSTR)("100\r\n"));
					fWritetxt.WriteString((LPTSTR)("0\r\n"));
					fWritetxt.WriteString((LPTSTR)("101\r\n"));
					fWritetxt.WriteString((LPTSTR)("\r\n"));
					fWritetxt.WriteString((LPTSTR)("T_B\r\n"));
				}
				else if (strData == "PRO_E")
				{

					fWritetxt.WriteString(strData);
					fWritetxt.WriteString((LPTSTR)("\r\n"));
					fWritetxt.WriteString((LPTSTR)("GROUP_E\r\n"));
				}
				else
				{
					fWritetxt.WriteString(strData);
					fWritetxt.WriteString((LPTSTR)("\r\n"));
				}
			}
			fReadtxt.Close();
			fWritetxt.Close();
			DeleteFile(strNewfile);
		}
	}
	OpenDocumentFile(strPath);
}


void CHLApp::OnPowerTest()
{
	// TODO: 在此添加命令处理程序代码
	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}
	CDlgPowerTest dlg;
	dlg.DoModal();
}


BOOL CHLApp::OnDDECommand(LPTSTR lpszCommand)
{
	//if (CWinApp::OnDDECommand(lpszCommand))
	//	return TRUE;

	// Handle any DDE commands recognized by your application
	// and return TRUE.  See implementation of CWinApp::OnDDEComand
	// for example of parsing the DDE command string.
	

	// Return FALSE for any DDE commands you do not handle.
	return FALSE;
}



void CHLApp::OnMatrixDocSet()
{
	// TODO: 在此添加命令处理程序代码

	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}

	CMatrixDoc dlg;
	dlg.DoModal();
}


void CHLApp::OnSetDatabase()
{
	// TODO: 在此添加命令处理程序代码


	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}

	g_para._Database = g_ParaIni.GetDbDatabase();
	g_para._Server = g_ParaIni.GetDbServer();
	g_para._User = g_ParaIni.GetDbLoginUser();
	g_para._Pwd = g_ParaIni.GetDbLoginPwd();
	g_para.nDriverType = g_ParaIni.GetDbDataType();

	CDataSourceDlg dlg;
	dlg.m_Para = g_para;
	if (IDOK==dlg.DoModal())
	{
		g_para = dlg.m_Para;

		g_ParaIni.SetDbDatabase((LPCTSTR)g_para._Database);
		g_ParaIni.SetDbServer((LPCTSTR)g_para._Server);
		g_ParaIni.SetDbLoginUser((LPCTSTR)g_para._User);
		g_ParaIni.SetDbLoginPwd((LPCTSTR)g_para._Pwd );
		g_ParaIni.SetDbDataType(g_para.nDriverType);
	}
}


void CHLApp::OnAgingTest()
{
	// TODO: 在此添加命令处理程序代码
	CDlgAgingTest dlg;
	dlg.DoModal();
}



