// VtrlListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "VtrlListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int BlockSize = 1024*256;

/////////////////////////////////////////////////////////////////////////////
// CVtrlListCtrl

CVtrlListCtrl::CVtrlListCtrl()
{
	m_dwItemCount = 0;
	m_Items.SetSize(BlockSize, 100);
}

CVtrlListCtrl::~CVtrlListCtrl()
{
}


BEGIN_MESSAGE_MAP(CVtrlListCtrl, CDsfListCtrl)
	//{{AFX_MSG_MAP(CVtrlListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVtrlListCtrl message handlers

void CVtrlListCtrl::SetStringAt(int iKey, const CString &str)
{
	m_keyStrings.SetAt(iKey, str);
}

BOOL CVtrlListCtrl::GetStringAt(int iKey, CString &str)
{
	return m_keyStrings.Lookup(iKey, str);
}

void CVtrlListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	LV_ITEM* pItem = &(pDispInfo->item);
	int item = pItem->iItem;
	CString strTemp("");

	VtrlItem& vItem = m_Items.ElementAt(item);
	if (pItem->mask & LVIF_TEXT)
	{
		switch(pItem->iSubItem)
		{
		case 2://ÐòºÅ
			{
				strTemp.Format("%d", item);
				strncpy(pItem->pszText, 
					strTemp.GetBuffer(strTemp.GetLength()), 
					pItem->cchTextMax);
				strTemp.ReleaseBuffer();
				break;
			}
		case 1://ÃèÊö
			{
				strTemp = "";
				m_keyStrings.Lookup(vItem.iKey, strTemp);
				if (0 == vItem.iKey)
				{
					int k = 0;
				}
				strncpy(pItem->pszText, 
					strTemp.GetBuffer(strTemp.GetLength()), 
					pItem->cchTextMax);
				strTemp.ReleaseBuffer();
				break;
			}
		case 3://±¸×¢
			{
				if (vItem.itemData.m_nType == DIDT_DSFOBJ)
				{
					CDsfObj *pDsfObj = (CDsfObj*)vItem.itemData.m_pData;
					switch(pDsfObj->GetDsfObjType())
					{
					case DSFOBJ_TIMER:
						{
							CDsfTimer *pTimer = (CDsfTimer*)pDsfObj;
							sprintf_s(pItem->pszText, pItem->cchTextMax, "%dms", pTimer->GetTime());
						}
						break;
					case DSFOBJ_DOOUPUT:
						{
							CDsfDoOutput *pDoOutput = (CDsfDoOutput*)pDsfObj;
							UINT nOutput,nMask;
							pDoOutput->GetOutput(nOutput, nMask);
							sprintf_s(pItem->pszText, pItem->cchTextMax, "...");
						}
						break;
					case DSFOBJ_WAITINPUT:
						{
							CDsfWaitInput *pWaitInput = (CDsfWaitInput*)pDsfObj;
							UINT nInput,nMask;
							pWaitInput->GetOutput(nInput, nMask);
							sprintf_s(pItem->pszText, pItem->cchTextMax, "...");
						}
						break;
					}
				}
				break;
			}
		}
	}
	if (pItem->mask & LVIF_IMAGE)
	{
		pItem->iImage = vItem.iImage;
	}
	*pResult = 0;
}

void CVtrlListCtrl::ScrollToItem(int index)
{
	int temp = index;
	while(GetTopIndex()!=index)
	{
		if (temp >= GetItemCount())
		{
			break;
		}
		EnsureVisible(temp++, TRUE);
	}
}

int CVtrlListCtrl::InsertItem(int nItem, int iKey, int iImage)
{
	if (m_dwItemCount==(DWORD)m_Items.GetSize())
	{
		m_Items.SetSize(m_dwItemCount+BlockSize);
	}

	int result = 0;
	if ((DWORD)nItem >= m_dwItemCount)
	{
		m_Items[m_dwItemCount].iKey = iKey;
		m_Items[m_dwItemCount].iImage = iImage;
		m_dwItemCount++;
		result = m_dwItemCount-1;
	}
	else
	{
		m_Items[nItem].iKey = iKey;
		m_Items[nItem].iImage = iImage;
		m_dwItemCount++;
		result = nItem;
	}

	return result;
}

BOOL CVtrlListCtrl::DeleteItem(int nItem)
{
	BOOL result = FALSE;
	if ((DWORD)nItem >= m_dwItemCount)
	{
		result = FALSE;
	}
	else
	{
		m_Items.RemoveAt(nItem);
		m_dwItemCount--;
		result = TRUE;
	}
	return result;
}

BOOL CVtrlListCtrl::DeleteAllItems()
{
	m_Items.RemoveAll();
	m_Items.SetSize(BlockSize);
	m_dwItemCount = 0;
	return TRUE;
}

void CVtrlListCtrl::UpdateListCtrl()
{
	SetItemCountEx(m_dwItemCount, LVSICF_NOINVALIDATEALL|LVSICF_NOSCROLL);
	Invalidate();
}

int CVtrlListCtrl::GetItemCount() const
{
	return (int)m_dwItemCount;
}
const DsfItemData &CVtrlListCtrl::GetItemData(int nItem) const
{
	return m_Items.GetAt(nItem).itemData;
}
void CVtrlListCtrl::SetItemData(int nItem, const DsfItemData &itemData)
{
	m_Items.GetAt(nItem).itemData.m_nType = itemData.m_nType;
	m_Items.GetAt(nItem).itemData.m_pData = itemData.m_pData;
}

void CVtrlListCtrl::SetBackColor(COLORREF crColor)
{
	m_crBackground = crColor;
	m_brBackground.DeleteObject();
	m_brBackground.CreateSolidBrush(m_crBackground);
}

BOOL CVtrlListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rectClient;
	GetClientRect(rectClient);
	pDC->FillRect(rectClient, &m_brBackground);
	return TRUE;
}

void CVtrlListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)    
{    
	// TODO: Add your control notification handler code here 
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = CDRF_DODEFAULT;
 	switch(pLVCD->nmcd.dwDrawStage) 
	{    
	case CDDS_PREPAINT:
		{    
			*pResult = CDRF_NOTIFYITEMDRAW; 
			break;    
		}    
	case CDDS_ITEMPREPAINT:   
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW; 
			break;
		} 
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:   
		{
			pLVCD->clrTextBk = m_crBackground;

			int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
			POSITION pos = GetFirstSelectedItemPosition();
			while(pos)
			{
				int nSelItem = GetNextSelectedItem(pos);
				if (nItem==nSelItem)
				{
					pLVCD->clrText = RGB(255,255,255);
					pLVCD->clrTextBk = RGB(49,106,197);
					break;
				}
			}
		
			*pResult = CDRF_DODEFAULT;
			break;   
		} 
	}    
}   