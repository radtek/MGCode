#pragma once


// CDlgSetDxfPrecision �Ի���

class CDlgSetDxfPrecision : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetDxfPrecision)

public:
	CDlgSetDxfPrecision(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetDxfPrecision();

// �Ի�������
	enum { IDD = IDD_SET_DXF_PRECISION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dbDxfPrecision;
	double m_dbGerberOffset;
	virtual void OnOK();
};
