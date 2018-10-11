// ObjectListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ObjectListDlg.h"
#include "HLView.h"
#include "DsfDlgInsertObj.h"
#include "DsfDlgEditTime.h"
#include "DsfDlgEditIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectListDlg dialog


CObjectListDlg::CObjectListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_listInfo.RemoveAll();
	m_nCurItem = -1;
}


void CObjectListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectListDlg)
	DDX_Control(pDX, IDC_LIST_DOC_OBJECT, m_DocObjList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectListDlg, CDialog)
	//{{AFX_MSG_MAP(CObjectListDlg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DOC_OBJECT, OnClickListDocObject)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DOC_OBJECT, OnRclickListDocObject)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_DOC_OBJECT, OnBeginlabeleditListDocObject)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_DOC_OBJECT, OnRdblclkListDocObject)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOC_OBJECT, OnDblclkListDocObject)
	ON_COMMAND(ID_OBJLIST_DEL, ObjDel)
	ON_COMMAND(ID_LOAD_OBJLIST, ObjListLoad)
	ON_COMMAND(ID_UNLOAD_OBJLIST, ObjListUnLoad)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DOC_OBJECT, &CObjectListDlg::OnLvnItemchangedListDocObject)
//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectListDlg message handlers

void CObjectListDlg::OnClickListDocObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (pNMHDR == NULL) return;

	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;
	
	int nCount = m_DocObjList.GetItemCount();
	int nItem    = m_DocObjList.HitTest( point, NULL);
	if ((-1 == nItem) || (nCount <= 0) || (nItem >= nCount))
	{
		*pResult = 0;
		return;
	}

	m_nCurItem = nItem;

	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc)
		return;

	int nSelCount = m_DocObjList.GetSelectedCount();
	if (nSelCount<=0)
		return;

	pDoc->SetSelect(FALSE);
	
	POSITION pos = m_DocObjList.GetFirstSelectedItemPosition();
	while (pos) 
	{
		int nIndex = m_DocObjList.GetNextSelectedItem(pos);
		const DsfItemData &itemData = m_DocObjList.GetItemData(nIndex);
		
		CGroupObj *pGroupObj = (CGroupObj *)itemData.m_pData;
		if (NULL == pGroupObj)
			continue;	

		pDoc->SetGroupObjSel(pGroupObj);
	}


	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(pView)
	{
		pView->DispSelObjProp();
	}

	
	*pResult = 0;
}

CHLDoc * CObjectListDlg::GetDocument()
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

BOOL CObjectListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DocObjList.SetBackColor(RGB(236,233,216));

	m_DocObjList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CString str("");
	str.LoadString(IDS_NAME);
	m_DocObjList.InsertColumn(0,(LPCTSTR)str,LVCFMT_LEFT,40,50);
	str.LoadString(IDS_TYPE);
	m_DocObjList.InsertColumn(1,(LPCTSTR)str,LVCFMT_LEFT,50,50);
	str.LoadString(IDS_NUM);
	m_DocObjList.InsertColumn(2,(LPCTSTR)str,LVCFMT_LEFT,50,50);
	str.LoadString(IDS_RECOMMENT);
	m_DocObjList.InsertColumn(3,(LPCTSTR)str,LVCFMT_LEFT,50,50);

	HICON hIcon[17];
	m_ImageList.Create(16,16,ILC_COLORDDB|ILC_MASK|ILC_COLOR24|ILC_COLOR32, 8, 8); // 32, 32 for large icons
	hIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON_UNKNOW);    // 未知节点
	hIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON_POLYLINE);  // 曲线 
	hIcon[2] = AfxGetApp()->LoadIcon(IDI_ICON_RECTANGLE); // 矩形
	hIcon[3] = AfxGetApp()->LoadIcon(IDI_ICON_TEXT);      // 文本
	hIcon[4] = AfxGetApp()->LoadIcon(IDI_ICON_TEXT);      // 跳号
	hIcon[5] = AfxGetApp()->LoadIcon(IDI_ICON_BMP);       // BMP
	hIcon[6] = AfxGetApp()->LoadIcon(IDI_ICON_ELLIPSE);   // 圆
	hIcon[7] = AfxGetApp()->LoadIcon(IDI_ICON_PLT);       // PLT
	hIcon[8] = AfxGetApp()->LoadIcon(IDI_ICON_GRID);      // 网格
	hIcon[9] = AfxGetApp()->LoadIcon(IDI_ICON_POLYGON);   // 多边形
	hIcon[10] = AfxGetApp()->LoadIcon(IDI_ICON_DXF);      // DXF
	hIcon[11] = AfxGetApp()->LoadIcon(IDI_ICON_CODEBAR);  // 二维条码
    hIcon[12] = AfxGetApp()->LoadIcon(IDI_ICON_COMBINOBJ);// 组合对象
	hIcon[13] = AfxGetApp()->LoadIcon(IDI_ICON_UNION);		//群组对象

	hIcon[14] = AfxGetApp()->LoadIcon(IDI_ICON_DOT);		  //IO

	hIcon[15] = AfxGetApp()->LoadIcon(IDI_ICON_IO);		  //IO
	hIcon[16] = AfxGetApp()->LoadIcon(IDI_ICON_IO);		  //IO

	
		
	for (int i = 0; i < 17; i++) 
	{
		m_ImageList.Add(hIcon[i]);
	}

	str.LoadString(IDS_UNKNOW);
	m_DocObjList.SetStringAt(NODE_BASE, str);

	str.LoadString(IDS_LINES);
	m_DocObjList.SetStringAt(NODE_LINE, str);

	str.LoadString(IDS_RECT);
	m_DocObjList.SetStringAt(NODE_RECT, str);

	str.LoadString(IDS_TEXT);
	m_DocObjList.SetStringAt(NODE_TEXT_DSF, str);

	str.LoadString(IDS_JUMPSTRING);
	m_DocObjList.SetStringAt(NODE_SERIAL, str);

	str.LoadString(IDS_BMP);
	m_DocObjList.SetStringAt(NODE_BMP, str);

	str.LoadString(IDS_ELLIPSE);
	m_DocObjList.SetStringAt(NODE_ELLIPSE, str);

	str.Format("%s", "PLT");
	m_DocObjList.SetStringAt(NODE_PLT, str);
	
	str.LoadString(IDS_GRID);
	m_DocObjList.SetStringAt(NODE_GRID, str);

	str.LoadString(IDS_POLYGON);
	m_DocObjList.SetStringAt(NODE_POLYGON, str);

	str.Format("%s", "DXF");
	m_DocObjList.SetStringAt(NODE_DXF, str);

	str.LoadString(IDS_CODEBAR);
	m_DocObjList.SetStringAt(NODE_CODEBAR, str);

	str.LoadString(IDS_GROUP);
	m_DocObjList.SetStringAt(NODE_COMBIN, str);



	str.LoadString(IDS_UNION);
	m_DocObjList.SetStringAt(13, str);


	str.Format("%s", "Dot");
	m_DocObjList.SetStringAt(NODE_DOT, str);

	
	str.LoadString(IDS_DSFOBJ_DOOUTPUT);
	m_DocObjList.SetStringAt(15, str);

	str.LoadString(IDS_DSFOBJ_WAITINPUT);
	m_DocObjList.SetStringAt(16, str);

	m_DocObjList.SetImageList(&m_ImageList, LVSIL_SMALL);

	m_DocObjList.GetHeaderCtrl().EnableMultipleSort(FALSE);
	m_DocObjList.EnableMarkSortedColumn(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CObjectListDlg::AddClmItem(CGroupObj *pGroupObj)
{
	if (NULL == pGroupObj)
		return FALSE;

	int nObjType = pGroupObj->GetType();	

	char szMsg[32];
	int nCount = m_DocObjList.GetItemCount();
	if (nCount != -1)
	{
		CString str("");
		switch(nObjType) 
		{
		case NODE_BASE:
		case NODE_LINE:
		case NODE_RECT:
		case NODE_TEXT_DSF:
		case NODE_SERIAL:
		case NODE_BMP:
		case NODE_ELLIPSE:
		case NODE_PLT:
		case NODE_GRID:
		case NODE_POLYGON:
		case NODE_DXF:
		case NODE_CODEBAR:
		case NODE_COMBIN:
		case NODE_UNION:
		case NODE_DOT:
			break;
		default:
			nObjType = NODE_BASE;
			break;
		}
		
		char szNum[32];
		sprintf(szNum, "%d", nCount);

		InsertColmItem(nCount, 1, szMsg, szNum, nObjType);
		DsfItemData itemData;
		itemData.m_nType = DIDT_TREE;
		itemData.m_pData = pGroupObj;
		m_DocObjList.SetItemData(nCount, itemData);
	}

	return TRUE;
}

BOOL CObjectListDlg::DelClmItem(CGroupObj *pGroupObj)
{
	int nCount = m_DocObjList.GetItemCount();
	if (nCount <= 0)
		return FALSE;

		for (int i = 0; i < nCount; i++)
	{
		const DsfItemData &itemData = m_DocObjList.GetItemData(i);
		if ((itemData.m_nType==DIDT_TREE) && ((DWORD)itemData.m_pData) == ((DWORD)pGroupObj))
		{
			m_DocObjList.DeleteItem(i);
		    nCount = m_DocObjList.GetItemCount();
			break;
		}
	}

	return TRUE;
}

BOOL CObjectListDlg::AddClmItem(CDsfObj *pObj)
{
	if (NULL == pObj)
	{
		return FALSE;
	}
	int nCount = m_DocObjList.GetItemCount();
	if (nCount != -1)
	{
		int nObjType = 13;
		switch(pObj->GetDsfObjType())
		{
		case DSFOBJ_TIMER:
			nObjType = 13;
			break;
		case DSFOBJ_DOOUPUT:
			nObjType = 14;
			break;
		case DSFOBJ_WAITINPUT:
			nObjType = 15;
			break;
		}
		char szMsg[32];
		char szNum[32];
		sprintf(szNum, "%d", nCount);

		InsertColmItem(nCount, 1, szMsg, szNum, nObjType);
		DsfItemData itemData;
		itemData.m_nType = DIDT_DSFOBJ;
		itemData.m_pData = pObj;
		m_DocObjList.SetItemData(nCount, itemData);
	}

	return TRUE;
}
BOOL CObjectListDlg::DelClmItem(CDsfObj *pObj)
{
	int nCount = m_DocObjList.GetItemCount();
	if (nCount <= 0)
	{
		return FALSE;
	}

	for (int i = 0; i < nCount; i++)
	{
		const DsfItemData &itemData = m_DocObjList.GetItemData(i);
		if ((itemData.m_nType==DIDT_DSFOBJ) && ((DWORD)itemData.m_pData) == ((DWORD)pObj))
		{
			m_DocObjList.DeleteItem(i);
			nCount = m_DocObjList.GetItemCount();
			break;
		}
	}

	return TRUE;
}

BOOL CObjectListDlg::DelAllClmItem()
{
	m_DocObjList.DeleteAllItems();
	return TRUE;
}

BOOL CObjectListDlg::InsertColmItem(int nItem, int nSubItem, LPCTSTR lpszText, LPCTSTR lpszNum, int nImage)
{
	m_DocObjList.InsertItem(nItem , nImage, nImage);
//	m_DocObjList.SetItemText(nItem, nSubItem , lpszText);
//	m_DocObjList.SetItemText(nItem, nSubItem + 1, lpszNum);
	return TRUE;
}

BOOL CObjectListDlg::DelColmItem(int nItem)
{
	if ((m_DocObjList.GetItemCount() -1) < nItem)
		return FALSE;

	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc)
		return FALSE;
		
	const DsfItemData &itemData = m_DocObjList.GetItemData(nItem);

	CGroupObj *pGoupObj = (CGroupObj*)itemData.m_pData;
	pDoc->DelObj(pGoupObj);

	m_DocObjList.DeleteItem(nItem);
	m_DocObjList.UpdateListCtrl();

	return TRUE;

}


BOOL CObjectListDlg::SetHeadings( const CString& strHeadings )
{
	int nNumColumns =0;
	int iStart = 0;
	for( ;; )
	{
		const int iComma = strHeadings.Find( _T(','), iStart );
		if( iComma == -1 )
		{
			break;
		}
		
		const CString strHeading = strHeadings.Mid( iStart, iComma - iStart );
		iStart = iComma + 1;
		int iSemiColon = strHeadings.Find( _T(';'), iStart );

		if( iSemiColon == -1 )
		{
			iSemiColon = strHeadings.GetLength();
		}

		const int iWidth = atoi( strHeadings.Mid( iStart, iSemiColon - iStart ) );
		iStart = iSemiColon + 1;

		switch(nNumColumns)
		{
		case 0:
			{
				if(m_DocObjList.InsertColumn(nNumColumns, strHeading, LVCFMT_LEFT, iWidth ) == -1)
				{
					return FALSE;
				}
				break;
			}
		case 1:
			{
				if(m_DocObjList.InsertColumn(nNumColumns, strHeading, LVCFMT_LEFT, iWidth ) == -1)
				{
					return FALSE;
				}
				break;
			}
		case 2:
			{
				if(m_DocObjList.InsertColumn(nNumColumns, strHeading, LVCFMT_CENTER, iWidth ) == -1)
				{
					return FALSE;
				}
				break;
			}
		default:
			{
				if(m_DocObjList.InsertColumn(nNumColumns, strHeading, LVCFMT_CENTER, iWidth ) == -1)
				{
					return FALSE;
				}
				break;
			}
		}

		nNumColumns++;
	}

	return TRUE;
}

void CObjectListDlg::OnRclickListDocObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (pNMHDR == NULL) return;

	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;

	int nCount = m_DocObjList.GetItemCount();
	int nItem    = m_DocObjList.HitTest( point, NULL);
	if ((-1 == nItem) || (nCount <= 0) || (nItem >= nCount))
	{
		*pResult = 0;
		return;
	}

	m_nCurItem = nItem;

	CMenu menu,*pMenu;
	menu.LoadMenu(IDR_POP);
	pMenu=menu.GetSubMenu(6);
	if(pMenu)
	{
		POINT point;
		NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	    point = pNMListView->ptAction;
		m_DocObjList.ClientToScreen(&point);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
	SetFocus();
	
	*pResult = 0;
}


BOOL CObjectListDlg::PreTranslateMessage(MSG* pMsg) 
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
	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_DELETE) & 0x8000))
	{
		ObjDel();
		return TRUE;
	}
	
	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_SPACE) & 0x8000))
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CObjectListDlg::ObjDel() 
{
	DelColmItem(m_nCurItem);
	return;
}

void CObjectListDlg::ObjListLoad() 
{
	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	ObjListUnLoad();

	pDoc->ObjListLoad();

	return;
}

void CObjectListDlg::ObjListUnLoad() 
{
	DelAllClmItem();
	return;
}


void CObjectListDlg::OnBeginlabeleditListDocObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pNMHDR == NULL) return;

	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;

	int nCount = m_DocObjList.GetItemCount();
	int nItem    = m_DocObjList.HitTest( point, NULL);
	if ((-1 == nItem) || (nCount <= 0) || (nItem >= nCount))
	{
		*pResult = 0;
		return;
	}

	m_nCurItem = nItem;
	
	*pResult = 0;
}

void CObjectListDlg::OnRdblclkListDocObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (pNMHDR == NULL) return;

	CPoint point;
	NMLISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	point = pNMListView->ptAction;

	int nCount = m_DocObjList.GetItemCount();
	int nItem    = m_DocObjList.HitTest( point, NULL);
	if ((-1 == nItem) || (nCount <= 0) || (nItem >= nCount))
	{
		*pResult = 0;
		return;
	}

	m_nCurItem = nItem;

	const DsfItemData &itemData = m_DocObjList.GetItemData(m_nCurItem);
	switch(itemData.m_nType)
	{
	case DIDT_DSFOBJ:
		break;
	case DIDT_TREE:
		{
			CTree *pTree = (CTree *)itemData.m_pData;
			BOOL bIsGroup = pTree->IsGroup();
			if (bIsGroup)
			{
				//		ObjListUnLoad();
			}
		}
		break;
	default:
		break;
	}
	
	*pResult = 0;
}

void CObjectListDlg::OnDblclkListDocObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}


void CObjectListDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	SetFocus();
//	m_DocObjList.SetFocus();
	CDialog::OnMouseMove(nFlags, point);
}

int CObjectListDlg::SetSelByGroupObj(CGroupObj *pGroupObj)
{
	int nIndex = -1;
	int nCount = m_DocObjList.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		const DsfItemData &itemData = m_DocObjList.GetItemData(i);

		CGroupObj *pGroupObjTemp = (CGroupObj *)itemData.m_pData;
		if (NULL != pGroupObjTemp)
		{
			if (pGroupObjTemp == pGroupObj)
			{
				if(nIndex == -1) nIndex = i;
				m_DocObjList.SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
				m_nCurItem = i;
				break;
			}
		}
	}

	return nIndex;
}

void CObjectListDlg::SetObjUnSel() 
{
	int nCount = m_DocObjList.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		m_DocObjList.SetItemState(i, 0, LVIS_SELECTED|LVIS_FOCUSED);
	}
	return;
}

void CObjectListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (GetSafeHwnd() && m_DocObjList.GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		m_DocObjList.MoveWindow(&rect);
	}
}


int CObjectListDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CObjectListDlg::OnAddListObj()
{/*
	CHLDoc *pDoc = GetDocument();
	CDsfDlgInsertObj dlg;
	if (dlg.DoModal() == IDOK)
	{
		dlg.Check();
		int nItem = m_nCurItem;
		CDsfObj *pObj = NULL;
		switch(dlg.m_nObjType)
		{
		case DSFOBJ_TIMER:
			pObj = new CDsfTimer;
			((CDsfTimer*)pObj)->SetTime(dlg.m_nTime);
			break;
		case DSFOBJ_DOOUPUT:
			pObj = new CDsfDoOutput;
			((CDsfDoOutput*)pObj)->SetOutput(dlg.m_nOutput, dlg.m_nOtMask);
			break;
		case DSFOBJ_WAITINPUT:
			pObj = new CDsfWaitInput;
			((CDsfWaitInput*)pObj)->SetOutput(dlg.m_nInput, dlg.m_nItMask);
			break;
		}
		if (m_nCurItem == -1)
		{
			switch(dlg.m_nSortType)
			{
			case DSFST_OBJ_BEGIN:
				dlg.m_nSortType = DSFST_DOC_BEGIN;
				break;
			case DSFST_OBJ_END:
				dlg.m_nSortType = DSFST_DOC_END;
				break;
			}
		}
		switch(dlg.m_nSortType)
		{
		case DSFST_DOC_BEGIN:
			{
				pDoc->m_chain.GetTreeList().AddHead(pObj);
			}
			break;
		case DSFST_DOC_END:
			{
				pDoc->m_chain.GetTreeList().AddTail(pObj);
			}
			break;
		case DSFST_OBJ_BEGIN:
			{
				const DsfItemData &itemData = m_DocObjList.GetItemData(m_nCurItem);
				IListType *pType = (IListType *)itemData.m_pData;
				POSITION pos = pDoc->m_chain.GetTreeList().Find(pType);
				pDoc->m_chain.GetTreeList().InsertBefore(pos, pObj);
			}
			break;
		case DSFST_OBJ_END:
			{
				const DsfItemData &itemData = m_DocObjList.GetItemData(m_nCurItem);
				IListType *pType = (IListType *)itemData.m_pData;
				POSITION pos = pDoc->m_chain.GetTreeList().Find(pType);
				pDoc->m_chain.GetTreeList().InsertAfter(pos, pObj);
			}
			break;
		}
		ObjListLoad();
	}
	*/
}
void CObjectListDlg::OnDelListObj()
{
	DelColmItem(m_nCurItem);
}
void CObjectListDlg::OnEditListObj()
{
	if (m_nCurItem == -1)
	{
		return;
	}
	const DsfItemData &itemData = m_DocObjList.GetItemData(m_nCurItem);
	if (itemData.m_nType == DIDT_DSFOBJ)
	{
		CDsfObj *pObj = (CDsfObj*)itemData.m_pData;
		switch(pObj->GetDsfObjType())
		{
		case DSFOBJ_TIMER:
			{
				CDsfDlgEditTime dlg;
				dlg.m_nTime = ((CDsfTimer*)pObj)->GetTime();
				if(dlg.DoModal() == IDOK)
				{
					((CDsfTimer*)pObj)->SetTime(dlg.m_nTime);
				}
			}
			break;
		case DSFOBJ_DOOUPUT:
			{
				CDsfDlgEditIO dlg;
				dlg.m_strTitle.LoadString(IDS_DSFOBJ_DOOUTPUT);
				 ((CDsfDoOutput*)pObj)->GetOutput(dlg.m_nValue, dlg.m_nMask);
				 if(dlg.DoModal() == IDOK)
				 {
					 ((CDsfDoOutput*)pObj)->SetOutput(dlg.m_nValue, dlg.m_nMask);
				 }
			}
			break;
		case  DSFOBJ_WAITINPUT:
			{
				CDsfDlgEditIO dlg;
				dlg.m_strTitle.LoadString(IDS_DSFOBJ_WAITINPUT);
				((CDsfWaitInput*)pObj)->GetOutput(dlg.m_nValue, dlg.m_nMask);
				if(dlg.DoModal() == IDOK)
				{
					((CDsfWaitInput*)pObj)->SetOutput(dlg.m_nValue, dlg.m_nMask);
				}
			}
			break;
		}
	}
}
void CObjectListDlg::OnUpListObj()
{
	CHLDoc *pDoc = GetDocument();
	if (m_nCurItem==-1 || m_nCurItem==0)
		return;

	const DsfItemData &itemData = m_DocObjList.GetItemData(m_nCurItem);
	const DsfItemData &itemDataPre = m_DocObjList.GetItemData(m_nCurItem-1);
	CGroupObj *pGroup1 = (CGroupObj*)itemData.m_pData;
	CGroupObj *pGroup2 = (CGroupObj*)itemDataPre.m_pData;
	POSITION pos1 = pDoc->m_chain.m_list.Find(pGroup1);
	POSITION pos2 = pDoc->m_chain.m_list.Find(pGroup2);
	pDoc->m_chain.m_list.SetAt(pos1, pGroup2);
	pDoc->m_chain.m_list.SetAt(pos2, pGroup1);
	ObjListLoad();
	m_nCurItem--;
	SetObjUnSel();
	m_DocObjList.SetItemState(m_nCurItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	
}
void CObjectListDlg::OnDownListObj()
{
	CHLDoc *pDoc = GetDocument();
	if (m_nCurItem==-1 || m_nCurItem==m_DocObjList.GetItemCount()-1)
		return;

	const DsfItemData &itemData = m_DocObjList.GetItemData(m_nCurItem);
	const DsfItemData &itemDataPre = m_DocObjList.GetItemData(m_nCurItem+1);
	CGroupObj *pGroup1 = (CGroupObj*)itemData.m_pData;
	CGroupObj *pGroup2 = (CGroupObj*)itemDataPre.m_pData;
	POSITION pos1 = pDoc->m_chain.m_list.Find(pGroup1);
	POSITION pos2 = pDoc->m_chain.m_list.Find(pGroup2);
	pDoc->m_chain.m_list.SetAt(pos1, pGroup2);
	pDoc->m_chain.m_list.SetAt(pos2, pGroup1);
	ObjListLoad();
	m_nCurItem++;
	SetObjUnSel();
	m_DocObjList.SetItemState(m_nCurItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
}

void CObjectListDlg::OnLvnItemchangedListDocObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	*pResult = 0;
}
