#pragma once


// CFileLoadData 对话框

class CFileLoadData : public CDialogEx
{
	DECLARE_DYNAMIC(CFileLoadData)

public:
	CFileLoadData(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileLoadData();

// 对话框数据
	enum { IDD = IDD_DLG_LFDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bUseFile;
	CString m_strPath;
	afx_msg void OnBnClickedBtnLoad();
	void ChangeText(void);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckUsefile();
	afx_msg void OnBnClickedOk();
	CString m_strNGMapPath;
	afx_msg void OnBnClickedButtonNgmap();
};
