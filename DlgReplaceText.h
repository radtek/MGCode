#pragma once


// CDlgReplaceText �Ի���

class CDlgReplaceText : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgReplaceText)

public:
	CDlgReplaceText(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgReplaceText();

// �Ի�������
	enum { IDD = IDD_REPLACE_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strText;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
