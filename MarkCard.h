#pragma once

#include <Mmsystem.h>
#include "Stroke.h"
typedef CTypedPtrList<CObList,CStroke*> STROKELIST;
#include "Lay.h"
#include "MarkMgr.h"

#define NOT_FIX_Z 1000000

typedef struct CardEvtSet
{
	HANDLE hReadyEvt;//数据缓冲区满事件
	HANDLE hTaskStartEvt;//每个Task打标任务开始打标事件
	HANDLE hTaskEndEvt;//每个Task打标任务结束事件
}CardEvtSet;

const TCHAR c_califile[] = "calidat.box";
const TCHAR c_cardfile[] = "systemset.ini";

typedef UINT (WINAPI *HS_INITCARD)();
typedef UINT (WINAPI *HS_GETCARDSTS)(UINT *nSts);
typedef UINT (WINAPI *HS_GETCARDERROR)(UINT *nErrCode);
typedef UINT (WINAPI *HS_REGEVENT)(CardEvtSet evtSet);
typedef UINT (WINAPI *HS_UNREGEVENT)();
typedef UINT (WINAPI *HS_GETHDVER)(UINT *pDwVer);
typedef UINT (WINAPI *HS_GETSWVER)(UINT *pSwVer);
typedef UINT (WINAPI *HS_SETCALIPARAFILE)(const char *pFileName);
typedef UINT (WINAPI *HS_SETCARDPARAFILE)(const char *pFileName);
typedef UINT (WINAPI *HS_SETMARKPARA)(LPVOID pMarkPara, int nNumPara);
typedef UINT (WINAPI *HS_ENTERMARKENV)(UINT nFlag);
typedef UINT (WINAPI *HS_LEAVEMARKENV)();
typedef UINT (WINAPI *HS_MARKSTROKE)(UINT nType, LPVOID pInfo, int nNum, UINT nLayIndex);
typedef UINT (WINAPI *HS_MARKSTART)(UINT nType, UINT nSts);
typedef UINT (WINAPI *HS_MARKEND)();
typedef UINT (WINAPI *HS_TASKSTART)(UINT nTaskIndex);
typedef UINT (WINAPI *HS_TASKEND)(UINT nTaskEnd);
typedef UINT (WINAPI *HS_GETMARKTIME)(UINT *pMarkTime);
typedef UINT (WINAPI *HS_SETCURRENT)(double dbCurrent);
typedef UINT (WINAPI *HS_INPUT)(UINT *pInput);
typedef UINT (WINAPI *HS_OUTPUT)(UINT nOutput, UINT nMask);
typedef UINT (WINAPI *HS_GETOUTPUT)(UINT *nOutput);
typedef UINT (WINAPI *HS_SETGRAYCURRENT)(double *pdbList, int nNum);
typedef UINT (WINAPI *HS_GETMARKRANGE)(double *pdbX, double *pdbY);
typedef UINT (WINAPI *HS_GETPARAINFO)(LPVOID pInfo, int nNum);
typedef UINT (WINAPI *HS_MARKCMDSTART)();
typedef UINT (WINAPI *HS_MARKCMDPAUSE)();
typedef UINT (WINAPI *HS_MARKCMDREMARK)();
typedef UINT (WINAPI *HS_MARKCMDSTOP)();
typedef UINT (WINAPI *HS_GETERRINFO)(UINT nErrCode, char *pszInfo, int nSize);
typedef UINT (WINAPI *HS_EXITCARD)();
typedef UINT (WINAPI *HS_GOTO)(double dbX,double dbY,double dbZ);
typedef UINT (WINAPI *HS_AUTOCAL)();
typedef UINT (WINAPI *HS_SetAutoCalRefPos)();
typedef UINT (WINAPI *HS_BeamOn)();
typedef UINT (WINAPI *HS_BeamOff)();
typedef UINT (WINAPI *HS_LAYER_CHANGE_CALLBACK)(int nOldLayerIndex, int nNewLayerIndex);
typedef UINT (WINAPI *HS_SET_LAYER_CHANGE_CALLBACK)(HS_LAYER_CHANGE_CALLBACK pCallback);

class CMarkCard
{
public:
	CardEvtSet m_evtSet;
	CString m_strMulSel;
	CMarkCard(void);
	~CMarkCard(void);
private:
	HS_INITCARD m_pInitCard;
	HS_GETCARDSTS m_pGetCardSts;
	HS_GETCARDERROR m_pGetCardError;
	HS_REGEVENT m_pRegEvent;
	HS_UNREGEVENT m_pUnRegEvent;
	HS_GETHDVER m_pGetHdVer;
	HS_GETSWVER m_pGetSwVer;
	HS_SETCALIPARAFILE m_pSetCaliParaFile;
	HS_SETCARDPARAFILE m_pSetCardParaFile;
	HS_SETMARKPARA m_pSetMarkPara;
	HS_ENTERMARKENV m_pEnterMarkEnv;
	HS_LEAVEMARKENV m_pLeaveMarkEnv;
	HS_MARKSTROKE m_pMarkStroke;
	HS_MARKSTART m_pMarkStart;
	HS_MARKEND m_pMarkEnd;
	HS_TASKSTART m_pTaskStart;
	HS_TASKEND m_pTaskEnd;
	HS_GETMARKTIME m_pGetMarkTime;
	HS_SETCURRENT m_pSetCurrent;
	HS_INPUT m_pInput;
	HS_OUTPUT m_pOutput;
	HS_GETOUTPUT m_pGetOutput;
	HS_SETGRAYCURRENT m_pSetGrayCurrent;
	HS_GETMARKRANGE m_pGetMarkRange;
	HS_GETPARAINFO m_pGetParaInfo;
	HS_MARKCMDSTART m_pMarkCmdStart;
	HS_MARKCMDPAUSE m_pMarkCmdPause;
	HS_MARKCMDREMARK m_pMarkCmdRemark;
	HS_MARKCMDSTOP m_pMarkCmdStop;
	HS_GETERRINFO m_pGetErrInfo;
	HS_EXITCARD m_pExitCard;
	HS_GOTO m_pGoTo;
	HS_AUTOCAL m_pAutoCal;
	HS_SetAutoCalRefPos m_SetAutoCalRefPos;
	HS_BeamOn  m_pBeamOn;
	HS_BeamOff m_pBeamOff;
	HS_SET_LAYER_CHANGE_CALLBACK m_pSetLayerChangeCallback;

	HMODULE m_hModule;
	CStdioFile m_logFile;
	//脚踏处理
	UINT m_nTouchCount;					//用于触发计数，每几个触发信号打标一次
	BOOL m_bTouchFrist;					//指示是否为第一次脚踏触发
	//当前启用的层参数
	CLay m_lay;

	int  m_nMarkLayNum;     
	BOOL m_bIsResMark;

	//0 最小值
	//1 选项，大于1为可用
	//2 默认值
	//3 最大值
	PARA m_paraInfo[4];

public:
	void Between(HANDLE hStop);
	BOOL Touch();
	BOOL GetInputStr(CString &str);
	UINT GetMarkCount(int nLayIndex);
	CLay *GetCurLay();
	void SetMarkLayNum(int nLayNum, BOOL bIsResMark = FALSE);

	BOOL LoadMarkLib(void);
	void FreeMarkLib(void);

	BOOL GetCardSts(UINT &nSts);
	BOOL GetCardError(UINT &nErrCode);
	BOOL RegEvent();
	BOOL UnRegEvent();
	BOOL TaskStart(UINT nTaskIndex);
	BOOL TaskEnd(UINT nTaskIndex);

	//主版本号,次版本号，定制版本号，次次版本号
	void GetHdVer(UINT &nMainVer, UINT &nSubVer, UINT &nSSubVer, UINT &nCustomVer);
	void GetSwVer(UINT &nMainVer, UINT &nSubVer, UINT &nSSubVer, UINT &nCustomVer);
	BOOL InitCard();
	void ExitCard();
	//nFlag 0-脱机 1-联机
	BOOL EnterMarkEnv(UINT nFlag=1);
	void LeaveMarkEnv();
	//nType: 0-打标，1-红光预览
	//nSts: 0-自动打标 1-手动打标 2－外部触发
	BOOL MarkStart(UINT nType=0, UINT nSts=1);
	void MarkEnd();
	BOOL MarkStroke(const CStroke &stroke);
	BOOL MarkStroke(const CStroke &stroke, int nLay, BOOL bIsResMark);
	BOOL MarkStroke(UINT nType, LPVOID pInfo, int nNum, UINT nLayIndex);
	BOOL BuffTime(UINT nTime);
	BOOL BuffOutput(UINT nValue, UINT nMask);
	BOOL BuffInput(UINT nValue, UINT nMask);
	//返回ms数
	DWORD GetMarkTime();
	BOOL SetCurrent(double dbCurrent);
	BOOL GetInput(UINT &nInput);
	BOOL SetOutput(UINT dwOutput, UINT nMask);
	BOOL GetOutput(UINT &dwOutput);
	BOOL GetMarkRange(double &dbX, double &dbY);
	BOOL MarkCmdStart();
	BOOL MarkCmdPause();
	BOOL MarkCmdRemark();
	BOOL MarkCmdStop();
	void SetCaliParaFile(const char *pszFile);
	void SetCardParaFile(const char *pszFile);
	BOOL SetMarkPara(CLay &lay);
	BOOL SetGrayCurrent(double *pdbList, int nNum);
	BOOL GetParaInfo(LPVOID pInfo, int nSize);
	void GoTo(double dbX, double dbY, double dbZ);
	void AutoCal();
	void SetAutoCalRefPos();
	void BeamOn();
	void BeamOff();
	void SetLayerChangeCallback(HS_LAYER_CHANGE_CALLBACK pCallback);
private:
	inline BOOL CheckReutrnValue(UINT nRet, const char *pFunc);
	BOOL OpenLogFile();
	void CloseLogFile();
	inline void Log(const CString &strLog);
public:
	static UINT CheckCardProc(LPVOID lpContext);
	HANDLE m_hCheckCardEvtContinue;
	HANDLE m_hCheckCardEvtExit;
	CWinThread *m_pCheckCardThread;
	double m_dbFixZ;
};
