#pragma once


// CDlgZTest 对话框

class CDlgZTest : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgZTest)

public:
	CDlgZTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgZTest();

// 对话框数据
	enum { IDD = IDD_Z_TEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bFixZ;
	double m_dbZ;
	CSliderCtrl m_ctrSliderZ;
	afx_msg void OnBnClickedButtonMove();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
};
