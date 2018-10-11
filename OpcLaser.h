#pragma once
class AFX_EXT_CLASS COpcLaser
{
public:
	static COpcLaser *CreateObject();
	void ReleaseObject();

	virtual BOOL Connect(LPCSTR szOPCIP)=0;
	virtual BOOL Disconnect(void)=0;
	virtual void SetWriteDelay(int nWriteDelay)=0;	

	virtual BOOL WriteFreq(int nFreq)=0;
	virtual BOOL InitLaserPara(void)=0;
	virtual BOOL CheckLaserReady(bool& bLaserReady)=0;
	virtual BOOL ClearLaserData(void)=0;
	virtual BOOL CheckProgActive(bool& bProgActive)=0;
	virtual BOOL CheckLaserOn(bool& bExtAtive, bool& bLaserOn)=0;
	virtual BOOL Reset(void)=0;
	virtual BOOL CloseLaser(void)=0;
	virtual BOOL WriteExtActive(void)=0;
	virtual BOOL WriteLaserOn(void)=0;
	virtual BOOL WriteRequestLaser(void)=0;
	virtual BOOL WriteLaserProNo(void)=0;
	virtual BOOL OpenLaser(void)=0;
};

