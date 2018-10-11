// Tree.cpp: implementation of the CTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HL.h"
#include "Tree.h"
#include "NodeCodebar.h"
#include "NodeText.h"
#include "HLView.h"
#include "Nodeplt.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//IMPLEMENT_SERIAL(CTree,CObject,2000)
CTree::CTree()
{
	m_pRoot = NULL;
	m_pRoot=new CNode;
	m_Fill.dbAngle1 = 0;
	m_Fill.dbAngle2 = 90;
	m_Fill.dbSpace1 = 0;
	m_Fill.dbSpace2 = 0;
	m_Fill.dbDistance = 0;
	m_Fill.nStyle = 0;
	m_Fill.dbPtp = 0.1;
	m_Fill.dbCircleLen = 0.1;

	m_bIsCurSortTree = FALSE;
	m_nGroupNum = -1;
	m_bMainSelect = FALSE;

	m_bIsUpdate      = FALSE;
	g_OBJID += 1;
	m_ID = g_OBJID;

	m_nResinRect = 0;

	m_bUnGroupOnlyRoot = FALSE;
}

CTree::CTree(CNode *pNode)
{
	m_pRoot = NULL;
	m_pRoot=pNode;
	m_Fill.dbAngle1 = 0;
	m_Fill.dbAngle2 = 90;
	m_Fill.dbSpace1 = 0;
	m_Fill.dbSpace2 = 0;
	m_Fill.dbDistance = 0;
	m_Fill.nStyle = 0;
	m_Fill.dbPtp = 0.1;
	m_Fill.dbCircleLen = 0.1;

	m_bIsCurSortTree = FALSE;
	m_nGroupNum = -1;
	m_bMainSelect = FALSE;

	m_bIsUpdate      = FALSE;
	g_OBJID += 1;
	m_ID     = g_OBJID;
	m_nResinRect = pNode->m_nResinRect;

//	m_property = pNode->m_property;
}

CTree::~CTree()
{
	Delete();
}

//nStyle:用来确定是COPY还是CHAIN赋值
//nStyle=0x00和原用法一样
//nStyle=0x01用于CHAIN的赋值，由于赋值时不用对TEXT进行复制
//nStyle=0x02用于CHAIN的赋值，用于Do与undo
//CTree::CTree(const CTree *src, UINT nStyle)
//{
//	m_pRoot  = CopyFrom(src->m_pRoot,nStyle);
//	m_bSelect= src->m_bSelect;
//	m_Fill   = src->m_Fill;
//	m_property=src->m_property;
//	m_bIsCurSortTree = src->m_bIsCurSortTree;
//	m_nGroupNum = src->m_nGroupNum;
//	m_bMainSelect = src->m_bMainSelect;
//	m_MarkSet = src->m_MarkSet;
//
//	CalLimit();
//	Fill();
//}

CTree::CTree(const CTree *src, UINT nStyle, BOOL bFill)
{
	m_pRoot = NULL;
	m_pRoot  = CopyFrom(src->m_pRoot,nStyle);
	m_bSelect= src->m_bSelect;
	m_Fill   = src->m_Fill;
	m_property=src->m_property;
	m_bIsCurSortTree = src->m_bIsCurSortTree;
	m_nGroupNum = src->m_nGroupNum;
	m_bMainSelect = src->m_bMainSelect;
	m_MarkSet = src->m_MarkSet;
	m_ID            = src->m_ID;
	m_nResinRect = src->m_nResinRect;

#ifdef _DOUBLE_HEAD_MARK
	m_DoubleOutParaIns = src->m_DoubleOutParaIns;
#endif

	if (0x01 == nStyle) 
	{
		m_bIsUpdate = src->m_bIsUpdate;
	}
	else
	{
		g_OBJID += 1;
		m_ID     = g_OBJID;
	}
	
	CalLimit();
	if (bFill)
	{
		Fill();
	}
}

void CTree::Add(CNode *pNode)
{
	if(m_pRoot->m_pLeft==NULL)
	{
		m_pRoot->m_pLeft=pNode;
	}
	else
	{
		CNode *pNext=m_pRoot->m_pLeft;
		while(pNext->m_pRight)
			pNext=pNext->m_pRight;
		pNext->m_pRight=pNode;

	}
	pNode->m_nResinRect = m_nResinRect;
}

void CTree::Draw(XDC *pDC)
{
	Draw(m_pRoot,pDC);

	//显示填充线
	CStroke *pStroke;
	POSITION pos;
	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_listFill.GetNext(pos);
		pStroke->Draw(pDC);
	}
}

void CTree::Draw(CNode *pNode,XDC *pDC)
{
	if(pNode)
	{
		pNode->Draw(pDC);
		Draw(pNode->m_pLeft,pDC);
		Draw(pNode->m_pRight,pDC);
	}	
}

void CTree::CalLimit(CNode *pNode)
{
	if(pNode)
	{		
//		if(!pNode->m_pLeft)
//		{
//			m_quadLimit.Compose(pNode->CalLimit());
//			pNode->SetParent(this);	//设置节点所在树
//		}
		m_quadLimit.Compose(pNode->CalLimit());
		CalLimit(pNode->m_pLeft);
		CalLimit(pNode->m_pRight);
	}	
}

CQuad CTree::CalLimit()
{
	m_quadLimit.Initial();

	CalLimit(m_pRoot);
	//m_pRoot->m_quadLimit=m_quadLimit;
	return m_quadLimit;
}

void CTree::Scale(CNode *pNode,CDot dot, double ratioX, double ratioY)
{
	if(pNode)
	{
		pNode->Scale(dot,ratioX,ratioY);
		Scale(pNode->m_pLeft,dot,ratioX,ratioY);
		Scale(pNode->m_pRight,dot,ratioX,ratioY);
	}	
}

void CTree::Scale(CDot dot, double ratioX, double ratioY)
{
	if(IsGroup())	{
		SetSelect(m_pRoot,TRUE);
	}
	Scale(m_pRoot,dot,ratioX,ratioY);
	CalLimit();
	Fill();
}

void CTree::Move(double moveX, double moveY)
{
	// 如果被锁定，就不移动
//	if (GetLockFlg())
//	{
//		return;
//	}

	if(IsGroup())	
		SetSelect(m_pRoot,TRUE);
//	SetSelect(TRUE);  //将该句去掉可实现条码中的文字和条码的单独移动变换
	Move(m_pRoot,moveX,moveY);
	CalLimit();
	int n  =GetType();

	//填充线移动
	CStroke *pStroke;
	POSITION pos;
	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_listFill.GetNext(pos);
		pStroke->Move(moveX,moveY);
	}
}

void CTree::Move(CNode *pNode,double moveX,double moveY)
{
	if(pNode)
	{
		pNode->Move(moveX,moveY);
		Move(pNode->m_pLeft,moveX,moveY);
		Move(pNode->m_pRight,moveX,moveY);
	}	
}

void CTree::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;
	
	if(bFlag)
	{
		fs<<"T_B";
		Save(m_pRoot,fp,FALSE);
		fs<<"T_E";
		fs<<"T_FILLB";
		fs<<100;
		fs<<m_Fill.dbAngle1;
		fs<<101;
		fs<<m_Fill.dbAngle2;
		fs<<102;
		fs<<m_Fill.dbDistance;
		fs<<103;
		fs<<m_Fill.dbSpace1;
		fs<<104;
		fs<<m_Fill.dbSpace2;
		fs<<105;
		fs<<m_Fill.nStyle;
#ifdef _DOUBLE_HEAD_MARK
		fs<<106;
		fs<<m_DoubleOutParaIns.bDWFlag;
		fs<<107;
		fs<<m_DoubleOutParaIns.dbLeftOutAliX;
		fs<<108;
		fs<<m_DoubleOutParaIns.dbLeftOutAliY;
		fs<<109;
		fs<<m_DoubleOutParaIns.dbLineWidth;
		fs<<110;
		fs<<m_DoubleOutParaIns.dbRightOutAliX;
		fs<<111;
		fs<<m_DoubleOutParaIns.dbRightOutAliY;
#endif
		fs<<112;
		fs<<m_Fill.dbCircleLen;
		fs<<113;
		fs<<m_MarkSet.nMarkCount;
		fs<<(int)OBJ_MAX_MARKCOUNT;
		for (int i = 0; i < OBJ_MAX_MARKCOUNT; i++) 
		{
			fs<<m_MarkSet.stSub[i].nSubCnt;
			fs<<m_MarkSet.stSub[i].nLayer;
			fs<<m_MarkSet.stSub[i].bIsMarkFram;
			fs<<m_MarkSet.stSub[i].bIsMarkFill;
			fs<<m_MarkSet.stSub[i].bIsFillFirst;
		}
		fs<<114;
		fs<<m_nResinRect;
		fs<<"T_FILLE";
		m_property.Serialize(fp,TRUE);
	}
	else
	{
		delete m_pRoot;
		m_pRoot = Load(fp);
		fs>>str;

		fs>>str;
		if(str == "T_FILLB")
		{
			while(fs>>str)
			{
				switch(atoi(str)) 
				{
				case 100:
					fs>>m_Fill.dbAngle1;
					break;
				case 101:
					fs>>m_Fill.dbAngle2;
					break;
				case 102:
					fs>>m_Fill.dbDistance;
					break;
				case 103:
					fs>>m_Fill.dbSpace1;
					break;
				case 104:
					fs>>m_Fill.dbSpace2;
					break;
				case 105:
					fs>>m_Fill.nStyle;
					break;
#ifdef _DOUBLE_HEAD_MARK
				case 106:
					fs>>m_DoubleOutParaIns.bDWFlag;
					break;
				case 107:
					fs>>m_DoubleOutParaIns.dbLeftOutAliX;
					break;
				case 108:
					fs>>m_DoubleOutParaIns.dbLeftOutAliY;
					break;
				case 109:
					fs>>m_DoubleOutParaIns.dbLineWidth;
					break;
				case 110:
					fs>>m_DoubleOutParaIns.dbRightOutAliX;
					break;
				case 111:
					fs>>m_DoubleOutParaIns.dbRightOutAliY;
					break;
#endif
				case 112:
					fs>>m_Fill.dbCircleLen;
					break;
				case 113:
					{
						fs>>m_MarkSet.nMarkCount;
						int nCount = 0;
						fs>>nCount;
						if (nCount > OBJ_MAX_MARKCOUNT)
						{
							nCount = OBJ_MAX_MARKCOUNT;
						}
						for (int i=0; i<nCount; i++)
						{
							fs>>m_MarkSet.stSub[i].nSubCnt;
							fs>>m_MarkSet.stSub[i].nLayer;
							fs>>m_MarkSet.stSub[i].bIsMarkFram;
							fs>>m_MarkSet.stSub[i].bIsMarkFill;
							fs>>m_MarkSet.stSub[i].bIsFillFirst;
						}
					}
					break;
				case 114:
					fs>>m_nResinRect;
					break;
				case 0:
					if(str == "T_FILLE")
						goto TREE_SERI_END;
				}
			}
		}
TREE_SERI_END:
		fs>>str;
		if (str == "PRO_B")
		{
			m_property.Serialize(fp,FALSE);
		}
		CalLimit();
		Fill();
		SetSelect(FALSE);
	}
	
	CStroke *pStroke;
	POSITION pos;
	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)		
	{	
		pStroke=m_listFill.GetNext(pos);
		pStroke->SetProperty(PTY_FILL,m_property);
	}
}

void CTree::Save(CNode *pNode,FILE* fp,BOOL bLeft)
{
	//本标准采用文本方式进行数据录入
	CMySerialize fs(fp);
	
	if(pNode)
	{
		if (bLeft)
		{
			fs<<"G_B";
		}

		if(pNode->GetType() != NODE_BASE)
		{
			//处理本结点类信息
			pNode->Serialize(fp,TRUE);
		}
		Save(pNode->m_pLeft,fp,TRUE);
		Save(pNode->m_pRight,fp,FALSE);

		if (bLeft)
		{
			fs<<"G_E";
		}
	}
	else
	{
		fs<<"NULL";			//告知本节点为空
	}
}

CNode * CTree::Load(FILE* fp)
{
	CMySerialize fs(fp);
	CString str;
	CNode* pNode = NULL;

	while(fs>>str)
	{
		if(str == "NULL")
		{
			break;
		}
		else if(str == "T_E")
		{
			break;
		}
		else if(str == "G_B")
		{
			//创建空结点
			pNode = new CNode();
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}
		else if(str == "NDIB_B")
		{
			//位图节点
			pNode = CNode::CreateFromType(NODE_BMP);
			pNode->Serialize(fp,FALSE);
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}
		else if(str == "NLIN_B")
		{
			pNode = CNode::CreateFromType(NODE_LINE);
			pNode->Serialize(fp,FALSE);
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}
		else if(str == "NTXT_B")
		{
			pNode = CNode::CreateFromType(NODE_TEXT);
			pNode->Serialize(fp,FALSE);
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}
		else if(str == "NEPS_B")
		{
			pNode = CNode::CreateFromType(NODE_ELLIPSE);
			pNode->Serialize(fp,FALSE);
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}
		else if(str == "NRCT_B")
		{
			pNode = CNode::CreateFromType(NODE_RECT);
			pNode->Serialize(fp,FALSE);
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}
		else if(str == "NPLT_B")
		{
			//PLT文件图档
			//节点内部处理到"NPLT_E"时结束
			pNode = CNode::CreateFromType(NODE_PLT);
			pNode->Serialize(fp,FALSE);
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}
		else if(str == "NBAR_B")
		{
			pNode = CNode::CreateFromType(NODE_CODEBAR);
			pNode->Serialize(fp,FALSE);
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}
		//为了兼容老的文本方式所以在此处进行了修改
		else if(str == "TXT_B")
		{
			CText* pText = new CText(0);
			pText->Serialize(fp,FALSE);
			g_MgrText->AddText(pText);
		}
		else if (str=="NDXF_B")
		{
			pNode = CNode::CreateFromType(NODE_DXF);
			pNode->Serialize(fp,FALSE);
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}

		else if (str=="NDOT_B")
		{
			pNode = CNode::CreateFromType(NODE_DOT);
			pNode->Serialize(fp,FALSE);
			pNode->m_pLeft = Load(fp);
			pNode->m_pRight = Load(fp);
			break;
		}
	}
	return pNode;
}

CQuad CTree::GetLimit()
{
	m_quadLimit=CalLimit();
	return m_quadLimit;
}

void CTree::Delete()
{
	Delete(m_pRoot);
	DeleteFill();
}

void CTree::Delete(CNode *pNode)
{
	if(pNode)
	{
		Delete(pNode->m_pLeft);
		Delete(pNode->m_pRight);
		if (pNode) 
		{
			delete pNode;
			pNode = NULL;
		}
	}

	return;
}

void CTree::Group(CTree *pTree)
{
	CNode *pNode=CopyFrom(pTree->m_pRoot);
	Add(pNode);
	SetSelect(TRUE);

#ifdef _DOUBLE_HEAD_MARK
	m_DoubleOutParaIns = pTree->m_DoubleOutParaIns;
#endif
}

BOOL CTree::IsGroup()
{
	if(m_pRoot->m_pLeft)
		return TRUE;
	else
	{
		if(m_pRoot->CanUnGroup())
			return TRUE;
		else
			return FALSE;
	}
}

BOOL CTree::CanGroup()
{
	if(m_pRoot->m_pLeft)
		return TRUE;
	else
	{
		if(m_pRoot->CanGroup())
			return TRUE;
		else
			return FALSE;
	}
}

CTree *CTree::UnGroup()
{
	if (m_bUnGroupOnlyRoot)	//拆解单个Node的线条,如CDxfNode
	{
		if (m_pRoot->GetList().GetCount()>=1)
		{
			CTree *pTree=m_pRoot->UnGroup();
			if(pTree)
			{
				pTree->CalLimit();
			}
			return pTree;
		}
		return NULL;
	}
	else	//针对CNode为根的组合结构
	{
		if (m_pRoot==NULL)
			return NULL;

		if (m_pRoot->m_pLeft)
		{
			//从一棵树中分解出第一个左节点
			CNode *pNode = m_pRoot->m_pLeft;
			m_pRoot->m_pLeft=pNode->m_pRight;
			pNode->m_pRight=NULL;
			CTree *pTree=new CTree(pNode);
			pTree->CalLimit();
			return pTree;
		}
		else
		{
			if (m_pRoot->GetType()==0)
				return NULL;

			CTree *pTree=new CTree(m_pRoot);
			pTree->CalLimit();

			m_pRoot = NULL;
			return pTree;
		}
	}

}


void CTree::SelMark(CNode *pNode, int nLayer, HWND hWnd, HANDLE hStop, BOOL bIsResMark)
{
	if(pNode)
	{	
		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
		{
			return;
		}
		
		pNode->SelMark(hWnd,  hStop, nLayer, bIsResMark);
		SelMark(pNode->m_pLeft, nLayer, hWnd, hStop, bIsResMark);
		SelMark(pNode->m_pRight, nLayer, hWnd, hStop, bIsResMark);
	}

	return;
}

void CTree::Mark(CNode *pNode,HWND hWnd)
{
	if(pNode)
	{	

		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0){
			return;
		}
		if (m_MarkSet.nMarkCount <= 0)
		{
			pNode->SetMarkMode(ENU_MARKNORMALL);
		}
		else
		{
			pNode->SetMarkMode(ENU_MARKSPECPARASET);
		}
		pNode->Mark(hWnd);
		Mark(pNode->m_pLeft,hWnd);
		Mark(pNode->m_pRight,hWnd);
	}
}

//
//树结构入口
//
void CTree::Mark(HWND hWnd)
{
	g_DAT.SetMarkLayNum(-1, FALSE);
#ifdef _DOUBLE_HEAD_MARK
	DOT6 off;
	off.x = (float)(m_DoubleOutParaIns.dbLeftOutAliX);
	off.y = (float)(m_DoubleOutParaIns.dbLeftOutAliY);
	off.z = (float)(m_DoubleOutParaIns.dbRightOutAliX);
	off.i = (float)(m_DoubleOutParaIns.dbRightOutAliY);
	g_DAT.SetMarkOffset(off);
#endif

	if (m_MarkSet.nMarkCount > 0)
	{
		MarkSpecParaSet(hWnd);
	}
    else
	{
		MarkNormal(hWnd);
	}

	return;
}

void CTree::MarkSpecParaSet(HWND hWnd)
{
	if (m_MarkSet.nMarkCount > OBJ_MAX_MARKCOUNT) 
	{
		m_MarkSet.nMarkCount = OBJ_MAX_MARKCOUNT;
	}
	else if (m_MarkSet.nMarkCount < 0) 
	{
		m_MarkSet.nMarkCount = 0;
	}

	for (int i = 0; i < m_MarkSet.nMarkCount; i++) 
	{
		SUBOBJMARKSET stSub = m_MarkSet.stSub[i];
		g_DAT.SetMarkLayNum(stSub.nLayer, stSub.bIsResMark);
		for (int j = 0; j < stSub.nSubCnt; j++)
		{
			if (stSub.bIsMarkFill && stSub.bIsMarkFram)
			{
				if (stSub.bIsFillFirst)
				{
					MarkFill(hWnd);
					Mark(m_pRoot,hWnd);
				}
				else
				{
					Mark(m_pRoot,hWnd);
					MarkFill(hWnd);
				}
			}
			else if (stSub.bIsMarkFill) 
			{
				MarkFill(hWnd);
			}
			else if (stSub.bIsMarkFram)
			{
				Mark(m_pRoot,hWnd);
			}
			else
			{
			}
			
		}// end for (int i = 0; i < m_MarkSet.nMarkCount; i++) 
	}// end for (int i = 0; i < m_MarkSet.nMarkCount; i++) 
	
	return;
}

void CTree::MarkNormal(HWND hWnd)
{
	if(!(m_Fill.nStyle&FILL_MORD))
	{
		Mark(m_pRoot,hWnd);
	}
	
	CStroke *pStroke;
	int nStrokeCount = m_listFill.GetCount();
	if(nStrokeCount>0)
	{
		//CLay *play = g_DAT.GetCurLay();
		int n  = g_DAT.GetMarkCount(m_property.m_nLayer);
		for(int i= 0;i<n;i++)
		{		
			POSITION pos=m_listFill.GetHeadPosition();
			//PARA para=play->GetPara(m_property.m_nLayer);
			while(pos!=NULL)	
			{
				pStroke=m_listFill.GetNext(pos);
				g_DAT.MarkStroke(*pStroke);
				if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0){
					break;
				}
			}
			if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0){
				break;
			}
		}
	}

	if(m_Fill.nStyle&FILL_MORD)
	{
		Mark(m_pRoot,hWnd);
	}

	return;
}

void CTree::MarkFill(HWND hWnd)
{
	CStroke *pStroke;
	int nStrokeCount = m_listFill.GetCount();
	if(nStrokeCount>0)
	{
		POSITION pos=m_listFill.GetHeadPosition();
		while(pos!=NULL)	
		{
			pStroke=m_listFill.GetNext(pos);
			g_DAT.MarkStroke(*pStroke);
			if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
			{
				break;
			}
		}
	}

	return;
}

void CTree::SelMarkFill(HWND hWnd, int nLayer, HANDLE hStop, BOOL bIsResMark)
{
	CStroke *pStroke;
	int nStrokeCount = m_listFill.GetCount();
	if(nStrokeCount>0)
	{
		POSITION pos=m_listFill.GetHeadPosition();
		while(pos!=NULL)	
		{
			pStroke=m_listFill.GetNext(pos);
			g_DAT.MarkStroke(*pStroke,nLayer, bIsResMark);
			if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
			{
				break;
			}
		}
	}

	return;
}

void CTree::SelMark(HWND hWnd, HANDLE hStop, SUBOBJMARKSET &stSub)
{
#ifdef _DOUBLE_HEAD_MARK
	DOT6 off;
	off.x = (float)(m_DoubleOutParaIns.dbLeftOutAliX);
	off.y = (float)(m_DoubleOutParaIns.dbLeftOutAliY);
	off.z = (float)(m_DoubleOutParaIns.dbRightOutAliX);
	off.i = (float)(m_DoubleOutParaIns.dbRightOutAliY);
	g_DAT.SetMarkOffset(off);
#endif

	for (int j = 0; j < stSub.nSubCnt; j++)
	{
		if (stSub.bIsMarkFill && stSub.bIsMarkFram)
		{
			if (stSub.bIsFillFirst)
			{
				SelMarkFill(hWnd, stSub.nLayer, hStop, stSub.bIsResMark);
				SelMark(m_pRoot,stSub.nLayer, hWnd, hStop, stSub.bIsResMark);
			}
			else
			{
				SelMark(m_pRoot,stSub.nLayer, hWnd, hStop, stSub.bIsResMark);
				SelMarkFill(hWnd, stSub.nLayer, hStop, stSub.bIsResMark);
			}
		}
		else if (stSub.bIsMarkFill) 
		{
			SelMarkFill(hWnd, stSub.nLayer, hStop, stSub.bIsResMark);
		}
		else if (stSub.bIsMarkFram)
		{
			SelMark(m_pRoot,stSub.nLayer, hWnd, hStop, stSub.bIsResMark);
		}
		else
		{
		}
		
	}// end for (int i = 0; i < m_MarkSet.nMarkCount; i++) 
	
	return;
}

void CTree::Rotate(CDot dot, double angle)
{
	Rotate(m_pRoot,dot,angle);
	CalLimit();
	Fill();
}

void CTree::Rotate(CNode *pNode,CDot dot, double angle)
{
	if(pNode)
	{
		if (GetLockFlg())
		{
			return;
		}
		
		pNode->Rotate(dot,angle);
		Rotate(pNode->m_pLeft,dot,angle);
		Rotate(pNode->m_pRight,dot,angle);
	}
}

CNode * CTree::CopyFrom(CNode *pNode,UINT nStyle)
{
	if(pNode)
	{
		CNode *tmp=pNode->Copy(nStyle);
		tmp->m_pLeft=CopyFrom(pNode->m_pLeft,nStyle);
		tmp->m_pRight=CopyFrom(pNode->m_pRight,nStyle);
		return tmp;
	}
	else 
	{
		return NULL;
	}
	
	return NULL;
}

void CTree::Fill()
{
	DeleteFill();
	
	if((fabs(m_Fill.dbSpace1) > 0.001) || (fabs(m_Fill.dbSpace2) > 0.001))
	{
		//有填充数据时处理此操作
		StrokeList listData;
		Scan(m_pRoot, &listData);
		if(fabs(m_Fill.dbSpace1) > 0.001)
		{
			CFill f1(m_Fill.dbSpace1,m_Fill.dbDistance,m_Fill.dbAngle1
				,m_Fill.nStyle,m_property,m_Fill.dbPtp, m_Fill.dbCircleLen, m_Fill.dbHeadKillLen, m_Fill.dbTailKillLen);
			f1.Proc(&listData,m_listFill);
		}
		
		if( (fabs(m_Fill.dbSpace2) > 0.001) && (m_Fill.nStyle&FILL_CROSS) )
		{
			StrokeList listFill;
			CFill f2(m_Fill.dbSpace2,m_Fill.dbDistance,m_Fill.dbAngle2
				,m_Fill.nStyle,m_property,m_Fill.dbPtp, m_Fill.dbCircleLen, m_Fill.dbHeadKillLen, m_Fill.dbTailKillLen);
			f2.Proc(&listData,listFill);
			POSITION prev    = NULL;
			POSITION pos     = NULL;
			CStroke *pStroke = NULL;
			pos=listFill.GetHeadPosition();
			while(pos!=NULL)
			{
				prev=pos;
				pStroke=listFill.GetNext(pos);
				m_listFill.AddTail(pStroke);
			}
			listFill.RemoveAll();
		}
	}

	return;
}

void CTree::Scan(CNode *pNode,StrokeList* listTemp)
{
	if(pNode)
	{
		pNode->Scan(listTemp); 
		Scan(pNode->m_pLeft,listTemp); 
		Scan(pNode->m_pRight,listTemp);
	}
}

void CTree::GetFillStyle(FILLDATA& fill)
{
	fill = m_Fill;
}

void CTree::SetFillStyle(FILLDATA& fill)
{
	m_Fill = fill;
	Fill();
	m_quadLimit=CalLimit();
}

void CTree::DeleteFill()
{
	POSITION prev;
	POSITION pos;
	CStroke *pStroke = NULL;

	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)
	{	
		prev=pos;
		pStroke=m_listFill.GetNext(pos);
		m_listFill.RemoveAt(prev);
		if (pStroke)
		{
			delete pStroke;
		    pStroke = NULL;
		}
	}// end while

	return;
}

void CTree::Drag(CDot dot, double ratioX, double ratioY, double moveX, double moveY)
{
	// 如果被锁定，就不移动
	if (GetLockFlg())
	{
		return;
	}

	Drag(m_pRoot,dot,ratioX,ratioY,moveX,moveY);
	Fill();
}

void  CTree::Drag(CNode *pNode,CDot dot, double ratioX, double ratioY, double moveX, double moveY)
{
	if(pNode)
	{
		pNode->Drag(dot,ratioX,ratioY,moveX,moveY);
		Drag(pNode->m_pLeft,dot,ratioX,ratioY,moveX,moveY);
		Drag(pNode->m_pRight,dot,ratioX,ratioY,moveX,moveY);
	}
}

void CTree::SetProperty(int mode,CProperty pty)
{	
	switch(mode)
	{
	case PTY_OUTLINE:
		if(IsSelect())
			SetProperty(m_pRoot,mode,pty);
		break;
	case PTY_FILL:
		{
			if(IsSelect())
			{
				//填充线属性设置
				CStroke *pStroke;
				POSITION pos;
				pos=m_listFill.GetHeadPosition();
				while(pos!=NULL)
				{	
					pStroke=m_listFill.GetNext(pos);
					pStroke->SetProperty(mode,pty);
				}
				m_property.Set(mode,pty);
			}
		}
		break;
	default:
		SetProperty(m_pRoot,mode,pty);

		//填充线属性设置
		CStroke *pStroke;
		POSITION pos;
		pos=m_listFill.GetHeadPosition();
		while(pos!=NULL)
		{	
			pStroke=m_listFill.GetNext(pos);
			pStroke->SetProperty(mode,pty);
		}
		m_property.Set(mode,pty);
		break;
	}
}

void CTree::SetProperty(CNode *pNode,int mode,CProperty pty)
{
	if(pNode)
	{
		pNode->SetProperty(mode,pty);
		SetProperty(pNode->m_pLeft,mode,pty);
		SetProperty(pNode->m_pRight,mode,pty);
	}
}

void CTree::SetGroup(BOOL bGroup)
{
	SetGroup(m_pRoot, bGroup);

}

void CTree::SetGroup(CNode *pNode,BOOL bGroup)
{
	if(pNode)
	{
		SetGroup(pNode->m_pLeft,bGroup);
		SetGroup(pNode->m_pRight,bGroup);
	}
}

BOOL CTree::PreprocessMark(CNode *pNode,DWORD nStyle)
{
	BOOL bFlag = FALSE;
	if(pNode)
	{
		if(pNode->PreprocessMark(nStyle))
			bFlag = TRUE;
		if(PreprocessMark(pNode->m_pLeft,nStyle))
			bFlag = TRUE;
		if(PreprocessMark(pNode->m_pRight,nStyle))
			bFlag = TRUE;
	}
	
	return bFlag;
}

BOOL CTree::PreprocessMark(DWORD nStyle)
{
// 	if(nStyle)
	{
		m_nMarkUnit=m_listFill.GetCount();
		BOOL bUpdate = PreprocessMark(m_pRoot,nStyle);
		
		if( bUpdate ){
			Fill();
		}
	}
	return TRUE;
}

void CTree::MarkPreview(CDC *pDC, const int &nPreviewSpeed)
{
	if(!(m_Fill.nStyle&FILL_MORD)){
		MarkPreview(m_pRoot,pDC, nPreviewSpeed);
	}
	//显示填充线
	CStroke *pStroke;
	POSITION pos;
	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_listFill.GetNext(pos);
		pStroke->MarkPreview(pDC, nPreviewSpeed);
	}
	if(m_Fill.nStyle&FILL_MORD){
		MarkPreview(m_pRoot,pDC, nPreviewSpeed);
	}
}

void CTree::MarkPreview(CNode *pNode,CDC *pDC, const int &nPreviewSpeed)
{
	if(pNode)
	{
		pNode->MarkPreview(pDC, nPreviewSpeed);
		MarkPreview(pNode->m_pLeft,pDC, nPreviewSpeed);
		MarkPreview(pNode->m_pRight,pDC, nPreviewSpeed);
	}
}

int CTree::GetType()
{
	return m_pRoot->GetType();
}

void CTree::SetSelect(CNode *pNode,BOOL bFlag)
{
	if(pNode)
	{
		pNode->SetSelect(bFlag);
		SetSelect(pNode->m_pLeft,bFlag);
		SetSelect(pNode->m_pRight,bFlag);
	}
}

void CTree::SetSelect(BOOL bFlag)
{
	SetSelect(m_pRoot,bFlag);
	m_bSelect = bFlag;
}

BOOL CTree::IsSelect()
{
	if(IsGroup())	
		return m_bSelect;
	return IsSelect(m_pRoot);
}

BOOL CTree::IsSelect(CNode *pNode)
{
	if(pNode)
	{
		return pNode->IsSelect();
		return IsSelect(pNode->m_pLeft);
		return IsSelect(pNode->m_pRight);
	}
	return FALSE;
}

void CTree::InvertSelect(CNode *pNode)
{
	if(pNode)
	{
		pNode->InvertSelect();
		InvertSelect(pNode->m_pLeft);
		InvertSelect(pNode->m_pRight);
	}
}

void CTree::InvertSelect()
{
	InvertSelect(m_pRoot);
	m_bSelect = !m_bSelect;
}

BOOL CTree::Include(CNode *pNode,CQuad quad)
{
	if(pNode)
	{
		return pNode->Include(quad);
		 Include(pNode->m_pLeft,quad);
		 Include(pNode->m_pRight,quad);
	}
	return FALSE;
}

BOOL CTree::Include(CQuad quad)
{		
	BOOL bInclude = FALSE;
 	bInclude = Include(m_pRoot,quad);
	if(bInclude&&IsGroup()){	
		SetSelect(m_pRoot,TRUE);
	}
	return bInclude;
}

void CTree::SetLayColor(CNode *pNode,int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{
	if(pNode)
	{
	pNode->SetLayColor(nLayer, color, nLayerFill, colorFill);
	SetLayColor(pNode->m_pLeft,nLayer, color, nLayerFill, colorFill);
	SetLayColor(pNode->m_pRight,nLayer, color, nLayerFill, colorFill);
	}
}


void CTree::SetLayGroupColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{

	SetLayColor(m_pRoot,nLayer,color,nLayerFill,colorFill);

	m_property.m_color  = color;
	m_property.m_nLayer = nLayer;

	CStroke *pStroke    = NULL;
	POSITION pos        = NULL;
	pos = m_listFill.GetHeadPosition();
	while(pos != NULL)
	{
		pStroke = m_listFill.GetNext(pos);
		pStroke->SetLayColor(nLayerFill,colorFill);
	}

}

void CTree::GetLayColor(CNode *pNode,int &nLayer,int &nFillLayer)
{
	if(pNode)
	{
	pNode->GetLayColor(nLayer, nFillLayer);
	GetLayColor(pNode->m_pLeft,nLayer, nFillLayer);
	GetLayColor(pNode->m_pRight,nLayer, nFillLayer);
	}
}

void CTree::GetLayColor(int &nLayer,int &nFillLayer)
{
	nFillLayer=m_property.m_nLayer;
	GetLayColor(m_pRoot,nLayer,nFillLayer);
}

void CTree::SetLayColor(int nLayer,int nFillLayer)
{
	CLay* pLay = g_DAT.GetCurLay();
	
	CProperty layProperty ;
	CProperty FillProperty;
	if (nLayer > -1)
	{
		layProperty  = pLay->GetLayer(nLayer).GetProperty();
		if(layProperty.m_nLayer == -1)
		{
			return;
		}
		SetLayColor(m_pRoot,nLayer,layProperty.m_color,nFillLayer,FillProperty.m_color);
	}// end if (nLayer != -1)

	if (nFillLayer > -1) 
	{
		FillProperty = pLay->GetLayer(nFillLayer).GetProperty();
		if(FillProperty.m_nLayer == -1)
		{
			return;
		}
		m_property.m_color  = FillProperty.m_color;
		m_property.m_nLayer = nFillLayer;

		CStroke *pStroke    = NULL;
		POSITION pos        = NULL;
		pos = m_listFill.GetHeadPosition();
		while(pos != NULL)
		{
			pStroke = m_listFill.GetNext(pos);
			pStroke->SetLayColor(nFillLayer, m_property.m_color);
		}
	}// end if (nFillLayer != -1) 

	return;
}

void CTree::SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor)
{
	SetLayColor(m_pRoot, nObjIndex, nObjColor, nFillIndex, nFillColor);

	m_property.m_color  = nFillColor;
	m_property.m_nLayer = nFillIndex;
	CStroke *pStroke    = NULL;
	POSITION pos        = NULL;
	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_listFill.GetNext(pos);
		pStroke->SetLayColor(nFillIndex, nFillColor);
	}

	return;
}

BOOL CTree::GetBarcode()
{
	return (m_pRoot->GetType() == NODE_CODEBAR);
}

BOOL CTree::CheckBarCode(CUIntArray &arrayTextID)
{
	if(m_pRoot->GetType() == NODE_CODEBAR){
		CNodeCodebar* pNode = (CNodeCodebar*)m_pRoot;
		UINT nID = pNode->m_nTextID;
		arrayTextID.Add(nID);
		
		return TRUE;
	}
	if(m_pRoot->GetType() == NODE_TEXT_DSF){
		CNodeText* pNode = (CNodeText*)m_pRoot;
		if(pNode->IsBarText()){
			int nCount = pNode->m_ArrayTextID.GetSize();
			UINT nID;
			for(int i=0;i<nCount;i++){
				nID = pNode->m_ArrayTextID.GetAt(i);
				arrayTextID.Add(nID);
			}
		}
		
		return TRUE;
	}
	return FALSE;	
}

CQuad CTree::GetReFurbishQuad()
{
	return m_pRoot->GetLimit(TRUE);
}

void CTree::GetStrokeList(STROKELIST *pStrokeList)
{
	GetStrokeList(m_pRoot,pStrokeList);
}

void CTree::GetStrokeList(CNode *pNode, STROKELIST *pStrokeList)
{
	if(pNode)
	{
		pNode->GetStrokeList(pStrokeList);
		GetStrokeList(pNode->m_pLeft,pStrokeList);
		GetStrokeList(pNode->m_pRight,pStrokeList);
	}
}

CQuad CTree::GetMarkRange()//zg add 2005/08/26 for v4.3
{
	CQuad quad;
	quad.SetEmpty();
	if(m_pRoot->m_pLeft==NULL&&m_pRoot->m_pRight==NULL)
      quad=m_pRoot->GetMarkRange();
	else
	{//对于组合对象，应该遍历组合对象中的所有对象，获取其打标范围，以防止该组合对象中有圆弧排布的文字对象
		if(m_pRoot->m_pLeft!=NULL)
           GetMarkRange(m_pRoot->m_pLeft,quad);
		if(m_pRoot->m_pRight!=NULL)
	       GetMarkRange(m_pRoot->m_pRight,quad);
	}
	return quad;
}

void CTree::GetMarkRange(CNode *pNode, CQuad &quad)//zg add 2005/08/26 for v4.3
{
     ASSERT(pNode!=NULL);

	 if(pNode->GetType()!=NODE_BASE)  
	 	 quad.Union(pNode->GetMarkRange());

	 if(pNode->m_pLeft!=NULL)
		 GetMarkRange(pNode->m_pLeft,quad);
	 if(pNode->m_pRight!=NULL)
		 GetMarkRange(pNode->m_pRight,quad);
}

BOOL CTree::SelectBarCode(CUIntArray& arrayTextID)
{
	UINT nTextID;
	BOOL bSelect=FALSE;
	//对于条码的归属文本不能进行组合，所以在此处只要进行如下判定即可，不需全部扫描
	if(m_pRoot->GetType() == NODE_TEXT_DSF)
	{
		CNodeText* pNode = (CNodeText*)m_pRoot;
		if(pNode->IsBarText())
		{
			int nCount = pNode->m_ArrayTextID.GetSize();
			int nCountIn = arrayTextID.GetSize();
			bSelect = FALSE;
			for(int i=0;i<nCount;i++)
			{
				nTextID = pNode->m_ArrayTextID.GetAt(i);
				for(int j=0;j<nCountIn;j++)
				{
					if(nTextID == arrayTextID.GetAt(j))
					{
						bSelect = TRUE;
						break;
					}
				}
				if(bSelect)
					break;
			}
		}
	}
	if(m_pRoot->GetType() == NODE_CODEBAR)
	{
		CNodeCodebar* pNode = (CNodeCodebar*)m_pRoot;
		int nCountIn = arrayTextID.GetSize();
		bSelect = FALSE;
		for(int j=0;j<nCountIn;j++)
		{
			nTextID = arrayTextID.GetAt(j);
			if(nTextID == pNode->m_nTextID)
			{
				bSelect = TRUE;
				break;
			}
		}
	}
	return bSelect;
}

BOOL CTree::GetFirstMarkCurrent(double & dbCurrent)
{
	//dsf 2010.04.06
	/*CLay *play= g_DAT.GetCurLay();
	PARA para=play->GetPara(m_property.m_nLayer);
	dbCurrent=para.dbCurrent;*/
	return TRUE;
}

BOOL CTree::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	ExportPlt(m_pRoot,file,bUnExportFill, bScall100);
	if (!bUnExportFill)
	{
		//显示填充线
		CStroke *pStroke;
		POSITION pos;
		pos=m_listFill.GetHeadPosition();
		while(pos!=NULL)
		{	
			pStroke=m_listFill.GetNext(pos);
			pStroke->ExportPlt(file,bUnExportFill, bScall100);
		}
	}	
	return TRUE;
}

void CTree::ExportPlt(CNode *pNode,CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	if(pNode)
	{
		pNode->ExportPlt(file, bUnExportFill, bScall100);
		ExportPlt(pNode->m_pLeft,file, bUnExportFill, bScall100);
		ExportPlt(pNode->m_pRight,file, bUnExportFill, bScall100);
	}	
}

BOOL CTree::PtInTree(CPoint &point,CWnd *pWnd)
{
	CQuad quad=m_quadLimit;
	double x,y;
	x=y=5;
	CHLView *pView=(CHLView*)pWnd;
	pView->ActualLen(x);
	pView->ActualLen(y);
	quad.Inflate(x,y);
	return(quad.Include(point,pWnd));
}

BOOL CTree::Click(CPoint &point,CWnd *pWnd)//点选
{
	return Click(m_pRoot,point,pWnd);
}

BOOL CTree::Click(CNode *pNode,CPoint &point,CWnd *pWnd)
{
	CQuad quad=GetLimit();
	if(quad.Include(point,pWnd)==FALSE)
		return FALSE;
	if(pNode)
	{
		if(pNode->Click(point,pWnd))
			return TRUE;
		if(Click(pNode->m_pLeft,point,pWnd))
			return TRUE;
		if(Click(pNode->m_pRight,point,pWnd))
			return TRUE;
	}
	POSITION pos=m_listFill.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_listFill.GetNext(pos);
		if(pStroke->Click(point,pWnd))
			return TRUE;
	}
	return FALSE;
}

BOOL CTree::PtInTree(CDot &dot)
{
	CQuad quad=CalLimit();
	quad.Inflate(2,2);
	return (quad.left<=dot.x && dot.x<=quad.right && quad.bottom<=dot.y && dot.y<=quad.top);
}

void CTree::AddRect(CWnd *pWnd)
{
	AddRect(m_pRoot,pWnd);
	POSITION pos=m_listFill.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_listFill.GetNext(pos);
		pStroke->AddRect(pWnd);
	}
}

void CTree::AddRect(CNode *pNode,CWnd *pWnd)
{
	if(pNode)
	{
		pNode->AddRect(pWnd);
		AddRect(pNode->m_pLeft,pWnd);
		AddRect(pNode->m_pRight,pWnd);
	}
}

void CTree::DelRect(CWnd *pWnd)
{
	DelRect(m_pRoot,pWnd);
}


void CTree::DelRect(CNode *pNode, CWnd *pWnd)
{
	if(pNode)
	{
		pNode->DelRect(pWnd);
		DelRect(pNode->m_pLeft,pWnd);
		DelRect(pNode->m_pRight,pWnd);
	}	
}

BOOL CTree::IsEmpty()
{
	return IsEmpty(m_pRoot);
}

BOOL CTree::IsEmpty(CNode *pNode)
{
	if(pNode)
	{
		if(!pNode->IsEmpty())
			return FALSE;
		if(!IsEmpty(pNode->m_pLeft))
			return FALSE;
		if(!IsEmpty(pNode->m_pRight))
			return FALSE;
	}
	return TRUE;
}


void CTree::SetStrokeList(StrokeList *pStrokeList,BOOL bFlag)
{
	// 如果层被锁定，则直接返回
	if (GetLockFlg()) 
	{
		return;
	}

 	SetStrokeList(m_pRoot,pStrokeList,bFlag);
}

void CTree::SetStrokeList(CNode *pNode, StrokeList *pStrokeList,BOOL bFlag)
{
	if(pNode)
	{
		if(!bFlag){
			if(pNode->GetType()==NODE_CODEBAR || pNode->GetType()==NODE_TEXT_DSF)
				return;
		}
		pNode->SetStrokeList(pStrokeList);
		SetStrokeList(pNode->m_pLeft,pStrokeList, bFlag);
		SetStrokeList(pNode->m_pRight,pStrokeList, bFlag);
	}
}

void CTree::CaptureObject(CQuad &quad,StrokeList *pStrokeList)
{
	CQuad quadTest=CalLimit();
	if(quad.Include(quadTest))
	{
		SetStrokeList(pStrokeList);
		SetSelect(TRUE);
	}
}

void CTree::AddFillToStrokeList(StrokeList *pStrokeList)
{
	POSITION pos=m_listFill.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_listFill.GetNext(pos);
		pStroke->SetStyle(TRUE);
		pStrokeList->AddTail(pStroke);
	}
}

UINT CTree::CanConvert()
{
	UINT nRet = 0;
	int nCount = m_listFill.GetCount();
	if (nCount > 0)
	{
		nRet = 3;
	}

	if(IsCode(m_pRoot))
	{
		nRet = 1;
		return nRet;
	}
	if(IsText(m_pRoot))
	{
		nRet = 2;
		return nRet;
	}
	
	return nRet;
}

BOOL CTree::IsCode(CNode *pNode)
{
	if(pNode)
	{
		if(pNode->GetType()==NODE_CODEBAR)
			return TRUE;
		if(IsCode(pNode->m_pLeft))
			return TRUE;
		if (IsCode(pNode->m_pRight))
			return TRUE;
	}
	return FALSE;
}

BOOL CTree::IsText(CNode *pNode)
{
	if(pNode)
	{
		if(pNode->GetType()==NODE_TEXT_DSF)
			return TRUE;
		if(IsText(pNode->m_pLeft))
			return TRUE;
		if (IsText(pNode->m_pRight))
			return TRUE;
	}
	return FALSE;
}

CTree* CTree::ConvertPLT(UINT nRes)
{
	CTree    *pTree = NULL;
	CNodePlt *pNode = NULL;
	switch(nRes)
	{
	case 1://条码
		{
			pNode=new CNodePlt;
			if (NULL == pNode)
			{
				return NULL;
			}
			AddToPlt(m_pRoot,pNode);
			break;
		}
	case 2://文本
		{
			pNode=new CNodePlt;
			if (NULL == pNode)
			{
				return NULL;
			}
			AddToPlt(m_pRoot,pNode);
			break;
		}
	default:
		{
			break;
		}
	}
	
	if (pNode)
	{
		pTree=new CTree(pNode);
	}
	return pTree;
}

CTree* CTree::ConvertFillToPLT()
{
	CTree    *pTree = NULL;
	CNodePlt *pNode = NULL;
	int nCount = m_listFill.GetCount();
	if (nCount > 0)
	{
		pNode=new CNodePlt;
		if (NULL == pNode)
		{
			return NULL;
		}

		POSITION pos = m_listFill.GetHeadPosition();
		int nStrkCnt = m_listFill.GetCount();
		CStroke *pStroke    = m_listFill.GetAt(pos);
		CStroke *pStrokeNew = NULL;
		
		if (nStrkCnt > 0) 
		{
			pNode->SetProperty(PTY_INIT, pStroke->m_property);
		}

		while(pos)
		{
			pStroke=m_listFill.GetNext(pos);
			pStrokeNew=pStroke->Copy();
			pNode->GetList().AddTail(pStrokeNew);
		}

	}// if (nCount > 0)
	
	if (pNode)
	{
		pTree=new CTree(pNode);
	}

	return pTree;
}

CTree* CTree::ConvertFramToPLT(UINT nResult)
{
	int nCount = m_listFill.GetCount();
	if (nCount <= 0)
	{
		return NULL;
	}

//	if (3 != nResult) 
//	{
//		return NULL;
//	}

	CTree    *pTree = NULL;
	CNodePlt *pNode = NULL;

	pNode=new CNodePlt;
	if (NULL == pNode)
	{
		return NULL;
	}

	AddFillToPlt(m_pRoot,pNode);

	CProperty propty;
	pNode->SetProperty(PTY_INIT, propty);
	
	if (pNode)
	{
		int nStrkCnt = pNode->GetList().GetCount();
		if (nStrkCnt <= 0)
		{
			delete pNode;
			pNode = NULL;

			return pTree;
		}
		else
		{
			pTree=new CTree(pNode);
		}
	}

	return pTree;
}

CTree*  CTree::ExtendObj(double dbSpace, BOOL bOpt)
{
	if(m_pRoot->GetType() == NODE_CODEBAR)
	{
		return NULL;
	}

	CNodePlt *pNode=new CNodePlt;
	if (NULL == pNode) 
	{
		return NULL;
	}
	
	StrokeList listData;
	listData.RemoveAll();
	StrokeList RetlistData;
	RetlistData.RemoveAll();
	
	Scan(m_pRoot,&listData);
	COffsetReg OffReg(dbSpace);
//	OffReg.Proc(&listData, RetlistData);
	OffReg.DsfProc(&listData, RetlistData, bOpt);

	POSITION pos= RetlistData.GetHeadPosition();
	CStroke *pStroke    = NULL;
	while(pos)
	{
		pStroke    = RetlistData.GetNext(pos);
		pNode->GetList().AddTail(pStroke);
	}

	CTree *pTree=new CTree(pNode);
	if (NULL == pTree)
	{
		return NULL;
	}

	pTree->CalLimit();

	return pTree;
}


void CTree::AddToPlt(CNode *pNode, CNode *pNodeplt)
{
	if(pNode)
	{
		pNode->AddToPlt(pNodeplt);
		AddToPlt(pNode->m_pLeft,pNodeplt);
		AddToPlt(pNode->m_pRight,pNodeplt);
	}
}

void CTree::AddFillToPlt(CNode *pNode, CNode *pNodeplt)
{
	if(pNode)
	{
		StrokeList &stroklist = pNode->GetList();
		POSITION pos= stroklist.GetHeadPosition();
		while(pos)
		{
			CStroke * pStroke=stroklist.GetNext(pos);
			CStroke * pStrokeNew=pStroke->Copy();
			pNodeplt->GetList().AddTail(pStrokeNew);
		}
		AddFillToPlt(pNode->m_pLeft,pNodeplt);
		AddFillToPlt(pNode->m_pRight,pNodeplt);
	}
}

void CTree::LockLayer(int nLayer)
{
	m_property.m_bLockFlag = TRUE;
	LockLayer(m_pRoot, nLayer);

	int nCount = m_listFill.GetCount();
	if (nCount > 0)
	{
		POSITION pos= m_listFill.GetHeadPosition();
		CStroke *pStroke    = NULL;
		while(pos)
		{
			pStroke=m_listFill.GetNext(pos);
			if ((pStroke->m_property).m_nLayer == nLayer)
			{
				pStroke->SetLockFlag(TRUE);
			}
		}
	}// if (nCount > 0)

	return;
}

void CTree::LockLayer(CNode *pNode, int nLayer)
{
	if(pNode)
	{
        pNode->LockLayer(nLayer);
		LockLayer(pNode->m_pLeft,nLayer);
		LockLayer(pNode->m_pRight,nLayer);
	}

	return;
}

void CTree::UnLockLayer(int nLayer)
{
	m_property.m_bLockFlag = FALSE;

	UnLockLayer(m_pRoot, nLayer);

	int nCount = m_listFill.GetCount();
	if (nCount > 0)
	{
		POSITION pos= m_listFill.GetHeadPosition();
		CStroke *pStroke    = NULL;
		while(pos)
		{
			pStroke=m_listFill.GetNext(pos);
			if ((pStroke->m_property).m_nLayer == nLayer)
			{
				pStroke->SetLockFlag(FALSE);
			}
		}
	}// if (nCount > 0)

	return;
}

void CTree::UnLockLayer(CNode *pNode, int nLayer)
{
	if(pNode)
	{
        pNode->UnLockLayer(nLayer);
		UnLockLayer(pNode->m_pLeft,nLayer);
		UnLockLayer(pNode->m_pRight,nLayer);
	}

	return;
}

void CTree::FilterPoint(double dbPrecis, double dbAngle)
{
	FilterPoint(m_pRoot, dbPrecis, dbAngle);
}

void CTree::FilterPoint(CNode *pNode, double dbPrecis, double dbAngle)
{
	if(pNode)
	{
        pNode->FilterPoint(dbPrecis, dbAngle);
		FilterPoint(pNode->m_pLeft,dbPrecis, dbAngle);
		FilterPoint(pNode->m_pRight,dbPrecis, dbAngle);
	}

	return;
}

BOOL CTree::GetFirstDot(CDot &dt)
{
	return GetTreeFirstdot(m_pRoot,dt);
}

BOOL CTree::GetTreeFirstdot(CNode *pNode, CDot &dt)
{
	if(pNode)
	{
		if(pNode->GetNodeFirstDot(dt))
		{
			return TRUE;
		}
		if (GetTreeFirstdot(pNode->m_pLeft, dt))
		{
			return TRUE;
		}
		
		if (GetTreeFirstdot(pNode->m_pRight, dt))
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CTree::GetLastDot(CDot &dt)
{
	return GetNodeLastDot(m_pRoot,dt);
}

BOOL CTree::GetNodeLastDot(CNode *pNode, CDot &dt)
{
	if(pNode)
	{
		pNode->GetNodeLastDot(dt);
		GetNodeLastDot(pNode->m_pLeft, dt);
		GetNodeLastDot(pNode->m_pRight, dt);
	}

	return FALSE;
}


void CTree::SetSortStatus(BOOL bIsSort)
{
	m_bIsCurSortTree = bIsSort;
	SetSortStatus(m_pRoot, bIsSort);
}

void CTree::SetSortStatus(CNode *pNode, BOOL bIsSort)
{
	if(pNode)
	{
        pNode->m_bIsCurSortTree = bIsSort;
		SetSortStatus(pNode->m_pLeft,bIsSort);
		SetSortStatus(pNode->m_pRight,bIsSort);
	}

	return;
}

void CTree::SetGroupNum(int nNum)
{
	m_nGroupNum = nNum;
}

int CTree::GetGroupNum()
{
	return m_nGroupNum;
}

CStroke* CTree::GetGoLineStroke()
{
	return GetGoLineStroke(m_pRoot);
}

CStroke* CTree::GetGoLineStroke(CNode *pNode)
{
	if(pNode)
	{
		CStroke *pStroke = pNode->GetGoLineStroke();
		if (pStroke)
		{
			return pStroke;
		}

		pStroke = GetGoLineStroke(pNode->m_pLeft);
		if (pStroke)
		{
			return pStroke;
		}

		pStroke =  GetGoLineStroke(pNode->m_pRight);
		if (pStroke)
		{
			return pStroke;
		}
	}

	return NULL;
}

void CTree::AddToOneNode(void* pNode)
{
	if (NULL == pNode)
	{
		return;
	}

	CNodePlt* pNodePlt = (CNodePlt*)pNode;

	if(IsCode(m_pRoot) || IsText(m_pRoot))
	{
		AddToPlt(m_pRoot,pNodePlt);
	}
	else
	{
		AddFillToPlt(m_pRoot,pNodePlt);
	}

	return;
}

void CTree::ComToOneNode(void *pNode)
{
	if (NULL == pNode)
	{
		return;
	}

	CNodePlt *pNodePlt = (CNodePlt*)pNode;

	AddToOneNode(pNodePlt);

	int nCount = m_listFill.GetCount();
	if (nCount > 0)
	{
		POSITION pos= m_listFill.GetHeadPosition();
		CStroke *pStroke    = NULL;
		CStroke *pStrokeNew = NULL;
		while(pos)
		{
			pStroke=m_listFill.GetNext(pos);
			pStrokeNew=pStroke->Copy();
			pNodePlt->GetList().AddTail(pStrokeNew);
		}
	}// if (nCount > 0)
	
	return;
}

BOOL CTree::GetLockFlg()
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (pFWnd==NULL) 
		return FALSE;

	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return FALSE;
	}

	CHLDoc *pDoc = pView->GetDocument();
	if (NULL == pDoc)
	{
		return FALSE;
	}

	int nLayer    = 0;
    int nFillLayer=0;
	GetLayColor(m_pRoot, nLayer, nFillLayer);

	return pDoc->GetLockFlg(nLayer);
}

BOOL CTree::IsMainSelect()
{
	return m_bMainSelect;
}
void CTree::SetMainSelect(BOOL bSelect)
{
	m_bMainSelect = bSelect;
}

double CTree::GetLength(CNode* pNode)
{
	double total = 0;
	if (pNode != NULL)
	{
		total += pNode->GetLength();
		total += GetLength(pNode->m_pLeft);
		total += GetLength(pNode->m_pRight);
	}
	return total;
}

double CTree::GetLength()
{
	double total = 0;
	total += GetLength(m_pRoot);

	POSITION pos= m_listFill.GetHeadPosition();
	CStroke *pStroke    = NULL;
	while(pos)
	{
		pStroke=m_listFill.GetNext(pos);
		total += pStroke->GetLength();
	}
	
	return total;
}

BOOL CTree::IsHaveTxtOrCodeBar()
{
	return IsHaveTxtOrCodeBar(m_pRoot);
}


void CTree::CodeBarAddNo(CNode *pNode,CString &strNo)
{
	if(pNode)
	{
		if (NODE_CODEBAR == pNode->m_type)
		{
			CNodeCodebar * pNodeCodebar = (CNodeCodebar *)pNode;
			
			CText* pText;
			pText = g_MgrText->GetAt(pNodeCodebar->m_nTextID);
			if (pText)
			{
				pText->m_strModelNo = strNo;
				CString str;
				str.Format(_T("%d"),_ttoi(strNo) + 1);
				strNo = str;
			}
		}
		TxtAddNo(pNode->m_pLeft,strNo);
		TxtAddNo(pNode->m_pRight,strNo);
	}

}


void CTree::TxtAddNo(CNode *pNode,CString &strNo)
{
	if(pNode)
	{
		if (NODE_TEXT_DSF == pNode->m_type )
		{
			CNodeText * pNodeText = (CNodeText *)pNode;
			int nCount = pNodeText->m_ArrayTextID.GetSize();
			UINT nTextID;
			CText* pText;
			for(int i=0;i<nCount;i++)
			{
				nTextID = pNodeText->m_ArrayTextID.GetAt(i);
				pText = g_MgrText->GetAt(nTextID);
				if (pText)
				{
					pText->m_strModelNo = strNo;
					CString str;
					str.Format(_T("%d"),_ttoi(strNo) + 1);
					strNo = str;
				}
			}
		}
		TxtAddNo(pNode->m_pLeft,strNo);
		TxtAddNo(pNode->m_pRight,strNo);
	}
}


BOOL CTree::IsHaveTxt(CNode *pNode)
{
	if(pNode)
	{
		if (NODE_TEXT_DSF == pNode->m_type)
		{
			return TRUE;
		}
		IsHaveTxt(pNode->m_pLeft);
		IsHaveTxt(pNode->m_pRight);
	}

	return FALSE;
}

BOOL CTree::IsHaveCodeBar(CNode *pNode)
{
	if(pNode)
	{
		if (NODE_CODEBAR == pNode->m_type)
		{
			return TRUE;
		}
		IsHaveCodeBar(pNode->m_pLeft);
		IsHaveCodeBar(pNode->m_pRight);
	}

	return FALSE;
}

BOOL CTree::GetBarcodeLength(CNode *pNode,SeriQuDot *QuadDot,int &nCodeId)
{
	if(pNode)
	{
		if (NODE_CODEBAR == m_pRoot->m_type)
		{
			QuadDot[nCodeId].nNum = nCodeId;
			QuadDot[nCodeId].quad = m_pRoot->GetLimit();
			QuadDot[nCodeId].dot = QuadDot[nCodeId].quad.GetBaseDot();
			nCodeId ++;
			return TRUE;
		}
		GetBarcodeLength(pNode->m_pLeft,QuadDot,nCodeId);
		GetBarcodeLength(pNode->m_pRight,QuadDot,nCodeId);
	}
	return TRUE;
}



BOOL CTree::GetTextBaseDot(CNode *pNode,CDot *QuadDot,int &nTextId)
{
	if(pNode)
	{
		QuadDot[nTextId++] = pNode->GetLimit().GetBaseDot();
		GetTextBaseDot(pNode->m_pLeft,QuadDot,nTextId);
		GetTextBaseDot(pNode->m_pRight,QuadDot,nTextId);
	}
	return TRUE;
}

BOOL CTree::MoveOldBaseDot(CNode *pNode,CDot *QuadDot,int &nTextId)
{
	if(pNode)
	{

		CDot dot = pNode->GetLimit().GetBaseDot();
		pNode->Move(QuadDot[nTextId].x-dot.x,QuadDot[nTextId].y-dot.y);
		nTextId++;

		MoveOldBaseDot(pNode->m_pLeft,QuadDot,nTextId);
		MoveOldBaseDot(pNode->m_pRight,QuadDot,nTextId);
	}
	return TRUE;
}


BOOL CTree::SetBarcodeLength(CNode *pNode,SeriQuDot *QuadDot,int &nCodeId)
{
	if(pNode)
	{
		if (NODE_CODEBAR == m_pRoot->m_type)
		{
			CQuad quad1 = m_pRoot->GetLimit();
			CDot dot1 = quad1.GetBaseDot();
			double dbScaleX = QuadDot[nCodeId].quad.Width()/quad1.Width();
			double dbScaleY = QuadDot[nCodeId].quad.Height()/quad1.Height();
			if (dbScaleX<0.001)
				dbScaleX=1.0;
			if(dbScaleY<0.001)
				dbScaleY=1.0;
			m_pRoot->Move(QuadDot[nCodeId].dot.x-dot1.x,QuadDot[nCodeId].dot.y-dot1.y);
			m_pRoot->Scale(QuadDot[nCodeId].dot,dbScaleX,dbScaleY);
			nCodeId ++;
			return TRUE;
		}
		SetBarcodeLength(pNode->m_pLeft,QuadDot,nCodeId);
		SetBarcodeLength(pNode->m_pRight,QuadDot,nCodeId);
	}
	return TRUE;
}

BOOL CTree::IsHaveTxtOrCodeBar(CNode *pNode)
{
	if(pNode)
	{
		if ((NODE_TEXT_DSF == pNode->m_type) || (NODE_CODEBAR == pNode->m_type))
		{
			return TRUE;
		}
		IsHaveTxtOrCodeBar(pNode->m_pLeft);
		IsHaveTxtOrCodeBar(pNode->m_pRight);
	}

	return FALSE;
}

BOOL CTree::IsBmpNode()
{
	return IsBmpNode(m_pRoot);
}

BOOL CTree::IsBmpNode(CNode *pNode)
{
	if(pNode)
	{
		if (NODE_BMP == pNode->m_type)
		{
			return TRUE;
		}
		IsBmpNode(pNode->m_pLeft);
		IsBmpNode(pNode->m_pRight);
	}

	return FALSE;
}


void CTree::DrawExtern(XDC *pDC)
{
	DrawExtern(m_pRoot,pDC);
}

void CTree::DrawExtern(CNode *pNode, XDC *pDC)
{
	if(pNode)
	{
		pNode->DrawExtern(pDC);
		DrawExtern(pNode->m_pLeft,pDC);
		DrawExtern(pNode->m_pRight,pDC);
	}	
}

CQuad& CTree::RetLimit()
{
	return m_quadLimit;
}

void CTree::JoinStroke(double dbMinDistance)
{
	JoinStroke(m_pRoot,dbMinDistance);
}

void CTree::JoinStroke(CNode *pNode, double dbMinDistance)
{
	if ( pNode == NULL )
		return;

	pNode->JoinStroke(dbMinDistance);
	JoinStroke(pNode->m_pLeft,dbMinDistance);
	JoinStroke(pNode->m_pRight,dbMinDistance);
}

void CTree::GetStrokeInQuad(CQuad &quad, StrokeList &strokeList)
{	
	BOOL bFirstFill = (m_Fill.nStyle&FILL_MORD)>0?1:0;
	if (!bFirstFill)
		GetStrokeInQuad(m_pRoot, quad, strokeList);

	if (!m_listFill.IsEmpty())
	{
		CStroke::GetStrokeInQuadFromStrokeList(m_listFill, strokeList, quad, m_property, m_nResinRect);
	}

	if (bFirstFill)
		GetStrokeInQuad(m_pRoot, quad, strokeList);
}

void CTree::GetStrokeInQuad(CNode *pNode, CQuad &quad, StrokeList &strokeList)
{
	if (pNode)
	{
		if(pNode->GetType() == NODE_ELLIPSE)
		{
			pNode->SetResinRect(m_nResinRect);
		}


		pNode->GetStrokeInQuad(quad, strokeList);
		GetStrokeInQuad(pNode->m_pLeft, quad, strokeList);
		GetStrokeInQuad(pNode->m_pRight, quad, strokeList);
	}
}

int CTree::GetStrokeNum(int nFocusType)
{
	return GetStrokeNum(m_pRoot, nFocusType);
}

int CTree::GetStrokeNum(CNode *pNode, int nFocusType)
{
	int nNum=0;
	if(pNode)
	{	
		nNum+=pNode->GetStrokeNum(nFocusType);

		if (pNode->m_pLeft)
			nNum+=GetStrokeNum(pNode->m_pLeft, nFocusType);

		if (pNode->m_pRight)
			nNum+=GetStrokeNum(pNode->m_pRight, nFocusType);
	}
	return nNum;
}


void CTree::SetResinRect(CNode *pNode,int nRect)
{
	if(pNode)
	{
		pNode->SetResinRect(nRect);
		SetResinRect(pNode->m_pLeft,nRect);
		SetResinRect(pNode->m_pRight,nRect);
	}

	POSITION pos= m_listFill.GetHeadPosition();
	CStroke *pStroke    = NULL;
	while(pos)
	{
		pStroke=m_listFill.GetNext(pos);
		pStroke->m_nResinRect = nRect;
	}
}

void CTree::GetResinRect(CNode *pNode,int aResinRect[],int &nId)
{
	if(pNode)
	{
		if(pNode->GetType() == NODE_ELLIPSE)
		{
			pNode->SetResinRect(m_nResinRect);
		}
		pNode->GetResinRect(aResinRect,nId);
		GetResinRect(pNode->m_pLeft,aResinRect,nId);
		GetResinRect(pNode->m_pRight,aResinRect,nId);
	}
}



void CTree::SetStrokeLayFreq(CNode *pNode,double dbQFreq)
{

	if(pNode)
	{
		//pNode->SetResinRect(nRect);
		CStroke *pStroke = NULL;
		POSITION pos = pNode->m_list.GetHeadPosition();
		while(pos)
		{
			pStroke = pNode->m_list.GetNext(pos);
			pStroke->m_dbQFre = dbQFreq;
		}
		SetStrokeLayFreq(pNode->m_pLeft, dbQFreq);
		SetStrokeLayFreq(pNode->m_pRight, dbQFreq);
	}
	POSITION pos= m_listFill.GetHeadPosition();
	CStroke *pStroke    = NULL;
	while(pos)
	{
		pStroke = pNode->m_list.GetNext(pos);
		pStroke->m_dbQFre = dbQFreq;
	}
}