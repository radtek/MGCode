// CmdFilterDocPoint.h: interface for the CCmdFilterDocPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDFILTERDOCPOINT_H__1A4134E2_4A08_4328_985A_55D76BCEE0F5__INCLUDED_)
#define AFX_CMDFILTERDOCPOINT_H__1A4134E2_4A08_4328_985A_55D76BCEE0F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCmdFilterDocPoint  : public CCommand  
{
public:
	CCmdFilterDocPoint(CHLDoc *pDoc);
	virtual ~CCmdFilterDocPoint();
	virtual BOOL Do(BOOL bRedo= FALSE);
	virtual BOOL Undo(BOOL bRedo= FALSE);

protected:
	CHLDoc * m_pDoc;
	CChain m_chainOld;
	CChain m_chainNew;
	BOOL m_bFirst;

};

#endif // !defined(AFX_CMDFILTERDOCPOINT_H__1A4134E2_4A08_4328_985A_55D76BCEE0F5__INCLUDED_)
