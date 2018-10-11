#pragma once

#include "adodb.h"
#include "afxcmn.h"

// CDlgCodeRecord 对话框

class CDlgCodeRecord : public CDialog
{
	DECLARE_DYNAMIC(CDlgCodeRecord)

public:
	CDlgCodeRecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCodeRecord();

// 对话框数据
	enum { IDD = IDD_DIALOG_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFind();
	virtual BOOL OnInitDialog();


	CAdoDB  m_adoDB;
	CAdoRecordset m_recordset;
	CListCtrl m_ListCodeRecord;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
