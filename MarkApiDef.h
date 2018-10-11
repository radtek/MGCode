#ifndef __MARK_API_DEF_H__
#define __MARK_API_DEF_H__

enum ENUM_SIG { SIG_LASER, SIG_MARK, SIG_N_MARK, SIG_END, SIG_SHUTTER };
enum ENUM_TTL {TTL_LOW, TTL_HIGH}; 

typedef UINT (*T_HS_OpenCard)(int nCardSerial);
/*
0、	UINT T_HS_OpenCard()
说明：初始化卡
参数定义：
返回值：0成功 1失败
*/

typedef UINT (*T_HS_CloseCard)(int nCardSerial);
/*
1、	UINT T_HS_CloseCard()
说明：初始化卡
参数定义：
返回值：0成功 1失败
*/

typedef UINT (*T_HS_MarkStroke)(int nCardSerial, UINT nType, LPVOID pMarkData,  int nDotCount, UINT nLayerIndex);
/*
2.UINT T_HS_MarkStroke(UINT nType, PSTROKE pStroke);
说明：打标所传数据线条（边框预览时传边框数据，打标时传打标数据）
参数定义：
nType：0线条数据 1点阵数据
pStroke：实际打标数据线条
nLayerIndex: 层索引
返回值：0成功 1失败
*/

typedef UINT (*T_HS_SetMarkPara)(int nCardSerial, LPVOID pPara, int nLayCount);
/*
3、	UINT T_HS_SetMarkPara(LAYER_MAP* pmapLayer)
说明：设置打标参数
参数定义：
		pmapLayer：打标层参数内容，由0-n层数据（可小于n层数据，n和控制卡有关）
返回值：0成功 1失败
*/

typedef UINT (*T_HS_EnterMarkEnv)(int nCardSerial);
/*
4、	UINT T_HS_EnterMarkEnv()
说明：进入打标环境
参数定义：
返回值：0成功 1失败
*/

typedef UINT (*T_HS_LeaveMarkEnv)(int nCardSerial);
/*
5、	UINT T_HS_LeaveMarkEnv()
说明：退出打标环境
参数定义：
返回值：0成功 1失败
*/

typedef UINT (*T_HS_MarkTaskStart)(int nCardSerial, UINT nType, UINT nSts);
/*
6、	UINT T_HS_MarkTaskStart(UINT nType)
说明：打标开始，启动打标计时定时器，且通知相关卡的打标初始化动作（如打标状态信号等）
参数定义：
		nType：0表示实际打标 1表示红光预览
返回值：0成功 1失败
*/

typedef UINT (*T_HS_MarkTaskEnd)(int nCardSerial, UINT nType, UINT nSts);
/*
7、	UINT T_HS_MarkTaskEnd()
说明：打标结束，终止打标计时定时器，且通知相关卡的打标结束动作（如打标状态信号等）
参数定义：
返回值：0成功 1失败
*/

typedef UINT (*T_MarkTaskPause)();
/*
8、	UINT T_MarkTaskPause()
说明：打标暂停，且通知相关卡的打标暂停动作（如打标状态信号等）
参数定义：
返回值：0成功 1失败
*/

typedef UINT (*T_HS_MarkTaskEnd)(int nCardSerial, UINT nType, UINT nSts);
/*
9、	UINT T_HS_MarkTaskEnd()
说明：打标中止，且通知相关卡的打标中止清理动作（如打标状态信号等）
参数定义：
返回值：0成功 1失败
*/


typedef UINT (*T_MarkTaskResume)();
/*
10、	UINT T_MarkTaskResume()
说明：打标继续，且通知相关卡的打标初始化动作（如打标状态信号等）
参数定义：
返回值：0成功 1失败
*/


typedef UINT (*T_HS_SetCardPara)(int nCardSerial, char*pFileName);
/*
15、	UINT T_HS_SetCardPara(int nCardSerial, char*pFileName)
说明：初始化激光控制卡参数
参数定义：
		pmapCardPara：设置链表指针（节点中保存属性字符及内容）
返回值：0成功 1失败
*/

typedef UINT (*T_HS_CMD_GetVersionInfo)(int nCardSerial, char* szVersionInfo, int nLen);
/*
16、	UINT T_HS_CMD_GetVersionInfo(int nCardSerial, char* szVersionInfo, int nLen)
说明：得到驱动动态库版本号
参数定义：[OUT] szVersionInfo - 版本描述字符串
返回值：具体版本号(位信息格式与具体驱动有关)
*/

typedef UINT (*T_HS_CMD_SetCurrent)(int nCardSerial, double dbCurrent);
/*
17、 UINT T_HS_CMD_SetCurrent(int nCardSerial, double dbCurrent)
说明：设置电流值
参数定义：dbCurrent -- 电流值
返回值：未定义
*/


typedef UINT (*T_HS_CMD_GetLibFlags)(int nCardSerial, UINT nLibSts);
/*
18. UINT T_HS_CMD_GetLibFlags()
	说明：获取附加标志
	参数：无
	返回值：BIT 0: 0 - 表示打标时间由上层程序计算， 1 - 表示打标时间由驱动程序计算
	备注：此函数为可选实现函数
*/


typedef UINT (*T_DeviceIsNormal)();
/*
20.BOOL DeviceIsNormal()
说明：判断设备是否正常
参数定义：无
返回值：0 - 正常 1 - 不正常
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