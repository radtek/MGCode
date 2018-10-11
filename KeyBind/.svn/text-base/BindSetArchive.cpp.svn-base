// BindSetArchive.cpp: implementation of the CBindSetArchive class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\hl.h"
#include "BindSetArchive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBindSetArchive::CBindSetArchive(UINT nMode)
{
	m_pArchive = NULL;

	CString strFilePath;
	char strTemp[1024];
	GetModuleFileName(NULL, strTemp, 1024);
	strFilePath = strTemp;
	int index = strFilePath.ReverseFind('\\');
	strFilePath = strFilePath.Left(index+1);

	CString strFileName;
	strFileName.LoadString(IDS_BINDSET_FILE);

	UINT fileMode = CFile::modeRead;
	if (nMode == CArchive::load)
	{
		fileMode = CFile::modeRead;
	}
	else
	{
		fileMode = CFile::modeCreate | CFile::modeWrite;
	}

	if( !m_file.Open( strFilePath+strFileName, fileMode ) ) 
	{
		return;
	}
	m_pArchive = new CArchive(&m_file, nMode);
}

CBindSetArchive::~CBindSetArchive()
{
	if (m_pArchive != NULL)
	{
		m_pArchive->Close();
		delete m_pArchive;
		m_file.Close();
	}
}