#pragma once
#include "resource.h"

// CCMarkTicps �Ի���

class CCMarkTicps : public CDialogEx
{
	DECLARE_DYNAMIC(CCMarkTicps)

public:
	CCMarkTicps(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCMarkTicps();

// �Ի�������
	enum { IDD = IDD_DIALOG_MARKTICPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMarkTicps;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
		CFont m_font_;
		BOOL m_bMarkTicps;
};
