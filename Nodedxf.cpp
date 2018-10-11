// NodeDxf.cpp: implementation of the CNodeDxf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeDxf.h"
#include "Tree.h"
#include "HLDoc.h"
#include "NodeLine.h"

#import "DxfLib.tlb" no_namespace

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeDxf::CNodeDxf()
{
	m_type  = NODE_DXF;
	m_bauto = TRUE;
	
	for(int i=0; i < 50; i++)
	{
		m_DXF[i]="";
	}

	return;
}

CNodeDxf::~CNodeDxf()
{
	Delete();
}

BOOL CNodeDxf::DeCode(CString file, CString strShxDir, double dbUnitTrans, double dbPrecision, BOOL bCenter, BOOL bBetter)
{
	IDxfTranslaterPtr pDxfTranslater;
	HRESULT hr = pDxfTranslater.CreateInstance(__uuidof(DxfTranslater));
	if (FAILED(hr))
	{
		AfxMessageBox("Fail to open DxfLib.dll");
		return FALSE;
	}
	
	_bstr_t bstrErrInfo;
	try
	{
		if (pDxfTranslater->ParseDxfFile(_bstr_t((LPCTSTR)file), _bstr_t((LPCTSTR)strShxDir), dbPrecision, dbUnitTrans, bstrErrInfo.GetAddress()))
		{
			long nType, nSize;
			_bstr_t bstrLayer;
			SAFEARRAY *pSafeArrayX, *pSafeArrayY;
			double *pArrayX, *pArrayY;
			while(pDxfTranslater->GetNextObject(&nType, bstrLayer.GetAddress(), &nSize, &pSafeArrayX, &pSafeArrayY))
			{
				SafeArrayAccessData(pSafeArrayX, (void**)&pArrayX);
				SafeArrayAccessData(pSafeArrayY, (void**)&pArrayY);

				CopyData(nType, (LPCTSTR)bstrLayer, nSize, pArrayX, pArrayY);

				SafeArrayUnaccessData(pSafeArrayX);
				SafeArrayUnaccessData(pSafeArrayY);
				SafeArrayDestroy(pSafeArrayX);
				SafeArrayDestroy(pSafeArrayY);
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
		}
		else
		{
			AfxMessageBox((LPCTSTR)bstrErrInfo);
			return FALSE;
		}

		if (bstrErrInfo.length()>0)
			AfxMessageBox((LPCTSTR)bstrErrInfo);
	}
	catch (CMemoryException* e)
	{
		TCHAR tchInfo[512] = {_T('\0')};
		e->GetErrorMessage(tchInfo, 512);
		AfxMessageBox(tchInfo);
		WriteRunLog(tchInfo);

		CString strErrorInfo;
		strErrorInfo = _T("导入dxf或dwg失败，内存异常，请检查");
		AfxMessageBox(strErrorInfo);
		WriteRunLog(strErrorInfo);

		return FALSE;
	}
	return TRUE;	
}

void CNodeDxf::CopyData(int nType, LPCTSTR strLayer, int nSize, double *arrX, double *arrY)
{
	int     count = 0;
	CDot    start(0.0,0.0), end(0.0,0.0);
	CDot    center(0.0,0.0);
	double  radius=0.0;
	int     nDirection = 1;
	CString strLayerName = strLayer;
	
	if(!strLayerName.IsEmpty())
		ChangeLay(strLayerName);

	if (nType==0)	//点、线
	{
		CStroke *pStroke=new CStroke(m_property);

		for (int i=0; i<nSize; i++)
		{
			end.x = ChTrue(arrX[i],0);
			end.y = ChTrue(arrY[i],0);
			pStroke->Add(end);
		}
		m_list.AddTail(pStroke);
	}
	else if (nType==1)	//弧
	{
		CDot m_dotStart(arrX[0], arrY[0]);	//起点
		CDot m_dotEnd(arrX[1], arrY[1]);		//终点
		CDot m_dotCenter(arrX[2], arrY[2]);	//圆心
		BOOL m_bCCW = (BOOL)arrX[3];		//方向:0-顺;1-逆
	}
	
	return;
}

void CNodeDxf::CopyData(DXFOBJOUTLIST_STROKE *pentlist, StrokeList& HLSList)
{
	int     count = 0;
	CDot    start(0.0,0.0), end(0.0,0.0);
	CDot    center(0.0,0.0);
	double  radius=0.0;
	int     nDirection = 1;
	CString strLayerName("");

	POSITION pos = pentlist->GetHeadPosition();
	while(pos!=NULL)
	{
		OBJ_OUTSTROKE* ent = pentlist->GetNext(pos);
		strLayerName.Format("%s",ent->m_nLayerName);
		if(!strLayerName.IsEmpty())
			ChangeLay(strLayerName);

		CStroke *pStroke;
		if (ent->m_bContinueStroke)
			pStroke=new CStroke(m_property);

		POSITION Arcpos = ent->lststkOut.GetHeadPosition();
		while(Arcpos!=NULL)
		{
			if(!ent->m_bContinueStroke)
				pStroke=new CStroke(m_property);

			CDxfStroke *arcCirOut = ent->lststkOut.GetNext(Arcpos);
			int iSize = arcCirOut->m_pointArray.GetSize();
			for (int i=0;i<iSize;i++)
			{
				end.x = ChTrue(arcCirOut->m_pointArray.GetAt(i).x,1);
				end.y = ChTrue(arcCirOut->m_pointArray.GetAt(i).y,0);
				pStroke->Add(end);
			}

			if (!ent->m_bContinueStroke)
				m_list.AddTail(pStroke);
		}

		if(ent->m_bContinueStroke)
			m_list.AddTail(pStroke);	
	}

	return;
}	
double CNodeDxf::ChTrue(double a,BOOL flag)
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

void CNodeDxf::ChangeLay(CString &layerName)
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

void CNodeDxf::CreatePen(int pen)
{
	CMDIFrameWnd *pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = 
             (CMDIChildWnd *) pFrame->GetActiveFrame();
	CHLDoc *pDoc = (CHLDoc *) pChild->GetActiveDocument();
	
	CLayer ly=pDoc->GetLayer(pen);
	m_property=ly.GetProperty();
}

CTree *CNodeDxf::UnGroup()
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

CNodeDxf *CNodeDxf::UnGroupDxf()
{
	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	int nLay= -1;
	BOOL bFirst = FALSE;
	CNodeDxf *pNode=NULL;

	while(pos!=NULL)
	{	
		prev = pos;
		pStroke=m_list.GetNext(pos);
		if(!bFirst)
		{
			nLay = pStroke->m_property.m_nLayer;
			pNode=new CNodeDxf();
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

void  CNodeDxf::GetScribe(CString&Scribe)
{
	CString   str;
	str.Format("This is  a  Dxf's node");
	Scribe = str;
}


//按图层分解导入的线条，每组相同图层的线条作为一个PLT对象。
CNodeDxf *CNodeDxf::UnGroupplt()
{
	POSITION prev;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	int nLay= -1;
	BOOL bFirst = FALSE;
	CNodeDxf *pNode=NULL;

	while(pos!=NULL)
	{	
		prev = pos;
		pStroke=m_list.GetNext(pos);
		if(!bFirst)
		{
			nLay = pStroke->m_property.m_nLayer;
			pNode=new CNodeDxf();
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

void CNodeDxf::GetCircleDot(CStroke *pStroke, CDot &start, CDot &center, double radius)
{
	//根据弦长来计算圆的信息
	//double dbLen = g_SysCfg.m_dbGraphImportPrics;		//以后可设置
	double dbLen = 0.01;
	double dbAngle = (asin((dbLen / 2) / radius)) * 2; //弦长半径对应的角度

	CDot sDt(start), cDt(center);
	ChCoord(center.x, center.y, 0.0, sDt, FALSE);

	double dbCurAngle(0.0);
	int nCount = (int)(2*PAI/dbAngle+0.5);
	CDot dot;

	dbAngle = (2*PAI/nCount);

	for(int i = 1; i < nCount; i++)
	{
		dbCurAngle = i*dbAngle;
		dot.x = radius*cos(dbCurAngle);
		dot.y = radius*sin(dbCurAngle);
		ChCoord(center.x, center.y, 0.0, dot, TRUE);
		pStroke->Add(dot);
	}
	
	dot.x = radius*cos((double)0);
	dot.y = radius*sin((double)0);
	ChCoord(center.x, center.y, 0.0, dot, TRUE);
	pStroke->Add(dot);

//	pStroke->Move(center.x, center.y);

	return;
}

void CNodeDxf::GetArcDot(CStroke *pStroke, CDot &start, CDot &end, CDot &center, double radius, int nDir)
{
	//根据弦长来计算圆的信息
	//double dbLen   = g_SysCfg.m_dbGraphImportPrics;		//以后可设置
	double dbLen = 0.01;
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
			int nCount = (int)floorl(fabs(dbStartEnd - dbStartAngle) / dbAngle + 0.5);
			for(int i = 1; i < nCount; i++)
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
			int nCount = (int)floorl((2 * PAI - fabs(dbStartEnd - dbStartAngle)) / dbAngle + 0.5);
			for(int i = 1; i < nCount; i++)
			{
				dbCurAngle = -i*dbAngle + dbStartAngle;
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
			int nCount = (int)floorl((2 * PAI - fabs(dbStartEnd - dbStartAngle)) / dbAngle + 0.5);
			for(int i = 1; i < nCount; i++)
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
			int nCount = (int)floorl(fabs(dbStartEnd - dbStartAngle) / dbAngle + 0.5);
			for(int i = 1; i < nCount; i++)
			{
				dbCurAngle = -i*dbAngle + dbStartAngle;
				dot.x = radius*cos(dbCurAngle);
				dot.y = radius*sin(dbCurAngle);
				ChCoord(center.x, center.y, 0.0, dot, TRUE);
				pStroke->Add(dot);
			}
		}
	}

	return;
}

void CNodeDxf::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;

	if (bFlag)
	{
		//写入头信息
		fs<<"NDXF_B";
		m_property.Serialize(fp,bFlag);
		Serialize_list(fp,bFlag);
		//写入尾信息
		fs<<"NDXF_E";
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
			if(str.Left(6) == "NDXF_E")
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
void CNodeDxf::ChCoord(double xf,double yf,double angle, CDot &dt, BOOL bIsOld)
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