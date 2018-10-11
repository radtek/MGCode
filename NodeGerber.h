// NodeGerber.h: interface for the CNodeGerber class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEDGerber_H__CC4B8483_6ADC_4F56_AC4F_FC935A262B95__INCLUDED_)
#define AFX_NODEDGerber_H__CC4B8483_6ADC_4F56_AC4F_FC935A262B95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Property.h"

class CNodeGerber : public CNode 
{
public:
	CNodeGerber();
	virtual ~CNodeGerber();

public:
	virtual void Serialize(FILE* fp,BOOL bFlag);
	virtual BOOL DeCode(CString file,BOOL bCenter,double dbPrecision);
	virtual double ChTrue(double a,BOOL flag=TRUE);
	virtual void ChangeLay(CString &layerName);
	virtual void CreatePen(int pen);
    virtual BOOL CanUnGroup(){return TRUE;};
	virtual CTree *UnGroup();
	CNodeGerber *UnGroupGerber();
	void    GetScribe(CString &Scribe);

	CNodeGerber *UnGroupplt();

	void ChCoord(double xf,double yf,double angle, CDot &dt, BOOL bIsOld);
	
	void GetArcDot(CStroke *pStroke, CDot &start, CDot &end, CDot &center, double radius, int nDir);
	void GetCircleDot(CStroke *pStroke, CDot &center, double radius);

	static BOOL CheckGerberDog();

protected:
	CString     m_DXF[50];
	BOOL        m_bauto;            //≈–∂œ «∑Ò «AUTOCAD  TRUE£ΩAUTOCAD£ªFALSE£ΩCORELDRAW£ª
};

#endif // !defined(AFX_NODEDxf_H__CC4B8483_6ADC_4F56_AC4F_FC935A262B95__INCLUDED_)
