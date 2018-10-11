#pragma once

#include "Stroke.h"

class CStroke;

class CMarkFocus
{
public:
	double dbZPos;
	int nMoveNum;
	double dbMoveDis;
};

class CStrokeList
{
public:
	CStrokeList(BOOL bAutoDeleteStroke=TRUE);
	~CStrokeList(void);
	
public:
	StrokeList m_list;
	CArray<CMarkFocus> *m_pArrayMarkFocus;
	BOOL m_bAutoDeleteStroke;

	CQuad m_quadLimit;

public:
	BOOL IsEmpty();
	void Empty();

	BOOL Move(double moveX,double moveY);
	void Rotate(CDot dot,double angle);
	void Scale(CDot dot,double ratioX,double ratioY);

	CQuad CalLimit();
	BOOL MarkNormal();
	BOOL MarkWithFocusType();

	void MarkByStrokeOrder();
	void MarkByFocusType();
};

