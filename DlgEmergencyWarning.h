#pragma once


// CDlgEmergencyWarning �Ի���

class CDlgEmergencyWarning : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEmergencyWarning)

public:
	CDlgEmergencyWarning(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgEmergencyWarning();

// �Ի�������
	enum { IDD = IDD_EMERGENCY_WARNING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
