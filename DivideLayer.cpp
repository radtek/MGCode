#include "stdafx.h"
#include "DivideLayer.h"

CDivideLayer::CDivideLayer(void)
{
	m_dbProductSize = g_sys.m_dbProductSize;
	m_nDivideType = 0;
	m_dbDivideX = 50;
	m_dbDivideY = 50;
	m_dbDivideAddX = 0;
	m_dbDivideAddY = 0;
	m_dbDivideOffsetX = 0;
	m_dbDivideOffsetY = 0;
	m_nStrokeMarkOrder = 0;
	ResetDivide();
}


CDivideLayer::~CDivideLayer(void)
{

}

void CDivideLayer::Draw(XDC *pDC, BOOL bSelMark)
{		
	Color c(80,159,191,240);
	SolidBrush myBrush(c);
	Graphics myGraphic(pDC->m_pDC->GetSafeHdc());

	//绘制打标进度标识
	Rect rectGDI;	
	CRgn  rgnCell;
	CRect rectCell;
	CQuad quadTemp;
	CBrush newBrush;
	newBrush.CreateHatchBrush(HS_FDIAGONAL , RGB(159,191,240));
	for (int i=0;i<m_nRowCount; i++)
	{
		for(int j=0; j<m_nColCount; j++)
		{
			if (m_pArrayCell[i][j].bNeedMark && m_pArrayCell[i][j].nMarkStatus)
			{
				quadTemp = m_pArrayCell[i][j].quad;
				pDC->PhysicToLogic(quadTemp.left, quadTemp.top);
				pDC->PhysicToLogic(quadTemp.right, quadTemp.bottom);

				rectGDI.X =  static_cast<INT>(quadTemp.left);
				rectGDI.Y = static_cast<INT>(quadTemp.top);
				rectGDI.Width = static_cast<INT>(quadTemp.Width());
				rectGDI.Height = static_cast<INT>(-quadTemp.Height());

				myGraphic.FillRectangle(&myBrush, rectGDI);
			}
		}
	}
	
	//未选中单元
	DWORD dwStyle[2]={1,2};
	LOGBRUSH logBrush;
	logBrush.lbStyle=BS_SOLID;
	logBrush.lbColor=RGB(118,156,217);

	CPen penDivideLine;
	penDivideLine.CreatePen(PS_COSMETIC|PS_USERSTYLE, 1, &logBrush, 2, dwStyle);
	CPen *pOldPen = pDC->SelectObject(&penDivideLine);
	pDC->m_pDC->SetBkMode(TRANSPARENT);
	for (int i=0;i<m_nRowCount; i++)
	{
		for(int j=0; j<m_nColCount; j++)
		{
			CQuad &qudCell = m_pArrayCell[i][j].quad;

			pDC->MoveTo(qudCell.left, qudCell.top);
			pDC->LineTo(qudCell.right, qudCell.top);
			pDC->LineTo(qudCell.right, qudCell.bottom);
			pDC->LineTo(qudCell.left, qudCell.bottom);
			pDC->LineTo(qudCell.left, qudCell.top);
		}
	}

	//绘制选区中单元
	CPen redPen(PS_SOLID, 0, RGB(255,0,0));	
	pDC->SelectObject(&redPen);
	if(bSelMark)
	{
		for (int i=0;i<m_nRowCount; i++)
		{
			for(int j=0; j<m_nColCount; j++)
			{
				if (m_pArrayCell[i][j].bSel)
				{
					CQuad &qudCell = m_pArrayCell[i][j].quad;

					pDC->MoveTo(qudCell.left, qudCell.top);
					pDC->LineTo(qudCell.right, qudCell.top);
					pDC->LineTo(qudCell.right, qudCell.bottom);
					pDC->LineTo(qudCell.left, qudCell.bottom);
					pDC->LineTo(qudCell.left, qudCell.top);
				}
			}
		}
	}

	//中心点
	pDC->MoveTo(-1, 0);
	pDC->LineTo(1, 0);
	pDC->MoveTo(0, -1);
	pDC->LineTo(0, 1);
		
	CQuad quadWafer;
	quadWafer.left = quadWafer.bottom = -m_dbProductSize/2;
	quadWafer.right = quadWafer.top = m_dbProductSize/2;

	/************************* Begin *****************************************/
	// 日期：2015/4/15  时间：9:13:10 by xief     
	//pDC->Ellipse(quadWafer);
	
	/*pDC->MoveTo(quadWafer.left,quadWafer.top);
	pDC->LineTo(quadWafer.left,quadWafer.bottom);
	pDC->LineTo(quadWafer.right ,quadWafer.bottom);
	pDC->LineTo(quadWafer.right ,quadWafer.top);
	pDC->LineTo(quadWafer.left,quadWafer.top);*/

	/************************* End   *****************************************/


	pDC->SelectObject(pOldPen);
}

void CDivideLayer::ResetDivide()
{
	////Debug XJ 14:50 2014-5-4
	CString strDebug;

	int nRowStart = int((m_dbProductSize/2-m_dbDivideY/2)/m_dbDivideY)+1;
	int nRowEnd   = int((-m_dbProductSize/2+m_dbDivideY/2)/m_dbDivideY)-1;
	int nColStart = int((-m_dbProductSize/2+m_dbDivideX/2)/m_dbDivideX)-1;
	int nColEnd   = int((m_dbProductSize/2-m_dbDivideX/2)/m_dbDivideX)+1;

	m_nRowCount = nRowStart-nRowEnd+1;
	m_nColCount = nColEnd-nColStart+1;

	m_pArrayCell = new Cell*[m_nRowCount];
	for (int i=0; i<m_nRowCount; i++)
	{
		m_pArrayCell[i] = new Cell[m_nColCount];
	}

	int nRowIndex, nColIndex;
	for (int i=nRowStart;i>=nRowEnd; i--)
	{
		for(int j=nColStart; j<=nColEnd; j++)
		{
			nRowIndex = nRowStart-i;
			nColIndex = j-nColStart;

			m_pArrayCell[nRowIndex][nColIndex].quad.left   = j*m_dbDivideX - m_dbDivideX/2 - m_dbDivideOffsetX;
			m_pArrayCell[nRowIndex][nColIndex].quad.right  = m_pArrayCell[nRowIndex][nColIndex].quad.left + m_dbDivideX;

			m_pArrayCell[nRowIndex][nColIndex].quad.top    =  (i*m_dbDivideY + m_dbDivideY/2) - m_dbDivideOffsetY;
			m_pArrayCell[nRowIndex][nColIndex].quad.bottom =  m_pArrayCell[nRowIndex][nColIndex].quad.top - m_dbDivideY;

			m_pArrayCell[nRowIndex][nColIndex].bSel = FALSE;
			m_pArrayCell[nRowIndex][nColIndex].nMarkStatus = FALSE;		
			m_pArrayCell[nRowIndex][nColIndex].bNeedMark = FALSE;
		}
	}	
}

void CDivideLayer::SelectCell(CQuad quadSel, BOOL bSel)
{
	if (bSel)
	{
		for (int i=0; i<m_nRowCount; i++)
		{
			for (int j=0; j<m_nColCount; j++)
			{
				CQuad &quadCell = m_pArrayCell[i][j].quad;

				if (quadCell.left>=quadSel.left && quadCell.right<=quadSel.right &&
					quadCell.top <=quadSel.top && quadCell.bottom>=quadSel.bottom)
				{
					m_pArrayCell[i][j].bSel = TRUE;
				}
			}
		}
	}
	else
	{
		for (int i=0; i<m_nRowCount; i++)
			for (int j=0; j<m_nColCount; j++)
				m_pArrayCell[i][j].bSel = FALSE;
	}
}

int  CDivideLayer::GetMarkCellCount(CQuad quadDoc, int &nFirtRow)
{
	quadDoc.Inflate(0.001, 0.001);

	nFirtRow = 0;
	int nCount = 0;
	for (int i=0; i<m_nRowCount; i++)
	{
		for (int j=0; j<m_nColCount; j++)
		{
			CQuad &quadCell = m_pArrayCell[i][j].quad;

			if (quadDoc.left<=quadCell.right && quadDoc.right>=quadCell.left &&
				quadDoc.top>=quadCell.bottom && quadDoc.bottom<=quadCell.top)
			{				
				nCount++;
				m_pArrayCell[i][j].bNeedMark = TRUE;

				if (nFirtRow==0)
					nFirtRow = i;
			}
			else
			{
				m_pArrayCell[i][j].bNeedMark = FALSE;
			}
		}
	}
	return nCount;
}

void CDivideLayer::SetMarkStartFlag()
{
	for (int i=0; i<m_nRowCount; i++)
	{
		for (int j=0; j<m_nColCount; j++)
		{
			m_pArrayCell[i][j].nMarkStatus = 0;
		}
	}
}

void CDivideLayer::SetMarkEndFlag()
{
	for (int i=0; i<m_nRowCount; i++)
	{
		for (int j=0; j<m_nColCount; j++)
		{
			m_pArrayCell[i][j].bNeedMark = FALSE;
			m_pArrayCell[i][i].nMarkStatus = 0;
		}
	}
}

void CDivideLayer::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize sa(fp);

	CString str;
	if (bFlag)
	{
		sa<<"DIVIDE_B";	
		sa<<100;
		sa<<m_nDivideType;
		sa<<101;
		sa<<m_dbDivideX;
		sa<<102;
		sa<<m_dbDivideY;
		sa<<103;
		sa<<m_dbDivideAddX;
		sa<<104;
		sa<<m_dbDivideAddY;
		sa<<105;
		sa<<m_dbDivideOffsetX;
		sa<<106;
		sa<<m_dbDivideOffsetY;
		sa<<"DIVIDE_E";
	}
	else
	{
		while ((sa>>str))
		{
			int nID = atoi(str);
			switch (nID)
			{
			case 0:
				if (str=="DIVIDE_E")
					goto Serialize_END;
			case 100:
				sa>>m_nDivideType;
				break;
			case 101:
				sa>>m_dbDivideX;
				break;
			case 102:
				sa>>m_dbDivideY;
				break;
			case 103:
				sa>>m_dbDivideAddX;
				break;
			case 104:
				sa>>m_dbDivideAddY;
				break;
			case 105:
				sa>>m_dbDivideOffsetX;
				break;
			case 106:
				sa>>m_dbDivideOffsetY;
				break;
			}
		}
Serialize_END:
	ResetDivide();
	}

}
