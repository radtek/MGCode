#pragma once


// CDlgSetDxfPrecision 对话框

class CDlgSetDxfPrecision : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetDxfPrecision)

public:
	CDlgSetDxfPrecision(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetDxfPrecision();

// 对话框数据
	enum { IDD = IDD_SET_DXF_PRECISION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_dbDxfPrecision;
	double m_dbGerberOffset;
	virtual void OnOK();
};
