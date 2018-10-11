#pragma once

#include "HLDoc.h"
#include "FStatic.h"

// CDlgAutoOffsetEx 对话框

class CDlgAutoOffsetEx : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAutoOffsetEx)

public:
	CDlgAutoOffsetEx(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAutoOffsetEx();

	int	   m_nOldMoveModel;
	double m_dbOldHandMoveDis;
	BOOL m_bInit;
	CHLDoc *m_pDoc;


// 对话框数据
	enum { IDD = IDD_AUTOOFFSETEX };

	double	m_dbLastX;
	double	m_dbLastY;
	CFStatic	m_left;
	CFStatic	m_right;
	CFStatic	m_top;
	CFStatic	m_bottom;
	double	m_dbHandMoveDis;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSetLtpos();
	afx_msg void OnBnClickedBtnResetAllpos();
	afx_msg void OnBnClickedBtnAutoStart();
	afx_msg void OnBnClickedBtnLoad();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditHandMoveDis();
	afx_msg void OnRadioMoveModel();
	afx_msg void OnRadioMoveModel1();
	afx_msg void OnRadioMoveModel2();
	afx_msg void OnRadioMoveModel3();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNMClickListPosinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnMark();
	afx_msg void OnBnClickedBtnSeltoccd();


	double m_dbLfPos;
	double m_dbRtPos;
	PARA m_oldMarkPara;
	CString m_strPath;
	double m_dbLTposX;
	double m_dbLtPosY;
	double m_dbRBPosX;
	double m_dbRBPosY;
	double m_dbDis;
	void OffsetMark(void);
	CListCtrl m_ListPos;
	BOOL ResetAllpos();
	BOOL WriteFileData(void);
	BOOL ReadFileData(void);
	CHLApp* m_pApp;
	HICON m_on;
	HICON m_off;
	CImageList m_image;
	int m_nAllCol;
	int m_nAllRow;
	int m_nSelCol;
	int m_nSelRow;
	int m_nCCDNum;
};
