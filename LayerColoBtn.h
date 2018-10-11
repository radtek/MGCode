#ifndef __LAYERCOLORBTN_H__
#define __LAYERCOLORBTN_H__


class CLayerColorButton : public CButton
{
DECLARE_DYNAMIC(CLayerColorButton)

public:
	CLayerColorButton(); 
	virtual ~CLayerColorButton(); 

public:
	BOOL Attach(
                const COLORREF BGColor = RGB(192, 192, 192),
                const COLORREF FGColor = RGB(1, 1, 1),
                const COLORREF DisabledColor = RGB(128, 128, 128),
                const UINT nBevel = 2
               );
	
	void SetBGColor(const COLORREF BGColor, BOOL bIsMark = FALSE);
	void DrawEllipse(CDC *pDC, CRect R);
	void DrawMark(BOOL bIsMark);
//	void ReDrawButton();

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
			void DrawFrame(CDC *DC, CRect R, int Inset);
			void DrawFilledRect(CDC *DC, CRect R, COLORREF color);
			void DrawLine(CDC *DC, CRect EndPoints, COLORREF color);
			void DrawLine(CDC *DC, long left, long top, long right, long bottom, COLORREF color);
			void DrawButtonText(CDC *DC, CRect R, const char *Buf, COLORREF TextColor);

			COLORREF GetFGColor() { return m_fgColor; }	
			COLORREF GetBGColor() { return m_bgColor; }
			COLORREF GetDisabledColor() { return m_disabledColor; }
			UINT GetBevel() { return m_bevel; }

private:
	COLORREF m_fgColor, m_bgColor, m_disabledColor;
	UINT     m_bevel;
	BOOL     m_bIsMark;
};
#endif 
