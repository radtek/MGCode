// Quad.h: interface for the CQuad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUAD_H__0BEB7285_E196_11D3_845D_444553540001__INCLUDED_)
#define AFX_QUAD_H__0BEB7285_E196_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dot.h"
#include "Stdafx.h"

#define BASE_MM 4

//定义四方体结构
typedef struct
{
	double left;
	double right;
	double top;
	double bottom;
}QUAD;

class CQuad : public QUAD
{
public:
	CQuad();
	virtual ~CQuad();

public:
	CQuad(RECT rect);
	CQuad(double l,double t,double r,double b);			//四个坐标点
	CQuad(CDot center,double width,double height);		//中心点，宽高
	CQuad(CDot dot1,CDot dot2);							//两个点
	CQuad(int base,CDot dot,double width,double height);//以某基本点，宽高确定一个矩形

public:
	void operator=(const CQuad& src);
	void operator=(const CRect& src);
	void operator*=(double ratio);
	operator CRect();
	
public:
	virtual void Compose(CQuad &quad);
	virtual BOOL Equal(CQuad quad);
	virtual BOOL Union(CQuad quad);
	virtual BOOL Include(CQuad quad);
	virtual BOOL Include(CPoint &point,CWnd *pWnd);
	virtual CDot Center();
	virtual CDot GetBaseDot(int base=BASE_MM);

	virtual void LogToTrue();
	virtual void TrueToLog();
	virtual void Move(double &MoveX,double &MoveY);
	virtual void SetOffset(double x,double y);
	virtual void Scale(CDot &dot,double &ratioX,double &ratioY);
	virtual double GetWid();
	virtual double GetLen();
	virtual void Initial();
	
	virtual BOOL Click(CDot dot, double dbR=15.0);	//拾取
	virtual BOOL DotIn(CDot dot);
	virtual void Inflate(double x,double y);
	
	virtual BOOL Include(CDot &dt);
	virtual double Height();
	virtual double Width();
	virtual void Normal();
	virtual BOOL IsEmpty();
	virtual BOOL IsNull();
	virtual BOOL Empty(){ return(left==right && top==bottom); }
	virtual void SetEmpty();
};


typedef struct tagSelQuad
{
	tagSelQuad()
	{
		m_bSel  = FALSE;
		m_bFill = FALSE;
		m_pGroupObj = NULL;
	}

	~tagSelQuad()
	{
	}

	void operator = ( const tagSelQuad & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		m_bSel   = src.m_bSel;
		m_bFill  = src.m_bFill;
        m_pGroupObj  = src.m_pGroupObj;
        m_QD     = src.m_QD;
	}
	
	BOOL    m_bSel;
	BOOL    m_bFill;
	DWORD   m_pGroupObj;
	CQuad   m_QD; 
} SELQUAD;



#endif // !defined(AFX_QUAD_H__0BEB7285_E196_11D3_845D_444553540001__INCLUDED_)
