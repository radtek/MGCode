// NodePlt.cpp: implementation of the CNodePlt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodePlt.h"
#include "NodeLine.h"
#include "Tree.h"
#include "HLDoc.h"
#include "OptStrokeList.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern void GenImportFullName(CString&);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CNodePlt::CNodePlt()
{
	m_type=NODE_PLT;
}

CNodePlt::~CNodePlt()
{
}

CTree *CNodePlt::UnGroup()
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

//按图层分解导入的线条，每组相同图层的线条作为一个PLT对象。
CNodePlt *CNodePlt::UnGroupplt()
{
	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	int nLay= -1;
	BOOL bFirst = FALSE;
	CNodePlt *pNode=NULL;

	while(pos!=NULL)	{	
		prev = pos;
		pStroke=m_list.GetNext(pos);
		if(!bFirst){
			nLay = pStroke->m_property.m_nLayer;
			pNode=new CNodePlt();
			pNode->m_property.m_color = pStroke->m_property.m_color;
			pNode->m_property.m_nLayer = pStroke->m_property.m_nLayer;
		bFirst = TRUE;
		}
		if(pStroke->m_property.m_nLayer==nLay){
			pNode->m_list.AddTail(pStroke);
			m_list.RemoveAt(prev);
		}
	}
	return pNode;
}

BOOL CNodePlt::DeCode(CString file,BOOL bCenter,BOOL bBetter)  
{
//	GenImportFullName(file);
	m_pFile=new CFile;
	if(!m_pFile->Open(file,CFile::modeRead)){
		CString strError;
		strError.LoadString(IDS_ERROR_OPENFILE);
		AfxMessageBox(strError);
		return FALSE;
	}
	char cmd[3];
	m_dotCur.x=CENTER;
	m_dotCur.y=CENTER;
	m_pStroke=NULL;

	m_bPen=FALSE;
	m_bCircle=FALSE;
	m_bPenUp=TRUE;
	m_bCoorA=TRUE;

	for(;;)
	{
		ReadCommand(cmd);
		if(!_strnicmp(cmd,"PD",2))
		{
			if(m_bPenUp)
			{
				//开始一个新的STROKE
				CStroke *pStroke;
				pStroke=new CStroke(m_property);
				m_pStroke=pStroke;
				m_list.AddTail(m_pStroke);
				m_pStroke->Add(m_dotCur);
				//m_bStart=FALSE;
			}
			m_bPenUp=FALSE;
		}
		else if(!_strnicmp(cmd,"PU",2))
		{
			//结束一个STROKE
			m_bPenUp=TRUE;
		}
		else if(!_strnicmp(cmd,"PA",2))
		{
			//转换为绝对坐标
			m_bCoorA=TRUE;
		}
		else if(!_strnicmp(cmd,"PR",2))
		{
			//转换为相对坐标
			m_bCoorA=FALSE;
		}
		else if(!_strnicmp(cmd,"SP",2))
		{
			//换笔
			m_bPen=TRUE;
		
		}
		else if(!_strnicmp(cmd,"CI",2))
		{
			m_bCircle=TRUE;
		}
		else if(!_strnicmp(cmd,"00",2))
		{
			break;
		}
		
		//参数处理
		Process();
	}

	m_pFile->Close();
	delete m_pFile;
	CalLimit();

	//居中
	if (bCenter)
	{
		CDot dot=GetLimit().GetBaseDot();
		double moveX,moveY;
		moveX=CENTER-dot.x;
		moveY=CENTER-dot.y;
		Move(moveX,moveY);
	}

	//优化数据
	if (bBetter) 
	{
		COptStrokeList tmp;
	    tmp.Attach(&m_list,0.01);
	    tmp.FullOpt();
	    tmp.Detach();
	}

	CalLimit();
	return TRUE;
}

//取命令字，
//如到文件结束，则返回为"00"
void CNodePlt::ReadCommand(char *cmd)	
{	
	char ch1,ch2;
		
	for(;;)
	{
		do
		{
			if(!m_pFile->Read(&ch1,1))
			{
				cmd[0]='0';
				cmd[1]='0';
				cmd[2]='\0';
				return;
			}
		}while(!IsCharAlpha(ch1));

		if(!m_pFile->Read(&ch2,1))
		{
			cmd[0]='0';
			cmd[1]='0';
			cmd[2]='\0';
			return;
		}

		if(IsCharAlpha(ch2))
		{
			cmd[0]=ch1;
			cmd[1]=ch2;
			cmd[2]='\0';
			break;
		}
	}
}

//取参数值，遇到" ", ",",";"或字母结束(首空格略)
//返回值为
//到文件结束为无效	0
//读到" ",","		1
//读到";"或字母		2
//直接读到";"		3
int CNodePlt::ReadValue(double &Value)
{
	int i=0;
	int result=1;
	char ch;
	char buf[20];
	BOOL bFirst=TRUE;
	buf[19]='\0';

	for(;;)
	{
		if(!m_pFile->Read(&ch,1))
		{
			result=0;
			break;
		}

		if(bFirst)
		{
			bFirst=FALSE;
			if(ch==';')
			{
				result=3;
				break;
			}
		}

		if((ch>='0'&& ch<='9')|| ch=='.'||ch=='+'||ch=='-')
		{
			buf[i++]=ch;
		}
		else	//	n[0]!=' '&&	n[0]!=',' && n[0]!=';' && !IsCharAlpha(n[0]))
		{
			if(ch==';')
			{
				result=3;
			}
			else if(IsCharAlpha(ch))
			{
				m_pFile->Seek(-1,CFile::current);
				result=3;
			}
			
			buf[i]='\0';
			break;
		}
	}
	Value=atof(buf);
	return result;
}	


void CNodePlt::Process()
{
	double Value;
	int result;
	int count=0;

	for(;;)
	{
		result=ReadValue(Value);

		if(m_bPen)
		{
			m_bPen=FALSE;
			CreatePen((int)Value);
		}
		if(m_bCircle)
		{
			double dbRadius;
			dbRadius=Value/40;
			m_bCircle=FALSE;
			CreateCircle(m_dotCur,dbRadius);
		}
		if(count==(count/2)*2 )
		{
			if(result!=3)
			{

				if(m_bCoorA)
				{
					m_dotCur.x=Value/40.0;
				}
				else
				{
					m_dotCur.x+=Value/40.0;
				}
			}
		}
		else
		{
			if(m_bCoorA)
			{
				m_dotCur.y=Value/40.0;
			}
			else
			{
				m_dotCur.y+=Value/40.0;
			}

			//根据实际情况加入到STROKE
			if(!m_bPenUp)
			{
				m_pStroke->Add(m_dotCur);
			}
		}

		count++;

		if(result==2||result==3)
		{
			return;
		}
	}
}

void CNodePlt::CreatePen(int pen)
{
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = 
             (CMDIChildWnd *) pFrame->GetActiveFrame();
	CHLDoc *pDoc = (CHLDoc *) pChild->GetActiveDocument();
	
	CLayer ly=pDoc->GetLayer(pen);
	m_property=ly.GetProperty();
}

void CNodePlt::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;

	if (bFlag)
	{
		//写入头信息
		fs<<"NPLT_B";
		m_property.Serialize(fp,bFlag);
		Serialize_list(fp,bFlag);
		//写入尾信息
		fs<<"NPLT_E";
	}
	else
	{
		while(fs>>str)
		{
			if(str.Left(5) == "PRO_B")
			{
				m_property.Serialize(fp,bFlag);
				continue;
			}
			if(str.Left(4) == "SL_B")
			{
				Serialize_list(fp,bFlag);
				continue;
			}
			if(str.Left(6) == "NPLT_E")
				break;
		}
	}
}

void CNodePlt::CreateCircle(CDot dotCenter, double dbRadius)
{
	//根据弦长来计算圆的信息
	double dbLen = 0.01;		//以后可设置

	double dbAngle;				//弦长半径对应的角度
	dbAngle = (asin((dbLen/2)/dbRadius))*2;

	double dbCurAngle;
	int nCount = (int)(2*PAI/dbAngle+0.5);
	CDot dot;

	dbAngle = (2*PAI/nCount);

	CStroke *pStroke=new CStroke(m_property);

	for(int i=0;i<nCount;i++)
	{
		dbCurAngle = i*dbAngle;
		dot.x = dbRadius*cos(dbCurAngle);
		dot.y = dbRadius*sin(dbCurAngle);
		pStroke->Add(dot);
	}
	dot.x = dbRadius*cos((double)0);
	dot.y = dbRadius*sin((double)0);
	pStroke->Add(dot);

	pStroke->Move(dotCenter.x,dotCenter.y);

	m_list.AddTail(pStroke);
}

//DEL void CNodePlt::GetDot(CPoint &point,myNode*pHead, CWnd *pWnd)
//DEL {
//DEL 	POSITION pos;
//DEL 	CStroke *pStroke;
//DEL 	int nCount;
//DEL 	CHLView *pView=(CHLView*)pWnd;
//DEL 	double dbdata=3;
//DEL 	pView->ActualLen(dbdata);
//DEL 	CDot dot;
//DEL 	int j;
//DEL 	CDot dot1,dot2;
//DEL 	CQuad quad;
//DEL 	CDot dotTemp;
//DEL 	dotTemp.x=point.x;
//DEL 	dotTemp.y=point.y;
//DEL 	pView->LogicToPhysic(dotTemp.x,dotTemp.y);
//DEL 	quad.top=dotTemp.y+dbdata;
//DEL 	quad.bottom=dotTemp.y-dbdata;
//DEL 	quad.left=dotTemp.x-dbdata;
//DEL 	quad.right=dotTemp.x+dbdata;
//DEL 	pos=m_list.GetHeadPosition();
//DEL 	BOOL bFlag;
//DEL 	int t=-1;
//DEL 	int z=0;
//DEL 	while(pos)
//DEL 	{
//DEL 		t++;
//DEL 		pStroke=m_list.GetNext(pos);
//DEL 		nCount=pStroke->m_dotArray.GetSize();
//DEL 		bFlag=FALSE;
//DEL 		for(int i=0;i<nCount;i++)
//DEL 		{
//DEL 			dot=pStroke->m_dotArray.GetAt(i);
//DEL 			if(quad.left<=dot.x && dot.x<quad.right && quad.bottom<=dot.y && dot.y<=quad.top)
//DEL 			{
//DEL 				bFlag=TRUE;
//DEL 				pHead->m_pmyStroke=new myStroke[1];
//DEL 				j=(i==0)? 0:i-1;
//DEL 				dot1=pStroke->m_dotArray.GetAt(j);
//DEL 				j=(i==nCount-1)? nCount-1:i+1;
//DEL 				dot2=pStroke->m_dotArray.GetAt(j);
//DEL 				if(i==0)
//DEL 					(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,1,0,t);
//DEL 				else if(i==nCount-1)
//DEL 					(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,0,1,t);
//DEL 				else 
//DEL 					(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,0,0,t);
//DEL 				(pHead->m_pmyStroke[z]).m_nCount=nCount;
//DEL 				g_bReady=FALSE;
//DEL 				pHead->m_nCount=1;
//DEL 				break;
//DEL 			}
//DEL 		}
//DEL 		if(bFlag)
//DEL 			break;
//DEL 	}
//DEL }

//DEL void CNodePlt::ReplaceDot(myNode *pHead)
//DEL {
//DEL 	int i,j,k,nNum;
//DEL 	j=-1;
//DEL 	POSITION pos=m_list.GetHeadPosition();
//DEL 	if(pos==NULL)
//DEL 		return;
//DEL 	CStroke *pStroke;
//DEL 	DotSelectList *pList;
//DEL 	CDot dot;
//DEL 	for(i=0;i<pHead->m_nCount;i++)
//DEL 	{
//DEL 		k=(pHead->m_pmyStroke[i]).m_nRow;
//DEL 		while(j!=k)
//DEL 		{
//DEL 			pStroke=m_list.GetNext(pos);
//DEL 			j++;
//DEL 		}
//DEL 		pList=(pHead->m_pmyStroke[i]).pHead;
//DEL 		while(pList)
//DEL 		{
//DEL 			dot=pList->m_Node;
//DEL 			nNum=pList->m_nNum;
//DEL 			pStroke->m_dotArray.SetAt(nNum,dot);
//DEL 			pList=pList->m_pNext;
//DEL 		}
//DEL 	}
//DEL }

//DEL void CNodePlt::GetDot(CQuad &quad, myNode *pHead,CWnd *pWnd)
//DEL {
//DEL 	POSITION pos=m_list.GetHeadPosition();
//DEL 	CStroke *pStroke;
//DEL 	int nCount;
//DEL 	CHLView *pView=(CHLView*)pWnd;
//DEL 	double dbdata=3;
//DEL 	pView->ActualLen(dbdata);
//DEL 	CDot dot;
//DEL 	int j;
//DEL 	CDot dot1,dot2;
//DEL 	pHead->m_nCount=0;
//DEL 	while(pos)
//DEL 	{
//DEL 		pStroke=m_list.GetNext(pos);
//DEL 		nCount=pStroke->m_dotArray.GetSize();
//DEL 		for(int i=0;i<nCount;i++)
//DEL 		{
//DEL 			dot=pStroke->m_dotArray.GetAt(i);
//DEL 			if(quad.left<=dot.x && dot.x<=quad.right && quad.bottom<=dot.y && dot.y<=quad.top)
//DEL 			{
//DEL 				pHead->m_nCount++;
//DEL 				break;
//DEL 			}
//DEL 		}
//DEL 	}
//DEL 	pHead->m_pmyStroke=new myStroke[pHead->m_nCount];
//DEL 	pos=m_list.GetHeadPosition();
//DEL 	BOOL bFlag;
//DEL 	int t=-1;
//DEL 	int z=0;
//DEL 	while(pos)
//DEL 	{
//DEL 		t++;
//DEL 		pStroke=m_list.GetNext(pos);
//DEL 		nCount=pStroke->m_dotArray.GetSize();
//DEL 		bFlag=FALSE;
//DEL 		for(int i=0;i<nCount;i++)
//DEL 		{
//DEL 			dot=pStroke->m_dotArray.GetAt(i);
//DEL 			if(quad.left<=dot.x && dot.x<=quad.right && quad.bottom<=dot.y && dot.y<=quad.top)
//DEL 			{
//DEL 				bFlag=TRUE;
//DEL 				j=(i==0)? 0:i-1;
//DEL 				dot1=pStroke->m_dotArray.GetAt(j);
//DEL 				j=(i==nCount-1)? nCount-1:i+1;
//DEL 				dot2=pStroke->m_dotArray.GetAt(j);
//DEL 				if(i==0)
//DEL 					(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,1,0,t);
//DEL 				else if(i==nCount-1)
//DEL 					(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,0,1,t);
//DEL 				else 
//DEL 					(pHead->m_pmyStroke[z]).AddSelectDot(dot,dot1,dot2,i,0,0,t);
//DEL 					(pHead->m_pmyStroke[z]).m_nCount=nCount;
//DEL 			}
//DEL 		}
//DEL 		if(bFlag)
//DEL 			z++;
//DEL 	}
//DEL }