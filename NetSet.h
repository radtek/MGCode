#pragma once
#include "SockDllDefine.h"
#include "socketclient.h"
#include "afxwin.h"


// CNetSet �Ի���

class CNetSet : public CDialogEx
{
	DECLARE_DYNAMIC(CNetSet)

public:
	CNetSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNetSet();

// �Ի�������
	enum { IDD = IDD_DIALOG_NETSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strIPAddress;
	int m_nPort;
	CString m_strSend_NET;
	CString m_strRec_NET;
	IMFCSockDll m_sock;                    //���������¼�
	BOOL m_bConnected;
	int m_nMainSockID;
	int m_ListenSockID;
	afx_msg void OnBnClickedBtnOpennet();
	void UpdateNetView();
	afx_msg void OnBnClickedBtnClosenet();
	afx_msg void OnBnClickedBtnSend();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	BOOL m_bIsNet;
	afx_msg void OnBnClickedCheckIsnet();
	CSocketClient m_ClientSock;
	CString m_strLotNo;
	CString m_strEqNo;
	CString m_strSendShow;
	CString m_strOperatorcode;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL m_bDebug;
	CComboBox m_ComBoSend;
	CComboBox m_ComBoStyle;
	BOOL NetSendRecv();
	BOOL NetSendRecvOpenDoc();
	BOOL NetSendRecvAfterData();
	afx_msg void OnCbnSelchangeComboSend();
	CString m_strReverMatriCode;
};
