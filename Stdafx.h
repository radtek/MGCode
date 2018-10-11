// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B8BC1849_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_STDAFX_H__B8BC1849_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define _CRT_SECURE_NO_WARNINGS

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include <WinSDKVer.h>
#define WIN32_WINNT 0x0501
#include <sdkddkver.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

const DWORD OBJ_MAX_MARKCOUNT = 6;

extern int g_nLeftDay;

#ifndef PCI3000_MARK_LIB
//#define PCI3000_MARK_LIB
#endif

#ifndef _DOUBLE_HEAD_MARK
//#define _DOUBLE_HEAD_MARK
#endif


#define EPSINON  (1e-10)
#define SAMEVALUE(a,b) (fabs(a-b)< EPSINON)

#define LAYERCOUNT 256
#pragma warning(disable:4786)
//�����Ǳ�������ӵĿ�
#include <math.h>
#include <conio.h>
//�Դ�������л�������
#include "MySerialize.h"
#include "InnerMsg.h"
#include "Layer.h"

//�����̣߳���ʼ�������
#define WM_MARK_END  (WM_USER+5)
#define WM_MARK_START (WM_USER+6)
#define WM_MARK_STOP  (WM_USER+7)
#define WM_MARK_PAUSE (WM_USER+10)

#define IDC_STATUS_BAR_BUTTON (WM_USER+8)
#define IDC_STATUS_PROGRESSCtrl (WM_USER+9)

#define WM_SELMARK_END  (WM_USER+11)
#define WM_FASTMARK_END  (WM_USER+12)

#define  WM_BN_CLICK  (WM_USER + 100)
#define  USE_TOPINDEX_BUTTON

#define WM_RESET_PRODUCT_DOC_PARA	(WM_USER+101)
#define WM_SHOW_MARKING_INFO_BOX (WM_USER+102)
//�����Ǳ��������ĺ궨��

#define PAI 3.141592653589793		//Բ����
#define CR (char)0x0d+(char)0x0a	//�س�

#define CENTER 0				//ʵ���������ĵ�

//���������괿����������任
#define LCENTER	16384			//�߼����ĵ�
#define DPMM	50				//�߼������е�ÿ������ʾ�ĵ���

//����խ�����
#define basenar 0.191

#define UPDATE_TRACKER_VIEW		1
#define UPDATE_TRACKER_PROPERTY	2
#define	UPDATE_LOGIC			3	
#define UPDATE_DEVICE			4
#define	UPDATE_QUAD			5

#define OFFSET_TEXTID(nBASE,nTextID)	nBASE+(nTextID-900)

#define PTY_INIT	0	//��ʼ��
#define PTY_LAYER	1	//����ɫ�ı�
#define PTY_PARA	2	//��������ı�
#define PTY_OUTLINE	3	//�ƶ��߿�
#define PTY_DEL		4	//ɾ����
#define PTY_FILL	5	//�ƶ������
#define PTY_ADD		6	//�����²�
#define PTY_CHANGE  7	//�ı䵱ǰ��

//������ɫ
#define BLACK 		RGB(0,0,0)
#define	GRAY	 	RGB(127,127,127)
#define	GREEN 		RGB(0,255,0)
#define	YANGREEN	RGB(0,255,255)
#define	YELLOW		RGB(255,255,0)
#define	BLUE  		RGB(0,0,255)
#define	MAGENTA		RGB(255,0,255)
#define RED   		RGB(255,0,0)
#define	BROWN		RGB(182,42,42)
#define  PINK		RGB(255,192,203)


//���������λ��
#define BASE_TL	0
#define BASE_TM	1
#define BASE_TR	2
#define BASE_ML	3
#define BASE_MM	4
#define BASE_MR	5
#define BASE_BL 6
#define BASE_BM 7
#define BASE_BR	8

//������䷽ʽ
#define FILL_HOR 0	
#define FILL_VER 1

//����������ʽ
#define POLYGON_LINE 0
#define POLYGON_STAR 1
#define POLYGON_CROSS 2

//�����ı���ʽ
#define TEXT_FIX	0
#define TEXT_SERIAL	1
#define TEXT_DATE	2

//�����Ų���ʽ
#define PATH_LINE	0
#define PATH_ROUND	1

//������뷽ʽ
#define ALIGN_LEFT	0
#define ALIGN_RIGHT	1
#define ALIGN_MID	2

//��������
#define FONT_TTF	0
#define FONT_SHX	1
#define FONT_MATRIX 2
//

//ʰȡ���λ��
#define PT			0
#define	TRACKLEFT		5	
#define	TRACKRIGHT		7
#define	UPPER		4
#define	LOWER		6
#define	UPPER_LEFT	0
#define	UPPER_RIGHT	1
#define	LOWER_LEFT	3
#define	LOWER_RIGHT	2
#define TRACKCENTER 9				//���ĵ�

void GetExepath(char *pPath);
double MyPrecision(double val,int digit=3);
double MyLogToTrue(double val);
double MyTrueToLog(double val);
double MyDegToRad(double val);
double MyRadToDeg(double val);

typedef int (APIENTRY *LPLOADDIB)(char*,int&,int&,BOOL,BOOL);
typedef int (APIENTRY *LPPROCDIB)(BYTE*,int,int,int,int,int,long,long);
typedef int (APIENTRY *LPPROCGRAYDIB)(BYTE*,int,int,int,int,int,long,long);
typedef int (APIENTRY *LPDELDIB)();


typedef BOOL (WINAPI *pINITAUTOPARASET)();
typedef BOOL (WINAPI *pDESTORYAUTOSET)();
typedef BOOL (WINAPI *pWRITEAUTOPARA)(double dbCurrent, double dbQSwitch, PARA &para);
typedef BOOL (WINAPI *pREADAUTOPARA)(double dbCurrent, double dbQSwitch, PARA &para);

extern LPLOADDIB LoadDib;
extern LPPROCDIB ProcDib;
extern LPPROCGRAYDIB ProcGrayDib;
extern LPDELDIB DelDib;

extern pINITAUTOPARASET InitAutoParaSet;
extern pDESTORYAUTOSET  DestoryAutoSet;
extern pWRITEAUTOPARA   WriteAutoPara;
extern pREADAUTOPARA    ReadAutoPara;


double ReadIniDouble(char *pszFileName, LPCTSTR lpszSection
				  , LPCTSTR lpszKey, LPCTSTR lpszDefaultValue);
void   WriteIniDouble(char *pszFileName, LPCTSTR lpszSection
				   , LPCTSTR lpszKey, double dbValue);

int  ReadIniInt(char *pszFileName, LPCTSTR lpszSection
			   , LPCTSTR lpszKey, LPCTSTR lpszDefaultValue);
void WriteIniInt(char *pszFileName, LPCTSTR lpszSection
			   , LPCTSTR lpszKey, int nValue);

 
#define MARKPREVIEW_SPEEDMAX	80		           // ���Ԥ���ٶȵ����ֵ

#define MAXDELAY	65535	// �����ʱ

extern DWORD64 g_OBJID;     // CTree����ID

extern LANGID  g_LangID;    // ϵͳ����ID
extern DWORD   g_dwVersion; // �汾��

extern SYSCFG g_SysCfg;                            // ϵͳ������Ϣ
//extern SYSTEMPARAM g_SysPara;                      // ϵͳ������

//ȫ���ⲿ����
extern DWORD g_dwJumpCnt;				// ����ʱ�����Զ�����
extern BOOL g_bIsSort;                  // ϵͳ��ǰ�Ƿ�������״̬
extern BOOL g_bIsRedPreview;
extern BOOL g_bIsSelMark;
extern BOOL g_bIsFastMark;
extern USERINFO g_CurUserInfo;


//IOѡ����ĵ��źſ�
#define I_MULSEL_0	0x02 
#define I_MULSEL_1	0x04
#define I_MULSEL_2	0x08
#define I_MULSEL_3	0x10
#define I_MULSEL_4	0x20

#define O_INIT_STS			0x04//����ڳ�ʼ�������ڳ�ʼ��ʱ���
//����ڶ�Ӧ���
#define O_POWER			0x40	//���ػ�
#define O_RED			0x20	//���ú��
#define O_ENT_MARK_ENV		0x10 //��բ��Ϣ�������껷��ʱOn,�˳���껷��ʱOff
#define O_MARK_END_PULSE 0x08 //�����������ź�
#define O_MARK_R		0x04 //������ź�ȡ��
#define O_MARK			0x02 //�����


//����༭״̬
#define BARSETERROR			0//error return
#define BARSETEDIT			1//only edit item
#define BARDELORADD			2//add or del item


#include "MgrText.h"
#include "system.h"
#include "BarCodeMgr.h"

// ����ڵ�����
enum ENU_OBJTYPE
{
	NODE_BASE		= 0,  // δ֪�ڵ�
	NODE_LINE		= 1,  // ���� 
	NODE_RECT		= 2,  // ����
	NODE_TEXT_DSF	= 3,  // �ı�
	NODE_SERIAL		= 4,  // ����
	NODE_BMP		= 5,  // BMP
	NODE_ELLIPSE	= 6,  // Բ
	NODE_PLT		= 7,  // PLT
	NODE_GRID		= 8,  // ����
	NODE_POLYGON	= 9,  // �����
	NODE_DXF		= 10, // DXF
	NODE_CODEBAR	= 11, // ��ά����
	NODE_COMBIN     = 12, // ��϶���
	NODE_UNION		= 13, // Ⱥ��
	NODE_DOT		= 14,
	NODE_GERBER		= 15,
	NODE_NC			= 25,
	NODE_BUTN
};

#define NEEDOUTPUTDBG

#ifdef NEEDOUTPUTDBG

#define DSF_TRACE(sz) OutputDebugString(sz)

#define MYTRACE(sz)\
OutputDebugString(sz);

#define MYTRACE1(sz, p1){\
	char szTemp[256];\
	sprintf(szTemp, sz, p1);\
OutputDebugString(szTemp);}

#define MYTRACE2(sz, p1, p2){\
	char szTemp[256];\
	sprintf(szTemp, sz, p1, p2);\
OutputDebugString(szTemp);}

#define MYTRACE3(sz, p1, p2, p3){\
	char szTemp[256];\
	sprintf(szTemp, sz, p1, p2, p3);\
OutputDebugString(szTemp);}

#define MYTRACE4(sz, p1, p2, p3, p4){\
	char szTemp[256];\
	sprintf(szTemp, sz, p1, p2, p3, p4);\
OutputDebugString(szTemp);}

#define MYTRACE8(sz, p1, p2, p3, p4, p5, p6, p7, p8){\
	char szTemp[256];\
	sprintf(szTemp, sz, p1, p2, p3, p4, p5, p6, p7, p8);\
OutputDebugString(szTemp);}

#define MYTRACE10(sz, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10){\
	char szTemp[256];\
	sprintf(szTemp, sz, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);\
OutputDebugString(szTemp);}

#else
#define DSF_TRACE(sz)
#define MYTRACE(sz)
#define MYTRACE1(sz,p1)
#define MYTRACE2(sz,p1,p2)
#define MYTRACE3(sz,p1,p2,p3)
#define MYTRACE4(sz,p1,p2,p3,p4)
#define MYTRACE8(sz, p1, p2, p3, p4, p5, p6, p7, p8)
#define MYTRACE10(sz,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10)

#endif //End NEEDOUTPUTDBG

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
//��ɾ�� - Ŀǰ��ĩ�õ��ĺ궨��
//�����������
#define	FONT_HZK_16		16
#define	FONT_HZK_14		14
#define	FONT_HZK_12		12
#define	FONT_ASC_8_16	8
#define	FONT_ASC_5_7	5

#define NUM_DOTOFCHAR 135
#define NUMCODE       92
#define NUM_ASCII     130	


#include "gdiplus.h"
using namespace Gdiplus; 

#include "HlParaIni.h"
enum RunLogLevel{eLogDataFlow, eLogDebug, eLogTime};
extern void WriteRunLog(LPCTSTR szLog, int nLevel=eLogDataFlow);

#include "HLException.h"
typedef BOOL (WINAPI *lpfn) (HWND hWnd, COLORREF cr,BYTE bAlpha, DWORD dwFlags);

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#endif // !defined(AFX_STDAFX_H__B8BC1849_DE24_11D3_845D_444553540001__INCLUDED_)
