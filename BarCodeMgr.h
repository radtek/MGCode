// BarCodeMgr.h: interface for the CBarCodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODEMGR_H__9E599B7E_93A8_4149_8339_D74FA704F0FF__INCLUDED_)
#define AFX_BARCODEMGR_H__9E599B7E_93A8_4149_8339_D74FA704F0FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GenCodeBar2MatrixH.h"
#include "GenCodebar1DimH.h"

enum BARTYPE{	EAN8	,EAN13	,EAN128	,CODE128A, CODE128B, CODE128C, CODE128AUTO,
				CODE39	,INT25	,EXT39	,UPCA  ,UPCE		,PDF417,
				DATAMATRIX		,QR};

class CBarCodeMgr  
{
public:
	CBarCodeMgr();
	virtual ~CBarCodeMgr();

public:
	CStringArray m_BarNameArray;
	IGenCodebar2Matrix m_BarCode2;
	IGenCodebar1Dim m_BarCode1;
};

extern CBarCodeMgr g_BarCodeMgr;

#endif // !defined(AFX_BARCODEMGR_H__9E599B7E_93A8_4149_8339_D74FA704F0FF__INCLUDED_)
