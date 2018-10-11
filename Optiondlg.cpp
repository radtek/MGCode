// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSystem g_sys;

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

void FlushPropToUI(void*pMap, DWORD dwProID)
{
	if (NULL == pMap)
	{
		return;
	}

	std::map<string,string>::iterator mit;
	std::map<string,string> *pMapProp = (std::map<string,string>*)pMap;
	if( (mit = pMapProp->find("PointFilterPrecis")) != pMapProp->end() )
	{
		char szTemp[64];
		sprintf(szTemp, "%f",g_SysCfg.m_dbDotFilterPrecis);
		mit->second     = szTemp;
	}
	if( (mit = pMapProp->find("PointPrecisMoveStep")) != pMapProp->end() )
	{
		char szTemp[64];
		sprintf(szTemp, "%f",g_SysCfg.m_dbDotMoveStepLen);
		mit->second     = szTemp;
	}
	if( (mit = pMapProp->find("MICRO")) != pMapProp->end() )
	{
		char szTemp[64];
		sprintf(szTemp, "%f",g_SysCfg.m_dbMicro);
		mit->second     = szTemp;
	}
	if( (mit = pMapProp->find("CLICK")) != pMapProp->end() )
	{
		char szTemp[64];
		sprintf(szTemp, "%f",g_SysCfg.m_dbClick);
		mit->second     = szTemp;
	}
	if( (mit = pMapProp->find("UNDO")) != pMapProp->end() )
	{
		char szTemp[64];
		sprintf(szTemp, "%d",g_SysCfg.m_nUndoCount);
		mit->second     = szTemp;
	}
	if( (mit = pMapProp->find("ONETOTWO")) != pMapProp->end() )
	{
		char szTemp[64];
		sprintf(szTemp, "%f",g_SysCfg.m_dbOneToTwo);
		mit->second     = szTemp;
	}
	if( (mit = pMapProp->find("ExtendSpace")) != pMapProp->end() )
	{
		char szTemp[64];
		sprintf(szTemp, "%f",g_SysCfg.m_dbExtendSpace);
		mit->second     = szTemp;
	}
	if( (mit = pMapProp->find("KillAngleSpace")) != pMapProp->end() )
	{
		char szTemp[64];
		sprintf(szTemp, "%f",g_SysCfg.m_dbKillAngleSpace);
		mit->second     = szTemp;
	}
	return;
}

void FlushPropToDoc(void*pMap, DWORD dwProID)
{
	if (NULL == pMap)
	{
		return;
	}


	std::map<string, string> *pMapProp = (std::map<string, string>*)pMap;
	std::map<string, string>::iterator mit;
	if( (mit = pMapProp->find("PointFilterPrecis")) != pMapProp->end() )
	{
		string strValue = mit->second;
		g_SysCfg.m_dbDotFilterPrecis = atof(strValue.data());
	}
	if( (mit = pMapProp->find("PointPrecisMoveStep")) != pMapProp->end() )
	{
		string strValue = mit->second;
		g_SysCfg.m_dbDotMoveStepLen = atof(strValue.data());
	}
	if( (mit = pMapProp->find("MICRO")) != pMapProp->end() )
	{
		string strValue = mit->second;
		g_SysCfg.m_dbMicro = atof(strValue.data());
	}
	if( (mit = pMapProp->find("CLICK")) != pMapProp->end() )
	{
		string strValue = mit->second;
		g_SysCfg.m_dbClick = atof(strValue.data());
	}
	if( (mit = pMapProp->find("UNDO")) != pMapProp->end() )
	{
		string strValue = mit->second;
		g_SysCfg.m_nUndoCount = atoi(strValue.data());
	}
	if( (mit = pMapProp->find("ONETOTWO")) != pMapProp->end() )
	{
		string strValue = mit->second;
		g_SysCfg.m_dbOneToTwo = atof(strValue.data());
	}
	if( (mit = pMapProp->find("ExtendSpace")) != pMapProp->end() )
	{
		string strValue = mit->second;
		g_SysCfg.m_dbExtendSpace = atof(strValue.data());
	}
	if( (mit = pMapProp->find("KillAngleSpace")) != pMapProp->end() )
	{
		string strValue = mit->second;
		g_SysCfg.m_dbKillAngleSpace = atof(strValue.data());
	}
// 	if( (mit = pMapProp->find("ScaleX")) != pMapProp->end() )
// 	{
// 		string strValue = mit->second;
// 		g_SysCfg.m_dbRedScaleX = atof(strValue.data());
// 	}
// 	if( (mit = pMapProp->find("ScaleY")) != pMapProp->end() )
// 	{
// 		string strValue = mit->second;
// 		g_SysCfg.m_dbRedScaleY = atof(strValue.data());
// 	}

	return;
}


COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionDlg)
	

	m_hMarkPropModule = NULL;

	CreateObjPropertyCtrl = NULL;
	GetKeyList            = NULL;
	FlushToPropList       = NULL;
	
	//}}AFX_DATA_INIT
}


void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

BOOL COptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	// TODO: Add extra initialization here
	LoadMarkerPropLib();

	CreateSysPropList();
	FlushToPropList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COptionDlg::LoadMarkerPropLib() 
{
	if (m_hMarkPropModule != NULL)
	{
		FreeLibrary(m_hMarkPropModule);
		m_hMarkPropModule = NULL;
	}

	char szPath[MAX_PATH];
	GetExepath(szPath);
	char szFileName[256];
	

#ifdef _DEBUG
	sprintf(szFileName, "%s%s", szPath, "HLSysPara_D.dll");
#else
	sprintf(szFileName, "%s%s", szPath, "HLSysPara.dll");
#endif
	m_hMarkPropModule =  LoadLibrary(szFileName);
	if (NULL == m_hMarkPropModule)
	{
		#ifdef _DEBUG
		AfxMessageBox("Load Library 'HLSysPara_D.dll' Error!");
		#else
		AfxMessageBox("Load Library 'HLSysPara.dll' Error!");
		#endif
		return FALSE;
	}
	
	CreateObjPropertyCtrl	=	(pCreateObjPropertyCtrl)GetProcAddress(m_hMarkPropModule, "CreateObjPropertyCtrl");     								
    GetKeyList	        	=	(pGetKeyList)GetProcAddress(m_hMarkPropModule, "GetKeyList");
	FlushToPropList         =   (pFlushToPropList)GetProcAddress(m_hMarkPropModule, "FlushToPropList");

	if ((NULL == CreateObjPropertyCtrl) 
		|| (NULL == GetKeyList) 
		|| (NULL == FlushToPropList)
		)
	{
		AfxMessageBox("Load Fuction Error!");
		return FALSE;
	}
	
	return TRUE;
}

BOOL UpdatePorpertyList(void*pMap, DWORD &dwProID)
{	
	// 把文档中的值刷新到界面
	if ((pMap != NULL) && (ENU_EXIT_FLUSH_UI == dwProID))
	{
		FlushPropToUI(pMap, dwProID);
	}

	// 把界面上的值刷新到文档中
	if ((pMap != NULL) && ((ENU_EXIT_OK == dwProID) || (ENU_EXIT_APPLAY == dwProID)))
	{
		FlushPropToDoc(pMap, dwProID);
	}
	
	// 退出时处理
	if ((ENU_EXIT_OK == dwProID) || (ENU_EXIT_CANCEL == dwProID))
	{
		HWND   hwnd=FindWindow(NULL ,"系统选项");   
        PostMessage(hwnd,WM_CLOSE,0,0);
	}

	return TRUE;
}

void COptionDlg::CreateSysPropList()
{
	char szPath[MAX_PATH];
	GetExepath(szPath);
	char szFileName[256];
	sprintf(szFileName, "%sconfig\\%s", szPath, "SysCfg.cfg");
	
    CWnd* pWnd = CreateObjPropertyCtrl(WS_CHILD,  szFileName
		                           , NULL, this->GetSafeHwnd(), UpdatePorpertyList);

	return;
}


void COptionDlg::OnClose() 
{
	if (m_hMarkPropModule != NULL)
	{
		FreeLibrary(m_hMarkPropModule);
		m_hMarkPropModule = NULL;
	}
	
	CDialog::OnClose();
}
