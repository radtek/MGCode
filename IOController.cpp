#include "stdafx.h"
#include "HL.H"
#include "IOController.h"

CIOController g_IO;

CIOController::CIOController(void)
{
	m_bOnLine = FALSE;
	m_bOffLine = TRUE;
}


CIOController::~CIOController(void)
{

}

void CIOController::SetOnline(BOOL bOnline)
{
	m_bOnLine = bOnline;
	m_bOffLine = !bOnline;
}

BOOL CIOController::INPUT_MARK()
{
	if (m_bOnLine)	return !g_pMotion->InBit(2);
	if (m_bOffLine)	return !g_pMotion->InBit(2);
	return FALSE;
}

BOOL CIOController::INPUT_RESET_ERROR()
{
	if (m_bOnLine)	return !g_pMotion->InBit(4);
	if (m_bOffLine)	return !g_pMotion->InBit(4);
	return FALSE;
}

BOOL CIOController::INPUT_SAFEDOOR()
{
	if (m_bOnLine)	return !g_pMotion->InBit(5);
	if (m_bOffLine)	return g_pMotion->InBit(5);
	return FALSE;
}

BOOL CIOController::INPUT_GOLOADPT()
{
	if (m_bOnLine)	return !g_pMotion->InBit(0);
	return FALSE;
}

BOOL CIOController::INPUT_GOUNLOADPT()
{
	if (m_bOnLine)	return !g_pMotion->InBit(1);
	return FALSE;
}

BOOL CIOController::INPUT_WORKBENCH_DISABLE()
{
	if (m_bOnLine)	return !g_pMotion->InBit(3);
	return FALSE;
}

BOOL CIOController::INPUT_MACHINE_READY()
{
	if (m_bOnLine)	return !g_pMotion->InBit(7);
	if (m_bOffLine)	return !g_pMotion->InBit(7);
	return TRUE;
}

void CIOController::OUTPUT_LIGHT_RED(BOOL b)
{
	if (m_bOffLine)	g_pMotion->OutBit(0, !b);
}

void CIOController::OUTPUT_LIGHT_YELLOW(BOOL b)
{
	if (m_bOffLine)	g_pMotion->OutBit(2, !b);
}
void CIOController::OUTPUT_LIGHT_GREEN(BOOL b)
{
	if (m_bOffLine)	g_pMotion->OutBit(4, !b);
}

void CIOController::OUTPUT_LIGHT_HUMMER(BOOL b)
{
	if (m_bOffLine)	g_pMotion->OutBit(6, !b);
}

void CIOController::OUTPUT_AIR_BLOW(BOOL b)
{
	if (m_bOnLine)	g_pMotion->OutBit(6, !b);
	if (m_bOffLine)	g_pMotion->OutBit(3, !b);
}

void CIOController::OUTPUT_SMOKE_FILTER(BOOL b)
{
	if (m_bOffLine)	g_pMotion->OutBit(5, !b);
}

void CIOController::OUTPUT_LOADPT(BOOL b)
{
	if (m_bOnLine)	g_pMotion->OutBit(0, !b);
}

void CIOController::OUTPUT_UNLOADPT(BOOL b)
{
	if (m_bOnLine)	g_pMotion->OutBit(1, !b);
}

void CIOController::OUTPUT_MARKEND(BOOL b)
{
	if (m_bOnLine)	g_pMotion->OutBit(2, !b);
}

void CIOController::OUTPUT_COD_BARCODE_ERROR(BOOL b)
{
	if (m_bOnLine)	g_pMotion->OutBit(3, !b);
}

void CIOController::OUTPUT_CCD_LOCATE_ERROR(BOOL b)
{
	if (m_bOnLine)	g_pMotion->OutBit(4, !b);
}

void CIOController::OUTPUT_SOFT_OK(BOOL b)
{
	if (m_bOnLine)	g_pMotion->OutBit(5, !b);
}

void CIOController::OUTPUT_CLOSE_DOOR(BOOL b)
{
	if (m_bOnLine)	g_pMotion->OutBit(7, !b);
}
