// GroupObj.cpp: implementation of the CGroupObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GroupObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGroupObj::CGroupObj()
{
	m_bSelect = FALSE;
	m_bMainSelect = FALSE;

	m_bNeedMark = TRUE;
	m_bMarked = FALSE;

	m_nGroupNum = 0;
	m_nGroupGraphicType = 0;
}

CGroupObj::CGroupObj( const CGroupObj *src, UINT nStyle, BOOL bFill )
{
	m_bNeedMark = TRUE;
	m_bMarked = FALSE;

	m_nGroupNum = 0;
	m_nGroupGraphicType = src->m_nGroupGraphicType;

	DeleteAll();

	m_strObjName = src->m_strObjName;
	m_bSelect = src->m_bSelect;
	
	CTree *pTree = NULL;
	POSITION pos = src->m_list.GetHeadPosition();
	while( pos != NULL )
	{
		pTree = (CTree*)(src->m_list.GetNext(pos));

		if( pTree != NULL )
		{
			CTree *pTreeNew = new CTree(pTree,nStyle,bFill);
			m_list.AddTail(pTreeNew);
		}
	}

	SetSelect(m_bSelect);
}


CGroupObj::~CGroupObj()
{
	DeleteAll();
}

BOOL CGroupObj::CopyToMe( CGroupObj *pSrc, DWORD dwStyle,BOOL bGenFill )
{
	if ( pSrc == NULL )
		return FALSE;

	DeleteAll();
	/////
	m_strObjName = pSrc->m_strObjName;
	m_bSelect = pSrc->m_bSelect;
	
	CTree *pTree = NULL;
	POSITION pos = pSrc->m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)(pSrc->m_list.GetNext(pos));
		if ( pTree  !=  NULL )
		{
			CTree *pTreeNew = new CTree(pTree, dwStyle);
			if ( pTreeNew  !=  NULL )
			{
				Add(pTreeNew) ;
				if ( bGenFill ) //拷贝时需要生成填充线
					pTreeNew->Fill();
			}
		}
	}
	SetSelect(m_bSelect);

	return TRUE;
}

//
CQuad CGroupObj::GetLimit()
{
	CQuad quad;
	quad.Initial();
	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
			quad.Compose( pTree->CalLimit() );
	}
	
	return quad;
}

BOOL CGroupObj::Mark( HWND hWnd )
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
			return FALSE;

		pTree = (CTree*)m_list.GetNext(pos);
		pTree->Mark(hWnd);
	}

	return TRUE;
}


void CGroupObj::Rotate(CDot &dot, double angle)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->Rotate(dot,angle);
	}
}


void CGroupObj::UnGroup( CTypedPtrList<CObList,CGroupObj *> &listGroupObj )
{
	CTree *pTree = NULL;
	POSITION prev = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		prev = pos;
		pTree = (CTree*)m_list.GetNext( pos );
		
		if ( pTree )
		{
			CTree *pTreeNew = NULL;
			CTree *pTreePre = NULL;
			//////////
			pTree->m_bUnGroupOnlyRoot = (pTree->m_pRoot->m_pLeft==NULL);
			while( pTreeNew = pTree->UnGroup() )
			{
				pTreeNew->SetSelect();
				Add(pTreeNew);
				pTreePre = pTreeNew;
			}
			
			if ( pTreePre )
			{
				delete pTree;
				pTree = NULL;	
				m_list.RemoveAt(prev);
			}
		}
	}
	//
	UnUnionGroup(listGroupObj);
}

//
void CGroupObj::Group(CGroupObj *pGroupObj)
{
	if ( pGroupObj == NULL )
		return;
	
	UnionGroup(pGroupObj);
	//
	CTree *pTree    = NULL;
	CTree *pTreeNew  = NULL;
	
	BOOL  bCreate = FALSE;
	POSITION prev = NULL;
	POSITION pos   = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		prev   = pos;
		pTree = (CTree*)m_list.GetNext(pos);
		
		if ( pTree &&  pTree->CanGroup() )
		{
			if ( !bCreate )//stroke未创建
			{
				pTreeNew = new CTree();
				if ( pTreeNew == NULL )
				{
					OutputDebugString(_T("CGroupObj::Group() new CTree failure!"));
					return;
				}
				bCreate = TRUE;
				m_list.InsertBefore(pos,pTreeNew);
			}
			if ( pTreeNew )
			{
				m_list.RemoveAt(prev);
				pTreeNew->Group(pTree);
				delete pTree;
				pTree = NULL;
			}
		}
	}
}


//
void CGroupObj::DeleteAll()
{	
	POSITION pos  = m_list.GetHeadPosition();
	CTree *pTree  = NULL;
	while( pos  !=  NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		{	
			delete pTree;
			pTree = NULL;
		}
	}

	m_list.RemoveAll();
}



void CGroupObj::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize sa(fp);

	CTree *pTree = NULL;
	
	if ( bFlag )
	{	
		sa << _T("GROUP_B");
		
		sa << 100;
		sa << m_bSelect;
		
		sa << 101;
		sa << m_strObjName;

		sa<<102;
		sa<<m_nGroupNum;

		sa<<103;
		sa<<m_nGroupGraphicType;
		
		POSITION pos = m_list.GetHeadPosition();
		while( pos  !=  NULL )
		{	
			pTree = (CTree*)m_list.GetNext(pos);
			pTree->Serialize(fp,bFlag);
		}

		sa << _T("GROUP_E");
	}
	else
	{	
		CString strRead;
		while( sa >> strRead )
		{
			if ( strRead == _T("GROUP_E") ){
				break;			
			}
			else if ( strRead == _T("T_B") )			
			{
				pTree = new CTree();
				if ( pTree == NULL )
				{
					OutputDebugString(_T("CGroupObj::Serialize() new CTree error!\n"));
					return;
				}
				pTree->Serialize(fp,bFlag);				
				Add(pTree);
			}

			switch ( _ttoi(strRead) )
			{
			case 100:
				sa >> m_bSelect;
				break;

			case 101:
				sa >> m_strObjName;
				break;
			case 102:
				sa>>m_nGroupNum;
				break;
			case 103:
				sa>>m_nGroupGraphicType;
				break;
			}
		}
		SetSelect(m_bSelect);
	}
}

void CGroupObj::SetSelect(BOOL bFlag)
{	
	m_bSelect = bFlag;

	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos  !=  NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		pTree->SetSelect(m_bSelect);
	}
}


void CGroupObj::Scale(CDot dot,double ratioX,double ratioY)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->Scale(dot,ratioX,ratioY);
	}
}


//
void CGroupObj::Drag(CDot dot, double ratioX, double ratioY, double moveX, double moveY)
{	
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->Drag(dot,ratioX,ratioY,moveX,moveY);
	}
}


void CGroupObj::Move(double moveX,double moveY)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->Move(moveX,moveY);
	}
}

void CGroupObj::Draw( XDC *pDC )
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		///////
		if ( pTree  !=  NULL  )
			pTree->Draw( pDC );
	}
}

//
BOOL CGroupObj::Add( CTree *pTree )
{
	if ( pTree == NULL )
		return FALSE;
	
	m_list.AddTail(pTree);
	/////
	return TRUE;
}

//
void CGroupObj::InsertHead(CTree *pTree)
{
	if ( pTree == NULL )
		return;
	
	m_list.AddHead(pTree);	
}


//
BOOL CGroupObj::IsSelect()
{
	return m_bSelect;
}


BOOL CGroupObj::Include(CQuad quad)
{
	CQuad quadTemp = GetLimit();
	return quad.Include(quadTemp);
}

//
void CGroupObj::InvertSelect()
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);		
		if ( pTree  !=  NULL )
			pTree->InvertSelect();
	}
	
	m_bSelect = !m_bSelect;
}


BOOL CGroupObj::GetLockFlg()
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			if ( pTree->GetLockFlg() == TRUE )//只要任意Tree被锁定，整个Group锁定
			{
				return TRUE;
			}
		}
	}
	
	return FALSE;
}


void CGroupObj::UnionGroup(CGroupObj *pGroupObj)
{
	if ( pGroupObj == NULL )
		return;
	
	POSITION pos = pGroupObj->m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while(pos != NULL)
	{	
		pTree = (CTree*)(pGroupObj->m_list.GetNext(pos));
		Add(pTree);
	}
	
	pGroupObj->m_list.RemoveAll();
}

void CGroupObj::UnUnionGroup( CTypedPtrList<CObList,CGroupObj *> &listGroupObj )
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL)
		{	
			CGroupObj *pGroupObj = new CGroupObj();
			if ( pGroupObj  !=  NULL )
			{
				pGroupObj->Add(pTree);
				pGroupObj->SetSelect(TRUE);
				listGroupObj.AddTail(pGroupObj);
			}
		}
	}
	
	m_list.RemoveAll();
}


//
BOOL CGroupObj::CheckBarCode(CUIntArray &arrayTextID)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree *)m_list.GetNext(pos);	
		if ( pTree  !=  NULL )
		{
			pTree->CheckBarCode(arrayTextID);
		}
	}
	
	return TRUE;
}



void CGroupObj::SelectBarCode(CUIntArray& arrayTextID)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree *)m_list.GetNext(pos);
		////////
		if ( pTree  !=  NULL )
		{
			if ( pTree->SelectBarCode(arrayTextID) == TRUE )
			{
				SetSelect(TRUE);
				break;
			}
		}
	}
}


CGroupObj*  CGroupObj::ExtendObj(double dbSpace, BOOL bOpt)
{
	CGroupObj *pGroupObj = new CGroupObj();
	if ( pGroupObj == NULL )
		return NULL;

	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pTree = (CTree *)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			CTree *pTreeNew = pTree->ExtendObj(dbSpace, bOpt);
			if (pTreeNew  !=  NULL)
			{
				pGroupObj->Add(pTreeNew);
			}
		}
	}

	if ( pGroupObj->m_list.GetCount()  <=  0 )
	{
		delete pGroupObj;
		return NULL;
	}
	else
		return pGroupObj;
}

void CGroupObj::SetFillStyle(FILLDATA& fill)
{
	CTree *pTree = NULL;
	POSITION pos =  m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{
		pTree = (CTree *)m_list.GetNext(pos);
		if ( pTree != NULL )
		{
			pTree->SetFillStyle(fill);
		}
	}
}


void CGroupObj::GetFillStyle(FILLDATA& fill)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL)
		{
			pTree->GetFillStyle(fill);
			return;
		}
	}
}


BOOL CGroupObj::IsMainSelect()
{
	return m_bMainSelect;
}

//
void CGroupObj::SetMainSelect(BOOL bSelect)
{
	m_bMainSelect = bSelect;
}


void CGroupObj::SetProperty(int mode,CProperty pty)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree	 = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->SetProperty(mode,pty);
	}
}


BOOL CGroupObj::PreprocessMark(DWORD nStyle)
{		
	CTree *pTree = NULL;	
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->PreprocessMark(nStyle);
	}

	return TRUE;
}

CNode * CGroupObj::GetNode()
{
	CTree *pTree = NULL;	
	CNode *pNode = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			pNode = pTree->GetRoot();
			if ( pNode)
				return pNode;
			else
				return NULL;
		}
	}
	
	return NULL;
}



void CGroupObj::GetArrayDot(CArray<OBJPOS, OBJPOS> &dtArray)
{	
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	CQuad  quad;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		{
			quad  = pTree->GetLimit();
			CDot dt =  quad.GetBaseDot();

			OBJPOS dt6;
			dt6.x = dt.x;
			dt6.y = dt.y;
			/////
			dtArray.Add(dt6);
		}
	}
}


void CGroupObj::CalLimit(CDot dot,BOOL bAll,double dbXScale,double dbYScale )
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->CalLimit();
	}
}

//
void CGroupObj::SetLayColor(int nLayer,int nFillLayer)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();

	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
			pTree->SetLayColor(nLayer,nFillLayer);
	}
}



void CGroupObj::SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		{
			pTree->SetObjColor(nObjIndex, nObjColor, nFillIndex, nFillColor);
		}
	}
}


void CGroupObj::GetLayColor(int &nLayer,int &nFillLayer)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		{
			pTree->GetLayColor(nLayer,nFillLayer);
			return;
		}
	}
}


CQuad CGroupObj::GetReFurbishQuad()
{
	CQuad quad;
	quad.SetEmpty();
	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
			quad.Union(pTree->GetReFurbishQuad());
	}

	return quad;	
}

//
void CGroupObj::GetSelectedStrokeList(StrokeList *pStrokeList)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree )
		  pTree->GetStrokeList(pStrokeList);
	}
}

CQuad CGroupObj::GetMarkRange()
{
    CQuad quad;
	quad.SetEmpty();

	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
			quad.Union(pTree->GetMarkRange());
	}
	
	return quad;
}


BOOL CGroupObj::GetFirstMarkCurrent(double & dbCurrent)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;		
	while( pos != NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL )
		{
			pTree->GetFirstMarkCurrent(dbCurrent);		
			return TRUE;
		}
	}
	
	return FALSE;
}

//
BOOL CGroupObj::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{	
	CTree *pTree = NULL;
	BOOL  bRtn = TRUE;
	POSITION pos = m_list.GetHeadPosition();
	while(pos  !=  NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		bRtn  &=  pTree->ExportPlt(file,bUnExportFill,bScall100);
	}
	return bRtn;
}

//
void CGroupObj::AddRect(CWnd *pWnd)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		{
			pTree->AddRect(pWnd);
		}
	}
}	


void CGroupObj::CaptureObject(CQuad &quad,XTracker *pTrack)
{
	if ( Include(quad) == FALSE )
		return;
	
	BOOL bCapture = FALSE;
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		
		if ( pTree  !=  NULL )
		{				
			pTree->SetStrokeList(&pTrack->m_StrokeList);
			bCapture = TRUE;
		}
	}
	
	if  ( bCapture == TRUE )
		SetSelect(TRUE);
}

//
BOOL CGroupObj::Click(CPoint &point,CWnd *pWnd)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->Click(point,pWnd) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

int CGroupObj::GetType()
{	
	int nType = NODE_BASE;
	if ( m_list.GetCount() > 1 )
	{
		nType = NODE_UNION;
	}
	else if (m_list.GetCount() == 1)
	{
		CTree *pTree = (CTree *)m_list.GetHead();
		if ( pTree->IsGroup() == TRUE )
		{
			nType = NODE_COMBIN;
		}
		else
		{
			nType = pTree->GetType();
		}
	}
	
	return nType;
}

//
void CGroupObj::RefreshBarText(CWnd* pWnd,UINT nTextID)
{
	/*
	PreprocessMark(0x00);
	//
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL && pTree->GetRoot()  )
			pTree->RefreshBarText(pTree->GetRoot(),pWnd,nTextID);
	}
	*/
}

//
void CGroupObj::SetPathOptimizeState(BOOL bState)
{
	/*
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
			pTree->SetPathOptimizeState(bState);
	}
	*/
}

BOOL CGroupObj::GetSelObjLockFlg()
{	
	BOOL bLock = FALSE;
	BOOL bFirst = TRUE;
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree == NULL) 
			continue;
		
		if ( bFirst )
			bLock = pTree->GetLockFlg();
		else
			bLock  |=  pTree->GetLockFlg();
	}

	return bLock;
}

int CGroupObj::GetSelObjLayerNo()
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if (  pTree != NULL && pTree->GetRoot() )
			return pTree->GetRoot()->m_property.m_nLayer;
	}
	/////
	return 0;
}

//
void CGroupObj::SetLayerSelect(int nItem,BOOL bFlag)
{	
	/*
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->SetLayerSelect(nItem,bFlag) == TRUE ) 
		{
			SetSelect(bFlag);
			break;
		}
	}
	*/
}

void CGroupObj::DrawExtern(XDC *pDC)
{
	POSITION pos = m_list.GetHeadPosition();
	while (pos  !=  NULL)
	{
		CTree *pTree = (CTree*)m_list.GetNext(pos);
		//////
		if ( pTree != NULL)
			pTree->DrawExtern(pDC);
	}
}

void CGroupObj::UpdateObjMarkSet(OBJMARKSET &ObjMarkSet)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL )
			pTree->m_MarkSet = ObjMarkSet;
	}
}

BOOL CGroupObj::IsBmpNode()
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->IsBmpNode() )
			return TRUE;
	}
	return FALSE;
}

BOOL CGroupObj::IsHaveTextOrCodeBar()
{	
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->IsHaveTxtOrCodeBar() )
			return TRUE;
	}
	
	return FALSE;
}

double CGroupObj::GetLength()
{
	double dbTotal = 0;
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL )
			dbTotal  +=  pTree->GetLength();
	}
	
	return dbTotal;
}

void CGroupObj::SetGroupNum(int nNum)
{
	m_nGroupNum = nNum;
}

int CGroupObj::GetGroupNum()
{
	return m_nGroupNum;
}

//
BOOL CGroupObj::ComToOneGroupObj(CNode *pNode)
{
	if ( NULL == pNode )
		return FALSE;
	
	BOOL bIsHaveData = FALSE;
	CTree* pTree = NULL;
	BOOL bRet = TRUE;
	
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree ) 
			continue;
		//
		pTree->ComToOneNode(pNode);

		//if (  !pTree->ComToOneNode(pNode)  )
		if (pNode == NULL)
		{
			bRet = FALSE;
			break;
		}
	}
	
	return bRet;
	//return TRUE;
}



BOOL CGroupObj::GetFirstDot(CDot &dt)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  && pTree->GetFirstDot(dt) )
			return TRUE;
	}
	
	return FALSE;
}

BOOL CGroupObj::GetLastDot(CDot &dt)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  && pTree->GetLastDot(dt) )
			return TRUE;
	}
	
	return FALSE;
}


//
void CGroupObj::FilterPoint(double dbPrecis, double dbAngle)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree) 
			continue;

		pTree->FilterPoint(dbPrecis, dbAngle);
	}
}


void CGroupObj::JoinStroke(double dbMinDistance)
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree ) 
			continue;
		
		pTree->JoinStroke(dbMinDistance);
	}
}

void CGroupObj::Convert()
{/*
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	POSITION prev =  NULL;
	while( pos )
	{
		prev = pos;
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree ) 
			continue;
		
		if ( pTree != NULL && pTree->CanConvert() )//转为曲线
		{	
			CTree *pNewTree = pTree->ConvertPLT();				
			CTree *pNewFillTree = pTree->ConvertFillToPLT();
	
			if ( pNewTree )
			{	
				Add(pNewTree);
				pNewTree->SetSelect(TRUE);
			}

			if ( pNewFillTree )
			{
				Add(pNewFillTree);
				pNewFillTree->SetSelect(TRUE);
			}
			
			delete pTree;
			m_list.RemoveAt(prev);
		} 
	}*/
}



UINT CGroupObj::CanConvert()
{
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree ) 
			continue;
		
		if ( pTree->CanConvert() == 0 )
			return 0;
	}

	return 1;
}



CGroupObj * CGroupObj::ConvertPLT()
{
	CGroupObj *pGroupObj = new CGroupObj();
	if ( pGroupObj == NULL )
		return NULL;
/*
	POSITION pos = m_list.GetHeadPosition();
	while (pos != NULL)
	{
		CTree *pTree = (CTree*)m_list.GetNext(pos);
		if ( NULL == pTree  )
			continue;

		CTree *pTreeNew = pTree->ConvertPLT();
		CTree *pFillTreeNew = pTree->ConvertFillToPLT();

		if ( pTreeNew  !=  NULL )
		{
			pGroupObj->Add(pTreeNew);
		}
		if ( pFillTreeNew  !=  NULL )
		{
			pGroupObj->Add(pFillTreeNew);
		}
	}*/
	
	return pGroupObj;
}

void CGroupObj::GetStrokeList(StrokeList *pStrokeList)
{	
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree  !=  NULL )
		  pTree->GetStrokeList(pStrokeList);
	}
}

BOOL CGroupObj::IsHaveTxtOrCodeBar()
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && pTree->IsHaveTxtOrCodeBar() )
			return TRUE;
	}
	
	return FALSE;
}


void CGroupObj::SetSortStatus(BOOL bIsSort)
{
	POSITION pos = m_list.GetHeadPosition();
	CTree *pTree = NULL;
	while( pos != NULL )
	{	
		pTree = (CTree*)m_list.GetNext(pos);
		
		if ( pTree != NULL)
		{
			pTree->SetSortStatus(bIsSort);
		}
	}
}


BOOL CGroupObj::HaveFunctionNode()
{/*
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree != NULL && pTree->HaveFunctionNode() )
			return TRUE;
	}*/
	return FALSE;
}

BOOL CGroupObj::GetRulerPos(CDot &dot, int nAlignType)
{/*
	CQuad quad;
	quad.SetEmpty();	
	//////
	CTree *pTree =  NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( pTree && !pTree->HaveNode(NODE_HLTEXT) )
		{
			quad.Compose(pTree->CalLimit());
		}			
	}

	if ( quad.IsEmpty() )
		return FALSE;

	if ( nAlignType == 0 )
		dot = quad.GetBaseDot(BASE_TM);
	else
		dot = quad.GetBaseDot(BASE_BM);
*/
	return TRUE;
}

void  CGroupObj::SetTextData(CString &strTxt)
{/*
	CTree *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( !pTree )
			continue;

		pTree->SetTextData(pTree->GetRoot(),strTxt);
	}
	*/
}

void  CGroupObj::GetTextValue(CString& strTxt)
{/*
	CString  strTmp;
	CTree  *pTree = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while( pos  !=  NULL )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if ( !pTree )
			continue;

		pTree->GetTextValue(pTree->GetRoot(),strTmp);
		if ( !strTmp.IsEmpty() )
			strTxt  +=  strTmp;
	}*/
}

void CGroupObj::SetStrokeList(StrokeList *pStrokeList)
{
	CTree *pTree=NULL;
	POSITION pos=m_list.GetHeadPosition();
	while( pos )
	{
		pTree = (CTree*)m_list.GetNext(pos);
		if(pTree != NULL)
		{		
//			if(pTree->m_Fill.bFillFirst == FALSE)
				pTree->SetStrokeList(pStrokeList);

			pTree->AddFillToStrokeList(pStrokeList);

//			if(pTree->m_Fill.bFillFirst == TRUE)
//				pTree->SetStrokeList(pStrokeList);
		}
	}
}

void CGroupObj::GetStrokeInQuad(CQuad &quad, StrokeList &strokeList)
{
	CQuad quadTree;
	CTree *pTree = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pTree = static_cast<CTree*>(m_list.GetNext(pos));

		if (!quad.IsEmpty())	//如果不是按对象切分
		{
			quadTree = pTree->GetLimit();
			if (quadTree.right<quad.left || quadTree.left>=quad.right || quadTree.top<=quad.bottom || quadTree.bottom>quad.top)
				continue;
		}

		pTree->GetStrokeInQuad(quad, strokeList);
	}

	//根据图形编号和类型设置权脂区域
	int nResinRect=0;
	if (m_nGroupGraphicType==3)
	{
		if (m_nGroupNum>=1 && m_nGroupNum<=24)
		{
			if (m_nGroupNum<=6) 
				nResinRect=1;
			else if (m_nGroupNum<=12)
				nResinRect=2;
			else if (m_nGroupNum<=18)
				nResinRect=3;
			else
				nResinRect=4;
		}
	}
	pos = strokeList.GetHeadPosition();
	while(pos)
	{
		strokeList.GetNext(pos)->m_nResinRect = nResinRect;
	}
}