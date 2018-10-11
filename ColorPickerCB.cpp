
#include "stdafx.h"
#include "ColorPickerCB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CColorPickerCB::CColorPickerCB()
{
}


CColorPickerCB::~CColorPickerCB()
{
}


BEGIN_MESSAGE_MAP(CColorPickerCB, CComboBox)
	//{{AFX_MSG_MAP(CColorPickerCB)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPickerCB message handlers

int CColorPickerCB::OnCreate( LPCREATESTRUCT pCStruct ) 
{
	if( CComboBox::OnCreate( pCStruct ) == -1 )				
		return( -1 );										

	return( 0 );											
}


//按照列表项的颜色值和名称重画列表框
void CColorPickerCB::DrawItem( LPDRAWITEMSTRUCT pDIStruct )
{
	static		CString	sColor;								

	CDC			dcContext;
	CRect		rItemRect( pDIStruct -> rcItem );
	CRect		rBlockRect( rItemRect );
	CRect		rTextRect( rBlockRect );
	CBrush		brFrameBrush;
	int			iFourthWidth = 0;
	int			iItem = pDIStruct -> itemID;
	int			iAction = pDIStruct -> itemAction;
	int			iState = pDIStruct -> itemState;
	COLORREF	crColor = 0;
	COLORREF	crNormal = GetSysColor( COLOR_WINDOW );
	COLORREF	crSelected = GetSysColor( COLOR_HIGHLIGHT );
	COLORREF	crText = GetSysColor( COLOR_WINDOWTEXT );

	// 联系 CDC对象
	if( !dcContext.Attach( pDIStruct -> hDC ) )				
		return;			

	//获得1/4的列表项宽度
	iFourthWidth = ( rBlockRect.Width() / 2 );				
	//创建黑色画刷
	brFrameBrush.CreateStockObject( BLACK_BRUSH );			

	//如果该条目被选择
	if( iState & ODS_SELECTED )								
	{										
		
		dcContext.SetTextColor(
				( 0x00FFFFFF & ~( crText ) ) );				
		dcContext.SetBkColor( crSelected );					
		dcContext.FillSolidRect( &rBlockRect, crSelected );	
	}
	else//如果没有被选择
	{
		dcContext.SetTextColor( crText );					
		dcContext.SetBkColor( crNormal );					
		dcContext.FillSolidRect( &rBlockRect, crNormal );	
	}
	//如果该项出于焦点状态
	if( iState & ODS_FOCUS )								
		dcContext.DrawFocusRect( &rItemRect );				

	//计算文字区域
	rTextRect.left += ( iFourthWidth + 2 );					
	rTextRect.top += 2;										
	
	//计算图像区域
	rBlockRect.DeflateRect( CSize( 2, 2 ) );				
	rBlockRect.right = iFourthWidth;						
	
	//显示文字(颜色名)
	if( iItem != -1 )										
	{
		GetLBText( iItem, sColor );							

		//如果处于不可用状态
		if( iState & ODS_DISABLED )							
		{
			crColor = GetSysColor( COLOR_INACTIVECAPTIONTEXT );
			dcContext.SetTextColor( crColor );				
		}
		else //如果处于一般状态
			crColor = GetItemData( iItem );					

		dcContext.SetBkMode( TRANSPARENT );				

		dcContext.TextOut( rTextRect.left, rTextRect.top,sColor );									

		//填充颜色区域
		dcContext.FillSolidRect( &rBlockRect, crColor );	
		
		//画边框
		dcContext.FrameRect( &rBlockRect, &brFrameBrush );	
	}
	//分离CDC对象
	dcContext.Detach();										
}


//增加指定颜色名称和颜色值的列表项
int	CColorPickerCB::AddColor(COLORREF crColor )
{
	int	 iItems = GetCount();
	char szName[64];
	sprintf_s(szName, 64, "层%d", iItems);
	int iItem = InsertString( iItems, szName );						
	if( iItem != LB_ERR )
	{
		SetItemData( iItem, crColor );
	}

	return( iItem );										
}


	
