#pragma once


// CDsfDlgInsertObj dialog

class CDsfDlgInsertObj : public CDialogEx
{
	DECLARE_DYNAMIC(CDsfDlgInsertObj)

public:
	CDsfDlgInsertObj(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDsfDlgInsertObj();

// Dialog Data
	enum { IDD = IDD_DSF_DLG_DSFOBJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nObjType;
	int m_nSortType;
	UINT m_nTime;
	CString m_strInput;
	CString m_strItMask;
	CString m_strOtMask;
	CString m_strOutput;
	UINT m_nInput;
	UINT m_nItMask;
	UINT m_nOutput;
	UINT m_nOtMask;
	afx_msg void OnBnClickedBtnEditinput();
	afx_msg void OnBnClickedBtnEditoutput();
	afx_msg void OnBnClickedOk();
	void Check();
};
