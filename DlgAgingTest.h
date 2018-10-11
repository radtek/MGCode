#pragma once


// CDlgAgingTest 对话框

class CDlgAgingTest : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAgingTest)

public:
	CDlgAgingTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAgingTest();

// 对话框数据
	enum { IDD = IDD_AGING_TEST };

	CFont m_font;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
};
