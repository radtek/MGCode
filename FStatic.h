#if !defined(AFX_FSTATIC_H__66FEDC04_B11F_11D7_8454_00055DAA27A9__INCLUDED_)
#define AFX_FSTATIC_H__66FEDC04_B11F_11D7_8454_00055DAA27A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MOVE_LEFT	0
#define MOVE_RIGHT	1
#define MOVE_TOP	2
#define MOVE_BOTTOM	3
#define MOVE_MODEL	4	//设置操作模式
#define MOVE_UP		5
#define MOVE_DOWN	6

// FStatic.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CFStatic window

class CFStatic : public CStatic
{
// Construction
public:
	CFStatic();

// Attributes
public:
	static int m_nMoveModel;  //移动模式：0代表慢移, 1代表快移, 2代表1X定移, 3代表10X定移
	static double m_dbMoveDis;	  //移动间距

private:
	void ProcDirRun();
	void GetButString(CString &str); 
	void ProcDirStop();
	
	BOOL m_bActive;
	BOOL m_bRun;
	BOOL m_bEnable;
	int m_nOper;
	BOOL m_bRegisterMouseLeave;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetFast(BOOL bFast);
	void EnableWin(BOOL bFlag);
	void SetOper(int nOper);
	void InActive();
	virtual ~CFStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFStatic)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FSTATIC_H__66FEDC04_B11F_11D7_8454_00055DAA27A9__INCLUDED_)
