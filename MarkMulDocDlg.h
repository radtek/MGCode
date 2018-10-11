#if !defined(AFX_MARKMULDOCDLG_H__AB089EA1_1A12_11D5_97CC_00E04C3AA16B__INCLUDED_)
#define AFX_MARKMULDOCDLG_H__AB089EA1_1A12_11D5_97CC_00E04C3AA16B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkMulDocDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMarkMulDocDlg dialog

class CMarkMulDocDlg : public CDialog
{
// Construction
public:
	void MarkSEnd();
	void MarkEnd();
	void Delay(UINT nMicroSecond);
	void RotateMoto(long nMove, int nDirection, int nFreq);
	void Mark() ;
	void UpdateCount();
	void UpdateTime();
	void Run();
	BOOL GetNext();
	void UpdateView();
	BOOL IsFileExist(CString &strFileName);
	CMarkMulDocDlg(CWnd* pParent = NULL);   // standard constructor
	CObArray m_arrayDoc;
	CHLDoc * m_pDoc;
	
	//单个打标时间；
	DWORD m_dwSingle;
	//多文档打标时间；
	DWORD m_dwTotalStart;
	DWORD m_dwTotalEnd;
	DWORD m_dwTotal;
	//打标个数；
	int m_nCount;
	BOOL m_bPrevFrame;
	
	CMarkMgr* m_pMarkMgr;	//打标管理类
	
// Dialog Data
	//{{AFX_DATA(CMarkMulDocDlg)
	enum { IDD = IDD_MARK_MULDOC };
	CProgressCtrl	m_progressMark;
	CListBox	m_listDocName;
	BOOL	m_bManual;
	int		m_nFreq;
	int		m_nMarkType;
	BOOL	m_bLoopMark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkMulDocDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bMark;
	BOOL m_bMarkStop;
	BOOL m_bMarkExit;
	BOOL m_bFirst;
	int m_nCur;

	// Generated message map functions
	//{{AFX_MSG(CMarkMulDocDlg)
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDel();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnButtonMark();
	afx_msg void OnButtonStop();
	afx_msg void OnCheckManual();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	afx_msg LRESULT OnMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStart(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStop(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKMULDOCDLG_H__AB089EA1_1A12_11D5_97CC_00E04C3AA16B__INCLUDED_)
