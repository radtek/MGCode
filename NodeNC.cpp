// NodeNC.cpp: implementation of the CNodeNC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeNC.h"
#include "NodeLine.h"
#include "Tree.h"
#include "HLDoc.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeNC::CNodeNC()
{
	m_type = NODE_NC;
	//
}

CNodeNC::~CNodeNC()
{

}


CTree *CNodeNC::UnGroup()
{
	CStroke *pStroke = (CStroke *)m_list.GetHead();
	
	if ( pStroke != NULL )
	{
		CNodeLine *pNode = new CNodeLine(pStroke);
		CTree *pTree = new CTree(pNode);
		
		m_list.RemoveHead();
		return pTree;
	}
	return NULL;
}

/*
GCode:针对只用
*/
BOOL CNodeNC::DeCode(CString strFile, BOOL bCenter, BOOL bBetter)  
{
	if( !m_file.Open(strFile,CFile::modeRead) )
	{
		AfxMessageBox(IDS_ERROR_OPENFILE);
		return FALSE;
	}
	
	m_dotCur.x = CENTER;
	m_dotCur.y = CENTER;
	m_pStroke  = NULL;
	
	m_bFirst  = TRUE;
	m_bCircle = FALSE;
	m_bPenUp  = TRUE;
	m_bCoorA  = TRUE;
	m_bMMUint = TRUE;
	
	CString strVal;
	CString strData;
	
	while( m_file.ReadString(strData) )
	{
		strData.TrimLeft();
		strData.TrimRight();
		if( strData.IsEmpty() )
			continue;
		
		strVal = strData.Left(1);
		if( strVal == _T("%") || strVal == _T("(") )
			continue;
		
		Process( strData );
	}

	m_file.Close();

	CalLimit();
	if( bCenter )//居中
	{
		CDot dot = GetLimit().GetBaseDot();
		double moveX,moveY;
		moveX = CENTER - dot.x;
		moveY = CENTER - dot.y;
		Move(moveX,moveY);
		CalLimit();
	}
	
	return TRUE;
}


void CNodeNC::Process( CString strData )
{
	CDot  dotMark = m_dotCur;
	int     nFind(0),nEnd(0),nStart(0);
	CString strCmd,strVal,strLeave;
	BOOL  bDataChg(FALSE);
	TCHAR   chCheck;
	
	strData.MakeUpper();
	strLeave = strData;
	while( !strLeave.IsEmpty() )
	{
		strCmd = strLeave.Left(1);
		if( strCmd == _T("X") || strCmd == _T("Y") || strCmd == _T("Z") || strCmd == _T("U") || strCmd == _T("V") || strCmd == _T("W"))
		{
			bDataChg = TRUE;
			nStart++;
			nFind = nStart;
			for (; nFind < strData.GetLength(); nFind++ )
			{
				chCheck = strData.GetAt(nFind);
				if (  chCheck >= _T('A') && chCheck <= _T('Z')  ) 
					break;
			}
			strVal = strData.Mid(nStart,nFind-nStart);
			nStart = nFind ;

			double dbPos = _tstof((LPCTSTR)strVal);
			if ( !m_bMMUint ) //英寸转换为mm
				dbPos *= 25.4;
			
			if( strCmd == _T("X") )
				dotMark.x =  m_bCoorA ? dbPos : dotMark.x + dbPos;
			else if ( strCmd == _T("Y") )
				dotMark.y =  m_bCoorA ? dbPos : dotMark.y + dbPos;
			else if ( strCmd == _T("Z") )
				dotMark.z =  m_bCoorA ? dbPos : dotMark.z + dbPos;
			else if ( strCmd == _T("U") )
				dotMark.x = m_dotCur.x + dbPos;
			else if ( strCmd == _T("V") )
				dotMark.y = m_dotCur.y + dbPos;
			else if ( strCmd == _T("W") )
				dotMark.z = m_dotCur.z + dbPos;
		}
		else
		{
			nFind = nStart+1;
			for (; nFind < strData.GetLength(); nFind++ )
			{
				chCheck = strData.GetAt(nFind);
				if (  chCheck >= 65 && chCheck <= 90  )
					break;
			}
			strCmd = strData.Mid(nStart,nFind-nStart);
			nStart = nFind ;
			//
			strCmd.TrimLeft();
			strCmd.TrimRight();

			OutputDebugString(strCmd);
			if ( strCmd == _T("G00") || strCmd == _T("G0") ){
				m_bCircle = FALSE;
				m_bPenUp = TRUE;
				m_bFirst = TRUE;
			}
			else if ( strCmd == _T("G01") || strCmd == _T("G1") ){
				m_bPenUp = FALSE;
				m_bCircle = FALSE;
			}
			else if ( strCmd == _T("G90") ){
				m_bCoorA = TRUE;
			}
			else if  ( strCmd == _T("G91") ){
				m_bCoorA = FALSE;
			}
			else if  ( strCmd == _T("G02") || strCmd == _T("G03") || strCmd == _T("G05") ){
				m_bCircle = TRUE;
			}
			else if  ( strCmd == _T("G91") ){
				m_bCoorA = FALSE;
			}
			else if ( strCmd == _T("G71") ){ //mm
				m_bMMUint = TRUE;
			}
			else if ( strCmd == _T("G70") ){ //寸
				m_bMMUint = FALSE;
			}
		}
		////////////////
		strLeave = strData.Mid(nFind);
	}

	if( bDataChg )
	{
		if( m_bCircle )//圆弧插补
		{

		}
		else//直线
		{
			if( !m_bPenUp ) // G01
			{
				if( m_bFirst ) // G00 转 G01
				{
					m_bFirst = FALSE;
					m_pStroke = new CStroke(m_property);
					m_pStroke->Add(m_dotCur);
					m_list.AddTail(m_pStroke);
				}
				m_pStroke->Add(dotMark);
			}
			m_dotCur = dotMark;
		}
	}
}





void CNodeNC::CreatePen(int pen)
{
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = 
             (CMDIChildWnd *) pFrame->GetActiveFrame();
	CHLDoc *pDoc = (CHLDoc *) pChild->GetActiveDocument();
	
	CLayer ly=pDoc->GetLayer(pen);
	m_property=ly.GetProperty();
}




void CNodeNC::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;
	
	if( bFlag )
	{
		//写入头信息
		fs << _T("NNC_B");
		m_property.Serialize(fp,bFlag);
		Serialize_list(fp,bFlag);

		//写入尾信息
		fs << _T("NNC_E");
	}
	else
	{
		while( fs >> str )
		{
			if ( str.Left(5) == _T("PRO_B") )
				m_property.Serialize(fp,bFlag);
			else if ( str.Left(4) == _T("SL_B") )
				Serialize_list(fp,bFlag);
			else if ( str.Left(5) == _T("NNC_E") )
				break;
		}
	}
}

void CNodeNC::CreateCircle(CDot dotCenter, double dbRadius)
{
	//根据弦长来计算圆的信息
	double dbLen = 0.01;		//以后可设置
	double dbAngle;				//弦长半径对应的角度
	double dbCurAngle;
	////////////
	dbAngle = (asin((dbLen/2)/dbRadius))*2;
	int nCount = (int)(2*PAI/dbAngle+0.5);
	
	CDot dot;
	dbAngle = (2*PAI/nCount);
	CStroke *pStroke = new CStroke(m_property);

	for(int i=0;i<nCount;i++)
	{
		dbCurAngle = i*dbAngle;
		dot.x = dbRadius*cos(dbCurAngle);
		dot.y = dbRadius*sin(dbCurAngle);
		pStroke->Add(dot);
	}
	dot.x = dbRadius;
	dot.y = 0;
	pStroke->Add(dot);

	pStroke->Move(dotCenter.x,dotCenter.y);

	m_list.AddTail(pStroke);
}


void CNodeNC::LoadWheelList(int nType, StrokeList *pListStroke)
{	
	if( m_list.GetCount() > 0 )
	{
		POSITION  pos = m_list.GetHeadPosition();
		CStroke *pStroke = NULL;
		while(pos != NULL)
		{
			pStroke = m_list.GetNext(pos);
			
			if( pStroke != NULL )
			{
				CStroke *pNew = new CStroke(pStroke->m_property);
				if ( NULL == pNew )
					continue;
				
				pNew->Append(*pStroke);	
				
				pListStroke->AddTail(pNew);
			}
		}
	}
}




BOOL CNodeNC::CanUnGroup()
{
	if( m_list.GetCount() > 0 )
		return TRUE;
	else
		return FALSE;
}



//
BOOL CNodeNC::SetLayerSelect(int nItem, BOOL bFlag)
{
	if( m_list.GetCount() > 0) 
	{
		CStroke *pStroke = NULL;
		POSITION pos = m_list.GetHeadPosition();
		while( pos!=NULL )
		{	
			pStroke = m_list.GetNext(pos);			
			if(pStroke->m_property.m_nLayer == nItem)
				return TRUE;
		}
	}
	
	return FALSE;
}
CNodeLine* CNodeNC::UnGroupplt()
{
	POSITION prev=NULL , pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	int nLay= -1;
	BOOL bFirst = TRUE;
	CNodeLine *pNode = NULL;

	while(pos != NULL)	
	{
		prev = pos;
		pStroke = (CStroke *)m_list.GetNext(pos);

		if(TRUE == bFirst )
		{
			if(TRUE == bFirst )
			{
				if(pStroke != NULL)
				{
					pNode = new CNodeLine();
					nLay = pStroke->m_property.m_nLayer;
					pNode->m_property.m_color = pStroke->m_property.m_color;
					pNode->m_property.m_nLayer = pStroke->m_property.m_nLayer;

					pNode->m_list.AddTail(pStroke);
					m_list.RemoveAt(prev);
					bFirst = FALSE;
				}
			}
		}
		else
		{
			if(pStroke->m_property.m_nLayer==nLay)
			{
				pNode->m_list.AddTail(pStroke);
				m_list.RemoveAt(prev);
			}
		}
	}

	return pNode;
}
