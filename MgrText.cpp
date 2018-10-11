// MgrText.cpp: implementation of the CMgrText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "MgrText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMgrText* g_MgrText = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMgrText::CMgrText()
{
	m_nTextNextID = 1000;
	m_bIsCopy = FALSE;
	m_nTextBase = 0;
}

CMgrText::~CMgrText()
{
	Delete();
}

void CMgrText::Delete()
{
	//删除所有的树
	POSITION pos=m_list.GetHeadPosition();
	CText *pTxt;
	while(pos!=NULL)
	{
		pTxt=m_list.GetNext(pos);
		delete pTxt;
	}
	m_list.RemoveAll();	
}

void CMgrText::operator =(const CMgrText &src)
{
	Delete();
	//加入新的树
	POSITION pos;
	CText *pTxt;
	pos=src.m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pTxt=src.m_list.GetNext(pos);
		
		CText *pNTxt=new CText(pTxt->m_nTextID);
		*pNTxt = *pTxt;
		m_list.AddTail(pNTxt);
	}
}

void CMgrText::AddText(CText* pText)
{
	m_list.AddTail(pText);
}

UINT CMgrText::GetNextID()
{
	return m_nTextNextID++;
}

BOOL CMgrText::IsOverMark(UINT nID,DWORD nStyle)
{
	BOOL bOverMark = FALSE;
	CText* pText;
	POSITION pos=m_list.GetHeadPosition();	
	while(pos!=NULL)	
	{
		pText = m_list.GetNext(pos);
		if(pText->m_nTextID == nID)
		{
			bOverMark = pText->m_bOverMark;
			break;
		}
	}
	return bOverMark;
}
void CMgrText::GetString(UINT nID,CString &strInfo,DWORD nStyle)
{
	BOOL bFind = FALSE;
	CText* pText;
	POSITION pos=m_list.GetHeadPosition();
	strInfo.Empty();

	while(pos!=NULL)	
	{
		pText = m_list.GetNext(pos);
		if(pText->m_nTextID == nID)
		{
			strInfo = pText->GetString(nStyle);
			bFind = TRUE;
			break;
		}
	}
}

void CMgrText::SetNextID(UINT nID)
{
	m_nTextNextID = nID;
}

CText* CMgrText::GetAt(UINT nIndex)
{
	CText* pText=NULL;

	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pText = m_list.GetNext(pos);
		if(pText->m_nTextID == nIndex)
			break;
	}
	return pText;
}

BOOL CMgrText::IsExist(UINT nIndex)
{
	CText* pText=NULL;
	
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pText = m_list.GetNext(pos);
		if(pText->m_nTextID == nIndex)
			return TRUE;
	}
	return FALSE;	
}

void CMgrText::SaveText(FILE* fp,BOOL bFlag)
{
	CText* pText=NULL;

	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pText = m_list.GetNext(pos);
		pText->Serialize(fp,bFlag);
	}	
}

UINT CMgrText::GetLastID()
{
	CText* pText=NULL;
	UINT nRtn = 1000;
	
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pText = m_list.GetNext(pos);
		if(nRtn<pText->m_nTextID)
		{
			nRtn = pText->m_nTextID;
		}
	}

	return (nRtn+1);
}

void CMgrText::SetCopySts(BOOL bFlag)
{
	m_bIsCopy = bFlag;
	if(bFlag)
	{
		//设置基地址
		m_nTextBase=GetLastID();
	}
	else{
		//设置最后的一个号
		m_nTextNextID = GetLastID();
	}
}

UINT CMgrText::GetBaseID()
{
	return m_nTextBase;
}

BOOL CMgrText::GetCopySts()
{
	return m_bIsCopy;
}

void CMgrText::SetBaseID(UINT nID)
{
	//用于被复制方做的设置基本点ID的值
	m_nTextBase = nID;
}

void CMgrText::Add(CMgrText *pMgrText)
{
	CText* pText=NULL;
	CText* pAdd = NULL;
	
	POSITION pos=pMgrText->m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pText = pMgrText->m_list.GetNext(pos);
		pAdd = new CText(0);
		*pAdd = *pText;
		AddText(pAdd);
	}
}

void CMgrText::SetUse(BOOL bFlag)
{
	CText* pText=NULL;
	
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pText = m_list.GetNext(pos);
		pText->m_bUse = bFlag;
	}
}

BOOL CMgrText::FlashNextText(DWORD nStyle)
{
	//如果是确定是否刷下一批数据
	if(nStyle==0x03)
		return TRUE;

	CText* pText=NULL;
	
	BOOL rtn = FALSE;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pText = m_list.GetNext(pos);
		if(pText->FlashNextText(nStyle))
			rtn = TRUE;
	}
	return rtn;
}

void CMgrText::FlashText(UINT nTextID, DWORD nStyle)
{
	BOOL bFind = FALSE;
	CText* pText;
	POSITION pos=m_list.GetHeadPosition();
	
	while(pos!=NULL)	
	{
		pText = m_list.GetNext(pos);
		if(pText->m_nTextID == nTextID)
		{
			pText->FlashNextText(nStyle);
			bFind = TRUE;
			break;
		}
	}	
}

void CMgrText::Clear()
{
	CText* pText;
	POSITION pre;
	POSITION pos=m_list.GetHeadPosition();
	
	while(pos!=NULL)	
	{
		pre=pos;
		pText = m_list.GetNext(pos);
		if(!pText->m_bUse){
			delete pText;
			m_list.RemoveAt(pre);
		}
	}		
}

void CMgrText::DeleteByID(UINT nTextID)
{
	//删除所有的树
	POSITION pos = m_list.GetHeadPosition();
	CText *pTxt;
	POSITION pre = NULL;
	while(pos!=NULL)
	{
		pre = pos;
		pTxt=m_list.GetNext(pos);
		if (pTxt->m_nTextID == nTextID)
		{
			delete pTxt;
			pTxt = NULL;
			m_list.RemoveAt(pre);
		}
	}

	return;
}


BOOL CMgrText::ChangeText(CString strTxt,CString ModelNo)
{
	BOOL  bOK=FALSE;
	CText* pText=NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pText=m_list.GetNext(pos);
		bOK|=pText->ChangeText(strTxt,ModelNo);
		pText->m_Changed = true;
	}

	return bOK;
}

BOOL CMgrText::ChangeText(CString strTxt,UINT textid){
	bool succeed=false;
	CText* pText=NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pText=m_list.GetNext(pos);
		if(pText->m_nTextID==textid){

			//CHLDoc	*pDoc = (CHLDoc*)CHLView::m_pActiveView->GetDocument();
			//auto s_chain = &(pDoc->m_chain);
			//CGroupObj *pGroupObj = NULL;
			//POSITION pos1 = s_chain->m_list.GetHeadPosition();
			//while (pos1 != NULL)
			//{
			//	pGroupObj = (CGroupObj *)s_chain->m_list.GetNext(pos1);
			//	pGroupObj->m_dwObjId
			//}

			pText->m_strFix=pText->m_strText=strTxt;
			succeed=true;
			break;
		}
	}
	return succeed;
}
