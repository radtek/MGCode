#include "stdafx.h"
#include "SocketClient.h"

#define MAX_RECV_NUM 1024*1024
#pragma comment (lib,"ws2_32.lib")

CSocketClient::CSocketClient()
{
	WSADATA wsaData;
	if(::WSAStartup(MAKEWORD(2,2), &wsaData))
		AfxMessageBox(_T("Fail to initialize WS2_32.DLL"));

	m_sockClient = NULL;
	m_bConnected = FALSE;
	m_pBuffer = new char[MAX_RECV_NUM];
	m_nWSALastError = 0;
}

CSocketClient::~CSocketClient()
{	
	Close();

	delete m_pBuffer;
	::WSACleanup();
}

void CSocketClient::SetSocket(int nType, LPCSTR strIP, int nPort, int nTimeout)
{
	if (m_sockClient)
		::closesocket(m_sockClient);

	if (nType)
	{
		m_sockClient = ::socket(AF_INET, SOCK_STREAM, 0);
		m_bConnected = TRUE;
	}
	else
	{
		m_sockClient = ::socket(AF_INET, SOCK_DGRAM, 0);
		m_bConnected = FALSE;
	}

	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_addr.S_un.S_addr = inet_addr(strIP);
	m_addrServer.sin_port = htons(nPort);

	m_nTimeout = nTimeout;
}

BOOL CSocketClient::Connet()
{
	m_bConnected = FALSE;

	ULONG argp = 1;
	::ioctlsocket(m_sockClient, FIONBIO, &argp);
	::connect(m_sockClient, (SOCKADDR *)&m_addrServer, sizeof(SOCKADDR_IN));

	TIMEVAL timeout = {2, 0};	//seconds
	FD_SET writefds;
	FD_ZERO(&writefds);
	FD_SET(m_sockClient, &writefds);
	if (select(0,  NULL, &writefds, NULL, &timeout)==1)
	{
		argp = 0;
		::ioctlsocket(m_sockClient, FIONBIO, &argp);
		m_bConnected = TRUE;
	}
	return m_bConnected;
}

BOOL CSocketClient::IsConnected()
{	
	if (m_bConnected)
	{
		Read(NULL, 0, TRUE);
	
		//getsockopt经测试无法实现此功能
	}

	return m_bConnected;
}

void CSocketClient::Close()
{
	::closesocket(m_sockClient);
}


CString CSocketClient::GetLocalIP()
{
	CString strIP;

	char hostname[32];
	PHOSTENT pHost;
	struct in_addr *pAddrIn;
	if(gethostname (hostname, sizeof(hostname)) == 0)
	{ 
		if((pHost = gethostbyname(hostname)) != NULL) 
		{ 
			pAddrIn	 = (struct in_addr *)*pHost->h_addr_list;
			strIP.Format("%d.%d.%d.%d",pAddrIn->s_net,pAddrIn->s_host,pAddrIn->s_lh,pAddrIn->s_impno);		
		}
	}
	return strIP;
}

void CSocketClient::Send(const char* buffer, int len)
{
	if (!m_bConnected)
		return;

	if (len==0)	len=strlen(buffer);
	::sendto(m_sockClient, buffer, len, 0, (SOCKADDR*)&m_addrServer, sizeof(SOCKADDR));
}


LPCSTR CSocketClient::Read(int *pNum, int nTimeout, BOOL bPeek)
{
	if (!m_bConnected)
		return NULL;

	if(pNum) *pNum=0;
	m_pBuffer[0] = 0;
	m_pBuffer[1] = 0;

	int nSeconds = nTimeout/1000;
	int nMicroseconds = nTimeout%1000;
	TIMEVAL timeout = {nSeconds, nMicroseconds};

	FD_SET readfds;
	FD_ZERO(&readfds);
	FD_SET(m_sockClient, &readfds);
	if (select(0,  &readfds, NULL, NULL, &timeout)>=0)
	{
		if (FD_ISSET(m_sockClient, &readfds))
		{
			int nFlags = bPeek ? MSG_PEEK: 0;
			int nRecvRtn =  recv(m_sockClient, m_pBuffer, MAX_RECV_NUM, nFlags);
			if (SOCKET_ERROR==nRecvRtn)
			{
				m_bConnected = FALSE;
				m_nWSALastError = WSAGetLastError();
			}
			else if (nRecvRtn==0)
			{
				m_bConnected = FALSE;
			}
			else
			{
				if(pNum) *pNum=nRecvRtn;
				m_pBuffer[nRecvRtn] = 0;
			}
		}
	}

	if (!m_bConnected)
		return NULL;

	return m_pBuffer;
}


LPCSTR CSocketClient::Receive(int *pNum)
{
	return Read(pNum, m_nTimeout, FALSE);
}

LPCSTR CSocketClient::ClearBuffer(int *pNum)
{
	return Read(pNum, 0, FALSE);	
}
