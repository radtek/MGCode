#pragma once


// CDlgArrayRename �Ի���

class CDlgArrayRename : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgArrayRename)

public:
	CDlgArrayRename(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgArrayRename();

// �Ի�������
	enum { IDD = IDD_DIALOG_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_strTextNum;
	CString m_strMatriNum;
};
