#pragma once


// CDlgSetWaferArray �Ի���

class CDlgSetWaferArray : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetWaferArray)

public:
	CDlgSetWaferArray(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSetWaferArray();

// �Ի�������
	enum { IDD = IDD_SET_WAFER_ARRAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dbRowDist;
	double m_dbColDist;
	double m_dbShrinkRow;
	double m_dbShrinkYCol;
};
