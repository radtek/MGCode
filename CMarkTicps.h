#pragma once
#include "resource.h"

// CCMarkTicps 对话框

class CCMarkTicps : public CDialogEx
{
	DECLARE_DYNAMIC(CCMarkTicps)

public:
	CCMarkTicps(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCMarkTicps();

// 对话框数据
	enum { IDD = IDD_DIALOG_MARKTICPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMarkTicps;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
		CFont m_font_;
		BOOL m_bMarkTicps;
};
