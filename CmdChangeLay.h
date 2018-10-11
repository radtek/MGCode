// CmdFill.h: interface for the CCmdChangeLay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDFILL_H__F1BA3841_4DCB_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDFILL_H__F1BA3841_4DCB_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdChangeLay : public CCommand  
{
public:
	CCmdChangeLay(CHLDoc *pDoc);
	virtual ~CCmdChangeLay();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
	void SetVerFillPara(double spaceVer,double spaceVerOld);
protected:
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
	CHLDoc * m_pDoc;
	double m_dbFillSpaceHor;
	double m_dbFillSpaceVer;
	int m_nFillStyle;

	double m_dbOldSpaceHor;
	double m_dbOldSpaceVer;
	int m_nOldStyle;
};

#endif // !defined(AFX_CMDFILL_H__F1BA3841_4DCB_11D4_A483_00105A615525__INCLUDED_)
