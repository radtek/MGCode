#pragma once
#include "afxcmn.h"
#include "DlgMarkNormalPane.h"


// CMatrixDoc 对话框

class CMatrixDoc : public CDialogEx
{
	DECLARE_DYNAMIC(CMatrixDoc)

public:
	CMatrixDoc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMatrixDoc();

// 对话框数据
	enum { IDD = IDD_DIALOG_MATRIXDOC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ListMatrixDoc;
	afx_msg void OnBnClickedButtonLoad();
	CString m_strPath;
	afx_msg void OnBnClickedButtonAdd();
	CString m_strMatrix;
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonChange();
	afx_msg void OnBnClickedOk();
	BOOL WriteFileData(void);
	BOOL ReadFileData(void);
	BOOL FileFind();
	afx_msg void OnNMClickListMatrixdoc(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
