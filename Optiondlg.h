#if !defined(AFX_OPTIONDLG_H__50FEF0A4_3FE6_11D4_B789_89FA424E960F__INCLUDED_)
#define AFX_OPTIONDLG_H__50FEF0A4_3FE6_11D4_B789_89FA424E960F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionDlg.h : header file
//


#pragma warning(disable:4786)
//#include "xtree.h"
#include <xtree>
#include <vector>
#include <map>
#include <string>

using namespace std;

enum ENU_OPERATE_TYPE
{
	ENU_EXIT_OK	    =   0X01,   // 确定         
	ENU_EXIT_CANCEL	=   0X02,   // 取消
	ENU_EXIT_APPLAY	=   0X03,   // 应用
	ENU_EXIT_FLUSH_UI	=   0X04,   // 把文档中的值刷新到界面
	ENU_EXIT_SAVE	=   0X05,   // 把界面中上值保存到文档
	ENU_EXIT_END
};

typedef BOOL(*UPDATEPROPERTYLIST)(void*, DWORD &dwProID);
typedef DWORD (WINAPI *pGetKeyList)(std::vector<CString> &vecKeyList, CString &strCfgName);
typedef DWORD (WINAPI *pFlushToPropList)();
typedef CWnd* (WINAPI *pCreateObjPropertyCtrl)(DWORD dwStyle,char* pszCfgName,char*pszIniName, HWND hPrarentWnd ,UPDATEPROPERTYLIST UpdatePropertyList);


/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

class COptionDlg : public CDialog
{
// Construction
public:
	COptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_OPTION };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
//	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:


	HMODULE m_hMarkPropModule;

	pCreateObjPropertyCtrl CreateObjPropertyCtrl;
	pGetKeyList            GetKeyList;
	pFlushToPropList       FlushToPropList;
	
	void CreateSysPropList();
	BOOL LoadMarkerPropLib();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDLG_H__50FEF0A4_3FE6_11D4_B789_89FA424E960F__INCLUDED_)
