#include "stdafx.h"
#include "DsfObj.h"


CDsfTimer::CDsfTimer(void)
{
	m_nDsfObjType = DSFOBJ_TIMER;
	m_nTime = 10;
}


CDsfTimer::~CDsfTimer(void)
{
}

void CDsfTimer::Mark(HWND hWnd)
{
	g_DAT.BuffTime(m_nTime);
	return;
}

void CDsfTimer::Serialize(FILE* fp,BOOL bSave)
{
	CMySerialize sa(fp);
	if (bSave)
	{
		sa<<m_nDsfObjType;
		sa<<m_nTime;
	}
	else
	{
		sa>>m_nDsfObjType;
		sa>>m_nTime;
	}
}

CDsfDoOutput::CDsfDoOutput(void)
{
	m_nDsfObjType = DSFOBJ_DOOUPUT;
	m_nOutPut = 0;
	m_nMask = 0;
}


CDsfDoOutput::~CDsfDoOutput(void)
{
}

void CDsfDoOutput::Mark(HWND hWnd)
{
	g_DAT.BuffOutput(m_nOutPut, m_nMask);
	return;
}

void CDsfDoOutput::Serialize(FILE* fp,BOOL bSave)
{
	CMySerialize sa(fp);
	if (bSave)
	{
		sa<<m_nDsfObjType;
		sa<<m_nOutPut;
		sa<<m_nMask;
	}
	else
	{
		sa>>m_nDsfObjType;
		sa>>m_nOutPut;
		sa>>m_nMask;
	}
}

CDsfWaitInput::CDsfWaitInput(void)
{
	m_nDsfObjType = DSFOBJ_WAITINPUT;
	m_nInput = 0;
	m_nMask = 0;
}


CDsfWaitInput::~CDsfWaitInput(void)
{
}

void CDsfWaitInput::Mark(HWND hWnd)
{
	g_DAT.BuffInput(m_nInput, m_nMask);
	return;
}

void CDsfWaitInput::Serialize(FILE* fp,BOOL bSave)
{
	CMySerialize sa(fp);
	if (bSave)
	{
		sa<<m_nDsfObjType;
		sa<<m_nInput;
		sa<<m_nMask;
	}
	else
	{
		sa>>m_nDsfObjType;
		sa>>m_nInput;
		sa>>m_nMask;
	}
}