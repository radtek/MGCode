// HLDoc.h : interface of the CHLDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLDOC_H__B8BC184F_DE24_11D3_845D_444553540001__INCLUDED_)
#define AFX_HLDOC_H__B8BC184F_DE24_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DocPipe.h"
#include "MgrText.h"
#include "XDC.h"
#include "XTracker.h"
#include "Quad.h"	// Added by ClassView
#include "stdafx.h"
#include "SortListObj.h"
#include "AutoSortListObj.h"
#include "DivideLayer.h"
#include "StrokeList.h"
#include "afxtempl.h"
#include "PrecisionCompensationPara.h"

#define CCD_MODEL_CCD1		0
#define CCD_MODEL_CCD2_CCD1	1
#define CCD_MODEL_CCD2		2

enum {LOCATE_MODEL_2_POINTS, LOCATE_MODEL_3_POINTS, LOCATE_MODEL_4_POINTS};

class CHLDoc : public CDocument
{
protected: // create from serialization only
	CHLDoc(); 
	DECLARE_DYNCREATE(CHLDoc)

// Attributes
public:
// Operations
public:
    inline CCommandProcessor& GetCommandProcessor() { return m_commandProcessor; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHLDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public: 
	CChain m_chain;			//所有绘图对象列表
	void AddRect(CQuad &quad,CWnd *pWnd);
	//设置或取出chain数据
	void SetChain(const CChain &chain );
	CChain & GetChain();

	//增加节点
	void AddNodeCmd(CNode *pNode, int nObjType = -1);
	void AddNode(CNode *pNode,    int nObjType = -1);

	//拉变
	void Drag(CDot &dot,double ratioX,double ratioY,double moveX,double moveY);
	void DragCmd(CDot &dot,double ratioX,double ratioY,double moveX,double moveY);
	
	//缩放
	void ScaleCmd(CDot dot,double ratioX,double ratioY);
	void Scale(CDot dot,double ratioX,double ratioY);

	//移动
	void MoveCmd(double moveX,double moveY);
	void Move(double moveX,double moveY);

	//旋转
	void RotateCmd(CDot dot,double angle );
	void Rotate(CDot dot,double angle);

	//删除
	void DeleteCmd();
	void Delete();

	//拷贝
	void CopyCmd();
	void Copy();

	//框选

	//右键选中

	//右键属性修改
	void DrawsetCmd();

	//成组
	void GroupCmd();

	//取消成组
	void UngroupCmd();

	//PASTE
	void PasteCmd();
	void FillCmd(FILLDATA fill);

	//阵列
	void ArrayCmd(CArray<OBJPOS, OBJPOS> &dtArray);

	CQuad GetTrackerQuad();
	void SetTrackerQuad(CQuad &quad,double angle = 0);
    CQuad GetAllLimit();
	CQuad GetLimit();
	void SetQuad(CQuad quad);
	void SetSelect(BOOL bFlag);

	BOOL IsSelectedObj();

	void SetGroupNum(UINT nChar);

	void Draw(XDC *pDC);
	int  GetStatus(){return m_nStatus;};

	void SetLayerColor(int index, COLORREF color);
	void OnLockLayer(int nLayer);
    void OnUnLockLayer(int nLayer);

	void ConvertSelToOneTree();
	void ComToOneTreeCmd();

	BOOL SelGroupByNum(UINT nChar, CWnd *pWnd);
	void SelGroupByNumCmd(int nNum, CWnd *pWnd);


	void SetGroupObjSel(CGroupObj *pGroupObj);

	BOOL isIntersect(CDot&p1 , CDot& p2 , CDot& p3 , CDot& p4 );

	void DelObjCmd(DWORD dwPara);
	void DelObj(CGroupObj *pGroupObj);

	void OnMAlignCmd(int nType);

	double GetSelTotalLength();

	void ClearTextMem();
	void SetProStep(int nStep);

	BOOL GetFillProp(FILLDATA &fillOld);
	void UpdateFill(FILLDATA &fillNew);

	void InitSelRowSort(CWnd* pWnd);

	virtual ~CHLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	void Serialize(FILE* fp,BOOL bFlag);
	
// Generated message map functions
protected:
	//{{AFX_MSG(CHLDoc)
	afx_msg void OnGroup();
	afx_msg void OnUnGroup();
	afx_msg void OnUpdateGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUnGroup(CCmdUI* pCmdUI);
	afx_msg void OnMarkNormal();
    afx_msg void OnMarkMul();
	afx_msg void OnUpdateMarkMul(CCmdUI* pCmdUI);
    afx_msg void OnRedPreview();
    afx_msg void OnUpdateRedPreview(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditSelectAll();
	afx_msg void OnDrawFill();
	afx_msg void OnChangeCenter();
	afx_msg void OnUpdateChangeCenter(CCmdUI* pCmdUI);
	afx_msg void OnChangeCcw();
	afx_msg void OnUpdateChangeCcw(CCmdUI* pCmdUI);
	afx_msg void OnChangeCw();
	afx_msg void OnUpdateChangeCw(CCmdUI* pCmdUI);
	afx_msg void OnChangeMirrorX();
	afx_msg void OnUpdateChangeMirrorX(CCmdUI* pCmdUI);
	afx_msg void OnChangeMirrorY();
	afx_msg void OnUpdateChangeMirrorY(CCmdUI* pCmdUI);
	afx_msg void OnMarkPreview();
	afx_msg void OnDrawSet();
	afx_msg void OnArray();
	afx_msg void OnUpdateArray(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnDrawBmp();
	afx_msg void OnSetLayer();
	afx_msg void OnDrawBartext();
	afx_msg void OnExportPlt();
	afx_msg void OnMarkSpc();
	afx_msg void OnUpdateMarkSpc(CCmdUI* pCmdUI);
    afx_msg void OnMarkFast();
	afx_msg void OnUpdateMarkFast(CCmdUI* pCmdUI);
	afx_msg void OnEditDBCopy();
	afx_msg void OnEditBDPaste();
	afx_msg void OnUpdateConvertOneTree(CCmdUI* pCmdUI);
    afx_msg void OnConvertOneTree();
	afx_msg void OnSaveAsSide1();
	afx_msg void OnUnion();
	afx_msg void OnUpdateUnion(CCmdUI* pCmdUI);
	afx_msg void OnUnunion();
	afx_msg void OnUpdateUnunion(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nStatus;			//当前选择状态
	CQuad m_quadPrevFram;   //预览边框
	BOOL  m_bEditCut;
	CCommandProcessor m_commandProcessor;	//命令处理器
	CLay m_lay;
	int  m_nProcessorPos;   // 进度条位置

	DOUBLEOUTPARA m_DBCopyPara;

public:
	MOTOR m_motor;			//打标结束后电机移动量
	void UpdateLay();
	int GetMarkUnit();
	BOOL PreprocessMark();
	void Mark(HWND hWnd,int repeat);
	void SetSelectFlag(BOOL *pFlag);
	void GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray);	//阵列时从屏幕取位置
	int GetMenuIndex();
	void SetCurLayer(int cur);
	int GetLaySize();

    CLayer GetCurLayer();
	CLayer GetLayer(int index=0);
	void   SetLayer(CLayer &layer);

	void SetLay(int mode,CLayer &ly);

	CLay & GetLay();
	void SetLay(const CLay &lay);
	void SetLayCmd(int mode,CLayer ly);

	int GetCur();
	CQuad GetQuadTracker(){return m_quadTracker;};

	void SetSelectedObjbeMarked(BOOL bFlag);

	void InserObjToList(int nObjType);

//	void GetQuadInTrackerQuad(CQuad &Quad);
	void SetObjSelInQuadList();

	void RedPreview(BOOL bIsRed = TRUE, BOOL bIsArray = FALSE);

public:	
	void MoveAccurate(double MoveX,double MoveY);
	void JoinPoint(CWnd *pWnd);
	void ConvertCmd(BOOL bPopMsg = TRUE);
	void SplitDot(CWnd *pWnd,double dbPoint);
	void ChangeQuadCmd(CPoint &point,CWnd *pWnd);
	void ReLoadStrokeList();
	void AddPoint(CPoint &point,CWnd *pWnd);
	BOOL HitOtherObject(CPoint &point,CWnd *pWnd);
	BOOL HaveSelectObject();
	void MultiSelect(CQuad &quad);
	void CaptureObjectCmd(CQuad &quad);
	BOOL SetStrokeList(StrokeList *pStrokeList,CPoint &point,CWnd *pWnd);
	void DelDot(CWnd *pWnd);
	void DotEdit(XTracker *pTrack,CPoint &point,CWnd *pWnd);
	void DelSelectTree(CTree *pTree);
	void SetCurMgrText();
	void AddNodeCmd(CTree *pTree);
	void AddTree(CTree *pAddTree);
	void GetSelectedStrokeList(STROKELIST *pStrokeList);
	BOOL GetBarcode();
	void UpdatePerUnit();
	void SetLayerCmd();
	BOOL OutOfLimit();
	void SelectDraw(CTree *pTree);
	void Changeconvert(BOOL bPopMsg = TRUE);
	void FilterPoint();
	void FilterPointCmd();

    void OnKillAngle();
	void KillAngleCmd();
	void OnExtendObj(BOOL bIsKillOldObj);
	void ExtendObjCmd(double dbSpace, BOOL bIsKillOldObj);
	void IniSort(CWnd* pWnd);
	void DrawMark(CDC* pDC, CWnd *pWnd);
	void SortAllObj();

	BOOL InitASort(CWnd* pWnd);
	void ExitASort();

	void SetObjProp();

	void InitGSort(CWnd* pWnd);
	void InitSelSort(CWnd* pWnd);
	
	double GetAngle(CDot &dtS, CDot &dtE);
	double Distance(CDot &sDot, CDot &eDot);

	void CreateNewRect(CRect &lRt, CRect &cRt, CRect &nRt);

	void ObjListLoad();
	void ObjListUnLoad();

	void MAlignCmd(int nType);

	BOOL GetLockFlg(int index);
	BOOL IsHaveTextOrCodeBar();

	void UpdateObjMarkSet(OBJMARKSET &ObjMarkSet);
	BOOL GetObjMarkSet(OBJMARKSET &ObjMarkSet);

	CNode* GetSelNode(FILLDATA &fill);

	void UpdateSelText(FILLDATA &fill);
	BOOL IsHaveSelTxetNode();

	CNode* IsHaveSelBarNode();

	void EditTextNode(CNode* pNode);
	void EditBarNode(CNode* pNode);

	void UpdateSelBar();

	BOOL CheckSetTrack(void);

public:
	CQuad m_quadTracker;	//当前选中的对象的范围大小（逻辑坐标）
	XTracker *m_pTracker;
	int m_nPosType;
	CArray<SORTNODE,SORTNODE>  m_Sort;
    CArray<ASNODE,ASNODE>      m_ASort;
	CArray<GNODE,GNODE>        m_GSort;
    CArray<SELQUAD ,SELQUAD>   m_QuadList;
	CSortListObj m_SortListObj;
	CAutoSortListObj m_ASortListObj;

	BOOL m_bExternOpt;
	afx_msg void OnZTest();
	void ModifyZ();

	void JoinStrokeCmd(BOOL bPopMsg = TRUE);

	//Wafer
public:
	//定位Mark点前所需要的输入型变量
	//除了下面列出的，还有g_ParaIni.m_dbWorkbenchFixtureAngle;
	double m_dbLPDrawX[4];
	double m_dbLPDrawY[4];

	int	   m_nRefPointNum;
	double m_arrDocRefPointX[100];	
	double m_arrDocRefPointY[100];

	double m_dbProductCCWAngle;

	///////////////////////////////////////////////////////////////////////////////
	//LP:LocatePoint, left, right, top, bottom
	double m_dbLPCCDTrueX[4];			//LP在CCD中心时，实际工作台位置
	double m_dbLPCCDTrueY[4];

	double	m_dbHandOffsetX;		//手动补偿值
	double	m_dbHandOffsetY;
	double	m_dbHandOffsetCCWAngle;

	double	m_dbDisMaxErr;
	double	m_dbAngleMaxErr;

	//Scale
	double	m_dbXAutoScaleAdd;
	double	m_dbYAutoScaleAdd;
	BOOL	m_bEnableXFixSacle;
	BOOL	m_bEnableYFixScale;
	double	m_dbFixXScale;
	double	m_dbFixYScale;

	int m_nWaferSize;

	/////////////////////////////////////////////////////////////////////////////
	//CCD
	CString	m_strProgID1;
	CString	m_strProgID2;
	CString	m_strProgID3;
	CString	m_strProgNewID2;
	CString	m_strProgNewID3;
	CString	m_strProgNewID4;

	int		m_nLight1;
	int		m_nLight2;
	int		m_nLight3;
	int		m_nLight4;
	int		m_nLocateModel;
	int		m_nCCDUseModel;


	//焦点
//	BOOL	m_bFocusMark;
	BOOL	m_bEnableMarkFocus;
	double	m_dbZCCDFocus;
//	double	m_dbZMarkFocus;


	CArray<CMarkFocus> m_arrMarkFocus;

	
	//真空
	int		m_nGasModel;

	//////////////////////////////////////////////////////////////////////////

	//CCD自动校正信息
	double	m_dbCrossInCCDX;
	double	m_dbCrossInCCDY;
	BOOL	m_bMarkCross;
	BOOL	m_bMarkCrossResult;	

	//扩束镜
	double m_dbBeamExpanderRatio;
	int m_nBeamExpanderOffset;

	//阵列信息
	double	m_dbRowDist;
	double	m_dbColDist;
	double	m_dbShrinkRow;
	double m_dbShrinkYCol;

	//工艺
	double m_dbTestPowerFrequency;
	double m_dbProductPowerMax;
	double m_dbProductPowerMin;

	//切分信息
	CDivideLayer m_DivideLayer;

	//扫码
	BOOL m_bScan;
	double m_dbScanZpos;
	double m_dbScanPtX;
	double m_dbScanPtY;
	int m_ScanPriNum;
	
public:
	void CustomSerialize(FILE* fp,BOOL bFlag);
	void CopyObjectListSerialize(FILE* fp,BOOL bFlag);

	void ResetDivideFlag();
	void GetStrokeInQuad(CQuad &quad, StrokeList &strokeList, BOOL bDivedeByStroke);


	void CustomMark();
	void GuardCustomMark();
	BOOL CheckGraphicAndWorkbenchLimit(double dbRefPointX, double dbRefPointY, CString &strInfo);
	void DivideMark(HWND hWnd, const CPrecisionCompensationPara &para);
	void DivideByCell(HWND hWnd, const CPrecisionCompensationPara &para, BOOL bDivedeByStroke);
	void DivideOneByOne(HWND hWnd, const CPrecisionCompensationPara &para);
	BOOL MarkDivideCell(HWND hWnd, CStrokeList &strokeList);
	void FixedDivideMarkPoint(HWND hWnd, int repeat);


	double GetFirstMarkFocus();

	void CalculateLPLogicPos(int nPointIndex, int nCCDUseModel, double& dbLogicX, double& dbLogicY);
	void CalculateLPCCDInitPos(int nPointIndex, int nCCDUseModel, double& dbCCDInitX, double& dbCCDInitY);

	int GetLocatePointsNum();
	BOOL GetCCDCapture(HWND hWnd, int nLocateModel); 
	BOOL PointPrecisionCapture(HWND hWnd, int nPointIndex);
	BOOL PointCCD1Capture(double &dbCaptureX, double &dbCaptureY,BOOL bOffset = FALSE);
	BOOL NoTicpsPointCCD1Capture(double &dbCaptureX, double &dbCaptureY,BOOL bOffset = FALSE);
	BOOL PointCCD2Capture(double &dbCaptureX, double &dbCaptureY);
	BOOL PointCCD2AssistCapture(double &dbCaptureX, double &dbCaptureY);
	
	void CalCompensationPara(CPrecisionCompensationPara &para, BOOL bCCD);
	void CalZoom(double &dbScaleX, double &dbScaleY);
	void CalOffset(CDot &dotBase, double &dbOffsetX, double &dbOffsetY, double &dbOffsetCCW);
	void CalculateCenterOf4Points(CDot dots[4], CDot &dotCenter);	

	afx_msg void OnSetProduct();
	afx_msg void OnSaveAsReverse();
	afx_msg void OnSetLeftMark();
	afx_msg void OnSetRightMark();
	afx_msg void OnSetTopMark();
	afx_msg void OnSetBottomMark();
	afx_msg void OnAutoOffset();
	afx_msg void OnFocusTypeSet();
	BOOL ScanMatri(BOOL bNet);
	afx_msg void OnCheckPower();
	afx_msg void OnNetSet();
	BOOL NetSendRecvOpenDoc();
	BOOL NetSendRecvAfterData();

	CArray<CString, CString&> m_strNetRecv;
	CArray<CString, CString&> m_strNetLastRecv;

	afx_msg void OnGroupnum();
	afx_msg void OnGroupSet();
	afx_msg void OnLdfiledata();
	CString m_strFileDataPath;
	BOOL LoadFileDataChange(CString& strPath);
	BOOL m_bUseFile;
	int m_dMarkLay;
	afx_msg void OnArrayRename();
	void NumAdd(CString &strNum);
	
	afx_msg void OnFoucesTest();

	void Setlight(int nLight1,int nLight2,int nLight3,int nLight4);
	afx_msg void OnSetBarcodeLen();
	BOOL isDeviationOK(double dbNewMarkToCCD1X,double dbNewMarkToCCD1Y,BOOL bTicps = TRUE);
	BOOL MoveMarkPos(void);
	BOOL AfterMarkMove(void);
	afx_msg void OnAutooffsetex();
	void SetStrokeLayFreq();
	afx_msg void OnWaferArray();
	afx_msg void OnReplaceText();
	CStringList m_listMarkText;
	void ProcessString(CString& str); 
	double m_dbScaleLimit;
	afx_msg void OnUpdateJointstrokeCurdoc(CCmdUI *pCmdUI);
	afx_msg void OnSetAsNoCutLay();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HLDOC_H__B8BC184F_DE24_11D3_845D_444553540001__INCLUDED_)
