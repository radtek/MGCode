#pragma once


// CDlgWorkbenchMeasure 对话框

class CDlgWorkbenchMeasure : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWorkbenchMeasure)

public:
	CDlgWorkbenchMeasure(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWorkbenchMeasure();

// 对话框数据
	enum { IDD = IDD_WORKBENCH_MEASURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dbX1;
	double m_dbX2;
//	CString m_dbY1;
	double m_dbY1;
	double m_dbY2;
	double m_dbPointsDis;
	afx_msg void OnBnClickedButtonGetPos1();
	afx_msg void OnBnClickedButtonGetPos2();
	afx_msg void OnBnClickedButtonCalculate();
	afx_msg void OnClose();
};
