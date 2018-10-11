// HlDACard.h : main header file for the HLDACARD DLL
//

#if !defined(AFX_HLDACARD_H__57E1391C_0B8A_11D8_9D95_00E04C779BB9__INCLUDED_)
#define AFX_HLDACARD_H__57E1391C_0B8A_11D8_9D95_00E04C779BB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHlDACardApp
// See HlDACard.cpp for the implementation of this class
//

class CHlDACardApp : public CWinApp
{
public:
	CHlDACardApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHlDACardApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHlDACardApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HLDACARD_H__57E1391C_0B8A_11D8_9D95_00E04C779BB9__INCLUDED_)
