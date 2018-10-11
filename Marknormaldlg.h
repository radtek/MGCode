#if !defined(AFX_MARKNORMALDLG_H__B8087380_FE40_11D3_A07E_00105A615525__INCLUDED_)
#define AFX_MARKNORMALDLG_H__B8087380_FE40_11D3_A07E_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarkNormalDlg.h : header file
//
#include "MarkMgr.h"

/////////////////////////////////////////////////////////////////////////////
// CMarkNormalDlg dialog

class CMarkNormalDlg : public CDialog
{
// Construction
public:
	CToolTipCtrl m_tooltip;
	CHLDoc * m_pDoc;
	CChain * m_pChain;
	void UpdateView();
	CMarkNormalDlg(CWnd* pParent = NULL);   // standard constructor
	int m_nTotal;			//打标总数(包括打标循环在内)；
	void UpdateTime(BOOL flag=FALSE);
	void Mark() ;

// Dialog Data
	//{{AFX_DATA(CMarkNormalDlg)
	enum { IDD = IDD_MARK_NORMAL };
	CProgressCtrl	m_progressMark;
	BOOL	m_bUpdate;
	int		m_nMarkType;
	BOOL	m_bLoopMark;
	int		m_nIntervalTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkNormalDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void MarkSEnd();
	void MarkEnd();
	int m_nCount;			//打标个数；
	CMarkMgr* m_pMarkMgr;	//打标管理类
		
protected:
	BOOL m_bMark;       //TRUE用于表示当前是否处于打标准备好，但又未进行打标的状态，例如处于打标暂停或停止打标状态
	BOOL m_bMarkStop;   //TRUE用于表示当前的停止按钮处于激活状态
	BOOL m_bMarkExit;   //表示是否处于某次的打标过程中，TRUE表示不处于，FALSE-表示处于
	
	BOOL m_bFirst;
	CTime m_timeStart;
	CTime m_timeEnd;
	CTimeSpan m_tsTotal;	//总时间
	CTimeSpan m_tsWork;		//工作时间
	CTimeSpan m_tsIdle;		//空闲时间

	DWORD m_dwSingle;		//单个打标时间
	DWORD m_dwWork;			//打标工作时间

	DWORD m_dwStart;		//单个打标时间开始
	DWORD m_dwEnd;			//单个打标时间结束


		
	// Generated message map functions
	//{{AFX_MSG(CMarkNormalDlg)
	afx_msg void OnButtonMark();
	afx_msg void OnButtonStop();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	afx_msg void OnCheckUpdate();
	afx_msg void OnRadioLaser();
	afx_msg void OnRadioRed();
	afx_msg void OnRadioFrame();
//	afx_msg void OnBUTTONRedCorrSetting();
	afx_msg void OnCheckLoopmark();
	//}}AFX_MSG
	afx_msg LRESULT OnMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStart(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMarkStop(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void OnMarkEndL(WPARAM wParam,LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKNORMALDLG_H__B8087380_FE40_11D3_A07E_00105A615525__INCLUDED_)
