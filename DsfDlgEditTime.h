#pragma once


// CDsfDlgEditTime dialog

class CDsfDlgEditTime : public CDialogEx
{
	DECLARE_DYNAMIC(CDsfDlgEditTime)

public:
	CDsfDlgEditTime(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDsfDlgEditTime();

// Dialog Data
	enum { IDD = IDD_DSF_DLG_EDITTIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nTime;
};
