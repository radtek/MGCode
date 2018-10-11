#pragma once


// CDsfListCtrl

class CDsfListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CDsfListCtrl)

public:
	CDsfListCtrl();
	virtual ~CDsfListCtrl();
	int AddItem( LPCTSTR pszText, ... );
	void SetCurSel(int nIndex);
	int GetCurSel() const;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual COLORREF OnGetCellBkColor(int nRow, int nColumn );
	CArray<COLORREF, COLORREF> m_arrayColor;
	int m_nColorColumn;
	int m_nCurRow;

	COLORREF m_crBackground;
	CBrush m_brBackground;
public:
	void SetRowColor(int nRow, COLORREF color);
	void SetColorColumn(int nColumn=-1);
};


