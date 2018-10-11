// DlgFileReplyRename.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "DlgFileReplyRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFileReplyRename dialog


CDlgFileReplyRename::CDlgFileReplyRename(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileReplyRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFileReplyRename)
	m_strPromptMessage = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFileReplyRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFileReplyRename)
	DDX_Text(pDX, IDC_File_Prompt, m_strPromptMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFileReplyRename, CDialog)
	//{{AFX_MSG_MAP(CDlgFileReplyRename)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFileReplyRename message handlers
