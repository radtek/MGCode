#pragma once


// CDlgSetWaferArray 对话框

class CDlgSetWaferArray : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetWaferArray)

public:
	CDlgSetWaferArray(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetWaferArray();

// 对话框数据
	enum { IDD = IDD_SET_WAFER_ARRAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dbRowDist;
	double m_dbColDist;
	double m_dbShrinkRow;
	double m_dbShrinkYCol;
};
