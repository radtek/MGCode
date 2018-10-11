#include "stdafx.h"
#include "LayerColoBtn.h"
  
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


#ifdef CLayerColorButton
#undef CLayerColorButton      CLayerColorButton
#endif

// CLayerColorButton
IMPLEMENT_DYNAMIC(CLayerColorButton, CButton)

CLayerColorButton::CLayerColorButton() 
{  
#if (_MFC_VER < 0x0250)
  hwndOwner = NULL;  // initialize hwndOwner for GetOwner() and SetOwner() support in MFC < 2.5
#endif 
  m_bIsMark = FALSE;
} 



CLayerColorButton::~CLayerColorButton()
{
} 



BOOL CLayerColorButton::Attach(const COLORREF BGColor, const COLORREF FGColor, const COLORREF DisabledColor, const UINT nBevel)
{
	m_fgColor       = FGColor;
	m_bgColor       = BGColor; 
	m_disabledColor = DisabledColor;
	m_bevel         = nBevel;

	return TRUE;
} 

void CLayerColorButton::SetBGColor(const COLORREF BGColor, BOOL bIsMark)
{
	m_bgColor  = BGColor; 
	m_bIsMark  = bIsMark;
	Invalidate();
	return;
}

void CLayerColorButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	UINT state = lpDIS->itemState; 
	CRect fcRect, btnRect;
	fcRect.CopyRect(&lpDIS->rcItem); 
	btnRect.CopyRect(&lpDIS->rcItem); 

	fcRect.left   += 4; fcRect.right  -= 4;
    fcRect.top    += 4; fcRect.bottom -= 4;

    DrawFilledRect(pDC, btnRect, GetBGColor()); 
    DrawFrame(pDC, btnRect, GetBevel());

	if (state & ODS_FOCUS) 
	{
		DrawFocusRect(lpDIS->hDC, (LPRECT)&fcRect);
		if (state & ODS_SELECTED)
		{ 
    		DrawFilledRect(pDC, btnRect, GetBGColor()); 
    		DrawFrame(pDC, btnRect, -1);
			DrawFocusRect(lpDIS->hDC, (LPRECT)&fcRect);
		}
	}

	if (m_bIsMark)
	{
		CRect mRect;
	    mRect.CopyRect(&lpDIS->rcItem); 
		mRect.left   += 6; mRect.right  -= 6;
		mRect.top    += 6; mRect.bottom -= 6;
		DrawEllipse(pDC, mRect);
	}
	return;
} 

void CLayerColorButton::DrawMark(BOOL bIsMark)
{
	m_bIsMark = bIsMark;
	Invalidate();
	return;
} 


void CLayerColorButton::DrawFrame(CDC *DC, CRect R, int Inset)
{ 
	COLORREF dark, light, tlColor, brColor;
	int i, m, width;
	width = (Inset < 0)? -Inset : Inset;
	
	for (i = 0; i < width; i += 1) {
		m = 255 / (i + 2);
		dark = PALETTERGB(m, m, m);
		m = 192 + (63 / (i + 1));
		light = PALETTERGB(m, m, m);
		  
	  	if ( width == 1 ) {
			light = RGB(255, 255, 255);
			dark = RGB(128, 128, 128);
		}
		
		if ( Inset < 0 ) {
			tlColor = dark;
			brColor = light;
		}
		else {
			tlColor = light;
			brColor = dark;
		}
		
		DrawLine(DC, R.left, R.top, R.right, R.top, tlColor);							// Across top
		DrawLine(DC, R.left, R.top, R.left, R.bottom, tlColor);							// Down left
	  
		if ( (Inset < 0) && (i == width - 1) && (width > 1) ) {
			DrawLine(DC, R.left + 1, R.bottom - 1, R.right, R.bottom - 1, RGB(1, 1, 1));// Across bottom
			DrawLine(DC, R.right - 1, R.top + 1, R.right - 1, R.bottom, RGB(1, 1, 1));	// Down right
		}
	  	else {
			DrawLine(DC, R.left + 1, R.bottom - 1, R.right, R.bottom - 1, brColor);		// Across bottom
			DrawLine(DC, R.right - 1, R.top + 1, R.right - 1, R.bottom, brColor);		// Down right
		}
	  	InflateRect(R, -1, -1);
	}
}



void CLayerColorButton::DrawFilledRect(CDC *DC, CRect R, COLORREF color)
{ 
	CBrush B;
	B.CreateSolidBrush(color);
	DC->FillRect(R, &B);
}
 

void CLayerColorButton::DrawLine(CDC *DC, CRect EndPoints, COLORREF color)
{ 
	CPen newPen;
	newPen.CreatePen(PS_SOLID, 1, color);
	CPen *oldPen = DC->SelectObject(&newPen);
	DC->MoveTo(EndPoints.left, EndPoints.top);
	DC->LineTo(EndPoints.right, EndPoints.bottom);
	DC->SelectObject(oldPen);
    newPen.DeleteObject();
}

void CLayerColorButton::DrawLine(CDC *DC, long left, long top, long right, long bottom, COLORREF color)
{ 
	CPen newPen;
	newPen.CreatePen(PS_SOLID, 1, color);
	CPen *oldPen = DC->SelectObject(&newPen);
	DC->MoveTo(left, top);
	DC->LineTo(right, bottom);
	DC->SelectObject(oldPen);
    newPen.DeleteObject();
}

void CLayerColorButton::DrawEllipse(CDC *pDC, CRect R)
{ 
	CBrush *pOldBrush = (CBrush *)pDC->SelectStockObject(WHITE_BRUSH);
	pDC->Ellipse(&R);
	pDC->SelectObject(pOldBrush);
}

void CLayerColorButton::DrawButtonText(CDC *DC, CRect R, const char *Buf, COLORREF TextColor)
{
    COLORREF prevColor = DC->SetTextColor(TextColor);
    DC->SetBkMode(TRANSPARENT);
	DC->DrawText(Buf, strlen(Buf), R, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	DC->SetTextColor(prevColor);
}


		







