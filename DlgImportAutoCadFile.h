#pragma once


// CDlgImportAutoCadFile �Ի���

class CDlgImportAutoCadFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImportAutoCadFile)

public:
	CDlgImportAutoCadFile(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgImportAutoCadFile();

// �Ի�������
	enum { IDD = IDD_IMPORT_AUTOCAD_FILE };

	double m_dbUnitTrans;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nCadUnit;
	BOOL m_bFilterText;
	afx_msg void OnBnClickedOk();
};
