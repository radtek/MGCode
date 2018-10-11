// CmdNewDrag.h: interface for the CCmdNewDrag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDNEWDRAG_H__1E1CC2F3_A491_4B99_80EA_FFC18FCD5148__INCLUDED_)
#define AFX_CMDNEWDRAG_H__1E1CC2F3_A491_4B99_80EA_FFC18FCD5148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mfccmd.h"
#include "HLDoc.h"
class CCmdNewDrag : public CCommand  
{
public:
	CCmdNewDrag(CHLDoc *pDoc,CDot &dot,double ratioX,double ratioY,double moveX,double moveY);
	virtual ~CCmdNewDrag();
	virtual BOOL Do(BOOL bRedo =FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);
	
protected:
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;
	CHLDoc * m_pDoc;
	double m_dbMoveX;
	double m_dbMoveY;
	CDot m_dotCenter;
	double m_dbRatioX;
	double m_dbRatioY;
};

#endif // !defined(AFX_CMDNEWDRAG_H__1E1CC2F3_A491_4B99_80EA_FFC18FCD5148__INCLUDED_)
