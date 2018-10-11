// HL.h : main header file for the HL application
//

#if !defined(AFX_HL_H__B8BC1847_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_HL_H__B8BC1847_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define ENABLEDOG

#include "resource.h"       // main symbols
#include "Stroke.h"
#include "RedMarkThread.h"
#include "SelMark.h"
#include "FastMark.h"
#include "HLMotion.h"
#include "OpcLaser.h"

extern BOOL g_bLaserReady;
extern COpcLaser* g_OPCLaser;
extern BOOL g_bReady;
extern double  g_nFreq; 


/////////////////////////////////////////////////////////////////////////////
// CHLApp:
// See HL.cpp for the implementation of this class

class CHLApp : public CWinAppEx
{
public:
	CHLApp();

	// Retrieve an integer value from INI file or registry.
	virtual UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);

	// Sets an integer value to INI file or registry.
	virtual BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);

	// Retrieve a string value from INI file or registry.
	virtual CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL);

	// Sets a string value to INI file or registry.
	virtual BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);

	// Retrieve an arbitrary binary value from INI file or registry.
	virtual BOOL GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes);

	// Sets an arbitrary binary value to INI file or registry.
	virtual BOOL WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes);

	void SetDATCard();
	double GetProfileDouble( LPCTSTR lpszSection, LPCTSTR lpszEntry, double dbDefault =0);
	BOOL WriteProfileDouble( LPCTSTR lpszSection, LPCTSTR lpszEntry, double dbValue );
	
	CString GetInstallPath();
	void OnMarkMul();

    UINT LoadImageProcLib();

// 	RITHT m_right;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHLApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CHLApp)
	afx_msg void OnAppAbout();
	afx_msg void OnSetHardware();
	afx_msg void OnOption();
	afx_msg void OnSetTracker();
	afx_msg void OnFileMuldoc();
	afx_msg void OnMarkStat();
	afx_msg void OnCalibrate();
	afx_msg void OnGrayCurrent();
	afx_msg void OnMenuFontFileNew();
	afx_msg void OnUpdateMenuFontFileNew(CCmdUI *pCmdUI);
	afx_msg void OnMenuFontFileOpen();
	afx_msg void OnUpdateMenuFontFileOpen(CCmdUI *pCmdUI);
	afx_msg void OnMenuFontFileSave();
	afx_msg void OnUpdateMenuFontFileSave(CCmdUI *pCmdUI);
	afx_msg void OnMenuFontFileSaveAs();
	afx_msg void OnUpdateMenuFontFileSaveAs(CCmdUI *pCmdUI);
	afx_msg void OnSetWorkbench();
	afx_msg void OnGridCali();
	afx_msg void OnCcdOperation();
	afx_msg void OnCcdView();
	afx_msg void OnFileMulDocOpen();
	afx_msg void OnFileMulDocSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CSelMark       m_SelMark;
	CFastMark      m_FastMark;
	CRedMarkThread m_RedPreView;

	GdiplusStartupInput m_Gdistart; 
	ULONG_PTR m_GdiplusToken;

private:
	HINSTANCE   m_hLangDLL;
	HMODULE     m_hLib;
	HANDLE m_hMutexNew;
	CString m_strAppIni;

public:
	//afx_msg void OnNetSet();
	afx_msg void OnChangedoc();
	afx_msg void OnPowerTest();
	BOOL OnDDECommand(LPTSTR lpszCommand);
	afx_msg void OnMatrixDocSet();
	afx_msg void OnSetDatabase();
	afx_msg void OnAgingTest();
	afx_msg void OnUpdateAgingTest(CCmdUI *pCmdUI);
};

extern HANDLE g_hEventMainThreadEmergency;
extern CStringArray g_arrMulDoc;

extern CHLMotion *g_pMotion;
extern CHLApp theApp;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HL_H__B8BC1847_DE24_11D3_845D_444553540001__INCLUDED_)
