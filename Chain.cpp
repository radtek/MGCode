// Chain.cpp: implementation of the CChain class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "Chain.h"
#include "Tree.h"
#include "HLView.h"
#include "KBChar.h"
#include "NodePlt.h"
#include "Mainfrm.h"
#include "MonitorQuad.h"
#include "DivideLayer.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CChain::selectNull	=1;
int CChain::selectSingle=2;
int CChain::selectMul	=4;
int CChain::selectGroup	=8;
int CChain::selectUnion =16;

extern CHLApp theApp;

CChain::CChain()
{
	m_bSelMark = FALSE;
	m_nSelectMarkType = 0;
}

CChain::~CChain()
{
	DeleteList();
}

void CChain::DeleteList()
{	
	CGroupObj *pGroupObj  = NULL;
	POSITION pos  = m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj=m_list.GetNext(pos);
		delete pGroupObj;
	}
	m_list.RemoveAll();
}

void CChain::AddList(const CChain &src)
{
	CGroupObj *pGroupObj  = NULL;
	POSITION pos=src.m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj=(CGroupObj*)src.m_list.GetNext(pos);
		CGroupObj *pNewGroupObj = new CGroupObj(pGroupObj);
		m_list.AddTail(pNewGroupObj);
	}
}

void CChain::operator=(const CChain& src)
{ 
	//文本内容的复制
	m_MgrText = src.m_MgrText;

	//删除所有的树
	DeleteList();

	//加入新的树
	AddList(src);

	//晶圆对象复制
	m_listCopyObject.RemoveAll();
	POSITION pos = src.m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject obj = src.m_listCopyObject.GetNext(pos);
		m_listCopyObject.AddTail(obj);
	}
}


void CChain::Add(CGroupObj *pGroupObj)
{
	m_list.AddTail(pGroupObj);
}

void CChain::Draw(XDC *pDC)
{	
	CRect rectClip;
	CRect rect;
	pDC->GetClipBox(&rectClip);
	rectClip.InflateRect(1,1);

	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{			
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);

		rect=(CRect)pGroupObj->GetLimit();
		rect.NormalizeRect();
		rect.InflateRect(3,3);
		pGroupObj->Draw(pDC);
	}

	//LZQ
	POSITION posObj = m_listCopyObject.GetHeadPosition();
	if (posObj)
	{
		CPen penBlack(PS_SOLID,1, RGB(0,0,0));
		CPen penRed(PS_SOLID,1, RGB(255,0,0));

		CPen *oldPen=pDC->SelectObject(&penRed);

		while(posObj)
		{			
			CCopyObject &obj = m_listCopyObject.GetNext(posObj);

			if (obj.m_bSelect)
				pDC->SelectObject(penRed);
			else
				pDC->SelectObject(penBlack);

			pDC->MoveTo(obj.m_quadLimit.left, obj.m_quadLimit.top);
			pDC->LineTo(obj.m_quadLimit.right, obj.m_quadLimit.top);
			pDC->LineTo(obj.m_quadLimit.right, obj.m_quadLimit.bottom);
			pDC->LineTo(obj.m_quadLimit.left, obj.m_quadLimit.bottom);
			pDC->LineTo(obj.m_quadLimit.left, obj.m_quadLimit.top);
		}

		pDC->SelectObject(oldPen);
	}
}

void CChain::Rotate(CDot dot, double angle)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
			pGroupObj->Rotate(dot,angle);
	}
}

void CChain::Drag(CDot dot, double ratioX, double ratioY, double moveX, double moveY)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
			pGroupObj->Drag(dot,ratioX,ratioY,moveX,moveY);
	}
}

void CChain::Move(double moveX, double moveY)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
			pGroupObj->Move(moveX,moveY);
	}
}

void CChain::Scale(CDot dot, double ratioX, double ratioY)
{	
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
			pGroupObj->Scale(dot,ratioX,ratioY);
	}
}

CQuad CChain::Include(CQuad quad,BOOL bMulSelect)
{
	//先设置所有的树都无效(单选)
	if(!bMulSelect)
		SetSelect(FALSE);

	//检查所有的树
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);
		if(pGroupObj->Include(quad))
			pGroupObj->InvertSelect();
	}

	//LZQ
	pos = m_listCopyObject.GetHeadPosition();
	if (!bMulSelect)
	{
		while(pos)
			m_listCopyObject.GetNext(pos).m_bSelect = FALSE;
	}
	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		if (quad.Include(obj.m_quadLimit))
			obj.m_bSelect = TRUE;
	}

	m_quadLimit=GetLimit();
	return m_quadLimit;
}

void CChain::SetSelect(BOOL bFlag)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);
		pGroupObj->SetSelect(bFlag);
	}
}

void CChain::Serialize(FILE* fp,BOOL bFlag)
{
	g_MgrText = &m_MgrText;
	CGroupObj *pGroupObj = NULL;

	CString str;
	CMySerialize sa(fp);

	if(bFlag)
	{
		sa<<"D_B";
		m_MgrText.SaveText(fp,bFlag);
		UINT nNextID = m_MgrText.GetLastID();
		m_MgrText.SetNextID(nNextID);

		POSITION pos=m_list.GetHeadPosition();
		while(pos!=NULL)
		{
			pGroupObj =dynamic_cast<CGroupObj*>(m_list.GetNext(pos));
			pGroupObj->Serialize(fp,bFlag);
		}
		sa<<"D_E";
	}
	else
	{
		while(sa>>str)
		{
			if(str == "TXT_B")
			{
				CText* pText = new CText(0);
				pText->Serialize(fp,FALSE);
				if(m_MgrText.GetCopySts())
				{
					//如果是COPY状态时则修改ID
					UINT nID = m_MgrText.GetBaseID();
					pText->m_nTextID = OFFSET_TEXTID(nID,pText->m_nTextID);
				}
				m_MgrText.AddText(pText);
			}
			else if(str == "GROUP_B")
			{
				//树开始
				pGroupObj = new CGroupObj();
				pGroupObj->Serialize(fp,bFlag);

				m_list.AddTail(pGroupObj);
			}
			//数据结束
			else if(str == "D_E")
				break;
		}
	}
}

void CChain::Delete()
{
	CGroupObj *pGroupObj = NULL;
	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		prev=pos;
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);

		if(pGroupObj->IsSelect())
		{
			m_list.RemoveAt(prev);
			delete pGroupObj;
		}
	}

	//LZQ
	pos=m_list.GetHeadPosition();
	if (pos==NULL)
	{
		m_listCopyObject.RemoveAll();
		return;
	}

	POSITION posPre;
	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		posPre = pos;
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		if (obj.m_bSelect)
			m_listCopyObject.RemoveAt(posPre);
	}
}

int CChain::GetStatus()
{
	int style=0;
	int count=0;

	CGroupObj* pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			if( pGroupObj->GetType() == NODE_COMBIN )
				style |= selectGroup;

			if( pGroupObj->GetType() == NODE_UNION )
				style |= selectUnion;

			count++;
			if(count>1)
				return selectMul;
		}
	}

	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		if (obj.m_bSelect)
			count++;
	}
	
	if(count)
		style|=selectSingle;
	else
		style|=selectNull;

	return style;
}

void CChain::Group()
{	
	UnionGroup();

	CGroupObj *pGroupObj = NULL;
	CGroupObj *pGroupObjNew = NULL;

	POSITION prev = NULL;
	POSITION pos  =m_list.GetHeadPosition();
	while( pos != NULL )
	{
		prev  = pos;
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);

		if( pGroupObj->IsSelect() && (pGroupObj->GetLockFlg() == FALSE) )
		{
			pGroupObjNew = new CGroupObj();
			m_list.InsertBefore(pos,pGroupObjNew);
			m_list.RemoveAt(prev);
			pGroupObjNew->Group(pGroupObj);

			if( pGroupObj )
			{
				delete pGroupObj;
				pGroupObj = NULL;
			}
			break;
		}
	}

	if( pGroupObjNew != NULL )
	{
		pGroupObjNew->SetSelect();
		pGroupObjNew->GetLimit();
	}
}

void CChain::UnGroup()
{
	CGroupObj *pGroupObj = NULL;

	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		prev=pos;
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);

		if(pGroupObj->IsSelect())
		{
			pGroupObj->UnGroup(m_list);
			m_list.RemoveAt(prev);
			if (pGroupObj!=NULL)
			{
				delete pGroupObj;
				pGroupObj = NULL;
			}
			break;
		}
	}
}

void CChain::UnionGroup()
{
	/////////
	CGroupObj *pGroupObj = NULL;
	CGroupObj *pGroupObjNew = NULL;

	POSITION prev = NULL;
	POSITION pos  =m_list.GetHeadPosition();
	while( pos != NULL )
	{
		prev  = pos;
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);

		if( pGroupObj->IsSelect() && ( pGroupObj->GetLockFlg() == FALSE ) )
		{
			if( pGroupObjNew == NULL )
			{
				pGroupObjNew = new CGroupObj();
				m_list.InsertBefore( pos, pGroupObjNew );
			}
			m_list.RemoveAt(prev);
			pGroupObjNew->UnionGroup(pGroupObj);

			if ( pGroupObj )
			{
				delete pGroupObj;
				pGroupObj = NULL;
			}
		}
	}

	if( pGroupObjNew != NULL )
	{
		pGroupObjNew->SetSelect();
		pGroupObjNew->GetLimit();
	}
}

//
void CChain::UnUnionGroup()
{
	/////////
	CGroupObj *pGroupObj = NULL;
	POSITION prev = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{
		prev = pos;
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);

		if ( pGroupObj->IsSelect() && (pGroupObj->GetLockFlg() == FALSE) && pGroupObj->m_list.GetCount() > 1 )
		{
			pGroupObj->UnUnionGroup(m_list);

			m_list.RemoveAt(prev);
			if ( pGroupObj != NULL )
			{
				delete pGroupObj;
				pGroupObj = NULL;
			}
		}
	}
}

CQuad CChain::GetAllLimit()
{
	CQuad quad;
	quad.Initial();

	POSITION pos=m_list.GetHeadPosition();
	if (m_listCopyObject.GetCount())	//LZQ
	{
		if (pos) m_list.GetNext(pos);
	}

	CGroupObj *pGroupObj = NULL;
	pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);
		quad.Compose(pGroupObj->GetLimit());
	}

	//LZQ
	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		quad.Compose(obj.m_quadLimit);
	}

	return quad;
}

CQuad CChain::GetMarkLimit()
{
	CQuad quad;
	quad.Initial();

	POSITION pos=m_list.GetHeadPosition();
	if (m_listCopyObject.GetCount())	//LZQ
	{
		if (pos) m_list.GetNext(pos);
	}

	CLay* play = g_DAT.GetCurLay();
	CGroupObj *pGroupObj = NULL;
	pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);

		CNode *pNode = pGroupObj->GetNode();
		int nRepeat =  play->GetRepeat(pNode->m_property.m_nLayer);
		if (nRepeat > 0)
			quad.Compose(pGroupObj->GetLimit());
	}

	//LZQ
	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		quad.Compose(obj.m_quadLimit);
	}

	return quad;
}

void CChain::Mark(HWND hWnd)
{
	DWORD nPos;
	int nCurPos = 0;
	CGroupObj *pGroupObj = NULL;

	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		//进度条显示处理
		nCurPos++;
		nPos = nCurPos*10/CMarkMgr::m_dwMarkCount;
		if(nPos != CMarkMgr::m_dwCurPos)
		{
			CMarkMgr::m_dwCurPos = nPos;
			::SendMessage(hWnd,WM_MARK_END,MARK_INFO,nPos);			
		}

		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(m_bSelMark)
		{
			if(pGroupObj->IsSelect())
				pGroupObj->Mark(hWnd); 
		}
		else
			pGroupObj->Mark(hWnd);
		
		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0){
			break;
		}
	}
}

void CChain::Copy()
{
	CGroupObj *pGroupObj = NULL;

	int count    = m_list.GetCount();
	POSITION pos = m_list.GetHeadPosition();
	for(int i = 0; i < count; i++)
	{
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);

		if(pGroupObj->IsSelect())
		{
			pGroupObj->SetSelect(FALSE);
			CGroupObj *pGroupObjNew = new CGroupObj(pGroupObj);
			if( pGroupObjNew == NULL )
			{
				break;
			}
			pGroupObjNew->SetSelect(TRUE);
			m_list.AddTail(pGroupObjNew);
		}
	}

	return;
}

void CChain::ArrayCopy(int iCnt)
{
	CGroupObj *pGroupObj = NULL;

	int count=m_list.GetCount();
	POSITION pos=m_list.GetHeadPosition();
	for(int i=0;i<count;i++)
	{
		pGroupObj = (CGroupObj*)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			CGroupObj *pGroupObjNew = new CGroupObj(pGroupObj);
			pGroupObjNew->SetSelect(FALSE);
			m_list.AddTail(pGroupObjNew);
		}
	}
}

void CChain::Clipboard(FILE* fp,BOOL bFlag)
{		
	POSITION pos,prev;
	CGroupObj* pGroupObj = NULL;
	CChain *p=new CChain;
	if (bFlag)
	{
		//如果是选中中有条码或是条码附属文本则选中它
		pos=m_list.GetHeadPosition();
		CUIntArray arrayTextID;
		//得到当前是否有选中的条码的TEXTID
		while(pos!=NULL)
		{
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			if(pGroupObj->IsSelect())
			{
				pGroupObj->CheckBarCode(arrayTextID);
			}
		}
		pos=m_list.GetHeadPosition();
		while(pos!=NULL)
		{
			//如果树有则加入选择项
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			if(!pGroupObj->IsSelect())
				pGroupObj->SelectBarCode(arrayTextID);
		}
		
		pos=m_list.GetHeadPosition();
		p->m_MgrText = m_MgrText;
		while(pos!=NULL)
		{	
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			if(pGroupObj->IsSelect())
				p->Add(pGroupObj);
		}
 		p->Serialize(fp,bFlag);
	}
	else
	{
		m_MgrText.SetCopySts(TRUE);
		UINT nBaseID = m_MgrText.GetBaseID();

		//将得到当前文档的基本ID传到复制对象中的ID中去
		p->m_MgrText.SetCopySts(TRUE);
		p->m_MgrText.SetBaseID(nBaseID);

 		p->Serialize(fp,bFlag);
		
		g_MgrText = &m_MgrText;

		g_MgrText->Add(&(p->m_MgrText));

		SetSelect(FALSE);
		p->SetSelect();

		pos=p->m_list.GetHeadPosition();
		while(pos!=NULL)
		{	
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			Add(pGroupObj);
		}
		p->m_MgrText.SetCopySts(FALSE);
		m_MgrText.SetCopySts(FALSE);
	}

	//删除临时对象
	/////////////////
	pos=p->m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		prev=pos;
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		p->m_list.RemoveAt(prev);
	}
	delete p;
}

void CChain::ExtendObj(double dbSpace,BOOL bIsKillOldObj, XTracker *pTrack, BOOL bOpt)
{
	POSITION pos = m_list.GetHeadPosition();
	POSITION pre = NULL;
	while(pos!=NULL)
	{	
		pre = pos;
		CGroupObj *pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if (NULL == pGroupObj)
			continue;

		if(pGroupObj->IsSelect())
		{
			CGroupObj *pRetGroupObj = pGroupObj->ExtendObj(dbSpace,bOpt);
			if (pRetGroupObj)
			{
				m_list.AddTail(pRetGroupObj);
				pRetGroupObj->SetSelect(FALSE);
				pRetGroupObj->SetStrokeList(&pTrack->m_StrokeList);

				if (bIsKillOldObj) 
				{
					m_list.RemoveAt(pre);
					delete pGroupObj;
					pGroupObj = NULL;
				}

				if (pGroupObj)
				{
					pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
				}
			}// if (pRetTree)
		}// if(pTree->IsSelect())

	}// end while

	return;
}

void CChain::SetFillStyle(FILLDATA& fill,XTracker *pTrack)
{
	CGroupObj *pGroupObj = NULL;

	POSITION pos=m_list.GetHeadPosition();
	while( pos!=NULL )
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if( pGroupObj->IsSelect() && (pGroupObj->GetLockFlg() == FALSE))
		{
			pGroupObj->SetFillStyle(fill);
			pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
		}
	}
}

void CChain::GetFillStyle(FILLDATA& fill)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while( pos != NULL )
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if( pGroupObj->IsSelect() == TRUE )
		{
			pGroupObj->GetFillStyle(fill);
			return;
		}
	}
}

void CChain::SetProperty(int mode,CProperty pty)
{	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos!=NULL )
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);

		if ( pGroupObj!=NULL )
			pGroupObj->SetProperty(mode,pty);
	}
}

BOOL CChain::PreprocessMark(DWORD nStyle)
{	
	BOOL bFlash = FALSE;

	g_MgrText = &m_MgrText;
	
	
	//预处理下一批数据文本信息
	if(m_MgrText.FlashNextText(nStyle))
		bFlash = TRUE;
	else{
		//如果文件中没有跳号内容，且处于要更新的情况下不必进行下一步的操作
	
		//	return TRUE; 因为图片跳号注释掉
	}
	
	m_nMarkUnit=0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		pGroupObj->PreprocessMark(nStyle);
	}

	return TRUE;
}

CNode * CChain::GetNode()
{
	if(GetStatus()&selectSingle)
	{
		CGroupObj *pGroupObj = NULL;
		POSITION pos=m_list.GetHeadPosition();
		while(pos!=NULL)
		{
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			if(pGroupObj->IsSelect())
			{
				CNode *pNode = pNode=pGroupObj->GetNode();
				return pNode;
			}
		}
		return NULL;
	}
	else
	{
		return NULL;
	}

	return NULL;
}

int CChain::GetMenuIndex()
{
	//根据选择状态确定相应菜单
	int type;
	int status;
	status=GetStatus();
	if(status&selectNull)
	{
		type=-1;
	}
	else
	{
		type=NODE_BASE;

		if(status&selectSingle)
		{
			CGroupObj *pGroupObj = NULL;
			POSITION pos=m_list.GetHeadPosition();
			while(pos!=NULL)
			{
				pGroupObj = (CGroupObj *)m_list.GetNext(pos);
				if(pGroupObj->IsSelect())
				{
					type=pGroupObj->GetType();
					break;
				}
			}
		}

		switch(type)
		{
		case NODE_SERIAL:
			type=1;
			break;
		case NODE_CODEBAR:
			type=2;
			break;
		case NODE_BMP:
			type=3;
			break;
		case NODE_TEXT_DSF:
			type=4;
			break;
		default:
			type=0;
			break;
		}
	}
	
	return type;
}

void CChain::GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray)
{
	dtArray.RemoveAll();

	CQuad  quad;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj   = (CGroupObj *)m_list.GetNext(pos);
		pGroupObj->GetArrayDot(dtArray);
	}
}

int CChain::GetCount()
{
	return m_list.GetCount();
}

void CChain::GetSelectFlag(BOOL *pFlag)
{
	int i=0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		pFlag[i]=pGroupObj->IsSelect();
		i++;
	}		
}

CQuad CChain::SetSelectFlag(BOOL *pFlag)
{
	SetSelect(FALSE);

	int i=0;
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		pGroupObj->SetSelect(pFlag[i]);
		i++;
	}
	
	m_quadLimit=GetLimit();
	return m_quadLimit;
}

void CChain::CalLimit(CDot dot,BOOL bAll,double dbXScale,double dbYScale )
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		pGroupObj->CalLimit(dot, bAll, dbXScale, dbYScale);
	}
}

CQuad CChain::GetLimit()
{
	CQuad quad;
	quad.SetEmpty();

	POSITION pos=m_list.GetHeadPosition();
	if (m_listCopyObject.GetCount())	//LZQ
	{
		if (pos) m_list.GetNext(pos);
	}

	CGroupObj *pGroupObj = NULL;
	pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
			quad.Compose(pGroupObj->GetLimit());
	}

	//LZQ
	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		if (obj.m_bSelect)
			quad.Union(obj.m_quadLimit);
	}
	return quad;
}

void CChain::Array(CArray<OBJPOS, OBJPOS> &dtArray)
{
	AyCopy(dtArray);

	return;
/*
	//求出阵列后选择集中被选对象的个数
 	DOT6 dotOld,dotNew;
	double moveX,moveY;
	int count=dtArray.GetSize();

	int nStep = 0;
	SetProStep(nStep);

	CList<CTree*, CTree*> Sellist;
	POSITION posF=m_list.GetHeadPosition();
	while (posF) 
	{
		CTree *pTree=m_list.GetNext(posF);
		if(pTree->IsSelect())
			Sellist.AddTail(pTree);
	}

	m_quadLimit=GetLimit();
	CDot oldDt;
	oldDt  = m_quadLimit.GetBaseDot();
	dotOld.x = oldDt.x;
	dotOld.y = oldDt.y;

	//////////////////////////////////////////////////////////////////////////
	CList<CTree*, CTree*> nList;
	//////////////////////////////////////////////////////////////////////////
	
	for(int i = 0; i < count; i++)
	{
		dotNew=dtArray[i];
		moveX=dotNew.x-dotOld.x;
		moveY=dotNew.y-dotOld.y;

		if(i)
		{
			Copy();
		}
		Move(moveX,moveY);

		CDot centDt(dotNew.x, dotNew.y);
		Rotate(centDt, dotNew.z);

		dotOld=dotNew;

		nStep = (int)((double)i / (double)count * 100);
		SetProStep(nStep);
	}

	//求出选择集中被选对象的个数
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	int j=0;
	while(pos!=NULL)
	{
		pTree=m_list.GetNext(pos);
		if(pTree->IsSelect())
			j++;
	}
	count--;
	count*=j;

	//设置选择标志
	pos=m_list.GetTailPosition();
	for(i=0; i < count; i++)
	{
		pTree=m_list.GetPrev(pos);
		pTree->SetSelect();
		pTree->SetMainSelect(FALSE);
	}

	pos=Sellist.GetTailPosition();
	while(pos)
	{
		pTree=Sellist.GetNext(pos);
		pTree->SetSelect();
		pTree->SetMainSelect(TRUE);
	}

	SetProStep(0);

	return;
	*/
}

void CChain::SetLayColor(int nLayer,int nFillLayer)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
			pGroupObj->SetLayColor(nLayer,nFillLayer);
	}
}

void CChain::SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
			pGroupObj->SetObjColor(nObjIndex, nObjColor, nFillIndex, nFillColor);
	}

	return;
}


void CChain::GetColorByNum(int nNum,COLORREF &nColor)
{
	switch (nNum)
	{
	case 0:
		nColor = BLACK;
		break;
	case 1:
		nColor = GRAY;
		break;
	case 2:
		nColor = GREEN;
		break;
	case 3:
		nColor = YANGREEN;
		break;
	case 4:
		nColor = YELLOW;
		break;
	case 5:
		nColor = BLUE;
		break;
	case 6:
		nColor = MAGENTA;
		break;
	case 7:
		nColor = RED;
		break;
	case 8:
		nColor = BROWN;
		break;
	case 9:
		nColor = PINK;
		break;

	default:
		nColor = BLACK;
		break;
	}
}

void CChain::GetLayColor(int &nLayer,int &nFillLayer)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);

		if(pGroupObj->IsSelect()){
			pGroupObj->GetLayColor(nLayer,nFillLayer);
			return;
		}
	}
}

CQuad CChain::GetReFurbishQuad()
{
	CQuad quad;
	quad.SetEmpty();

	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);

		if(pGroupObj->IsSelect())
			quad.Union(pGroupObj->GetReFurbishQuad());
	}
	return quad;	
}

void CChain::GetSelectedStrokeList(STROKELIST *pStrokeList)
{	
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);

		if( pGroupObj->IsSelect() )
			pGroupObj->GetStrokeList(pStrokeList);
	}
}

CQuad CChain::GetMarkRange()
{
    CQuad quad;
	quad.SetEmpty();

	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	if(m_bSelMark)
	{
		while(pos!=NULL)
		{
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			if(pGroupObj->IsSelect())
				quad.Union(pGroupObj->GetMarkRange());
		}
	}
	else
	{
		while(pos!=NULL)
		{	
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			quad.Union(pGroupObj->GetMarkRange());
		}
	}
	return quad;
}

void CChain::MarkPreview(CDC *pDC, const int &nPreviewSpeed)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
//		pGroupObj->MarkPreview(pDC, nPreviewSpeed);
	}
}

void CChain::PreTrack()
{
	CQuad quad= GetLimit();

	//生成四个点
	CStroke Stroke;
	
	CDot dot0,dot1,dot2,dot3;
	dot0.x=quad.left;
	dot0.y=quad.bottom;
	dot1.x=quad.right;
	dot1.y=quad.bottom;
	dot2.x=quad.right;
	dot2.y=quad.top;
	dot3.x=quad.left;
	dot3.y=quad.top;
	
	Stroke.Add(dot0);
	Stroke.Add(dot1);
	Stroke.Add(dot2);
	Stroke.Add(dot3);
	Stroke.Add(dot0);

	while(1)
	{
		g_DAT.MarkStroke(Stroke);
		if(WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
			break;
	}
}

BOOL CChain::GetFirstMarkCurrent(double & dbCurrent)
{
	if (GetCount()!=0)
	{
		CGroupObj *pGroupObj = NULL;
		POSITION pos=m_list.GetHeadPosition();
		while(pos!=NULL)
		{
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			if(m_bSelMark)
			{
				if(pGroupObj->IsSelect())
				{
					pGroupObj->GetFirstMarkCurrent(dbCurrent);
					return TRUE;
				}
			}
			else
			{
				pGroupObj->GetFirstMarkCurrent(dbCurrent);		
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

BOOL CChain::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	BOOL brtn=TRUE;
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		brtn=brtn&&pGroupObj->ExportPlt(file,bUnExportFill,bScall100);
	}
	return brtn;
}

void CChain::AddRect(CQuad &quad,CWnd *pWnd)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->Include(quad))
		{
			pGroupObj->AddRect(pWnd);
		}
	}
}	

void CChain::SelectDraw(CTree *pTree,XTracker *pTrack)
{
	pTree->SetStrokeList(&pTrack->m_StrokeList);
}

BOOL CChain::MouseOnObject(CPoint &point,CWnd *pWnd)
{
	POSITION pos;
	CGroupObj* pGroupObj = NULL;
	pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj)
		{
			return pGroupObj->Click(point,pWnd);
		}
	}
	return FALSE;
}

//DEL BOOL CChain::HitOnObject(CPoint &point,CWnd *pWnd)
//DEL {
//DEL 	POSITION pos=m_list.GetHeadPosition();
//DEL 	while(pos)
//DEL 	{
//DEL 		CTree *pTree;
//DEL 		pTree=m_list.GetNext(pos);
//DEL 		if(pTree->PtInTree(dot) &&pTree->MouseOnObject(dot))
//DEL 		{
//DEL 			DeleteList(pListHead);
//DEL 			SetSelect(FALSE);
//DEL 			AddList(pTree,pListHead);
//DEL 			pTree->SetSelect(TRUE);
//DEL 			return TRUE;
//DEL 		}
//DEL 	}
//DEL 	return FALSE;
//DEL }


void CChain::DelSelectTree(CTree *pTestTree)
{/*
	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{	
		prev=pos;
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);

		if(pGroupObj==pTestTree)
		{
			m_list.RemoveAt(prev);
			delete pGroupObj;
			break;
		}
	}*/
}


BOOL CChain::SetStrokeList(StrokeList *pStrokeList, CPoint &point, CWnd *pWnd)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->Click(point,pWnd))
		{
			if(!pGroupObj->IsSelect())
			{		
				//取消其它树的选中状态
				SetSelect(FALSE);
				pGroupObj->SetSelect(TRUE);
				pGroupObj->SetStrokeList(pStrokeList);
				return FALSE;
			}
			else
			{
				pGroupObj->SetStrokeList(pStrokeList);
					return TRUE;
			}
		}
	}
	return FALSE;
}

void CChain::CaptureObject(CQuad &quad,XTracker *pTrack)
{
	SetSelect(FALSE);
	pTrack->ClearStrokeList();

	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		pGroupObj->CaptureObject(quad,pTrack);
	}

	//LZQ
	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		if (quad.Include(obj.m_quadLimit))
			obj.m_bSelect = TRUE;
	}


	return;
}

BOOL CChain::DragSelectTree(CDot &dot, double ratioX, double ratioY, double moveX, double moveY)
{
	BOOL bOK = FALSE;
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			bOK = TRUE;
			pGroupObj->Drag(dot,ratioX,ratioY,moveX,moveY);
		}
	}

	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		if (obj.m_bSelect)
			obj.m_quadLimit.SetOffset(moveX, moveY);
	}

	return bOK;
}


BOOL CChain::HaveSelectObject()
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);

		if(pGroupObj->IsSelect())
			return TRUE;
	}
	return FALSE;
}

BOOL CChain::HitOtherObject(CPoint &point,CWnd *pWnd)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(!pGroupObj->IsSelect() && pGroupObj->Click(point,pWnd))
		{
			return TRUE;
		}
	}

	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		if (!obj.m_bSelect &&  obj.m_quadLimit.Include(point,pWnd))
			return TRUE;
	}
	return FALSE;
}


//bFlag为FALSE时不装载文本与条码数据
//为TRUE时装载文本与条码数据
void CChain::ReLoadStrokeList(StrokeList *pStrokeList,BOOL bFlag)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if( pGroupObj->IsSelect() )
		{
			pGroupObj->SetStrokeList(pStrokeList);
		}
	}
}

void CChain::ChangeQuad(CPoint &point, XTracker *pTrack,CWnd *pWnd, CArray<SELQUAD ,SELQUAD> &QuadList)
{
	ClearALLMainSelect();
	
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if( pGroupObj == NULL )
			continue;
		
		if(!pGroupObj->IsSelect() && pGroupObj->Click(point,pWnd))
		{
			/////////////////////
			short  nVKey=GetKeyState(VK_SHIFT);
			if(nVKey & SHIFTED)
			{
				pGroupObj->SetSelect(TRUE);
				pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
				SELQUAD Quad;
				Quad.m_QD   = pGroupObj->GetLimit();
				if (Quad.m_QD.IsEmpty()) 
				{
					continue;
				}
				Quad.m_bSel = TRUE;
				Quad.m_pGroupObj = (DWORD)pGroupObj;
				QuadList.Add(Quad);
				SetDrawFill(QuadList, -1);
			}
			else
			{
				SetSelect(FALSE);
				POSITION pos = m_listCopyObject.GetHeadPosition();
				while(pos)
					m_listCopyObject.GetNext(pos).m_bSelect = FALSE;

				pGroupObj->SetSelect(TRUE);
				pTrack->ClearStrokeList();
				QuadList.RemoveAll();
				pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
				SELQUAD Quad;
				Quad.m_QD   = pGroupObj->GetLimit();
				if (Quad.m_QD.IsEmpty()) 
				{
					continue;
				}
				Quad.m_bSel = TRUE;
				Quad.m_pGroupObj = (DWORD)pGroupObj;
				QuadList.Add(Quad);
				SetDrawFill(QuadList, -1);

				CMainFrame *pMainFram = (CMainFrame*)AfxGetMainWnd();
				if( pMainFram )
				{
					pMainFram->SetAllObjecUnSel();
					pMainFram->SetObjecSel(pGroupObj);
				}
			}			
			return;
		}
	}

	//LZQ
	short  nVKey=GetKeyState(VK_SHIFT);
	BOOL bMulSelect = nVKey & SHIFTED;
	if (!bMulSelect)
	{
		SetSelect(FALSE);

		pos = m_listCopyObject.GetHeadPosition();
		while(pos)
			m_listCopyObject.GetNext(pos).m_bSelect = FALSE;	
	}

	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		CCopyObject &obj = m_listCopyObject.GetNext(pos);
		if (obj.m_quadLimit.Include(point, pWnd))
		{
			obj.m_bSelect = TRUE;
			pTrack->ClearStrokeList();
			QuadList.RemoveAll();
			break;
		}
	}

	return;
}


/*
void CChain::SetDrawFill(CArray<SELQUAD ,SELQUAD> &QuadList, int nIndex)
{
	int nCount = QuadList.GetSize();
	if (nIndex > (nCount - 1)) 
	{
		return;
	}

	for (int i = 0; i < nCount; i++)
	{
		QuadList[i].m_bFill = FALSE;
	}

	if (-1 == nIndex) 
	{
		QuadList[nCount - 1].m_bFill = TRUE;
	}
	else
	{
		QuadList[nIndex].m_bFill = TRUE;
		QuadList[nIndex].m_bSel = TRUE;
	}

	return;
}
*/


void CChain::SetDrawFill(CArray<SELQUAD ,SELQUAD> &QuadList, int nIndex)
{
	int nCount = QuadList.GetSize();
	if (0 >= nCount)
	{
		return;
	}

	if ((nIndex > (nCount - 1)) && (nCount > 0))
	{
		return;
	}

	for (int i = 0; i < nCount; i++)
	{
		QuadList[i].m_bFill = FALSE;
		((CGroupObj*)QuadList[i].m_pGroupObj)->SetMainSelect(FALSE);
	}

	if (-1 == nIndex) 
	{
		QuadList[nCount - 1].m_bFill = TRUE;
		((CGroupObj*)QuadList[nCount - 1].m_pGroupObj)->SetMainSelect(TRUE);
	}
	else
	{
		QuadList[nIndex].m_bFill = TRUE;
		QuadList[nIndex].m_bSel = TRUE;
		((CGroupObj*)QuadList[nIndex].m_pGroupObj)->SetMainSelect(TRUE);
	}

	return;
}


void CChain::SetSelInQuadListByDot(CArray<SELQUAD ,SELQUAD> &QuadList, CPoint &point, CWnd* pWnd )
{
	int nCount = QuadList.GetSize();
	if (0 >= nCount) 
		return;

	for (int i = 0; i < nCount; i++)
	{
		CGroupObj *pGroupObj = (CGroupObj *)(QuadList[i].m_pGroupObj);
		if( pGroupObj->Click(point,pWnd) )
		{
			SetDrawFill(QuadList, i);
			break;
		}
	}

	return;
}

void CChain::SetUnSelInQuadListByDot(CArray<SELQUAD ,SELQUAD> &QuadList, CPoint &point, CWnd* pWnd )
{
	int nCount = QuadList.GetSize();
	if (0 >= nCount) 
		return;

	
	for (int i = 0; i < nCount; i++)
	{
		CGroupObj *pGroupObj = (CGroupObj *)(QuadList[i].m_pGroupObj);
		if( pGroupObj->Click(point,pWnd) ) 
		{
			pGroupObj->SetSelect(FALSE);
			
			break;
		}
	}
	
	return;
}


void CChain::GetQuadListInQuad(CArray<SELQUAD ,SELQUAD> &QuadList, CQuad &TrakerQD)
{
	CQuad quad;
	quad.SetEmpty();
	
	CMainFrame *pMainFram = (CMainFrame*)AfxGetMainWnd();
	if( pMainFram )
		pMainFram->SetAllObjecUnSel();

	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		quad  = pGroupObj->GetLimit();
		if (quad.IsEmpty()) 
			continue;

		if (TrakerQD.Include(quad)) 
		{
			if (!IsInSelQuadList(QuadList, quad)) 
			{
				SELQUAD QD;
				QD.m_bFill = FALSE;
				QD.m_bSel  = TRUE;
				QD.m_QD    = quad;
				QD.m_pGroupObj = (DWORD)pGroupObj;
				QuadList.Add(QD);

				if( pMainFram )
					pMainFram->SetObjecSel(pGroupObj);
			}
		}
	}

	return;
}

BOOL CChain::IsInSelQuadList(CArray<SELQUAD ,SELQUAD> &QuadList, CQuad &QD)
{
	int nCount = QuadList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (QuadList[i].m_QD.Equal(QD)) 
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CChain::Convert(BOOL bPopMsg)
{
	BOOL bConvertFill = FALSE;
	if (bPopMsg) 
	{
		if (IDOK == AfxMessageBox(IDS_CONVERTTOLINE, MB_OKCANCEL))
		{
			bConvertFill = TRUE;
		}
		else
		{
			return;
		}
	}
	else
	{
		bConvertFill = TRUE;
	}

	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	POSITION pre = NULL;
	while(pos)
	{
		pre=pos;
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if( NULL == pGroupObj ) 
			continue;

		if(pGroupObj->IsSelect())
		{
			UINT nResult=pGroupObj->CanConvert();
			if (nResult)
			{
				UINT nResult = pGroupObj->CanConvert();
				if( nResult != 0 )
				{
					CGroupObj *pGroupObjNew = pGroupObj->ConvertPLT();				

					m_list.RemoveAt(pre);
					if(pGroupObj != NULL)
					{
						delete pGroupObj;
						pGroupObj = NULL;
					}

					if(pGroupObjNew != NULL)
					{
						m_list.AddTail(pGroupObjNew);
						pGroupObjNew->SetSelect(TRUE);
					}
				}
			}// if (nResult)
		} // if(pTree->IsSelect())
	}// while(pos)

	return;
}

void CChain::FilterPoint(double dbPrecis, double dbAngle)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	POSITION pre;
	while(pos)
	{
		pre=pos;
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(NULL == pGroupObj) 
			continue;

		pGroupObj->FilterPoint(dbPrecis, dbAngle);
	}
}

BOOL CChain::UnLockLayer(int nLayer)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
//		if (pGroupObj) 
//			pGroupObj->UnLockLayer(nLayer);
	}

	return TRUE;
}

BOOL CChain::LockLayer(int nLayer)
{
	CGroupObj *pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
//		if (pGroupObj) 
//			pGroupObj->LockLayer(nLayer);

	}

	return TRUE;
}

BOOL CChain::InitSort(CArray<SORTNODE,SORTNODE> &Sort, CWnd *pWnd)
{
	if ( NULL == pWnd )
		return FALSE;
	
	Sort.RemoveAll();
	
	int    nIndex = 0;
	CGroupObj *pGroupObj  = NULL;
	CHLView *pHLView=(CHLView*)pWnd;
	POSITION pos  = m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if (NULL == pGroupObj)
			continue;
		
		CDot dt;
		if (pGroupObj->GetFirstDot(dt))
		{
			SORTNODE SortNode;
			SortNode.m_SPt   = dt;
			SortNode.m_nSort  = nIndex;
            SortNode.m_Color = RGB(128, 128, 128);
			SortNode.m_pTree = (DWORD)pGroupObj;
			Sort.Add(SortNode);
			nIndex++;
		}
	}// end while

	CalcMarkDot(Sort);

	return TRUE;
}


void CChain::SortAllObj(CArray<SORTNODE,SORTNODE> &Sort)
{
	int nCount = Sort.GetSize();
	if (nCount < 0)
	{
		return;
	}
	DWORD *pTreeLis = new DWORD[nCount];
	if (NULL == pTreeLis)
	{
		return;
	}
	memset(pTreeLis, 0, sizeof(DWORD) * nCount);
	//m_list.RemoveAll();
 
	for (int i = 0; i < nCount; i++)
	{
		int nIndex = Sort[i].m_nSort;
		DWORD dwTree = Sort[i].m_pTree;
		pTreeLis[nIndex] = dwTree;
	}
    /*for (int j = 0; j < nCount; j++)
	{
		DWORD dwT = (pTreeLis[j]);
		CTree*pTree = (CTree*)dwT;
		m_list.AddTail(pTree);
	}*/

	int j = 0;
	CGroupObj *pGroupObj  = NULL;
	POSITION posPre = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos)
	{
		posPre = pos;
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		DWORD dwT = (pTreeLis[j]);
		m_list.SetAt(posPre, (CGroupObj*)dwT);
		j++;
	}

	if (pTreeLis)
	{
		delete []pTreeLis;
		pTreeLis = NULL;
	}

	return;
}

void CChain::CalcMarkDot(CArray<SORTNODE,SORTNODE> &Sort)
{
	int nCount = Sort.GetSize();
	for (int i = 0; i < nCount; i++) 
	{
		if (Sort[i].m_MNum > 0)
		{
			continue;
		}

		int k = 0;
		CDot dti = Sort[i].m_SPt;
		for (int j =0; j < nCount; j++) 
		{
			if (Sort[j].m_MNum > 0)
			{
				continue;
			}
			CDot dtj = Sort[j].m_SPt;
			double dbD = Distance(dti, dtj);
			if (dbD < 0.5) 
			{
				Sort[j].m_MNum = k;
				k++;
			}
		}
	}
	
	return;
}


// 求两点的距离
double CChain::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return dbD;
}

BOOL CChain::InitASort(CArray<ASNODE,ASNODE> &Sort)
{
	Sort.RemoveAll();

	int nIndex = 0;
	CGroupObj *pGroupObj  = NULL;
	POSITION pos  = m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if (NULL == pGroupObj) 
			continue;
		
		CDot sDt;
		if (pGroupObj->GetFirstDot(sDt))
		{
			CDot eDt;
			pGroupObj->GetLastDot(eDt);
			ASNODE sNode;
			sNode.m_SDt    = sDt;
			sNode.m_EDt    = eDt;
			sNode.m_nSort  = nIndex;
			sNode.m_pTree  = (DWORD)pGroupObj;
			Sort.Add(sNode);
			nIndex++;
		}
	}// end while

	return TRUE;
}


void CChain::AutoSortAllObj(CArray<ASNODE,ASNODE> &Sort, DWORD dwStartTree)
{
	int nCount = Sort.GetSize();
	if (nCount < 0)
	{
		return;
	}

	DWORD *pTreeLis = new DWORD[nCount];
	if (NULL == pTreeLis)
	{
		return;
	}

	memset(pTreeLis, 0, sizeof(DWORD) * nCount);
	//m_list.RemoveAll();
 
	for (int i = 0; i < nCount; i++)
	{
		int nIndex   = Sort[i].m_nSort;
		DWORD dwTree = Sort[nIndex].m_pTree;
		pTreeLis[i] = dwTree;
	}

	// 根据首点，再次排序
	DWORD *pTreeLisTemp = new DWORD[nCount];
	if (NULL == pTreeLis)
	{
		return;
	}
	
	if (dwStartTree != NULL) 
	{
		int nPost = -1;
		for (int k = 0; k < nCount; k++)
		{
			DWORD dwT   = (pTreeLis[k]);
			if (dwStartTree == dwT) 
			{
				nPost = k;
				break;
			}
		}

		int j = 0;
		POSITION posPre = NULL;
		CGroupObj *pGroupObj  = NULL;
		POSITION pos = m_list.GetHeadPosition();
		while(pos)
		{
			posPre = pos;
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			/*for (int m = nPost; m < nCount; m++) 
			{
			DWORD dwT   = (pTreeLis[m]);
			CTree*pTree = (CTree*)dwT;
			m_list.AddTail(pTree);
			}*/
			if (j+nPost<nCount)
			{
				DWORD dwT = pTreeLis[j+nPost];
				m_list.SetAt(posPre, (CGroupObj*)dwT);
			}
			else
			{
				DWORD dwT = pTreeLis[j+nPost-nCount];
				m_list.SetAt(posPre, (CGroupObj*)dwT);
			}

			/*for (int n = 0; n < nPost; n++) 
			{
				DWORD dwT   = (pTreeLis[n]);
				CTree*pTree = (CTree*)dwT;
				m_list.AddTail(pTree);
			}*/
			j++;
		}
	}
	else
	{
		// 更新到m_list中
		/*for (int j = 0; j < nCount; j++)
		{
		DWORD dwT   = (pTreeLis[j]);
		CTree*pTree = (CTree*)dwT;
		m_list.AddTail(pTree);
		}*/

		int j = 0;
		CGroupObj *pGroupObj  = NULL;
		POSITION posPre = NULL;
		POSITION pos = m_list.GetHeadPosition();
		while(pos)
		{
			posPre = pos;
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			DWORD dwT = (pTreeLis[j]);
			m_list.SetAt(posPre, (CGroupObj*)dwT);
			j++;
		}
	}

	if (pTreeLis)
	{
		delete []pTreeLis;
		pTreeLis = NULL;
	}

	if (pTreeLisTemp)
	{
		delete []pTreeLisTemp;
		pTreeLisTemp = NULL;
	}

	return;
}

void CChain::SetGroupNum(int nNum)
{	
	CGroupObj *pGroupObj  = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			pGroupObj->SetGroupNum(nNum);
		}
	}// end while

	return;
}

void CChain::GetLayNoByType(int Type ,int &LayNo)
{	
	CGroupObj *pGroupObj  = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->GetType() == Type)
		{
			pGroupObj->GetLayColor(LayNo,LayNo);
			break;
		}
	}
	return;
}


BOOL CChain::InitSelSort(CArray<GNODE,GNODE> &Sort)
{
	Sort.RemoveAll();

	int nIndex = 0;
	CGroupObj *pGroupObj  = NULL;
	POSITION pre;
	POSITION pos  = m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pre = pos;
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if (NULL == pGroupObj) 
			continue;

		BOOL bIsSel = pGroupObj->IsSelect();
		if (!bIsSel)
			continue;
		
		CDot dt;
		if (pGroupObj->GetFirstDot(dt))
		{
			GNODE gNode;
			gNode.m_nNum  = nIndex;
			gNode.m_pGroupObj = (DWORD)pGroupObj;
			CQuad qd = pGroupObj->GetLimit();
			gNode.m_rt.left   = qd.left;
			gNode.m_rt.top    = qd.top;
			gNode.m_rt.right  = qd.right;
			gNode.m_rt.bottom = qd.bottom;
			Sort.Add(gNode);
			nIndex++;
		}
	}// end while

	return TRUE;
}

BOOL CChain::InitGSort(CArray<GNODE,GNODE> &Sort, DWORD &pStroke)
{/*
	Sort.RemoveAll();

	int nIndex = 0;
	CGroupObj* pGroupObj = NULL;
	POSITION pre  = NULL;
	POSITION pos  = m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pre = pos;
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if (NULL == pGroupObj) 
			continue;

		BOOL bIsSel = pGroupObj->IsSelect();
		if (bIsSel)
		{
			CStroke *pStk = pGroupObj->GetGoLineStroke();
			if (pStk) 
			{
				pStroke = (DWORD)pStk;
				delete pGroupObj;
				pGroupObj = NULL;
				m_list.RemoveAt(pre);
				continue;
			}
		}
		
		CDot dt;
		if (pGroupObj->GetFirstDot(dt))
		{
			GNODE gNode;
			gNode.m_nNum  = nIndex;
			gNode.m_pGroupObj = (DWORD)pGroupObj;
			CQuad qd = pGroupObj->GetLimit();
			gNode.m_rt.left   = qd.left;
			gNode.m_rt.top    = qd.top;
			gNode.m_rt.right  = qd.right;
			gNode.m_rt.bottom = qd.bottom;
			Sort.Add(gNode);
			nIndex++;
		}
	}// end while
*/
	return TRUE;
}

void CChain::GSortAllObj(CArray<GNODE,GNODE> &Sort)
{/*
	int nCount = Sort.GetSize();
	if (nCount < 0)
	{
		return;
	}
	//
	DWORD *pTreeLis = new DWORD[nCount];
	if (NULL == pTreeLis)
	{
		return;
	}

	memset(pTreeLis, 0, sizeof(DWORD) * nCount);
 
	for (int i = 0; i < nCount; i++)
	{
		if (Sort[i].m_bIsSort)
		{
			int nIndex   = Sort[i].m_nNum;
			DWORD dwTree = Sort[i].m_pGroupObj;
			pTreeLis[nIndex] = dwTree;
		}
	}// end for
	
    for (int j = 0; j < nCount; j++)
	{
		DWORD dwT   = (pTreeLis[j]);
		CGroupObj* pGroupObj = (CGroupObj*)dwT;
		if (pGroupObj) 
		{
			DelTree(pGroupObj);
		    m_list.AddTail(pGroupObj);
		}
	}

	if (pTreeLis)
	{
		delete []pTreeLis;
		pTreeLis = NULL;
	}
    
	return;*/
}

void CChain::DelTree(CTree *pTree)
{/*
	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{	
		prev=pos;
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);

		if(pTree == pCTree)
		{
			m_list.RemoveAt(prev);
		}
	}*/
}


void CChain::ComToOneTreeCmd()
{
	CNodePlt *pNodePlt = new CNodePlt;
	if (NULL == pNodePlt)
		return;

	int nLayer = 0;
	CGroupObj* pGroupObj = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);

		if (NULL == pGroupObj) 
			continue;

		if(pGroupObj->IsSelect())
		{
			//if(TRUE == pGroupObj->m_bMainSelect)
				nLayer = pGroupObj->GetSelObjLayerNo();
			pGroupObj->ComToOneGroupObj(pNodePlt);
		} // if(pTree->IsSelect())
	}// while(pos)

	if (pNodePlt->CalLimit().IsEmpty())
	{
		delete pNodePlt;
		pNodePlt = NULL;
	}
	else
	{
		ConvertToOneNodeDelete();
		CTree *pTree = new CTree(pNodePlt);
		if (NULL == pTree)
		{
			delete pNodePlt;
			return;
		}
		CGroupObj *pGroupObjNew = new CGroupObj();
		pGroupObjNew->Add(pTree);

		m_list.AddTail(pGroupObjNew);
		pGroupObjNew->SetSelect(TRUE);
		pGroupObjNew->SetLayColor(nLayer, nLayer);	
	}
}


void CChain::SelGroupByNum(int nNum, XTracker *pTrack,CWnd *pWnd)
{
	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;

	short  nVKey=GetKeyState(VK_SHIFT);
	if(nVKey & SHIFTED)
	{
		while(pos)
		{
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			if(nNum == pGroupObj->GetGroupNum())
			{
				pGroupObj->SetSelect(TRUE);
				pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
			}// end if(pTree->IsSelect())
		}// end while
	}
	else
	{
		SetSelect(FALSE);
		pTrack->ClearStrokeList();
		while(pos)
		{
			pGroupObj = (CGroupObj *)m_list.GetNext(pos);
			if(nNum == pGroupObj->GetGroupNum())
			{
				pGroupObj->SetSelect(TRUE);
				pGroupObj->SetStrokeList(&pTrack->m_StrokeList);
			}// end if(pTree->IsSelect())
		}// end while
	}
	

	return;
}

void CChain::SetAllSelTreeDoubleOutParaAdd(double dbLeftXAdd, double dbLeftYAdd, double dbRightXAdd, double dbRightYAdd)
{
#ifdef _DOUBLE_HEAD_MARK
	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			pTree->m_DoubleOutParaIns.dbLeftOutAliX=pTree->m_DoubleOutParaIns.dbLeftOutAliX+dbLeftXAdd;
			pTree->m_DoubleOutParaIns.dbLeftOutAliY=pTree->m_DoubleOutParaIns.dbLeftOutAliY+dbLeftYAdd;
			pTree->m_DoubleOutParaIns.dbRightOutAliX=pTree->m_DoubleOutParaIns.dbRightOutAliX+dbRightXAdd;
			pTree->m_DoubleOutParaIns.dbRightOutAliY=pTree->m_DoubleOutParaIns.dbRightOutAliY+dbRightYAdd;
		}
	}

#endif
	
	return;
}

void CChain::GetAllSelTreeProp(DOUBLEOUTPARA &DbPara, CQuad &QD)
{

	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
#ifdef _DOUBLE_HEAD_MARK
			DbPara=pTree->m_DoubleOutParaIns;
#endif
			break;
		}
	}

	QD = GetLimit();

	return;
}

void CChain::SetAllSelTreeDoubleOutPara(DOUBLEOUTPARA &DoubleOutParaIns)
{
#ifdef _DOUBLE_HEAD_MARK
	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			pTree->m_DoubleOutParaIns=DoubleOutParaIns;
		}
	}
#endif
	return;
}

void CChain::GetSelTreeDoubleOutPara(DOUBLEOUTPARA &DoubleOutParaIns)
{
#ifdef _DOUBLE_HEAD_MARK
	POSITION pos=m_list.GetHeadPosition();
	BOOL bIsGetParam = FALSE;
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			DoubleOutParaIns = pTree->m_DoubleOutParaIns;
			CQuad quad       = pTree->CalLimit();
			DoubleOutParaIns.pos = quad.Center();
			bIsGetParam      = TRUE;
			return;
		}
	}

	if (!bIsGetParam)
	{
		AfxMessageBox(IDS_SELOBJ);
	}

#endif
	return;

}

void CChain::InsertChainObjToList()
{
	if (m_list.GetCount() <= 0) 
	{
		return;
	}

	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj)
			InserObjToList(pGroupObj);
	}

	return;
}

void CChain::InserObjToList(CGroupObj *pGroupObj)
{
	CMainFrame *pMainFram = (CMainFrame*)AfxGetMainWnd();
	pMainFram->m_XBarObjectlist.m_ObjectListDlg.AddClmItem(pGroupObj);
	
	return;
}

void CChain::DelObjInList(CGroupObj *pGroupObj)
{
	CMainFrame *pMainFram = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFram)
	{
		return;
	}
	
	pMainFram->m_XBarObjectlist.m_ObjectListDlg.DelClmItem(pGroupObj);

	return;
}

void CChain::DelAllObjInList()
{
	CMainFrame *pMainFram = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFram)
	{
		return;
	}
	
	pMainFram->m_XBarObjectlist.m_ObjectListDlg.DelAllClmItem();

	return;
}

void CChain::SetTreeSel(CGroupObj *pGroupObj, XTracker* pTracker)
{
	if ((!pGroupObj) ||(!pTracker))
	{
		return;
	}

	CGroupObj* pGroupObjTmp = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pGroupObjTmp = (CGroupObj *)m_list.GetNext(pos);
		if ((pGroupObjTmp == pGroupObj))
		{
			pGroupObj->SetSelect(TRUE);
			pTracker->ClearStrokeList();
			pGroupObj->SetStrokeList(&pTracker->m_StrokeList);
			break;
		}
	}// end while
	
	return;
}

void CChain::DelOBj(DWORD dwGroupObj)
{
	CGroupObj *pGroupObj = NULL;

	POSITION prev;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		prev=pos;
		pGroupObj=m_list.GetNext(pos);
		if((DWORD)pGroupObj == dwGroupObj)
		{
			m_list.RemoveAt(prev);
			delete pGroupObj;
		}
	}
	return;
}

void CChain::ObjListLoad() 
{
	CMainFrame *pMainFram = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFram)
		return;

	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{
		pGroupObj=m_list.GetNext(pos);

		if (pGroupObj)
			pMainFram->m_XBarObjectlist.m_ObjectListDlg.AddClmItem(pGroupObj);

	}

	pMainFram->m_XBarObjectlist.m_ObjectListDlg.m_DocObjList.UpdateListCtrl();

	return;
}

void CChain::ObjListUnLoad() 
{
	CMainFrame *pMainFram = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pMainFram)
	{
		return;
	}

	pMainFram->m_XBarObjectlist.m_ObjectListDlg.ObjListUnLoad();

	return;
}


void CChain::AjustTreeForQuad(CArray<SELQUAD,SELQUAD> &quadList)
{
	int count = quadList.GetSize();
	for (int i=0; i<count; i++)
	{
		SELQUAD selQuad = quadList.GetAt(i);
		//if (selQuad.m_bFill) continue;
		//CTree *pTree = (CTree *)selQuad.m_pGroupObj;
		CGroupObj * pGroupObj= (CGroupObj *)selQuad.m_pGroupObj;
		//CQuad quad = pTree->GetLimit();
		CQuad quad = pGroupObj->GetLimit();
		if (quad.Equal(selQuad.m_QD))
		{
			continue;
		}
		double rationX = selQuad.m_QD.Width()/quad.Width();
		double rationY = selQuad.m_QD.Height()/quad.Height();
		double selMidX = (selQuad.m_QD.left + selQuad.m_QD.right)/2;
		double selMidY = (selQuad.m_QD.top + selQuad.m_QD.bottom)/2;
		double midX = (quad.left + quad.right)/2;
		double midY = (quad.top + quad.bottom)/2;
		double moveX = selMidX - midX;
		double moveY = selMidY - midY;
		//pTree->Move(moveX, moveY);
		CDot dot;
		dot.x = selMidX;
		dot.y = selMidY;
		//pTree->Scale(dot, rationX, rationY);


		//pTree->Drag(dot, rationX, rationY, moveX, moveY);
		pGroupObj->Drag(dot, rationX, rationY, moveX, moveY);
	}
}

void CChain::RefreshSelectQuadList(CArray<SELQUAD,SELQUAD> &quadList)
{
	quadList.RemoveAll();

	int nIndex = -1;
	CQuad quad;

	CMainFrame *pMainFram = (CMainFrame*)AfxGetMainWnd();
	if (pMainFram)
	{
		pMainFram->SetAllObjecUnSel();
		pMainFram->m_XBarObjectlist.m_ObjectListDlg.ObjListLoad();
	}

	POSITION pos = m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if (pGroupObj->IsSelect()) 
		{
			SELQUAD QD;
			QD.m_bFill = pGroupObj->IsMainSelect();
			QD.m_bSel  = TRUE;
			QD.m_QD    = pGroupObj->GetLimit();
			quad.Compose(QD.m_QD);
			if (QD.m_QD.IsEmpty())
			{
				continue;
			}
			QD.m_pGroupObj = (DWORD)pGroupObj;
			quadList.Add(QD);

			int nId = pMainFram->SetObjecSel(pGroupObj);
			if (-1 == nIndex)
			{
				nIndex = nId;
			}
		}
	}

	if (-1 != nIndex)
	{
		pMainFram->m_XBarObjectlist.m_ObjectListDlg.m_DocObjList.ScrollToItem(nIndex);
	}

	theApp.m_RedPreView.SetQuadArray(quadList);

	m_quadLimit = quad;

	return;
}

double CChain::GetLength()
{
	double total = 0;
	POSITION pos = m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		total += pGroupObj->GetLength();
	}

	return total;
}

void CChain::OnMAlign(CArray<SELQUAD,SELQUAD> &quadList, int nType)
{
	CMonitorQuad monitorQuad(this);
	monitorQuad.DoQuadCmd(nType, quadList);

	return;
}

BOOL CChain::IsHaveTextOrCodeBar()
{	
	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			if (TRUE == pGroupObj->IsHaveTxtOrCodeBar())
			{
				return TRUE;
			}
		}
	}// end While
	
	return FALSE;
}


void CChain::ConvertToOneNodeDelete()
{
	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{
		prev=pos;
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if((pGroupObj->IsSelect()) && (!pGroupObj->IsBmpNode()))
		{
			m_list.RemoveAt(prev);
			delete pGroupObj;
		}
	}

	return;
}

void CChain::UpdateObjMarkSet(OBJMARKSET &ObjMarkSet)
{
	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			pGroupObj->m_MarkSet = ObjMarkSet;
		}
	}
}

void CChain::GetObjMarkSet(OBJMARKSET &ObjMarkSet)
{
	POSITION pos=m_list.GetHeadPosition();
	CGroupObj* pGroupObj = NULL;
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			ObjMarkSet = pGroupObj->m_MarkSet;
			return;
		}
	}

	return;
}

void CChain::StoreChain(const CChain& chain)
{ 
	//文本内容的复制
	m_MgrText = chain.m_MgrText;

	//删除所有的树
	DeleteList();

	CGroupObj *pGrouObj = NULL;
	CGroupObj *pGroupObjNew = NULL;
	POSITION pos = chain.m_list.GetHeadPosition();
	while( pos != NULL )
	{
		pGrouObj = (CGroupObj *)chain.m_list.GetNext(pos);
		pGroupObjNew = new CGroupObj(pGrouObj,0x01, FALSE);
		if ( NULL == pGroupObjNew )
			continue;

		m_list.AddTail(pGroupObjNew);
	}
}

void CChain::ReStoreChain(const CChain& chain)
{ 
	//文本内容的复制
	m_MgrText = chain.m_MgrText;
	
	//删除所有的树
	DeleteList();

	CGroupObj *pGrouObj = NULL;
	CGroupObj *pGroupObjNew = NULL;
	POSITION pos = chain.m_list.GetHeadPosition();
	while( pos != NULL )
	{
		pGrouObj = (CGroupObj *)chain.m_list.GetNext(pos);
		CGroupObj *pGroupObjNew = new CGroupObj(pGrouObj,0x01, TRUE);
		if( NULL == pGroupObjNew )
			continue;

		m_list.AddTail(pGroupObjNew);
	}
}

void CChain::SelMark(HWND hWnd, HANDLE hStop, SUBOBJMARKSET &stSub)
{
	CGroupObj* pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();

	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
	//		pGroupObj->SelMark(hWnd, hStop, stSub); 
		}
		
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(hStop,0))
		{
			break;
		}
	}

	return;
}

void CChain::FastMark(HWND hWnd)
{
	CGroupObj* pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	
	while(pos!=NULL)
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if(pGroupObj->IsSelect())
		{
			pGroupObj->Mark(hWnd); 
		}
		
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(hStop,0))
		{
			break;
		}
	}
	
	return;
}

void CChain::ClearALLMainSelect()
{
	CGroupObj* pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		pGroupObj->SetMainSelect(FALSE);
	}

	return;
}

CQuad& CChain::RetSelQuad()
{
	return m_quadLimit;
}


void CChain::SetProStep(int nStep)
{
	CMainFrame *pFWnd = (CMainFrame*)AfxGetMainWnd();
	if (NULL == pFWnd)
	{
		return;
	}

	pFWnd->SetProStep(nStep);

	return;
}

void CChain::AyCopy(CArray<OBJPOS, OBJPOS> &dtArray)
{
	int nAyCnt = dtArray.GetSize();
	if (0 == nAyCnt)
	{
		return;
	}

	OBJPOS fDot= dtArray[0];
	CList<CGroupObj*, CGroupObj*> Tplist;

	int nStep = 0;
	SetProStep(nStep);

	CQuad SelQuad;
	CGroupObj* pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while (pos) 
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if (NULL == pGroupObj)
			continue;

		if(pGroupObj->IsSelect())
		{
			Tplist.AddTail(pGroupObj);
			SelQuad.Compose(pGroupObj->GetLimit());
			pGroupObj->SetMainSelect(FALSE);
		}
	}

	if (SelQuad.IsEmpty())
	{
		return;
	}

	CDot BaseDot = SelQuad.GetBaseDot();
	for (int i = 1; i < nAyCnt; i++)
	{
		CGroupObj* pGroupObj = NULL;
		OBJPOS dotNew  = dtArray[i];
		POSITION pos = Tplist.GetHeadPosition();
		while (pos)
		{
			pGroupObj  = Tplist.GetNext(pos);

			if(g_dwJumpCnt != -1)
				g_dwJumpCnt = i;

			CGroupObj *pNewGroupObj = new CGroupObj(pGroupObj);
			if (NULL == pNewGroupObj)
				continue;

			#ifdef _DOUBLE_HEAD_MARK
				pNewGroupObj->m_DoubleOutParaIns.dbLeftOutAliX  = dotNew.Lf.x;
				pNewGroupObj->m_DoubleOutParaIns.dbLeftOutAliY  = dotNew.Lf.y;
				pNewGroupObj->m_DoubleOutParaIns.dbRightOutAliX = dotNew.Rf.x;
				pNewGroupObj->m_DoubleOutParaIns.dbRightOutAliY = dotNew.Rf.y;
			#endif

			pNewGroupObj->Move(dotNew.x - BaseDot.x, dotNew.y - BaseDot.y);
			CDot centDt(dotNew.x, dotNew.y);
			pNewGroupObj->Rotate(centDt, dotNew.z);
			m_list.AddTail(pNewGroupObj);
		}

		nStep = (int)((double)i / (double)nAyCnt * 100);
		SetProStep(nStep);
	}

	POSITION ePos = Tplist.GetHeadPosition();
	while (ePos) 
	{
		CGroupObj *pGroupObj = Tplist.GetNext(ePos);
		if (NULL == pGroupObj)
		{
			continue;
		}

		#ifdef _DOUBLE_HEAD_MARK
			pGroupObj->m_DoubleOutParaIns.dbLeftOutAliX  = fDot.Lf.x;
			pGroupObj->m_DoubleOutParaIns.dbLeftOutAliY  = fDot.Lf.y;
			pGroupObj->m_DoubleOutParaIns.dbRightOutAliX = fDot.Rf.x;
			pGroupObj->m_DoubleOutParaIns.dbRightOutAliY = fDot.Rf.y;
		#endif

		pGroupObj->Move(fDot.x - BaseDot.x, fDot.y - BaseDot.y);
		CDot centDt(fDot.x, fDot.y);
		pGroupObj->Rotate(centDt, fDot.z);
		pGroupObj->SetMainSelect(TRUE);
	}

	SetProStep(0);

	return;
}

BOOL CChain::ChangeText(CString strTxt,CString ModelNo)
{
	return m_MgrText.ChangeText(strTxt,ModelNo);
}

void CChain::JoinStroke(BOOL bPopMsg,double dbMinDistance)
{
	BOOL bJoinStroke = TRUE;
	CGroupObj *pGroupObj = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pGroupObj = (CGroupObj *)m_list.GetNext(pos);
		if ( !pGroupObj )
			continue;

		if ( pGroupObj->IsSelect() && (pGroupObj->GetLockFlg() == FALSE))
			pGroupObj->JoinStroke(dbMinDistance);
	}
}

void CChain::ResetDivideFlag()
{
	CGroupObj *pGroupObj  = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj = static_cast<CGroupObj*>(m_list.GetNext(pos));
		pGroupObj->m_bMarked = FALSE;
	}

	pos = m_listCopyObject.GetHeadPosition();
	while(pos)
	{
		m_listCopyObject.GetNext(pos).m_bMarked = FALSE;
	}
}

void CChain::SetNeedMarkFlag(BOOL arrNeedMark[600])
{
	CGroupObj *pGroupObj  = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pGroupObj = static_cast<CGroupObj*>(m_list.GetNext(pos));
		if (pGroupObj->m_nGroupNum>0 && pGroupObj->m_nGroupNum<600)
		{
			pGroupObj->m_bNeedMark = arrNeedMark[pGroupObj->m_nGroupNum];
		}
	}
}

void CChain::GetStrokeFram(CQuad &quad, CTree &Tree)
{
	CStroke *pStroke;
	StrokeList &strokeList = Tree.GetRoot()->m_list;
	POSITION pos=strokeList.GetHeadPosition();
	while(pos)
	{
		pStroke = strokeList.GetNext(pos);
		delete pStroke;
	}
	strokeList.RemoveAll();

	pStroke = new CStroke;
	quad = GetLimit();
	CDot LeftTopDot,LeftBottemDot,RightTopDot,RightBottemDot;
	LeftTopDot.x = LeftBottemDot.x = quad.left;
	RightTopDot.x = RightBottemDot.x = quad.right;
	LeftTopDot.y = RightTopDot.y = quad.top;
	LeftBottemDot.y = RightBottemDot.y = quad.bottom;

	pStroke->m_dotArray.Add(LeftTopDot);
	pStroke->m_dotArray.Add(LeftBottemDot);
	pStroke->m_dotArray.Add(RightBottemDot);
	pStroke->m_dotArray.Add(RightTopDot);
	pStroke->m_dotArray.Add(LeftTopDot);
	strokeList.AddTail(pStroke);

}


void CChain::GetStrokeInQuad(CQuad &quad, StrokeList &strokeList, BOOL bDivedeByStroke)
{

	CQuad quadEmpty, quadInflate= quad;
	quadEmpty.SetEmpty();
	quadInflate.Inflate(0.001, 0.001);

	CStroke *pStroke;
	POSITION pos=strokeList.GetHeadPosition();
	while(pos)
	{
		pStroke = strokeList.GetNext(pos);
		delete pStroke;
	}
	strokeList.RemoveAll();

	CDot dot;
	CQuad quadGroupObj;
	CGroupObj *pGroupObj  = NULL;
	pos=m_list.GetHeadPosition();
	if (pos==NULL) return;

	if (m_listCopyObject.GetCount())
	{
		if (pos) m_list.GetNext(pos);
	}

	while(pos!=NULL)
	{
		pGroupObj = static_cast<CGroupObj*>(m_list.GetNext(pos));

		if (!pGroupObj->m_bNeedMark)
			continue;

		if ((!m_bSelMark) || (m_nSelectMarkType!=0) || pGroupObj->IsSelect())
		{
			quadGroupObj = pGroupObj->GetLimit();
			if (bDivedeByStroke)	//按线条切分
			{
				if (quadGroupObj.right<quadInflate.left || quadGroupObj.left>=quadInflate.right || quadGroupObj.top<=quadInflate.bottom || quadGroupObj.bottom>quadInflate.top)
					continue;

				pGroupObj->GetStrokeInQuad(quad, strokeList);
			}
			else					//按对象切分
			{
				dot = quadGroupObj.GetBaseDot();
				if (quad.DotIn(dot) && !pGroupObj->m_bMarked)
				{
					pGroupObj->m_bMarked = TRUE;
					pGroupObj->GetStrokeInQuad(quadEmpty, strokeList);
				}
			}
		}
	}

	//Copy Object
	CQuad quadCopy = quad;
	//quadCopy.TrueToLog();
	pos = m_listCopyObject.GetHeadPosition();
	pGroupObj = m_list.GetHead();
	if (pGroupObj && pos)
	{
		CGroupObj *pGroupObjNew = new CGroupObj(pGroupObj);
		pGroupObjNew->PreprocessMark(0);
		pGroupObjNew->CalLimit(CDot(0,0), TRUE, 1.0, 1.0);

		CDot dotTemp, dotNewTree = pGroupObjNew->GetLimit().GetBaseDot();
		while(pos)
		{
			CCopyObject &obj = m_listCopyObject.GetNext(pos);

			if ((!m_bSelMark) || obj.m_bSelect)
			{
				dotTemp = obj.m_quadLimit.GetBaseDot();
				if (quadCopy.DotIn(dotTemp) && !obj.m_bMarked)
				{
					pGroupObjNew->Move(dotTemp.x-dotNewTree.x, dotTemp.y-dotNewTree.y);
					pGroupObjNew->GetStrokeInQuad(quadEmpty, strokeList);

					obj.m_bMarked = TRUE;
					dotNewTree = dotTemp;
				}
			}
		}
		delete pGroupObjNew;
	}
}


