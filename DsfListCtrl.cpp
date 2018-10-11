// DsfListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "HL.h"
#include "DsfListCtrl.h"
#include "MyFont.h"

// CDsfListCtrl

IMPLEMENT_DYNAMIC(CDsfListCtrl, CMFCListCtrl)

CDsfListCtrl::CDsfListCtrl()
{
	m_nCurRow = 0;
	m_nColorColumn = -1;
	m_arrayColor.SetSize(50);
	for(int i=0; i<50; i++)
	{
		m_arrayColor[i] = RGB(0,0,0);
	}
}

CDsfListCtrl::~CDsfListCtrl()
{
}


BEGIN_MESSAGE_MAP(CDsfListCtrl, CMFCListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CDsfListCtrl::OnLvnColumnclick)
END_MESSAGE_MAP()



// CDsfListCtrl message handlers
void CDsfListCtrl::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

COLORREF CDsfListCtrl::OnGetCellBkColor(int nRow, int nColumn )
{
	if(nColumn == m_nColorColumn)
	{
		int nSize = m_arrayColor.GetSize();
		if(nRow >= nSize)
		{
			nRow = nSize - 1;
		}
		return m_arrayColor.GetAt(nRow);
	}
	return CMFCListCtrl::OnGetCellBkColor(nRow, nColumn);
}

void CDsfListCtrl::SetRowColor(int nRow, COLORREF color)
{
	if(nRow < m_arrayColor.GetSize())
	{
		m_arrayColor.SetAt(nRow, color);
	}
}

void CDsfListCtrl::SetColorColumn(int nColumn)
{
	m_nColorColumn = nColumn;
}

int CDsfListCtrl::AddItem( LPCTSTR pszText, ... )
{
	const int iIndex   = InsertItem( GetItemCount(), pszText, -1);

 	va_list list;
	va_start( list, pszText );
	for( int iColumn = 1; iColumn < GetHeaderCtrl().GetItemCount(); iColumn++ )
	{
		pszText = va_arg( list, LPCTSTR );
		VERIFY( CMFCListCtrl::SetItemText( iIndex, iColumn, pszText) );
	}
	va_end( list );

	return iIndex;
}

int CDsfListCtrl::GetCurSel() const
{
	return m_nCurRow;
}

void CDsfListCtrl::SetCurSel(int nIndex)
{
	m_nCurRow = nIndex;
}

