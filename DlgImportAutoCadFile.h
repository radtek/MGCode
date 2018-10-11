#pragma once


// CDlgImportAutoCadFile 对话框

class CDlgImportAutoCadFile : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImportAutoCadFile)

public:
	CDlgImportAutoCadFile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgImportAutoCadFile();

// 对话框数据
	enum { IDD = IDD_IMPORT_AUTOCAD_FILE };

	double m_dbUnitTrans;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nCadUnit;
	BOOL m_bFilterText;
	afx_msg void OnBnClickedOk();
};
