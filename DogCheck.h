// DogCheck.h: interface for the CDogCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOGCHECK_H__B9F5BC4D_500F_4466_B367_DFE5DA8D725F__INCLUDED_)
#define AFX_DOGCHECK_H__B9F5BC4D_500F_4466_B367_DFE5DA8D725F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum FDICH_RETURN_CODE
{
	FRC_SUCCESS,//返回成功
	FRC_SUCESS_WITH_USERID,//带工号返回成功
	FRC_SUCCESS_WITH_TIMESHOW,//成功但限时使用，并提醒使用剩余时间
	//软件授权还允许运行%d天，需要联系大族客服人员，重新申请与(狗ID=%s, 校验码=%s)匹配的许可授权补丁，以便到期后能继续使用
	FRC_SUCCESS_WITH_INSPECT,//成功但巡检提醒
	//设备已连续运行一年，需要联系大族客服人员，对设备进行维护检修，以确保安全正常运行，
	//并申请与(狗ID=%s, 校验码=%s)匹配的软件授权检修确认补丁
		
	FRC_FAIL_DOG_LIB_LOAD,//载入软件狗操作动态库失败
	FRC_FAIL_DOG_LIB_GUID,//操作失败，HansLmtH.dll模块库类型不匹配
	FRC_FAIL_DOG_LIB_VERSION,//操作失败，HansLmtH.dll模块库版本至少是V1.1.0
	//模块库载入失败或模块库版本不正确，请检测软件安装是否正确
		
	FRC_FAIL_DOG_EXECUTE,//将初始狗转换成运行狗失败
	//软件狗操作异常，请检测狗连接是否正常，驱动是否安装正确
		
	FRC_FAIL_DOG_TIMEOUT,//试用时间已经用完
	//当前软件授权已到期，需要联系大族客服人员，重新申请与(狗ID=%s, 校验码=%s)匹配的许可授权补丁，以便继续使用
		
	FRC_FAIL_DOG_WRONG_TYPE,
	//!=0x00软件狗类型不匹配，请确认软件狗是否是该设备配套的软件狗，否则请联系大族客服人员
	//!=0x01软件狗类型不匹配，请确认软件狗是否是该设备配套的软件狗，否则，请确认软件，控制卡及软件狗是否需要升级
		
	FRC_FAIL_DOG_CMP_CARD,//与打标卡不对应
		
	FRC_FAIL_DOG_FUNC_UNKNOW,//狗操作中发生未知错误
	FRC_FAIL_DOG_FUNC_NODOG,//没有检测到软件狗
	FRC_FAIL_DOG_FUNC_WRONGDOG,//检测到软件狗，但不是合法的软件狗
		
	FRC_FAIL_DOG_FUNC_CHECK_WRONG_BEFORE_EXECUTE,//在Execute函数调用前没有成功调用过Check函数
	FRC_FAIL_DOG_FUNC_LIB_L_VERSION,//HansLmtL.dll模块库版本至少V1.1.0
	//模块库载入失败或模块库版本不正确，请检测软件安装是否正确
		
	FRC_FAIL_DOG_FUNC_MOREDOG,//插入的软件狗多于1个
	FRC_FAIL_DOG_FUNC_WRONG_PARAM,//函数传入参数不支持
		
	FRC_FAIL_PARA_UNKNOW_CARDTYPE,//未知的控制卡类型
	FRC_FAIL_PARA_UNKONW_DOGTYPE,//未知的狗类型
		
	FRC_FAIL_CARD_LIB_LOAD,//控制卡库加载失败
	FRC_FAIL_CARD_FUNC_GET,//取控制卡库接口函数失败
	FRC_FAIL_CARD_NONE,//无控制卡
	FRC_FAIL_CARD_WRITE_NEW,//写新卡失败
	//写卡失败，请检测控制卡的连接或坏损
		
	FRC_FAIL_CANCEL,
};

enum DOG_CARD_TYPE
{
	CT_PCI3000,
	CT_DCP1000,
	CT_EMCC,
	CT_PM5000
};

//LoadLibrary("Fdich.dll")

typedef DWORD (WINAPI *CheckAll)(DWORD dwDogType, char *pStrReturn, DWORD dwStrSize, DWORD dwCardType, DWORD &dwReturnCode);
//GetProcAddress(hModule, (LPCSTR)0x0001)
//dwDogType 标准机器用0x00,CO2机器用0x01
//pStrReturn 返回错误字符串
//dwStrSize 字符串大小,推荐256字节或更多
//dwCardType 控制卡类型CT_PCI3000,CT_DCP1000
//dwReturnCode 返回上述正确或错误状态码

typedef DWORD (WINAPI *CheckDogIsExist)(long nStyle, DWORD &dwReturnCode);
//GetProcAddress(m_hModule, (LPCSTR)0x0002)
//nStyle：
//		0表示仅仅检测狗是否存在
//		1表示检测狗的存在以及狗是否符合大族规范
//		2表示检测狗是否存在以及狗是否符合大族规范，以及检测狗ID是否相同 
//dwReturnCode 返回上述正确或错误状态码

typedef DWORD (WINAPI *GetDogIdAndLmtVerifyCode)(char *pszDogID, DWORD nDogIDSize, 
												 char *pszLmtVerifyCode, DWORD nLmtVerifySize);
												 //GetProcAddress(hModule, (LPCSTR)0x0004)
												 //pszDogID 输入字符串缓冲区得到狗的ID
												 //nDogIDSize 缓冲区大小，至少33个字节
												 //pszLmtVerifyCode 输入字符缓冲区得到狗的限制激活码
												 //nLmtVerifySize 输入字符，至少9个字节
												 
typedef DWORD (WINAPI *pOnDogRemove)(LPVOID lpContext);
typedef void (WINAPI *SetDogRemoveProc)(pOnDogRemove dogRemoveProc, LPVOID lpContext);
//GetProcAddress(hModule, (LPCSTR)0x0005)
//设置回调函数，当狗被拔掉时调用
//lpContext 输入回调函数参数

typedef DWORD (WINAPI *GetTypeVerifyCode)(char *pszTypeVerifyID, DWORD nTypeVerifySize);
typedef DWORD (WINAPI *GetDogVer)(DWORD &dwMainVer, DWORD &dwSubVer);
typedef DWORD (WINAPI *GetDogTimeInfo)(BYTE &bTmSupport, char *pszTmEsVerifyID, DWORD nTmEsVerifySize,
							BYTE &bTmEable, DWORD &dwTmUsed);
typedef DWORD (WINAPI *TimeCountStart)();
typedef DWORD (WINAPI *TimeCountSaveCur)();

//所有函数返回值均无意义，需使用输出参数dwReturnCode作返回值
//注意，后面的所有函数都需要先执行第一个CheckAll成功后才能使用

DWORD WINAPI OnDogRemove(LPVOID lpContext);

class CDogCheck  
{
public:
	CDogCheck();
	virtual ~CDogCheck();
	BOOL __CheckAll(DWORD dwSoftType, DWORD dwCardType);
	BOOL __CheckDogIsExist(long nStyle);
	void __GetDogIdAndLmtVerifyCode(char *pszDogID, DWORD nDogIDSize, 
								char *pszLmtVerifyCode, DWORD nLmtVerifySize);
	void __SetDogRemoveProc(pOnDogRemove dogRemoveProc, LPVOID lpContext);
public:
	BOOL DogTimeCountSaveCur();
	BOOL DogTimeCountStart();
	void __RegRemoveProc();
	CString GetExePath();
	BOOL __Dog();
	BOOL __CheckCode(DWORD dwCode, char *pszMsg);
	CheckAll m_pCheckAll;
	CheckDogIsExist m_pCheckDogIsExist;
	GetDogIdAndLmtVerifyCode m_pGetDogIdAndLmtVerifyCode;
	SetDogRemoveProc m_pSetDogRemoveProc;
	GetTypeVerifyCode m_pGetTypeVerifyCode;
	GetDogVer m_pGetDogVer;
	GetDogTimeInfo m_pGetDogTimeInfo;
	TimeCountStart m_pTimeCountStart;
	TimeCountSaveCur m_pTimeCountSaveCur;

	BOOL m_bInit;
	HMODULE m_hModule;

	char m_pszDogID[256];//狗ID
	char m_pszLmtVerifyID[256];//激活限制校验码
	char m_pszTypeVerifyID[256];//类别校验码
	DWORD m_dwDogVerMain;//狗主版本号
	DWORD m_dwDogVerSub;//狗次版本号
	BYTE m_bTmSupport;//是否支持狗时间统计
	char m_pszTmEsVerifyID[256];//时间校验码
	BYTE m_bTmEnable;//是否启用狗时间统计
	DWORD m_dwTmUsed;//狗已用时间，单位秒
};
extern CDogCheck g_DogCheck;
#endif // !defined(AFX_DOGCHECK_H__B9F5BC4D_500F_4466_B367_DFE5DA8D725F__INCLUDED_)
