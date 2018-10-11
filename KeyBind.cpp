// KeyBind.cpp: implementation of the CKeyBind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "KeyBind.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#pragma warning(disable : 4244)

CKeyBind::CKeyBind()
{
	m_hAccel = NULL;
	RefreshBindSet();
}

CKeyBind::~CKeyBind()
{
	if (m_hAccel != NULL)
	{
		DestroyAcceleratorTable(m_hAccel);
	}
	while (!m_keyBindSet.IsEmpty())
	{
		delete m_keyBindSet.GetHead();
		m_keyBindSet.RemoveHead();
	}
}

int CKeyBind::DoModal()
{
	CString strFilePath;
	char strTemp[1024];
	GetModuleFileName(NULL, strTemp, 1024);
	strFilePath = strTemp;
	int index = strFilePath.ReverseFind('\\');
	strFilePath = strFilePath.Left(index+1);

	SHELLEXECUTEINFO  ShExecInfo  =  {0};  
	ShExecInfo.cbSize  =  sizeof(SHELLEXECUTEINFO);  
	ShExecInfo.fMask  =  SEE_MASK_NOCLOSEPROCESS;  
	ShExecInfo.hwnd  =  NULL;  
	ShExecInfo.lpVerb  =  _T("open");
	CString strKeyBindExe;
	strKeyBindExe.Format("%s%s",strFilePath,"KeyBind.exe");
	ShExecInfo.lpFile  =  LPCSTR(strKeyBindExe);                          
	ShExecInfo.lpParameters  = NULL;              
	ShExecInfo.lpDirectory  =  NULL;  
	ShExecInfo.nShow  =  SW_NORMAL;  
	ShExecInfo.hInstApp  =  NULL;              
	
	BOOL bRunOK=ShellExecuteEx(&ShExecInfo); 
	if (bRunOK)
	{
		DWORD dwExitCode;
		MSG msg;
		while(WaitForSingleObject(ShExecInfo.hProcess,0)!=WAIT_OBJECT_0)
		{
			//»¹Î´ÍË³ö
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
		RefreshBindSet();
	}
	return IDCANCEL;
}

void CKeyBind::RefreshBindSet()
{
	CBindSetArchive archiveSet(CArchive::load);
	if (archiveSet.m_pArchive != NULL)
	{
		m_keyBindSet.Serialize(*archiveSet.m_pArchive);
	}
	int count = m_keyBindSet.GetCount();
	if (count != 0)
	{
		LPACCEL pAccel = new ACCEL[count];
		LPACCEL p = pAccel;
		POSITION pos = m_keyBindSet.GetHeadPosition();
		while (pos != NULL)
		{
			CKeyBindObj* pObj = m_keyBindSet.GetNext(pos);
			p->cmd = pObj->m_dwCmdId;
			p->key = pObj->m_wVirtualKeyCode;
			p->fVirt = FVIRTKEY;
			if (pObj->m_wModifiers & HOTKEYF_CONTROL)
			{
				p->fVirt |= FCONTROL;
			}
			if (pObj->m_wModifiers & HOTKEYF_ALT)
			{
				p->fVirt |= FALT;
			}
			if (pObj->m_wModifiers & HOTKEYF_SHIFT)
			{
				p->fVirt |= FSHIFT;
			}
			p++;
		}
		if (m_hAccel != NULL)
		{
			DestroyAcceleratorTable(m_hAccel);
			m_hAccel = NULL;
		}
		m_hAccel = CreateAcceleratorTable(pAccel, count);
		delete[] pAccel;
	}
}

#pragma warning(default : 4244)
