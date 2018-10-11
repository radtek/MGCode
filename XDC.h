// XDC.h: interface for the XDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XDC_H__B58828D8_A6A1_48F9_8833_10FAF3E4C1DA__INCLUDED_)
#define AFX_XDC_H__B58828D8_A6A1_48F9_8833_10FAF3E4C1DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Quad.h"

class XDC  
{
public:
	
	XDC();
	virtual ~XDC();

public:
	CDC *m_pDC;
	POINT m_ptCenter;
	double m_dbScale;
	double m_dbPhysicX;
	double m_dbPhysicY;
	double m_dx;
	double m_dy;
	int m_nResX;
	int m_nResY;
	double m_dbMaxX;
	double m_dbMinX;
	double m_dbMaxY;
	double m_dbMinY;

public:
	void Ellipse(CQuad &quad);
	void FillSolidQuad(CQuad &quad,COLORREF cor);
	void SetViewRect(double dbViewWid,double dbViewHei);
	void CalPhyRect(double dbViewWid,double dbViewHei);
	void SetPixelV(int x, int y, COLORREF crColor);
	BOOL IsPrinting();
	int  GetClipBox( LPRECT lpRect );
	COLORREF SetPixel( int x, int y, COLORREF crColor );
	CPen* SelectObject( CPen* pPen );
	CBrush* SelectObject( CBrush* pBrush );
	int SetROP2( int nDrawMode );
	HGDIOBJ SelectObject(HGDIOBJ hObject);
	void LogicToPhysic(double &x,double &y);
	void PhysicToLogic(double &x,double &y);
	double LenLogicToPhysic(double& dbVal);
	double LenPhysicToLogic(double& dbVal);
	void MoveTo(double x,double y);
	void LineTo(double x,double y);
	void SetScale(double dbVal);
	double GetScale()
	{
		return m_dbScale;
	}

private:
	BOOL IsLine(double x1,double y1,double x2,double y2,double x,double y);
	BOOL IsDraw(double& x1,double &y1,double& x2,double& y2);
	BOOL IsInRect(double x,double y);
	int FindInterSectNum(double &x1,double &y1,double &x2,double &y2);
	//上个点的位置
	double m_dbPreX;
	double m_dbPreY;
	//View大小
	double m_dbViewX;
	double m_dbViewY;
};


#endif // !defined(AFX_XDC_H__B58828D8_A6A1_48F9_8833_10FAF3E4C1DA__INCLUDED_)
