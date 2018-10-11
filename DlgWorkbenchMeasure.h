#pragma once


// CDlgWorkbenchMeasure �Ի���

class CDlgWorkbenchMeasure : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWorkbenchMeasure)

public:
	CDlgWorkbenchMeasure(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWorkbenchMeasure();

// �Ի�������
	enum { IDD = IDD_WORKBENCH_MEASURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
