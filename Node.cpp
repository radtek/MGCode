// Node.cpp: implementation of the CNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "Node.h"

#include "NodePlt.h"
#include "NodeLine.h"
#include "Tree.h"
#include "NodeRect.h"
#include "NodeEllipse.h"
#include "NodeGrid.h"
#include "NodePolygon.h"
#include "NodeText.h"
#include "NodeDib.h"
#include "NodeCodebar.h"
#include "Nodedxf.h"
#include "NodeDot.h"
#include "NodeGerber.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNode,CObject,2000)

CNode::CNode()
{
	m_type    = NODE_BASE;
	m_pLeft   = NULL;
	m_pRight  = NULL;

	m_bSelect  = FALSE;
	m_bGroup   = FALSE;
	m_nObjID   = 0;
	m_nObjType = NODE_BUTN;

	m_bIsCurSortTree = FALSE;

	m_nMarkMode = ENU_MARKNORMALL;
	m_nResinRect = 0;

}

CNode::~CNode()
{
	Delete();
}

CNode * CNode::CreateFromType(INT type)
{
	//Fencer edit
 	CNode *pNew = NULL;
	switch(type)
	{
		case NODE_PLT:
			pNew=new CNodePlt();
			break;
		case NODE_LINE:
			pNew=new CNodeLine();
			break;
		case NODE_RECT:
			pNew=new CNodeRect();
			break;
		case NODE_TEXT_DSF:
			pNew=new CNodeText();
			break;
		case NODE_ELLIPSE:
			pNew=new CNodeEllipse();
			break;
		case NODE_BMP:
			pNew=new CNodeDib();
			break;
		case NODE_GRID:
			pNew=new CNodeGrid();
			break;
		case NODE_POLYGON:
			pNew=new CNodePolygon();
			break;
		case NODE_BASE:
			pNew=new CNode();
			break;
		case NODE_DXF:
			pNew=new CNodeDxf();
			break;
		case NODE_CODEBAR:
			pNew=new CNodeCodebar();
			break;
		case NODE_DOT:
			pNew=new CNodeDot();
			break;
		case NODE_GERBER:
			pNew =new CNodeGerber();
			break;

	}
	pNew->m_nResinRect = 0;
	return pNew;
}

void CNode::Draw(XDC *pDC)
{
	CStroke::m_bRedColorDraw = m_bSelect;

	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{
		pStroke=m_list.GetNext(pos);

		if (m_type == NODE_DOT)
		{
			pStroke->Draw(pDC, FONT_MATRIX, m_bIsCurSortTree);
		}
		else
		{
			pStroke->Draw(pDC, 0, m_bIsCurSortTree);
		}
	}
	return ;
}

void CNode::Adjust(MSG &msg,const CPoint &ptOrg)
{
	//CTRL控制
	if(MK_CONTROL&msg.wParam)
	{
		CPoint pointNow;
		pointNow.x = (int)(short)LOWORD(msg.lParam);
    	pointNow.y = (int)(short)HIWORD(msg.lParam);

		int x,y,r;
		x=pointNow.x-ptOrg.x;
		y=pointNow.y-ptOrg.y;
		r=min(abs(x),abs(y));
		x=(x>0?1:-1)*r;
		y=(y>0?1:-1)*r;

		msg.lParam=ptOrg.y+y;
		msg.lParam<<=16;
		msg.lParam+=(ptOrg.x+x);
	}
}

BOOL CNode::CanUnGroup()
{
	return FALSE;
}

BOOL CNode::CanGroup()
{
	return TRUE;
}

CTree * CNode::UnGroup()
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	if(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		CNodeLine *pNode;
		pNode=new CNodeLine(pStroke);
		CTree *pTree=new CTree(pNode);
		
		m_list.RemoveHead();
		
		return pTree;
	}
	return NULL;
}

CNode * CNode::Copy(UINT nStyle)
{
	CNode *tmp = CreateFromType(m_type);
	if (NULL == tmp)
	{
		return NULL;
	}
	
	POSITION pos     = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos!=NULL)
	{	
		pStroke = m_list.GetNext(pos);
		CStroke *pNew = pStroke->Copy();
		if (NULL == pNew)
		{
			continue;
		}

		tmp->m_list.AddTail(pNew);
	}
	
	tmp->m_property = m_property;
	tmp->m_bSelect  = m_bSelect;
	tmp->m_nResinRect = m_nResinRect;
	return tmp;
}

void CNode::Delete()
{
	POSITION prev = NULL;
	POSITION pos  = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	while(pos!=NULL)
	{	
		prev=pos;
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
			m_list.RemoveAt(prev);
		    delete pStroke;
			pStroke = NULL;
		}
	}// end while

	return;
}

BOOL CNode::Move(double moveX,double moveY)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
			pStroke->Move(moveX,moveY);
		}
	}// end While

	return TRUE;
}

void CNode::Rotate(CDot dot,double angle)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
		    pStroke->Rotate(dot,angle);
		}
	}
	CalLimit();

	return;
}

void CNode::Scale(CDot dot,double ratioX,double ratioY)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
			pStroke->Scale(dot,ratioX,ratioY);
		}
	}// end while

	CalLimit();

	return;
}

void CNode::Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY)
{
	Move(moveX,moveY);

	Scale(dot,ratioX,ratioY);
	
	return;
}

BOOL CNode::Create(CRect &rect)
{
	return TRUE;
}

BOOL CNode::Track(CWnd* pWnd,CPoint ptOrg)
{
	return TRUE;
}

CQuad CNode::CalLimit()
{
	//m_quadLimit.Initial();
	m_quadLimit.left = m_quadLimit.right = m_quadLimit.bottom = m_quadLimit.top = 0.0;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		m_quadLimit.Compose(pStroke->CalLimit());
	}
	return m_quadLimit;
}

void CNode::Mark(HWND hWnd)
{
	if(m_list.GetCount()<=0)
		return;

	CStroke *pStroke;
//	int n =  g_DAT.GetMarkCount(m_property.m_nLayer);
	int n = 1;

	// 在指定打标参数集模式下，层参数的打标次数无效。
	if (ENU_MARKSPECPARASET == m_nMarkMode) 
	{
		n = 1;
	}

    if (ENU_MARKSEL == m_nMarkMode) 
	{
		n = 1;
	}

	BOOL bStop = FALSE;
	for(int i= 0;i<n;i++)
	{
		POSITION pos=m_list.GetHeadPosition();

		while(pos!=NULL)
		{	
			pStroke=m_list.GetNext(pos);
			bStop = !(g_DAT.MarkStroke(*pStroke));
		}
		if(bStop)
			break;
	}
}

void CNode::SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsResMark)
{
	if(m_list.GetCount()<=0)
	{
		return;
	}

	CStroke *pStroke = NULL;
	BOOL bStop       = FALSE;
	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{	
		if(::WaitForSingleObject(hSelStop, 0)==WAIT_OBJECT_0)
		{
			return;
		}
		pStroke = m_list.GetNext(pos);
		bStop   = !(g_DAT.MarkStroke(*pStroke, nLayer, bIsResMark));
	}

	return;
}

void CNode::Scan(StrokeList *pList)
{
	//扫描所有线条数据的指针到FILL中进行填充处理
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{
		pStroke=m_list.GetNext(pos);
		pList->AddTail(pStroke);
	}
}

int CNode::GetType()
{
	return m_type;
}

void CNode::ChangeTo(CQuad quad)
{
	CalLimit();

	//计算变形情况
	double ratioX,ratioY;
	if(m_quadLimit.Width())
 		ratioX=quad.Width()/m_quadLimit.Width();
	else
 		ratioX=1;

	if(m_quadLimit.Height())
 		ratioY=quad.Height()/m_quadLimit.Height();
	else
 		ratioY=1;

	double moveX,moveY;
	CDot dotOld,dotNew;
	dotOld=m_quadLimit.GetBaseDot();
	dotNew=quad.GetBaseDot();
	
	moveX=dotNew.x-dotOld.x;
	moveY=dotNew.y-dotOld.y;

	Scale(dotOld,ratioX,ratioY);
	Move(moveX,moveY);
}

void CNode::SetProperty(int mode,CProperty pty, XDC *pDc)
{
	m_property.Set(mode,pty);

	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->SetProperty(mode,pty);
	}
	return ;
}

BOOL CNode::PreprocessMark(DWORD nStyle)
{
	return FALSE;
}

int CNode::GetBaseType()
{
	return m_type;
}

void CNode::MarkPreview(CDC *pDC, const int &nPreviewSpeed)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->MarkPreview(pDC, nPreviewSpeed);
	}
}

void CNode::InvertSelect()
{
	m_bSelect=!m_bSelect;
}

BOOL CNode::Include(CQuad quad)
{		
	return quad.Include(m_quadLimit);
}

void CNode::SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pStroke->SetLayColor(nLayer, color);
	}
	m_property.m_color = color;
	m_property.m_nLayer = nLayer;
}

void CNode::GetLayColor(int &nLayer,int &nFillLayer)
{
	nLayer=m_property.m_nLayer;
}

BOOL CNode::GetBarcode()
{
	return FALSE;
}

void CNode::DeleteFill()
{
}

void CNode::GetFillStyle(double &spaceHor,double &spaceVer,int &style,BOOL &bFillBack,double &dbBorderSpace)
{
}

void CNode::SetFillStyle(double spaceHor,double spaceVer,int style,BOOL bRedo,BOOL bFillBack,double dbBorderSpace)
{
}

void CNode::GetStrokeList(STROKELIST *pStrokeList)
{
	POSITION pos=m_list.GetHeadPosition();
    CStroke *pStroke;
	CStroke *pNewStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		pNewStroke=new CStroke;
		pNewStroke->CopyStroke(pStroke);
		pStrokeList->AddTail(pNewStroke);
	}
}

void CNode::Serialize_list(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CStroke *pStroke;
	CString str;
	
	if(bFlag)
	{
		fs<<"SL_B";
		POSITION pos=m_list.GetHeadPosition();
		while(pos!=NULL)
		{	
			pStroke=m_list.GetNext(pos);
			pStroke->Serialize(fp,bFlag);
		}
		fs<<"SL_E";
	}
	else
	{
		m_list.RemoveAll();
		while(fs>>str)
		{
			if(str == "S_B")
			{
				pStroke = new CStroke();
				pStroke->Serialize(fp,bFlag);
				m_list.AddTail(pStroke);
			}
			if(str == "SL_E")
				break;
		}
	}
}

void CNode::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{
		pStroke=m_list.GetNext(pos);
		pStroke->ExportPlt(file,bUnExportFill,bScall100);
	}
	return ;
}

void CNode::AddRect(CWnd *pWnd)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		pStroke->AddRect(pWnd);
	}
}

BOOL CNode::Click(CPoint &point,CWnd *pWnd)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		if(pStroke->Click(point,pWnd))
			return TRUE;
	}
	return FALSE;
}

void CNode::DelRect(CWnd *pWnd)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		pStroke->DelRect(pWnd);
	}
}

BOOL CNode::IsOnDot(CPoint &point, CWnd *pWnd)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		if(pStroke->IsOnDot(point,pWnd))
			return TRUE;
	}
	return FALSE;
}


BOOL CNode::HasDot(CQuad &quad)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		if(pStroke->HasDot(quad))
			return TRUE;
	}
	return FALSE;
}

void CNode::SetFlag(BOOL bflag)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		pStroke->SetFlag(bflag);
	}
}

BOOL CNode::IsEmpty()
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		if(!pStroke->IsEmpty())
			return FALSE;
	}
	return TRUE;
}

void CNode::SetStrokeList(StrokeList *pStrokeList)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		pStroke->SetParent((LPVOID*)this);
		if (pStroke)
		{
			pStrokeList->AddTail(pStroke);
		}
	}

	return;
}

CNode *CNode::ReDefineCopy()
{
	return NULL;
}

void CNode::AddToPlt(CNode *pNodeplt)
{
	return;
}

int CNode::GetObjID()
{
	return m_nObjID;
}

void CNode::SetObjID(int nObjID)
{
	m_nObjID = nObjID;
	return;
}

int CNode::GetObjType()
{
	return m_nObjID;
}

void CNode::SetObjType(int nObjType)
{
	m_nObjType = nObjType;
	return;
}

void CNode::LockLayer(int nLayer)
{
	m_property.m_bLockFlag = TRUE;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
			if ((pStroke->m_property).m_nLayer == nLayer)
			{
				pStroke->SetLockFlag(TRUE);
			}
		}
	}

	return;
}

void CNode::UnLockLayer(int nLayer)
{
	m_property.m_bLockFlag = FALSE;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
			if ((pStroke->m_property).m_nLayer == nLayer)
			{
				pStroke->SetLockFlag(FALSE);
			}
		}
	}

	return;
}

void CNode::FilterPoint(double dbPrecis, double dbAngle)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
			pStroke->FilterPoint(dbPrecis, dbAngle);
		}
	}

	return;
}

BOOL CNode::GetNodeFirstDot(CDot &dt)
{
	int nCount = m_list.GetCount();
	if(nCount <= 0)
	{
		return FALSE;
	}

	CStroke *pStroke = NULL;
	POSITION pos=m_list.GetHeadPosition();
	while(pos != NULL)
	{
		pStroke = m_list.GetNext(pos);
		if (pStroke != NULL)
		{
			if (pStroke->m_dotArray.GetSize() > 0)
			{
				dt = pStroke->m_dotArray.GetAt(0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CNode::GetNodeLastDot(CDot &dt)
{
	int nCount = m_list.GetCount();
	if(nCount <= 0)
	{
		return FALSE;
	}

	POSITION pos=m_list.GetTailPosition();
	if (pos != NULL)
	{
		CStroke *pStroke = m_list.GetNext(pos);
		if (pStroke != NULL)
		{
			int nDCount = pStroke->m_dotArray.GetSize();
			if (nDCount > 0)
			{
				dt = pStroke->m_dotArray.GetAt(nDCount - 1);
				return TRUE;
			}
		}
	}

	return FALSE;
}

CStroke* CNode::GetGoLineStroke()
{
	if (NODE_LINE != m_type)
	{
		return NULL;
	}
	
	POSITION pos    = m_list.GetHeadPosition();
	POSITION prev = pos;
    CStroke *pStroke = NULL;
	while(pos!=NULL)
	{	
		prev = pos;
		pStroke=m_list.GetNext(pos);
		if (pStroke) 
		{
			m_list.RemoveAt(prev);
			return pStroke;
		}
	}
	return NULL;
}

double CNode::GetLength()
{
	double total = 0.0;
	POSITION pos=m_list.GetHeadPosition();
    CStroke *pStroke = NULL;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		if (pStroke) 
		{
			total += pStroke->GetLength();
		}
	}	
	return total;
}


void CNode::SetMarkMode(int nMarkMode)
{
	m_nMarkMode = nMarkMode;
}

void CNode::DrawExtern(XDC *pDC)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{
		pStroke=m_list.GetNext(pos);
		pStroke->DrawExtern(pDC);
	}
}

int CNode::GetStrokeNum(int nFocusType)
{
	int nNum=0;
	CLay* play = g_DAT.GetCurLay();
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos)
	{
		pStroke = m_list.GetNext(pos);
		if (pStroke->m_nFocusType == nFocusType)
			nNum++;		
	}
	return nNum;
}

void CNode::GetStrokeInQuad(CQuad &quad, StrokeList &strokeList)
{
	CStroke::GetStrokeInQuadFromStrokeList(m_list, strokeList, quad, m_property, m_nResinRect);
}


void CNode::SetResinRect(int nRect)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos)
	{
		pStroke = m_list.GetNext(pos);
		pStroke->m_nResinRect = nRect;
	}
	m_nResinRect = nRect;
}

void CNode::GetResinRect(int aResinRect[],int &nId)
{
	CStroke *pStroke = NULL;
	POSITION pos = m_list.GetHeadPosition();
	while(pos)
	{
		pStroke = m_list.GetNext(pos);
		
		int i = 0;
		for (; i < 10; i ++)
		{
			if (pStroke->m_nResinRect == aResinRect[i])
			{
				break;
			}
		}
		if (i == 10)
		{
			aResinRect[nId] = pStroke->m_nResinRect;
			nId++;
		}
	}
}


void CNode::JoinStroke(double dbMinDistance)
{
	POSITION pos1, pos2;
	CStroke *pStroke1, *pStroke2;
	bool bJoint = true;

	for (int i=0; i<m_list.GetSize(); )
	{
		pos1 = m_list.FindIndex(i);
		pStroke1 = m_list.GetAt(pos1);

		if (pStroke1->GetFirstDot()==pStroke1->GetLastDot())
		{
			i++;
			continue;
		}

		CDot dotA1 = pStroke1->GetFirstDot();
		CDot dotA2 = pStroke1->GetLastDot();

		bJoint = false;
		for (int j=i+1; j<m_list.GetSize(); j++)
		{
			pos2 = m_list.FindIndex(j);
			pStroke2 = m_list.GetAt(pos2);

			if (pStroke2->GetFirstDot()==pStroke2->GetLastDot())
				continue;

			CDot dotB1 = pStroke2->GetFirstDot();
			CDot dotB2 = pStroke2->GetLastDot();

			if (pStroke1->Distance(dotA1, dotB1) < dbMinDistance)
			{
				pStroke1->ConnectHeadToHead(pStroke2);
				delete pStroke2;
				m_list.RemoveAt(pos2);
				bJoint = true;
				break;
			}

			if (pStroke1->Distance(dotA1, dotB2) < dbMinDistance)
			{
				pStroke2->Append(*pStroke1);
				delete pStroke1;
				m_list.RemoveAt(pos1);	
				bJoint = true;
				break;
			}

			if (pStroke1->Distance(dotA2, dotB1) < dbMinDistance)
			{
				pStroke1->Append(*pStroke2);
				delete pStroke2;
				m_list.RemoveAt(pos2);
				bJoint = true;
				break;
			}

			if (pStroke2->Distance(dotA2, dotB2) < dbMinDistance)
			{
				pStroke1->ConnectTailToTail(pStroke2);
				delete pStroke2;
				m_list.RemoveAt(pos2);
				bJoint = true;
				break;
			}
		}

		if (!bJoint)
			i++;
	}	
}
