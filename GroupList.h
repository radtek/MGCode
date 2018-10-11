#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "groupobj.h"


// CGroupList 对话框

class CGroupList : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupList)

public:
	CGroupList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroupList();

// 对话框数据
	enum { IDD = IDD_DIALOG_GROUPLISI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_GroupList;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkListGroup(NMHDR *pNMHDR, LRESULT *pResult);
	int m_Row;
	int m_Col;
	CEdit m_edit;
	afx_msg void OnEnKillfocusEditGroupnum();
	CGroupObj *m_pGroupObj;
	afx_msg void OnNMClickListGroup(NMHDR *pNMHDR, LRESULT *pResult);
};
