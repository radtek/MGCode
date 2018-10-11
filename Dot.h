// DotB.h: interface for the CDot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOTB_H__CC5347BE_2640_407E_B6C0_DB18F575E434__INCLUDED_)
#define AFX_DOTB_H__CC5347BE_2640_407E_B6C0_DB18F575E434__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	double x;
 	double y;
	double z;
}DOT;

class CDot:public DOT
{
public:
	CDot();
	CDot(CPoint &pt);
	CDot(POINT pt);	
	CDot(double xValue,double yValue);
	virtual ~CDot();

public:
	void XChangeY();
	void TrueToLog();
	void LogToTrue();

	void Rotate(CDot center,double angle);
	void Move(double moveX,double moveY);
	void SetOffset(double xf,double yf);
	void Scale(CDot center,double ratioX,double ratioY);
	BOOL IsDotInLine(CDot& p1 , CDot& p2 );
	void Useless(){return;};
	double DistanceFromDot(CDot dot);

	void Round(int digit);

	static CDot Center(const CDot &dot1, const CDot &dot2);

public:
	void operator*=(double mul);
	void operator=(const CDot& src);
	void operator=(const DOT& src);
	operator CPoint();
	BOOL operator==(const CDot& src);

	BOOL m_bScale;

};

#endif // !defined(AFX_DOTB_H__CC5347BE_2640_407E_B6C0_DB18F575E434__INCLUDED_)
