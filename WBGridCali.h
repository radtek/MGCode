//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WBGridCali)
#define AFX_WBGridCali

#ifdef WBGC_EXPORTS
#define WBGC_API __declspec(dllexport)
#else
#define WBGC_API __declspec(dllimport)
#endif

extern "C" {

class WBGC_API CGridData  
{
public:
	int m_nVersion;
	int m_nRowNum;
	int m_nColNum;
	double m_dbDotDisX;
	double m_dbDotDisY;
	double m_dotsX[500][500];
	double m_dotsY[500][500];
	BOOL   m_bOk[500][500];

	CGridData();
};

//API
WBGC_API void WBGC_GetVersion(char buffer[32]);

WBGC_API BOOL WBGC_LoadGridData(LPCSTR strFileTitle, void *pGridData);
WBGC_API BOOL WBGC_SaveGridData(LPCSTR strFileTitle, void *pGridData);

WBGC_API int  WBGC_InitCaliGrid(LPCSTR strFileTitle, void *pGridData=NULL);
WBGC_API BOOL WBGC_SoftToHard(double &dbX, double &dbY);
WBGC_API BOOL WBGC_HardToSoft(double &dbX, double &dbY);

//Grid Dialog
extern WBGC_API BOOL g_bCCDGridImported;
extern WBGC_API BOOL g_bStopWBGridDialogRun;
extern WBGC_API CGridData *g_pCCDGridData;
extern WBGC_API CGridData *g_pMeasureGridData;

typedef	 void (*WBGC_PosCallback)(double &dbX, double &dbY);
typedef  BOOL (*WBGC_MoveCallback)(double dbX, double dbY);
typedef  BOOL (*WBGC_CCDCallback)(double &dbX, double &dbY, BOOL &bPrecisionOK);
WBGC_API void WBGC_ShowGridDialog(WBGC_PosCallback pfnPosCallback, WBGC_MoveCallback pfnMoveCallback, WBGC_CCDCallback pfnCCDCallback, int nLang=0);

//RTC File
WBGC_API BOOL WBGC_AngleAdjust(CGridData *pGridData, BOOL bBaseCenter, char szErrInfo[256]);
WBGC_API int  WBGC_CreateRTC4File(CGridData *pGridData, BOOL bAngleAdjust, int nBitMM, LPCSTR szOldFile, char szNewFile[256], char szErrInfo[256]);
WBGC_API int  WBGC_CreateRTC5File(CGridData *pGridData, BOOL bAngleAdjust, int nBitMM, LPCSTR szOldFile, char szNewFile[256], char szErrInfo[256]);
}
#endif