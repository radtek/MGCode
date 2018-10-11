#ifndef __MARK_API_DEF_H__
#define __MARK_API_DEF_H__

enum ENUM_SIG { SIG_LASER, SIG_MARK, SIG_N_MARK, SIG_END, SIG_SHUTTER };
enum ENUM_TTL {TTL_LOW, TTL_HIGH}; 

typedef UINT (*T_HS_OpenCard)(int nCardSerial);
/*
0��	UINT T_HS_OpenCard()
˵������ʼ����
�������壺
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_HS_CloseCard)(int nCardSerial);
/*
1��	UINT T_HS_CloseCard()
˵������ʼ����
�������壺
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_HS_MarkStroke)(int nCardSerial, UINT nType, LPVOID pMarkData,  int nDotCount, UINT nLayerIndex);
/*
2.UINT T_HS_MarkStroke(UINT nType, PSTROKE pStroke);
˵����������������������߿�Ԥ��ʱ���߿����ݣ����ʱ��������ݣ�
�������壺
nType��0�������� 1��������
pStroke��ʵ�ʴ����������
nLayerIndex: ������
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_HS_SetMarkPara)(int nCardSerial, LPVOID pPara, int nLayCount);
/*
3��	UINT T_HS_SetMarkPara(LAYER_MAP* pmapLayer)
˵�������ô�����
�������壺
		pmapLayer������������ݣ���0-n�����ݣ���С��n�����ݣ�n�Ϳ��ƿ��йأ�
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_HS_EnterMarkEnv)(int nCardSerial);
/*
4��	UINT T_HS_EnterMarkEnv()
˵���������껷��
�������壺
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_HS_LeaveMarkEnv)(int nCardSerial);
/*
5��	UINT T_HS_LeaveMarkEnv()
˵�����˳���껷��
�������壺
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_HS_MarkTaskStart)(int nCardSerial, UINT nType, UINT nSts);
/*
6��	UINT T_HS_MarkTaskStart(UINT nType)
˵������꿪ʼ����������ʱ��ʱ������֪ͨ��ؿ��Ĵ���ʼ������������״̬�źŵȣ�
�������壺
		nType��0��ʾʵ�ʴ�� 1��ʾ���Ԥ��
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_HS_MarkTaskEnd)(int nCardSerial, UINT nType, UINT nSts);
/*
7��	UINT T_HS_MarkTaskEnd()
˵��������������ֹ����ʱ��ʱ������֪ͨ��ؿ��Ĵ���������������״̬�źŵȣ�
�������壺
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_MarkTaskPause)();
/*
8��	UINT T_MarkTaskPause()
˵���������ͣ����֪ͨ��ؿ��Ĵ����ͣ����������״̬�źŵȣ�
�������壺
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_HS_MarkTaskEnd)(int nCardSerial, UINT nType, UINT nSts);
/*
9��	UINT T_HS_MarkTaskEnd()
˵���������ֹ����֪ͨ��ؿ��Ĵ����ֹ������������״̬�źŵȣ�
�������壺
����ֵ��0�ɹ� 1ʧ��
*/


typedef UINT (*T_MarkTaskResume)();
/*
10��	UINT T_MarkTaskResume()
˵��������������֪ͨ��ؿ��Ĵ���ʼ������������״̬�źŵȣ�
�������壺
����ֵ��0�ɹ� 1ʧ��
*/


typedef UINT (*T_HS_SetCardPara)(int nCardSerial, char*pFileName);
/*
15��	UINT T_HS_SetCardPara(int nCardSerial, char*pFileName)
˵������ʼ��������ƿ�����
�������壺
		pmapCardPara����������ָ�루�ڵ��б��������ַ������ݣ�
����ֵ��0�ɹ� 1ʧ��
*/

typedef UINT (*T_HS_CMD_GetVersionInfo)(int nCardSerial, char* szVersionInfo, int nLen);
/*
16��	UINT T_HS_CMD_GetVersionInfo(int nCardSerial, char* szVersionInfo, int nLen)
˵�����õ�������̬��汾��
�������壺[OUT] szVersionInfo - �汾�����ַ���
����ֵ������汾��(λ��Ϣ��ʽ����������й�)
*/

typedef UINT (*T_HS_CMD_SetCurrent)(int nCardSerial, double dbCurrent);
/*
17�� UINT T_HS_CMD_SetCurrent(int nCardSerial, double dbCurrent)
˵�������õ���ֵ
�������壺dbCurrent -- ����ֵ
����ֵ��δ����
*/


typedef UINT (*T_HS_CMD_GetLibFlags)(int nCardSerial, UINT nLibSts);
/*
18. UINT T_HS_CMD_GetLibFlags()
	˵������ȡ���ӱ�־
	��������
	����ֵ��BIT 0: 0 - ��ʾ���ʱ�����ϲ������㣬 1 - ��ʾ���ʱ���������������
	��ע���˺���Ϊ��ѡʵ�ֺ���
*/


typedef UINT (*T_DeviceIsNormal)();
/*
20.BOOL DeviceIsNormal()
˵�����ж��豸�Ƿ�����
�������壺��
����ֵ��0 - ���� 1 - ������
*/

typedef UINT (*T_HS_CMD_MarkStart)(int nCardSerial);

typedef UINT (*T_HS_CMD_MarkPause)(int nCardSerial);

typedef UINT (*T_HS_CMD_MarkResume)(int nCardSerial);

typedef UINT (*T_HS_CMD_MarkStop)(int nCardSerial);

typedef UINT (*T_HS_CMD_GetMarkTime)(int nCardSerial, int *pMarkTime);

typedef UINT (*T_HS_CMD_JumpTo)(int nCardSerial, double x, double y);

typedef UINT (*T_HS_CMD_LaserON)(int nCardSerial, double dbQFreq, double dbQRelease, int nDelay);

typedef UINT (*T_HS_CMD_LaserOff)();

typedef UINT (*T_HS_CMD_GetInput)(int nCardSerial, UINT *pInput);

typedef UINT (*T_HS_CMD_GetOutput)(int nCardSerial, UINT *pOutput);

typedef UINT (*T_HS_CMD_Output)(int nCardSerial, UINT nMsk, UINT nSts);

typedef UINT (*T_HS_CMD_GetCardSts)(int nCardSerial, UINT *pCardSts);

typedef UINT (WINAPI *T_HS_SetOffset)(int nCardSerial, DOT6 dtOffset);

typedef UINT (*T_HS_CMD_SetObjAngle)(DOT6 &LDot, double dbLAngle, DOT6 &RDot, double dbRAngle);


#endif //__DRV_API_DEF_H__