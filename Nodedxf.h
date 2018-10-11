// NodeDxf.h: interface for the CNodeDxf class.
//
//////////////////////////////////////////////////////////////////////

//*******************************************************************//
//Class Interface for translate DXF to stroke list for HL4.0,designed by 
//PengZili,May. 2005.
//phone:0755-86161316;E-mail:pzili@21cn.com
//copyright reserved by HAN'S LASER co. ltd.(Shen Zhen)
//*******************************************************************//

#if !defined(AFX_NODEDxf_H__CC4B8483_6ADC_4F56_AC4F_FC935A262B95__INCLUDED_)
#define AFX_NODEDxf_H__CC4B8483_6ADC_4F56_AC4F_FC935A262B95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Property.h"
//#include "DrwIO.h"
#include "DxfTranslate.h"

class CNodeDxf : public CNode 
{
public:
	CNodeDxf();
	virtual ~CNodeDxf();

public:
	virtual void Serialize(FILE* fp,BOOL bFlag);
	virtual BOOL DeCode(CString file, CString strShxDir, double dbUnitTrans, double dbPrecision, BOOL bCenter, BOOL bBetter);
	virtual double ChTrue(double a,BOOL flag=TRUE);
	virtual void ChangeLay(CString &layerName);
	virtual void CreatePen(int pen);
    virtual BOOL CanUnGroup(){return TRUE;};
	CNodeDxf *UnGroupDxf();
	virtual  CTree *UnGroup();
	void    GetScribe(CString &Scribe);

	CNodeDxf *UnGroupplt();

	void ChCoord(double xf,double yf,double angle, CDot &dt, BOOL bIsOld);
	
	void GetArcDot(CStroke *pStroke, CDot &start, CDot &end, CDot &center, double radius, int nDir);
	void GetCircleDot(CStroke *pStroke, CDot &start, CDot &center, double radius);

protected:
	void CopyData(DXFOBJOUTLIST_STROKE *pentlist, StrokeList& HLSList);
	void CopyData(int nType, LPCTSTR strLayer, int nSize, double *arrX, double *arrY);
	
protected:
	CString     m_DXF[50];
	BOOL        m_bauto;            //≈–∂œ «∑Ò «AUTOCAD  TRUE£ΩAUTOCAD£ªFALSE£ΩCORELDRAW£ª
};

#endif // !defined(AFX_NODEDxf_H__CC4B8483_6ADC_4F56_AC4F_FC935A262B95__INCLUDED_)
