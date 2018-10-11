// MeasureLine.h: interface for the CMeasureLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEASURELINE_H__C535B2DA_3724_453E_ACA5_FB37AAE7E0CF__INCLUDED_)
#define AFX_MEASURELINE_H__C535B2DA_3724_453E_ACA5_FB37AAE7E0CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMeasureLine  
{
public:
	enum ENU_LINETYPE
	{
		LEFT_TO_RIGHT,
		TOP_TO_BOTTOM,
	};
	CMeasureLine();
	virtual ~CMeasureLine(){}
public:
	double m_dbPos;
	UINT m_nType;			//测量线类型，横或竖
};

#endif // !defined(AFX_MEASURELINE_H__C535B2DA_3724_453E_ACA5_FB37AAE7E0CF__INCLUDED_)
