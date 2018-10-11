// NodeDxf_GP.h: interface for the CNodeDxf_GP class.
//
//////////////////////////////////////////////////////////////////////

//*******************************************************************//
//Class Interface for translate DXF to stroke list for HL4.0,designed by 
//PengZili,May. 2005.
//phone:0755-86161316;E-mail:pzili@21cn.com
//copyright reserved by HAN'S LASER co. ltd.(Shen Zhen)
//*******************************************************************//

#if !defined(AFX_NODEDXF_GP_H__CC4B8483_6ADC_4F56_AC4F_FC935A262B95__INCLUDED_)
#define AFX_NODEDXF_GP_H__CC4B8483_6ADC_4F56_AC4F_FC935A262B95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DxfTranslate.h"	//Added by pzl,05-4-14,DDc.dll's Headfile
#include "Nodeline.h"

class CNodeDxf_GP : public CNode 
{
public:
	CNodeDxf_GP();
	virtual ~CNodeDxf_GP();
	virtual BOOL DeCode(CString file,BOOL bCenter=TRUE,BOOL bBetter=FALSE);
	virtual void ChangeLay(CString layerName);
	virtual void CreatePen(int pen);
	virtual void Serialize(FILE* fp,BOOL bFlag/*,BOOL bUnicode*/);
	CNodeLine *UnGroupDxf_GP();

protected:
	void CopyData(CTypedPtrList<CObList,CDxfStroke *>* pDxfSList,StrokeList& HLSList);

private:
	CDxfTranslate m_dxfTranslate;

protected:
	CString    m_DXF[50];
	
};

#endif // !defined(AFX_NODEDXF_GP_H__CC4B8483_6ADC_4F56_AC4F_FC935A262B95__INCLUDED_)
