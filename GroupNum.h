#pragma once


// CGroupNum 对话框

class CGroupNum : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupNum)

public:
	CGroupNum(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGroupNum();

// 对话框数据
	enum { IDD = IDD_DIALOG_GROUPNUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nGroupNum;
};
