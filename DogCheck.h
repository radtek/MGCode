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
	FRC_SUCCESS,//���سɹ�
	FRC_SUCESS_WITH_USERID,//�����ŷ��سɹ�
	FRC_SUCCESS_WITH_TIMESHOW,//�ɹ�����ʱʹ�ã�������ʹ��ʣ��ʱ��
	//�����Ȩ����������%d�죬��Ҫ��ϵ����ͷ���Ա������������(��ID=%s, У����=%s)ƥ��������Ȩ�������Ա㵽�ں��ܼ���ʹ��
	FRC_SUCCESS_WITH_INSPECT,//�ɹ���Ѳ������
	//�豸����������һ�꣬��Ҫ��ϵ����ͷ���Ա�����豸����ά�����ޣ���ȷ����ȫ�������У�
	//��������(��ID=%s, У����=%s)ƥ��������Ȩ����ȷ�ϲ���
		
	FRC_FAIL_DOG_LIB_LOAD,//���������������̬��ʧ��
	FRC_FAIL_DOG_LIB_GUID,//����ʧ�ܣ�HansLmtH.dllģ������Ͳ�ƥ��
	FRC_FAIL_DOG_LIB_VERSION,//����ʧ�ܣ�HansLmtH.dllģ���汾������V1.1.0
	//ģ�������ʧ�ܻ�ģ���汾����ȷ�����������װ�Ƿ���ȷ
		
	FRC_FAIL_DOG_EXECUTE,//����ʼ��ת�������й�ʧ��
	//����������쳣�����⹷�����Ƿ������������Ƿ�װ��ȷ
		
	FRC_FAIL_DOG_TIMEOUT,//����ʱ���Ѿ�����
	//��ǰ�����Ȩ�ѵ��ڣ���Ҫ��ϵ����ͷ���Ա������������(��ID=%s, У����=%s)ƥ��������Ȩ�������Ա����ʹ��
		
	FRC_FAIL_DOG_WRONG_TYPE,
	//!=0x00��������Ͳ�ƥ�䣬��ȷ��������Ƿ��Ǹ��豸���׵����������������ϵ����ͷ���Ա
	//!=0x01��������Ͳ�ƥ�䣬��ȷ��������Ƿ��Ǹ��豸���׵��������������ȷ����������ƿ���������Ƿ���Ҫ����
		
	FRC_FAIL_DOG_CMP_CARD,//���꿨����Ӧ
		
	FRC_FAIL_DOG_FUNC_UNKNOW,//�������з���δ֪����
	FRC_FAIL_DOG_FUNC_NODOG,//û�м�⵽�����
	FRC_FAIL_DOG_FUNC_WRONGDOG,//��⵽������������ǺϷ��������
		
	FRC_FAIL_DOG_FUNC_CHECK_WRONG_BEFORE_EXECUTE,//��Execute��������ǰû�гɹ����ù�Check����
	FRC_FAIL_DOG_FUNC_LIB_L_VERSION,//HansLmtL.dllģ���汾����V1.1.0
	//ģ�������ʧ�ܻ�ģ���汾����ȷ�����������װ�Ƿ���ȷ
		
	FRC_FAIL_DOG_FUNC_MOREDOG,//��������������1��
	FRC_FAIL_DOG_FUNC_WRONG_PARAM,//�������������֧��
		
	FRC_FAIL_PARA_UNKNOW_CARDTYPE,//δ֪�Ŀ��ƿ�����
	FRC_FAIL_PARA_UNKONW_DOGTYPE,//δ֪�Ĺ�����
		
	FRC_FAIL_CARD_LIB_LOAD,//���ƿ������ʧ��
	FRC_FAIL_CARD_FUNC_GET,//ȡ���ƿ���ӿں���ʧ��
	FRC_FAIL_CARD_NONE,//�޿��ƿ�
	FRC_FAIL_CARD_WRITE_NEW,//д�¿�ʧ��
	//д��ʧ�ܣ�������ƿ������ӻ���
		
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
//dwDogType ��׼������0x00,CO2������0x01
//pStrReturn ���ش����ַ���
//dwStrSize �ַ�����С,�Ƽ�256�ֽڻ����
//dwCardType ���ƿ�����CT_PCI3000,CT_DCP1000
//dwReturnCode ����������ȷ�����״̬��

typedef DWORD (WINAPI *CheckDogIsExist)(long nStyle, DWORD &dwReturnCode);
//GetProcAddress(m_hModule, (LPCSTR)0x0002)
//nStyle��
//		0��ʾ������⹷�Ƿ����
//		1��ʾ��⹷�Ĵ����Լ����Ƿ���ϴ���淶
//		2��ʾ��⹷�Ƿ�����Լ����Ƿ���ϴ���淶���Լ���⹷ID�Ƿ���ͬ 
//dwReturnCode ����������ȷ�����״̬��

typedef DWORD (WINAPI *GetDogIdAndLmtVerifyCode)(char *pszDogID, DWORD nDogIDSize, 
												 char *pszLmtVerifyCode, DWORD nLmtVerifySize);
												 //GetProcAddress(hModule, (LPCSTR)0x0004)
												 //pszDogID �����ַ����������õ�����ID
												 //nDogIDSize ��������С������33���ֽ�
												 //pszLmtVerifyCode �����ַ��������õ��������Ƽ�����
												 //nLmtVerifySize �����ַ�������9���ֽ�
												 
typedef DWORD (WINAPI *pOnDogRemove)(LPVOID lpContext);
typedef void (WINAPI *SetDogRemoveProc)(pOnDogRemove dogRemoveProc, LPVOID lpContext);
//GetProcAddress(hModule, (LPCSTR)0x0005)
//���ûص��������������ε�ʱ����
//lpContext ����ص���������

typedef DWORD (WINAPI *GetTypeVerifyCode)(char *pszTypeVerifyID, DWORD nTypeVerifySize);
typedef DWORD (WINAPI *GetDogVer)(DWORD &dwMainVer, DWORD &dwSubVer);
typedef DWORD (WINAPI *GetDogTimeInfo)(BYTE &bTmSupport, char *pszTmEsVerifyID, DWORD nTmEsVerifySize,
							BYTE &bTmEable, DWORD &dwTmUsed);
typedef DWORD (WINAPI *TimeCountStart)();
typedef DWORD (WINAPI *TimeCountSaveCur)();

//���к�������ֵ�������壬��ʹ���������dwReturnCode������ֵ
//ע�⣬��������к�������Ҫ��ִ�е�һ��CheckAll�ɹ������ʹ��

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

	char m_pszDogID[256];//��ID
	char m_pszLmtVerifyID[256];//��������У����
	char m_pszTypeVerifyID[256];//���У����
	DWORD m_dwDogVerMain;//�����汾��
	DWORD m_dwDogVerSub;//���ΰ汾��
	BYTE m_bTmSupport;//�Ƿ�֧�ֹ�ʱ��ͳ��
	char m_pszTmEsVerifyID[256];//ʱ��У����
	BYTE m_bTmEnable;//�Ƿ����ù�ʱ��ͳ��
	DWORD m_dwTmUsed;//������ʱ�䣬��λ��
};
extern CDogCheck g_DogCheck;
#endif // !defined(AFX_DOGCHECK_H__B9F5BC4D_500F_4466_B367_DFE5DA8D725F__INCLUDED_)
