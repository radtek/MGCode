#pragma once


#include "HLDoc.h"
#include "FStatic.h"

// CCHandPos 对话框

class CCHandPos : public CDialogEx
{
	DECLARE_DYNAMIC(CCHandPos)

public:
	CCHandPos(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCHandPos();


	CFStatic	m_left;
	CFStatic	m_right;
	CFStatic	m_top;
	CFStatic	m_bottom;
	double	m_dbHandMoveDis;


	int	   m_nOldMoveModel;
	double m_dbOldHandMoveDis;

	BOOL m_bInit;
	CHLDoc *m_pDoc;

// 对话框数据
	enum { IDD = IDD_HAND_POS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioMoveModel();
	afx_msg void OnBnClickedRadioMoveModel1();
	afx_msg void OnBnClickedRadioMoveModel2();
	afx_msg void OnBnClickedRadioMoveModel3();
	afx_msg void OnEnChangeEditHandMoveDis();
//	afx_msg void OnDestroy();
	afx_msg void OnDestroy();
};
