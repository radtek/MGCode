#pragma once


// CDsfDlgEditIO dialog

class CDsfDlgEditIO : public CDialogEx
{
	DECLARE_DYNAMIC(CDsfDlgEditIO)

public:
	CDsfDlgEditIO(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDsfDlgEditIO();

// Dialog Data
	enum { IDD = IDD_DSF_DLG_EDITOUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nValue;
	UINT m_nMask;
	CString m_strTitle;
	CString m_strValue1;
	CString m_strValue2;
	CString m_strMask1;
	CString m_strMask2;
	BOOL m_bValues[32];
	BOOL m_bMasks[32];
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strValue;
	CString m_strMask;
	afx_msg void OnBnClickedCheck1();
};
