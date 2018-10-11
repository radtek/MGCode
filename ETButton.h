#pragma once


// CETButton

class CETButton : public CButton
{
	DECLARE_DYNAMIC(CETButton)

public:
	CETButton();
	CETButton( int nItem, int nSubItem, CRect rect, HWND hParent );
	virtual ~CETButton();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
	int m_inItem;
	int m_inSubItem;
	CRect m_rect;
	HWND m_hParent;
	BOOL bEnable;
};


