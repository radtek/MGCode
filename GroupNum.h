#pragma once


// CGroupNum �Ի���

class CGroupNum : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupNum)

public:
	CGroupNum(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGroupNum();

// �Ի�������
	enum { IDD = IDD_DIALOG_GROUPNUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nGroupNum;
};
