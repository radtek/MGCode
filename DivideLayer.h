#pragma once

#include "XDC.h"

class CDivideLayer
{
public:
	CDivideLayer(void);
	~CDivideLayer(void);

public:
	void ResetDivide();
	void Draw(XDC *pDC, BOOL bSelMark);
	void SelectCell(CQuad quadSel, BOOL bSel=TRUE);
	int  GetMarkCellCount(CQuad quadDoc, int &nFirtRow);
	void SetMarkStartFlag();
	void SetMarkEndFlag();
	void Serialize(FILE* fp,BOOL bFlag);

public:
	class Cell
	{
	public:
		CQuad quad;
		BOOL  bSel;
		BOOL  bNeedMark;
		int	  nMarkStatus; //0wait, 1marking, 2marked
	};
	
	int m_nRowCount;
	int m_nColCount;
	Cell **m_pArrayCell;

public:
	int		m_nDivideType;
	int		m_nStrokeMarkOrder;	//0¡ã¡ä2??3D¨°¡ê?1¡ã¡ä???¨®¡ä¡ä?¡§?3D¨°
	double	m_dbDivideX;
	double	m_dbDivideY;
	double	m_dbDivideAddX;
	double	m_dbDivideAddY;
	double  m_dbDivideOffsetX;
	double  m_dbDivideOffsetY;

	double m_dbProductSize;
};
