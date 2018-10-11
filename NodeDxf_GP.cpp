// NodeDxf_GP.cpp: implementation of the CNodeDxf_GP class.
//
//////////////////////////////////////////////////////////////////////

//*******************************************************************//
//Class implementation for translate DXF to stroke list for HL4.0,designed by 
//PengZili,May. 2005.
//Revised by base software development department 2011.04
//
//phone:0755-86161316;E-mail:pzili@21cn.com
//copyright reserved by HAN'S LASER co. ltd.(Shen Zhen)
//*******************************************************************//

#include "stdafx.h"
#include "hl.h"
#include "NodeDxf_GP.h"
//#include "baseMacro.h"
#include "OptStrokeList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeDxf_GP::CNodeDxf_GP()
{
	m_type  = NODE_DXF;
	
	for(int i=0; i < 50; i++)
	{
		m_DXF[i]=_T("");
	}
}

CNodeDxf_GP::~CNodeDxf_GP()
{
	Delete();
}


BOOL CNodeDxf_GP::DeCode(CString file,BOOL bCenter,BOOL bBetter)
{
	CString strFontPath = ::g_sys.GetInstallPath()+_T("EFonts\\");
	
	if( !m_dxfTranslate.Translate(file,0.1,strFontPath) )
		return FALSE;

	CopyData(&m_dxfTranslate.m_DxfStrokeList,m_list);
	CalLimit();
	
	//居中
	if ( bCenter )
	{
		CDot dot = GetLimit().GetBaseDot();
		double moveX=CENTER-dot.x;
		double moveY=CENTER-dot.y;
		Move(moveX,moveY);
	}
	//优化数据
	if ( bBetter ) 
	{
		COptStrokeList tmp;
		tmp.Attach(&m_list,0.01);
		tmp.FullOpt();
		tmp.Detach();
	}

	CalLimit();
	
	return TRUE;
}

void CNodeDxf_GP::CopyData(CTypedPtrList<CObList,CDxfStroke *> *pDxfSList,StrokeList& HLSList)
{
	CStdioFile file1;
	TCHAR tcFileName[MAX_PATH];
	::GetModuleFileName(NULL, tcFileName, MAX_PATH);
	file1.Open(_T("C:\\RefPoint.txt"),CFile::modeCreate|CFile::modeWrite);


	POSITION pos=pDxfSList->GetHeadPosition();
	CDxfStroke *pDxfStroke=NULL;
	while( pos != NULL )
	{
		pDxfStroke=pDxfSList->GetNext(pos);
		if ( pDxfStroke->m_nLayerName != "" )
			ChangeLay(pDxfStroke->m_nLayerName);
		
		int pointSum = pDxfStroke->m_pointArray.GetSize();
		CStroke* pHLStroke=new CStroke(m_property);
		
		CDot tempDot;
		for(int i=0;i<pointSum;i++)
		{
			tempDot.x = pDxfStroke->m_pointArray.GetAt(i).x;
			tempDot.y = pDxfStroke->m_pointArray.GetAt(i).y;
			pHLStroke->Add(tempDot);

			CString str;
			str.Format(_T("%f,%f\n"),tempDot.x,tempDot.y);
			file1.WriteString(str);
		}
		HLSList.AddTail(pHLStroke);
	}

	file1.Close();
}

void CNodeDxf_GP::ChangeLay(CString layerName)
{
	for( int i=0; i<50; i++ )
	{
		if ( m_DXF[i]==_T("") )
		{		
			m_DXF[i]=layerName;
			CreatePen(i);
			break;
		}
		if( m_DXF[i]==layerName )
		{	
			CreatePen(i);
			break;
		}
	}
}

void CNodeDxf_GP::CreatePen(int pen)
{
// 	CHLDoc *pDoc = (CHLDoc *) GetCurActiveDocument();
// 	if ( pDoc == NULL )
// 		return;
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild =(CMDIChildWnd *) pFrame->GetActiveFrame();
	CHLDoc *pDoc = (CHLDoc *) pChild->GetActiveDocument();
	if ( pDoc == NULL )
		return;
	
	CLayer ly = pDoc->GetLayer(pen);
	m_property= ly.GetProperty();
}



CNodeLine *CNodeDxf_GP::UnGroupDxf_GP()
{
	POSITION prev=NULL , pos = m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
	int nLay= -1;
	BOOL bFirst = TRUE;
	CNodeLine *pNode = NULL;
	
	while( pos != NULL )	
	{
		prev = pos;
		pStroke = (CStroke *)m_list.GetNext(pos);
		
		if( TRUE == bFirst )
		{
			if( TRUE == bFirst )
			{
				if( pStroke != NULL )
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




void CNodeDxf_GP::Serialize(FILE* fp,BOOL bFlag/*,BOOL bUnicode*/)
{
	CMySerialize fs(fp/*,bUnicode*/);
	CString str;
	
	if ( bFlag )
	{
		//写入头信息
		fs << _T("NDXF_B");
		//m_quadRangle.Serialize(fp,bFlag,bUnicode);
		m_property.Serialize(fp,bFlag/*,bUnicode*/);
		Serialize_list(fp,bFlag/*,bUnicode*/);
		//写入尾信息
		fs << _T("NDXF_E");
	}
	else
	{
		while( fs >> str ) 
		{
			if ( str.Left(5) == _T("PRO_B") )
				m_property.Serialize(fp,bFlag/*,bUnicode*/);
			else if ( str == _T("QUAD_RANG_B") )
				//m_quadRangle.Serialize(fp,bFlag,bUnicode)
				;
			else if ( str.Left(4) == _T("SL_B") )
				Serialize_list(fp,bFlag/*,bUnicode*/);
			else if ( str.Left(6) == _T("NDXF_E") )
				break;
		}
	}
}
