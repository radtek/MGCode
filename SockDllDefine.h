#ifndef __SOCKDLLDEFINE2_H__
#define __SOCKDLLDEFINE2_H__
/////////////////////////////////////
#include "vSocketMacro.h"
/////////////////////////////////////
//各种消息定义
#define WM_SOCKETS				(WM_USER+57)
#define WM_VBSENDFILE           (WM_USER+50)	

#define IDWM_SOCKET_ACCEPT			0		//一个新的客户机连到服务器端
#define IDWM_SOCKET_RECEIVE			1		//有网络信息需要读取
#define IDWM_SOCKET_CLOSE			2		//关闭指定的SOCKET 或关闭网络连接
//扩展版
#define IDWM_SOCKET_SEND_COMPLETED	3		//用于支持大容量的数据传送

//接收发送的缓冲区大小
#define SOCKET_BUFSIZE		0x4000

////////////////////////////////
//调用者响应消息	WM_SOCKETS
/*
//LOWORD(LPARAM)==nID(服务器版ListenSocket's_ID), HIWORD(LPARAM)--预留扩展
//LOWORD(WPARAM)==网络事件消息, HIWORD(WPARAM)==nID(主SOCKET's_ID)
switch( LOWORD(wParam) )
{
	case IDWM_SOCKET_ACCEPT:
		此消息对服务器有效，用于通知一个新的客户加入

	case IDWM_SOCKET_RECEIVE:
		有信息需要读取，调用ReadData();

	case IDWM_SOCKET_CLOSE:
		SOCKET关闭
	case IDWM_SOCKET_SEND_COMPLETED:
		通知前一次SendData()函数调用数据已传送完成
}
*/

typedef BOOL (__stdcall* pfuncInitSock)(HWND,BOOL,int&,LPCTSTR,UINT,BOOL);
typedef BOOL (__stdcall* pfuncReadSend)(LPCTSTR,DWORD,int,DWORD*,int);
typedef BOOL (__stdcall* pfuncCloseSock)(int,int);
typedef BOOL (__stdcall* pfuncGetIP)(int,int,DWORD*,DWORD*);

class IMFCSockDll
{
public:
	IMFCSockDll(){ Clear(); }
	~IMFCSockDll(){ ReleaseLib(); }
	BOOL InitSock(HWND hWnd,BOOL bServer,int& nMainSocketID,LPCTSTR lpsIP=NULL,UINT nPort=5000,BOOL bEnableMultiSameClients=FALSE)
	{
		if( !LoadLib() ) return FALSE;
		return m_pfuncInit(hWnd,bServer,nMainSocketID,lpsIP,nPort,bEnableMultiSameClients);
	}
	BOOL ReadData(LPTSTR lpsData,DWORD dwBuffLen,int nMainSocketID,DWORD* pdwReaded=NULL,int nListenSocketID=0)
	{//接收数据(最多收取dwBuffLen-1个字符)
		if( !LoadLib() ) return FALSE;
		return m_pfuncRead(lpsData,dwBuffLen,nMainSocketID,pdwReaded,nListenSocketID);
	}
	BOOL SendData(LPCTSTR lpsData,DWORD dwToSend,int nMainSocketID,DWORD* pdwSended=NULL,int nListenSocketID=0)
	{
		if( !LoadLib() ) return FALSE;
		return m_pfuncSend(lpsData,dwToSend,nMainSocketID,pdwSended,nListenSocketID);
	}
	BOOL CloseSock(int nMainSocketID=0,int nListenSocketID=0)
	{
		if( !LoadLib() ) return FALSE;
		return m_pfuncClose(nMainSocketID,nListenSocketID);
	}
	BOOL GetAssociatsSocketIP(int nMainSocketID,int nListenSocketID,DWORD* pdwMainSocketIP=NULL,DWORD* pdwListenSocketIP=NULL)
	{
		if( !LoadLib() ) return FALSE;
		return m_pfuncGetIP(nMainSocketID,nListenSocketID,pdwMainSocketIP,pdwListenSocketIP);
	}


private:
	void Clear()
	{
		m_hLib=NULL; m_pfuncInit=NULL; m_pfuncRead=NULL;
		m_pfuncSend=NULL; m_pfuncClose=NULL; m_pfuncGetIP=NULL;
	}

	void ReleaseLib()
	{
		if( m_hLib ) ::FreeLibrary(m_hLib);
		Clear();
	}




	BOOL LoadLib()
	{
		if( !m_hLib ) m_hLib=::LoadLibrary(TEXT("SockDll.dll"));
		if( !m_hLib )
		{
			Trace(_T("SockDll.dll不存在或路径不正确!"));
			return FALSE;
		}

		
		char szInit[] = {"InitSockEx"};
		char szRead[] = {"ReadDataEx"};
		char szSend[] = {"SendDataEx"};
		char szClose[] = {"CloseSockEx"};
		char szGetIP[] = {"GetAssociatsSocketIP"};

		if( !m_pfuncInit )
			m_pfuncInit=GetPfunc(pfuncInitSock,m_hLib,szInit);
		if( !m_pfuncRead )
			m_pfuncRead=GetPfunc(pfuncReadSend,m_hLib,szRead);
		if( !m_pfuncSend )
			m_pfuncSend=GetPfunc(pfuncReadSend,m_hLib,szSend);
		if( !m_pfuncClose )
			m_pfuncClose=GetPfunc(pfuncCloseSock,m_hLib,szClose);
		if( !m_pfuncGetIP )
			m_pfuncGetIP=GetPfunc(pfuncGetIP,m_hLib,szGetIP);
		if( !m_pfuncInit || !m_pfuncRead || !m_pfuncSend ||
			!m_pfuncClose || !m_pfuncGetIP )
		{
			Trace(_T("SockDll.dll版本不正确,请选择正确版本的SockDll.dll!"));
			ReleaseLib();
			return FALSE;
		}
		return TRUE;
	}
	//////////////
	pfuncInitSock m_pfuncInit;
	pfuncReadSend m_pfuncRead;
	pfuncReadSend m_pfuncSend;
	pfuncCloseSock m_pfuncClose;
	pfuncGetIP m_pfuncGetIP;
	HINSTANCE m_hLib;
};
/////////////////////////////////////
#endif
