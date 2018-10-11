#if !defined(AFX_TEXTPAGETEXTDLG_H__A0E1C923_93D6_4327_94D1_09049744767F__INCLUDED_)
#define AFX_TEXTPAGETEXTDLG_H__A0E1C923_93D6_4327_94D1_09049744767F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// TextPageTextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextPageTextDlg dialog
class CTextPageTextDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CTextPageTextDlg)

	// Construction
public:
	CTextPageTextDlg();
	~CTextPageTextDlg();
	void UpdateDataView();
	
	CString GetSerialString();
	CStringArray * m_pSerial;
	int *m_pCur;

	//跳号文本
	BOOL m_bFill;			//是否补齐
	CString m_strPrefix;	//数字前置字符
	CString m_strLeft;		//数字前字符串
	CString m_strRight;		//数字后字符串
	CString m_strFromFile;	//跳号取自于文件之名字
	int m_nJumpStart;		//数字开始
	int m_nJumpEnd;			//数字结束
	int m_nJumpLen;			//跳号部分长度
	int m_nJumpStep;		//跳号时步长
	int m_nJumpInterval;	//跳号时间隔
	int m_nFromFile;		//是否跳号取自于文件
	CString m_strBinText;	//进制文本内容
	
	// Dialog Data
	//{{AFX_DATA(CTextPageTextDlg)
	enum { IDD = IDD_TEXT_PAGE_TEXT };
	CString	m_strDate;
	CString	m_strFix;
	int		m_nText;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTextPageRoundDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTextPageTextDlg)
	afx_msg void OnRadioFix();
	afx_msg void OnRadioSerial();
	afx_msg void OnRadioTime();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSerial();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeDateEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
public:
	BOOL m_bModel;
	CString m_strModelNo;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTPAGETEXTDLG_H__A0E1C923_93D6_4327_94D1_09049744767F__INCLUDED_)
