#pragma once


// CFileLoadData �Ի���

class CFileLoadData : public CDialogEx
{
	DECLARE_DYNAMIC(CFileLoadData)

public:
	CFileLoadData(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileLoadData();

// �Ի�������
	enum { IDD = IDD_DLG_LFDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
