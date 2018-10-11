#pragma once


// CMyMFCPropertyGridCtrl

class CMyMFCPropertyGridCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CMyMFCPropertyGridCtrl)

public:
	CMyMFCPropertyGridCtrl();
	virtual ~CMyMFCPropertyGridCtrl();

	int m_nMyLeftColumnWidth;

public:
	void SetColumnWidth(INT value){m_nMyLeftColumnWidth=value;};

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void AdjustLayout();
};


