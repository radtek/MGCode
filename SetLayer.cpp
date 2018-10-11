// SetLayer.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "SetLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLayer dialog


CSetLayer::CSetLayer(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLayer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLayer)
	m_nLayer = 0;
	m_nFillLayer = 0;
	//}}AFX_DATA_INIT
}


void CSetLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLayer)
	DDX_Text(pDX, IDC_EDIT_LAYER, m_nLayer);
	//DDV_MinMaxInt(pDX, m_nLayer, 0, 6); //Updated by AndyF.Shelby on 17 Feb. 2006
	DDX_Text(pDX, IDC_EDIT_FILL, m_nFillLayer);
	//DDV_MinMaxUInt(pDX, m_nFillLayer, 0, 6); //Updated by AndyF.Shelby on 17 Feb. 2006
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLayer, CDialog)
	//{{AFX_MSG_MAP(CSetLayer)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetLayer message handlers
