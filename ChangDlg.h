#if !defined(AFX_CCHANGDLG_H__E693A3C0_D560_4803_83D8_04A9296417A0__INCLUDED_)
#define AFX_CCHANGDLG_H__E693A3C0_D560_4803_83D8_04A9296417A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangDlg.h : header file
//
enum ENUM_OBJECT_OPERATE
{
	ENUM_OBJECT_SIZE  = 0,
	ENUM_OBJECT_SCALE = 1,
	ENUM_OBJECT_ROTATE= 2,
	ENUM_OBJECT_POS   = 3,
	ENUM_OBJECT_END
};

/////////////////////////////////////////////////////////////////////////////
// CChangDlg dialog

class CChangDlg : public CDialog
{
// Construction
public:
	CChangDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CChangDlg)
	enum { IDD = ID_CHANG_DLG };
	double	m_dbX;
	double	m_dbY;
	double	m_dbRotate;
	BOOL	m_bProrata;
	CString	m_strSizeX;
	CString	m_strSizeY;
	CString	m_strPropX;
	CString	m_strPropY;
	//}}AFX_DATA

	int m_nBase;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangDlg)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangDlg)
	afx_msg void OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangePropX();
	afx_msg void OnChangePropY();
	afx_msg void OnChangeRotate();
	afx_msg void OnButtSelpos();
	afx_msg void OnCenter();
	afx_msg void OnMakecopy();
	afx_msg void OnChangeSizeX();
	afx_msg void OnChangeSizeY();
	afx_msg void OnCheckScale();
    afx_msg void OnCheckPOS();
	afx_msg void OnMakecopySet();
	afx_msg void OnButtonExtendobj();
	afx_msg void OnChangePosx();
	afx_msg void OnChangePosy();
	afx_msg void OnSetfocusPosx();
	afx_msg void OnSetfocusPosy();
	afx_msg void OnSetfocusRotate();
	afx_msg void OnSetfocusSizex();
	afx_msg void OnSetfocusSizey();
	afx_msg void OnSetfocusPropex();
	afx_msg void OnSetfocusPropey();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:

    void MakeitUse();

	void SetPos(double dbX,double dbY);
	void SetSize(double dbSizeX,double dbSizeY);

	void CloseDoor();
	void OpenDoor();

    CHLDoc * GetDocument();
	BOOL OnNewDocument();

	double	m_dbPropX;
	double	m_dbPropY;

	double m_dbSizeX;
	double m_dbSizeY;

	void UpdateUIData(BOOL bSaveAndValidate = TRUE);

	void UpdateProp(CQuad &quad);
	
private:
	DWORD m_dwOperate;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCHANGDLG_H__E693A3C0_D560_4803_83D8_04A9296417A0__INCLUDED_)





















