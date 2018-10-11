#pragma once

#include "PowerMeter.h"
// CDlgPowerTest 对话框

class CDlgPowerTest : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPowerTest)

public:
	CDlgPowerTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPowerTest();

// 对话框数据
	enum { IDD = IDD_POWER_TEST };
	
	CString m_strLogFilePath;
	void LogResult(int nCount, double dPowerMeter);
	
	BOOL m_nCurStep;	//当前步骤
	
	CTime m_tmStart;
	CTime m_tmLastMarkEnd;
	CTime m_tmLastCheckPower;

	CPowerMeter *m_pPowerMeter;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	double m_dbTotalTime;
	double m_dbPowerCheckInterval;
	int m_nPowerCheckWait;
	int m_nMarkInterval;
	CString m_strRunTime;
	int m_nPowerCheckCount;
	double m_dbLastPower;
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonStart();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
};
