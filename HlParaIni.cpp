// HlParaIni.cpp: implementation of the CHlParaIni class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DivideLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHlParaIni::CHlParaIni()
{
	SetFileName(IniFile);
}

void CHlParaIni::LoadPara()
{	
	m_nRtcBitmm =   ReadInt("BOX","BMM",7440);
	m_dbScaleX =	ReadDouble("BOX", "XScale", 1);
	m_dbSacleY =	ReadDouble("BOX", "YScale", 1);
	m_dbCCWAngle =  ReadDouble("BOX","CCW",0);
	m_strCaliFile = ReadString("BOX", "CORFILE", "");


	m_nMotionDelayForCCD  = ReadInt("Motion", "MotionDelayForCCD", 1000);
	m_nMotionDelayForMark = ReadInt("Motion", "MotionDelayForMark", 1000);
	m_dbRefPointX   = ReadDouble("WB", "RefPointX", 0);
	m_dbRefPointY   = ReadDouble("WB", "RefPointY", 0);
	m_dbMarkToCCD1X = ReadDouble("WB", "MarkToCCD1X", 0);
	m_dbMarkToCCD1Y = ReadDouble("WB", "MarkToCCD1Y", 0);
	m_dbMarkToCCD2X = ReadDouble("WB", "MarkToCCD2X", 0);
	m_dbMarkToCCD2Y = ReadDouble("WB", "MarkToCCD2Y", 0);
	m_dbWorkbenchFixtureAngle = ReadDouble("WB", "WorkbenchFixtureAngle", 0);
	m_bUseCutBlow   = ReadInt("WB", "UseCutBlow", 1);
	
	m_dbXMMPerPixel	  = ReadDouble("CCDPARA","XMMPPULSE",0.0225);
	m_dbYMMPerPixel	  = ReadDouble("CCDPARA","YMMPPULSE",0.0226);
	m_dbXMMPerPixel2  = ReadDouble("CCDPARA","XMMPPULSE2",0.0225);
	m_dbYMMPerPixel2  = ReadDouble("CCDPARA","YMMPPULSE2",0.0226);
	m_dbCCD1Precision = ReadDouble("CCDPARA","CCD1Precision",1.0);
	m_dbCCD2Precision = ReadDouble("CCDPARA","CCD2Precision",6.0);

	m_dbMaxPower		=	ReadDouble("PowerMeter","MaxPower",100);
	m_dbMinPower		=	ReadDouble("PowerMeter","MinPower",0);
	m_dbLastPower		=	ReadDouble("PowerMeter","LastPower",50);
	m_dbPowerCheckX		=	ReadDouble("PowerMeter","PowerCheckX", 0);
	m_dbPowerCheckY		=	ReadDouble("PowerMeter","PowerCheckY", 0);
	m_dbPowerCheckZ		=	ReadDouble("PowerMeter","PowerCheckZ", 0);

	m_nDxfUnit = ReadInt("DXF", "Unit", 2);

	m_dbAutoOffsetLastX = ReadDouble("AutoOffset", "LastX", 0);
	m_dbAutoOffsetLastY = ReadDouble("AutoOffset", "LastY", 0);
	m_dbAutoOffsetDeviationMax = ReadDouble("AutoOffset", "AutoOffsetDeviationMax", 0.005);
	m_dbAutoOffsetDeviationMin = ReadDouble("AutoOffset", "AutoOffsetDeviationMin", -0.005);
	
	m_nPowerCheckWait =	ReadInt("PowerTest", "LastX", 10);
	m_dbPowerTestTotalTime =	ReadDouble("PowerTest", "TotalTime", 1.0);
	m_dbPowerCheckInterval = ReadDouble("PowerTest", "PowerCheckInterval", 1.0);

	m_bUseQPosite = ReadInt("QPosite", "QPosite", 0);

	m_dbWorkPosX = ReadDouble("WB", "WorkPosX", 0);
	m_dbWorkPosY = ReadDouble("WB", "WorkPosY", 0);

	m_nCloseDoorTime = ReadInt("XY", "CloseDoorTime", 0);
}

void CHlParaIni::SavePara()
{
	WriteInt("BOX","BMM", m_nRtcBitmm);
	WriteDouble("BOX", "XScale", m_dbScaleX);
	WriteDouble("BOX", "YScale", m_dbSacleY);
	WriteDouble("BOX","CCW", m_dbCCWAngle);
	WriteString("BOX", "CORFILE", m_strCaliFile);

	WriteInt("Motion", "MotionDelayForCCD", m_nMotionDelayForCCD);
	WriteInt("Motion", "MotionDelayForMark", m_nMotionDelayForMark);
	WriteDouble("WB", "RefPointX", m_dbRefPointX);
	WriteDouble("WB", "RefPointY", m_dbRefPointY);
	WriteDouble("WB", "MarkToCCD1X", m_dbMarkToCCD1X);
	WriteDouble("WB", "MarkToCCD1Y", m_dbMarkToCCD1Y);
	WriteDouble("WB", "MarkToCCD2X", m_dbMarkToCCD2X);
	WriteDouble("WB", "MarkToCCD2Y", m_dbMarkToCCD2Y);
	WriteDouble("WB", "WorkbenchFixtureAngle", m_dbWorkbenchFixtureAngle);
	WriteDouble("WB", "WorkPosX", m_dbWorkPosX);
	WriteDouble("WB", "WorkPosY", m_dbWorkPosY);
	WriteInt("WB", "UseCutBlow", m_bUseCutBlow);

	WriteDouble("CCDPARA","XMMPPULSE",m_dbXMMPerPixel);
	WriteDouble("CCDPARA","YMMPPULSE",m_dbYMMPerPixel);
	WriteDouble("CCDPARA","XMMPPULSE2",m_dbXMMPerPixel2);
	WriteDouble("CCDPARA","YMMPPULSE2",m_dbYMMPerPixel2);
	WriteDouble("CCDPARA","CCD1Precision",m_dbCCD1Precision);
	WriteDouble("CCDPARA","CCD2Precision",m_dbCCD2Precision);

	WriteDouble("PowerMeter","MaxPower",m_dbMaxPower);
	WriteDouble("PowerMeter","MinPower",m_dbMinPower);
	WriteDouble("PowerMeter","LastPower",m_dbLastPower);
	WriteDouble("PowerMeter", "PowerCheckX", m_dbPowerCheckX);
	WriteDouble("PowerMeter", "PowerCheckY", m_dbPowerCheckY);
	WriteDouble("PowerMeter", "PowerCheckZ", m_dbPowerCheckZ);

	WriteInt("DXF", "Unit", m_nDxfUnit);

	WriteDouble("AutoOffset", "LastX", m_dbAutoOffsetLastX);	
	WriteDouble("AutoOffset", "LastY", m_dbAutoOffsetLastY);
	WriteDouble("AutoOffset", "AutoOffsetDeviationMax", m_dbAutoOffsetDeviationMax);
	WriteDouble("AutoOffset", "AutoOffsetDeviationMin", m_dbAutoOffsetDeviationMin);
	
	WriteInt("PowerTest", "LastX", m_nPowerCheckWait);
	WriteDouble("PowerTest", "TotalTime", m_dbPowerTestTotalTime);
	WriteDouble("PowerTest", "PowerCheckInterval", m_dbPowerCheckInterval);


	WriteInt("QPosite", "QPosite", m_bUseQPosite);

	WriteInt("XY", "CloseDoorTime", m_nCloseDoorTime);
}
