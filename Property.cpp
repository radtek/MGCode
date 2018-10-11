// Property.cpp: implementation of the CProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Property.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CProperty,CObject,2000)

CProperty::CProperty()
{
	m_color	=BLACK;
	m_nLayer=0;
	m_bLockFlag = FALSE;
}

CProperty::~CProperty()
{
}

void CProperty::operator=(const CProperty& src)
{  	
	m_nLayer	=src.m_nLayer;
	m_color		=src.m_color;
    m_bLockFlag	=src.m_bLockFlag;
}

BOOL CProperty::operator==(const CProperty& src)
{  	
	if((m_nLayer==src.m_nLayer)&&(m_color==src.m_color)&&(m_bLockFlag==src.m_bLockFlag))
		return TRUE;
	return FALSE;
}

CProperty::CProperty(const CProperty& src)
{  	
	m_nLayer	=src.m_nLayer;
	m_color		=src.m_color;
	m_bLockFlag	=src.m_bLockFlag;
}

void CProperty::Set(int mode,CProperty &src)
{
	switch(mode)
	{
	case PTY_DEL:
		if(m_nLayer==src.m_nLayer)
		{
			m_nLayer	= 0;
			m_color		= src.m_color;
			m_bLockFlag	= src.m_bLockFlag;
		}
		break;
	case PTY_OUTLINE:
	case PTY_FILL:
	case PTY_INIT:
		m_nLayer	= src.m_nLayer;
		m_color		= src.m_color;
		m_bLockFlag	= src.m_bLockFlag;
		break;
	case PTY_LAYER:
		if(m_nLayer==src.m_nLayer)
		{
			m_color 	= src.m_color;
			m_bLockFlag	=src.m_bLockFlag;
		}
		    
		break;
	}
}

void CProperty::Serialize(FILE* fp,BOOL bFlag)
{
	//本标准采用文本方式进行数据录入
	CMySerialize fs(fp);
	CString str;

	if (bFlag)
	{
		//写入头信息
		fs<<"PRO_B";
		fs<<100;
		fs<<m_nLayer;
		fs<<101;
		fs<<m_color;
		fs<<102;
		fs<<m_bLockFlag;
		//写入尾信息
		fs<<"PRO_E";
	}
	else
	{
		while(fs>>str)
		{
			switch(atoi(str))
			{
			case 0:
				if (str=="PRO_E")
					goto PROPERTY_SERI_END;
				break;
			case 100:
				fs>>m_nLayer;
				break;
			case 101:
				fs>>m_color;
				break;
			case 102:
				fs>>m_bLockFlag;
				break;
			}
		}
PROPERTY_SERI_END:
		;
	}
}