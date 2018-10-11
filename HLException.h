// HLException.h: interface for the CHLException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HLEXCEPTION_H__C3963E0D_8FFC_443D_AB80_F69C6EB91985__INCLUDED_)
#define AFX_HLEXCEPTION_H__C3963E0D_8FFC_443D_AB80_F69C6EB91985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//初始化"未捕获异常"的处理框架:InitHLExceptionManager();
//抛出异常:throw new CHLException("XXX..."); catch(){e->Delete();}

void InitHLExceptionManager();

class CHLException
{
public:
	CHLException(LPCSTR strErrorInfo);
	virtual ~CHLException();

	virtual void Delete();
		
public:
	const int m_nSEHFlag;	//0xE0011000
	char m_strMessage[1024];
	char m_strSource[256];
};


class CHLMCIException : public CHLException
{
public:
	CHLMCIException(LPCSTR strErrorInfo) : CHLException(strErrorInfo) {}
};


class CHLCCDException : public CHLException
{
public:
	CHLCCDException(LPCSTR strErrorInfo) : CHLException(strErrorInfo) {}
};

#endif // !defined(AFX_HLEXCEPTION_H__C3963E0D_8FFC_443D_AB80_F69C6EB91985__INCLUDED_)
