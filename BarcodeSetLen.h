#pragma once


// CBarcodeSetLen �Ի���

class CBarcodeSetLen : public CDialogEx
{
	DECLARE_DYNAMIC(CBarcodeSetLen)

public:
	CBarcodeSetLen(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBarcodeSetLen();

// �Ի�������
	enum { IDD = IDD_DIALOG_BARCODESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	double m_dbBarcodeLen;
	double m_dbBarcodeWid;
	BOOL m_bSameLen;
};
