#pragma once


// CDlgArrayRename 对话框

class CDlgArrayRename : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgArrayRename)

public:
	CDlgArrayRename(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgArrayRename();

// 对话框数据
	enum { IDD = IDD_DIALOG_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strTextNum;
	CString m_strMatriNum;
};
