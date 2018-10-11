#pragma once


// CDsfDockablePane

class CDsfDockablePane : public CDockablePane
{
	DECLARE_DYNAMIC(CDsfDockablePane)

public:
	CDsfDockablePane();
	virtual ~CDsfDockablePane();
	virtual BOOL OnShowControlBarMenu(CPoint point);
	virtual void OnPaneContextMenu(CWnd* pParentFrame,CPoint point);
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};


