#pragma once


// CDlgEmergencyWarning 对话框

class CDlgEmergencyWarning : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEmergencyWarning)

public:
	CDlgEmergencyWarning(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEmergencyWarning();

// 对话框数据
	enum { IDD = IDD_EMERGENCY_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnClosesoft();
	BOOL m_bSetTime;
};
