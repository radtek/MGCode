// NodeDxf.cpp: implementation of the CNodeDxf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeGerber.h"
#include "Tree.h"
#include "HLDoc.h"
#include "NodeLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL g_bGerberCheckOK = false;

BOOL GerberCommandLine(LPCTSTR strCommand, LPCTSTR strFile, double dbGerberOffset, BOOL &bResult, LPVOID*pData)
{
	static HANDLE hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024*1024*100, "{ABD8B157-F237-401F-9F47-1D832F4488CC}");
	static LPVOID pBuffer = ::MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	DWORD dwExitCode;
	char szCommandLine[512];
	if (strFile==NULL)
		sprintf_s(szCommandLine, MAX_PATH, "\"%s\" %s", g_sys.GetInstallPath()+"Gerber\\HLGerber.exe", strCommand);
	else
		sprintf_s(szCommandLine, MAX_PATH, "\"%s\" %s \"%s\" %.6f", g_sys.GetInstallPath()+"Gerber\\HLGerber.exe", strCommand, strFile, dbGerberOffset);

	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;
	BOOL ret = CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE,CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	if (ret)
	{
		CloseHandle(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
		CloseHandle(pi.hProcess);

		if (dwExitCode==0)
		{
			memcpy(&bResult, pBuffer, 4);
			if (pData)	*pData = (int*)pBuffer + 1;
			return TRUE;
		}
	}

	CString strlog;
	strlog.Format("GerberInfo: %x %x %d %d %d", hMap, pBuffer, ret, dwExitCode, GetLastError());
	WriteRunLog(strlog);
	return FALSE;
}

inline int ReadInt(LPVOID &buffer)
{
	int type = 0;
	memcpy(&type, buffer, sizeof(int));
	buffer = (int*)buffer + 1;
	return type;
}

inline double ReadDouble(LPVOID &buffer)
{
	double data;
	memcpy(&data, buffer, sizeof(double));
	buffer = (double*)buffer + 1;
	return data;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeGerber::CNodeGerber()
{
	m_type  = NODE_GERBER;
	m_bauto = TRUE;
	
	for(int i=0; i < 50; i++)
	{
		m_DXF[i]="";
	}

	return;
}

CNodeGerber::~CNodeGerber()
{
	Delete();
}

BOOL CNodeGerber::CheckGerberDog()
{
	static bool bChecked = false;
	if (!bChecked)
	{
		LPVOID pData;
		if (GerberCommandLine("d_check_for_hanslaser", NULL, 0, g_bGerberCheckOK, &pData))
			bChecked = true;

		if (!g_bGerberCheckOK)
			WriteRunLog((LPCTSTR)pData);
	}
	return g_bGerberCheckOK;
}

BOOL CNodeGerber::DeCode(CString file,BOOL bCenter,double dbPrecision)
{
	if(!g_bGerberCheckOK)
		return FALSE;

	BOOL bResult = FALSE;
	LPVOID pData = NULL;
	if (!GerberCommandLine("d_gerber_outline", file, g_ParaIni.m_dbGerberOffset, bResult, &pData))
		return FALSE;

	if (!bResult)
	{
		AfxMessageBox((LPCTSTR)pData);
		return FALSE;
	}

	int type = 0;
	while( (type=ReadInt(pData))!=0 )
	{
		if (type==1) //圆
		{
			CDot center;
			double radius;
			center.x = ReadDouble(pData);
			center.y = ReadDouble(pData);
			radius = ReadDouble(pData);

			CStroke *pStroke=new CStroke(m_property);
			GetCircleDot(pStroke, center, radius);
			m_list.AddTail(pStroke);
		}
		else if (type==2) //距形
		{
			CStroke *pStroke=new CStroke(m_property);	

			CDot dot;
			for (int i=0; i<5; i++)
			{
				dot.x = ReadDouble(pData);
				dot.y = ReadDouble(pData);
				pStroke->Add(dot);
			}
			m_list.AddTail(pStroke);
		}
		else if (type==3)	//多边形
		{
			CDot dot;
			int num = ReadInt(pData);

			CStroke *pStroke=new CStroke(m_property);
			for (int i=0; i<num; i++)
			{
				dot.x = ReadDouble(pData);
				dot.y = ReadDouble(pData);
				pStroke->Add(dot);
			}
			m_list.AddTail(pStroke);
		}
		else
		{
			break;
		}
	}

	CalLimit();

	//居中
	if (bCenter)
	{
		//CDot dot= GetLimit().GetBaseDot();
		CDot dot= CalLimit().GetBaseDot();
		double moveX,moveY;
		moveX=CENTER-dot.x;
		moveY=CENTER-dot.y;
		Move(moveX,moveY);
	}
			
	return TRUE;	
}

double CNodeGerber::ChTrue(double a,BOOL flag)
{
	double b;

	b = a;
/*	if(m_bauto){
		if(flag)
            b=DPMM*a+CENTER;
		else
            b=-DPMM*a+CENTER;
	}
	else{
		if(flag)
			b=1250*a+CENTER;
		else
			b=-1250*a+CENTER;
	}*/
	return b;
}

void CNodeGerber::ChangeLay(CString &layerName)
{
	for(int i=0;i<50;i++)
	{
		if(m_DXF[i]=="")
		{		
			m_DXF[i]=layerName;
			CreatePen(i);
			break;
		}
		if(m_DXF[i]==layerName)
		{	
			CreatePen(i);
			break;
		}
	}

	return;
}

void CNodeGerber::CreatePen(int pen)
{
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = 
             (CMDIChildWnd *) pFrame->GetActiveFrame();
	CHLDoc *pDoc = (CHLDoc *) pChild->GetActiveDocument();
	
	CLayer ly=pDoc->GetLayer(pen);
	m_property=ly.GetProperty();
}

CTree *CNodeGerber::UnGroup()
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke = NULL;
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

CNodeGerber *CNodeGerber::UnGroupGerber()
{
	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	int nLay= -1;
	BOOL bFirst = FALSE;
	CNodeGerber *pNode=NULL;

	while(pos!=NULL)
	{	
		prev = pos;
		pStroke=m_list.GetNext(pos);
		if(!bFirst)
		{
			nLay = pStroke->m_property.m_nLayer;
			pNode=new CNodeGerber();
			pNode->m_property.m_color = pStroke->m_property.m_color;
			pNode->m_property.m_nLayer = pStroke->m_property.m_nLayer;
			bFirst = TRUE;
		}
		if(pStroke->m_property.m_nLayer==nLay)
		{
			pNode->m_list.AddTail(pStroke);
			m_list.RemoveAt(prev);
		}
	}
	return pNode;
}

void  CNodeGerber::GetScribe(CString&Scribe)
{
	CString   str;
	str.Format("This is  a  Dxf's node");
	Scribe = str;
}


//按图层分解导入的线条，每组相同图层的线条作为一个PLT对象。
CNodeGerber *CNodeGerber::UnGroupplt()
{
	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	int nLay= -1;
	BOOL bFirst = FALSE;
	CNodeGerber *pNode=NULL;

	while(pos!=NULL)
	{	
		prev = pos;
		pStroke=m_list.GetNext(pos);
		if(!bFirst)
		{
			nLay = pStroke->m_property.m_nLayer;
			pNode=new CNodeGerber();
			pNode->m_property.m_color = pStroke->m_property.m_color;
			pNode->m_property.m_nLayer = pStroke->m_property.m_nLayer;
			bFirst = TRUE;
		}

		if(pStroke->m_property.m_nLayer==nLay)
		{
			pNode->m_list.AddTail(pStroke);
			m_list.RemoveAt(prev);
		}
	}
	
	return pNode;
}

void CNodeGerber::GetCircleDot(CStroke *pStroke, CDot &center, double radius)
{
	//根据弦长来计算圆的信息
	double dbLen = g_ParaIni.m_dbDxfPrecision;
	double dbAngle = (asin((dbLen / 2) / radius)) * 2; //弦长半径对应的角度
	
	double dbCurAngle(0.0);
	int nCount = (int)(2*PAI/dbAngle+0.5);
	dbAngle = (2*PAI/nCount);

	CDot dot;
	for(int i = 0; i <= nCount; i++)
	{
		dbCurAngle = i*dbAngle;
		dot.x = center.x + radius*cos(dbCurAngle);
		dot.y = center.y + radius*sin(dbCurAngle);

		pStroke->Add(dot);
	}
}

void CNodeGerber::GetArcDot(CStroke *pStroke, CDot &start, CDot &end, CDot &center, double radius, int nDir)
{
	//根据弦长来计算圆的信息
	//double dbLen   = g_SysCfg.m_dbGraphImportPrics;		//以后可设置
	double dbLen = g_ParaIni.m_dbDxfPrecision;
	double dbAngle = (asin((dbLen / 2) / radius)) * 2; //弦长半径对应的角度

	CDot sDt(start), eDt(end), cDt(center);
	ChCoord(center.x, center.y, 0.0, sDt, FALSE);
	ChCoord(center.x, center.y, 0.0, eDt, FALSE);

	double dbStartAngle = atan2(sDt.y, sDt.x);
	if (dbStartAngle < 0) 
	{
		dbStartAngle += 2*PAI;
	}

	double dbStartEnd   = atan2(eDt.y,     eDt.x);
	if (dbStartEnd < 0) 
	{
		dbStartEnd += 2*PAI;
	}

	CDot dot;
	double dbCurAngle(0.0);
	if (dbStartAngle <= dbStartEnd)
	{
		// 逆时钟方向
		if (1 == nDir)
		{
			int nCount = (int)(fabs(dbStartEnd - dbStartAngle) / dbAngle);
			for(int i = 0; i <= nCount; i++)
			{
				dbCurAngle = i * dbAngle + dbStartAngle;
				dot.x = radius*cos(dbCurAngle);
				dot.y = radius*sin(dbCurAngle);
				ChCoord(center.x, center.y, 0.0, dot, TRUE);
				pStroke->Add(dot);
			}
		}
		else // 顺时钟方向
		{
			int nCount = (int)((2 * PAI - fabs(dbStartEnd - dbStartAngle)) / dbAngle);
			for(int i = 0; i <= nCount; i++)
			{
				dbCurAngle = i*dbAngle + dbStartAngle;
				dot.x = radius*cos(dbCurAngle);
				dot.y = radius*sin(dbCurAngle);
				ChCoord(center.x, center.y, 0.0, dot, TRUE);
				pStroke->Add(dot);
			}
		}
	}
	else
	{
		// 逆时钟方向
		if (1 == nDir)
		{
			int nCount = (int)((2 * PAI - fabs(dbStartEnd - dbStartAngle)) / dbAngle);
			for(int i = 0; i <= nCount; i++)
			{
				dbCurAngle = i*dbAngle + dbStartAngle;
				dot.x = radius*cos(dbCurAngle);
				dot.y = radius*sin(dbCurAngle);
				ChCoord(center.x, center.y, 0.0, dot, TRUE);
				pStroke->Add(dot);
			}

		}
		else // 顺时钟方向
		{
			int nCount = (int)(fabs(dbStartEnd - dbStartAngle) / dbAngle);
			for(int i = 0; i <= nCount; i++)
			{
				dbCurAngle = i*dbAngle + dbStartAngle;
				dot.x = radius*cos(dbCurAngle);
				dot.y = radius*sin(dbCurAngle);
				ChCoord(center.x, center.y, 0.0, dot, TRUE);
				pStroke->Add(dot);
			}
		}	
	}
	if (!(pStroke->GetLastDot()==end))
		pStroke->Add(end);

	return;
}

void CNodeGerber::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;

	if (bFlag)
	{
		//写入头信息
		fs<<"NGBR_B";
		m_property.Serialize(fp,bFlag);
		Serialize_list(fp,bFlag);
		//写入尾信息
		fs<<"NGBR_E";
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
			if(str.Left(4) == "SL_B"){
				Serialize_list(fp,bFlag);
				continue;
			}
			if(str.Left(6) == "NGBR_E")
				break;
		}
	}
}

//***********************************************************************
// Param: (double xf, yf) :   “新坐标系”相对于“老坐标系” 的偏移量
//        (double   angle):   “新坐标系”相对于“老坐标系” 的旋转角度
//        (CDot &dt      ):    需要转换的坐标值
//        (BOOL    bIsOld):  TURE  返回值相对于老坐标
//                           FALSE 返回值相对于新坐标
//***********************************************************************
void CNodeGerber::ChCoord(double xf,double yf,double angle, CDot &dt, BOOL bIsOld)
{
	double xS(dt.x);
	double yS(dt.y);
	double dbCos = cos(angle);
	double dbSin = sin(angle);
	
	if (bIsOld)
	{
		xS = dbCos * dt.x - dbSin * dt.y;
		yS = dbCos * dt.y + dbSin * dt.x;
		dt.x  = xS + xf;
		dt.y  = yS + yf;
	}
	else
	{
		xS   = dt.x - xf;
		yS   = dt.y - yf;
		dt.x = dbCos * xS + dbSin * yS;
		dt.y = dbCos * yS - dbSin * xS;
	}

	return;
}