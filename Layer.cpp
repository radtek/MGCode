// Layer.cpp: implementation of the CLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "Layer.h"
#include "Property.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CLayer,CObject,2000)

CLayer::CLayer()
{
	m_color	=RGB(0,0,0);
	m_nLayer=0;
	m_bDefault=TRUE;
	m_bLockFlag = FALSE;
//	m_nCount = 1;

	char szPath[256];
	GetExepath(szPath);
	char szFileName[256];
	sprintf(szFileName, "%s%s\\%s", szPath, g_sys.m_strCardName, "default.yh");
	
	FILE *fp;
	if((fp=fopen(szFileName,"rb"))==NULL)
	{
		PARA para;
		m_para=para;//初始化
	}
	else
	{
//		fread(&m_para,sizeof(PARA),1, fp);
		//
		Serialize(fp, FALSE);
		//
		fclose(fp);
	}

	m_color	=RGB(0,0,0);
	m_nLayer=0;
	m_bDefault=TRUE;
	m_bLockFlag = FALSE;
}

CLayer::CLayer(int nType)
{
	
}

CLayer::~CLayer()
{

}

void CLayer::operator=(const CLayer& src)
{  	
	m_nLayer	=src.m_nLayer;
	m_color		=src.m_color;
	m_bDefault	=src.m_bDefault;
	m_para		=src.m_para;
	m_bLockFlag	=src.m_bLockFlag;
//	m_nCount	=src.m_nCount;
}

CLayer::CLayer(const CLayer& src)
{  	
	m_nLayer	=src.m_nLayer;
	m_color		=src.m_color;
	m_bDefault	=src.m_bDefault;
	m_bLockFlag = src.m_bLockFlag;
	m_para		=src.m_para;
//	m_nCount	=src.m_nCount;
}

void CLayer::Serialize(FILE* fp,BOOL bFlag)
{
	//本标准采用文本方式进行数据录入
	CMySerialize fs(fp);
	CString str;

	if (bFlag)
	{	
		fs<<"B_Y";
		fs<<100;
		fs<<m_nLayer;
		fs<<101;
		fs<<m_color;
		fs<<102;
		fs<<m_bDefault;
		for (int i=0; i<sizeof(PARA)/8; i++)
		{
			fs<<(200+i);
			fs<<*((double*)&m_para+i);
		}
		//写入结尾
		fs<<"E_Y";
	}
	else
	{
		while(fs>>str)
		{
			int i= atoi(str);
			switch(i)
			{
			case 0:
				if (str=="E_Y")
				{
					goto LAYSEAR_END;
				}
				break;
			case 100:
				fs>>m_nLayer;
				break;;
			case 101:
				fs>>m_color;
				break;
			case 102:
				fs>>m_bDefault;
				break;
			default:
				i-=200;
				if (i>=0 && i<sizeof(PARA)/8)	
					fs>>*((double*)&m_para+i);
				break;
			}
		}
LAYSEAR_END:
		;
	}
}

CProperty CLayer::GetProperty()
{
	CProperty pty;
	pty.m_color	=m_color;
	pty.m_nLayer=m_nLayer;
	pty.m_bLockFlag=m_bLockFlag;
	return pty;
}

void CLayer::UpdatePara()
{
	//dsf 2010.04.06
	//g_DAT.ProcRelease(m_para.dbQFreq,m_para.dbQRelease);
}

void CLayer::SetColor(COLORREF color)
{
	m_color = color;
}
