#pragma once


// CBarcodeSetLen 对话框

class CBarcodeSetLen : public CDialogEx
{
	DECLARE_DYNAMIC(CBarcodeSetLen)

public:
	CBarcodeSetLen(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBarcodeSetLen();

// 对话框数据
	enum { IDD = IDD_DIALOG_BARCODESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	double m_dbBarcodeLen;
	double m_dbBarcodeWid;
	BOOL m_bSameLen;
};
