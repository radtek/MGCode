// CmdArray.h: interface for the CCmdArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDARRAY_H__8DBD87A5_DCCA_11D4_A485_00105A615525__INCLUDED_)
#define AFX_CMDARRAY_H__8DBD87A5_DCCA_11D4_A485_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdArray : public CCommand  
{
public:
	CCmdArray(CHLDoc *pDoc, CArray<OBJPOS, OBJPOS> &dtArray, BOOL flag);
	virtual ~CCmdArray();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;

};

#endif // !defined(AFX_CMDARRAY_H__8DBD87A5_DCCA_11D4_A485_00105A615525__INCLUDED_)
