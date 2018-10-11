#pragma once

#include "hl.h"
#include "AdoDb.h"

// CDataSourceDlg 对话框

class CDataSourceDlg : public CDialog
{
	DECLARE_DYNAMIC(CDataSourceDlg)

public:
	CDataSourceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataSourceDlg();

// 对话框数据
	enum { IDD = IDD_SET_DATASOURCE };
	//CString	m_strDsn;
	CString	m_strMdb;
	//CString	m_strUserName;
	CString	m_strPwd;
	CString	m_strServer;
	//int		m_nType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboData();
	afx_msg void OnBnClickedButtonMdb();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedOk();
public:
	DBPARA m_Para;
	void UpdateFrame();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnEditchangeComboData();
	int m_nType;
	CString m_strDsn;
	CString m_strUserName;
};
