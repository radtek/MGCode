// CmdScale.h: interface for the CCmdScale class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSCALE_H__C7A85BA2_4604_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDSCALE_H__C7A85BA2_4604_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdScale : public CCommand  
{
public:
	CCmdScale(CHLDoc *pDoc,CDot dot,double ratioX,double ratioY);
	virtual ~CCmdScale();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc *m_pDoc;
	CDot m_dotCenter;
	double m_dbRatioX;
	double m_dbRatioY;
};

#endif // !defined(AFX_CMDSCALE_H__C7A85BA2_4604_11D4_A483_00105A615525__INCLUDED_)
