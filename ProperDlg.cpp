// ProperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ProperDlg.h"
#include "HLView.h"
#include "STYLESAVEDLG.H"
#include "LocNukeForHL.h"
#include "StringIDTable.h"
#include "MainFrm.h"
#include "MyFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProperDlg dialog
#define WM_PG_ITEMCHANGED WM_USER+486
#define MINSPEED 0.01
#define FPK_ADVPARA

#include "resource.h"
CProperDlg::CProperDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProperDlg)
	m_bSelMark = FALSE;
	m_bDefault = FALSE;
	m_nItem = 0;
	m_nSelectMarkType = 0;
	//}}AFX_DATA_INIT
}

void CProperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProperDlg)
	DDX_Control(pDX, IDC_COMBO_STYLE, m_style);
	DDX_Control(pDX, IDC_LIST_LAYER, m_listLayer);
	DDX_Control(pDX, IDC_BUTTON_LAYER1, m_layerColorBt0);
	DDX_Control(pDX, IDC_BUTTON_LAYER2, m_layerColorBt1);
	DDX_Control(pDX, IDC_BUTTON_LAYER3, m_layerColorBt2);
	DDX_Control(pDX, IDC_BUTTON_LAYER4, m_layerColorBt3);
	DDX_Control(pDX, IDC_BUTTON_LAYER5, m_layerColorBt4);
	DDX_Control(pDX, IDC_BUTTON_LAYER6, m_layerColorBt5);
	DDX_Control(pDX, IDC_BUTTON_LAYER7, m_layerColorBt6);
	DDX_Check(pDX, IDC_CHECK_SEL_MARK, m_bSelMark);
	DDX_Check(pDX, IDC_CHECK_DEFAULT, m_bDefault);
	DDX_Radio(pDX, IDC_RADIO_SELECT_MARK_TYPE, m_nSelectMarkType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProperDlg, CDialogEx)
	//{{AFX_MSG_MAP(CProperDlg)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LAYER, OnRclickListLayer)
	ON_COMMAND(ID_MOD_LAYER_COLOR, OnModLayerColor)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LAYER, OnClickListLayer)
	ON_BN_CLICKED(IDC_BUTTON_LAYER1, OnButtonLayer1)
	ON_BN_CLICKED(IDC_BUTTON_LAYER2, OnButtonLayer2)
	ON_BN_CLICKED(IDC_BUTTON_LAYER3, OnButtonLayer3)
	ON_BN_CLICKED(IDC_BUTTON_LAYER4, OnButtonLayer4)
	ON_BN_CLICKED(IDC_BUTTON_LAYER5, OnButtonLayer5)
	ON_BN_CLICKED(IDC_BUTTON_LAYER6, OnButtonLayer6)
	ON_BN_CLICKED(IDC_BUTTON_LAYER7, OnButtonLayer7)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER1, OnDoubleclickedButtonLayer1)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER2, OnDoubleclickedButtonLayer2)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER4, OnDoubleclickedButtonLayer4)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER3, OnDoubleclickedButtonLayer3)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER5, OnDoubleclickedButtonLayer5)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER6, OnDoubleclickedButtonLayer6)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_LAYER7, OnDoubleclickedButtonLayer7)
	ON_BN_CLICKED(IDC_CHECK_SEL_MARK, OnCheckSelMark)
	ON_BN_CLICKED(IDC_CHECK_DEFAULT, OnCheckDefault)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, OnButtonErase)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnSelchangeComboStyle)
	ON_COMMAND(ID_LOCK_LAYER,  OnLockLayer)
	ON_COMMAND(ID_UNLOCK_LAYER,  OnUNLockLayer)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_CANCELMODE()
	ON_WM_MOUSEMOVE()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyItemChange)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_RADIO_SELECT_MARK_TYPE, &CProperDlg::OnClickedRadioSelectMarkType)
	ON_COMMAND(IDC_RADIO_SELECT_MARK_TYPE1, &CProperDlg::OnRadioSelectMarkType1)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_LAYNUM, &CProperDlg::OnBnClickedButtonChangeLaynum)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProperDlg message handlers

BOOL CProperDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	m_propertyGrid.SetColumnWidth(140);

	m_manageLayPara.LoadParaInfo();
	m_manageLayPara.InitPropertyGrid(m_propertyGrid);
	m_manageLayPara.LoadDefaultPara();
	m_manageLayPara.LoadAdvParaLib();


	InitLayer();

	SetParaListShow();

	CRect rect;
	this->GetDlgItem(IDC_STATIC_PARA1)->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	m_propertyGrid.MoveWindow(&rect);

	GetDlgItem(IDC_EDIT_LAYNUM)->SetWindowText("1");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CProperDlg::InitLayer() 
{
//	m_listLayer.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    m_listLayer.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_SHAREIMAGELISTS|LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES);
	char szNum[32];
	LcnLoadString(IDS_PARA_NUM,szNum,NAME_RES_MODULE,_T("Num"));
	char szColor[32];
	LcnLoadString(IDS_PARA_COLOR,szColor,NAME_RES_MODULE,_T("Color"));
	//char szParaHead[256];
	/*sprintf(szParaHead, "%s,25;%s,40;%s,40;%s,30;%s,38",szNum,szColor,
		m_manageLayPara.GetLayItemName(0),
		m_manageLayPara.GetLayItemName(1),
		m_manageLayPara.GetLayItemName(2));
	m_listLayer.SetHeadings(szParaHead);*/
	m_listLayer.InsertColumn(0, szNum, LVCFMT_LEFT, 28);
	m_listLayer.InsertColumn(1, szColor, LVCFMT_LEFT, 40);
	m_listLayer.InsertColumn(2, m_manageLayPara.GetLayItemName(0), LVCFMT_LEFT, 40);
	m_listLayer.InsertColumn(3, m_manageLayPara.GetLayItemName(1), LVCFMT_LEFT, 40);
	m_listLayer.InsertColumn(4, m_manageLayPara.GetLayItemName(2), LVCFMT_LEFT, 40);
	m_listLayer.SetColorColumn(1);

	m_layerColorBt0.Attach(RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128));
	m_layerColorBt1.Attach(RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128));
	m_layerColorBt2.Attach(RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128));
	m_layerColorBt3.Attach(RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128));
	m_layerColorBt4.Attach(RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128));
	m_layerColorBt5.Attach(RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128));
	m_layerColorBt6.Attach(RGB(127,127,127), RGB(255, 0, 0), RGB(0, 128, 128));

	HICON hIcon[2];
//	m_ImageList.Create(12,12,ILC_COLORDDB|ILC_MASK|ILC_COLOR16|ILC_COLOR24|ILC_COLOR32, 8, 8); // 32, 32 for large icons
    m_ImageList.Create(12, 12, ILC_MASK, 12, 0);
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON_LOCK);    // 未锁定
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON_UNLOCK);  // 锁定
	
	for (int i = 0; i < 2; i++) 
	{
		m_ImageList.Add(hIcon[i]);
	}
	m_ImageList.Add(hIcon[0]);
	m_ImageList.Add(hIcon[1]);
	
	//m_listLayer.SetImageList(&m_ImageList, LVSIL_SMALL);

	return TRUE;
}

BOOL CProperDlg::OnNewDocument(CLay &lay) 
{
	m_listLayer.DeleteAllItems();
	int nLayCount = lay.GetSize();
	for (int j = 0; j < g_SysCfg.m_nLayerCnt; j++)
	{
		CLayer layer   = lay.GetLayer(j);

		char szNum[8];
		sprintf(szNum, "%d", j);	
		m_listLayer.AddItem(szNum, _T(""), 
			m_manageLayPara.GetLayItemValue(0, (BYTE*)&layer.m_para, sizeof(PARA)), 
			m_manageLayPara.GetLayItemValue(1, (BYTE*)&layer.m_para, sizeof(PARA)), 
			m_manageLayPara.GetLayItemValue(2, (BYTE*)&layer.m_para, sizeof(PARA)));
	}

	for (int i = 0; i < g_SysCfg.m_nLayerCnt; i++) 
	{
		CLayer layer = lay.GetLayer(i);
		m_listLayer.SetRowColor(i, layer.m_color);
	    switch(i) 
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, TRUE);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color);
			break;
		default:
			break;
		}
	}

	SetFillObjMarkSet(lay);


	char szPath[256];
	GetExepath(szPath);
	m_strPath.Format("%s", szPath);
	m_style.ResetContent();

	CString searchFile=m_strPath+g_sys.m_strCardName+"\\*.yh";

	CString yh;
	CFileFind finder;
    BOOL bWorking = finder.FindFile(searchFile);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		yh=finder.GetFileName();
		yh=yh.Left(yh.GetLength()-3);
		m_style.AddString(yh);
	}

	m_style.SetCurSel(-1);
	m_bDefault = FALSE;
	m_listLayer.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	m_listLayer.SetCurSel(0);
	m_nItem=0;
	OnLayerSelChange(NULL,NULL);

	GetDlgItem(IDC_EDIT_LAYNUM)->SetWindowText("1");

	this->Invalidate();
	
	return TRUE;
}

BOOL CProperDlg::UpdateProperty()
{
	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL){
		return FALSE;
	}
	m_bSelMark = pDoc->m_chain.GetMarkSel();
	m_nSelectMarkType = pDoc->m_chain.m_nSelectMarkType;

	char szPath[256];
	GetExepath(szPath);
	m_strPath.Format("%s", szPath);
	m_style.ResetContent();

	CString searchFile=m_strPath+g_sys.m_strCardName+"\\*.yh";

	CString yh("");
	CFileFind finder;
    BOOL bWorking = finder.FindFile(searchFile);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();
		yh=finder.GetFileName();
		yh=yh.Left(yh.GetLength()-3);
		m_style.AddString(yh);
	}



	CString strLayNum;
	strLayNum.Format("%d",pDoc->m_dMarkLay);
	GetDlgItem(IDC_EDIT_LAYNUM)->SetWindowText(strLayNum);


	m_style.SetCurSel(-1);
	m_bDefault = FALSE;
//	m_listLayer.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
//	m_listLayer.SetCurSel(0);
    OnLayerSelChange(NULL,NULL);

	

	return TRUE;
}

CHLDoc * CProperDlg::GetDocument()
{
	CMDIFrameWnd *pMainWnd=(CMDIFrameWnd *)AfxGetMainWnd();
	CMDIChildWnd *pChildWnd=pMainWnd->MDIGetActive();
	if(pChildWnd)
	{
		CHLDoc *pDoc=(CHLDoc *)pChildWnd->GetActiveDocument();
		return pDoc;
	}
	return NULL;
}

void CProperDlg::OnRclickListLayer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (!(g_CurUserInfo.dwRight1 & 512))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	CHLDoc *pDoc=GetDocument();
	if (NULL == pDoc)
		return;

	HitTestEx(&m_listLayer, pNMHDR, m_nItem, m_nSubItem);
	BOOL bIsDrawMark = FALSE;
	CLay lay = pDoc->GetLay();
	int nIndex = m_listLayer.GetTopIndex();
	for (int i = nIndex; i < nIndex + 7; i++) 
	{
		if (m_nItem == i)
		{
			bIsDrawMark = TRUE;
		}
		else
		{
			bIsDrawMark = FALSE;
		}
		CLayer layer = lay.GetLayer(i);
		int nSetIndex = i - nIndex;
	    switch(nSetIndex) 
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		default:
			break;
		}
	}

	int index=pDoc->GetMenuIndex();
	CMenu menu,*pMenu;
	menu.LoadMenu(IDR_POP);
	pMenu=menu.GetSubMenu(5);
	if(pMenu)
	{
		POINT point;
		NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	    point = pNMListView->ptAction;
		ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
	SetFocus();

	*pResult = 0;
}

void CProperDlg::SetButtonColor(int nIndex, COLORREF color) 
{
	int nTopIndex = m_listLayer.GetTopIndex();
	int nSetIndex = nIndex - nTopIndex;
	switch(nSetIndex) 
	{
	case 0:
		m_layerColorBt0.SetBGColor(color, TRUE);
		break;
	case 1:
		m_layerColorBt1.SetBGColor(color, TRUE);
		break;
	case 2:
		m_layerColorBt2.SetBGColor(color, TRUE);
		break;
	case 3:
		m_layerColorBt3.SetBGColor(color, TRUE);
		break;
	case 4:
		m_layerColorBt4.SetBGColor(color, TRUE);
		break;
	case 5:
		m_layerColorBt5.SetBGColor(color, TRUE);
		break;
	case 6:
		m_layerColorBt6.SetBGColor(color, TRUE);
		break;
	default:
		break;
	}
	
	return;
}

void CProperDlg::OnModLayerColor() 
{
	CColorDialog colorDlg;
	if (IDOK == colorDlg.DoModal())
	{
		COLORREF selColor =colorDlg.GetColor();
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		if(NULL == pFWnd)
			return;
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(NULL == pView)
			return;
		CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
		if (NULL == pDoc)
			return;

		pDoc->SetLayerColor(m_nItem, selColor);
		m_listLayer.SetRowColor(m_nItem,selColor);
//		SetButtonColor(m_nItem, selColor);
		int nIndex = m_listLayer.GetTopIndex();
		int nNum   = m_nItem - nIndex;
		OnColorButtonClick(nNum);

		OnLayerColorChange(m_nItem, selColor);
	}

	return;
}

void CProperDlg::OnLayerColorChange(int nIndex, COLORREF color)
{
	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL)
		return;

	//层更新	
	CLayer ly=pDoc->GetLayer(nIndex);
	ly.m_color=color;
	
	//更新文档中相应层及其图形对象
	pDoc->SetLay(PTY_LAYER, ly);
	pDoc->UpdateAllViews(NULL);

	return;
}

BOOL CProperDlg::HitTestEx(CListCtrl *pCtrl, NMHDR* pNMHDR, int &nItem, int &nSubItem)
{
    if (pCtrl  == NULL) return FALSE ;
	if (pNMHDR == NULL) return FALSE;

	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;
	
	nItem    = pCtrl->HitTest( point, NULL);
	nSubItem = -1;
	m_listLayer.SetCurSel(nItem);

	return TRUE;
}

void CProperDlg::DispSelObjLayer(int nLayer)
{
	CHLDoc *pDoc=GetDocument();
	if (NULL == pDoc)
		return;

	m_nItem = nLayer;
	m_listLayer.SetItemState(m_nItem, LVIS_SELECTED , LVIS_SELECTED);
	m_listLayer.SetCurSel(m_nItem);
	m_listLayer.EnsureVisible(m_nItem, TRUE);

	int nIndex = m_listLayer.GetTopIndex();
	CLay lay = pDoc->GetLay();

	CLayer layer = lay.GetLayer(m_nItem);
	BOOL bIsDrawMark = FALSE;
	for (int i = nIndex; i < nIndex + 7; i++) 
	{
		if (m_nItem == i)
		{
			bIsDrawMark = TRUE;
		}
		else
		{
			bIsDrawMark = FALSE;
		}

		CLayer layer = lay.GetLayer(i);
		int nSetIndex = i - nIndex;
		switch(nSetIndex) 
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		default:
			break;
		}
	}

	OnLayerSelChange((NMHDR*)NULL, NULL);
}

void CProperDlg::OnClickListLayer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (!(g_CurUserInfo.dwRight1 & 512))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}
	
	CHLDoc *pDoc=(CHLDoc*)GetDocument();
	if (NULL == pDoc)
		return;

	HitTestEx(&m_listLayer, pNMHDR, m_nItem, m_nSubItem);

	BOOL bIsDrawMark = FALSE;

	int nIndex = m_listLayer.GetTopIndex();
	for (int i = nIndex; i < nIndex + 7; i++) 
	{
		if (m_nItem == i)
		{
			bIsDrawMark = TRUE;
		}
		else
		{
			bIsDrawMark = FALSE;
		}
		
        CLayer layer = pDoc->GetLayer(i);
		int nSetIndex = i - nIndex;
	    switch(nSetIndex) 
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		default:
			break;
		}
	}
	
	OnLayerSelChange(pNMHDR, pResult);
	
	*pResult = 0;
}

void CProperDlg::OnColorButtonClick(int nNum) 
{
	CHLDoc *pDoc=GetDocument();
	if (NULL == pDoc)
		return;

	CLay lay = pDoc->GetLay();
	int nIndex = m_listLayer.GetTopIndex();
	m_nItem = nIndex + nNum;
	m_listLayer.SetItemState(m_nItem, LVIS_SELECTED , LVIS_SELECTED);
	if (m_nItem)
	{
		m_listLayer.SetFocus();
	}

	m_listLayer.SetCurSel(m_nItem);
	CLayer layer = lay.GetLayer(m_nItem);
	BOOL bIsDrawMark = FALSE;
	for (int i = nIndex; i < nIndex + 7; i++) 
	{
		if (nIndex + nNum == i)
		{
			bIsDrawMark = TRUE;
		}
		else
		{
			bIsDrawMark = FALSE;
		}

		CLayer layer = lay.GetLayer(i);
		int nSetIndex = i - nIndex;
	    switch(nSetIndex) 
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color, bIsDrawMark);
			break;
		default:
			break;
		}
	}

	OnLayerSelChange((NMHDR*)0x01, NULL);
	
	return;
}

void CProperDlg::OnDoubleclickedColorButton() 
{
	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL)
		return;
	
	if(pDoc->GetStatus()==CChain::selectNull)
	{
		CString str;
		str.LoadString(IDS_NOOBJECT);
		AfxMessageBox(str,MB_ICONWARNING);
		return;
	}

	BOOL bIsColorToFil = ((CButton*)GetDlgItem(IDC_CHECK_FILL_COLOR))->GetCheck();
	CLayer layer = pDoc->GetLayer(m_nItem);
	if (bIsColorToFil)
	{
//		pDoc->m_chain.SetObjColor(m_nItem, layer.m_color, m_nItem, layer.m_color);
		pDoc->SetLay(PTY_FILL, pDoc->GetLayer(m_nItem));
	}
	else
	{
	    pDoc->SetLay(PTY_OUTLINE, pDoc->GetLayer(m_nItem));
	}

	((CButton*)GetDlgItem(IDC_CHECK_FILL_COLOR))->SetCheck(FALSE);

	pDoc->UpdateAllViews(NULL);

	return;
}

void CProperDlg::OnButtonLayer1() 
{
	OnColorButtonClick(0);

	return;
}

void CProperDlg::OnDoubleclickedButtonLayer1() 
{
	OnDoubleclickedColorButton();

	return;
}

void CProperDlg::OnButtonLayer2() 
{
	OnColorButtonClick(1);

	return;
}

void CProperDlg::OnDoubleclickedButtonLayer2() 
{
	OnDoubleclickedColorButton();

	return;
}

void CProperDlg::OnButtonLayer3() 
{
	OnColorButtonClick(2);

	return;
}

void CProperDlg::OnDoubleclickedButtonLayer3() 
{
	OnDoubleclickedColorButton();

	return;
}

void CProperDlg::OnButtonLayer4() 
{
	OnColorButtonClick(3);

	return;
	
}

void CProperDlg::OnDoubleclickedButtonLayer4() 
{
	OnDoubleclickedColorButton();

	return;
}

void CProperDlg::OnButtonLayer5() 
{
	OnColorButtonClick(4);

	return;
}

void CProperDlg::OnDoubleclickedButtonLayer5() 
{
	OnDoubleclickedColorButton();

	return;
}


void CProperDlg::OnButtonLayer6() 
{
	OnColorButtonClick(5);

	return;
}

void CProperDlg::OnDoubleclickedButtonLayer6() 
{
	OnDoubleclickedColorButton();

	return;
}

void CProperDlg::OnButtonLayer7() 
{
	OnColorButtonClick(6);

	return;
}

void CProperDlg::OnDoubleclickedButtonLayer7() 
{
	OnDoubleclickedColorButton();

	return;
}

void CProperDlg::OnCheckSelMark() 
{
	UpdateData(TRUE);
	CHLDoc *pDoc=GetDocument();
	pDoc->SetSelectedObjbeMarked(m_bSelMark);

	return;
}

void CProperDlg::OnCheckDefault() 
{
	UpdateData(TRUE);

	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL)
		return;

	UpdateData(TRUE);

	int index=m_listLayer.GetCurSel();
	if(index==LB_ERR)
	{
//		CString str;
//		str.LoadString(IDS_LAYER_NOSELECT);
//		AfxMessageBox(str,MB_ICONWARNING);

		return;
	}
	else 
	{
		CLayer ly=pDoc->GetLayer(index);
		ly.m_bDefault=m_bDefault;
		if(ly.m_bDefault)
		{
			CLayer tmp=pDoc->GetLayer();
			ly.m_para=tmp.m_para;
		}
		//更新文档中相应层
		pDoc->SetLay(PTY_PARA,ly);

		OnLayerSelChange(NULL,NULL);
	}

	return;
}

void CProperDlg::OnLayerSelChange(NMHDR* pNMHDR, LRESULT* pRes)
{
	CHLDoc *pDoc = GetDocument();
	if(pDoc == NULL)
	{
		return;
	}

	int index = m_listLayer.GetCurSel();
	if(index == LB_ERR)
	{
		return;
	}
	else 
	{	
		CLayer ly=pDoc->GetLayer(index);

		//第0层的处理
		if(index==0)
		{
			GetDlgItem(IDC_CHECK_DEFAULT)->ShowWindow(SW_HIDE);
			m_bDefault=FALSE;
		}
		else
		{
			GetDlgItem(IDC_CHECK_DEFAULT)->ShowWindow(SW_SHOW);
			m_bDefault=ly.m_bDefault;
		}

		if(pNMHDR)
		{
			//非内部更新,加入命令处理器
			ly.m_nLayer=index;
			pDoc->SetLay(PTY_CHANGE,ly);
		}

		//更新打标参数列表
		SetPara(ly.m_para);
	}

	return;
}

void CProperDlg::SetPara(PARA &para)
{
	m_manageLayPara.SetPara((BYTE*)&para, sizeof(PARA), m_propertyGrid);
	int index = m_listLayer.GetCurSel();
	m_listLayer.SetItemText(index, 2, 
		m_manageLayPara.GetLayItemValue(0, (BYTE*)&para, sizeof(PARA)));
	m_listLayer.SetItemText(index, 3, 
		m_manageLayPara.GetLayItemValue(1, (BYTE*)&para, sizeof(PARA)));
	m_listLayer.SetItemText(index, 4, 
		m_manageLayPara.GetLayItemValue(2, (BYTE*)&para, sizeof(PARA)));


	OnBnClickedButtonChangeLaynum();

	UpdateView();

	return;
}

void CProperDlg::UpdateView()
{
	GetDlgItem(IDC_COMBO_STYLE)->EnableWindow(!m_bDefault);	
	m_propertyGrid.EnableWindow(!m_bDefault);
	UpdateData(FALSE);
}

void CProperDlg::GetPara(PARA &para)
{
	m_manageLayPara.GetPara((BYTE*)&para, sizeof(PARA), m_propertyGrid);
}

void CProperDlg::OnButtonSave() 
{
	if (!(g_CurUserInfo.dwRight1 & 512))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	CStyleSaveDlg dlg;
	if(dlg.DoModal()==IDOK)
	{
		int index;
		index=m_style.FindStringExact(-1,dlg.m_strStyle);
		if(index!=CB_ERR)
		{	
			CString strError;
			strError.LoadString(IDS_YH_DULNAME);
			if(AfxMessageBox(strError,MB_YESNO) != IDYES)
			{
				return;
			}
		}

		CString strStyle = dlg.m_strStyle;
		
		CString filename;
		filename = m_strPath + g_sys.m_strCardName + "\\" + strStyle + ".yh";
		FILE *fp;
		if((fp=fopen(filename,"wb"))==NULL)
		{	
			CString strError;
			strError.LoadString(IDS_YH_ADD);
			MessageBox(strError);
		}
		else
		{
			if(index==CB_ERR)
			{
				m_style.AddString(dlg.m_strStyle);
			}
			m_style.SelectString(-1, dlg.m_strStyle);

//			fwrite(&buf,sizeof(PARA),1, fp);
			//
			CLayer ly(0);
			GetPara(ly.m_para);
			ly.Serialize(fp, TRUE);
			//
			fclose(fp);
		}
	}

	return;
}

void CProperDlg::OnButtonErase() 
{
	if (!(g_CurUserInfo.dwRight1 & 512))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	int index;
	index=m_style.GetCurSel();
	if(index==CB_ERR)
	{
		CString strError;
		strError.LoadString(IDS_YH_NOSEL);
		MessageBox(strError);
	}
	else
	{	
		CString filename;
		m_style.GetLBText(index,filename);
		filename=m_strPath + g_sys.m_strCardName + "\\" + filename + ".yh";
 		if(!DeleteFile(filename))
		{
			CString strError;
			strError.LoadString(IDS_YH_DELETE);
			MessageBox(strError);
		}
		else
		{
			m_style.DeleteString(index);
			m_style.SetCurSel(-1);
		}
	}

	return;
}

void CProperDlg::OnButtonDefault() 
{
	if (!(g_CurUserInfo.dwRight1 & 512))
	{
		AfxMessageBox(IDS_RIGHT_NO);
		return;
	}

	CString strMessage;
	strMessage.LoadString(IDS_YH_PROMPT);
	if(AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION)!=IDYES)
	{
		return;
	}

	CString filename;
	FILE *fp;
	filename=m_strPath + g_sys.m_strCardName + "\\default.yh";
	SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL); 
	if((fp=fopen(filename,"wb"))==NULL)
	{	
		CString strError;
		strError.LoadString(IDS_YH_DEFAULT);
		MessageBox(strError);
	}
	else
	{
		CLayer ly(0);
		GetPara(ly.m_para);
		ly.Serialize(fp, TRUE);
		fclose(fp);
	}

	SetFileAttributes(filename,FILE_ATTRIBUTE_READONLY); 	

	m_manageLayPara.LoadDefaultPara();

	return;
}

void CProperDlg::OnSelchangeComboStyle() 
{
	int index;
	index=m_style.GetCurSel();
	if(index!=CB_ERR)
	{	
		CString filename;
		m_style.GetLBText(index,filename);	
		filename=m_strPath + g_sys.m_strCardName + "\\" + filename + ".yh";

		FILE *fp;
		if((fp=fopen(filename,"rb"))==NULL)
		{	
			CString strError;
			strError.LoadString(IDS_YH_CHANGE);
			MessageBox(strError);
		}
		else
		{
			CLayer ly(0);
			ly.Serialize(fp, FALSE);
			fclose(fp);
			SetPara(ly.m_para);
			int nCurSel = m_listLayer.GetCurSel();
			OnParaListChange(NULL, NULL);
		}
	}

	return;
}

void CProperDlg::OnParaListChange(NMHDR* pNMHDR, LRESULT* pRes)
{
	OnParaChange(pNMHDR, pRes);

	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL)
		return;

	ListLayerTextRefresh(pDoc);
}

void CProperDlg::OnParaChange(NMHDR* pNMHDR, LRESULT* pRes)
{
	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL)
		return;
		
	int index=m_listLayer.GetCurSel();
	if(index==LB_ERR)
	{
//		CString str;
//		str.LoadString(IDS_LAYER_NOSELECT);
//		AfxMessageBox(str,MB_ICONWARNING);
	}
	else 
	{	
		//更新数据
		CLayer ly=pDoc->GetLayer(index);
		PARA OldPara;
		OldPara = ly.m_para;
		GetPara(ly.m_para);
		if (OldPara == ly.m_para) 
		{
			UpdateView();
			return;
		}
		pDoc->SetLay(PTY_PARA,ly);
		SetPara(ly.m_para);
		//更新文档中相应层
		UpdateView();
	}

	return;
}

BOOL CProperDlg::PreTranslateMessage(MSG* pMsg) 
{
	/*
	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F10) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_MARK_NORMAL , NULL);
		return TRUE;
	}

	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F7) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FAST_MARK , NULL);
		return TRUE;
	}
	*/

	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_RETURN) & 0x8000))
	{
		return TRUE;
	}
	
	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_ESCAPE) & 0x8000))
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CProperDlg::OnLockLayer()
{
	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL)
	{
		return;
	}
	
	char szValue[64] = {0};
	sprintf(szValue, "%d %s", m_nItem, "☆");
	m_listLayer.SetItemText(m_nItem, 0, szValue);
	pDoc->OnLockLayer(m_nItem);

	return;
}
void CProperDlg::OnUNLockLayer()
{
	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL)
	{
		return;
	}

	char szValue[64] = {0};
	sprintf(szValue, "%d", m_nItem);
	m_listLayer.SetItemText(m_nItem, 0, szValue);
	pDoc->OnUnLockLayer(m_nItem);

	return;
}

void CProperDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();
	return;
}

void CProperDlg::SetParaListShow() 
{  
	BOOL bShow = FALSE;
	if (!(g_CurUserInfo.dwRight1 & 1024))
	{
		bShow = FALSE;
	}
	else
	{
		bShow = TRUE;
	}

	//显示高级选项
	
	return;
}


void CProperDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	
	CDialogEx::OnMouseMove(nFlags, point);
}

void CProperDlg::OnCaptureChanged(CWnd *pWnd) 
{
	// TODO: Add your message handler code here
	
	CDialogEx::OnCaptureChanged(pWnd);
}

void CProperDlg::OnDocChange()
{
	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL)
	{
		return;
	}

	int nLayerCount = 0;
    nLayerCount = pDoc->GetLaySize();
	if (nLayerCount > g_SysCfg.m_nLayerCnt) 
	{
		nLayerCount = g_SysCfg.m_nLayerCnt;
	}
	else if(nLayerCount < g_SysCfg.m_nLayerCnt)
	{
		int nCount = g_SysCfg.m_nLayerCnt - nLayerCount;
		OnAddLayer(nLayerCount, nCount, pDoc);
	}

	ListLayerTextRefresh(pDoc);
	
	if (m_nItem >= nLayerCount) 
	{
		m_nItem = 0;
	}

	BOOL bSelNum = FALSE;
	int nIndex = m_listLayer.GetTopIndex();
	for (int j = nIndex; j < nLayerCount; j++) 
	{
		CLayer layer = pDoc->GetLayer(j);
		m_listLayer.SetRowColor(j,layer.m_color);
		if (m_nItem == j) 
		{
			bSelNum = TRUE;
		}
		
	    switch(j - nIndex) 
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, bSelNum);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color, bSelNum);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color, bSelNum);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color, bSelNum);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color, bSelNum);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color, bSelNum);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color, bSelNum);
			break;
		default:
			break;
		}

		bSelNum = FALSE;
	}

	m_listLayer.SetItemState(m_nItem, LVIS_SELECTED, LVIS_SELECTED);
	m_listLayer.SetCurSel(m_nItem);
	pDoc->SetCurLayer(m_nItem);

	CString strLayNum;
	strLayNum.Format("%d",pDoc->m_dMarkLay);
	GetDlgItem(IDC_EDIT_LAYNUM)->SetWindowText(strLayNum);

	return;
}


void CProperDlg::OnAddLayer(int nStart, int nCount, CHLDoc *pDoc)
{
	for (int j = nStart; j < nStart + nCount; j++)
	{
		CLayer layer   = pDoc->GetLayer(j);
		char szNum[8];
		sprintf(szNum, "%d", j);
		m_listLayer.AddItem(szNum, _T(""), 
			m_manageLayPara.GetLayItemValue(0, (BYTE*)&layer.m_para, sizeof(PARA)), 
			m_manageLayPara.GetLayItemValue(1, (BYTE*)&layer.m_para, sizeof(PARA)), 
			m_manageLayPara.GetLayItemValue(2, (BYTE*)&layer.m_para, sizeof(PARA)));
	}

	for (int i = nStart; i < nStart + nCount; i++) 
	{
		CLayer layer = pDoc->GetLayer(i);
		m_listLayer.SetRowColor(i,layer.m_color);
	    switch(i) 
		{
		case 0:
			m_layerColorBt0.SetBGColor(layer.m_color, TRUE);
			break;
		case 1:
			m_layerColorBt1.SetBGColor(layer.m_color);
			break;
		case 2:
			m_layerColorBt2.SetBGColor(layer.m_color);
			break;
		case 3:
			m_layerColorBt3.SetBGColor(layer.m_color);
			break;
		case 4:
			m_layerColorBt4.SetBGColor(layer.m_color);
			break;
		case 5:
			m_layerColorBt5.SetBGColor(layer.m_color);
			break;
		case 6:
			m_layerColorBt6.SetBGColor(layer.m_color);
			break;
		default:
			break;
		}
	}

	CLay &lay = pDoc->GetLay();
	SetFillObjMarkSet(lay);

	return;
}

void CProperDlg::SetFillObjMarkSet(CLay &lay)
{
	CMainFrame *pFWnd = (CMainFrame*)AfxGetMainWnd();
	if(NULL == pFWnd)
	{
		return;
	}

	pFWnd->m_XBarProperty.m_FillEditDlg.m_FillObjMarkSet.SetLayerCount(lay);

	return;
}


void CProperDlg::SetFpkDataByLayerIndex(int nLayer, PARA &para)
{
	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	if (-1 == nLayer)
	{
		return;
	}

	CLayer ly = pDoc->GetLayer(nLayer);
/*
	ly.m_para.bQSoftStart    = para.bQSoftStart;
	ly.m_para.nQSoftStartCnt = para.nQSoftStartCnt;
	memcpy(ly.m_para.dbQSoftStartVal, para.dbQSoftStartVal, sizeof(float) * MAX_QSOFT_CNT);

	ly.m_para.bQSoftEnd      = para.bQSoftEnd;
	ly.m_para.nQSoftEndCnt   = para.nQSoftEndCnt;
	memcpy(ly.m_para.dbQSoftEndVal, para.dbQSoftEndVal, sizeof(float) * MAX_QSOFT_CNT);
	
	ly.m_para.nQSwitchDly    = para.nQSwitchDly;
	ly.m_para.nQSoftEndDly   = para.nQSoftEndDly;
	*/
	ly.m_para = para;
	pDoc->SetLayer(ly);

	return;
}


void CProperDlg::DoUndoLayerPara()
{
	CHLDoc *pDoc = GetDocument();
	if(pDoc == NULL)
	{
		return;
	}

	int index = m_listLayer.GetCurSel();
	if(index == LB_ERR)
	{
		return;
	}
	else 
	{
		CLayer ly=pDoc->GetLayer(index);

		//第0层的处理
		if(index==0)
		{
			GetDlgItem(IDC_CHECK_DEFAULT)->ShowWindow(SW_HIDE);
			m_bDefault=FALSE;
		}
		else
		{
			GetDlgItem(IDC_CHECK_DEFAULT)->ShowWindow(SW_SHOW);
			m_bDefault=ly.m_bDefault;
		}

		ly.m_bDefault = m_bDefault;
		ly.m_nLayer=index;
		pDoc->SetLayCmd(PTY_PARA,ly);

		//更新打标参数列表
		SetPara(ly.m_para);

		ListLayerTextRefresh(pDoc);
	}

	return;
}

LRESULT CProperDlg::OnPropertyItemChange(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty* pProp = reinterpret_cast<CMFCPropertyGridProperty*>(lParam);	
	int nParaIndex=pProp->GetData();	

	if (nParaIndex==0) //限制打标次数，预防GetStrokeInQuad时内存崩溃
	{
		double dbRepeat = pProp->GetValue().dblVal;
		if (dbRepeat>10000)
			dbRepeat = 10000;
		pProp->SetValue(COleVariant(dbRepeat));
	}


	if (nParaIndex==11)	//电流
	{
		double dbCurrent = pProp->GetValue().dblVal;
		if (dbCurrent<g_sys.m_nLaserCurrentMin)
			dbCurrent = g_sys.m_nLaserCurrentMin;
		if (dbCurrent>g_sys.m_nLaserCurrentMax)
			dbCurrent = g_sys.m_nLaserCurrentMax;
		pProp->SetValue(COleVariant(dbCurrent));
	}
	

	CHLDoc *pDoc=GetDocument();
	if(pDoc==NULL)
	{
		return 0;
	}

	int index=m_listLayer.GetCurSel();
	if(index==LB_ERR)
	{
		//		CString str;
		//		str.LoadString(IDS_LAYER_NOSELECT);
		//		AfxMessageBox(str,MB_ICONWARNING);
	}
	else 
	{	
		//更新数据
		CLayer ly=pDoc->GetLayer(index);
		PARA OldPara;
		OldPara = ly.m_para;
		if (!(g_CurUserInfo.dwRight1 & 512))
		{
			//AfxMessageBox(IDS_RIGHT_NO);
			SetPara(ly.m_para);
		}
		else
		{
			GetPara(ly.m_para);
		}
		if (OldPara == ly.m_para) 
		{
			UpdateView();
			return 0;
		}
		pDoc->SetLay(PTY_PARA,ly);
		SetPara(ly.m_para);
		ListLayerTextRefresh(pDoc);
		UpdateView();
	}
	return 0;
}

int CProperDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_propertyGrid.Create(WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_STATIC_PARA);

	return 0;
}


void CProperDlg::OnClickedRadioSelectMarkType()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!UpdateData())
		return;	

	CHLDoc *pDoc=GetDocument();
	pDoc->GetChain().m_nSelectMarkType = m_nSelectMarkType;
	pDoc->UpdateAllViews(NULL);	
}


void CProperDlg::OnRadioSelectMarkType1()
{
	// TODO: 在此添加命令处理程序代码
	if (!UpdateData())
		return;

	CHLDoc *pDoc=GetDocument();
	pDoc->GetChain().m_nSelectMarkType = m_nSelectMarkType;
	pDoc->UpdateAllViews(NULL);	
}


void CProperDlg::OnBnClickedButtonChangeLaynum()
{
	// TODO: 在此添加控件通知处理程序代码

	CHLDoc *pDoc = GetDocument();
	if(pDoc == NULL)
	{
		return;
	}

	CString strLayNum;
	GetDlgItem(IDC_EDIT_LAYNUM)->GetWindowText(strLayNum);
	pDoc->m_dMarkLay = atoi(strLayNum);
}

void CProperDlg::ListLayerTextRefresh(CHLDoc *pDoc)
{
	int nLayerCount = pDoc->GetLaySize();
	for (int i = 0; i < nLayerCount; i++)
	{
		CLayer layer   = pDoc->GetLayer(i);
		char szNum[64];
		BOOL bLock = pDoc->GetLockFlg(i);
		if (bLock)
		{
			sprintf(szNum, "%d%s", i, "☆");
		}
		else
		{
			sprintf(szNum, "%d", i);
		}

		if (i==6)
			strcpy(szNum, "N/A");

		m_listLayer.SetItemText(i, 0, szNum);
		m_listLayer.SetItemText(i, 2, 
			m_manageLayPara.GetLayItemValue(0, (BYTE*)&layer.m_para, sizeof(PARA)));
		m_listLayer.SetItemText(i, 3, 
			m_manageLayPara.GetLayItemValue(1, (BYTE*)&layer.m_para, sizeof(PARA)));
		m_listLayer.SetItemText(i, 4, 
			m_manageLayPara.GetLayItemValue(2, (BYTE*)&layer.m_para, sizeof(PARA)));
	}
}