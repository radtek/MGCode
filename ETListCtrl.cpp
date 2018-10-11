/********************************************************************
* 	Project  : NetMonitor
* 	FileName : ListCtrlEx.h
* 	Change   : 	
* 	Brief    :  	
* 	Author   : Chen Jun ( chenjun@3cis.com.cn )
* 	Copyright ( c ) 2007-2008 3cis 
* 	All Right Reserved
*********************************************************************/

#include "stdafx.h"
#include "ETListCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

#define IDC_BUTTON_ID 0x1235

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx



CETListCtrl::CETListCtrl()
{
	m_uID = IDC_BUTTON_ID;
}



CETListCtrl::~CETListCtrl()
{
	
}



BEGIN_MESSAGE_MAP( CETListCtrl, CListCtrl )
	//{{AFX_MSG_MAP( CETListCtrl )
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(LVN_ENDSCROLL, &CETListCtrl::OnLvnEndScroll)
	ON_WM_DRAWITEM()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETListCtrl メッセージ ハンドラ

void CETListCtrl::createItemButton( int nItem, int nSubItem, HWND hMain ,CString strCaption)
{
	CRect rect;
	int offset = 0;

	// Make sure that the item is visible
	if( !EnsureVisible(nItem, TRUE)) 
		return ;

	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}

	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nSubItem);
	if(rect.right > rcClient.right) 
		rect.right = rcClient.right;
	//basic code end

	rect.bottom = rect.top + rect.Height();

	int iPageCout = GetCountPerPage();
	if ( nItem >= iPageCout )
	{
		rect.top += rect.Height();
		rect.bottom += rect.Height();
	}

	DWORD dwStyle =  WS_BORDER | WS_VISIBLE;
	CETButton *pButton = new CETButton(nItem,nSubItem,rect,hMain);
	m_uID++;
	pButton->Create(strCaption,dwStyle, rect, this, m_uID);
	m_mButton.insert( make_pair( nItem, pButton ) );

	int iTopIndex = GetTopIndex();
	if ( iTopIndex > 0 )
	{
		updateListCtrlButtonPos();
	}
	
	return;
}

void CETListCtrl::release()
{
	button_map::iterator iter;
	iter = m_mButton.begin();
	while ( iter != m_mButton.end() )
	{
		delete iter->second;
		iter->second = NULL;
		iter++;
	}

}
void CETListCtrl::deleteItemEx( int nItem )
{
	int iCount = GetItemCount();
	DeleteItem( nItem );
	button_map::iterator iter;
	button_map::iterator iterNext;
#ifdef USE_TOPINDEX_BUTTON
	//add-----------------------------------
	iter = m_mButton.find( nItem );
	iterNext = iter;
	iterNext++;
	while ( iterNext != m_mButton.end() )
	{
		iter->second->bEnable = iterNext->second->bEnable;
		iterNext++;
		iter++;
	}
	//------------------------------
#endif
	iter = m_mButton.find( iCount - 1 );
	if ( iter != m_mButton.end() )
	{
		delete iter->second;
		iter->second = NULL;
		m_mButton.erase( iter );
		updateListCtrlButtonPos();
	}
}
void CETListCtrl::OnLvnEndScroll(NMHDR *pNMHDR, LRESULT *pResult)
{
	// この機能は Internet Explorer 5.5 かそれ以降のバージョンを必要とします。
	// シンボル _WIN32_IE は >= 0x0560 にならなければなりません。
	LPNMLVSCROLL pStateChanged = reinterpret_cast<LPNMLVSCROLL>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	updateListCtrlButtonPos();
	*pResult = 0;
}

void CETListCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	
	CListCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CETListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if(SB_ENDSCROLL==nSBCode)
	{
		updateListCtrlButtonPos();
		Invalidate();
	}
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CETListCtrl::updateListCtrlButtonPos()
{
	int iTopIndex = GetTopIndex();
	int nItem = iTopIndex;
	button_map::iterator iter;
	button_map::iterator iterUp;
	int iLine = 0;
#ifdef USE_TOPINDEX_BUTTON
	iter = m_mButton.find( iTopIndex );
	iterUp = m_mButton.begin();
	while ( iter != m_mButton.end() )
	{
		iterUp->second->EnableWindow( iter->second->bEnable );
		iter ++;
		iterUp++;
	}
#else
	for ( ; nItem < GetItemCount(); nItem++ )
	{
		iter = m_mButton.find(nItem);
		if( iter!= m_mButton.end() )
		{
			CRect rect;
			iterUp = m_mButton.find(iLine);
			rect = iterUp->second->m_rect;
			iter->second->MoveWindow( &rect );
			iter->second->ShowWindow( SW_SHOW );
			if( iLine < iTopIndex )
			{
				iterUp->second->ShowWindow( SW_HIDE );
			}
			iLine++;
		}
	}
#endif

}

void CETListCtrl::enableButton( BOOL bFlag, int iItem )
{
	button_map::iterator iter;
#ifdef USE_TOPINDEX_BUTTON
	int iTopIndex = GetTopIndex();
	int nItem = iItem - iTopIndex;
	iter = m_mButton.find( iItem );
	if ( iter != m_mButton.end() )
	{
		iter->second->bEnable = bFlag;
	}
	iter = m_mButton.find( nItem );
	if ( iter != m_mButton.end() )
	{
		iter->second->EnableWindow( bFlag );
	}
#else
	iter = m_mButton.find( iItem );
	if ( iter != m_mButton.end() )
	{
		iter->second->EnableWindow( bFlag );
	}
#endif

}
