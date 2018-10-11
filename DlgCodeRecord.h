#pragma once

#include "adodb.h"
#include "afxcmn.h"

// CDlgCodeRecord �Ի���

class CDlgCodeRecord : public CDialog
{
	DECLARE_DYNAMIC(CDlgCodeRecord)

public:
	CDlgCodeRecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCodeRecord();

// �Ի�������
	enum { IDD = IDD_DIALOG_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFind();
	virtual BOOL OnInitDialog();


	CAdoDB  m_adoDB;
	CAdoRecordset m_recordset;
	CListCtrl m_ListCodeRecord;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
