// CmdRotate.h: interface for the CCmdRotate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDROTATE_H__583122A4_46BF_11D4_A483_00105A615525__INCLUDED_)
#define AFX_CMDROTATE_H__583122A4_46BF_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"

class CCmdRotate : public CCommand  
{
public:
	CCmdRotate(CHLDoc *pDoc,CDot dot,double angle );
	virtual ~CCmdRotate();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CDot m_dotCenter;
	double m_dbAngle;

};

#endif // !defined(AFX_CMDROTATE_H__583122A4_46BF_11D4_A483_00105A615525__INCLUDED_)
