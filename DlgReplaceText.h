#pragma once


// CDlgReplaceText 对话框

class CDlgReplaceText : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgReplaceText)

public:
	CDlgReplaceText(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgReplaceText();

// 对话框数据
	enum { IDD = IDD_REPLACE_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
