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

class dpoint
{
public:
	double x;
	double y;
};

class dround
{
public:
	dpoint set;
	double dia;
};
class drect
{
public:
	dpoint minxy;
	dpoint maxxy;
};

//////////////////////////////////////////////////////////////////////////////////
bool g_bGerberCheckOK = false;
int (*d_check_for_hanslaser)(char *path)=0;
int (*d_gerber_outline)(char *exepath,char *gbrfile,double offset_dia)=0;
int (*d_gerber_outline_get)(dpoint *set1,dpoint *set2,double *width1,double *width2,int *data )=0;
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
		bChecked = true;

		if (d_gerber_outline==0)
		{
			CString strDllPath = g_sys.GetInstallPath() + "Gerber\\work_pcb.dll";
			HMODULE SD=LoadLibrary(strDllPath);
			if(SD)
			{
				d_check_for_hanslaser = ( int (*)(char *) )GetProcAddress(SD,"_d_check_for_hanslaser");
				d_gerber_outline = ( int (*)(char *, char*, double) )GetProcAddress(SD,"_d_gerber_outline");
				d_gerber_outline_get = (int (*)(dpoint *,dpoint *,double *,double *,int *) )GetProcAddress(SD,"_d_gerber_outline_get");
			}
			if (d_check_for_hanslaser==0 || d_gerber_outline==0 || d_gerber_outline_get==0)
			{
				WriteRunLog("Fail to open work_pcb.dll");
				return FALSE;
			}
		}

		CString strDllDir = g_sys.GetInstallPath()+"Gerber";
		SetDllDirectory(strDllDir);

		int ret = d_check_for_hanslaser((char*)(LPCTSTR)strDllDir);
		if (ret!=1)
		{
			if (ret==-2)
				WriteRunLog("Gerber dongle Error!");
			else
				WriteRunLog("lisence Error!");
		}
		
		SetDllDirectory(NULL);
		g_bGerberCheckOK = (ret==1);
	}
	return g_bGerberCheckOK;
}

BOOL CNodeGerber::DeCode(CString file,BOOL bCenter,double dbPrecision)
{
	if(!g_bGerberCheckOK)
		return FALSE;

	CString strDllDir = g_sys.GetInstallPath()+"Gerber";
	SetDllDirectory(strDllDir);
	int ret = d_gerber_outline((char*)(LPCTSTR)(strDllDir),(char*)(LPCTSTR)file, g_ParaIni.m_dbGerberOffset/25.4);
	SetDllDirectory(NULL);
	if (ret!=0)
	{
		AfxMessageBox("lisence Error!");
		return FALSE;
	}

	dpoint set1;
	dpoint set2;
	double width1,width2;
	int data;
	int type;

	while( (type=d_gerber_outline_get(&set1,&set2,&width1,&width2,&data))!=0 )
	{
		//1英寸(in)=25.4毫米(mm)
		double dbUnitConvert = 25.4;

		switch (type)
		{
		case 1:	//圆
			{
				CStroke *pStroke=new CStroke(m_property);
				GetCircleDot(pStroke, CDot(set1.x*dbUnitConvert, set1.y*dbUnitConvert), width1/2.0*dbUnitConvert);
				m_list.AddTail(pStroke);
			}
			break;
		case 2:	//多边形
			{
				dpoint *ps;
				int psn;
				int *a=(int*)(&width1);
				ps=(dpoint *)(*a);
				a=(int*)(&width2);
				psn=*a;


				CStroke *pStroke=new CStroke(m_property);
				for( int m=0;m<psn;m++ )					
					pStroke->Add(CDot(ps[m].x*dbUnitConvert, ps[m].y*dbUnitConvert));
				pStroke->Add(CDot(ps[0].x*dbUnitConvert, ps[0].y*dbUnitConvert));
				m_list.AddTail(pStroke);
			}
			break;
		case 3:
			{
				//DPOINT sp[2]; sp[0]=set1;sp[1]=set2;
				//baseobjs.addLine2p(width1,set1,set2,0,0);
			}
			break;
		case 4:	//距形
			{
				CStroke *pStroke=new CStroke(m_property);
				pStroke->Add(CDot(set1.x*dbUnitConvert, set2.y*dbUnitConvert));
				pStroke->Add(CDot(set2.x*dbUnitConvert, set2.y*dbUnitConvert));
				pStroke->Add(CDot(set2.x*dbUnitConvert, set1.y*dbUnitConvert));
				pStroke->Add(CDot(set1.x*dbUnitConvert, set1.y*dbUnitConvert));
				pStroke->Add(CDot(set1.x*dbUnitConvert, set2.y*dbUnitConvert));
				m_list.AddTail(pStroke);
			}
			break;
		default:
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