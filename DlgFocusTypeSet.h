#pragma once
#include "afxcmn.h"


// CDlgFocusTypeSet �Ի���

class CDlgFocusTypeSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFocusTypeSet)

public:
	CDlgFocusTypeSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFocusTypeSet();
	
// �Ի�������
	enum { IDD = IDD_FOCUS_TYPE_SET };

	void RefreshFocusID();



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrlFocusType;
	virtual BOOL OnInitDialog();
	CString m_strFocus;
	CString m_strMoveDis;
	CString m_strMoveNum;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnNMClickListFocusType(NMHDR *pNMHDR, LRESULT *pResult);
};
