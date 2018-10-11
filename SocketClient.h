#pragma once

#include <WINSOCK2.H>

//blocking mode
class CSocketClient
{
public:
	CSocketClient();
	~CSocketClient();
	
	//UPD:0, TCP:1
	void SetSocket(int nType, const char *strIP, int nPort, int nTimeout=3000);

	//For TCP
	BOOL Connet();
	BOOL IsConnected();
	void Close();

	//Common
	CString GetLocalIP();

	LPCSTR ClearBuffer(int *pNum=NULL);
	LPCSTR Receive(int *pNum=NULL);
	void Send(const char *buffer, int len=0);
	char* GetData(){return m_pBuffer;};
	

private:
	int m_nTimeout;
	SOCKET m_sockClient;
	SOCKADDR_IN m_addrServer;
	BOOL m_bConnected;
	int m_nWSALastError;
	char *m_pBuffer;
	LPCSTR Read(int *pNum, int nTimeout, BOOL bPeek);
};
