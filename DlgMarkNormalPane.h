#pragma once
#include "afxwin.h"

#include "Led.h"
#include "FStatic.h"
#include "LaserController.h"
#include "SockDllDefine.h"
#include "socketclient.h"

#include "PowerMeter.h"
#include "LaserController.h"

#include "CMarkTicps.h"
#include "AdoDb.h"
#include "IOController.h"

#define MARK_MODEL_NORMAL_MARK			0
#define MARK_MODEL_FRAM					1
#define MARK_MODEL_GRID_MARK			2
#define MARK_MODEL_DIVIDE_NO_CCD		3
#define MARK_MODEL_DIVIDE_MANUAL_CCD	4
#define MARK_MODEL_DIVIDE_AUTO_CCD		5

// CDlgMarkNormalPane 对话框

class CMatrixMarkDoc
{
public:
	CString strMatrix;
	CString strPath;
};


class CDlgMarkNormalPane : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMarkNormalPane)

public:
	CDlgMarkNormalPane(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMarkNormalPane();

// 对话框数据
	enum { IDD = IDD_PANE_MARK_NORMAL };

public:	
	static CDlgMarkNormalPane *m_pDlgMarkNormalPane;	//打标时锁定软件
	static CLaserController *m_pLaserController;			//COM设备

	void ResetProductDocPara(int nItem); //nItem:0x04表示文档参数变更，0x08表示工作台参数变更
	BOOL SetCurrent(double dbCurrent);

	//Time
	CTime m_tmRunStart;
	DWORD m_dwMarkStart;
	DWORD m_dwSingleTime;
	DWORD m_dwMarkCount;
	void  UpdateTime();
	void  UpdatePos();

	//外部操纵接口
	BOOL m_bDisableOtherWindowInputOnMarking;
	int  m_nMarkError;  //0表示正常, 1表示打标检查出错，2表示CCD定位出错
	BOOL m_bMarking;
	CHLDoc * m_pDoc;
	CMarkMgr* m_pMarkMgr;
	BOOL CheckPower();
	void Mark() ;
	void MarkSEnd();
	void MarkEnd();

	BOOL ScanCodeMark();
	
	//IO控制
	void SetLightStandby();
	void SetLightRun();	
	void SetLightError();	
	void SetGas(BOOL bOpen);
	void RefreshGas();

	void LogStartInfoAndInitVariable();

	void MarkCheck();
	void CheckWorkbench();
	void CheckBox();
	void CheckRefPoints();
	void CheckDocument();

	void ScanMatrix();
	void ApplyMarkData();

	void MarkInit();
	void ResetBarcodeSize();
	void initOpcLaser();
	void InitLaserPara();	


	//CCD
	CLed* m_pLP[4];
	BOOL CheckLeftLP();
	void RefreshRefPointsListBox(int nSel);

	//MulDoc
	BOOL CheckMulDoc();
	CArray<CHLDoc*> m_arrDocPoint;

	//
	static BOOL m_bReplaceTxt;

public:
	CProgressCtrl	m_progressMark;
	int		m_nMarkType;
	int		m_nGasStatus;
	double	m_dbHandMoveDis;
	int		m_nProductSide;
	CButton m_btnHandPoint;
	CListBox m_listLocatePoint;
	CLed m_LP3;
	CLed m_LP2;
	CLed m_LP1;
	CLed m_LP0;
	CFStatic m_up;
	CFStatic m_down;
	CFStatic m_left;
	CFStatic m_right;
	CFStatic m_top;
	CFStatic m_bottom;
	CFStatic m_fast;
	CListBox m_lbxRefPoints;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
 	afx_msg void OnNcPaint();
	afx_msg void OnBnClickedButtonHadnPoint();
	afx_msg void OnBnClickedButtonMark();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnChangeEditHandMoveDis();
	afx_msg void OnBnClickedButtonToMark();
	afx_msg void OnBnClickedButtonToCCD();
	afx_msg void OnBnClickedButtonSelectToCCD();
	afx_msg void OnBnClickedButtonXy();
	afx_msg void OnBnClickedButtonZ();
	afx_msg void OnBnClickedButtonLoadPosition();
	afx_msg void OnBnClickedButtonUnloadPosition();
	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnBnClickedButtonStopMove();
	afx_msg void OnBnClickedButtonZToFocus();
	afx_msg void OnBnClickedButtonSwitchGasStatus();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonHandOkPoint();
	afx_msg void OnBnClickedButtonCheckPoint();
	afx_msg HRESULT OnResetProductDocPara(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnShowMarkingInfoBox(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnMarkEnd(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnMarkStart(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnMarkStop(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnMarkPause(WPARAM wParam,LPARAM lParam);
	afx_msg HRESULT OnEmergencyEventHandler(WPARAM wParam,LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonRefPointAdd();
	afx_msg void OnBnClickedButtonRefPointArray();
	afx_msg void OnBnClickedButtonRefPointDel();
	afx_msg void OnBnClickedButtonZToFocus2();
	afx_msg void OnSelchangeListRefPoints();
	afx_msg void OnBnClickedButtonMarkCountClear();
	BOOL m_bMulDocMark;
	BOOL m_bgoLoadPt;
	BOOL m_bgoUnLPt;
	BOOL m_bInputMark;
	BOOL m_bDoor;
	BOOL m_bInputError;
	afx_msg void OnBnClickedBtnLoadscan();
	void GetIOinfo(void);
	afx_msg void OnBnClickedBtnCcdview();
	PARA m_oldMarkPara;
	BOOL m_bFirstPts;
	afx_msg void OnBnClickedBtnGetcode();

	CString m_strIPAddress;
	int m_nPort;
	IMFCSockDll m_sock;                    //处理网络事件
	BOOL m_bConnected;
	int m_nMainSockID;
	int m_ListenSockID;
	BOOL m_bIsNet;

	void ConnectNet();
	void ShowConnect();
	void DisConnectNet();
	CString m_NetInfo;
//	afx_msg void OnClose();
	CSocketClient m_sockClient;
	void UpdateMotionView(BOOL bEnable);
	void CheckMachineReady();
	afx_msg void OnBnClickedBtnLfdata();
	BOOL LoadFileData();
	afx_msg void OnDestroy();
	int m_nCutNum;

	CPowerMeter *m_pPowerMeter;


	BOOL GetNodeCodeLength();
	BOOL SetNodeCodeLength();

	SeriQuDot m_QuadDot[100];
	CString m_strOperatorcode;

	CString m_strMarking;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CFont m_font_;
	BOOL PreNetSendRecv(CString & strCode);

	CString m_strSendCode;

	CString m_strSendLast;
	afx_msg void OnBnClickedBtnAutooffset();
	void OffsetMark(void);
	BOOL Autooffset();
	BOOL m_bSetTimeAutoOffset;
	CHLDoc * m_pOldDoc;
	CString m_strReverMatriCode;
	afx_msg void OnBnClickedBtnOpclaserReset();


	CArray<CMatrixMarkDoc> m_arrMatrixMarkDoc;

	void initMatrixDoc();
	BOOL ScanMatriDoc(CString& strBarcode, CString &strErrInfo);
	BOOL m_bStandby;
	afx_msg void OnBnClickedButtonLightStandby();
	afx_msg void OnBnClickedButtonMoveScanpt();


	CAdoDB  m_adoDB;
	CAdoRecordset m_recordset;
	BOOL ConnectDatebase();
	BOOL SaveMarkCode(CString strVinCode);
	CString m_strSaveMatrix;
	afx_msg void OnBnClickedButtonRecord();

	BOOL PreProcessBarcode(CString strScanText);
	afx_msg void OnBnClickedButtonDust();
	BOOL m_bOpenDust;
	BOOL m_bSingleRefPointMark;
};

extern CDlgMarkNormalPane *g_pDlgMarkNormalPane;