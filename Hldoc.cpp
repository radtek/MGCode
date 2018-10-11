// HLDoc.cpp : implementation of the CHLDoc class
//
#include "stdafx.h"
#include "HL.h"
#include "resource.h"
#include <afxole.h>

#include "HLDoc.h"
#include "MainFrm.h"

#include "Node.h"
#include "NodeDib.h"
#include "NodeCodeBar.h"
#include "NodeText.h"

#include "DrawFillDlg.h"
#include "MarkNormalDlg.h"
#include "MarkPreviewDlg.h"
#include "ArrayDlg.h"
#include "DibDlg.h"
#include "KBChar.h"

#include "CmdStoreDoc.h"
#include "CmdMove.h"
#include "CmdAddNode.h"
#include "CmdScale.h"
#include "CmdDelete.h"
#include "CmdRotate.h"
#include "CmdCopy.h"
#include "CmdGroup.h"
#include "CmdUngroup.h"
#include "CmdPaste.h"
#include "CmdFill.h"
#include "CmdSetLay.h"
#include "CmdDrawSet.h"
#include "CmdArray.h"
#include "Cmdchanglay.h"
#include "CmdNewDrag.h"
#include "SetLayer.h"
#include "HLView.h"
#include "CmdDotEdit.h"
#include "CmdDelDot.h"
#include "CmdMultiSelect.h"
#include "CmdHitOtherObj.h"
#include "CmdConvert.h"
#include "SplitPointCmd.h"
#include "JoinPointCmd.h"
#include "MoveAccCmd.h"
//#include "LocNukeForHL.h"
#include "StringIDTable.h"
#include "DlgExportPlt.h"
#include "CmdAddPoint.h"
#include "CmdFilterDocPoint.h"
#include "CmdExtendObj.h"
#include "CmdKillAngle.h"
#include "CmdConvertSelToOneTree.h"
#include "CmdSelGroupByNum.h"
#include "CmdDelObj.h"
#include "CmdMAlign.h"
#include "MarkSpecialDlg.h"
#include "SelSort.h"

#include "DlgZTest.h"

#include "CCDCalibrate.h"
#include "DlgSetWafer.h"
#include "HlParaIni.h"
#include <math.h>
#include "WBGridCali.h"

#include "DivideLayer.h"
#include "DlgAutoOffset.h"
#include "DlgAutoOffsetEx.h"
#include "DlgFocusTypeSet.h"
#include "DlgCheckPower.h"
#include "NetSet.h"
#include "FileLoadData.h"

#include "GroupNum.h"
#include "GroupList.h"

#include "DlgArrayRename.h"

#include "DlgTestFocus.h"
#include "BarcodeSetLen.h"

#include "DlgSetWaferArray.h"
#include "DlgReplaceText.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_OpenFileName; //GenImportFullName()中取不到Document时用
CString g_OldFileName;
BOOL g_bClipBoardCopyNode=FALSE;

extern CHLApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CHLDoc
IMPLEMENT_DYNCREATE(CHLDoc, CDocument)

BEGIN_MESSAGE_MAP(CHLDoc, CDocument)
	//{{AFX_MSG_MAP(CHLDoc)
	ON_COMMAND(ID_GROUP, OnGroup)
	ON_COMMAND(ID_UN_GROUP, OnUnGroup)
	ON_UPDATE_COMMAND_UI(ID_GROUP, OnUpdateGroup)
	ON_UPDATE_COMMAND_UI(ID_UN_GROUP, OnUpdateUnGroup)
//	ON_COMMAND(ID_MARK_NORMAL, OnMarkNormal)
//	ON_COMMAND(ID_MARK_MUL, OnMarkMul) //
	ON_UPDATE_COMMAND_UI(ID_MARK_MUL, OnUpdateMarkMul)
    ON_COMMAND(ID_RED_PREVIEW, OnRedPreview)
	ON_UPDATE_COMMAND_UI(ID_RED_PREVIEW, OnUpdateRedPreview)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_DRAW_FILL, OnDrawFill)
	ON_COMMAND(ID_CHANGE_CENTER, OnChangeCenter)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_CENTER, OnUpdateChangeCenter)
	ON_COMMAND(ID_CHANGE_CCW, OnChangeCcw)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_CCW, OnUpdateChangeCcw)
	ON_COMMAND(ID_CHANGE_CW, OnChangeCw)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_CW, OnUpdateChangeCw)
	ON_COMMAND(ID_CHANGE_MIRROR_X, OnChangeMirrorX)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_MIRROR_X, OnUpdateChangeMirrorX)
	ON_COMMAND(ID_CHANGE_MIRROR_Y, OnChangeMirrorY)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_MIRROR_Y, OnUpdateChangeMirrorY)
	ON_COMMAND(ID_MARK_PREVIEW, OnMarkPreview)
	ON_COMMAND(ID_DRAW_SET, OnDrawSet)
	ON_COMMAND(ID_ARRAY, OnArray)
	ON_UPDATE_COMMAND_UI(ID_ARRAY, OnUpdateArray)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_COMMAND(ID_DRAW_BMP, OnDrawBmp)
	ON_COMMAND(ID_SET_LAYER, OnSetLayer)
	ON_COMMAND(ID_DRAW_BARTEXT, OnDrawBartext)
	ON_COMMAND(IDM_EXPORT_PLT, OnExportPlt)
	ON_COMMAND(ID_MARK_SPC, OnMarkSpc)
	ON_UPDATE_COMMAND_UI(ID_MARK_SPC, OnUpdateMarkSpc)
    ON_COMMAND(ID_FAST_MARK, OnMarkFast)
	ON_UPDATE_COMMAND_UI(ID_FAST_MARK, OnUpdateMarkFast)
	ON_COMMAND(ID_EDIT_DB_COPY, OnEditDBCopy)
	ON_COMMAND(ID_EDIT_DB_PASTE, OnEditBDPaste)
	ON_COMMAND(IDD_ADDONETREE, OnConvertOneTree)
	ON_UPDATE_COMMAND_UI(IDD_ADDONETREE, OnUpdateConvertOneTree)
	ON_COMMAND(ID_UNION, OnUnion)
	ON_UPDATE_COMMAND_UI(ID_UNION, OnUpdateUnion)
	ON_COMMAND(ID_UNUNION, OnUnunion)
	ON_UPDATE_COMMAND_UI(ID_UNUNION, OnUpdateUnunion)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_Z_TEST, &CHLDoc::OnZTest)
	ON_COMMAND(ID_SET_PRODUCT, &CHLDoc::OnSetProduct)
	ON_COMMAND(ID_SAVE_AS_REVERSE, &CHLDoc::OnSaveAsReverse)
	ON_COMMAND(ID_SET_LEFT_MARK, &CHLDoc::OnSetLeftMark)
	ON_COMMAND(ID_SET_RIGHT_MARK, &CHLDoc::OnSetRightMark)
	ON_COMMAND(ID_SET_TOP_MARK, &CHLDoc::OnSetTopMark)
	ON_COMMAND(ID_SET_BOTTOM_MARK, &CHLDoc::OnSetBottomMark)
	ON_COMMAND(ID_AUTO_OFFSET, &CHLDoc::OnAutoOffset)
	ON_COMMAND(ID_FOCUS_TYPE_SET, &CHLDoc::OnFocusTypeSet)
	ON_COMMAND(ID_CHECK_POWER, &CHLDoc::OnCheckPower)
ON_COMMAND(ID_NET_SET, &CHLDoc::OnNetSet)
ON_COMMAND(ID_GROUPNUM, &CHLDoc::OnGroupnum)
ON_COMMAND(ID_GROUP_SET, &CHLDoc::OnGroupSet)
ON_COMMAND(ID_LDFILEDATA, &CHLDoc::OnLdfiledata)
ON_COMMAND(ID_ARRAY_RENAME, &CHLDoc::OnArrayRename)
ON_COMMAND(ID_FOUCES_TEST, &CHLDoc::OnFoucesTest)
ON_COMMAND(ID_SET_BARCODE_LEN, &CHLDoc::OnSetBarcodeLen)
ON_COMMAND(ID_AUTOOFFSETEX, &CHLDoc::OnAutooffsetex)
ON_COMMAND(ID_WAFER_ARRAY, &CHLDoc::OnWaferArray)
ON_COMMAND(ID_REPLACE_TEXT, &CHLDoc::OnReplaceText)
ON_UPDATE_COMMAND_UI(ID_JOINTSTROKE_CURDOC, &CHLDoc::OnUpdateJointstrokeCurdoc)
ON_COMMAND(ID_SET_AS_NO_CUT_LAY, &CHLDoc::OnSetAsNoCutLay)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHLDoc construction/destruction

CHLDoc::CHLDoc()
	: m_strFileDataPath(_T(""))
	, m_bUseFile(FALSE)
	,m_dMarkLay(1)
	, m_dbScanPtX(0)
	, m_dbScanPtY(0)
	, m_dbScanZpos(0)
	, m_ScanPriNum(-1)
	, m_bScan(FALSE)
	, m_dbScaleLimit(0.2)
{
	int undoTime = g_SysCfg.m_nUndoCount;
	m_commandProcessor.SetMaxCommands(undoTime);
	m_nStatus=CChain::selectNull;
	m_quadTracker.SetEmpty();

	m_motor.nDelay = 0;
	m_motor.nMove = 0;
	m_motor.nDirection = 0;
	m_nProcessorPos = 0;

	m_bEditCut = FALSE;
	m_nPosType = 4;

	m_pTracker = NULL;

	m_bExternOpt = TRUE;

	m_Sort.RemoveAll();
    m_ASort.RemoveAll();
	m_GSort.RemoveAll();
	m_QuadList.RemoveAll();

	memset(&m_DBCopyPara, 0, sizeof(DOUBLEOUTPARA));

	m_QuadList.SetSize(100000, 1000);

	//custom

	m_strProgID1="-1";
	m_strProgID2="-1";
	m_strProgID3="-1";

	m_strProgNewID2 = "-1";
	m_strProgNewID3 = "-1";
	m_strProgNewID4 = "-1";

	m_nLight1=220;
	m_nLight2=220;
	m_nLight3=220;
	m_nLight4=220;

	m_dbDisMaxErr=2;
	m_dbAngleMaxErr=10;
	m_dbHandOffsetX=0;
	m_dbHandOffsetY=0;
	m_dbHandOffsetCCWAngle=0;
	m_dbXAutoScaleAdd=0;
	m_dbYAutoScaleAdd=0;
	m_bEnableXFixSacle=0;
	m_bEnableYFixScale=0;
	m_dbFixXScale=1;
	m_dbFixYScale=1;
	m_dbLPDrawX[0]=0.0;
	m_dbLPDrawY[0]=0.0;
	m_dbLPDrawX[1]=0.0;
	m_dbLPDrawY[1]=0.0;
	m_dbLPDrawX[2]=0.0;
	m_dbLPDrawY[2]=0.0;
	m_dbLPDrawX[3]=0.0;
	m_dbLPDrawY[3]=0.0;
	m_nLocateModel = LOCATE_MODEL_2_POINTS;
	m_nCCDUseModel=0;
	m_dbRowDist = 0;
	m_dbColDist = 0;
	m_dbShrinkRow = 0;
	m_dbShrinkYCol = 0;

	m_dbProductCCWAngle = 0;

	m_dbBeamExpanderRatio = 1.0;
	m_nBeamExpanderOffset = 0;

//	m_bFocusMark = 0;
	m_bEnableMarkFocus = 0;
	m_dbZCCDFocus = 0;
//	m_dbZMarkFocus = 0;	

	m_dbTestPowerFrequency = -1;
	m_dbProductPowerMin = 0;
	m_dbProductPowerMax = 10000;

	m_nGasModel = 0;

	m_nRefPointNum = 0;
	ZeroMemory(m_arrDocRefPointX, sizeof(m_arrDocRefPointX));
	ZeroMemory(m_arrDocRefPointY, sizeof(m_arrDocRefPointY));

	CMarkFocus focus;	
	ZeroMemory(&focus, sizeof(CMarkFocus));
	m_arrMarkFocus.Add(focus);
	m_nWaferSize = 300;
}

CHLDoc::~CHLDoc()
{
	m_QuadList.RemoveAll();
}

//BOOL CHLDoc::OnCloseDocument()
//{
//}

BOOL CHLDoc::OnNewDocument()
{
	//打标控制参数设置缺省值
	for(int i = 0; i < g_SysCfg.m_nLayerCnt; i++)
	{
		CLayer ly;
		ly.m_nLayer=i;
		switch(i)
		{
		case 0:
			ly.m_color=BLACK;
			break;
		case 1:
			ly.m_color=GRAY;
			break;
		case 2:
			ly.m_color=GREEN;
			break;
		case 3:
			ly.m_color=YANGREEN;
			break;
		case 4:
			ly.m_color=YELLOW;
			break;
		case 5:
			ly.m_color=BLUE;
			break;
		case 6:
			ly.m_color=MAGENTA;
			ly.m_bDefault = FALSE;
			ly.m_para.dbCount = 0;
			break;
		case 7:
			ly.m_color=RED;
			break;
		case 8:
			ly.m_color=BROWN;
			break;
		case 9:
			ly.m_color=PINK;
			break;
         default:
			ly.m_color=BLACK;
			break;
		}
		m_lay.Add(ly);
	}
	m_lay.UpdateList();
	m_lay.SetCur(0);

	CMainFrame *pFWnd = (CMainFrame*)AfxGetMainWnd();
	if(NULL != pFWnd)
	{
		pFWnd->m_XBarMarkPara.m_PropDlg.OnNewDocument(m_lay);
		pFWnd->m_XBarProperty.m_PSDlg.OnNewDocument();
		pFWnd->m_XBarProperty.m_FillEditDlg.m_TextEditDlg.OnNewDocment();
	}

	CArray<SELQUAD,SELQUAD> quadList;
	quadList.RemoveAll();
	theApp.m_RedPreView.SetQuadArray(quadList);

	g_IO.OUTPUT_SOFT_OK(TRUE);

	return CDocument::OnNewDocument();
}


void CHLDoc::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize sa(fp);

	CString str;
	if (bFlag)
	{
		//保存文件
		sa<<"HAN'S SOFTWARE FOR PCB V1";
		m_chain.Serialize(fp,bFlag);
		m_lay.Serialize(fp,bFlag);
		m_DivideLayer.Serialize(fp, bFlag);
		
		CustomSerialize(fp,bFlag);

		CopyObjectListSerialize(fp,bFlag);

		sa<<"END FILE";
		SetModifiedFlag(FALSE);
	}
	else
	{
		g_arrMulDoc.RemoveAll();

		sa>>str;
		if (str.Left(14)=="HAN'S SOFTWARE")
		{
			while ((sa>>str))
			{
				if(str == "END FILE")
					break;
				else if(str == "D_B")
				{
					m_chain.Serialize(fp,bFlag);

					m_nProcessorPos += 20;
					SetProStep(m_nProcessorPos);
				}
				else if(str == "L_B")
				{
					m_lay.Serialize(fp,bFlag);

					m_nProcessorPos += 30;
					SetProStep(m_nProcessorPos);
				}
				else if (str=="DIVIDE_B")
				{
					m_DivideLayer.Serialize(fp, bFlag);
				}
				else if (str=="Custom_B")
				{
					CustomSerialize(fp,bFlag);
				}
				else if (str=="CopyObjectList_B")
				{
					CopyObjectListSerialize(fp,bFlag);
				}
			}
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CHLDoc diagnostics

#ifdef _DEBUG
void CHLDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHLDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG 

/////////////////////////////////////////////////////////////////////////////
// CHLDoc commands

void CHLDoc::AddNodeCmd(CNode *pNode, int nObjType)
{
	CTree *pTree=new CTree(pNode);
	if (NULL == pTree)
		return;
	
	CGroupObj *pGroupObj = new CGroupObj();
	if( pGroupObj == NULL )
		return;

	pGroupObj->Add(pTree);
	m_chain.Add(pGroupObj);

	SetModifiedFlag();
	m_chain.SetSelect(FALSE);//其它为没选择
	pTree->SetSelect(TRUE);  //新的为选择

	pGroupObj->m_bSelect = TRUE;

	CQuad quad = pTree->CalLimit();
	
	SetTrackerQuad(quad);
	
	SelectDraw(pTree);


	return;
}

void CHLDoc::AddNodeCmd(CTree *pTree)
{
	if( pTree == NULL )
		return;

	CGroupObj *pGroupObj = new CGroupObj();
	if( pGroupObj == NULL )
		return;

	pGroupObj->Add(pTree);
	m_chain.Add(pGroupObj);

	m_chain.SetSelect(FALSE);
	pTree->SetSelect(TRUE);
	pGroupObj->m_bSelect = TRUE;
	
	
	SetModifiedFlag();
	CQuad quad=pTree->CalLimit();
	SetTrackerQuad(quad);
	SelectDraw(pTree);
}

void CHLDoc::AddNode(CNode *pNode, int nObjType)
{
	if (!(g_CurUserInfo.dwRight1 & 8))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	} 

	m_commandProcessor.Submit(new CCmdAddNode(this,pNode));

	SetObjProp();

	return;
}

void CHLDoc::InserObjToList(int nObjType)
{
	/*
	CMainFrame *pMainFram = (CMainFrame*)AfxGetMainWnd();
	switch(nObjType) 
	{
	case ENU_OBJ_POLYLINE:
		pMainFram->m_ObjectListDlg.AddClmItem("曲线", 0);
		break;
	case ENU_OBJ_RECTANGLE:
		pMainFram->m_ObjectListDlg.AddClmItem("矩形", 1);
		break;
	case ENU_OBJ_ELLIPSE:
		pMainFram->m_ObjectListDlg.AddClmItem("圆", 2);
		break;
	case ENU_OBJ_POLYGON:
		pMainFram->m_ObjectListDlg.AddClmItem("多边形", 3);
		break;
	case ENU_OBJ_GRID:
		pMainFram->m_ObjectListDlg.AddClmItem("网格", 4);
		break;
	case ENU_OBJ_CODEBAR:
		pMainFram->m_ObjectListDlg.AddClmItem("条码", 5);
		break;
	case ENU_OBJ_TEXT:
		pMainFram->m_ObjectListDlg.AddClmItem("文本", 6);
		break;
	case ENU_OBJ_PLT:
		pMainFram->m_ObjectListDlg.AddClmItem("PLT", 7);
		break;
	case ENU_OBJ_BMP:
		pMainFram->m_ObjectListDlg.AddClmItem("位图", 8);
		break;
	case ENU_OBJ_DXF:
		pMainFram->m_ObjectListDlg.AddClmItem("DXF", 9);
		break;
	default:
		break;
	}
*/
	return;
}

void CHLDoc::Draw(XDC *pDC)
{
	m_chain.Draw(pDC);
}

BOOL CHLDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!(g_CurUserInfo.dwRight1 & 256))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return FALSE;
	}

	m_nProcessorPos = 0;
	SetProStep(m_nProcessorPos);
	
	//采用自制的序列化流程
	FILE *fp;
	g_OpenFileName=CString(lpszPathName);
	if( (fp = fopen(lpszPathName,"rb")) != NULL )
	{
		Serialize(fp,FALSE);
		fclose( fp );
	}


	int nLayerCount = m_lay.GetSize();

	//打标控制参数设置缺省值
	for(int i = nLayerCount; i < g_SysCfg.m_nLayerCnt; i++)
	{
		CLayer ly;
		ly.m_nLayer=i;
		switch(i)
		{
		case 0:
			ly.m_color=GRAY;
			break;
		case 1:
			ly.m_color=RED;
			break;
		case 2:
			ly.m_color=YELLOW;
			break;
		case 3:
			ly.m_color=GREEN;
			break;
		case 4:
			ly.m_color=YANGREEN;
			break;
		case 5:
			ly.m_color=BLUE;
			break;
		case 6:
			ly.m_color=MAGENTA;
			break;
		case 7:
			ly.m_color=BLACK;
			break;
         default:
			ly.m_color=BLACK;
			break;
		}
		m_lay.Add(ly);
	}
	m_lay.UpdateList();
	m_lay.SetCur(0);

	m_nProcessorPos += 10;
	SetProStep(m_nProcessorPos);

	CMainFrame *pFWnd = (CMainFrame*)AfxGetMainWnd();
	if(NULL != pFWnd)
	{
		pFWnd->m_XBarMarkPara.m_PropDlg.OnNewDocument(m_lay);
		m_nProcessorPos += 20;
	    SetProStep(m_nProcessorPos);
		pFWnd->m_XBarProperty.m_PSDlg.OnNewDocument();
		m_nProcessorPos += 20;
	    SetProStep(m_nProcessorPos);
	}

	//层更新列表
	if (NULL != g_MgrText) 
	{
		UINT nID = g_MgrText->GetLastID();
		g_MgrText->SetNextID(nID);
	}
	
 	m_lay.UpdateList();
	UpdateAllViews(NULL);

	SetProStep(100);

	SetProStep(0);

	g_IO.OUTPUT_SOFT_OK(TRUE);
	
	return TRUE;
}

void CHLDoc::DeleteCmd()
{
	CQuad quad;
	m_chain.CalLimit(CDot(0,0),TRUE);
	m_quadTracker = m_chain.GetLimit();
	m_chain.Delete();
	SetModifiedFlag();

	SetTrackerQuad(m_quadTracker);
	quad.SetEmpty();
	SetTrackerQuad(quad);
	UpdateAllViews(NULL);

	return;
}

void CHLDoc::Delete()
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	m_commandProcessor.Submit(new CCmdDelete(this));
	if (NULL != m_pTracker)
	{
		m_pTracker->ClearAll();
	    ReLoadStrokeList();
	}
	else
	{
		m_pTracker = &(pView->m_track);
		m_pTracker->ClearAll();
	    ReLoadStrokeList();
	}

	pView->DispSelObjProp();

//	UpdateAllViews(NULL);

	return;
}

void CHLDoc::GroupCmd() 
{
	m_chain.Group();
	SetModifiedFlag();
	ReLoadStrokeList();
	SetTrackerQuad(m_chain.GetLimit());
	m_chain.RefreshSelectQuadList(m_QuadList);
}

void CHLDoc::OnGroup() 
{
	m_commandProcessor.Submit(new CCmdGroup(this));
}

void CHLDoc::UngroupCmd() 
{
	m_chain.UnGroup();
	SetModifiedFlag();
	ReLoadStrokeList();
	SetTrackerQuad(m_chain.GetLimit());
	m_chain.RefreshSelectQuadList(m_QuadList);

}

void CHLDoc::OnUnGroup() 
{
	m_commandProcessor.Submit(new CCmdUngroup(this));
}

void CHLDoc::OnUpdateGroup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nStatus&CChain::selectMul)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}

	return;
}

void CHLDoc::OnUpdateUnGroup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nStatus&CChain::selectGroup)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);	
	}

	return;
}

BOOL CHLDoc::OutOfLimit()
{
	CQuad qd;
	qd=m_chain.GetMarkRange();

	double x,y;
	g_DAT.GetMarkRange(x,y);
	double BoxX=x;
	double BoxY=y;	
	double x1,y1,x2,y2;
	x1 = (CENTER-BoxX/2);
	x2 = (CENTER+BoxX/2);
	y1 = (CENTER-BoxY/2);
	y2 = (CENTER+BoxY/2);
	if(qd.left<x1 || qd.bottom<y1 || qd.right>x2 || qd.top>y2)
	{
		return FALSE;
	}
	
	return TRUE;
}


void CHLDoc::OnRedPreview() 
{
	RedPreview();

	return;
}

void CHLDoc::RedPreview(BOOL bIsRed, BOOL bIsArray) 
{
	theApp.m_SelMark.StopSelMark();
	if (!g_bIsRedPreview) 
	{
		g_bIsRedPreview = TRUE;
		g_DAT.SetMarkPara(m_lay);
		theApp.m_RedPreView.StartPreview(bIsRed, bIsArray);
	}
	else
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	return;
}

void CHLDoc::OnUpdateRedPreview(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(FALSE);
	pCmdUI->SetCheck(g_bIsRedPreview);
	return;
}

void CHLDoc::OnMarkNormal() 
{
	if (!(g_CurUserInfo.dwRight1 & 2))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
//	if(!OutOfLimit())
//	{
//		AfxMessageBox(IDS_ERROT_OUTAREA);
//		return;
//	}

	//打标参数更新
	g_DAT.SetMarkPara(m_lay);

	//没有打标内容
	if(m_chain.GetCount() == 0)
		return;

	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview(); 
//		AfxGetMainWnd()->SendMessage(WM_COMMAND,(ID_RED_PREVIEW&(CN_UPDATE_COMMAND_UI)<<16),NULL);
//		theApp.GetMainWnd()->UpdateWindow();
//		UpdateAllViews();
		return;
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
		return;
	}
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
		return;
	}

	CMarkNormalDlg dlg;
	dlg.m_pDoc=this;
	dlg.m_pChain=(CChain *)&m_chain;

	if(g_DAT.EnterMarkEnv())
	{
		//硬件部分准备就绪时才能进入此操作
		dlg.DoModal();
		g_DAT.LeaveMarkEnv();
	}
	
	UpdateAllViews(NULL);
	SetTrackerQuad(m_chain.GetLimit());

	return;
}

void CHLDoc::OnMarkMul() 
{
	if (!(g_CurUserInfo.dwRight1 & 4))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}


	CHLApp *pApp=(CHLApp *)AfxGetApp();

	pApp->OnMarkMul();
	
}

void CHLDoc::OnUpdateMarkMul(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(FALSE);
}

BOOL CHLDoc::IsSelectedObj()
{
	return (!(m_nStatus==CChain::selectNull));
}

void CHLDoc::SetTrackerQuad(CQuad &quad,double angle)
{	
	m_quadTracker=quad;
	m_quadTracker.Normal();

	CQuad Quad;
//	Quad = m_chain.GetReFurbishQuad();

	Quad = quad;
	Quad.top    += 5;
	Quad.bottom -= 5;
	Quad.left   -= 5;
	Quad.right  += 5;

	//更新所有的视
	UpdateAllViews(NULL,UPDATE_TRACKER_VIEW);
	UpdateAllViews(NULL,UPDATE_LOGIC,(CObject *)(&Quad));
	//得到当前的选择状态
	m_nStatus=m_chain.GetStatus();
	
	return;
}

CQuad CHLDoc::GetTrackerQuad()
{
	m_quadTracker = m_chain.m_quadLimit;//此处获取m_chain中的选中框（此为实时性的选中框）	周欢  2018.7.5
	m_quadTracker.Normal();
	return m_quadTracker;
}

//void CHLDoc::GetQuadInTrackerQuad(CQuad &Quad)
//{
//	m_chain.GetQuadListInQuad(m_QuadList, Quad);
//}

void CHLDoc::SetObjSelInQuadList()
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	m_pTracker->ClearStrokeList();

	int nCount = m_QuadList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		SELQUAD SQD = m_QuadList.GetAt(i);
		if (TRUE == SQD.m_bSel) 
		{
			CGroupObj *pGroupObj = (CGroupObj*)(m_QuadList[i].m_pGroupObj);
			pGroupObj->SetSelect(TRUE);
			pGroupObj->SetStrokeList(&m_pTracker->m_StrokeList);
		}
	}

	SetTrackerQuad(GetLimit());
//	UpdateAllViews(NULL);

	return;
}

void CHLDoc::MoveCmd(double moveX,double moveY)
{
	m_chain.Move(moveX,moveY);
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
	m_chain.RefreshSelectQuadList(m_QuadList);
	return;
}

void CHLDoc::Move(double moveX, double moveY)
{
	m_commandProcessor.Submit(new CCmdMove(this,moveX,moveY));
}

void CHLDoc::ScaleCmd(CDot dot, double ratioX, double ratioY)
{
	m_chain.Scale(dot,ratioX,ratioY);
	SetModifiedFlag();
	
	SetTrackerQuad(m_chain.GetLimit());
	UpdateAllViews(NULL);

	return;
}

void CHLDoc::Scale(CDot dot, double ratioX, double ratioY)
{
	m_commandProcessor.Submit(new CCmdScale(this,dot,ratioX,ratioY));
}

void CHLDoc::RotateCmd(CDot dot, double angle )
{
	CQuad quad;
	CPoint pointtemp(0,13);//zg add
  	quad=m_chain.GetLimit();
	m_chain.Rotate(dot,angle);
	SetModifiedFlag();
	SetTrackerQuad(GetLimit());
	m_chain.RefreshSelectQuadList(m_QuadList);
	UpdateAllViews(NULL);

	return;
}

void CHLDoc::Rotate(CDot dot, double angle )
{
	m_commandProcessor.Submit(new CCmdRotate(this,dot,angle ));
}

void CHLDoc::Copy()
{
	m_commandProcessor.Submit(new CCmdCopy(this));
}

void CHLDoc::CopyCmd()
{
	m_chain.Copy();
	m_chain.RefreshSelectQuadList(m_QuadList);
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
	
	return;
}

void CHLDoc::OnEditCopy() 
{
	FILE *fp;
	if( (fp = fopen("C:\\TMPDATA.DAT","wt")) != NULL )
	{
		g_MgrText = &m_chain.m_MgrText;
		g_MgrText->SetUse(FALSE);
		m_chain.PreprocessMark(0x03);
		g_MgrText->Clear();
		g_bClipBoardCopyNode=TRUE;
		m_chain.Clipboard(fp,TRUE);
		g_bClipBoardCopyNode=FALSE;
		fclose( fp );
	}

	return;
}

void CHLDoc::PasteCmd()
{
	FILE *fp;
	
	if( (fp = fopen("C:\\TMPDATA.DAT","rt")) != NULL )
	{
		m_chain.Clipboard(fp,FALSE);
		fclose( fp );
	}

	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
	UpdateAllViews(NULL);

	return;
}

void CHLDoc::OnEditPaste() 
{
	m_commandProcessor.Submit(new CCmdPaste(this));
	if (NULL != m_pTracker)
	{
		m_pTracker->ClearAll();
	    ReLoadStrokeList();
	}
	else
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
		m_pTracker->ClearAll();
	    ReLoadStrokeList();
	}

	return;
}

void CHLDoc::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	return;
}

void CHLDoc::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	CFileStatus st;
	BOOL bEnable = CFile::GetStatus("C:\\TMPDATA.DAT",st);

	pCmdUI->Enable(bEnable);
}

void CHLDoc::OnEditCut() 
{
	m_bEditCut = TRUE;
	OnEditCopy();
	Delete();
	m_bEditCut = FALSE;

	return;
}

void CHLDoc::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	return;
}

void CHLDoc::OnEditDelete() 
{
	Delete();
}

void CHLDoc::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

    return;
}

void CHLDoc::OnEditRedo() 
{
	m_commandProcessor.Redo();
	if (NULL != m_pTracker)
	{
		m_pTracker->ClearAll();
	    ReLoadStrokeList();
	}
	else
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
		m_pTracker->ClearAll();
	    ReLoadStrokeList();
	}

	m_chain.RefreshSelectQuadList(m_QuadList);
	return;
}

void CHLDoc::OnEditUndo() 
{
	m_commandProcessor.Undo();
	if (NULL != m_pTracker)
	{
		m_pTracker->ClearAll();
		ReLoadStrokeList();
	}
	else
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
		m_pTracker->ClearAll();
	    ReLoadStrokeList();
	}

	m_chain.RefreshSelectQuadList(m_QuadList);
	return;
}

void CHLDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_commandProcessor.CanRedo());
}

void CHLDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_commandProcessor.CanUndo());
}

CChain & CHLDoc::GetChain()
{
	SetCurMgrText();
	return m_chain;
}

void CHLDoc::SetChain(const CChain &chain)
{
	SetCurMgrText();
//	m_chain=chain;
	m_chain.ReStoreChain(chain);
	SetModifiedFlag();
	UpdateAllViews(NULL);
	SetTrackerQuad(m_chain.GetLimit());

	return;
}

void CHLDoc::OnEditSelectAll() 
{
	int nCount=m_chain.GetCount();
	if(nCount<=0)
	{
		return;
	}

	m_commandProcessor.Submit(new CCmdStoreDoc(this));
	if (NULL != m_pTracker)
	{
		m_pTracker->ClearAll();
		ReLoadStrokeList();
	}
	else
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
		m_pTracker->ClearAll();
	    ReLoadStrokeList();
	}

	m_chain.RefreshSelectQuadList(m_QuadList);

	CFrameWnd *pWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pWnd->GetActiveFrame()->GetActiveView();
	pView->DispSelObjProp();
	pView->Invalidate();

	return;
}

void CHLDoc::FillCmd(FILLDATA fill)
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	m_pTracker->ClearAll();
	m_chain.SetFillStyle(fill,m_pTracker);
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
	CFrameWnd *pFWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pFWnd->GetActiveFrame()->GetActiveView();
	pView->Invalidate();

	return;
}

void CHLDoc::OnDrawFill() 
{
	if(m_nStatus==CChain::selectNull)
	{
		CString str;
		str.LoadString(IDS_NOOBJECT);
		AfxMessageBox(str,MB_ICONWARNING);
	}
	else
	{
		FILLDATA fillOld,fillNew;
 		m_chain.GetFillStyle(fillOld);

		CDrawFillDlg dlg;
		dlg.m_dbAngle1 = fillOld.dbAngle1;
		dlg.m_dbAngle2 = fillOld.dbAngle2;
		dlg.m_dbSpace1 = fillOld.dbSpace1;
		dlg.m_dbSpace2 = fillOld.dbSpace2;
		dlg.m_dbDistance = fillOld.dbDistance;
		dlg.m_dbPtp = fillOld.dbPtp;
        dlg.m_dbCircleLen = fillOld.dbCircleLen;
        dlg.m_dbHeadKillLen = fillOld.dbHeadKillLen;
		dlg.m_dbTailKillLen = fillOld.dbTailKillLen;

		dlg.m_bBack = (fillOld.nStyle&FILL_BACK)>0?1:0;
		dlg.m_bCross = (fillOld.nStyle&FILL_CROSS)>0?1:0;
		dlg.m_bEven = (fillOld.nStyle&FILL_EVEN)>0?1:0;
		dlg.m_bFirstFill = (fillOld.nStyle&FILL_MORD)>0?1:0;
		dlg.m_bPtp = (fillOld.nStyle&FILL_PTP)>0?1:0;
        dlg.m_bCircle = (fillOld.nStyle&FILL_CIRCLE)>0?1:0;
        dlg.m_bBetter = (fillOld.nStyle&FILL_BETTER)>0?1:0;

		if(dlg.DoModal()==IDOK)
		{
			fillNew.dbAngle1 = dlg.m_dbAngle1;
			fillNew.dbAngle2 = dlg.m_dbAngle2;
			fillNew.dbSpace1 = dlg.m_dbSpace1;
			fillNew.dbSpace2 = dlg.m_dbSpace2;
			fillNew.dbDistance = dlg.m_dbDistance;
			fillNew.dbPtp = dlg.m_dbPtp;
			fillNew.dbCircleLen = dlg.m_dbCircleLen;
			fillNew.dbHeadKillLen = dlg.m_dbHeadKillLen;
			fillNew.dbTailKillLen = dlg.m_dbTailKillLen;
			fillNew.nStyle = 0;
			fillNew.nStyle = dlg.m_bBack?fillNew.nStyle|FILL_BACK:fillNew.nStyle;
			fillNew.nStyle = dlg.m_bCross?fillNew.nStyle|FILL_CROSS:fillNew.nStyle;
			fillNew.nStyle = dlg.m_bEven?fillNew.nStyle|FILL_EVEN:fillNew.nStyle;
			fillNew.nStyle = dlg.m_bFirstFill?fillNew.nStyle|FILL_MORD:fillNew.nStyle;
			fillNew.nStyle = dlg.m_bPtp?fillNew.nStyle|FILL_PTP:fillNew.nStyle;
			fillNew.nStyle = dlg.m_bCircle?fillNew.nStyle|FILL_CIRCLE:fillNew.nStyle;
            fillNew.nStyle = dlg.m_bBetter?fillNew.nStyle|FILL_BETTER:fillNew.nStyle;

			CCmdFill* pFillCmd = new CCmdFill(this,fillNew,fillOld);
			m_commandProcessor.Submit(pFillCmd);

			CFrameWnd *pFWnd=(CFrameWnd*)AfxGetMainWnd();
			CHLView *pView=(CHLView*)pFWnd->GetActiveFrame()->GetActiveView();
			if (pView) 
			{
				pView->DispSelObjProp();
			}
		}
	}

	return;
}

void CHLDoc::OnExtendObj(BOOL bIsKillOldObj) 
{
	if(m_nStatus==CChain::selectNull)
	{
		CString str;
		str.LoadString(IDS_NOOBJECT);
		AfxMessageBox(str, MB_ICONWARNING);
	}
	else
	{
		CCmdExtendObj* pExtendCmd = new CCmdExtendObj(this, g_SysCfg.m_dbExtendSpace, bIsKillOldObj);
		m_commandProcessor.Submit(pExtendCmd);
	}

	return;
}

void CHLDoc::ExtendObjCmd(double dbSpace, BOOL bIsKillOldObj)
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	m_pTracker->ClearAll();
//	m_chain.ExtendObj(dbSpace,bIsKillOldObj, m_pTracker);
	m_chain.ExtendObj(dbSpace,bIsKillOldObj, m_pTracker, m_bExternOpt);
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView *pView   = (CHLView*)pFWnd->GetActiveFrame()->GetActiveView();
	pView->Invalidate();

	return;
}

BOOL CHLDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	if (!(g_CurUserInfo.dwRight1 & 128))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return TRUE;
	}
	
	{
		g_OpenFileName=lpszPathName;
		g_OldFileName=m_strPathName;				
	}

	m_commandProcessor.Commit();

	//用于去除多的文本对象
	g_MgrText = &(m_chain.m_MgrText);
	g_MgrText->SetUse(FALSE);
	m_chain.PreprocessMark(0x03);

	SetProStep(0);

	//采用自制的序列化流程
	FILE *fp;	
	if( (fp = fopen(lpszPathName,"wb")) != NULL )
	{
		Serialize(fp,TRUE);
		fclose( fp );
	}

	SetProStep(0);

	return TRUE;
}

void CHLDoc::OnChangeCenter() 
{
	//居中
	CDot dot=m_quadTracker.GetBaseDot();
	double moveX=CENTER-dot.x;
	double moveY=CENTER-dot.y;

	Drag(dot,1,1,moveX,moveY);
	CQuad quad=GetLimit();

	SetTrackerQuad(quad);
	m_chain.RefreshSelectQuadList(m_QuadList);
	UpdateAllViews(NULL);

	return;
}

void CHLDoc::OnUpdateChangeCenter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	return;
}

void CHLDoc::OnChangeCcw() 
{
	// TODO: Add your command handler code here
	CDot dot;
	dot=m_quadTracker.GetBaseDot();
	Rotate(dot,90);

	return;
}

void CHLDoc::OnUpdateChangeCcw(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	return;
}

void CHLDoc::OnChangeCw() 
{
	// TODO: Add your command handler code here
	CDot dot;
	dot=m_quadTracker.GetBaseDot();
	Rotate(dot, -90);

	return;
}

void CHLDoc::OnUpdateChangeCw(CCmdUI* pCmdUI) 
{
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	return;
}

void CHLDoc::OnChangeMirrorX() 
{
 	if(GetBarcode())
	{
		return;
	}

	CDot dot=m_quadTracker.GetBaseDot();
	Drag(dot,-1,1,0,0);
	CFrameWnd *pWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pWnd->GetActiveFrame()->GetActiveView();
	pView->Invalidate();

	return;
}

void CHLDoc::OnUpdateChangeMirrorX(CCmdUI* pCmdUI) 
{
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	return;
}

void CHLDoc::OnChangeMirrorY() 
{
 	if(GetBarcode())
	{
		return;
	}

	CDot dot = m_quadTracker.GetBaseDot();
	Drag(dot,1,-1,0,0);

	CFrameWnd *pWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView *pView  = (CHLView*)pWnd->GetActiveFrame()->GetActiveView();
//	CDC *pDC        = pView->GetDC();
	pView->Invalidate();

	return;
}

void CHLDoc::OnUpdateChangeMirrorY(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	return;
}

void CHLDoc::SetLay(int mode,CLayer &ly)
{
	m_commandProcessor.Submit(new CCmdSetLay(this,mode,ly));
}

void CHLDoc::SetLayCmd(int mode,CLayer ly)
{
	SetModifiedFlag();
	switch(mode)
	{
	case PTY_ADD:
		m_lay.AddNew();
		break;
	case PTY_LAYER:
		m_lay.Set(mode,ly);
		m_chain.SetProperty(mode,ly.GetProperty());
		break;
	case PTY_PARA:
		m_lay.Set(mode,ly);
		break;
	case PTY_OUTLINE:
		g_DAT.SetMarkPara(m_lay);
		m_chain.SetLayColor(ly.GetProperty().m_nLayer, -1);
		break;
	case PTY_FILL:
	    g_DAT.SetMarkPara(m_lay); // 最好放到启动打标对话框中进行设置
//		int nFrameLayer,nFillLayer;
//		m_chain.GetLayColor(nFrameLayer,nFillLayer);
//		if(mode==PTY_OUTLINE)
//		{
//		    m_chain.SetLayColor(ly.GetProperty().m_nLayer, -1);
//		}
//		else if(mode==PTY_FILL)
//		{
//			m_chain.SetLayColor(-1, ly.GetProperty().m_nLayer);
//		}

		m_chain.SetLayColor(-1, ly.GetProperty().m_nLayer);
		break;
	case PTY_DEL:
		//将被删层所对应的图形对象移至第0层
		m_chain.SetProperty(mode,ly.GetProperty());
		m_lay.Del(ly.m_nLayer);
		break;
	case PTY_CHANGE:
		m_lay.SetCur(ly.m_nLayer);
		break;
	}

	return;
}

//Updated by AndyF.Shelby on 2 Mar. 2006 for SleepMode
CLayer& CLay::GetDefaultLayer()
{
	int nMinLayerNo = m_list[0].m_nLayer;
	int nSize = m_list.GetSize();
	for(int i=0; i<nSize; i++)
	{
		nMinLayerNo = nMinLayerNo < m_list[i].m_nLayer ? nMinLayerNo : m_list[i].m_nLayer;
	}

	return m_list[nMinLayerNo];
}

CLayer CHLDoc::GetCurLayer()
{
	return m_lay.GetCurLayer();
}

CLayer CHLDoc::GetLayer(int index)
{
	return m_lay.GetLayer(index);
}

void CHLDoc::SetLayer(CLayer &layer)
{
	m_lay.SetLayer(layer);
	SetModifiedFlag();

	CMainFrame *pFrame=(CMainFrame *)AfxGetMainWnd();
	if (pFrame)
	{
		pFrame->m_XBarMarkPara.m_PropDlg.DoUndoLayerPara();
	}
	
	return;
}

void CHLDoc::SetLayerColor(int index, COLORREF color)
{
	m_lay.SetLayerColor(index, color);
}

BOOL CHLDoc::GetLockFlg(int index)
{
	return m_lay.GetLockFlg(index);
}


int CHLDoc::GetLaySize()
{
	return m_lay.GetSize();
}

void CHLDoc::SetCurLayer(int cur)
{
	m_lay.SetCur(cur);
}

int CHLDoc::GetCur()
{
	return m_lay.GetCur();
}


CLay & CHLDoc::GetLay()
{
	return m_lay;
}

void CHLDoc::SetLay(const CLay &lay)
{
	m_lay=lay;
	SetModifiedFlag();

	CMainFrame *pFrame=(CMainFrame *)AfxGetMainWnd();
	if (pFrame)
	{
		pFrame->m_XBarMarkPara.m_PropDlg.UpdateProperty();
	}
}

void CHLDoc::OnMarkPreview() 
{
	g_DAT.SetMarkPara(m_lay);
	m_chain.PreprocessMark();

	CMarkPreviewDlg dlg;
	dlg.m_pChain=(CChain *)&m_chain;

	dlg.DoModal();
}

int CHLDoc::GetMenuIndex()
{	
	//根据当前选中的对象确定右键菜单
	return m_chain.GetMenuIndex();
}

void CHLDoc::DrawsetCmd() 
{
	CProperty pty=m_lay.GetCurLayer().GetProperty();

	CNode *pNode = m_chain.GetNode();
	if(pNode == NULL)
	{
		return;
	}

	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	if(pNode->GetType() == NODE_TEXT_DSF)
	{
/*		ClearTextMem();
		//文本方式则在此处处理
		pNode->Create(CRect(0,0,0,0));
		FILLDATA fill;
		m_chain.GetFillStyle(fill);
		m_chain.SetFillStyle(fill,m_pTracker);
		SetModifiedFlag();
		SetTrackerQuad(m_chain.GetLimit());
		UpdateAllViews(NULL);
*/		
		EditTextNode(pNode);
	}

	//条码设置在此处处理
	if(pNode->GetType() == NODE_CODEBAR)
	{
		//文本方式则在此处处理
		pNode->Create(CRect(0,0,0,0));
		//用于刷新文本内容
		m_chain.PreprocessMark(0x04);
		SetModifiedFlag();
		SetTrackerQuad(m_chain.GetLimit());
		UpdateAllViews(NULL);	
	}

	return;
}

void CHLDoc::OnDrawSet() 
{
	m_commandProcessor.Submit(new CCmdDrawSet(this));
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	m_pTracker->ClearAll();
	ReLoadStrokeList();
}

void CHLDoc::ArrayCmd(CArray<OBJPOS, OBJPOS> &dtArray)
{
	m_chain.Array(dtArray);

	m_chain.RefreshSelectQuadList(m_QuadList);
	SetTrackerQuad(m_chain.RetSelQuad());
}

void CHLDoc::OnUpdateArray(CCmdUI* pCmdUI) 
{
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}

	return;
}

void CHLDoc::OnArray() 
{
	CArrayDlg dlg;
	CDot dot;
	dot=m_quadTracker.GetBaseDot();
	dlg.m_dotBase=dot;

	CHLApp *pApp=(CHLApp *)AfxGetApp();
	dlg.m_bAuto=pApp->GetProfileInt("OPTION","ARRAY",0);
	if(dlg.DoModal()==IDOK)
	{	
		if(m_nStatus==CChain::selectNull)
		{
			CString str;
			str.LoadString(IDS_NOOBJECT);
			AfxMessageBox(str,MB_ICONWARNING);
		}
		else
		{
			m_commandProcessor.Submit(new CCmdArray(this,dlg.m_dtArray,dlg.m_bAuto));
		}

//		CFrameWnd *pWnd = (CFrameWnd*)AfxGetMainWnd();
//		CHLView *pView  = (CHLView*)pWnd->GetActiveFrame()->GetActiveView();
//		pView->Invalidate();

		pApp->WriteProfileInt("OPTION","ARRAY",dlg.m_bAuto);
	}

	return;
}

void CHLDoc::GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray)
{
	m_chain.GetArrayDot(dtArray);
}

void CHLDoc::SetSelectFlag(BOOL *pFlag)
{
    SetTrackerQuad(m_chain.SetSelectFlag(pFlag));
}

void CHLDoc::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_chain.GetCount());
}


void CHLDoc::Mark(HWND hWnd,int repeat)
{
	m_chain.Mark(hWnd);
}

BOOL CHLDoc::PreprocessMark()
{
	if(!m_chain.PreprocessMark())
	{
		return FALSE;
	}
	
	return TRUE;
}

int CHLDoc::GetMarkUnit()
{
	return m_chain.GetCount();
}

void CHLDoc::OnDrawBmp()
{
	CNodeDib *pNode;
	pNode=(CNodeDib *)(m_chain.GetNode());
	if( pNode && (pNode->GetType() ==NODE_BMP))
	{	
		CDibDlg dlg;
		CString str      = pNode->m_strBmpName;
		dlg.m_strBmpName = str;
		dlg.m_dbSpaceX   = pNode->m_dbSpaceX;
		dlg.m_dbSpaceY   = pNode->m_dbSpaceY;
		dlg.m_dbDotTime  = pNode->m_dbDotTime;
		dlg.m_bOppsitioncol = pNode->m_bOppsitionCol;
		dlg.m_bLoop    = pNode->m_bLoop;
		dlg.m_nDibType = pNode->m_nDibType;
		dlg.m_dbLoopCompate = pNode->m_dbLoopCompate;
		dlg.m_nAccTime      = pNode->m_nAccTime;
		dlg.m_dbBrightness  =pNode->m_dbBrightness;
		dlg.m_dbContrast    =pNode->m_dbContrast;

		PARA para         = m_lay.GetPara(pNode->m_property.m_nLayer);
		//dsf 2010.04.06
		//dlg.m_dbMarkSpeed = para.dbMarkSpeed/(1e6); 
		if(dlg.DoModal()==IDOK)
		{
			pNode->m_bOppsitionCol=dlg.m_bOppsitioncol;
			pNode->m_dbSpaceX = dlg.m_dbSpaceX; 
			pNode->m_dbSpaceY = dlg.m_dbSpaceY;
			pNode->m_dbDotTime = dlg.m_dbDotTime;
			pNode->m_bLoop = dlg.m_bLoop;
			pNode->m_nDibType=dlg.m_nDibType;
			pNode->m_dbLoopCompate=dlg.m_dbLoopCompate;
			pNode->m_nAccTime=dlg.m_nAccTime;
			pNode->m_dbBrightness  = dlg.m_dbBrightness;
			pNode->m_dbContrast    = dlg.m_dbContrast;
			
			pNode->DataChange();
			m_chain.CalLimit(CDot(0,0));
			SetTrackerQuad(m_chain.GetLimit());
			//g_DAT.SetLay(m_lay);
		}
	}
	else
	{
		CString str;
		str.LoadString(IDS_NOOBJECT);
		AfxMessageBox(str,MB_ICONWARNING);
	}
	UpdateAllViews(NULL,UPDATE_TRACKER_VIEW);

	return;
}

void CHLDoc::UpdateLay()
{
	m_lay.UpdateList();

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	if (pFrame)
	{
		pFrame->m_XBarMarkPara.m_PropDlg.UpdateProperty();
	}

	return;
}

void CHLDoc::SetSelectedObjbeMarked(BOOL bFlag)
{
	m_chain.SetSelectedObjbeMarked(bFlag);
}

void CHLDoc::SetLayerCmd()
{
	g_DAT.SetMarkPara(m_lay);
	CSetLayer dlg;
	m_chain.GetLayColor(dlg.m_nLayer,dlg.m_nFillLayer);

	while(1)
	{
		UINT nRet = dlg.DoModal();

		if (nRet == IDOK)
		{
			CLayer& lay = GetLayer(dlg.m_nLayer);
			CLayer& layFill = GetLayer(dlg.m_nFillLayer);
			if (lay.m_nLayer != dlg.m_nLayer || layFill.m_nLayer != dlg.m_nFillLayer)
			{
				AfxMessageBox(IDS_LAYER_LIMIT);
				continue;
			}
			m_chain.SetLayColor(dlg.m_nLayer,dlg.m_nFillLayer);
		}
		break;
	}
	
	SetTrackerQuad(m_chain.GetLimit());

	return;
}

void CHLDoc::OnSetLayer() 
{
	m_commandProcessor.Submit(new CCmdChangLay(this));
}

BOOL CHLDoc::GetBarcode()
{
	return FALSE;
}

void CHLDoc::SetSelect(BOOL bFlag)
{
	m_chain.SetSelect(bFlag);
}

void CHLDoc::UpdatePerUnit()
{
	m_chain.CalLimit(CDot(0,0));
	SetTrackerQuad(m_chain.GetLimit());
}

void CHLDoc::GetSelectedStrokeList(STROKELIST *pStrokeList)
{
	m_chain.GetSelectedStrokeList(pStrokeList);
}

void CHLDoc::AddTree(CTree *pAddTree)
{
	m_commandProcessor.Submit(new CCmdAddNode(this,pAddTree));
}


void CHLDoc::SetCurMgrText()
{
	g_MgrText = &(m_chain.m_MgrText);
}

//建立与条码关联的文本设置
void CHLDoc::OnDrawBartext() 
{
	CNode *pNode=NULL;
	pNode = m_chain.GetNode();
	if(pNode == NULL)
	{
		return;
	}
	
	if(pNode->GetType() == NODE_CODEBAR)
	{
		CNodeCodebar* pNodeBar = (CNodeCodebar*)pNode;
		UINT nTextID = pNodeBar->m_nTextID;
		CNodeText* pNodeText = new CNodeText();
		pNodeText->CreateFromBar(nTextID);

		if(pNodeText->Create(CRect(0,0,0,0)))
		{
			AddNode(pNodeText);
			SetModifiedFlag();
			SetTrackerQuad(m_chain.GetLimit());
			UpdateAllViews(NULL);
		}
		else
		{
			delete pNodeText;
		}
	}

	return;
}

void CHLDoc::OnExportPlt() 
{
	// TODO: Add your command handler code here
	if(0 == m_chain.GetCount())
	{
		return;
	}

	CDlgExportPlt fileDlg(FALSE,"plt",NULL, OFN_OVERWRITEPROMPT,"PLT File(*.plt)|*.plt||");
	if(IDOK==fileDlg.DoModal())
	{
		CStdioFile file;
		if (file.Open(fileDlg.GetPathName(),CFile::modeCreate|CFile::modeWrite))
		{
			file.WriteString("IN;DF;PA;");
			m_chain.ExportPlt(file,fileDlg.m_bUnExportFill,fileDlg.m_bScale100);
			file.Close();
		}
		else
		{
			CString strError;
			strError.LoadString(IDS_PLT_EXPORT_ERR);
			AfxMessageBox(strError);
		}
	}

	return;
}

void CHLDoc :: AddRect(CQuad &quad,CWnd *pWnd)
{
	m_chain.AddRect(quad,pWnd);
}


void CHLDoc::DelSelectTree(CTree *pTree)
{
	m_chain.DelSelectTree(pTree);
}

void CHLDoc::Drag(CDot &dot,double ratioX,double ratioY,double moveX,double moveY)
{
	if (!(g_CurUserInfo.dwRight1 & 16))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	if(m_nStatus == CChain::selectNull)
	{
	    return;
	}
	
	m_commandProcessor.Submit(new CCmdNewDrag(this,dot,ratioX,ratioY,moveX,moveY));

	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());

	// 重新刷新
	m_chain.RefreshSelectQuadList(m_QuadList);
	return;
}

void CHLDoc::DotEdit(XTracker *pTrack, CPoint &point, CWnd *pWnd)
{
	m_commandProcessor.Submit(new CCmdDotEdit(this,point,pWnd));
}

void CHLDoc::DelDot(CWnd *pWnd)
{
	m_commandProcessor.Submit(new CCmdDelDot(this,pWnd));
}

BOOL CHLDoc::SetStrokeList(StrokeList *pStrokeList, CPoint &point, CWnd *pWnd)
{
	return m_chain.SetStrokeList(pStrokeList,point,pWnd);
}

void CHLDoc::CaptureObjectCmd(CQuad &quad)
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}
    m_chain.CaptureObject(quad,m_pTracker);
	
	return;
}

void CHLDoc::MultiSelect(CQuad &quad)
{
	m_commandProcessor.Submit(new CCmdMultiSelect(this,quad));
//	CaptureObjectCmd(quad);

	return;
}

void CHLDoc::DragCmd(CDot &dot, double dbratioX, double dbratioY, double dbmoveX, double dbmoveY)
{
	m_chain.DragSelectTree(dot,dbratioX,dbratioY,dbmoveX,dbmoveY);

	SetObjProp();

	return;
}


void CHLDoc::SelectDraw(CTree *pTree)
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	m_pTracker->ClearStrokeList();
	m_chain.SelectDraw(pTree,m_pTracker);
}

BOOL CHLDoc::HaveSelectObject()
{
	return m_chain.HaveSelectObject();
}

BOOL CHLDoc::HitOtherObject(CPoint &point, CWnd *pWnd)
{
	BOOL bflag=FALSE;
	bflag=m_chain.HitOtherObject(point,pWnd);
	if(bflag)
	{
		m_commandProcessor.Submit(new CCmdHitOtherObj(this,point,pWnd));
		CQuad quad=GetLimit();
		SetTrackerQuad(quad);
	}
	else
	{
		short  nVKey=GetKeyState(VK_SHIFT);
		if(nVKey & SHIFTED)
		{
			CHLView *pView=(CHLView*)pWnd;
			m_chain.SetSelInQuadListByDot(m_QuadList, point, pWnd);
		}
		else if (GetKeyState(VK_CONTROL) & SHIFTED)
		{
			m_chain.SetUnSelInQuadListByDot(m_QuadList, point, pWnd);
			m_chain.RefreshSelectQuadList(m_QuadList);
		}
		else
		{
			m_QuadList.RemoveAll();

			POSITION pos = m_chain.m_listCopyObject.GetHeadPosition();
			while(pos)
				m_chain.m_listCopyObject.GetNext(pos).m_bSelect = FALSE;
		}
	}

	return bflag;
}

void CHLDoc::AddPoint(CPoint &point,CWnd *pWnd)
{
	m_commandProcessor.Submit(new CCmdAddPoint(this,point,pWnd));
}

void CHLDoc::ReLoadStrokeList()
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	m_pTracker->m_StrokeList.RemoveAll();
	m_chain.ReLoadStrokeList(&m_pTracker->m_StrokeList);
}

void CHLDoc::ChangeQuadCmd(CPoint &point, CWnd *pWnd)
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	m_chain.ChangeQuad(point, m_pTracker, pWnd, m_QuadList);

	return;
}


CQuad CHLDoc::GetAllLimit()
{
	return m_chain.GetAllLimit();
}

CQuad CHLDoc::GetLimit()
{
	return m_chain.GetLimit();
}

void CHLDoc::SplitDot(CWnd *pWnd, double dbPoint)
{
	m_commandProcessor.Submit(new SplitPointCmd(this,pWnd,dbPoint));
}

void CHLDoc::Changeconvert(BOOL bPopMsg) //转为曲线，适用于将TREE，CODEBAR，CTEXT转为NODEPLT
{
	// TODO: Add your command handler code here
	m_commandProcessor.Submit(new CCmdConvert(this, bPopMsg));
}

void CHLDoc::ConvertCmd(BOOL bPopMsg)
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	if (NULL == m_pTracker)
	{
		m_pTracker = &(pView->m_track);
	}

	m_chain.Convert(bPopMsg);
	m_pTracker->ClearAll();

	ReLoadStrokeList();

	CQuad quad=GetLimit();
	SetTrackerQuad(quad);

	pView->DispSelObjProp();

	return;
}

void CHLDoc::JoinPoint(CWnd *pWnd)
{
	m_commandProcessor.Submit(new JoinPointCmd(this,pWnd));
}

void CHLDoc::MoveAccurate(double MoveX, double MoveY)
{
	CFrameWnd *pFWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pFWnd->GetActiveFrame()->GetActiveView();
	if (NULL == pView) 
	{
		return;
	}
	
	if (pView->m_nDrawTool==ID_DRAW_DRAG) 
	{
		m_commandProcessor.Submit(new MoveAccCmd(this,m_pTracker,MoveX,MoveY));
	}
	else
	{
		Drag(CDot(0,0),1.0,1.0,MoveX, MoveY);
		SetTrackerQuad(GetLimit());
	}
}

void CHLDoc::OnLockLayer(int nLayer)
{
	m_lay.SetLockFlg(nLayer, TRUE);
}

void CHLDoc::OnUnLockLayer(int nLayer)
{
	m_lay.SetLockFlg(nLayer, FALSE);
}

void CHLDoc::FilterPoint()
{
	m_commandProcessor.Submit(new CCmdFilterDocPoint(this));
}

void CHLDoc::FilterPointCmd()
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}
	
	m_chain.FilterPoint(g_SysCfg.m_dbDotFilterPrecis, g_SysCfg.m_dbDotFilterAngle);
	m_pTracker->ClearAll();

	ReLoadStrokeList();

	CQuad quad=GetLimit();
	SetTrackerQuad(quad);

	return;
}

void CHLDoc::OnKillAngle()
{
	m_commandProcessor.Submit(new CCmdKillAngle(this));
}

void CHLDoc::KillAngleCmd()
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	if (NULL == m_pTracker)
	{
		m_pTracker = &(pView->m_track);
	}

	m_pTracker->KillAngle(pView);
	m_pTracker->ClearAll();

	ReLoadStrokeList();

	CQuad quad=GetLimit();
	SetTrackerQuad(quad);

	return;
}

void CHLDoc::IniSort(CWnd* pWnd)
{
	if (NULL == pWnd)
	{
		return;
	}

	m_chain.InitSort(m_Sort, pWnd);
	m_SortListObj.InitSortPro();

	pWnd->Invalidate();
	
	return;
}


void CHLDoc::DrawMark(CDC* pDC, CWnd *pWnd)
{
	if (NULL == pDC)
	{
		return;
	}
	
	CHLView *pView = (CHLView*)pWnd;
	CPen   pen(PS_SOLID, 1, RGB(255,0,0));
	CPen  *OldPen = pDC->SelectObject(&pen);

	CFont fontSort;
    fontSort.CreateFont(13, 0,0,0, 50, FALSE, FALSE, 0,
                         ANSI_CHARSET, OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                         DEFAULT_PITCH | FF_SWISS, "Arial");
    CFont* pOldFont = pDC->SelectObject(&fontSort);
	pDC->SetTextColor(RGB(255, 255, 255));

	int nCount = m_Sort.GetSize();
	for (int i = 0 ; i < nCount; i++) 
	{
		SORTNODE sNode;
		sNode = m_Sort.GetAt(i);

	    double x = sNode.m_SPt.x;
		double y = sNode.m_SPt.y;

		pView->PhysicToLogic(x, y);

		int nOff = sNode.m_MNum * 21;
		
		CRect rt(nOff + (int)x, (int)y - 10, nOff +(int)x + 20, (int)y);
		pDC->FillSolidRect(rt, sNode.m_Color);
		CString strIndex("");
		strIndex.Format("%d", sNode.m_nSort);
		pDC->DrawText(strIndex, &rt, DT_CENTER);
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(OldPen);

	return;
}

void CHLDoc::SortAllObj()
{
	m_chain.SortAllObj(m_Sort);
	return;
}

void CHLDoc::CreateNewRect(CRect &lRt, CRect &cRt, CRect &nRt)
{
	CRect r;
	BOOL bRet = IntersectRect(r, lRt, cRt);
	if (bRet)
	{
		nRt = lRt;
		nRt.left  += 20;
		nRt.right += 20;
	}

	return;
}

BOOL CHLDoc::InitASort(CWnd* pWnd)
{
	m_ASortListObj.m_dwSelObj = 0;
	m_chain.InitASort(m_ASort);
	int nCount = m_ASort.GetSize();
	if (nCount < 6)
	{
		AfxMessageBox(IDS_AOTUSORTLESSSIXOBJ);
		m_ASort.RemoveAll();
		return FALSE;
	}
	
	CHLView *pView = (CHLView*)pWnd;
	pView->Invalidate();

	if (IDOK == AfxMessageBox(IDS_IFSETSTARTDOT, MB_OKCANCEL)) 
	{   
		if (!m_ASortListObj.TrackSelPt(pWnd, m_ASort))
		{
			m_ASort.RemoveAll();
			return FALSE;
		}
	}

	m_ASortListObj.RunSort(pWnd);
	AfxGetMainWnd()->SetFocus();
	m_ASortListObj.Track(pWnd);
	
	return TRUE;
}

void CHLDoc::ExitASort()
{
	m_ASortListObj.ExitSort();
	if (IDOK ==AfxMessageBox(IDS_APPLYALLOBJECT, MB_OKCANCEL)) 
	{
		m_chain.AutoSortAllObj(m_ASort, m_ASortListObj.m_dwSelObj);
		m_ASortListObj.m_dwSelObj = 0;
	}
	
	return;
}

void CHLDoc::SetGroupNum(UINT nChar) 
{
	int nNum = -1;
	switch(nChar) 
	{
	case VK_0 :
		nNum = 0;
		break;
	case VK_1 :
		nNum = 1;
		break;
	case VK_2 :
		nNum = 2;
		break;
	case VK_3 :
		nNum = 3;
		break;
	case VK_4 :
		nNum = 4;
		break;
	case VK_5 :
		nNum = 5;
		break;
	case VK_6 :
		nNum = 6;
		break;
	case VK_7 :
		nNum = 7;
		break;
	case VK_8 :
		nNum = 8;
		break;
	case VK_9 :
		nNum = 9;
		break;
	default:
		return;
	}

	m_chain.SetGroupNum(nNum);

	return;
}


void CHLDoc::InitSelSort(CWnd* pWnd)
{
	DWORD dwStroke = 0;
	int   nNum     = 0;
	
	m_chain.InitSelSort(m_GSort);

	CSelSort selSort;
	BOOL bSel = selSort.Track(pWnd, m_GSort);
	if (bSel)
	{
		m_chain.GSortAllObj(m_GSort);
	}

	SetTrackerQuad(GetLimit());

	CHLView *pView = (CHLView*)pWnd;
	pView->Invalidate();

	return;
}

void CHLDoc::InitGSort(CWnd* pWnd)
{
	DWORD dwStroke = 0;
	int   nNum     = 0;
	double dbStp  = 0.01;
	
	m_chain.InitGSort(m_GSort, dwStroke);
	CStroke *pStroke = (CStroke *)dwStroke;
	if (NULL == pStroke)
	{
		AfxMessageBox(IDS_GSORT_NO_LINE);
		return;
	}

	CSelSort selSort;
	selSort.GetSort(m_GSort, pStroke);

	m_chain.GSortAllObj(m_GSort);

	if (pStroke)
	{
		delete pStroke;
		pStroke = NULL;
	}

	ReLoadStrokeList();

	SetTrackerQuad(GetLimit());

	CHLView *pView = (CHLView*)pWnd;
	pView->Invalidate();

	return;
}

BOOL CHLDoc::isIntersect(CDot&p1 , CDot& p2 , CDot& p3 , CDot& p4 )
{
	double dblMulti = 0;
	double dblTmp1  = 0;
	double dblTmp2  = 0;

	// 顶点相交
	if ( p1 == p3 || p1==p4 || p2==p3 || p2==p4 )
	{
		return TRUE;
	}

    // 判断两条线段外接矩形是否相交
	if ( min(p1.x,p2.x) > max(p3.x,p4.x) || max(p1.x,p2.x)
		< min(p3.x,p4.x) || min(p1.y,p2.y) > max(p3.y,p4.y)
		|| max(p1.y,p2.y) < min(p3.y,p4.y))
	{
		return FALSE;
	}

    //计算叉积  
	dblTmp1 = (double)((p1.x - p3.x)*(p4.y-p3.y) - (p4.x-p3.x)*(p1.y - p3.y));
	dblTmp2 = (double)((p4.x -p3.x)*(p2.y - p3.y) - (p2.x-p3.x)*(p4.y-p3.y));
	dblMulti = dblTmp1 * dblTmp2;
	
	if ( dblMulti >= 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

double CHLDoc::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return dbD;
}

// 求两点之间的角度
double CHLDoc::GetAngle(CDot &dtB, CDot &dtC)
{
	double dbL = 0;
	double dbK = 0;
	double ang = 0;
	
	// 如果两点重合时弧度为零
	if (fabs(dtB.x - dtC.x) < EPSINON && 
		fabs(dtB.y - dtC.y) < EPSINON)  
	{
		return 0;
	}

	// 如果两点在一条垂线上
	if (fabs(dtB.x - dtC.x) < EPSINON)       
	{
		if (dtC.y > dtB.y)            // 90度
		{
			ang = PAI / 2;
		}
		else
		{
			ang = PAI * 3 / 2;	      // 270度
		}
	}
	else// 如果两点不在一条垂线上
	{
		dbL  = Distance(dtB,dtC);      // 两点距离
		dbK  = (dtC.x - dtB.x) / dbL;  // 余弦
		ang  = acos(dbK);              // 反余弦求弧度   
		
		if (dtC.y < dtB.y)
		{
			ang = PAI * 2 - ang;       // 弧度在三 四象限
		}
	}
	
	return ang;
}


void CHLDoc::ComToOneTreeCmd()
{
	g_DAT.SetMarkPara(m_lay);
	m_chain.ComToOneTreeCmd();
	
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());

	m_chain.RefreshSelectQuadList(m_QuadList);
	return;
}

void CHLDoc::ConvertSelToOneTree()
{
	// TODO: Add your command handler code here
	m_commandProcessor.Submit(new CCmdConvertSelToOneTree(this));
}


BOOL CHLDoc::SelGroupByNum(UINT nChar, CWnd *pWnd)
{
	int nNum = -1;
	switch(nChar) 
	{
	case VK_0 :
		nNum = 0;
		break;
	case VK_1 :
		nNum = 1;
		break;
	case VK_2 :
		nNum = 2;
		break;
	case VK_3 :
		nNum = 3;
		break;
	case VK_4 :
		nNum = 4;
		break;
	case VK_5 :
		nNum = 5;
		break;
	case VK_6 :
		nNum = 6;
		break;
	case VK_7 :
		nNum = 7;
		break;
	case VK_8 :
		nNum = 8;
		break;
	case VK_9 :
		nNum = 9;
		break;
	default:
		return FALSE;
	}

	m_commandProcessor.Submit(new CCmdSelGroupByNum(this,nNum,pWnd));
	CQuad quad=GetLimit();
	SetTrackerQuad(quad);

	m_chain.RefreshSelectQuadList(m_QuadList);

	return TRUE;
}

void CHLDoc::SelGroupByNumCmd(int nNum, CWnd *pWnd)
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	m_chain.SelGroupByNum(nNum, m_pTracker, pWnd);

	return;
}

void CHLDoc::SetGroupObjSel(CGroupObj *pGroupObj)
{
	if (NULL == pGroupObj)	
		return;
	
	if ( !CheckSetTrack() )
		return;

	m_chain.SetTreeSel(pGroupObj, m_pTracker);
	
	CQuad quad=GetLimit();
	SetTrackerQuad(quad);
}

void CHLDoc::DelObj(CGroupObj *pGroupObj)
{
	if ((NULL == m_pTracker) || (NULL == pGroupObj))
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	m_commandProcessor.Submit(new CCmdDelObj(this, (DWORD)pGroupObj));
	m_pTracker->ClearAll();
	ReLoadStrokeList();

}

void CHLDoc::DelObjCmd(DWORD dwPara)
{
	CQuad quad;
	m_chain.CalLimit(CDot(0,0),TRUE);
	m_quadTracker = m_chain.GetLimit();
	m_chain.DelOBj(dwPara);
	SetModifiedFlag();

	SetTrackerQuad(m_quadTracker);
	quad.SetEmpty();
	SetTrackerQuad(quad);
	UpdateAllViews(NULL);
}

void CHLDoc::ObjListLoad()
{
	m_chain.ObjListLoad();
}

void CHLDoc::ObjListUnLoad()
{
	m_chain.ObjListUnLoad();
}

void CHLDoc::OnMAlignCmd(int nType) 
{
	// TODO: Add your command handler code here
	m_commandProcessor.Submit(new CCmdMAlign(this, nType));
	UpdateAllViews(NULL);
}

void CHLDoc::MAlignCmd(int nType)
{
	m_chain.OnMAlign(m_QuadList, nType);
	return;
}

double CHLDoc::GetSelTotalLength()
{
	double total = 0;
	int count = m_QuadList.GetSize();
	if ( count == 0 ) 
		return total;

	for (int i=0; i<count; i++)
	{
		CGroupObj* pGroupObj = (CGroupObj *)(m_QuadList[i].m_pGroupObj);
		total += pGroupObj->GetLength();
	}

	return total;
}

void CHLDoc::SetObjProp()
{
	if (NULL == m_pTracker)
	{
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
		{
			return;
		}
		m_pTracker = &(pView->m_track);
	}

	CQuad quad = GetLimit();
	CDot  dt   = quad.GetBaseDot(m_nPosType);
	CFrameWnd *pFrame   = (CFrameWnd*)AfxGetMainWnd();
	CMainFrame *pMFrame = (CMainFrame*)pFrame;
	pMFrame->SetPos(dt.x,dt.y);
	pMFrame->SetSize(quad.right-quad.left,quad.top-quad.bottom);
	m_pTracker->ClearAll();
	ReLoadStrokeList();
	SetTrackerQuad(quad);

	return;
}

void CHLDoc::ClearTextMem()
{
	m_chain.m_MgrText.SetUse(FALSE);
	m_chain.PreprocessMark(0x03);
}

void CHLDoc::SetProStep(int nStep)
{
	CMainFrame *pFWnd = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pFWnd)
	{
		return;
	}

	pFWnd->SetProStep(nStep);

	return;
}

BOOL CHLDoc::IsHaveTextOrCodeBar()
{
	return m_chain.IsHaveTextOrCodeBar();
}

void CHLDoc::OnMarkFast() 
{
	CMainFrame *pFrame=(CMainFrame *)AfxGetMainWnd();
	if (NULL == pFrame)
	{
		return;
	}

	pFrame->m_XBarProperty.m_FillEditDlg.CheckFastmark();
}

void CHLDoc::OnUpdateMarkSpc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CHLDoc::OnMarkSpc() 
{
	// TODO: Add your command handler code here
	CHLApp *pApp=(CHLApp *)AfxGetApp();
	CMultiDocTemplate *pDocTem;
	CHLDoc *pDoc;
	POSITION posTem = theApp.GetFirstDocTemplatePosition();
	while(posTem != NULL)
	{
		pDocTem = (CMultiDocTemplate *)theApp.GetNextDocTemplate(posTem);
		POSITION posDoc = pDocTem->GetFirstDocPosition();
		while(posDoc != NULL)
		{
			pDoc = (CHLDoc *)pDocTem->GetNextDoc(posDoc);
			pDoc->PreprocessMark();
			if(!pDoc->OutOfLimit())
			{
				AfxMessageBox(IDS_ERROT_OUTAREA);
				return;
			}
		}
	}
	
	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
		//		theApp.GetMainWnd()->UpdateWindow();
		//		UpdateAllViews();

		return;
	}

	if (g_bIsSelMark) 
	{
		pApp->m_SelMark.StopSelMark();
		return;
	}
	if (g_bIsFastMark) 
	{
		pApp->m_FastMark.StopFastMark();
		return;
	}

	CMarkSpecialDlg dlg;
	
	if(g_DAT.EnterMarkEnv())
	{
		//硬件部分准备就绪时才能进入此操作
		dlg.DoModal();
		g_DAT.LeaveMarkEnv();
	}

	return;
}

BOOL CHLDoc::GetFillProp(FILLDATA &fillOld) 
{
	if(m_nStatus==CChain::selectNull)
	{
		return FALSE;
	}
	else
	{
 		m_chain.GetFillStyle(fillOld);
	}

	return TRUE;
}

void CHLDoc::UpdateFill(FILLDATA &fillNew) 
{
	FILLDATA fillOld;
 	m_chain.GetFillStyle(fillOld);
	CCmdFill* pFillCmd = new CCmdFill(this,fillNew,fillOld);
	m_commandProcessor.Submit(pFillCmd);

	return;
}

void CHLDoc::UpdateObjMarkSet(OBJMARKSET &ObjMarkSet)
{
	m_chain.UpdateObjMarkSet(ObjMarkSet);

	return;
}

BOOL CHLDoc::GetObjMarkSet(OBJMARKSET &ObjMarkSet) 
{
	if(m_nStatus==CChain::selectNull)
	{
		return FALSE;
	}
	else
	{
 		m_chain.GetObjMarkSet(ObjMarkSet);
	}

	return TRUE;
}


void CHLDoc::OnEditDBCopy() 
{
	m_chain.GetSelTreeDoubleOutPara(m_DBCopyPara);
	return;
}

void CHLDoc::OnEditBDPaste() 
{
	CQuad quad = GetLimit();
	m_chain.SetAllSelTreeDoubleOutPara(m_DBCopyPara);
	
	CDot pos = quad.Center();
	double dbMoveX = m_DBCopyPara.pos.x - pos.x;
	double dbMoveY = m_DBCopyPara.pos.y - pos.y;
	Drag(CDot(0,0),1.0,1.0,dbMoveX,dbMoveY);
	SetTrackerQuad(GetLimit());

	memset(&m_DBCopyPara, 0, sizeof(DOUBLEOUTPARA));
	return;
}

void CHLDoc::OnUpdateMarkFast(CCmdUI* pCmdUI) 
{
//	pCmdUI->Enable(FALSE);
	pCmdUI->SetCheck(g_bIsFastMark);
}


CNode* CHLDoc::GetSelNode(FILLDATA &fill)
{
	CNode *pNode = m_chain.GetNode();
	if(pNode == NULL)
	{
		return NULL;
	}

	if(pNode->GetType() == NODE_TEXT_DSF)
	{
		ClearTextMem();
		m_chain.GetFillStyle(fill);

		return pNode;
	}

	if(pNode->GetType() == NODE_CODEBAR)
	{
		return pNode;
	}
	
	return NULL;
}

void CHLDoc::UpdateSelText(FILLDATA &fill)
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	pView->DispSelObjProp();

	if (NULL == m_pTracker)
	{
		m_pTracker = &(pView->m_track);
	}
	
	m_chain.SetFillStyle(fill,m_pTracker);
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
	UpdateAllViews(NULL);

	return;
}

BOOL CHLDoc::IsHaveSelTxetNode()
{
	CNode *pNode = m_chain.GetNode();
	if(pNode == NULL)
	{
		return FALSE;
	}

	if(pNode->GetType() == NODE_TEXT_DSF)
	{
		return TRUE;
	}

	return FALSE;
}

void CHLDoc::EditTextNode(CNode* pNode)
{
	if (NULL == pNode) 
	{
		return;
	}

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	if (NULL == m_pTracker)
	{
		m_pTracker = &(pView->m_track);
	}

	CNode *pGetNode = m_chain.GetNode();
	if(NULL == pGetNode)
	{
		return;
	}

	if(pGetNode->GetType() != NODE_TEXT_DSF)
	{
		return;
	}


	int nSelObjID = pNode->GetObjID();
    int nGetObjID = pGetNode->GetObjID();
	if (nSelObjID != nGetObjID)
	{
		return;
	}

	if(pNode->GetType() == NODE_TEXT_DSF)
	{
		ClearTextMem();
		//文本方式则在此处处理
		pNode->Create(CRect(0,0,0,0));
		FILLDATA fill;
		m_chain.GetFillStyle(fill);
		m_chain.SetFillStyle(fill,m_pTracker);
		SetModifiedFlag();
		SetTrackerQuad(m_chain.GetLimit());
		UpdateAllViews(NULL);	
	}

	pView->DispSelObjProp();

	return;
}

void CHLDoc::EditBarNode(CNode* pNode)
{
	if (NULL == pNode) 
	{
		return;
	}

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	if (NULL == m_pTracker)
	{
		m_pTracker = &(pView->m_track);
	}

	//条码设置在此处处理
	if(pNode->GetType() == NODE_CODEBAR)
	{
		//文本方式则在此处处理
		pNode->Create(CRect(0,0,0,0));
		//用于刷新文本内容
		m_chain.PreprocessMark(0x04);
		SetModifiedFlag();
		SetTrackerQuad(m_chain.GetLimit());
		UpdateAllViews(NULL);	
	}

	pView->DispSelObjProp();

	return;
}

void CHLDoc::UpdateSelBar()
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	if (NULL == m_pTracker)
	{
		m_pTracker = &(pView->m_track);
	}
	
	//用于刷新文本内容
	m_chain.PreprocessMark(0x04);
	SetModifiedFlag();
	SetTrackerQuad(m_chain.GetLimit());
	UpdateAllViews(NULL);

	pView->DispSelObjProp();

	return;
}

void CHLDoc::OnConvertOneTree() 
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	ConvertSelToOneTree();
	pView->DispSelObjProp();	

	return;
}


void CHLDoc::OnUpdateConvertOneTree(CCmdUI* pCmdUI) 
{
	if(m_nStatus==CChain::selectNull)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
	
	return;
}

void CHLDoc::OnUnion() 
{
	m_chain.UnionGroup();
	SetModifiedFlag();

	ReLoadStrokeList();
	SetTrackerQuad(m_chain.GetLimit());

	m_chain.RefreshSelectQuadList(m_QuadList);

}


void CHLDoc::OnUpdateUnion(CCmdUI* pCmdUI)
{
	//pCmdUI->Enable(m_nStatus & CChain::selectMul);
	
	if(m_nStatus&CChain::selectMul)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}

	return;
}


void CHLDoc::OnUnunion()
{
	m_chain.UnUnionGroup();
	SetModifiedFlag();

	ReLoadStrokeList();
	SetTrackerQuad(m_chain.GetLimit());
	m_chain.RefreshSelectQuadList(m_QuadList);
}


void CHLDoc::OnUpdateUnunion(CCmdUI* pCmdUI) 
{

	if( (m_nStatus & CChain::selectUnion))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	
}

CNode* CHLDoc::IsHaveSelBarNode()
{
	CNode *pNode = m_chain.GetNode();
	if(pNode == NULL)
	{
		return NULL;
	}

	if(NODE_CODEBAR == pNode->GetType())
	{
		return pNode;
	}

	return NULL;
}

CHLView* GetCurActiveView()
{
	CWnd *pWnd = AfxGetApp()->m_pMainWnd;
	if ( pWnd == NULL || !pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)) )
		return NULL;

	CHLView *pHView = NULL;
	CFrameWnd *pMainWnd = ((CFrameWnd*)pWnd)->GetActiveFrame();
	if ( pMainWnd == NULL )
		return NULL;

	CView *pView = pMainWnd->GetActiveView();
	if( NULL == pView || !pView->IsKindOf(RUNTIME_CLASS(CHLView)) )
		return NULL;

	pHView = (CHLView*) pView;

	return pHView;
}

//取Track
BOOL CHLDoc::CheckSetTrack()
{
	if( NULL == m_pTracker )
	{
		CHLView   *pView = GetCurActiveView();
		if( NULL == pView )
			return FALSE;

		m_pTracker = &(pView->m_track);
	}

	return TRUE;
}

void CHLDoc::OnZTest()
{
	// TODO: 在此添加命令处理程序代码
	CDlgZTest dlg;
	dlg.DoModal();
}


void CHLDoc::CustomSerialize(FILE* fp,BOOL bFlag)
{
	CMySerialize sa(fp);
	CString str;

	double dbTemp=0;
	int nTemp = 0;
	int nFocusTypeNum=0;
	CMarkFocus focus;

	if (bFlag)
	{
		sa<<"Custom_B";
		sa<<99;
		sa<<m_nRefPointNum;
		for (int i=0; i<m_nRefPointNum; i++)
		{
			sa<<m_arrDocRefPointX[i];
			sa<<m_arrDocRefPointY[i];
		}
		sa<<100;
		sa<<dbTemp;		//m_arrDocRefPointX;
		sa<<101;
		sa<<dbTemp;		//m_arrDocRefPointY;		
		sa<<102;		
		sa<<dbTemp;
		sa<<103;		
		sa<<dbTemp;	
		sa<<104;
		sa<<dbTemp;
		sa<<105;
		sa<<dbTemp;
		sa<<106;	
		sa<<dbTemp;
		sa<<107;	
		sa<<m_dbProductCCWAngle;
		sa<<108;	
		sa<<m_dbLPDrawX[0];
		sa<<109;
		sa<<m_dbLPDrawY[0];
		sa<<110;	
		sa<<m_dbLPDrawX[1];
		sa<<111;
		sa<<m_dbLPDrawY[1];	
		sa<<112;
		sa<<m_dbLPDrawX[2];
		sa<<113;
		sa<<m_dbLPDrawY[2];	
		sa<<114;
		sa<<m_dbLPDrawX[3];
		sa<<115;
		sa<<m_dbLPDrawY[3];
		sa<<116;
		sa<<dbTemp;
		sa<<117;
		sa<<dbTemp;
		sa<<118;
		sa<<dbTemp;
		sa<<119;
		sa<<m_dbHandOffsetX;
		sa<<120;
		sa<<m_dbHandOffsetY;
		sa<<121;
		sa<<m_dbHandOffsetCCWAngle;
		sa<<122;	
		sa<<m_dbDisMaxErr;
		sa<<123;
		sa<<m_dbAngleMaxErr;
		sa<<124;
		sa<<m_dbXAutoScaleAdd;
		sa<<125;
		sa<<m_dbYAutoScaleAdd;
		sa<<126;
		sa<<m_bEnableXFixSacle;
		sa<<127;
		sa<<m_bEnableYFixScale;
		sa<<128;
		sa<<m_dbFixXScale;
		sa<<129;
		sa<<m_dbFixYScale;
		sa<<130;
		sa<<m_strProgID1;
		sa<<131;
		sa<<m_strProgID2;
		sa<<132;
		sa<<m_strProgID3;
		sa<<133;
		sa<<m_nLight1;
		sa<<134;
		sa<<m_nLight2;
		sa<<135;		
		sa<<m_nLocateModel;
		sa<<136;
		sa<<m_nCCDUseModel;
		sa<<137;
		sa<<m_dbRowDist;
		sa<<138;
		sa<<m_dbColDist;
		sa<<139;
		sa<<m_dbShrinkRow;
		sa<<140;
		sa<<m_nWaferSize;
		sa<<141;
		sa<<m_dbBeamExpanderRatio;
		sa<<142;
		sa<<m_nBeamExpanderOffset;
		sa<<143;
		sa<<m_bEnableMarkFocus;
		sa<<144;
		sa<<m_dbZCCDFocus;
		sa<<146;
		sa<<m_nGasModel;
		sa<<150;
		nFocusTypeNum = m_arrMarkFocus.GetSize();
		sa<< nFocusTypeNum;
		for (int i=0; i<m_arrMarkFocus.GetSize(); i++)
		{	
			focus = m_arrMarkFocus.GetAt(i);
			sa<<focus.dbZPos;
			sa<<focus.nMoveNum;
			sa<<focus.dbMoveDis;
		}
		sa<<250;
		sa<<m_dbTestPowerFrequency;
		sa<<251;
		sa<<m_dbProductPowerMax;
		sa<<252;
		sa<<m_dbProductPowerMin;


		sa<<253;
		sa<<m_nLight3;
		sa<<254;
		sa<<m_nLight4;
		sa<<255;
		sa<<m_bUseFile;
		sa<<256;
		sa<<m_strFileDataPath;

		sa<<257;
		sa<<m_dMarkLay;

		sa<<258;
		sa<<dbTemp;
		sa<<259;
		sa<<dbTemp;
		sa<<260;
		sa<<dbTemp;
		sa<<261;
		sa<<dbTemp;
		sa<<262;
		sa<<nTemp;
		sa<<263;
		sa<<dbTemp;
		sa<<264;
		sa<<dbTemp;
		sa<<265;
		sa<<dbTemp;
		sa<<266;
		sa<<dbTemp;
		sa<<267;
		sa<<dbTemp;
		sa<<268;
		sa<<dbTemp;

		sa<<269;
		sa<<nTemp;
		sa<<270;
		sa<<nTemp;
		sa<<271;
		sa<<nTemp;
		sa<<272;
		sa<<nTemp;

		sa<<273;
		sa<<m_strProgNewID2;
		sa<<274;
		sa<<m_strProgNewID3;
		sa<<275;
		sa<<m_strProgNewID4;



		sa<<276;
		sa<<m_bScan;
		sa<<277;
		sa<<m_dbScanZpos;
		sa<<278;
		sa<<m_dbScanPtX;
		sa<<279;
		sa<<m_dbScanPtY;
		sa<<280;
		sa<<m_ScanPriNum;

		sa<<281;
		sa<<m_dbShrinkYCol;

		sa<<282;
		sa<<m_dbScaleLimit;

		sa<<"Custom_E";
	}
	else
	{
		while(sa>>str)
		{
			switch (atoi(str))
			{
			case 0:
				if (str=="Custom_E")
					goto CUSTOM_SERI_END;
			case 99:
				sa>>m_nRefPointNum;
				for (int i=0; i<m_nRefPointNum; i++)
				{
					sa>>m_arrDocRefPointX[i];
					sa>>m_arrDocRefPointY[i];
				}
				break;
			case 100:
				sa>>dbTemp;	//m_arrDocRefPointX;
				break;
			case 101:
				sa>>dbTemp;	//m_arrDocRefPointY;
				break;
			case 102:
				sa>>dbTemp;
				break;
			case 103:
				sa>>dbTemp;
				break;
			case 104:
				sa>>dbTemp;
				break;
			case 105:
				sa>>dbTemp;
				break;
			case 106:
				sa>>dbTemp;
				break;
			case 107:
				sa>>m_dbProductCCWAngle;
				break;
			case 108:
				sa>>m_dbLPDrawX[0];
				break;
			case 109:
				sa>>m_dbLPDrawY[0];
				break;
			case 110:
				sa>>m_dbLPDrawX[1];
				break;
			case 111:
				sa>>m_dbLPDrawY[1];
				break;
			case 112:
				sa>>m_dbLPDrawX[2];
				break;
			case 113:
				sa>>m_dbLPDrawY[2];
				break;
			case 114:
				sa>>m_dbLPDrawX[3];
				break;
			case 115:
				sa>>m_dbLPDrawY[3];
				break;
			case 116:
				sa>>dbTemp;
				break;
			case 117:
				sa>>dbTemp;
				break;
			case 118:
				sa>>dbTemp;
				break;
			case 119:
				sa>>m_dbHandOffsetX;
				break;
			case 120:
				sa>>m_dbHandOffsetY;
				break;
			case 121:
				sa>>m_dbHandOffsetCCWAngle;
				break;
			case 122:
				sa>>m_dbDisMaxErr;
				break;
			case 123:
				sa>>m_dbAngleMaxErr;
				break;
			case 124:
				sa>>m_dbXAutoScaleAdd;
				break;
			case 125:
				sa>>m_dbYAutoScaleAdd;
				break;
			case 126:
				sa>>m_bEnableXFixSacle;
				break;
			case 127:
				sa>>m_bEnableYFixScale;
				break;
			case 128:
				sa>>m_dbFixXScale;
				break;
			case 129:
				sa>>m_dbFixYScale;
				break;
			case 130:
				sa>>m_strProgID1;
				break;
			case 131:
				sa>>m_strProgID2;
				break;
			case 132:
				sa>>m_strProgID3;
				break;
			case 133:
				sa>>m_nLight1;
				break;
			case 134:
				sa>>m_nLight2;
				break;
			case 135:
				sa>>m_nLocateModel;
				break;
			case 136:
				sa>>m_nCCDUseModel;
				break;
			case 137:
				sa>>m_dbRowDist;
				break;
			case 138:
				sa>>m_dbColDist;
				break;
			case 139:
				sa>>m_dbShrinkRow;
				break;
			case 140:
				sa>>m_nWaferSize;
				break;
			case 141:
				sa>>m_dbBeamExpanderRatio;
				break;
			case 142:
				sa>>m_nBeamExpanderOffset;
				break;
			case 143:
				sa>>m_bEnableMarkFocus;
				break;
			case 144:
				sa>>m_dbZCCDFocus;
				break;
			case 146:
				sa>>m_nGasModel;
				break;
			case 150:
				sa>>nFocusTypeNum;
				m_arrMarkFocus.SetSize(nFocusTypeNum);
				for (int i=0; i<m_arrMarkFocus.GetSize(); i++)
				{	
					sa>>focus.dbZPos;
					sa>>focus.nMoveNum;
					sa>>focus.dbMoveDis;
					m_arrMarkFocus.SetAt(i, focus);
				}
				break;
			case 250:
				sa>>m_dbTestPowerFrequency;
				break;
			case 251:
				sa>>m_dbProductPowerMax;
				break;	
			case 252:
				sa>>m_dbProductPowerMin;
				break;
			case 253:
				sa>>m_nLight3;
				break;
			case 254:
				sa>>m_nLight4;
				break;

			case 255:
				sa>>m_bUseFile;
				break;
			case 256:
				sa>>m_strFileDataPath;
				break;

			case 257:
				sa>>m_dMarkLay;
				break;
			case 258:
				sa>>dbTemp;
				break;
			case 259:
				sa>>dbTemp;
				break;
			case 260:
				sa>>dbTemp;
				break;
			case 261:
				sa>>dbTemp;
				break;
			case 262:
				sa>>nTemp;
				break;
			case 263:
				sa>>dbTemp;
				break;
			case 264:
				sa>>dbTemp;
				break;
			case 265:
				sa>>dbTemp;
				break;
			case 266:
				sa>>dbTemp;
				break;
			case 267:
				sa>>dbTemp;
				break;
			case 268:
				sa>>dbTemp;
				break;

			case 269:
				sa>>nTemp;
				break;
			case 270:
				sa>>nTemp;
				break;
			case 271:
				sa>>nTemp;
				break;
			case 272:
				sa>>nTemp;
				break;
			case 273:
				sa>>m_strProgNewID2;
				break;

			case 274:
				sa>>m_strProgNewID3;
				break;
			case 275:
				sa>>m_strProgNewID4;
				break;


			case 276:
				sa>>m_bScan;
				break;
			case 277:
				sa>>m_dbScanZpos;
				break;
			case 278:
				sa>>m_dbScanPtX;
				break;
			case 279:
				sa>>m_dbScanPtY;
				break;
			case 280:
				sa>>m_ScanPriNum;
				break;
			case 281:
				sa>>m_dbShrinkYCol;
				break;
			case 282:
				sa>>m_dbScaleLimit;
				break;
			default:
				sa>>str;
			}
		}
CUSTOM_SERI_END:
		;
	}
}


void CHLDoc::CopyObjectListSerialize(FILE* fp,BOOL bFlag)
{
	CMySerialize sa(fp);

	int nVersion = 1;
	int nCount = m_chain.m_listCopyObject.GetCount();

	if (bFlag)
	{
		sa<<"CopyObjectList_B";
		sa<<nVersion;
		sa<<nCount;

		POSITION pos = m_chain.m_listCopyObject.GetHeadPosition();
		while(pos)
		{
			CChain::CCopyObject &obj = m_chain.m_listCopyObject.GetNext(pos);

			sa<<obj.m_quadLimit.left;
			sa<<obj.m_quadLimit.right;
			sa<<obj.m_quadLimit.top;
			sa<<obj.m_quadLimit.bottom;
		}		
	}
	else
	{
		CChain::CCopyObject obj;
		obj.m_bSelect = FALSE;
		obj.m_bMarked = FALSE;
		m_chain.m_listCopyObject.RemoveAll();

		sa>>nVersion;
		if (nVersion==1)
		{
			sa>>nCount;
			for(int i=0; i<nCount; i++)
			{
				sa>>obj.m_quadLimit.left;
				sa>>obj.m_quadLimit.right;
				sa>>obj.m_quadLimit.top;
				sa>>obj.m_quadLimit.bottom;
				m_chain.m_listCopyObject.AddTail(obj);
			}
		}
	}
}

void CHLDoc::ResetDivideFlag()
{
	m_chain.ResetDivideFlag();
}

void CHLDoc::GetStrokeInQuad(CQuad &quad, StrokeList &strokeList, BOOL bDivedeByStroke)
{
	m_chain.GetStrokeInQuad(quad, strokeList, bDivedeByStroke);
}

void CHLDoc::GuardCustomMark()
{
	char szErrInfo[512];
	try
	{
		CustomMark();
		return;
	}
	catch(CHLCCDException *e)
	{
		g_pDlgMarkNormalPane->m_nMarkError = 2;
		strcpy_s(szErrInfo, e->m_strMessage);
		e->Delete();

		g_IO.OUTPUT_CCD_LOCATE_ERROR(TRUE);
		Sleep(100);
		g_IO.OUTPUT_CCD_LOCATE_ERROR(FALSE);
	}
	catch(CHLException *e)
	{
		strcpy_s(szErrInfo, e->m_strMessage);
		e->Delete();
	}
	catch (CException* e)
	{
		e->GetErrorMessage(szErrInfo, 512);
		e->Delete();
	}

	WriteRunLog(szErrInfo);
	g_pDlgMarkNormalPane->SetLightError();
	g_pDlgMarkNormalPane->SetDlgItemText(IDC_STATIC_WARNING_INFO, szErrInfo);
}

void CHLDoc::CustomMark()
{
	WriteRunLog("Enter CustomMark Thead", eLogTime);

	//参数转换
	CHLApp* pApp=(CHLApp*)AfxGetApp();
	HWND hWndProgress = g_pDlgMarkNormalPane->m_progressMark.GetSafeHwnd();
	g_pDlgMarkNormalPane->SetDlgItemText(IDC_STATIC_WARNING_INFO, "");

	g_IO.OUTPUT_SOFT_OK(FALSE);

	double BoxX, BoxY;
	g_DAT.GetMarkRange(BoxX,BoxY);

	if (m_dMarkLay < 1)
	{
		m_dMarkLay = 1;
	}


	if (g_pDlgMarkNormalPane->m_nMarkType==MARK_MODEL_NORMAL_MARK)
	{
		//BOX
		CStrokeList strokeList;
		strokeList.m_pArrayMarkFocus = &m_arrMarkFocus;

		CQuad quad(CDot(0,0), BoxX, BoxY);
		ResetDivideFlag();

		if (g_sys.m_bUseOpcLaser)
		{
			SetStrokeLayFreq();
		}
		m_chain.GetStrokeInQuad(quad, strokeList.m_list, FALSE);
		if (!strokeList.IsEmpty())
		{
			WriteRunLog("CloseDoor", eLogTime);
			g_IO.OUTPUT_CLOSE_DOOR(TRUE);			
			Sleep(g_ParaIni.m_nCloseDoorTime);

			g_IO.OUTPUT_AIR_BLOW(TRUE);

			MarkDivideCell(hWndProgress, strokeList);
		}
	}
	else if (g_pDlgMarkNormalPane->m_nMarkType==MARK_MODEL_FRAM)
	{
		CTree treeTemp;
		CQuad quad(CDot(0,0), BoxX, BoxY);
		ResetDivideFlag();
		m_chain.GetStrokeFram(quad, treeTemp);

		if (!quad.Empty())
		{
			g_IO.OUTPUT_CLOSE_DOOR(TRUE);
			Sleep(g_ParaIni.m_nCloseDoorTime);

			while (TRUE)
			{
				if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0){
					break;
				}
				g_DAT.MarkStart(1);
				treeTemp.Mark(hWndProgress);
				g_DAT.MarkEnd();
			}
		}
	}
	else if (g_pDlgMarkNormalPane->m_nMarkType==MARK_MODEL_GRID_MARK)
	{
		FixedDivideMarkPoint(hWndProgress, 0);
	}
	else if (g_pDlgMarkNormalPane->m_nMarkType==MARK_MODEL_DIVIDE_NO_CCD)
	{
		CPrecisionCompensationPara para;
		CalCompensationPara(para, FALSE);

		g_IO.OUTPUT_AIR_BLOW(TRUE);

		DivideMark(hWndProgress, para);
	}
	else if (g_pDlgMarkNormalPane->m_nMarkType==MARK_MODEL_DIVIDE_MANUAL_CCD)
	{
		CPrecisionCompensationPara para;
		CalCompensationPara(para, TRUE);
		
		g_IO.OUTPUT_AIR_BLOW(TRUE);

		DivideMark(hWndProgress, para);
	}
	else
	{
		Setlight(m_nLight1,m_nLight2,m_nLight3,m_nLight4);

		WriteRunLog("Move Axis Z", eLogTime);
		g_pMotion->AMove(eMaskZ, m_dbZCCDFocus);
		
		WriteRunLog("CCDCapture", eLogTime);
		GetCCDCapture(g_pDlgMarkNormalPane->m_hWnd, m_nLocateModel);

		CPrecisionCompensationPara para;
		CalCompensationPara(para, TRUE);

		g_IO.OUTPUT_AIR_BLOW(TRUE);

		DivideMark(hWndProgress, para); 
	}
}

double CHLDoc::GetFirstMarkFocus()
{
	double dbZ = m_dbZCCDFocus;
	if (m_bEnableMarkFocus && m_arrMarkFocus.GetCount()>0)
		dbZ = m_arrMarkFocus.GetAt(0).dbZPos;

	return dbZ;
}

BOOL CHLDoc::MarkDivideCell(HWND hWnd, CStrokeList &strokeList)
{
	if (m_bEnableMarkFocus)
	{
		if (m_DivideLayer.m_nStrokeMarkOrder==0)
			strokeList.MarkByFocusType();
		else
			strokeList.MarkByStrokeOrder();
	}
	else
	{
		strokeList.MarkNormal();
	}
		
	return TRUE;
}

void CHLDoc::DivideMark(HWND hWnd, const CPrecisionCompensationPara &para)
{
	WriteRunLog("DivideMark", eLogTime);
	if (WaitForSingleObject(g_hEventMainThreadEmergency, 0)!=WAIT_OBJECT_0)
	{

		switch(m_DivideLayer.m_nDivideType)
		{
		case 0:
			DivideByCell(hWnd, para, TRUE);
			break;
		case 1:
			DivideByCell(hWnd, para, FALSE);
			break;
		case 2:
			DivideOneByOne(hWnd, para);
		}
	}
}


BOOL CHLDoc::CheckGraphicAndWorkbenchLimit(double dbRefPointX, double dbRefPointY, CString &strInfo)
{
	CQuad quadDoc = m_chain.GetMarkLimit();
	quadDoc.Inflate(0.001, 0.001);

	double dbAxisXMin,dbAxisXMax;
	g_pMotion->GetPara("AXIS_0", "Min", dbAxisXMin);
	g_pMotion->GetPara("AXIS_0", "Max", dbAxisXMax);
	
	CString strFormat;
	CDot dotQuadCenter;
	if (m_DivideLayer.m_nDivideType==2) //逐个打标
	{
		POSITION pos = m_chain.m_list.GetHeadPosition();
		while(pos)
		{
			CGroupObj *pGroupObj = m_chain.m_list.GetNext(pos);

			if (!pGroupObj->m_bNeedMark)
				continue;

			if (m_chain.m_bSelMark && !pGroupObj->m_bSelect)
				continue;

			dotQuadCenter = pGroupObj->GetLimit().GetBaseDot();

			double dbPosX = (dbRefPointX - g_ParaIni.m_dbMarkToCCD1X) - dotQuadCenter.x;
			double dbPosY = (dbRefPointY - g_ParaIni.m_dbMarkToCCD1Y) - dotQuadCenter.y; 	

			if (dbPosX < dbAxisXMin)
			{	
				strFormat.LoadString(IDS_HLDOC_LINE_4504);
				strInfo.Format(strFormat, dbPosX, dbPosY, dbAxisXMin, dbAxisXMax);
				return FALSE;
			}
			else if (dbPosX > dbAxisXMax)
			{
				strFormat.LoadString(IDS_HLDOC_LINE_4510);
				strInfo.Format(strFormat, dbPosX, dbPosY, dbAxisXMin, dbAxisXMax);
				return FALSE;
			}						
		}
	}
	else //切分
	{
		for (int i=0; i< m_DivideLayer.m_nRowCount; i++)
		{
			for (int j=0; j<m_DivideLayer.m_nColCount; j++)
			{
				CQuad &quadCell = m_DivideLayer.m_pArrayCell[i][j].quad;

				if (quadDoc.left<=quadCell.right && quadDoc.right>=quadCell.left &&
					quadDoc.top>=quadCell.bottom && quadDoc.bottom<=quadCell.top)
				{	
					dotQuadCenter = quadCell.GetBaseDot();

					double dbPosX = (dbRefPointX - g_ParaIni.m_dbMarkToCCD1X) - dotQuadCenter.x;
					double dbPosY = (dbRefPointY - g_ParaIni.m_dbMarkToCCD1Y) - dotQuadCenter.y; 

					if (dbPosX < dbAxisXMin)
					{
						double dbDivideOffsetX = quadCell.left - quadDoc.left + 1;
						strFormat.LoadString(IDS_HLDOC_LINE_4534);
						strInfo.Format(strFormat, dbPosX, dbPosY, dbAxisXMin, dbAxisXMax, dbDivideOffsetX);
						return FALSE;
					}
					else if (dbPosX > dbAxisXMax)
					{
						strFormat.LoadString(IDS_HLDOC_LINE_4539);
						double dbDivideOffsetX = -(quadCell.right - quadDoc.right + 1);
						strInfo.Format(strFormat, dbPosX, dbPosY, dbAxisXMin, dbAxisXMax, dbDivideOffsetX);
						return FALSE;
					}
				}
			}
		}

	}

	return TRUE;
}

void CHLDoc::DivideByCell(HWND hWnd, const CPrecisionCompensationPara &para, BOOL bDivedeByStroke)
{
	WriteRunLog("Enter DivideByCell", eLogTime);

	BOOL bOutSeven = FALSE;
	int nFirstMarkRow = 0;
	CQuad quadDoc = GetAllLimit();

	POSITION pos = GetFirstViewPosition();
	CView *pView = GetNextView(pos);

	CWnd *pWnd=CWnd::FromHandle(hWnd);
	CProgressCtrl *pCtrl=(CProgressCtrl *)pWnd;
	int nCount = m_DivideLayer.GetMarkCellCount(quadDoc, nFirstMarkRow);
	pCtrl->SetRange32(0, nCount);
	pCtrl->SetStep(1);
	pCtrl->SetPos(0);

	ResetDivideFlag();
	m_DivideLayer.SetMarkStartFlag();
	pView->Invalidate();
	
	CStrokeList strokeListCell;
	strokeListCell.m_pArrayMarkFocus = &m_arrMarkFocus;

	int nRowCount=0, nGirdCount=0;
	CQuad quadMark;
	CDot dotQuadCenter;
	BOOL bBreakRow = FALSE;

	double dbDocProGress;
	g_pMotion->GetPara("XY", "bDocProGress", dbDocProGress);
	BOOL bDocProGress = static_cast<BOOL>(dbDocProGress);

	WriteRunLog("Start to divide cell", eLogTime);
	for (int i=0; i<m_DivideLayer.m_nRowCount; i++)
	{
		if (bBreakRow)
			break;

		int nCurI = i;
		nRowCount++;
		for (int j=0; j<m_DivideLayer.m_nColCount; j++)
		{
			int nCurJ = ((nRowCount-nFirstMarkRow)%2) ? j : (m_DivideLayer.m_nColCount-j-1);

		//	if (bDocProGress)
			{
				if (m_DivideLayer.m_pArrayCell[nCurI][nCurJ].bNeedMark)
				{
					nGirdCount++;
					m_DivideLayer.m_pArrayCell[nCurI][nCurJ].nMarkStatus = 1;
					pView->Invalidate();
				}
				else
				{
					m_DivideLayer.m_pArrayCell[nCurI][nCurJ].nMarkStatus = 2;
					pView->Invalidate();
					continue;
				}

			}
						
			if (!m_chain.m_bSelMark || m_chain.m_nSelectMarkType!=1 || m_DivideLayer.m_pArrayCell[nCurI][nCurJ].bSel)
			{
				quadMark = m_DivideLayer.m_pArrayCell[nCurI][nCurJ].quad;
				dotQuadCenter = quadMark.GetBaseDot();

				quadMark.right  += m_DivideLayer.m_dbDivideAddX;//g_ParaIni.m_dbDivideAddX;
				quadMark.bottom -= m_DivideLayer.m_dbDivideAddY;//g_ParaIni.m_dbDivideAddY;			

				if (g_sys.m_bUseOpcLaser)
				{
					SetStrokeLayFreq();
				}

				WriteRunLog("GetStrokeInQuad",eLogTime);
				m_chain.GetStrokeInQuad(quadMark, strokeListCell.m_list, bDivedeByStroke);
				if (!strokeListCell.IsEmpty())
				{ 
					CString strTemp;
					strTemp.Format("当前区域中心点(%f, %f), 区域大小(%f, %f), 线条数(%d)", dotQuadCenter.x, dotQuadCenter.y, g_ParaIni.m_dbDivideX, g_ParaIni.m_dbDivideY, strokeListCell.m_list.GetCount());
					WriteRunLog(strTemp,eLogDebug);

					double dbPosX = (g_ParaIni.m_dbRefPointX - g_ParaIni.m_dbMarkToCCD1X) - dotQuadCenter.x;
					double dbPosY = (g_ParaIni.m_dbRefPointY - g_ParaIni.m_dbMarkToCCD1Y) - dotQuadCenter.y; 		
										
					if (m_bEnableMarkFocus)
					{
						int nFocusType = strokeListCell.m_list.GetHead()->m_nFocusType;
						double dbZ = strokeListCell.m_pArrayMarkFocus->GetAt(nFocusType).dbZPos;

						WriteRunLog("StartAMove eMaskZ", eLogTime);
						g_pMotion->AMove(eMaskZ, dbZ);
					}

					WriteRunLog("LnXY", eLogTime);
					g_pMotion->LnXY(dbPosX, dbPosY,eSpeedFast);

					WriteRunLog("MotionDelayForMark", eLogTime);
					Sleep(g_ParaIni.m_nMotionDelayForMark);

					double dbActualX = g_pMotion->GetPos(eMaskX);
					double dbActualY = g_pMotion->GetPos(eMaskY);
					double dbXDiff = dbActualX - dbPosX;
					double dbYDiff = dbActualY - dbPosY;
					if (fabs(dbXDiff)>g_sys.m_dbWorkbenchMaxPosErr || fabs(dbYDiff)>g_sys.m_dbWorkbenchMaxPosErr)
					{
						CString strFormat, strError;
						strFormat.LoadString(IDS_HLDOC_LINE_4682);
						strError.Format(strFormat, dbPosX, dbPosY, dbActualX, dbActualY, g_sys.m_dbWorkbenchMaxPosErr);
						WriteRunLog(strError);

						g_pDlgMarkNormalPane->SetDlgItemText(IDC_STATIC_WARNING_INFO, strError);
						g_pDlgMarkNormalPane->SetLightError();	

						bBreakRow = TRUE;
						break;					
					}

					if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
					{
						WriteRunLog("人工中止打标!",eLogDataFlow);
						bBreakRow = TRUE;
						break;
					}
					if (!bOutSeven)
					{
						WriteRunLog("CloseDoor", eLogTime);
						g_IO.OUTPUT_CLOSE_DOOR(TRUE);
						Sleep(g_ParaIni.m_nCloseDoorTime);

						bOutSeven = TRUE;
					}

					WriteRunLog("move strokeList",  eLogTime);
					strokeListCell.Scale(para.dotBase, para.dbScaleX, para.dbScaleY);		
					strokeListCell.Rotate(para.dotBase, para.dbOffsetCCW);
					strokeListCell.Move(-dotQuadCenter.x+para.dbOffsetX, -dotQuadCenter.y+para.dbOffsetY);			

					for (int nLayi = 0; nLayi < m_dMarkLay; nLayi++)
					{
						if (!MarkDivideCell(hWnd, strokeListCell))
						{
							bBreakRow = TRUE;
							break;
						}
					}

					if (g_sys.m_nLaserCurrentKeep)
					{
						WriteRunLog("set keep current", eLogTime);
						g_DAT.SetCurrent(g_sys.m_nLaserCurrentKeep);
					}
				}
			}
			pCtrl->SetPos(nGirdCount);
		//	if (bDocProGress)
			{
				m_DivideLayer.m_pArrayCell[nCurI][nCurJ].nMarkStatus = 2;
				pView->Invalidate();
			}
			
			Sleep(10);	//显示最后一个切分区域完成状态

			if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
			{
				pCtrl->SetPos(0);
				WriteRunLog("人工中止打标!", eLogDataFlow);
				bBreakRow = TRUE;
				break;
			}
		}
	}
	m_DivideLayer.SetMarkEndFlag();
	pView->Invalidate();

	if (g_sys.m_nLaserCurrentKeep)
	{
		WriteRunLog("set keep current", eLogTime);
		g_DAT.SetCurrent(g_sys.m_nLaserCurrentKeep);
	}
}


void CHLDoc::DivideOneByOne(HWND hWnd, const CPrecisionCompensationPara &para)
{
	WriteRunLog("DivideOneByOne", eLogTime);

	BOOL bOutSeven = FALSE;
	
	CWnd *pWnd=CWnd::FromHandle(hWnd);
	CProgressCtrl *pCtrl=(CProgressCtrl *)pWnd;
	int nCount = m_chain.m_list.GetCount();
	pCtrl->SetRange32(0, nCount);
	pCtrl->SetStep(1);
	pCtrl->SetPos(0);

	ResetDivideFlag();
		
	CStrokeList strokeListCell;
	strokeListCell.m_pArrayMarkFocus = &m_arrMarkFocus;
		
	CQuad quadMark;
	CDot dotQuadCenter;


	BOOL bBreakRow = FALSE;
	
	int nGroupObjCount=0;
	POSITION pos = m_chain.m_list.GetHeadPosition();
	while(pos)
	{		

		if (bBreakRow)
			break;

		nGroupObjCount++;
		CGroupObj *pGroupObj = m_chain.m_list.GetNext(pos);

		if (!pGroupObj->m_bNeedMark)
			continue;

		if (m_chain.m_bSelMark && !pGroupObj->m_bSelect)
		{
			pCtrl->SetPos(nGroupObjCount);
			continue;
		}

		quadMark = pGroupObj->GetLimit();
		dotQuadCenter = quadMark.GetBaseDot();
		strokeListCell.Empty();

		if (g_sys.m_bUseOpcLaser)
		{
			SetStrokeLayFreq();
		}
		CQuad quadEmpty;
		pGroupObj->GetStrokeInQuad(quadEmpty, strokeListCell.m_list);

		CString strTemp;
		strTemp.Format("当前是第%d个对象，区域中心点(%f, %f), 区域大小(%f, %f), 线条数(%d)", nGroupObjCount, dotQuadCenter.x, dotQuadCenter.y, quadMark.GetWid(), quadMark.Height(), strokeListCell.m_list.GetCount());
		WriteRunLog(strTemp,eLogDebug);

		if (!strokeListCell.IsEmpty())
		{ 
			double dbPosX = (g_ParaIni.m_dbRefPointX - g_ParaIni.m_dbMarkToCCD1X) - dotQuadCenter.x;
			double dbPosY = (g_ParaIni.m_dbRefPointY - g_ParaIni.m_dbMarkToCCD1Y) - dotQuadCenter.y; 	
			
			if (m_bEnableMarkFocus)
			{
				int nFocusType = strokeListCell.m_list.GetHead()->m_nFocusType;
				double dbZ = strokeListCell.m_pArrayMarkFocus->GetAt(nFocusType).dbZPos;

				g_pMotion->StartAMove(eMaskZ, dbZ);
			}

			g_pMotion->LnXY(dbPosX, dbPosY,eSpeedFast);
			Sleep(g_ParaIni.m_nMotionDelayForMark);

			double dbActualX = g_pMotion->GetPos(eMaskX);
			double dbActualY = g_pMotion->GetPos(eMaskY);
			double dbXDiff = dbActualX - dbPosX;
			double dbYDiff = dbActualY - dbPosY;
			if (fabs(dbXDiff)>g_sys.m_dbWorkbenchMaxPosErr || fabs(dbYDiff)>g_sys.m_dbWorkbenchMaxPosErr)
			{
				CString strError;
				strError.Format("工作台移动出错，目标位置(%.3f, %.3f)，实际位置(%.3f,%.3f)，超出误差许可(%.3f), 打标中止!", dbPosX, dbPosY, dbActualX, dbActualY, g_sys.m_dbWorkbenchMaxPosErr);
				WriteRunLog(strError);

				g_pDlgMarkNormalPane->SetDlgItemText(IDC_STATIC_WARNING_INFO, strError);
				g_pDlgMarkNormalPane->SetLightError();	

				bBreakRow = TRUE;
				break;					
			}

			if (!bOutSeven)
			{
				g_IO.OUTPUT_CLOSE_DOOR(TRUE);
				Sleep(g_ParaIni.m_nCloseDoorTime);

				bOutSeven = TRUE;
			}
			strokeListCell.Scale(para.dotBase, para.dbScaleX, para.dbScaleY);
			strokeListCell.Rotate(para.dotBase, para.dbOffsetCCW);
			strokeListCell.Move(-dotQuadCenter.x+para.dbOffsetX, -dotQuadCenter.y+para.dbOffsetY);
			
			for (int nLayi = 0; nLayi < m_dMarkLay; nLayi++)
			{
				if (!MarkDivideCell(hWnd, strokeListCell))
				{
					bBreakRow = TRUE;
					break;
				}
			}
			if (g_sys.m_nLaserCurrentKeep)
				g_DAT.SetCurrent(g_sys.m_nLaserCurrentKeep);
		}
		pCtrl->SetPos(nGroupObjCount);
		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
		{
			pCtrl->SetPos(0);
			WriteRunLog("人工中止打标!", eLogDataFlow);
			bBreakRow = TRUE;
			break;
		}
	}

	if (g_sys.m_nLaserCurrentKeep)
		g_DAT.SetCurrent(g_sys.m_nLaserCurrentKeep);
}

void CHLDoc::FixedDivideMarkPoint(HWND hWnd, int repeat)
{
	CGroupObj *pGoupObj=NULL, *pMarkGroupObj=NULL;
	pGoupObj = m_chain.m_list.GetHead();
	if (pGoupObj) 
	{
		pMarkGroupObj = new CGroupObj(pGoupObj);
		CDot dotCenter = pMarkGroupObj->GetLimit().GetBaseDot();
		pMarkGroupObj->Move(CENTER-dotCenter.x, CENTER-dotCenter.y);		
		pMarkGroupObj->PreprocessMark(0);
	}
	
	int nCount = g_pCCDGridData->m_nColNum*g_pCCDGridData->m_nRowNum;
	CGridData *pGridData = g_pCCDGridData;

	CWnd *pWnd=CWnd::FromHandle(hWnd);
	CProgressCtrl *pCtrl=(CProgressCtrl *)pWnd;
	pCtrl->SetStep(nCount);
	pCtrl->SetPos(0);
	
	BOOL bBreakRow = FALSE;
	for (int i=0; i<pGridData->m_nRowNum; i++)
	{
		if (bBreakRow)
			break;


		for (int j=0; j<pGridData->m_nColNum; j++)
		{
			int nCurI = i;
			int nCurJ = (i%2) ? (pGridData->m_nColNum-j-1) : j;
			
			double dbCurSoftX = pGridData->m_dotsX[nCurI][nCurJ];
			double dbCurSoftY = pGridData->m_dotsY[nCurI][nCurJ];
			WBGC_HardToSoft(dbCurSoftX, dbCurSoftY);					

			g_pMotion->LnXY(dbCurSoftX, dbCurSoftY,eSpeedFast);
			Sleep(g_ParaIni.m_nMotionDelayForMark);
			
			g_DAT.MarkStart();
			pMarkGroupObj->Mark(hWnd);
			g_DAT.MarkEnd();
			
			pCtrl->SetPos(i*pGridData->m_nColNum+j);
			
			if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
			{
				pCtrl->SetPos(0);
				WriteRunLog("人工中止打标!", eLogDataFlow);
				return;
			}
		}
	}
	
	if (pMarkGroupObj)
	{
		delete pMarkGroupObj;
		pMarkGroupObj=NULL;
	}
}

int CHLDoc::GetLocatePointsNum()
{
	int nPointNum = 0;

	switch (m_nLocateModel)
	{
	case LOCATE_MODEL_2_POINTS:
		nPointNum = 2;
		break;
	case LOCATE_MODEL_3_POINTS:
		nPointNum = 3;
		break;
	case LOCATE_MODEL_4_POINTS:
		nPointNum = 4;
		break;
	}
	return nPointNum;
}

//Exception: CHLCCDException
void CHLDoc::CalCompensationPara(CPrecisionCompensationPara &para, BOOL bCCD)
{
	if (bCCD)
	{
		CalZoom(para.dbScaleX, para.dbScaleY);

		CalOffset(para.dotBase, para.dbOffsetX, para.dbOffsetY, para.dbOffsetCCW);
	}
			
	para.dbOffsetX += m_dbHandOffsetX;
	para.dbOffsetY += m_dbHandOffsetY;
	para.dbOffsetCCW += m_dbHandOffsetCCWAngle;
}

//Exception: CHLCCDException
void CHLDoc::CalOffset(CDot &dotBase, double &dbOffsetX, double &dbOffsetY, double &dbOffsetCCW)
{
	CDot dotDrawPosOfLocatePoints[4];
	CDot dotTruePosOfLocatePoints[4];
	CDot dotLogicPosOfLocatePoints[4];//LP在CCD中心时，理论工作台位置
	for (int i=0; i<4; i++)
	{
		if (i==GetLocatePointsNum())
			break;

		dotDrawPosOfLocatePoints[i].x = m_dbLPDrawX[i];
		dotDrawPosOfLocatePoints[i].y = m_dbLPDrawY[i];	

		dotTruePosOfLocatePoints[i].x = m_dbLPCCDTrueX[i];
		dotTruePosOfLocatePoints[i].y = m_dbLPCCDTrueY[i];

		CalculateLPLogicPos(i, CCD_MODEL_CCD1, dotLogicPosOfLocatePoints[i].x, dotLogicPosOfLocatePoints[i].y);

		CString strTip;
		strTip.Format("定位点%d的坐标(%f, %f), (%f, %f)", i+1, dotLogicPosOfLocatePoints[i].x, dotLogicPosOfLocatePoints[i].y, dotTruePosOfLocatePoints[i].x, dotTruePosOfLocatePoints[i].y);
		WriteRunLog(strTip, eLogDebug);
	}

	if (GetLocatePointsNum()==4)
	{
		CalculateCenterOf4Points(dotDrawPosOfLocatePoints, dotBase);

		CDot dotTrueCenter, dotLogicCenter;
		CalculateCenterOf4Points(dotTruePosOfLocatePoints, dotTrueCenter);
		CalculateCenterOf4Points(dotLogicPosOfLocatePoints, dotLogicCenter);
		dbOffsetX =  dotLogicCenter.x - dotTrueCenter.x;
		dbOffsetY =  dotLogicCenter.y - dotTrueCenter.y;
	}
	else
	{
		dotBase.x = m_dbLPDrawX[0];
		dotBase.y = m_dbLPDrawY[0];

		dbOffsetX =  dotLogicPosOfLocatePoints[0].x - dotTruePosOfLocatePoints[0].x;
		dbOffsetY =  dotLogicPosOfLocatePoints[0].y - dotTruePosOfLocatePoints[0].y;
	}

	if (fabs(m_dbLPDrawX[1]-m_dbLPDrawX[0]) > fabs(m_dbLPDrawY[1]-m_dbLPDrawY[0])) //水平Mark点
		dbOffsetCCW = (atan((dotTruePosOfLocatePoints[1].y - dotTruePosOfLocatePoints[0].y)/(dotTruePosOfLocatePoints[1].x - dotTruePosOfLocatePoints[0].x)) - atan((m_dbLPDrawY[1]-m_dbLPDrawY[0])/(m_dbLPDrawX[1]-m_dbLPDrawX[0])))/PAI*180.0;
	else  //垂平Mark点
		dbOffsetCCW = (atan((m_dbLPDrawX[1]-m_dbLPDrawX[0])/(m_dbLPDrawY[1]-m_dbLPDrawY[0])) - atan((dotTruePosOfLocatePoints[1].x - dotTruePosOfLocatePoints[0].x)/(dotTruePosOfLocatePoints[1].y - dotTruePosOfLocatePoints[0].y)))/PAI*180.0;

	CString strTemp;
	strTemp.Format("产品位置偏移(%f,%f)，逆时针角度偏移(%f)", dbOffsetX, dbOffsetY, dbOffsetCCW);
	WriteRunLog(strTemp, eLogDataFlow);

	if(fabs(dbOffsetX)>m_dbDisMaxErr || fabs(dbOffsetY)>m_dbDisMaxErr)
	{
		strTemp.Format("XY Offset(%.3f,%.3f) exceeds the limit(%.3f)!", dbOffsetX, dbOffsetY, m_dbDisMaxErr);
		throw new CHLCCDException(strTemp);
	}

	if(fabs(dbOffsetCCW)>m_dbAngleMaxErr)
	{
		strTemp.Format("Angle Offset(%.3f) exceeds the limit(%.3f)!", dbOffsetCCW, m_dbAngleMaxErr);
		throw new CHLCCDException(strTemp);
	}
}

//Exception: CHLCCDException
void CHLDoc::CalculateCenterOf4Points(CDot dots[4], CDot &dotCenter)
{
	CDot dotCenterOfLeft   = CDot::Center(dots[0], dots[3]);
	CDot dotCenterOfRight  = CDot::Center(dots[1], dots[2]);
	CDot dotCenterOfTop    = CDot::Center(dots[0], dots[1]);
	CDot dotCenterOfBottom = CDot::Center(dots[2], dots[3]);

	BOOL bValid = FALSE;
	CStroke::CalCross(dotCenterOfLeft, dotCenterOfRight, dotCenterOfTop, dotCenterOfBottom, dotCenter, bValid);
	if (!bValid)
		throw new CHLCCDException("Failed to CalCross");
};

//Exception: CHLCCDException
BOOL CHLDoc::GetCCDCapture(HWND hWnd, int nLocateModel)
{	
	CString strTip;
	char szName[4][32] = {"左定位点", "右定位点", "上定位点", "下定位点"};

	CString strTemp = m_strProgID1;

	g_ccd.SetWorkProgNo(atoi(m_strProgID1));
	//Sleep(1000);

	int nPointsNum = GetLocatePointsNum();
	
	for (int i=0; i<nPointsNum; i++)
	{
		strTip.Format("CCD开始定位“%s”(%f, %f)", szName[i], m_dbLPDrawX[i], m_dbLPDrawY[i]);
		WriteRunLog(strTip, eLogTime);

		switch(i)
		{
			case 1:
				{
					if (strTemp != m_strProgNewID2 && -1 != atoi(m_strProgNewID2))
					{
						g_ccd.SetWorkProgNo(atoi(m_strProgNewID2));
						//Sleep(500);
						strTemp = m_strProgNewID2;
					}
				}
				break;
			case 2:
				{
					if (strTemp != m_strProgNewID3 && -1 != atoi(m_strProgNewID3))
					{
						g_ccd.SetWorkProgNo(atoi(m_strProgNewID3));
						//Sleep(500);
						strTemp = m_strProgNewID3;
					}
				}
				break;
			case 3:
				{
					if (strTemp != m_strProgNewID4 && -1 != atoi(m_strProgNewID4))
					{
						g_ccd.SetWorkProgNo(atoi(m_strProgNewID4));
						//Sleep(500);
					}
				}
				break;
		}


		BOOL bCCDOK = PointPrecisionCapture(hWnd, i);
		if (!bCCDOK && !g_sys.m_bPLCOnline && g_sys.m_bHandPosOnCCDFail)
		{
			if (g_pDlgMarkNormalPane->SendMessage(WM_SHOW_MARKING_INFO_BOX, 1))
			{
				bCCDOK = TRUE;
				m_dbLPCCDTrueX[i] = g_pMotion->GetPos(eMaskX);
				m_dbLPCCDTrueY[i] = g_pMotion->GetPos(eMaskY);
			}
		}
		if (!bCCDOK)
			throw new CHLCCDException("CCD positioning failed!");

		strTip.Format("“%s”定位成功!定位后工作台坐标(%f, %f)", szName[i], m_dbLPCCDTrueX[i], m_dbLPCCDTrueY[i]);
		WriteRunLog(strTip,eLogTime);
		
		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
		{
			WriteRunLog("人工中止定位!", eLogDataFlow);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CHLDoc::NoTicpsPointCCD1Capture(double &dbCaptureX, double &dbCaptureY,BOOL bOffset)
{
	CString strTip;
	double err;

	try	
	{	
		BOOL bCapture = g_ccd.CCDLocate();		

		if (!g_ParaIni.m_bUseQPosite)
		{
			for(int i=0; i<5; i++)
			{ 
				err=g_ccd.GetCaptPrecision(TRUE);
				if (!bCapture || err<g_ParaIni.m_dbCCD1Precision)	//如果捕获失败，或者“捕获成功且达到指定精度”
					break;

				g_ccd.MoveToCCDCenter(TRUE, dbCaptureX, dbCaptureY);			
				g_pMotion->LnXY(dbCaptureX, dbCaptureY,eSpeedFast);
				Sleep(g_ParaIni.m_nMotionDelayForCCD);

				bCapture=g_ccd.CCDLocate(); 
			}
		}
		else
		{
			g_ccd.MoveToCCDCenter(TRUE, dbCaptureX, dbCaptureY);	
		}
		if (!bCapture) throw 4;
		if (err>=g_ParaIni.m_dbCCD1Precision) throw 5;
	}
	catch (int e)
	{
		switch (e)
		{
		case 4:
			strTip.Format("CCD1捕获失败!");
			break;
		case 5:
			strTip.Format("CCD1定位无法达到指定精度(%f)，当前精度(%f)!",g_ParaIni.m_dbCCD1Precision,err);
			break;
		}

		WriteRunLog(strTip, eLogDataFlow);

		return FALSE;
	}	
	return TRUE;

}

BOOL CHLDoc::PointCCD1Capture(double &dbCaptureX, double &dbCaptureY,BOOL bOffset)
{
	CString strTip;
	double err;
	
	try	
	{	
		BOOL bCapture = g_ccd.CCDLocate();	

		if (!g_ParaIni.m_bUseQPosite || bOffset)
		{
			for(int i=0; i<5; i++)
			{ 
				err=g_ccd.GetCaptPrecision(TRUE);
				if (!bCapture || err<g_ParaIni.m_dbCCD1Precision)	//如果捕获失败，或者“捕获成功且达到指定精度”
					break;

				g_ccd.MoveToCCDCenter(TRUE, dbCaptureX, dbCaptureY);			
				g_pMotion->LnXY(dbCaptureX, dbCaptureY,eSpeedFast);
				Sleep(g_ParaIni.m_nMotionDelayForCCD);


				bCapture=g_ccd.CCDLocate(); 
			}

		}
		else
		{
			g_ccd.MoveToCCDCenter(TRUE, dbCaptureX, dbCaptureY);	
		}
		
		if (!bCapture) throw 4;
		if (err>=g_ParaIni.m_dbCCD1Precision) throw 5;
	}
	catch (int e)
	{
		switch (e)
		{
		case 4:
			strTip.Format("CCD1捕获失败!");
			break;
		case 5:
			strTip.Format("CCD1定位无法达到指定精度(%f)，当前精度(%f)!",g_ParaIni.m_dbCCD1Precision,err);
			break;
		}
		
		WriteRunLog(strTip,eLogDataFlow);
		return FALSE;
	}	
	return TRUE;
}

BOOL CHLDoc::PointCCD2Capture(double &dbCaptureX, double &dbCaptureY)
{
	CString strTip;
	double err;
	
	try	
	{	
		BOOL bCapture = g_ccd.CCDLocate();	

		if (!g_ParaIni.m_bUseQPosite)
		{
			for(int i=0; i<5; i++)
			{ 
				err=g_ccd.GetCaptPrecision(FALSE);
				if (!bCapture || err<g_ParaIni.m_dbCCD2Precision)	//如果捕获失败，或者“捕获成功且达到指定精度”
					break;

				g_ccd.MoveToCCDCenter(FALSE, dbCaptureX, dbCaptureY);			
				g_pMotion->LnXY(dbCaptureX, dbCaptureY,eSpeedFast);
				Sleep(g_ParaIni.m_nMotionDelayForCCD);


				bCapture=g_ccd.CCDLocate(); 
			}
		}
		else
		{
			g_ccd.MoveToCCDCenter(TRUE, dbCaptureX, dbCaptureY);	
		}
		
		if (!bCapture) throw 4;
		if (err>=g_ParaIni.m_dbCCD2Precision) throw 5;
	}
	catch (int e)
	{
		switch (e)
		{
		case 4:
			strTip.Format("CCD2捕获失败!");
			break;
		case 5:
			strTip.Format("CCD2定位无法达到指定精度(%f)，当前精度(%f)!",g_ParaIni.m_dbCCD2Precision,err);
			break;
		}
		
		WriteRunLog(strTip, eLogDataFlow);


		g_IO.OUTPUT_CCD_LOCATE_ERROR(TRUE);
		Sleep(100);
		g_IO.OUTPUT_CCD_LOCATE_ERROR(FALSE);

		double dbPosTicps;
		g_pMotion->GetPara("XY", "PosTicps", dbPosTicps);
		BOOL bPos = (BOOL)dbPosTicps;
		if (bPos)
		{
			g_IO.OUTPUT_SOFT_OK(FALSE);
			AfxMessageBox(strTip);
			g_IO.OUTPUT_SOFT_OK(TRUE);
		}
		return FALSE;
	}	
	return TRUE;
}

BOOL CHLDoc::PointCCD2AssistCapture(double &dbCaptureX, double &dbCaptureY)
{
	int nCCDNo=0;

	try
	{
		g_pMotion->LnXY(dbCaptureX, dbCaptureY,eSpeedFast);
		Sleep(g_ParaIni.m_nMotionDelayForCCD);
		
		
		nCCDNo = atoi(m_strProgID2);
		if (!g_ccd.SetWorkProgNo(nCCDNo)) throw 1;
		if (!g_ccd.CCDLocate()) throw 2;
		
		g_ccd.CCD2ToCCD1Center(dbCaptureX, dbCaptureY);
		g_pMotion->LnXY(dbCaptureX, dbCaptureY,eSpeedFast);
		Sleep(g_ParaIni.m_nMotionDelayForCCD);
		
		
		nCCDNo = atoi(m_strProgID1);
		if (!g_ccd.SetWorkProgNo(nCCDNo)) throw 3;
	}

	catch (int e)
	{
		CString strTip;

		switch (e)
		{
		case 1:
			strTip.Format("切换至CCD2模式失败, CCDNO=%d", nCCDNo);
			break;
		case 2:
			strTip.Format("CCD2定位失败!");
			break;
		case 3:
			strTip.Format("切换至CCD1模式失败, CCDNO=%d", nCCDNo);
			break;
		}
		

		WriteRunLog(strTip,eLogDataFlow);

		g_IO.OUTPUT_CCD_LOCATE_ERROR(TRUE);
		Sleep(100);
		g_IO.OUTPUT_CCD_LOCATE_ERROR(FALSE);

		double dbPosTicps;
		g_pMotion->GetPara("XY", "PosTicps", dbPosTicps);
		BOOL bPos = (BOOL)dbPosTicps;
		if (bPos)
		{
			g_IO.OUTPUT_SOFT_OK(FALSE);
			AfxMessageBox(strTip);
			g_IO.OUTPUT_SOFT_OK(TRUE);
		}
		return FALSE;
	}
	return TRUE;
}

void CHLDoc::CalculateLPLogicPos(int nPointIndex, int nCCDUseModel, double& dbLogicX, double& dbLogicY)
{
	switch(nCCDUseModel)
	{
	case CCD_MODEL_CCD1:
	case CCD_MODEL_CCD2_CCD1:
		dbLogicX = g_ParaIni.m_dbRefPointX - m_dbLPDrawX[nPointIndex];
		dbLogicY = g_ParaIni.m_dbRefPointY - m_dbLPDrawY[nPointIndex];
		break;
	case CCD_MODEL_CCD2:
		dbLogicX = (g_ParaIni.m_dbRefPointX - m_dbLPDrawX[nPointIndex]) + (g_ParaIni.m_dbMarkToCCD2X-g_ParaIni.m_dbMarkToCCD1X);
		dbLogicY = (g_ParaIni.m_dbRefPointY - m_dbLPDrawY[nPointIndex]) + (g_ParaIni.m_dbMarkToCCD2Y-g_ParaIni.m_dbMarkToCCD1Y);
		break;
	}	
}

void CHLDoc::CalculateLPCCDInitPos(int nPointIndex, int nCCDUseModel, double& dbCCDInitX, double& dbCCDInitY)
{
	CDot dotLPDrawModify(m_dbLPDrawX[nPointIndex], m_dbLPDrawY[nPointIndex]);
	dotLPDrawModify.Rotate(CDot(0,0), g_ParaIni.m_dbWorkbenchFixtureAngle);	

	switch(nCCDUseModel)
	{
	case CCD_MODEL_CCD1:
		dbCCDInitX = g_ParaIni.m_dbRefPointX - dotLPDrawModify.x;
		dbCCDInitY = g_ParaIni.m_dbRefPointY - dotLPDrawModify.y;
		break;
	case CCD_MODEL_CCD2_CCD1:
	case CCD_MODEL_CCD2:
		dbCCDInitX = (g_ParaIni.m_dbRefPointX - dotLPDrawModify.x) + (g_ParaIni.m_dbMarkToCCD2X-g_ParaIni.m_dbMarkToCCD1X);
		dbCCDInitY = (g_ParaIni.m_dbRefPointY - dotLPDrawModify.y) + (g_ParaIni.m_dbMarkToCCD2Y-g_ParaIni.m_dbMarkToCCD1Y);
		break;
	}
}

BOOL CHLDoc::PointPrecisionCapture(HWND hWnd, int nPointIndex)
{
	if (m_nCCDUseModel==CCD_MODEL_CCD1)
	{
		CalculateLPCCDInitPos(nPointIndex, CCD_MODEL_CCD1, m_dbLPCCDTrueX[nPointIndex], m_dbLPCCDTrueY[nPointIndex]);


		WriteRunLog("CCD开始运动到定位点", eLogTime);
		g_pMotion->LnXY(m_dbLPCCDTrueX[nPointIndex], m_dbLPCCDTrueY[nPointIndex], eSpeedFast);
		Sleep(g_ParaIni.m_nMotionDelayForCCD);

		WriteRunLog("CCD运动后捕捉定位点，发送拍照指令", eLogTime);
		return PointCCD1Capture(m_dbLPCCDTrueX[nPointIndex], m_dbLPCCDTrueY[nPointIndex]);

	}
	else if(m_nCCDUseModel==CCD_MODEL_CCD2_CCD1)
	{
		CalculateLPCCDInitPos(nPointIndex, CCD_MODEL_CCD2_CCD1, m_dbLPCCDTrueX[nPointIndex], m_dbLPCCDTrueY[nPointIndex]);

		if (!PointCCD2AssistCapture(m_dbLPCCDTrueX[nPointIndex], m_dbLPCCDTrueY[nPointIndex]))
			return FALSE;
		
		return PointCCD1Capture(m_dbLPCCDTrueX[nPointIndex], m_dbLPCCDTrueY[nPointIndex]);
	}
	else //CCD_MODEL_CCD2
	{
		CalculateLPCCDInitPos(nPointIndex, CCD_MODEL_CCD2, m_dbLPCCDTrueX[nPointIndex], m_dbLPCCDTrueY[nPointIndex]);

		WriteRunLog("CCD开始运动到定位点", eLogTime);
		g_pMotion->LnXY(m_dbLPCCDTrueX[nPointIndex], m_dbLPCCDTrueY[nPointIndex],eSpeedFast);
		Sleep(g_ParaIni.m_nMotionDelayForCCD);
		WriteRunLog("CCD运动后捕捉定位点，发送拍照指令", eLogTime);
		return PointCCD2Capture(m_dbLPCCDTrueX[nPointIndex], m_dbLPCCDTrueY[nPointIndex]);
	}
	return TRUE;
}

//Exception: CHLCCDException
void CHLDoc::CalZoom(double &dbZoomX, double &dbZoomY)
{
	//验证定位点间距，并计算胀缩比
	CDot dotOrg1(m_dbLPDrawX[0], m_dbLPDrawY[0]);
	CDot dotOrg2(m_dbLPDrawX[1], m_dbLPDrawY[1]);
	CDot dotOrg3(m_dbLPDrawX[2], m_dbLPDrawY[2]);
	CDot dotOrg4(m_dbLPDrawX[3], m_dbLPDrawY[3]);
	CDot dot1(m_dbLPCCDTrueX[0], m_dbLPCCDTrueY[0]);
	CDot dot2(m_dbLPCCDTrueX[1], m_dbLPCCDTrueY[1]);
	CDot dot3(m_dbLPCCDTrueX[2], m_dbLPCCDTrueY[2]);
	CDot dot4(m_dbLPCCDTrueX[3], m_dbLPCCDTrueY[3]);

	double dbDistOrg1=1, dbDistOrg2=1, dbDist1=1, dbDist2=1;
	if (m_nLocateModel == LOCATE_MODEL_2_POINTS)
	{
		dbDistOrg1 = dotOrg1.DistanceFromDot(dotOrg2);
		dbDist1 = dot1.DistanceFromDot(dot2);
	}
	else if (m_nLocateModel == LOCATE_MODEL_3_POINTS)
	{
		dbDistOrg1 = dotOrg1.DistanceFromDot(dotOrg2);
		dbDist1 = dot1.DistanceFromDot(dot2);

		dbDistOrg2 = dotOrg2.DistanceFromDot(dotOrg3);
		dbDist2 = dot2.DistanceFromDot(dot3);
	}
	else if (m_nLocateModel == LOCATE_MODEL_4_POINTS)
	{
		dbDistOrg1 = CDot::Center(dotOrg1, dotOrg4).DistanceFromDot(CDot::Center(dotOrg2, dotOrg3));
		dbDistOrg2 = CDot::Center(dotOrg1, dotOrg2).DistanceFromDot(CDot::Center(dotOrg3, dotOrg4));

		dbDist1 = CDot::Center(dot1, dot4).DistanceFromDot(CDot::Center(dot2, dot3));
		dbDist2 = CDot::Center(dot1, dot2).DistanceFromDot(CDot::Center(dot3, dot4));
	}
	
	if (dbDistOrg1==0 || (m_nLocateModel && dbDistOrg2==0))
	{
		WriteRunLog("未设置MARK点坐标，胀缩默认为1",eLogDataFlow);
		return;
	}

	double dbScale1 = dbDist1/dbDistOrg1;
	double dbScale2 = dbDist2/dbDistOrg2;
	
	CString strTemp, strDisErr1, strDisErr2;
	strTemp.Format("水平两定位点理论间距%f, 实际间距%f, 胀缩比%f; 垂直两定位点理论间距%f, 实际间距%f, 胀缩比%f", dbDistOrg1, dbDist1, dbScale1, dbDistOrg2, dbDist2, dbScale2);
	WriteRunLog(strTemp, eLogDebug);
	

	double dbDisErr1 = dbDist1-dbDistOrg1;
	double dbDisErr2 = dbDist2-dbDistOrg2;

	BOOL bLocalPointDistanceBeyond = FALSE;
	if (fabs(dbDisErr1)>m_dbScaleLimit)
	{
		bLocalPointDistanceBeyond = TRUE;
		strDisErr1.Format("水平方向mark点间距误差%.3f,", dbDisErr1);
	}
	if (m_nLocateModel && fabs(dbDisErr2)>m_dbScaleLimit)
	{
		bLocalPointDistanceBeyond = TRUE;
		strDisErr2.Format("垂直方向mark点间距误差%.3f,", dbDisErr2);
	}	
	if (bLocalPointDistanceBeyond)
	{
		strTemp.Format("%s%s超过限定范围(-%.3f,%.3f)! \n可能是产品变形或CCD捕获错误，退出标记!", strDisErr1, strDisErr2,  m_dbScaleLimit, m_dbScaleLimit);
		throw new CHLCCDException(strTemp);
	}
	
	CQuad quad = GetAllLimit();
	if (m_bEnableXFixSacle)
	{
		dbZoomX = m_dbFixXScale;
		strTemp.Format("X轴固定缩放%f", dbZoomX);
	}
	else
	{
		dbZoomX = dbScale1 + m_dbXAutoScaleAdd;
		strTemp.Format("X轴自动缩放%f+%f=%f", dbScale1, m_dbXAutoScaleAdd, dbZoomX);
	}
	WriteRunLog(strTemp, eLogDebug);
	if (m_bEnableYFixScale)
	{
		dbZoomY = m_dbFixYScale;
		strTemp.Format("X轴固定缩放%f", dbZoomY);
	}
	else
	{
		if (m_nLocateModel)
		{
			dbZoomY = dbScale2 + m_dbYAutoScaleAdd;
			strTemp.Format("Y轴自动缩放%f+%f=%f", dbScale2, m_dbYAutoScaleAdd, dbZoomY);
			
		}
		else
		{
			dbZoomY = dbZoomX;
			strTemp.Format("二点定位，Y轴采用X轴缩放系数%f", dbZoomX);
		}
	}
	WriteRunLog(strTemp, eLogDebug);
}

void CHLDoc::OnSaveAsSide1() 
{
	// TODO: Add your command handler code here
	for (int i=0; i<4; i++)
	{
		m_dbLPDrawX[i] = -m_dbLPDrawX[i];
	}

	SetSelect(TRUE);

	CDot dot(CENTER,CENTER);
	Scale(dot,-1,1);
	PreprocessMark();
	UpdateAllViews(NULL);

	CString strPath = GetPathName();
	if (strPath.IsEmpty())
	{
		AfxMessageBox("Please save the current document!");
		return;
	}
	strPath.Insert(strPath.GetLength()-3, "_反面");
	
	OnSaveDocument(strPath);
	SetPathName(strPath);
}

void CHLDoc::OnSetProduct()
{
	// TODO: 在此添加命令处理程序代码

	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}


	CDlgSetWafer dlg;
	dlg.m_pDoc = this;
	dlg.m_strProgID1 = m_strProgID1;
	dlg.m_strProgNewID2 = m_strProgNewID2;
	dlg.m_strProgNewID3 = m_strProgNewID3;
	dlg.m_strProgNewID4 = m_strProgNewID4;
	dlg.m_nLight1    = m_nLight1;
	dlg.m_nLight2    = m_nLight2;
	dlg.m_nLight3    = m_nLight3;
	dlg.m_nLight4    = m_nLight4;
	dlg.m_dbDisMaxErr  	 = m_dbDisMaxErr;
	dlg.m_dbAngleMaxErr  = m_dbAngleMaxErr;
	dlg.m_dbHandOffsetX  = m_dbHandOffsetX;
	dlg.m_dbHandOffsetY  = m_dbHandOffsetY;
	dlg.m_dbHandOffsetCCWAngle = m_dbHandOffsetCCWAngle;
	dlg.m_dbXAutoScaleAdd   = m_dbXAutoScaleAdd;
	dlg.m_dbYAutoScaleAdd   = m_dbYAutoScaleAdd;
	dlg.m_bEnableXFixSacle  = m_bEnableXFixSacle;
	dlg.m_bEnableYFixScale  = m_bEnableYFixScale;
	dlg.m_dbFixXScale  = m_dbFixXScale;
	dlg.m_dbFixYScale  = m_dbFixYScale;
	dlg.m_dbPoint0X = m_dbLPDrawX[0];
	dlg.m_dbPoint0Y = m_dbLPDrawY[0];
	dlg.m_dbPoint1X = m_dbLPDrawX[1];
	dlg.m_dbPoint1Y = m_dbLPDrawY[1];
	dlg.m_dbPoint2X = m_dbLPDrawX[2];
	dlg.m_dbPoint2Y = m_dbLPDrawY[2];
	dlg.m_dbPoint3X = m_dbLPDrawX[3];
	dlg.m_dbPoint3Y = m_dbLPDrawY[3];
	dlg.m_nLocateModel = m_nLocateModel;
	dlg.m_bEnableMarkFocus = m_bEnableMarkFocus;
	dlg.m_dbZCCDFocus = m_dbZCCDFocus;
	dlg.m_nGasModel	= m_nGasModel;
	dlg.m_nDivideType = m_DivideLayer.m_nDivideType;
	dlg.m_dbDivideX = m_DivideLayer.m_dbDivideX;
	dlg.m_dbDivideY = m_DivideLayer.m_dbDivideY;
	dlg.m_dbDivideAddX = m_DivideLayer.m_dbDivideAddX;
	dlg.m_dbDivideAddY = m_DivideLayer.m_dbDivideAddY;
	dlg.m_dbDivideOffsetX = m_DivideLayer.m_dbDivideOffsetX;
	dlg.m_dbDivideOffsetY = m_DivideLayer.m_dbDivideOffsetY;
	dlg.m_dbTestPowerFrequency = m_dbTestPowerFrequency;
	dlg.m_dbProductPowerMax = m_dbProductPowerMax;
	dlg.m_dbProductPowerMin = m_dbProductPowerMin;
	dlg.m_dMarkLay = m_dMarkLay;

	dlg.m_bScan = m_bScan;
	dlg.m_dbScanZpos = m_dbScanZpos;
	dlg.m_dbScanPtX = m_dbScanPtX;
	dlg.m_dbScanPtY = m_dbScanPtY;
	dlg.m_ScanPriNum = m_ScanPriNum;

	dlg.m_dbScaleLimit = m_dbScaleLimit;



	dlg.m_bQPosite = g_ParaIni.m_bUseQPosite;

	if (dlg.DoModal()==IDOK)
	{
		m_strProgID1 = dlg.m_strProgID1;
		m_strProgNewID2 = dlg.m_strProgNewID2;
		m_strProgNewID3 = dlg.m_strProgNewID3;
		m_strProgNewID4 = dlg.m_strProgNewID4;
		m_nLight1    = dlg.m_nLight1;
		m_nLight2    = dlg.m_nLight2;
		m_nLight3    = dlg.m_nLight3;
		m_nLight4    = dlg.m_nLight4;
		m_dbDisMaxErr  	 = dlg.m_dbDisMaxErr;
		m_dbAngleMaxErr  = dlg.m_dbAngleMaxErr;
		m_dbHandOffsetX  = dlg.m_dbHandOffsetX;
		m_dbHandOffsetY  = dlg.m_dbHandOffsetY;
		m_dbHandOffsetCCWAngle = dlg.m_dbHandOffsetCCWAngle;
		m_dbXAutoScaleAdd   = dlg.m_dbXAutoScaleAdd;
		m_dbYAutoScaleAdd   = dlg.m_dbYAutoScaleAdd;
		m_bEnableXFixSacle  = dlg.m_bEnableXFixSacle;
		m_bEnableYFixScale  = dlg.m_bEnableYFixScale;
		m_dbFixXScale  = dlg.m_dbFixXScale;
		m_dbFixYScale  = dlg.m_dbFixYScale;
		m_dbLPDrawX[0] = dlg.m_dbPoint0X;
		m_dbLPDrawY[0] = dlg.m_dbPoint0Y;
		m_dbLPDrawX[1] = dlg.m_dbPoint1X;
		m_dbLPDrawY[1] = dlg.m_dbPoint1Y;
		m_dbLPDrawX[2] = dlg.m_dbPoint2X;
		m_dbLPDrawY[2] = dlg.m_dbPoint2Y;
		m_dbLPDrawX[3] = dlg.m_dbPoint3X;
		m_dbLPDrawY[3] = dlg.m_dbPoint3Y;
		m_nLocateModel = dlg.m_nLocateModel;

		m_dMarkLay = dlg.m_dMarkLay;

		m_dbScaleLimit = dlg.m_dbScaleLimit;


		m_bScan = dlg.m_bScan;
		m_dbScanZpos = dlg.m_dbScanZpos;
		m_dbScanPtX = dlg.m_dbScanPtX;
		m_dbScanPtY = dlg.m_dbScanPtY;
		m_ScanPriNum = dlg.m_ScanPriNum;

		g_ParaIni.m_bUseQPosite = dlg.m_bQPosite ;
		g_ParaIni.SavePara();


		m_bEnableMarkFocus = dlg.m_bEnableMarkFocus;
//		m_bFocusMark = dlg.m_bFocusMark;
		m_dbZCCDFocus = dlg.m_dbZCCDFocus;
//		m_dbZMarkFocus = dlg.m_dbZMarkFocus;	

		m_nGasModel = dlg.m_nGasModel;
		m_DivideLayer.m_nDivideType = dlg.m_nDivideType;
		m_DivideLayer.m_dbDivideX = dlg.m_dbDivideX;
		m_DivideLayer.m_dbDivideY = dlg.m_dbDivideY;
		m_DivideLayer.m_dbDivideAddX = dlg.m_dbDivideAddX;
		m_DivideLayer.m_dbDivideAddY = dlg.m_dbDivideAddY;
		m_DivideLayer.m_dbDivideOffsetX = dlg.m_dbDivideOffsetX;
		m_DivideLayer.m_dbDivideOffsetY = dlg.m_dbDivideOffsetY;
		m_dbTestPowerFrequency = dlg.m_dbTestPowerFrequency;
		m_dbProductPowerMax = dlg.m_dbProductPowerMax;
		m_dbProductPowerMin = dlg.m_dbProductPowerMin;
		m_DivideLayer.ResetDivide();

		g_pDlgMarkNormalPane->ResetProductDocPara(0x04);
		UpdateAllViews(NULL);

		UpdateLay();
	}
}

void CHLDoc::OnSaveAsReverse()
{
	// TODO: 在此添加命令处理程序代码
	for (int i=0; i<4; i++)
	{
		m_dbLPDrawX[i] = -m_dbLPDrawX[i];
	}

	SetSelect(TRUE);

	CDot dot(0,0);
	Scale(dot,-1,1);
	PreprocessMark();
	UpdateAllViews(NULL);

	CString strPath = GetPathName();
	if (strPath.IsEmpty())
	{
		AfxMessageBox("Please save the current document!");
		return;
	}
	strPath.Insert(strPath.GetLength()-3, "_反面");

	OnSaveDocument(strPath);
	SetPathName(strPath);
}


void CHLDoc::OnSetLeftMark()
{
	if (m_QuadList.GetSize()>0)
	{
		CDot dotCenter = m_QuadList.GetAt(0).m_QD.GetBaseDot();
		dotCenter.Round(3);
		m_dbLPDrawX[0] = dotCenter.x;
		m_dbLPDrawY[0] = dotCenter.y;

		OnSetAsNoCutLay();
	}
}


void CHLDoc::OnSetRightMark()
{
	if (m_QuadList.GetSize()>0)
	{
		CDot dotCenter = m_QuadList.GetAt(0).m_QD.GetBaseDot();
		dotCenter.Round(3);
		m_dbLPDrawX[1] = dotCenter.x;
		m_dbLPDrawY[1] = dotCenter.y;

		OnSetAsNoCutLay();
	}
}


void CHLDoc::OnSetTopMark()
{
	if (m_QuadList.GetSize()>0)
	{
		CDot dotCenter = m_QuadList.GetAt(0).m_QD.GetBaseDot();
		dotCenter.Round(3);
		m_dbLPDrawX[2] = dotCenter.x;
		m_dbLPDrawY[2] = dotCenter.y;

		OnSetAsNoCutLay();
	}
}


void CHLDoc::OnSetBottomMark()
{
	if (m_QuadList.GetSize()>0)
	{
		CDot dotCenter = m_QuadList.GetAt(0).m_QD.GetBaseDot();
		dotCenter.Round(3);
		m_dbLPDrawX[3] = dotCenter.x;
		m_dbLPDrawY[3] = dotCenter.y;

		OnSetAsNoCutLay();
	}
}


void CHLDoc::OnAutoOffset()
{
	// TODO: 在此添加命令处理程序代码

	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}
	if (!g_pMotion->IsHomed())
	{
		AfxMessageBox("The workbench is not home");
		return;
	}
	if ((g_pMotion->GetStatus(eMaskX)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskY)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskZ)&eStatusBitMoving))
	{
		AfxMessageBox("The workbench is Moving");
		return;
	}
	int nMoveModel = CFStatic::m_nMoveModel;
	CDlgAutoOffset dlg;
	dlg.m_pDoc = this;
	dlg.m_nOldMoveModel = nMoveModel;
	dlg.DoModal();
}


void CHLDoc::OnFocusTypeSet()
{
	// TODO: 在此添加命令处理程序代码
	CDlgFocusTypeSet dlg;
	dlg.DoModal();
}

BOOL CHLDoc::ScanMatri(BOOL bNet)
{
	WriteRunLog("ScanMatri", eLogTime);
	BOOL bScanRet = FALSE;
	//double dbScanId;
	//g_pMotion->GetPara("XY", "ScanId", dbScanId);

	g_ccd.SetWorkProgNo(m_ScanPriNum);

	CString strRec;
	g_ccd.ClearCCDBuffer();
	g_ccd.SendCCDCmd("T1");
	strRec = g_ccd.RecCCDReply();
	WriteRunLog(strRec, eLogDataFlow);
	//strRec = "T1,0,123456789";
	g_ccd.SetWorkProgNo(atoi(m_strProgID1));

	CString strCode = strRec;
	int nRet = strCode.Find(_T("T1"));
	CString strError = "";
	if (nRet != -1)
	{
		strCode = strCode.Right(strCode.GetLength() - nRet - 3);
		strError = strCode.Left(1);
		if (strError == "0")
		{
			strError  = strCode.Mid(2);
			CString strEmpty = strError;
			strEmpty.Trim();
			if (strEmpty == "")
			{
				strError = "ScanError";
				bScanRet = FALSE;
			}
			//读取数据库
			else
			{
				if(bNet)
				{
					WriteRunLog(strError, eLogDataFlow);
					g_pDlgMarkNormalPane->m_strSendCode = strError;

						if (!g_pDlgMarkNormalPane->PreNetSendRecv(strError))
						{
							return FALSE;
						}
				}

				else 
				{
					if (m_bUseFile)
					{

						CHLApp *pApp = (CHLApp *)AfxGetApp();
						CString strNGPath = pApp->GetProfileString("NGMAPPATH", "NGMapPath","C:\\Users\\lenovo\\Desktop\\ChangeDoc\\Release\\ChangeDoc.exe");
						strNGPath.Replace("//","/");
						strNGPath = strNGPath + " " + strError;
						STARTUPINFO si;
						ZeroMemory( &si, sizeof(si) );
						si.cb = sizeof(si);
						si.dwFlags=STARTF_USESHOWWINDOW;
						si.wShowWindow=SW_NORMAL;
						PROCESS_INFORMATION pi;
						ZeroMemory( &pi, sizeof(pi) );
						if( !CreateProcess( NULL,   // No module name (use command line). 
							(LPTSTR)(LPCTSTR)strNGPath, // Command line. 
							NULL,             // Process handle not inheritable. 
							NULL,             // Thread handle not inheritable. 
							FALSE,            // Set handle inheritance to FALSE. 
							0,                // No creation flags. 
							NULL,             // Use parent's environment block. 
							NULL,             // Use parent's starting directory. 
							&si,              // Pointer to STARTUPINFO structure.
							&pi)             // Pointer to PROCESS_INFORMATION structure.
							) 
						{
							AfxMessageBox(_T("CreateProcess Fail "));
							CloseHandle(pi.hThread);
							CloseHandle(pi.hProcess);
						}

						//CString strNGName = strNGPath;
						//strNGName.MakeReverse();
						
						Sleep(1000);
						HWND hwnd = FindWindow(NULL,"NGMap");
						if (NULL != hwnd)
						{
							SendMessage(hwnd,WM_CLOSE,0,0);
						}

					}
				}

				bScanRet = TRUE;
			}
		}
		else
		{
			strError = strCode + "ScanError";
		}
	}
	else
	{
		strError = "ScanError";
		bScanRet = FALSE;
	}
	g_pDlgMarkNormalPane->SetDlgItemText(IDC_STATIC_WARNING_INFO, strError);

	if (!bScanRet)
	{
		g_IO.OUTPUT_COD_BARCODE_ERROR(TRUE);
		Sleep(100);
		g_IO.OUTPUT_COD_BARCODE_ERROR(FALSE);

		g_IO.OUTPUT_SOFT_OK(FALSE);
		g_pDlgMarkNormalPane->MessageBox(strError,_T("Error"));
		return FALSE;
	}
	return TRUE;
}


void CHLDoc::OnCheckPower()
{
	// TODO: 在此添加命令处理程序代码
	int nTH=0;
	CString strCMD, strInput, strTip, strLog;

	strLog.Format("调用功率检测:%s", GetPathName());
	WriteRunLog(strLog, eLogDataFlow);

	if (g_sys.m_nPowerMeterPort==0) 
	{
		strTip = "没有启用该设备!";

		strLog.Format("%s,Send(%s),Rec(%s)", strTip, strCMD, strInput);
		WriteRunLog(strLog, eLogDataFlow);
		AfxMessageBox(strTip);
		return;
	}

	if (!g_pMotion->IsHomed())
	{
		AfxMessageBox("The workbench is not home");
		return;
	}
	if ((g_pMotion->GetStatus(eMaskX)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskY)&eStatusBitMoving) ||
		(g_pMotion->GetStatus(eMaskZ)&eStatusBitMoving))
	{
		AfxMessageBox("The workbench is Moving");
		return;
	}
		static CDlgCheckPower *dlg  = NULL;
		if ( dlg )
		{
			dlg->ShowWindow(SW_SHOW);
		}
		else
		{
			dlg = new CDlgCheckPower();
			dlg->Create(CDlgCheckPower::IDD);

			dlg->m_strDocPath = GetPathName();
			dlg->m_dbTestPowerFrequency = m_dbTestPowerFrequency;
			dlg->m_dbProductPowerMax = m_dbProductPowerMax;
			dlg->m_dbProductPowerMin = m_dbProductPowerMin;
			dlg->m_pLaserController = g_pDlgMarkNormalPane->m_pLaserController;
			dlg->m_nThermaTrack = nTH;

			dlg->CenterWindow();
			dlg->ShowWindow(SW_SHOW);
		}

	if (g_sys.m_nLaserCurrentKeep)//维持电流
	{
		g_pDlgMarkNormalPane->SetCurrent(g_sys.m_nLaserCurrentKeep);
	}
}

void CHLDoc::OnNetSet()
{
	// TODO: 在此添加命令处理程序代码
	CNetSet dlg;
	dlg.DoModal();
} 


BOOL CHLDoc::NetSendRecvOpenDoc()
{
	CString strSend     = "TP02\r\n!COMMAND_START\r\nID 510008\r\n";
	CString strLotNo    = g_ParaIni.ReadString(_T("LOTNO"),_T("LotNo"),_T(""));
	CString strEqNo     = g_ParaIni.ReadString(_T("EQNO"),_T("EqNo"),_T(""));
	CString strPanleID   = "ARG IDC6017 " + g_pDlgMarkNormalPane->m_strSendCode + "\r\n";
	CString strNo		 = "ARG CDC0010 " + strEqNo + "\r\n";

	CString strOperatorCode = "ARG CDC0145 " + g_pDlgMarkNormalPane->m_strOperatorcode + "\r\n";
	
	int nStyle =  g_ParaIni.ReadInt(_T("COMBOSTYLE"),_T("ComBoStyle"),0);
	CString strProcess;
	if (nStyle)
	{
		strProcess = "ARG DHC0013 CUT\r\n";
	}
	else
	{
		strProcess = "ARG DHC0013 TRENCH\r\n";
	}


	strSend = strSend + strPanleID + strProcess + strNo + strOperatorCode  + "!COMMAND_END\r\nEND\r\n";

	g_pDlgMarkNormalPane->m_sockClient.ClearBuffer();
	g_pDlgMarkNormalPane->m_sockClient.Send(strSend);
	WriteRunLog(strSend, eLogDebug);

	CString strFirst = g_pDlgMarkNormalPane->m_sockClient.Receive();
	CString strRecv = g_pDlgMarkNormalPane->m_sockClient.Receive();
	//strRecv = "0000000 成功\r\n!RESPONSE_START 701223:0000000:24:24\r\nDV342850000WXYZAG|DV342850000WXYZAGCCCC\r\nDV342850001WXYZAF|DV342850001WXYZAFCCCC\r\n!RESPONSE_END 701223\r\nEND\r\n";
	CString str;
	str = "One Receive: " + strFirst;
	WriteRunLog(str, eLogDebug);
	str = "Two Receive: " + strRecv;
	WriteRunLog(str, eLogDebug);

	strRecv = strFirst + strRecv;
	int nRet = strFirst.Find(_T("0000000"));
	if (-1 == nRet)
	{
		return FALSE;
	}
	int nFirst = strRecv.Find(_T("!RESPONSE_START"));
	int nEnd = strRecv.Find(_T("!RESPONSE_END"));
	nEnd = nEnd - nFirst;
	CString strData = strRecv.Mid(nFirst,nEnd);
	nFirst = strData.Find(_T("\r\n"));
	strData = strData.Mid(nFirst + 2);
	strData.Trim();

	if (-1 != nFirst && -1 != nEnd)
	{
		m_strNetRecv.RemoveAll();
		CString strTemp;
		while(strData != "")
		{
			nRet = strData.Find(_T("\r\n"));
			if (-1 == nRet)
			{
				strData.Trim();
				m_strNetRecv.Add(strData);
				break;
			}
			strTemp = strData.Left(nRet);
			strTemp.Trim();
			m_strNetRecv.Add(strTemp);
			strData  = strData.Mid(nRet + 2);
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;

}


BOOL CHLDoc::NetSendRecvAfterData()
{
	CString strSend     = "TP02\r\n!COMMAND_START\r\nID 510024\r\n";

	CString strEqNo     =  g_ParaIni.ReadString(_T("EQNO"),_T("EqNo"),_T(""));
	CString strPanleID   = "ARG IDC6017 "+ g_pDlgMarkNormalPane->m_strSendCode + "\r\n";
	CString strNo		 = "ARG CDC0010 " + strEqNo + "\r\n";
	CString strName	 =		"ARG DHC0937 " + GetPathName() + "\r\n";


	PARA curMarkPara = GetLay().GetPara(0);
	CString strSpeed;
	strSpeed.Format(_T("0000%d"),(int)curMarkPara.dbMarkSpeed);
	strSpeed = "/" + strSpeed.Right(4);

	CString strQFreq;
	strQFreq.Format(_T("000%d"),(int)curMarkPara.dbQFreq);
	strQFreq =  "/" + strQFreq.Right(3);

	CString strQRelease;
	strQRelease.Format(_T("000%d"),(int)curMarkPara.dbQRelease);
	strQRelease =  "/" + strQRelease.Right(3);

	CString strCurrent;
	strCurrent.Format(_T("00%d"),(int)curMarkPara.dbCurrent);
	strCurrent =  "/" + strCurrent.Right(2);

	CString strPara = strSpeed + strQFreq + strQRelease + strCurrent + "\r\n";
	strPara = "ARG DHC6280 " + strPara;

	strSend = strSend + strPanleID + strNo + strName + strPara + "!COMMAND_END\r\nEND\r\n";

	g_pDlgMarkNormalPane->m_sockClient.ClearBuffer();
	g_pDlgMarkNormalPane->m_sockClient.Send(strSend);
	WriteRunLog(strSend, eLogDebug);

	CString strFirst = g_pDlgMarkNormalPane->m_sockClient.Receive();
	CString strRecv = g_pDlgMarkNormalPane->m_sockClient.Receive();
	//strRecv = "0000000 成功\r\n!RESPONSE_START 701223:0000000:24:24\r\nDV342850000WXYZAG|DV342850000WXYZAGCCCC\r\nDV342850001WXYZAF|DV342850001WXYZAFCCCC\r\n!RESPONSE_END 701223\r\nEND\r\n";
	CString str;
	str = "One Receive: " + strFirst;
	WriteRunLog(str, eLogDebug);
	str = "Two Receive: " + strRecv;
	WriteRunLog(str, eLogDebug);

	strRecv = strFirst + strRecv;
	int nRet = strFirst.Find(_T("0000000"));
	if (-1 == nRet)
	{
		return FALSE;
	}
	int nFirst = strRecv.Find(_T("!RESPONSE_START"));
	int nEnd = strRecv.Find(_T("!RESPONSE_END"));
	nEnd = nEnd - nFirst;
	CString strData = strRecv.Mid(nFirst,nEnd);
	nFirst = strData.Find(_T("\r\n"));
	strData = strData.Mid(nFirst + 2);
	strData.Trim();

	return TRUE;
}


void CHLDoc::OnGroupnum()
{
	// TODO: 在此添加命令处理程序代码
	BOOL bgetNum = FALSE;
	int nGroupNum = -1;
	CGroupObj *pGroupObj = NULL;

	POSITION pos  =m_chain.m_list.GetHeadPosition();
	while( pos != NULL )
	{
		pGroupObj = (CGroupObj*)m_chain.m_list.GetNext(pos);
		if( pGroupObj->IsSelect() && (pGroupObj->GetLockFlg() == FALSE) )
		{
			if (!bgetNum)
			{
				bgetNum  = TRUE;
				CGroupNum dlg;
				dlg.m_nGroupNum = pGroupObj->GetGroupNum();
				if (dlg.DoModal() == IDOK)
				{
					pGroupObj->SetGroupNum(dlg.m_nGroupNum);
					SetModifiedFlag(TRUE);
					nGroupNum = dlg.m_nGroupNum;
				}
			}
			else
			{
				if (nGroupNum != -1)
				{
					pGroupObj->SetGroupNum(nGroupNum);
					SetModifiedFlag(TRUE);
				}
			}
		}
	}
}


void CHLDoc::OnGroupSet()
{
	// TODO: 在此添加命令处理程序代码
	CGroupList dlg;
	dlg.DoModal();
}


void CHLDoc::OnLdfiledata()
{
	// TODO: 在此添加命令处理程序代码
	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}
	CFileLoadData dlg;
	dlg.DoModal();
}

BOOL CHLDoc::LoadFileDataChange(CString& strPath)
{
	g_pDlgMarkNormalPane->GetNodeCodeLength();

	CStdioFile ftxt;
	BOOL bcanOpen = ftxt.Open(strPath,CFile::modeRead|CFile::modeNoTruncate);
	if (!bcanOpen)
	{
		CString strFile = "cannot Open" + strPath;
		AfxMessageBox(strFile);
		return FALSE;
	}
	CString strData;
	int nCount = 1;
	m_strNetRecv.RemoveAll();
	for (int i = 1; i < 300; i++)
	{
		ftxt.ReadString(strData);
		strData.Trim();
		int nRet = strData.Find(_T("|"));
		if (-1 != nRet)
		{
			m_strNetRecv.Add(strData);

			CString strMatri = strData.Left(nRet);
			CString strText = strData.Right(strData.GetLength() - nRet - 1);
			CString strTextID,strMatriID;
			strTextID.Format(_T("%d"),nCount);
			strMatriID.Format(_T("%d"),nCount + 1000);
			if (strMatri != "")
			{
				m_chain.ChangeText(strMatri,strMatriID);
			}
			if (strText != "")
			{
				m_chain.ChangeText(strText,strTextID);
			}
			nCount++;
		}
		else
		{
			if (strData != "")
			{
				m_strNetRecv.Add(strData);
			}
		}
	}
	m_chain.PreprocessMark();
	UpdateAllViews(NULL);

	for (int i = 0 ; i < 3; i++)
	{
		g_pDlgMarkNormalPane->SetNodeCodeLength();
		m_chain.PreprocessMark();
		UpdateAllViews(NULL);
	}
	
	m_strFileDataPath = strPath;
	m_bUseFile = TRUE;
	ftxt.Close();
///	DeleteFile(strPath);
	return TRUE;
}

void CHLDoc::NumAdd(CString &strNum)
{
	//CString strFormat = "0123456789";
	int Num = _ttoi(strNum);
	Num++;
	strNum.Format(_T("%d"),Num);

}

void CHLDoc::OnArrayRename()
{
	// TODO: 在此添加命令处理程序代码
	CDlgArrayRename arrayDlg;
	if (arrayDlg.DoModal() == IDOK)
	{
		CString strTextNum = arrayDlg.m_strTextNum;
		CString strMatriNum = arrayDlg.m_strMatriNum;

		CGroupObj *pGroupObj = NULL;
		POSITION pos=m_chain.m_list.GetHeadPosition();
		while( pos!=NULL )
		{	
			pGroupObj = (CGroupObj *)m_chain.m_list.GetNext(pos);
			POSITION pos = pGroupObj->m_list.GetHeadPosition();
			CTree *pTree = NULL;
			while( pos != NULL )
			{	
				pTree = (CTree*)pGroupObj->m_list.GetNext(pos);
				if ( pTree )
				{
					if ( pTree->IsHaveTxt(pTree->m_pRoot) )
					{
						pTree->TxtAddNo(pTree->m_pRoot,strTextNum);
					}
					else if(pTree->IsHaveCodeBar(pTree->m_pRoot))
					{
						pTree->CodeBarAddNo(pTree->m_pRoot,strMatriNum);
					}
					else
					{
						continue;
					}
				}
			}
		}
	}
	m_chain.PreprocessMark(0x00);
	UpdateAllViews(NULL);
}

void CHLDoc::OnFoucesTest()
{
	// TODO: 在此添加命令处理程序代码
	int nMoveModel = CFStatic::m_nMoveModel;

	CDlgTestFocus TestFoucesDlg;
	TestFoucesDlg.m_nOldMoveModel = nMoveModel;
	TestFoucesDlg.m_pDoc = this;
	TestFoucesDlg.DoModal();
	
}

void CHLDoc::Setlight(int nLight1,int nLight2,int nLight3,int nLight4)
{
	WriteRunLog("Setlight",eLogTime);
	CString strCMD, strInput, strCheck;
	if (g_ccd.m_pCommLighter->IsOpen())
	{
		g_ccd.m_pCommLighter->ClearBuffer();

		int nCur =  1;
		while (nCur < 5)
		{
			if (nCur == 1)
			{
				strCMD.Format("$3%d0%02X", nCur, nLight1);
			}
			else if (nCur == 2)
			{
				strCMD.Format("$3%d0%02X", nCur, nLight2);
			}
			else if (nCur == 3)
			{
				strCMD.Format("$3%d0%02X", nCur, nLight3);
			}
			else
			{
				strCMD.Format("$3%d0%02X", nCur, nLight4);
			}
			WORD wCheck=0;
			for (int i=0; i<6; i++)	wCheck^=strCMD[i];

			strCheck.Format("%02X", wCheck);
			strCMD += strCheck;

			g_ccd.m_pCommLighter->WriteString(strCMD);
			strInput = g_ccd.m_pCommLighter->ReadString();

			if (strInput!="$")
			{
				CString strMsg;
				strMsg.Format("Set Light Fail!(%s,%s)", strCMD, strInput);
				WriteRunLog(strMsg,eLogDataFlow);
				return ;
			}
			nCur++;
		}
	}
}


void CHLDoc::OnSetBarcodeLen()
{
	// TODO: 在此添加命令处理程序代码
	CBarcodeSetLen dlg;
	dlg.DoModal();
}


BOOL CHLDoc::isDeviationOK(double dbNewMarkToCCD1X,double dbNewMarkToCCD1Y,BOOL bTicps)
{

	BOOL bDeviationOK = TRUE;
	double dbDeviationX = dbNewMarkToCCD1X-g_ParaIni.m_dbMarkToCCD1X;
	double dbDeviationY = dbNewMarkToCCD1Y-g_ParaIni.m_dbMarkToCCD1Y;
	if (dbDeviationX<g_ParaIni.m_dbAutoOffsetDeviationMin || dbDeviationX>g_ParaIni.m_dbAutoOffsetDeviationMax)
		bDeviationOK = FALSE;
	if (dbDeviationY<g_ParaIni.m_dbAutoOffsetDeviationMin || dbDeviationY>g_ParaIni.m_dbAutoOffsetDeviationMax)
		bDeviationOK = FALSE;

	CString strTip;
	if (bDeviationOK)
	{
		strTip.Format("偏移变化(%.1fμm,%.1fμm)，确认使用？", (dbNewMarkToCCD1X-g_ParaIni.m_dbMarkToCCD1X)*1000, (dbNewMarkToCCD1Y-g_ParaIni.m_dbMarkToCCD1Y)*1000);
		WriteRunLog(strTip, eLogDataFlow);

		if (bTicps)
		{
			if (AfxMessageBox(strTip, MB_OKCANCEL)==IDCANCEL)
				return FALSE;
		}
	}
	else
	{
		strTip.Format("偏移变化(%.1fμm,%.1fμm)，超出允许校正范围(%.0fμ,%.0fμ)!\n请重新校正,或更改允许校正范围!", 
			1000*(dbNewMarkToCCD1X-g_ParaIni.m_dbMarkToCCD1X), 1000*(dbNewMarkToCCD1Y-g_ParaIni.m_dbMarkToCCD1Y), 
			1000*(g_ParaIni.m_dbAutoOffsetDeviationMin), 1000*(g_ParaIni.m_dbAutoOffsetDeviationMax));
		WriteRunLog(strTip, eLogDataFlow);
		return FALSE;
	}


	g_ParaIni.m_dbMarkToCCD1X = dbNewMarkToCCD1X;
	g_ParaIni.m_dbMarkToCCD1Y = dbNewMarkToCCD1Y;
	g_ParaIni.SavePara();

	strTip.Format("“打标中心到CCD中心距离”已更新：(%.3f,%.3f)", g_ParaIni.m_dbMarkToCCD1X, g_ParaIni.m_dbMarkToCCD1Y);
	WriteRunLog(strTip,eLogDataFlow);
	return TRUE;
}


BOOL CHLDoc::MoveMarkPos(void)
{
	g_ParaIni.m_dbAutoOffsetLastX = g_pMotion->GetPos(eMaskX);
	g_ParaIni.m_dbAutoOffsetLastY = g_pMotion->GetPos(eMaskY);
	g_ParaIni.SavePara();

	CString strTip;
	strTip.Format("Start offset correction, marking the location(%.3f,%.3f), the old 'marking center distance into the center of the CCD'：(%.3f,%.3f)", 
		g_ParaIni.m_dbAutoOffsetLastX, g_ParaIni.m_dbAutoOffsetLastY,  g_ParaIni.m_dbMarkToCCD1X, g_ParaIni.m_dbMarkToCCD1Y);
	WriteRunLog(strTip, eLogDebug);

	double dbZ = GetFirstMarkFocus();
	g_pMotion->AMove(eMaskZ, dbZ);

	double CurX = g_pMotion->GetPos(eMaskX);
	double CurY = g_pMotion->GetPos(eMaskY);
	double dbRtPos;
	g_pMotion->GetPara("XY", "AutoOfferRightPtX", dbRtPos);

	if (CurX-g_ParaIni.m_dbMarkToCCD1X > dbRtPos)
	{
		AfxMessageBox("Beyond Moving Limit");
		g_pDlgMarkNormalPane->UpdateMotionView(TRUE);
		return FALSE;
	}
	g_pMotion->LnXY(CurX-g_ParaIni.m_dbMarkToCCD1X,CurY -g_ParaIni.m_dbMarkToCCD1Y);
	Sleep(g_ParaIni.m_nMotionDelayForMark+3000);

	return TRUE;
}


BOOL CHLDoc::AfterMarkMove(void)
{
	double CurX = g_pMotion->GetPos(eMaskX);
	double CurY = g_pMotion->GetPos(eMaskY);
	double dbLfPos;
	g_pMotion->GetPara("XY", "AutoOfferLeftPtX", dbLfPos);

	if (CurX+g_ParaIni.m_dbMarkToCCD1X < dbLfPos)
	{
		AfxMessageBox("Beyond Moving Limit");
		g_pDlgMarkNormalPane->UpdateMotionView(TRUE);
		return FALSE;
	}
	g_pMotion->LnXY(CurX+g_ParaIni.m_dbMarkToCCD1X,CurY +g_ParaIni.m_dbMarkToCCD1Y);
	Sleep(g_ParaIni.m_nMotionDelayForCCD);

	CString strTip;
	double dbMarkCrossPosX1 = g_pMotion->GetPos(eMaskX);
	double dbMarkCrossPosY1 = g_pMotion->GetPos(eMaskY);
	strTip.Format("CCD(%.3f,%.3f)", dbMarkCrossPosX1, dbMarkCrossPosY1);
	WriteRunLog(strTip, eLogDebug);

	return TRUE;
}


void CHLDoc::OnAutooffsetex()
{
	// TODO: 在此添加命令处理程序代码
	
		if (!g_pMotion->IsHomed())
		{
			AfxMessageBox("The workbench is not home");
			return;
		}
		if ((g_pMotion->GetStatus(eMaskX)&eStatusBitMoving) ||
			(g_pMotion->GetStatus(eMaskY)&eStatusBitMoving) ||
			(g_pMotion->GetStatus(eMaskZ)&eStatusBitMoving))
		{
			AfxMessageBox("The workbench is Moving");
			return;
		}
		int nMoveModel = CFStatic::m_nMoveModel;
		CDlgAutoOffsetEx dlg;
		dlg.m_pDoc = this;
		dlg.m_nOldMoveModel = nMoveModel;
		dlg.DoModal();
}


void CHLDoc::SetStrokeLayFreq()
{
	WriteRunLog("SetStrokeLayFreq", eLogTime);

	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_chain.m_list.GetHeadPosition();
	while( pos  != NULL )
	{	
		pGroupObj = (CGroupObj *)m_chain.m_list.GetNext(pos);
		POSITION pos = pGroupObj->m_list.GetHeadPosition();
		CTree *pTree = NULL;
		while( pos != NULL )
		{	
			pTree = (CTree*)pGroupObj->m_list.GetNext(pos);
			if ( pTree )
			{

				int nLayer,nFillLayer;
				pTree->GetLayColor(nLayer,nFillLayer);
				PARA curMarkPara =	GetLay().GetPara(nLayer);   

				pTree->SetStrokeLayFreq(pTree->m_pRoot,curMarkPara.dbQFreq);
			}
		}
	}
}





void CHLDoc::OnWaferArray()
{
	// TODO: 在此添加命令处理程序代码
	BOOL bMng = g_CurUserInfo.dwRight1 & 0x00000001;
	if (!bMng) 
	{
		AfxMessageBox(IDS_NOMNGRIGHT);
		return;
	}

	if (GetLimit().IsEmpty()) //没有选中对象
	{
		return;
	}

	CDlgSetWaferArray dlg;
	dlg.m_dbRowDist = m_dbRowDist;
	dlg.m_dbColDist = m_dbColDist;
	dlg.m_dbShrinkRow = m_dbShrinkRow;
	dlg.m_dbShrinkYCol = m_dbShrinkYCol;

	if (dlg.DoModal()!=IDOK) 
		return;

	if (dlg.m_dbRowDist==0 || dlg.m_dbColDist==0)
	{
		m_chain.m_listCopyObject.RemoveAll();
		UpdateAllViews(NULL);
		return;
	}
	
   // CGroupObj * pGroupObj=m_chain.m_list.GetHead();
	CGroupObj* pGroupObj = NULL;
	POSITION pos=m_chain.m_list.GetHeadPosition(), posPrv;
	while(pos!=NULL)
	{
		posPrv = pos;
		pGroupObj = (CGroupObj *)m_chain.m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			m_chain.m_list.RemoveAt(posPrv);
			m_chain.m_list.AddHead(pGroupObj);
			break;
		}
	}

	CQuad quad=pGroupObj->GetLimit();

	CDot dot=quad.GetBaseDot();
	double dbHeight = quad.Height();
	double dbWidth = quad.Width();

	double dbshrinkRow=dlg.m_dbShrinkRow;
	double dbshrinkYCol=dlg.m_dbShrinkYCol;

	double dbWaferSize = m_DivideLayer.m_dbProductSize;
	
	int nCol,nRow;  //垂直下方个数，右方个数，水平个数


	nCol = (int)dlg.m_dbShrinkRow;
	nRow = (int)dlg.m_dbShrinkYCol;
	
	//寻找左下点
	CDot dotBase;
	dotBase.x=dot.x;//-dlg.m_dbColDist*nRight;
	dotBase.y=dot.y;//-dlg.m_dbRowDist*nDown;
	

	CChain::CCopyObject obj;
	m_chain.m_listCopyObject.RemoveAll();

	//阵列
	CStringArray ArrayPosi;
    CString strPosi;
	for(int i=0;i<nCol;i++)
	{
		for(int j=0;j<nRow;j++)
		{
			CDot dt,posi;
			dt.x=dotBase.x+i*dlg.m_dbColDist;
			dt.y=dotBase.y+j*dlg.m_dbRowDist;
			/*
			if(i%2==0)
			{
			dt.y=dotBase.y+j*dlg.m_dbRowDist;
			}
			else
			{
			dt.y=dotBase.y+(nRow-1-j)*dlg.m_dbRowDist;//按列向S形排布
		}*/
			double dbDiX = dt.x;
			double dbDiY = dt.y;
//			if (dbDiX < (dbWaferSize/2+dbshrink) && dbDiY < (dbWaferSize/2+dbshrinkY))
			{
				obj.m_quadLimit = CQuad(dt, dbWidth, dbHeight);
				m_chain.m_listCopyObject.AddTail(obj);
//				dtArray.Add(dt);
                posi.x=j;//行数
				posi.y=i;//列数
//				posiArray.Add(posi);
				
				strPosi.Format("%02d%02d",j,i);
				ArrayPosi.Add(strPosi);
			}
		}
	}
	
	m_dbRowDist = dlg.m_dbRowDist;
	m_dbColDist = dlg.m_dbColDist;
	m_dbShrinkRow  = dlg.m_dbShrinkRow;
	m_dbShrinkYCol  = dlg.m_dbShrinkYCol;
	UpdateAllViews(NULL);

//	ArrayCmd(dtArray);
//	m_chain.AddPosiInfo(posiArray);//记录行列序号
//	if(dlg.m_bAutoArray)
//		m_chain.ChangePosiString(ArrayPosi);
}


void CHLDoc::OnReplaceText()
{
	// TODO: 在此添加命令处理程序代码
	

	int nCodeId = 0;
	CDot QuadDot[1000];
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_chain.m_list.GetHeadPosition();
	while( pos  != NULL )
	{	
		pGroupObj = (CGroupObj *)m_chain.m_list.GetNext(pos);
		POSITION pos = pGroupObj->m_list.GetHeadPosition();
		CTree *pTree = NULL;
		while( pos != NULL )
		{	
			pTree = (CTree*)pGroupObj->m_list.GetNext(pos);
			if ( pTree )
			{
				pTree->GetTextBaseDot(pTree->m_pRoot,QuadDot,nCodeId);
			}
		}
	}

	CDlgReplaceText dlg;
	if (dlg.DoModal()==IDOK)
	{
		CString strchange, strModelNO;
		CString str1=dlg.m_strText;
		POSITION pos=m_listMarkText.GetHeadPosition();
		BOOL bAddString = TRUE;
		if (str1.IsEmpty()==FALSE) 
		{
			for (int j=0; j<10 ;j++) 
			{  //和上10次条码相同
				if (pos==NULL) 
					break;

				CString strText=m_listMarkText.GetNext(pos);

				if (str1==strText)
				{
					AfxMessageBox("和前10次条码相同，请注意");
					bAddString = FALSE;
					break;
				}
			}
			if(bAddString)
				m_listMarkText.AddHead(str1);

			int posi,i=0;
			while (1)
			{
				posi=str1.Find("/");
				if (posi==-1) 
				{
					ProcessString(str1);

					strModelNO.Format("%d", i);
					m_chain.ChangeText(str1, strModelNO);
					break;
				}
				strchange=str1.Mid(0,posi);
				ProcessString(strchange);
				str1=str1.Mid(posi+1);

				strModelNO.Format("%d", i);
				m_chain.ChangeText(strchange, strModelNO);
				i++;
			}

		}
		if (dlg.m_strText.IsEmpty()==FALSE ) 
		{
			m_chain.PreprocessMark();
			UpdateAllViews(NULL);



			/*nCount = 0;
			pos = m_chain.m_list.GetHeadPosition();
			while( pos != NULL )
			{
				CGroupObj *pGroupObj = NULL;
				pGroupObj = (CGroupObj*)m_chain.m_list.GetNext(pos);
				CDot dot = pGroupObj->GetLimit().GetBaseDot();
				pGroupObj->Move(QuadDot[nCount].x-dot.x,QuadDot[nCount].y-dot.y);

				nCount ++;
			}*/

			nCodeId = 0;
			pos = m_chain.m_list.GetHeadPosition();
			while( pos  != NULL )
			{	
				pGroupObj = (CGroupObj *)m_chain.m_list.GetNext(pos);
				POSITION pos = pGroupObj->m_list.GetHeadPosition();
				CTree *pTree = NULL;
				while( pos != NULL )
				{	
					pTree = (CTree*)pGroupObj->m_list.GetNext(pos);
					if ( pTree )
					{
						pTree->MoveOldBaseDot(pTree->m_pRoot,QuadDot,nCodeId);
					}
				}
			}



			g_pDlgMarkNormalPane->m_bReplaceTxt=TRUE;//LZQ,20121226

			CString str;
			str.Format("打标内容替换为(%s)!", dlg.m_strText);
			WriteRunLog(str, eLogDataFlow);
		}
		m_bModified=TRUE;
	}
}
void CHLDoc::ProcessString(CString& str) //"A.BC"-->"AbC"
{
	CString strtmp=str,strchange,strReturn;
	CString strLow;
	strReturn="";
	while (1) {
		int posi=strtmp.Find(".");
		if (posi==-1) {
			strReturn+=strtmp;
			break;
		}
		if (posi==0) {
			strLow=strtmp.Mid(1,1);
			strLow.MakeLower();
			strReturn+=strLow;
			strtmp=strtmp.Mid(2);
			continue;
		}
		strchange=strtmp.Mid(0,posi);
		strLow=strtmp.Mid(posi+1,1);
		strLow.MakeLower();
		strchange+=strLow;
		strReturn+=strchange;
		strtmp=strtmp.Mid(posi+2);
	}
	str=strReturn;
}


void CHLDoc::OnUpdateJointstrokeCurdoc(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(	!( GetStatus() & CChain::selectNull ) && (g_CurUserInfo.dwRight1 & 0x0010));
}

void CHLDoc::JoinStrokeCmd(BOOL bPopMsg)
{
	CHLView   *pView = GetCurActiveView();
	if ( NULL == pView )
		return;

	if ( NULL == m_pTracker )
	{	
		m_pTracker = &(pView->m_track);
	}

	m_chain.JoinStroke(bPopMsg, g_SysCfg.m_dbJoinDistance);
	m_pTracker->ClearAll();

	ReLoadStrokeList();

	CQuad quad = GetLimit();
	SetTrackerQuad(quad);

	pView->DispSelObjProp();
}


void CHLDoc::OnSetAsNoCutLay()
{
	// TODO: 在此添加命令处理程序代码
	g_DAT.SetMarkPara(m_lay);
	m_chain.SetLayColor(6, 6);
}
