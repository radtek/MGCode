// BarCodeMgr.cpp: implementation of the CBarCodeMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "BarCodeMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBarCodeMgr g_BarCodeMgr;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarCodeMgr::CBarCodeMgr()
{
	//建立与BARTYPE相对应的字串
	CString str = "EAN 8";
	m_BarNameArray.Add(str);
	str = "EAN 13";
	m_BarNameArray.Add(str);
	str = "EAN 128";
	m_BarNameArray.Add(str);
	str = "CODE 128A";
	m_BarNameArray.Add(str);
	str = "CODE 128B";
	m_BarNameArray.Add(str);
	str = "CODE 128C";
	m_BarNameArray.Add(str);
	str = "CODE 128AUTO";
	m_BarNameArray.Add(str);
	str = "CODE 39";
	m_BarNameArray.Add(str);
	str = "INT 25";
	m_BarNameArray.Add(str);
	str = "EXT 39";
	m_BarNameArray.Add(str);
	str = "UPCA";
	m_BarNameArray.Add(str);
	str = "UPCE";
	m_BarNameArray.Add(str);
	str = "PDF417";
	m_BarNameArray.Add(str);
	str = "DATA MATRIX";
	m_BarNameArray.Add(str);
	str = "QR";
	m_BarNameArray.Add(str);
}

CBarCodeMgr::~CBarCodeMgr()
{
}
