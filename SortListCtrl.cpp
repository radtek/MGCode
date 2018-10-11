#include "stdafx.h"
#include "SortListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  ID_LIST_PRINT     200
#define  ID_LIST_SAVE      201
#define  ID_LIST_DELETE    202
#define	 ID_LIST_DELETEALL 203

LPCTSTR g_pszSection = _T("ListCtrls");

struct ItemData
{
public:
	ItemData():arrpsz( NULL ), dwData( NULL ),crText ( NULL ),crBak (NULL) {}
	LPTSTR   * arrpsz;
	DWORD      dwData;
	COLORREF * crText;
	COLORREF * crBak;
	
private:
	ItemData( const ItemData& );
	ItemData& operator=( const ItemData& );
};


CSortListCtrl::CSortListCtrl(): m_nNumColumns( 0 ), m_iSortColumn( -1 ), m_bSortAscending( TRUE )
{
	crWindow        = ::GetSysColor(COLOR_WINDOW);
	crWindowText    = ::GetSysColor(COLOR_WINDOWFRAME);
	crHighLight     = ::GetSysColor(COLOR_HIGHLIGHT);
	crHighLightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
}


CSortListCtrl::~CSortListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSortListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CSortListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl message handlers

void CSortListCtrl::PreSubclassWindow()
{
	ASSERT( GetStyle() & LVS_REPORT );

	CListCtrl::PreSubclassWindow();
	HWND hWnd = GetHeaderCtrl()->GetSafeHwnd();
	VERIFY( m_ctlHeader.SubclassWindow( hWnd ) );
}

BOOL CSortListCtrl::SetHeadings( UINT uiStringID )
{
	CString strHeadings;
	VERIFY( strHeadings.LoadString( uiStringID ) );
	return SetHeadings( strHeadings );
}

BOOL CSortListCtrl::SetHeadings( const CString& strHeadings )
{
	int iStart = 0;
	for( ;; )
	{
		const int iComma = strHeadings.Find( _T(','), iStart );
		if( iComma == -1 )
		{
			break;
		}
		
		const CString strHeading = strHeadings.Mid( iStart, iComma - iStart );
		iStart = iComma + 1;
		int iSemiColon = strHeadings.Find( _T(';'), iStart );

		if( iSemiColon == -1 )
		{
			iSemiColon = strHeadings.GetLength();
		}

		const int iWidth = atoi( strHeadings.Mid( iStart, iSemiColon - iStart ) );
		iStart = iSemiColon + 1;

		switch(m_nNumColumns)
		{
		case 0:
			{
				if(InsertColumn(m_nNumColumns, strHeading, LVCFMT_LEFT, iWidth ) == -1)
				{
					return FALSE;
				}
				break;
			}
		case 1:
			{
				if(InsertColumn(m_nNumColumns, strHeading, LVCFMT_LEFT, iWidth ) == -1)
				{
					return FALSE;
				}
				break;
			}
		case 2:
			{
				if(InsertColumn(m_nNumColumns, strHeading, LVCFMT_CENTER, iWidth ) == -1)
				{
					return FALSE;
				}
				break;
			}
		default:
			{
				if(InsertColumn(m_nNumColumns, strHeading, LVCFMT_CENTER, iWidth ) == -1)
				{
					return FALSE;
				}
				break;
			}
		}

		m_nNumColumns++;
	}

	return TRUE;
}


int CSortListCtrl::AddItem( LPCTSTR pszText, ... )
{
	const int iIndex   = InsertItem( GetItemCount(), pszText, 1);
	
	LPTSTR* arrpsz     = new LPTSTR[m_nNumColumns];
	COLORREF * clrText = new COLORREF[m_nNumColumns];
	COLORREF * clrBak  = new COLORREF[m_nNumColumns];

	arrpsz[ 0 ]  = new TCHAR[ lstrlen( pszText ) + 1 ];
	clrText[ 0 ] = crWindowText;
	clrBak[ 0 ]  = crWindow;
	(void)lstrcpy( arrpsz[ 0 ], pszText );

 	va_list list;
	va_start( list, pszText );
	for( int iColumn = 1; iColumn < m_nNumColumns; iColumn++ )
	{
		pszText = va_arg( list, LPCTSTR );
		ASSERT_VALID_STRING( pszText );
		VERIFY( CListCtrl::SetItem( iIndex, iColumn, LVIF_TEXT, pszText, 0, 0, 0, 0 ) );

		arrpsz[ iColumn ] = new TCHAR[ lstrlen( pszText ) + 1 ];
		clrText[ iColumn ] = crWindowText;
		clrBak[ iColumn ] = crWindow;
		(void)lstrcpy( arrpsz[ iColumn ], pszText );
	}
	va_end( list );
	VERIFY( SetArray( iIndex, arrpsz,clrText,clrBak ) );

	return iIndex;
}


void CSortListCtrl::FreeItemMemory( const int iItem )
{
	ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) );

	LPTSTR* arrpsz = pid->arrpsz;

	for( int i = 0; i < m_nNumColumns; i++ )
	{
		delete[] arrpsz[ i ];	
	}
	delete[] pid->crText;
	delete[] pid->crBak;
	delete[] arrpsz;
	delete pid;

	VERIFY( CListCtrl::SetItemData( iItem, NULL ) );
}

BOOL CSortListCtrl::DeleteItem( int iItem )
{
	FreeItemMemory( iItem );
	return CListCtrl::DeleteItem( iItem );
}

BOOL CSortListCtrl::DeleteAllItems()
{
	for( int iItem = 0; iItem < GetItemCount(); iItem ++ )
		FreeItemMemory( iItem );

	return CListCtrl::DeleteAllItems();
}

BOOL IsNumber( LPCTSTR pszText )
{
	ASSERT_VALID_STRING( pszText );

	for( int i = 0; i < lstrlen( pszText ); i++ )
	{
		if( !_istdigit( pszText[ i ] ) ) return FALSE;
	}

	return TRUE;
}

int NumberCompare( LPCTSTR pszNumber1, LPCTSTR pszNumber2 )
{
	ASSERT_VALID_STRING( pszNumber1 );
	ASSERT_VALID_STRING( pszNumber2 );

	const int iNumber1 = atoi( pszNumber1 );
	const int iNumber2 = atoi( pszNumber2 );

	if( iNumber1 < iNumber2 )
		return -1;
	
	if( iNumber1 > iNumber2 )
		return 1;

	return 0;
}

BOOL IsDate( LPCTSTR pszText )
{
	ASSERT_VALID_STRING( pszText );

	// format should be 99/99/9999.
	if( lstrlen( pszText ) != 10 )
		return FALSE;

	return _istdigit( pszText[ 0 ] )
		&& _istdigit( pszText[ 1 ] )
		&& pszText[ 2 ] == _T('/')
		&& _istdigit( pszText[ 3 ] )
		&& _istdigit( pszText[ 4 ] )
		&& pszText[ 5 ] == _T('/')
		&& _istdigit( pszText[ 6 ] )
		&& _istdigit( pszText[ 7 ] )
		&& _istdigit( pszText[ 8 ] )
		&& _istdigit( pszText[ 9 ] );
}

int DateCompare( const CString& strDate1, const CString& strDate2 )
{
	const int iYear1 = atoi( strDate1.Mid( 6, 4 ) );
	const int iYear2 = atoi( strDate2.Mid( 6, 4 ) );

	if( iYear1 < iYear2 )
		return -1;

	if( iYear1 > iYear2 )
		return 1;

	const int iMonth1 = atoi( strDate1.Mid( 3, 2 ) );
	const int iMonth2 = atoi( strDate2.Mid( 3, 2 ) );

	if( iMonth1 < iMonth2 )
		return -1;

	if( iMonth1 > iMonth2 )
		return 1;

	const int iDay1 = atoi( strDate1.Mid( 0, 2 ) );
	const int iDay2 = atoi( strDate2.Mid( 0, 2 ) );

	if( iDay1 < iDay2 )
		return -1;

	if( iDay1 > iDay2 )
		return 1;

	return 0;
}

int CALLBACK CSortListCtrl::CompareFunction( LPARAM lParam1, LPARAM lParam2, LPARAM lParamData )
{
	CSortListCtrl* pListCtrl = reinterpret_cast<CSortListCtrl*>( lParamData );
	ASSERT( pListCtrl->IsKindOf( RUNTIME_CLASS( CListCtrl ) ) );

	ItemData* pid1 = reinterpret_cast<ItemData*>( lParam1 );
	ItemData* pid2 = reinterpret_cast<ItemData*>( lParam2 );

	ASSERT( pid1 );
	ASSERT( pid2 );

	LPCTSTR pszText1 = pid1->arrpsz[ pListCtrl->m_iSortColumn ];
	LPCTSTR pszText2 = pid2->arrpsz[ pListCtrl->m_iSortColumn ];

	ASSERT_VALID_STRING( pszText1 );
	ASSERT_VALID_STRING( pszText2 );

	if( IsNumber( pszText1 ) )
		return pListCtrl->m_bSortAscending ? NumberCompare( pszText1, pszText2 ) : NumberCompare( pszText2, pszText1 );
	else if( IsDate( pszText1 ) )
		return pListCtrl->m_bSortAscending ? DateCompare( pszText1, pszText2 ) : DateCompare( pszText2, pszText1 );
	else
		return pListCtrl->m_bSortAscending ? lstrcmp( pszText1, pszText2 ) : lstrcmp( pszText2, pszText1 );
}

void CSortListCtrl::OnColumnClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iColumn = pNMListView->iSubItem;
	*pResult = 0;
}

void CSortListCtrl::Sort( int iColumn, BOOL bAscending )
{
	return;
	m_iSortColumn = iColumn;
	m_bSortAscending = bAscending;
	m_ctlHeader.SetSortArrow( m_iSortColumn, m_bSortAscending );
	VERIFY( SortItems( CompareFunction, reinterpret_cast<DWORD>( this ) ) );
}

void CSortListCtrl::LoadColumnInfo()
{
	ASSERT( m_nNumColumns > 0 );

	CString strKey;
	strKey.Format( _T("%d"), GetDlgCtrlID() );

	UINT nBytes = 0;
	BYTE* buf = NULL;
	if( AfxGetApp()->GetProfileBinary( g_pszSection, strKey, &buf, &nBytes ) )
	{
		if( nBytes > 0 )
		{
			CMemFile memFile( buf, nBytes );
			CArchive ar( &memFile, CArchive::load );
			m_ctlHeader.Serialize( ar );
			ar.Close();

			m_ctlHeader.Invalidate();
		}
		delete[] buf;
	}
}

void CSortListCtrl::SaveColumnInfo()
{
	ASSERT( m_nNumColumns > 0 );

	CString strKey;
	strKey.Format( _T("%d"), GetDlgCtrlID() );

	CMemFile memFile;
	CArchive ar( &memFile, CArchive::store );
	m_ctlHeader.Serialize( ar );
	ar.Close();

	DWORD dwLen = (DWORD)memFile.GetLength();
	BYTE* buf = memFile.Detach();	

	VERIFY( AfxGetApp()->WriteProfileBinary( g_pszSection, strKey, buf, dwLen ) );

	free( buf );
}


void CSortListCtrl::OnDestroy() 
{
	for( int iItem = 0; iItem < GetItemCount(); iItem ++ )
		FreeItemMemory( iItem );
	CListCtrl::OnDestroy();
}


BOOL CSortListCtrl::SetItemText( int nItem, int nSubItem, LPCTSTR lpszText )
{
	if (0 == GetItemCount()) 
	{
		return FALSE;
	}
	
	if(!CListCtrl::SetItemText( nItem, nSubItem, lpszText ))
		return FALSE;

	LPTSTR* arrpsz = GetTextArray( nItem );
	LPTSTR pszText = arrpsz[ nSubItem ];
	delete[] pszText;
	pszText = new TCHAR[lstrlen( lpszText ) + 1 ];
	(void)lstrcpy( pszText, lpszText );
	arrpsz[ nSubItem ] = pszText;

	return TRUE;
}


BOOL CSortListCtrl::SetItemData( int nItem, DWORD dwData )
{
/*
		if( nItem >= GetItemCount() )
			return FALSE;
	
		ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( nItem ) );
		ASSERT( pid );
		pid->dwData = dwData;
	
		return TRUE;*/
	CListCtrl::SetItemData( nItem,  dwData);
	return TRUE;
}

DWORD CSortListCtrl::GetItemData( int nItem ) const
{
		ASSERT( nItem < GetItemCount() );
		ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( nItem ) );
		ASSERT( pid );
		return pid->dwData;
}

BOOL CSortListCtrl::SetArray( int iItem, LPTSTR* arrpsz,COLORREF * clrText,COLORREF * clrBak )
{
//	ASSERT( CListCtrl::GetItemData( iItem ) == NULL );
	ItemData* pid = new ItemData;
	pid->arrpsz = arrpsz;
	pid->crText = clrText;
	pid->crBak = clrBak;
	return CListCtrl::SetItemData( iItem, reinterpret_cast<DWORD>( pid ) );//set item data /
}

LPTSTR* CSortListCtrl::GetTextArray( int iItem ) const
{
	ASSERT( iItem < GetItemCount() );

	ItemData* pid = reinterpret_cast<ItemData*>( CListCtrl::GetItemData( iItem ) );
	return pid->arrpsz;
}

void CSortListCtrl::OnPaint() 
{
	Default();
	CPaintDC dc(this);
}

void CSortListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	int nItem    = static_cast<int> (pLVCD->nmcd.dwItemSpec);
	int nSubItem = pLVCD->iSubItem;

	*pResult = CDRF_DODEFAULT;
	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		ItemData *pXLCD = (ItemData *) pLVCD->nmcd.lItemlParam;
		ASSERT(pXLCD);
		
		COLORREF crText  = crWindowText;
		COLORREF crBkgnd = crWindow;
		
		if (pXLCD){
			crText  = (pXLCD->crText)[nSubItem];
			crBkgnd = (pXLCD->crBak)[nSubItem];
		}
								
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;
		
		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
		if (GetItemState(nItem, LVIS_SELECTED) && (1 != nSubItem))
		{
			DrawText(nItem, nSubItem, pDC, crHighLightText, crHighLight , rect);
		}
		else
		{
			DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect);
		}
				
		*pResult = CDRF_SKIPDEFAULT;
	}
}

int CSortListCtrl::AddItemColor(LPCTSTR pszText, COLORREF crText, COLORREF crBak)
{
	const int iIndex = InsertItem( GetItemCount(), pszText );
	ItemData *m_pSortItemData=new ItemData[GetColumns()];
/*
	m_pSortItemData[0].crText=crText;
	m_pSortItemData[0].crBak=crBak;
*/
	SetItemData(iIndex,(DWORD) m_pSortItemData);

	return iIndex;
}

int CSortListCtrl::GetColumns()
{
	return m_ctlHeader.GetItemCount();

}

BOOL CSortListCtrl::GetSubItemRect(int nItem, int nSubItem, int nArea, CRect &rect)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;
	
	BOOL bRC = CListCtrl::GetSubItemRect(nItem, nSubItem, nArea, rect);
	if (nSubItem == 0)
	{
		if (GetColumns() > 1)
		{
			CRect rect1;
			bRC = GetSubItemRect(nItem, 1, LVIR_BOUNDS, rect1);
			rect.right = rect1.left;
		}
	}
	
	return bRC;
}

void CSortListCtrl::DrawText(int nItem, int nSubItem, CDC *pDC, COLORREF crText, COLORREF crBkgnd, CRect &rect)
{
	ASSERT(pDC);
	pDC->FillSolidRect(&rect, crBkgnd);
	
	CString str;
	str = GetItemText(nItem, nSubItem);
	if (!str.IsEmpty())
	{
		HDITEM hditem;
		hditem.mask = HDI_FORMAT;
		m_ctlHeader.GetItem(nSubItem, &hditem);
		int nFmt     = hditem.fmt & HDF_JUSTIFYMASK;
		UINT nFormat = DT_VCENTER | DT_SINGLELINE;

		if (nFmt == HDF_CENTER)
			nFormat |= DT_CENTER;
		else if (nFmt == HDF_LEFT)
			nFormat |= DT_LEFT;
		else
			nFormat |= DT_RIGHT;
		
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		pDC->DrawText(str, &rect, nFormat);
	}
	return;
}

void CSortListCtrl::OnSysColorChange() 
{
	CListCtrl::OnSysColorChange();

	crWindow        = ::GetSysColor(COLOR_WINDOW);
	crWindowText    = ::GetSysColor(COLOR_WINDOWTEXT);
	crHighLight     = ::GetSysColor(COLOR_HIGHLIGHT);
	crHighLightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);

	return;
}

void CSortListCtrl::SetItemColor(int nItem, int nSubItem, COLORREF clrText, COLORREF clrBkgnd)
{
	ASSERT(nItem >= 0);
	if (0 == GetItemCount()) 
	{
		return;
	}
	
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())    return ;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns()) return ;

	if (nItem < 0)  return ;
	
	ItemData *pid = (ItemData*) CListCtrl::GetItemData(nItem);
	if (pid)
	{
		(pid->crText)[nSubItem]	 = (clrText == -1) ? crWindowText : clrText;
		(pid->crBak)[nSubItem]	 = (clrBkgnd == -1) ? crWindow : clrBkgnd;
	}
	
	UpdateSubItem(nItem, nSubItem);

	return;
}

void CSortListCtrl::UpdateSubItem(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return;
	
	CRect rect;
	if (nSubItem == -1)
	{
		GetItemRect(nItem, &rect, LVIR_BOUNDS);
	}
	else
	{
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	}

	InvalidateRect(&rect);
	UpdateWindow();

	return;
}

int CSortListCtrl::GetCurSel() const
{
	return m_nCurRow;
}

void CSortListCtrl::SetCurSel(int nIndex)
{
	m_nCurRow = nIndex;
}

void CSortListCtrl::GetCurRwColm(int &nItem, int &nSubItem)
{
	nItem    = m_nCurRow;
	nSubItem =m_nCurColm;
}