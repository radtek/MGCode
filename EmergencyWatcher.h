#pragma once

#define WM_EMERGENCY_EVENT (WM_APP+10)

class CEmergencyWatcher
{
public:
	CEmergencyWatcher(void);
	~CEmergencyWatcher(void);

public:
	static UINT WatchThreadFun(LPVOID pParam);	//Check, then Do yourself or Notify 
	void RunWatcher(HWND hWnd);
	void StopWatcher();

	//SafeDoor
	BOOL SafeDoorCheck(BOOL &bFirst);
	void SafeDoorProc(BOOL bFirst);	

private:
	HWND m_hWndNotify;
	CWinThread* m_pThread;
	BOOL volatile m_bThreadContinue;
};

extern CEmergencyWatcher g_EmergencyWatcher;
