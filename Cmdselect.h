// CmdSelect.h: interface for the CCmdSelect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSELECT_H__583122A5_46BF_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDSELECT_H__583122A5_46BF_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdSelect : public CCommand  
{
public:
	CCmdSelect(CHLDoc *pDoc,BOOL *pFlag1,BOOL *pFlag2,int count);
	virtual ~CCmdSelect();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	BOOL *m_pFlag1;
	BOOL *m_pFlag2;
	BOOL m_bFirst;
};

#endif // !defined(AFX_CMDSELECT_H__583122A5_46BF_11D4_A483_00105A615525__INCLUDED_)
