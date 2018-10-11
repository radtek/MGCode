// HLMotion.h: interface for the CHLMotion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLMOTION_H__FA2042FB_4B1C_4CA5_B5D4_FD33F75F6747__INCLUDED_)
#define AFX_HLMOTION_H__FA2042FB_4B1C_4CA5_B5D4_FD33F75F6747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define AXIS_TOTAL_NUM 16
#define eMaskX   0x0001
#define eMaskY   0x0002
#define eMaskZ   0x0004
#define eMaskA   0x0008
#define eMaskXY	 0x0003
#define eMaskXYZ 0x0007
#define eMaskAll 0xFFFF
enum SpeedEnum {eSpeedHome, eSpeedSlow, eSpeedFast};
enum MotionEnum {eDebugMotion, eLPTMotion, eDMC2000Motion, eDMC2210Motion, eGTMotion, eGEMotion, eGTSMotion, eParker62Motion, eACSMotion,eDMC2410Motion};
enum StatusBitEnum {eStatusBitMoving=0x01, eStatusBitPLimit=0x02, eStatusBitNLimit=0x04, eStatusBitORG=0x08, eStatusBitAlarm=0x10};

class AFX_EXT_CLASS CHLMotion
{
public:
	static CHLMotion* CreateMotion(int nCardType=-1);
	void DestoryMotion();

	virtual void GetDllVersion(char buffer[32])=0;
	virtual int	 GetCardType()=0;
	virtual void GetConfigInfo(DWORD &dwCaliInfo, DWORD &dwOther)=0;
	virtual BOOL CheckError(char szError[1024])=0; //called after Init() succeeded

	virtual void LoadPara(LPCSTR strFile=NULL)=0;
	virtual void SavePara(LPCSTR strFile=NULL)=0;
	virtual void GetPara(LPCSTR strSection, LPCSTR strItem, double &dbValue)=0;
	virtual void WritePara(LPCSTR strSection, LPCSTR strItem, double dbValue)=0;

	virtual BOOL IsInit()=0;
	virtual BOOL Init(char szError[1024])=0;
	virtual void Close()=0;

	virtual void StartHome(int nMask=eMaskAll)=0;
	virtual BOOL WaitHomeFinish(DWORD dwTime=INFINITE)=0;
	virtual BOOL IsHomed(int nMask=eMaskAll)=0;

	virtual void StartMoveToLoadPosition()=0;
	virtual void StartMoveToUnloadPosition()=0;

	virtual void StartAMove(int nMask, double dbPos, int nSpeedType=eSpeedFast)=0;
	virtual void StartRMove(int nMask, double dbMove, int nSpeedType=eSpeedFast)=0;
	virtual void StartXY(double dbPosX, double dbPosY, int nSpeedType=eSpeedFast)=0;
	virtual void WaitMoveFinish(int nMask=eMaskAll)=0;

	virtual void AMove(int nMask, double dbPos,  int nSpeedType=eSpeedFast)=0;
	virtual void RMove(int nMask, double dbMove, int nSpeedType=eSpeedFast)=0;
	virtual void LnXY(double dbPosX, double dbPosY, int nSpeedType=eSpeedFast)=0;	
	virtual void MovedDelay(int nMask=eMaskX)=0;

	virtual void StartGear(int nMasterMask, int nSlaveMask, double dbRatio)=0;

	virtual void Stop(int nMask=eMaskAll)=0;
	virtual void Disable(int nMask=eMaskAll)=0;

	virtual UINT GetStatus(int nMask)=0;				//StatusBitEnum
	virtual void SetPos(int nMask, double dbPos)=0;
	virtual double GetPos(int nMask)=0;
	virtual double GetPos(int nMask, DWORD &dwInfo)=0;	//dwInfo:0-OK, 1-Calibration err

	virtual BOOL InBit(int nBitNo)=0; //0,1,2,3...
	virtual void OutBit(int nBitNo, BOOL bHigh)=0; //0,1,2,3...
};

#endif // !defined(AFX_HLMOTION_H__FA2042FB_4B1C_4CA5_B5D4_FD33F75F6747__INCLUDED_)
