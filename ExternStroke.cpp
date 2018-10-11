// ExternStroke.cpp: implementation of the CExternStroke class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "ExternStroke.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExternStroke::CExternStroke()
{
	
}

CExternStroke::~CExternStroke()
{
	
}

BOOL CExternStroke::IsOutStroke(const CStroke &stroke, const StrokeList &strokeList, POSITION indexPos)
{
	POSITION pos=strokeList.GetHeadPosition();
	while (pos!=NULL)
	{
		if(pos == indexPos) 
		{
			strokeList.GetNext(pos);
			continue;
		}
		CStroke *pStroke=strokeList.GetNext(pos);
		if(IsStrokeInStroke(stroke, *pStroke)) 
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CExternStroke::DoExtern(const CStroke &strokeIn, CStroke &strokeOut, double dbSpace, BOOL bOpt)
{
	CArray<CDot, CDot> dotArray;
	dotArray.Copy(strokeIn.m_dotArray);
	RemoveSameDot(dotArray);
	RemoveMidDotInLine(dotArray);//把三个在一直线的点中间的中心点去掉
	int count = dotArray.GetSize();
	CDot dotLeft, dotMid, dotRight;
	CDot dotOut;
	for (int i=0; i<count-1; i++)
	{
		if (i-1==-1) dotLeft = dotArray.GetAt(count-2);			
		else dotLeft = dotArray.GetAt(i-1);
		dotMid = dotArray.GetAt(i);
		dotRight = dotArray.GetAt(i+1);
		GetExternDot(dotLeft, dotMid, dotRight, dotOut, dbSpace, dotArray);
		strokeOut.m_dotArray.Add(dotOut);
	}
	if(bOpt) TrimDot(dotArray, strokeOut.m_dotArray, dbSpace);
	if(strokeOut.m_dotArray.GetSize() != 0)
	{
		dotOut = strokeOut.m_dotArray.GetAt(0);
		strokeOut.m_dotArray.Add(dotOut);
	}
}
//相似三角形求缩扩的距离
/*
/|\
/ | \c2
/  |c1\
/   | / \
/    |/a1 \
-----------
a2
c1垂直于a2
a1垂直于c2
*/
void CExternStroke::GetExternDot(const CDot &dotLeft, const CDot &dotMid, const CDot &dotRight, 
								 CDot &dotOut, double dbSpace, const CArray<CDot, CDot> &dotArray)
{
	CDot tempDotLeft,tempDotRight,tempDotCenter;
	tempDotLeft = dotLeft;
	tempDotRight = dotRight;
	GetEqualLenAngle(tempDotLeft, dotMid, tempDotRight);
	tempDotCenter.x = (tempDotLeft.x+tempDotRight.x)/2;
	tempDotCenter.y = (tempDotLeft.y+tempDotRight.y)/2;
	//相似三角形求缩扩的距离C1
	double a1 = fabs(dbSpace);
	double a2 = Distance(tempDotLeft, tempDotCenter);
	double c2 = Distance(tempDotLeft, dotMid);
	double c1 = c2*(a1/a2);
	///////////////////////////////////////////
	double cx,cy;
	GetCxCy(cx, cy, c1, dotMid, tempDotCenter);
	CRadialLine radialLine1(dotMid, tempDotCenter, 1);
	int count1 = GetCrossCount(radialLine1, dotArray);
	CRadialLine radialLine2(dotMid, tempDotCenter, -1);
	int count2 = GetCrossCount(radialLine2, dotArray);
	if (count1%2==0 && count2%2==1)
	{//tempDotCenter方向是外扩
		if (dbSpace > 0)
		{
			dotOut.x=(tempDotCenter.x>dotMid.x)?(dotMid.x+cx):(dotMid.x-cx);
			dotOut.y=(tempDotCenter.y>dotMid.y)?(dotMid.y+cy):(dotMid.y-cy);
		}
		else
		{
			dotOut.x=(tempDotCenter.x<dotMid.x)?(dotMid.x+cx):(dotMid.x-cx);
			dotOut.y=(tempDotCenter.y<dotMid.y)?(dotMid.y+cy):(dotMid.y-cy);
		}
	}
	else //if(count1%2==1 && count2%2==0)
	{//tempDotCenter方向是内缩
		if (dbSpace > 0)
		{
			dotOut.x=(tempDotCenter.x<dotMid.x)?(dotMid.x+cx):(dotMid.x-cx);
			dotOut.y=(tempDotCenter.y<dotMid.y)?(dotMid.y+cy):(dotMid.y-cy);
		}
		else
		{
			dotOut.x=(tempDotCenter.x>dotMid.x)?(dotMid.x+cx):(dotMid.x-cx);
			dotOut.y=(tempDotCenter.y>dotMid.y)?(dotMid.y+cy):(dotMid.y-cy);
		}
	}
	//else
	{
		//DSF_TRACE("Error in GetExternDot");
	}
}

void CExternStroke::RemoveSameDot(CArray<CDot, CDot> &dotArray)
{
	//过滤重点
	CDot dot1,dot2;
	int size = dotArray.GetSize();
	while(--size)
	{
		if (size-1<0)
		{
			break;
		}
		dot1 = dotArray[size];
		dot2 = dotArray[size-1];
		if(dot1==dot2)
		{
			dotArray.RemoveAt(size);
		}
	}
}

inline BOOL CExternStroke::IsLine(const CDot &dotLeft, const CDot &dotMid, const CDot &dotRight)
{
	double cx1 = dotLeft.x - dotMid.x;
	double cy1 = dotLeft.y - dotMid.y;
	double cx2 = dotMid.x - dotRight.x;
	double cy2 = dotMid.y - dotRight.y;
	if (SAMEVALUE(cx1,0) && SAMEVALUE(cx2,0))
	{
		return TRUE;
	}
	else if(SAMEVALUE(cx1,0) || SAMEVALUE(cx2,0))
	{
		return FALSE;
	}
	else
	{
		return SAMEVALUE(cy1/cx1, cy2/cx2);
	}
}

void CExternStroke::RemoveMidDotInLine(CArray<CDot, CDot> &dotArray)
{
	CArray<int,int> indexArray;
	int count = dotArray.GetSize();
	CDot dotLeft, dotMid, dotRight;
	for (int i=0; i<count-1; i++)
	{
		if (i==0) dotLeft = dotArray.GetAt(count-2);			
		else dotLeft = dotArray.GetAt(i-1);
		dotMid = dotArray.GetAt(i);
		dotRight = dotArray.GetAt(i+1);
		if (IsLine(dotLeft, dotMid, dotRight))
		{
			indexArray.Add(i);
		}
	}
	int size = indexArray.GetSize();
	for (int j=size-1; j>=0; j--)
	{
		int index = indexArray.GetAt(j);
		dotArray.RemoveAt(index);
	}
}

inline double CExternStroke::Distance(const CDot dot1, const CDot dot2)
{
	double cx = dot2.x - dot1.x;
	double cy = dot2.y - dot1.y;
	return sqrt(cx*cx+cy*cy);
}

inline void CExternStroke::GetEqualLenAngle(CDot &dotLeft, const CDot &dotMid, CDot &dotRight)
{
	CDot tempDotLeft,tempDotRight;
	tempDotLeft = dotLeft;
	tempDotRight = dotRight;
	double disLeft = Distance(dotLeft, dotMid);
	double disRight = Distance(dotMid, dotRight);
	if (disLeft > disRight)
	{
		tempDotRight = dotRight;
		if (SAMEVALUE(dotLeft.x,dotMid.x))
		{
			tempDotLeft.x = dotMid.x;
			tempDotLeft.y = (dotLeft.y<dotMid.y)?(dotMid.y-disRight):(dotMid.y+disRight);
		}
		else if(SAMEVALUE(dotLeft.y, dotMid.y))
		{
			tempDotLeft.x = (dotLeft.x<dotMid.x)?(dotMid.x-disRight):(dotMid.x+disRight);
			tempDotLeft.y = dotMid.y;
		}
		else
		{
			double cx = disRight*(fabs(dotLeft.x-dotMid.x)/disLeft);
			double cy = disRight*(fabs(dotLeft.y-dotMid.y)/disLeft);
			tempDotLeft.x = (dotLeft.x<dotMid.x)?(dotMid.x-cx):(dotMid.x+cx);
			tempDotLeft.y = (dotLeft.y<dotMid.y)?(dotMid.y-cy):(dotMid.y+cy);
		}
	}
	else
	{
		tempDotLeft = dotLeft;
		if (SAMEVALUE(dotRight.x,dotMid.x))
		{
			tempDotRight.x = dotMid.x;
			tempDotRight.y = (dotRight.y<dotMid.y)?(dotMid.y-disLeft):(dotMid.y+disLeft);
		}
		else if(SAMEVALUE(dotRight.y, dotMid.y))
		{
			tempDotRight.x = (dotRight.x<dotMid.x)?(dotMid.x-disLeft):(dotMid.x+disLeft);
			tempDotRight.y = dotMid.y;
		}
		else
		{
			double cx = disLeft*(fabs(dotRight.x-dotMid.x)/disRight);
			double cy = disLeft*(fabs(dotRight.y-dotMid.y)/disRight);
			tempDotRight.x = (dotRight.x<dotMid.x)?(dotMid.x-cx):(dotMid.x+cx);
			tempDotRight.y = (dotRight.y<dotMid.y)?(dotMid.y-cy):(dotMid.y+cy);
		}
	}
	dotLeft = tempDotLeft;
	dotRight = tempDotRight;
}

inline void CExternStroke::GetCxCy(double &cx, double &cy, double c1, const CDot &dot1, const CDot &dot2)
{
	if (SAMEVALUE(dot1.x, dot2.x))
	{
		cx = 0;
		cy = c1;
	}
	else if (SAMEVALUE(dot1.y, dot2.y))
	{
		cx = c1;
		cy = 0;
	}
	else
	{
		double dis = Distance(dot1, dot2);
		cx = c1*(fabs(dot1.x-dot2.x)/dis);
		cy = c1*(fabs(dot1.y-dot2.y)/dis);
	}
}

CExternStroke::CRadialLine::CRadialLine(const CDot &beginDot, const CDot &dot2, int nDirection)
{
	if (SAMEVALUE(beginDot.x, dot2.x))
	{
		this->isEqualX = TRUE;
		this->b = beginDot.x;
	}
	else
	{
		this->isEqualX = FALSE;
		this->k = (dot2.y-beginDot.y)/(dot2.x-beginDot.x);
		this->b = dot2.y - k*dot2.x;
	}
	this->beginX = beginDot.x;
	this->beginY = beginDot.y;
	this->isXbig = dot2.x > beginDot.x;
	this->isYbig = dot2.y > beginDot.y;
	if (nDirection < 0)
	{
		this->isXbig = !this->isXbig;
		this->isYbig = !this->isYbig;
	}
}

CExternStroke::CRadialLine::~CRadialLine()
{
	
}

CExternStroke::CSegmentLine::CSegmentLine(const CDot &dot1, const CDot &dot2)
{
	isCrossAtDot = FALSE;
	this->dot1 = dot1;
	this->dot2 = dot2;
	if (SAMEVALUE(dot1.x, dot2.x))
	{
		this->isEqualX = TRUE;
		this->b = dot1.x;
	}
	else
	{
		this->isEqualX = FALSE;
		this->k = (dot2.y-dot1.y)/(dot2.x-dot1.x);
		this->b = dot2.y - k*dot2.x;
	}
	if (dot1.x > dot2.x)
	{
		this->minX = dot2.x;
		this->maxX = dot1.x;
	}
	else
	{
		this->minX = dot1.x;
		this->maxX = dot2.x;
	}
	if (dot1.y > dot2.y)
	{
		this->minY = dot2.y;
		this->maxY = dot1.y;
	}
	else
	{	
		this->minY = dot1.y;
		this->maxY = dot2.y;
	}
}

CExternStroke::CSegmentLine::~CSegmentLine()
{
	
}

BOOL CExternStroke::CRadialLine::IsCross(CSegmentLine &segment)
{
	if (this->isEqualX)
	{
		if (segment.isEqualX)
		{
			return SAMEVALUE(this->b, segment.b);
		}
		else
		{
			CDot tempDot;
			tempDot.x = this->b;
			tempDot.y = segment.k*tempDot.x + segment.b;
			return (this->DotInLine(tempDot)&&(segment.DotInLine(tempDot)));
		}
	}
	else
	{
		if (segment.isEqualX)
		{
			CDot tempDot;
			tempDot.x = segment.b;
			tempDot.y = this->k*tempDot.x + this->b;
			return (this->DotInLine(tempDot)&&segment.DotInLine(tempDot));
		}
		else
		{
			if (SAMEVALUE(this->k, segment.k))
			{
				return SAMEVALUE(this->b, segment.b);
			}
			else
			{
				CDot tempDot;
				tempDot.x = (segment.b-this->b)/(this->k-segment.k);
				tempDot.y = this->k*tempDot.x + this->b;
				return (this->DotInLine(tempDot)&&segment.DotInLine(tempDot));
			}
		}
	}
}

//因为传进来的点是通过线条相交求出来的，
//所以不需要额外判断是否在线条上,
//只需求判断在不在范围
BOOL CExternStroke::CRadialLine::DotInLine(const CDot &dot)
{
	if(SAMEVALUE(beginX, dot.x) && SAMEVALUE(beginY, dot.y)) return FALSE;
	if (this->isEqualX)
	{
		if(this->isYbig) return (dot.y>this->beginY);
		else return (dot.y<this->beginY);
	}
	else
	{
		if(this->isXbig) return (dot.x>this->beginX);
		else return (dot.x<this->beginX);
	}
}

BOOL CExternStroke::CSegmentLine::DotInLine(const CDot &dot)
{
	if(SAMEVALUE(dot1.x, dot.x) && SAMEVALUE(dot1.y, dot.y)) 
	{
		isCrossAtDot = TRUE;
		return TRUE;
	}
	if(SAMEVALUE(dot2.x, dot.x) && SAMEVALUE(dot2.y, dot.y)) return FALSE;
	if (this->isEqualX)
	{
		return ((dot.y>=this->minY)&&(dot.y<this->maxY));
	}
	else
	{
		return ((dot.x>=this->minX)&&(dot.x<this->maxX));
	}
}

int CExternStroke::GetCrossCount(CRadialLine radialLine, const CArray<CDot, CDot> &dotArray)
{
	CDot beginDot(radialLine.beginX, radialLine.beginY);
	int count = 0;
	int size = dotArray.GetSize();
	CDot tempDot = dotArray.GetAt(size-2);
	for (int i=0; i<size-1; i++)
	{
		CDot dot1 = dotArray.GetAt(i);
		CDot dot2;
		if(i+1==size-1) dot2=dotArray.GetAt(0);
		else dot2=dotArray.GetAt(i+1);
		if(dot1==beginDot || dot2==beginDot) continue;
		CSegmentLine segmentLine(dot1, dot2);
// 		CString strTemp;
// 		strTemp.Format("%f,%f,%f,%f\r\n",dot1.x,dot1.y,dot2.x,dot2.y);
// 		DSF_TRACE(strTemp);
		if (radialLine.IsCross(segmentLine) && !segmentLine.isCrossAtDot)
		{
			count++;
		}
		else if(segmentLine.isCrossAtDot)
		{
			//判断两个点是否分别在直线的两侧
			//CSegmentLine segmentLineTemp(tempDot, /*dot2*/dot2);
			if (!radialLine.IsInOneSide(tempDot, dot2))
			{
				count++;
			}
		}
// 		if (radialLine.IsCross(segmentLine))
// 		{
// 			DSF_TRACE("");
// 		}
		tempDot = dot1;
	}
	return count;
}

void CExternStroke::ClearDifferent(const CStroke &strokeIn, CStroke &strokeOut)
{
	int count = strokeOut.m_dotArray.GetSize();
	for (int i=0; i<count-1; i++)
	{
		CDot dot1 = strokeIn.m_dotArray.GetAt(i);
		CDot dot2 = strokeIn.m_dotArray.GetAt(i+1);
		CDot &dot3 = strokeOut.m_dotArray.ElementAt(i);
		CDot &dot4 = strokeOut.m_dotArray.ElementAt(i+1);
		double cx1 = dot2.x-dot1.x;
		double cy1 = dot2.y-dot1.y;
		double cx2 = dot4.x-dot3.x;
		double cy2 = dot4.y-dot3.y;
		if (SAMEVALUE(cx1,0)&&SAMEVALUE(cx2,0))
		{
			if (cy1*cy2<0)
			{
				SetDotToCenter(dot3, dot4);
			}
			else
			{
				continue;
			}
		}
		else if (SAMEVALUE(cx1,0)||SAMEVALUE(cx2,0))
		{
			SetDotToCenter(dot3, dot4);
		}
		else if(SAMEVALUE(cy1/cx1,cy2/cx2))
		{
			if (cx1*cx2<0 || cy1*cy2<0)
			{
				SetDotToCenter(dot3, dot4);
			}
			else
			{
				continue;
			}
		}
		else
		{
			SetDotToCenter(dot3, dot4);
		}
	}
}

inline void CExternStroke::SetDotToCenter(CDot &dot1, CDot &dot2)
{
	CDot tempDot;
	tempDot.x = (dot1.x+dot2.x)/2;
	tempDot.y = (dot1.y+dot2.y)/2;
	dot1.x = tempDot.x;
	dot1.y = tempDot.y;
	dot2.x = tempDot.x;
	dot2.y = tempDot.y;
}

BOOL CExternStroke::ClearAngleShort(const CArray<CDot, CDot> &dotArray, CArray<CDot, CDot> &dstDotArray, double dbSpace)
{
	DSF_TRACE("Begin ClearAngleShort\r\n");
	double fabDbSpace = fabs(dbSpace);
	CArray<int,int> indexArray;
	int count = dotArray.GetSize();
	CDot dotLeft, dotAngle1, dotAngle2, dotRight;
	for (int i=0; i<count-1; i++)
	{	
		if (i==0) 
		{
			dotLeft = dotArray.GetAt(count-2);
		}
		else 
		{
			dotLeft = dotArray.GetAt(i-1);
		}
		dotAngle1 = dotArray.GetAt(i);
		if(i==count-2) 
		{
			dotAngle2 = dotArray.GetAt(0);
			dotRight = dotArray.GetAt(1);
		}
		else
		{
			dotAngle2 = dotArray.GetAt(i+1);
			dotRight = dotArray.GetAt(i+2);
		}
		double a = Distance(dotLeft, dotAngle1);
		double b = Distance(dotAngle1, dotAngle2);
		double c = Distance(dotAngle2, dotLeft);
		double angle1 = acos((a*a+b*b-c*c)/(2*a*b));
		a = b;//Distance(dotAngle1, dotAngle2)
		b = Distance(dotAngle2, dotRight);
		c = Distance(dotAngle1, dotRight);
		double angle2 = acos((a*a+b*b-c*c)/(2*a*b));
		//if (angle1>PAI/2 || angle2>PAI/2)
		{
			BOOL bIsInAngle1 = IsInAngle(dotLeft, dotAngle1, dotAngle2, dotArray);
			BOOL bIsInAngle2 = IsInAngle(dotAngle1, dotAngle2, dotRight, dotArray);
			BOOL condition1 = ((bIsInAngle1&&bIsInAngle2)||
				(bIsInAngle1&&angle2>angle1)||(bIsInAngle2&&angle1>angle2)) && dbSpace<0;
			BOOL condition2 = ((!bIsInAngle1&&!bIsInAngle2)||
				(!bIsInAngle1&&angle2>angle1)||(!bIsInAngle2&&angle1>angle2)) && dbSpace>0;
			if(condition1 || condition2)
			{
				double angleA = angle1/2;
				double angleB = angle2/2;
				double angleC = PAI-angleA-angleB;
				c = a;//Distance(dotAngle1, dotAngle2)
				b = (c*(1/cos(angleB)+cos(angleA)/cos(angleC)))/(1/cos(angleC)+cos(angleA)/cos(angleB));
				//a = c/cos(angleB)-b*(cos(angleA)/cos(angleB));
				double d = b*sin(angleA);
				if (d < fabDbSpace)
				{
					//int index = (angle1<angle2)?(i+1):i;
					//indexArray.Add(index);
					indexArray.Add(i);
					if (i == count-2) indexArray.Add(0);
					else indexArray.Add(i+1);
				}
			}
		}
	}
	ClearSameIndex(indexArray, dstDotArray);
	int size = indexArray.GetSize();
	int dstSize = dstDotArray.GetSize();
	int aIndex,bIndex,cIndex,dIndex;
	CDot dotA,dotB,dotC,dotD;
	if (size > 0)
	{
		BOOL bRemoveStart = FALSE;
		for (int i=size-2; i>=0; i-=2)
		{
			bIndex = indexArray.GetAt(i);
			cIndex = indexArray.GetAt(i+1);
			if(bIndex==0) aIndex=dstSize-2;
			else aIndex = bIndex-1;
			if (cIndex==dstSize-2) dIndex=0;
			else dIndex = cIndex+1;
			dotA = dstDotArray.GetAt(aIndex);
			dotB = dstDotArray.GetAt(bIndex);
			dotC = dstDotArray.GetAt(cIndex);
			dotD = dstDotArray.GetAt(dIndex);
			CDot dotCross = GetCrossDot(dotA, dotB, dotC, dotD);
			if (cIndex == 0)
			{
				dstDotArray.RemoveAt(dstSize-2);
				dstDotArray.RemoveAt(0);
				dstDotArray.InsertAt(0, dotCross);
				bRemoveStart = TRUE;
			}
			else
			{
				dstDotArray.RemoveAt(cIndex);
				dstDotArray.RemoveAt(bIndex);
				dstDotArray.InsertAt(bIndex, dotCross);
			}
		}
		dstSize = dstDotArray.GetSize();
		CDot beginDot = dstDotArray.GetAt(0);
		CDot &endDot = dstDotArray.ElementAt(dstSize-1);
		endDot = beginDot;
	}
	DSF_TRACE("End ClearAngleShort\r\n");
	return TRUE;
}

//三角形开口向里
BOOL CExternStroke::IsInAngle(CDot &dotLeft, const CDot &dotMid, CDot &dotRight, 
							  const CArray<CDot, CDot> &dotArray)
{
	CDot tempDotLeft,tempDotRight,tempDotCenter;
	tempDotLeft = dotLeft;
	tempDotRight = dotRight;
	GetEqualLenAngle(tempDotLeft, dotMid, tempDotRight);
	tempDotCenter.x = (tempDotLeft.x+tempDotRight.x)/2;
	tempDotCenter.y = (tempDotLeft.y+tempDotRight.y)/2;
	CRadialLine radialLine1(dotMid, tempDotCenter, 1);
	int count1 = GetCrossCount(radialLine1, dotArray);
	return (count1%2==1);
}

BOOL CExternStroke::IsStrokeInStroke(const CStroke &stroke1, const CStroke &stroke2)
{
	CArray<CDot, CDot> dotArray1;
	CArray<CDot, CDot> dotArray2;
	dotArray1.Copy(stroke1.m_dotArray);
	dotArray2.Copy(stroke2.m_dotArray);
	RemoveSameDot(dotArray1);
	RemoveMidDotInLine(dotArray1);//把三个在一直线的点中间的中心点去掉
	RemoveSameDot(dotArray2);
	RemoveMidDotInLine(dotArray2);//把三个在一直线的点中间的中心点去掉
	int size = dotArray1.GetSize();
	for (int i=0; i<size-1; i++)
	{
		CDot dot1 = dotArray1.GetAt(i);
		if(DotInRegion(dotArray2, dot1) != -1) 
			return FALSE;
	}
	int size2 = dotArray2.GetSize();
	for (int j=0; j<size2-1; j++)
	{
		CDot dot2 = dotArray2.GetAt(j);
		if(DotInRegion(dotArray1, dot2) != 1) 
			return FALSE;
	}
	return TRUE;
}

int CExternStroke::DotInRegion(const CArray<CDot, CDot> &dotArray, CDot dot)
{
	int n = dotArray.GetSize() - 1;
	int i,j,wn=0;
	double k;
	for(i=n-1, j=0; j<n; i=j, j++)
	{
		k = (dot.x - dotArray[i].x) * (dotArray[j].y - dotArray[i].y) - 
			(dotArray[j].x - dotArray[i].x) * (dot.y - dotArray[i].y);
		if((dot.y >= dotArray[i].y && dot.y <= dotArray[j].y)||
			(dot.y <= dotArray[i].y && dot.y >= dotArray[j].y))
		{
			if( k < 0) 
			{
				wn++;
			}
			else if(k > 0) 
			{
				wn--;
			}
			else
			{
				if( (dot.y <= dotArray[i].y && dot.y >= dotArray[j].y && 
					dot.x <= dotArray[i].x && dot.x >= dotArray[j].x) ||
					(dot.y <= dotArray[i].y && dot.y >= dotArray[j].y && 
					dot.x >= dotArray[i].x && dot.x <= dotArray[j].x) ||
					(dot.y >= dotArray[i].y && dot.y <= dotArray[j].y && 
					dot.x <= dotArray[i].x && dot.x >= dotArray[j].x) ||
					(dot.y >= dotArray[i].y && dot.y <= dotArray[j].y && 
					dot.x >= dotArray[i].x && dot.x <= dotArray[j].x) )
				{
					return 0; //点在多边形边界上
				}
			}
		}
	}
	if(wn == 0)
	{
		return 1; //点在多边形外部
	}
	else
	{
		return -1; //点在多边形内部
	}
}

void CExternStroke::ClearSameIndex(CArray<int, int> &indexArray, CArray<CDot, CDot> &dstDotArray)
{
	DSF_TRACE("Begin ClearSameIndex\r\n");
	int count = indexArray.GetSize();
	while (--count)
	{
		if (count-1<0)
		{
			break;
		}
		else
		{
			int i = indexArray.GetAt(count);
			int j = indexArray.GetAt(count-1);
			if (i == j)
			{
				indexArray.RemoveAt(count);
				indexArray.RemoveAt(count-1);
				dstDotArray.RemoveAt(i);
				count--;
			}
		}
	}
	DSF_TRACE("End ClearSameIndex Loop\r\n");
	int size = indexArray.GetSize();
	if(size==0) return;
	int i = indexArray.GetAt(0);
	int j = indexArray.GetAt(size-1);
	if (i+j==0)
	{
		indexArray.RemoveAt(size-1);
		indexArray.RemoveAt(0);
		int temp = dstDotArray.GetSize();
		dstDotArray.RemoveAt(temp-1);
		dstDotArray.RemoveAt(0);
	}
	DSF_TRACE("End ClearSameIndex\r\n");
}

CDot CExternStroke::GetCrossDot(CDot dotA, CDot dotB, CDot dotC, CDot dotD)
{
	CLine line1(dotA, dotB);
	CLine line2(dotC, dotD);
	return line1.GetCrossDot(line2);
}

CExternStroke::CLine::CLine(const CDot &dot1, const CDot &dot2)
{
	this->dot1 = dot1;
	this->dot2 = dot2;
	if (SAMEVALUE(dot1.x, dot2.x))
	{
		this->isEqualX = TRUE;
		this->b = dot1.x;
	}
	else
	{
		this->isEqualX = FALSE;
		k = (dot1.y-dot2.y)/(dot1.x-dot2.x);
		b = dot1.y - k*dot1.x;
	}
}

CExternStroke::CLine::~CLine()
{
	
}

CDot CExternStroke::CLine::GetCrossDot(const CLine &line)
{
	CDot tempDot;
	if (this->isEqualX)
	{
		if (line.isEqualX)
		{
			tempDot.x = line.b;
			CDot mid1,mid2;
			mid1.x = (this->dot1.x+this->dot2.x)/2;
			mid1.y = (this->dot1.y+this->dot2.y)/2;
			mid2.x = (line.dot1.x+line.dot2.x)/2;
			mid2.y = (line.dot1.y+line.dot2.y)/2;
			tempDot.y = (mid1.y+mid2.y)/2;
		}
		else
		{
			tempDot.x = this->b;
			tempDot.y = line.k*tempDot.x + line.b;
		}
	}
	else
	{
		if (line.isEqualX)
		{
			tempDot.x = line.b;
			tempDot.y = this->k*tempDot.x + this->b;
		}
		else
		{
			if (SAMEVALUE(this->k, line.k))
			{
				CDot mid1,mid2;
				mid1.x = (this->dot1.x+this->dot2.x)/2;
				mid1.y = (this->dot1.y+this->dot2.y)/2;
				mid2.x = (line.dot1.x+line.dot2.x)/2;
				mid2.y = (line.dot1.y+line.dot2.y)/2;
				tempDot.x = (mid1.x+mid2.x)/2;
				tempDot.y = (mid1.y+mid2.y)/2;
			}
			else
			{
				tempDot.x = (line.b-this->b)/(this->k-line.k);
				tempDot.y = this->k*tempDot.x + this->b;
			}
		}
	}
	return tempDot;
}

BOOL CExternStroke::CRadialLine::IsCross(const CLine &line)
{
	if (this->isEqualX)
	{
		if (line.isEqualX)
		{
			return SAMEVALUE(this->b, line.b);
		}
		else
		{
			CDot tempDot;
			tempDot.x = this->b;
			tempDot.y = line.k*tempDot.x + line.b;
			return this->DotInLine(tempDot);
		}
	}
	else
	{
		if (line.isEqualX)
		{
			CDot tempDot;
			tempDot.x = line.b;
			tempDot.y = this->k*tempDot.x + this->b;
			return this->DotInLine(tempDot);
		}
		else
		{
			if (SAMEVALUE(this->k, line.k))
			{
				return SAMEVALUE(this->b, line.b);
			}
			else
			{
				CDot tempDot;
				tempDot.x = (line.b-this->b)/(this->k-line.k);
				tempDot.y = this->k*tempDot.x + this->b;
				return this->DotInLine(tempDot);
			}
		}
	}
}

BOOL CExternStroke::MidAngleLineCrossShort(int index1, int index2, double dbSpace, const CArray<CDot, CDot> &dotArray)
{
	CDot dotLeft1, dotAngle1, dotRight1;
	CDot dotLeft2, dotAngle2, dotRight2;
	int size = dotArray.GetSize();
	
	if(index1-1<0) dotLeft1=dotArray.GetAt(size-2);
	else dotLeft1=dotArray.GetAt(index1-1);
	dotAngle1 = dotArray.GetAt(index1);
	if(index1+1>size-2) dotRight1=dotArray.GetAt(0);
	else dotRight1=dotArray.GetAt(index1+1);
	
	if(index2-1<0) dotLeft2=dotArray.GetAt(size-2);
	else dotLeft2=dotArray.GetAt(index2-1);
	dotAngle2 = dotArray.GetAt(index2);
	if(index2+1>size-2) dotRight2=dotArray.GetAt(0);
	else dotRight2=dotArray.GetAt(index2+1);
	
	double a1 = Distance(dotAngle1, dotLeft1);
	double b1 = Distance(dotAngle1, dotRight1);
	double c1 = Distance(dotLeft1, dotRight1);
	double angle1 = acos((a1*a1+b1*b1-c1*c1)/(2*a1*b1));
	
	//double a2 = Distance(dotAngle2, dotLeft2);
	//double b2 = Distance(dotAngle2, dotRight2);
	//double c2 = Distance(dotLeft2, dotRight2);
	//double angle2 = acos((a2*a2+b2*b2-c2*c2)/(2*a2*b2));
	
	BOOL bIsInAngle1 = IsInAngle(dotLeft1, dotAngle1, dotRight1, dotArray);
	//BOOL bIsInAngle2 = IsInAngle(dotLeft2, dotAngle2, dotRight2, dotArray);
	
	GetEqualLenAngle(dotLeft1, dotAngle1, dotRight1);
	GetEqualLenAngle(dotLeft2, dotAngle2, dotRight2);
	CDot dotAngleTemp1, dotAngleTemp2;
	dotAngleTemp1.x = (dotLeft1.x+dotRight1.x)/2;
	dotAngleTemp1.y = (dotLeft1.y+dotRight1.y)/2;
	dotAngleTemp2.x = (dotLeft2.x+dotRight2.x)/2;
	dotAngleTemp2.y = (dotLeft2.y+dotRight2.y)/2;
	
	BOOL bFlag = (bIsInAngle1&&(dbSpace<0))||(!bIsInAngle1&&(dbSpace>0));
	CRadialLine rLine(dotAngle1, dotAngleTemp1, (bFlag?1:-1));
	CLine line(dotAngle2, dotAngleTemp2);
	if(rLine.IsCross(line))
	{
		CLine tempLine(dotAngle1, dotAngleTemp1);
		CDot dot = line.GetCrossDot(tempLine);
		double d = fabs(Distance(dot, dotAngle1)*sin(angle1/2));
		if (d<fabs(dbSpace))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CExternStroke::ClearExceptionalDot(const CArray<CDot, CDot> &dotArray, CArray<CDot, CDot> &dstDotArray, double dbSpace)
{
	CArray<int,int> indexArray;
	int srcSize = dotArray.GetSize();
	for (int i=0; i<srcSize-1; i++)
	{
		for (int j=0; j<srcSize-1; j++)
		{
			if(j==i) continue;
			if (MidAngleLineCrossShort(i, j, dbSpace, dotArray))
			{
				indexArray.Add(i);
				break;
			}
		}
	}
	CArray<CDot*, CDot*> pDotArray;
	int dstSize = dstDotArray.GetSize();
	for (int k=0; k<dstSize-1; k++)
	{
		CDot *pDot = new CDot;
		*pDot = dstDotArray.GetAt(k);
		pDotArray.Add(pDot);
	}
	for (int m=0; m<indexArray.GetSize(); m++)
	{
		int index = indexArray.GetAt(m);
		CDot *&pDot = pDotArray.ElementAt(index);
		delete pDot;
		pDot = NULL;
	}
	dstDotArray.RemoveAll();
	for (int n=0; n<pDotArray.GetSize(); n++)
	{
		CDot *pDot = pDotArray.GetAt(n);
		if(pDot!= NULL) dstDotArray.Add(*pDot);
	}
	dstDotArray.Add(dstDotArray.GetAt(0));
	for (int n=0; n<pDotArray.GetSize(); n++)
	{
		CDot *pDot = pDotArray.GetAt(n);
		if(pDot!=NULL) delete pDot;
	}
}

void CExternStroke::TrimDot(const CArray<CDot, CDot> &dotArray, CArray<CDot, CDot> &dstDotArray, double dbSpace)
{
	m_arrayExceptDot.RemoveAll();
	CList<int, int> indexList;
	int srcSize = dotArray.GetSize();
	for (int i=0; i<srcSize-1; i++)
	{
		int j = i;
		int k = (i==srcSize-2)?0:i+1;
		BOOL bFlag = MidAngleLineCrossShort(j, k, dbSpace, dotArray);
		if (bFlag)
		{
			AddToIndexList(indexList, j);
			AddToIndexList(indexList, k);
			int indexUp1 = AddUpIndex(indexList, j, dbSpace, dotArray);
			int indexDown1 = AddDownIndex(indexList, j, dbSpace, dotArray);
			int indexUp2 = AddUpIndex(indexList, k, dbSpace, dotArray);
			int indexDown2 = AddDownIndex(indexList, k, dbSpace, dotArray);
			AddExcludeIndex(indexList, indexUp1, j, indexDown1, dbSpace, dotArray);
			AddExcludeIndex(indexList, indexUp2, k, indexDown2, dbSpace, dotArray);
		}
	}
	if (indexList.GetCount() >= srcSize-1)
	{
		dstDotArray.RemoveAll();
		return;
	}
	if (indexList.GetCount() <= 0)
	{
		return;
	}
	SortIndexList(indexList);
	int nCount = dstDotArray.GetSize();

	if (indexList.GetHead()==0 && indexList.GetTail()==nCount-1)
	{
		int index = nCount-1;
		while (index==indexList.GetTail())
		{
			indexList.RemoveTail();
			indexList.InsertBefore(indexList.GetHeadPosition(), index-nCount);
			index--;
		}
	}
	while(indexList.GetCount() > 0)
	{
		DSF_TRACE("1");
		BOOL bIsNotCrossShort = FALSE;
		CDot dot1, dot2;
		CDot maxDot, minDot;
		int maxIndex, minIndex;
		POSITION pos = indexList.GetTailPosition();
		maxIndex=minIndex=indexList.GetTail();
		if(maxIndex<0) {DSF_TRACE("TrimDot Error\r\n"); return;}//基本不可能，仅为容错
		maxDot = dstDotArray.GetAt(maxIndex);
		while (pos != NULL)
		{
			//index1大,index2小
			CDot tempDot1, tempDot2;
			POSITION revPos = pos;
			int index1 = indexList.GetPrev(pos);
			int nDstCount = dstDotArray.GetSize();
			tempDot1 = dstDotArray.GetAt(index1<0?nDstCount-1:index1);
			dstDotArray.RemoveAt(index1<0?nDstCount-1:index1);
			indexList.RemoveAt(revPos);
			if(pos == NULL) break;
			int index2 = indexList.GetAt(pos);
			if (index1==index2+1)
			{
				minIndex = index2;
				nDstCount = dstDotArray.GetSize();
				minDot = dstDotArray.GetAt(minIndex<0?nDstCount-1:minIndex);

				tempDot2 = dstDotArray.GetAt(index2<0?nDstCount-1:index2);
				BOOL res = MidAngleLineCrossShort(index1<0?index1+nCount:index1, 
					index2<0?index2+nCount:index2, dbSpace, dotArray);
				if (!res)
				{
					res = CheckDistance(index1, index2, dbSpace, dotArray);
					if(res)
					{
						dot1 = tempDot1;
						dot2 = tempDot2;
						bIsNotCrossShort = TRUE;
					}
				}
			}
			else
			{
				break;
			}
		}
		int nDstCount = dstDotArray.GetSize();
		if(bIsNotCrossShort)
		{
			DSF_TRACE("2");
			CDot dotA, dotB, dotC, dotD;
			int index = minIndex-1;
			if(index<0) index=nDstCount-1;
			dotA = dstDotArray.GetAt(index);
			dotB = minDot;
			dotC = maxDot;
			index = minIndex;
			if(index<0) index=0;
			else if(index>nDstCount-1) index=0;
			dotD = dstDotArray.GetAt(index);
			CDot dot = GetCrossDot(dotA, dotB, dot2, dot1);
			CDot dotCross2 = GetCrossDot(dotC, dotD, dot2, dot1);
			dstDotArray.InsertAt(minIndex<0?0:minIndex, dotCross2);
			dstDotArray.InsertAt(minIndex<0?0:minIndex, dot);
			DSF_TRACE("3");
		}
		else
		{
			//index1小,index2大
			DSF_TRACE("4");
			CDot dotA, dotB, dotC, dotD;
			int index1 = minIndex-1;
			if(index1<0) index1=nDstCount-1;
			dotA = dstDotArray.GetAt(index1);
			dotB = minDot;
			dotC = maxDot;
			int index2 = minIndex;
			if(index2<0) index2=0;
			else if(index2>nDstCount-1) index2=0;
			dotD = dstDotArray.GetAt(index2);
			//if(CheckCrossBefore(index1, index2, dstDotArray)) continue;
			CDot dot = GetCrossDot(dotA, dotB, dotC, dotD);

			CSegmentLine line1(dot, dotB);
			CSegmentLine line2(dot, dotC);
			if(!line1.IncludeDot(dotA) && !line2.IncludeDot(dotD))
			{
				dstDotArray.InsertAt(minIndex<0?0:minIndex, dot);
			}
			else
			{
				m_arrayExceptDot.Add(dotA);
			}
			DSF_TRACE("5");
		}
	}
}

void CExternStroke::AddToIndexList(CList<int,int> &indexList, int index)
{
	POSITION pos = indexList.Find(index);
	if (pos == NULL)
	{
		indexList.AddTail(index);
	}
}

int CExternStroke::AddUpIndex(CList<int,int> &indexList, int index, 
							   double dbSpace, const CArray<CDot, CDot> &dotArray)
{
	int size = dotArray.GetSize();
	int returnIndex = index-1;
	int indexUp = index-1;
	while (indexUp!=index)
	{
		if(indexUp<0) indexUp=size-2;
		BOOL bFlag = MidAngleLineCrossShort(index, indexUp, dbSpace, dotArray);
		if (bFlag)
		{
			AddToIndexList(indexList, indexUp);
		}
		else
		{
			break;
		}
		indexUp--;
		returnIndex--;
	}
	return returnIndex;
}

int CExternStroke::AddDownIndex(CList<int,int> &indexList, int index, 
								 double dbSpace, const CArray<CDot, CDot> &dotArray)
{
	int returnIndex = index+1;
	int indexDown = index+1;
	int size = dotArray.GetSize();
	while (indexDown!=index)
	{
		if(indexDown>size-2) indexDown=0;
		BOOL bFlag = MidAngleLineCrossShort(index, indexDown, dbSpace, dotArray);
		if (bFlag)
		{
			AddToIndexList(indexList, indexDown);
		}
		else
		{
			break;
		}
		indexDown++;
		returnIndex++;
	}
	return returnIndex;
}

void CExternStroke::SortIndexList(CList<int,int> &indexList)
{
	int count = indexList.GetCount();
	while (--count)
	{
		POSITION pos = indexList.GetHeadPosition();
		while (pos != NULL)
		{
			POSITION pos1 = pos;
			int index1 = indexList.GetNext(pos);
			POSITION pos2 = pos;
			if(pos == NULL) break;
			int index2 = indexList.GetAt(pos);
			if (index1>index2)
			{
				indexList.SetAt(pos1, index2);
				indexList.SetAt(pos2, index1);
			}
		}
	}
}

BOOL CExternStroke::CheckDistance(int index1, int index2, double dbspace, const CArray<CDot, CDot> &dotArray)
{

#define CheckDistance_CheckIndex(x)\
	if (x < 0)\
	{\
		x += nCount;\
	}\
	else if(x > nCount-1)\
	{\
		x -= nCount;\
	}

	int nCount = dotArray.GetSize()-1;
	int aIndex = index1+1;
	int bIndex = index1;
	int cIndex = index2;
	int dIndex = index2-1;
	CheckDistance_CheckIndex(aIndex);
	CheckDistance_CheckIndex(bIndex);
	CheckDistance_CheckIndex(cIndex);
	CheckDistance_CheckIndex(dIndex);

	CDot dotA = dotArray.GetAt(aIndex);
	CDot dotB = dotArray.GetAt(bIndex);
	CDot dotC = dotArray.GetAt(cIndex);
	CDot dotD = dotArray.GetAt(dIndex);

	double a,b,c,d;
	a = Distance(dotB, dotC);
	b = Distance(dotA, dotC);
	c = Distance(dotA, dotB);
	double angleB = acos((a*a+c*c-b*b)/(2*a*c));
	b = Distance(dotC, dotD);
	c = Distance(dotB, dotD);
	d = a;//Distance(dotB, dotC);
	double angleC = acos((b*b+d*d-c*c)/(2*b*d));
	//angleB = angleB/2;
	//angleC = angleC/2;
	//double minDistance = fabs(dbspace)*((cos(angleB)/sin(angleB))+(cos(angleC)/sin(angleC)));
	if (angleB<PAI*3/4 && angleC<PAI*3/4)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CExternStroke::DotInRegion2(const CArray<CDot, CDot> &dotArray, CDot dot)
{
	int count = 0;
	CDot dot2(dot.x-2, dot.y);
	CRadialLine line1(dot, dot2, 1);
	int size = dotArray.GetSize();
	for (int i=0; i<size-1; i++)
	{
		CDot dotTemp1 = dotArray.GetAt(i);
		CDot dotTemp2 = dotArray.GetAt(i+1==size-1?0:i+1);
		CSegmentLine line2(dotTemp1, dotTemp2);
		//点在线段上
		if (line2.DotInLine(dot))
		{
			return 0;
		}
		//线段不是水平线
		if (line2.isEqualX || !SAMEVALUE(line2.k,0))
		{
			if (line1.DotInLine(dotTemp1))
			{
				if(dotTemp1.y > dotTemp2.y) count++;
			}
			else if (line1.DotInLine(dotTemp2))
			{
				if(dotTemp2.y > dotTemp1.y) count++;
			}
			else if (line1.IsCross(line2))
			{
				count++;
			}
		}
	}
	if (count%2==1)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

BOOL CExternStroke::CSegmentLine::IsCross(CSegmentLine segment, CDot &dot)
{
	if (this->isEqualX)
	{
		if (segment.isEqualX)
		{
			return FALSE;
		}
		else
		{
			dot.x = this->b;
			dot.y = segment.k*dot.x + segment.b;
			return (this->DotInLine(dot)&&(segment.DotInLine(dot)));
		}
	}
	else
	{
		if (segment.isEqualX)
		{
			dot.x = segment.b;
			dot.y = this->k*dot.x + this->b;
			return (this->DotInLine(dot)&&segment.DotInLine(dot));
		}
		else
		{
			if (SAMEVALUE(this->k, segment.k))
			{
				return FALSE;
			}
			else
			{
				dot.x = (segment.b-this->b)/(this->k-segment.k);
				dot.y = this->k*dot.x + this->b;
				return (this->DotInLine(dot)&&segment.DotInLine(dot));
			}
		}
	}
}

BOOL CExternStroke::CheckCrossBefore(int index1, int index2, CArray<CDot, CDot> &dstDotArray)
{
	CDot tempDot;
	CArray<CDot, CDot> tempDotArray;
	tempDotArray.Copy(dstDotArray);
	int size = dstDotArray.GetSize();
	int tempIndex2 = (index2+1>size-1)?0:index2+1;
	while ((index1-1<0?size-1:index1-1)!=tempIndex2)
	{
		CDot dot1 = dstDotArray.GetAt(index1);
		CDot dot2 = dstDotArray.GetAt(index1-1<0?size-1:index1-1);
		//tempDotArray.RemoveAt(index1);
		index1--;
		if(index1<0) index1=size-1;
		CSegmentLine line1(dot1, dot2);
		int tempIndex1 = (index1-1<0)?0:index1-1;
		while ((index2+1>size-1?0:index2+1)!=tempIndex1)
		{
			CDot dot3 = dstDotArray.GetAt(index2);
			CDot dot4 = dstDotArray.GetAt(index2+1>size-1?0:index2+1);
			//tempDotArray.RemoveAt(index2);
			index2++;
			if (index2>size-1) index2=0;
			CSegmentLine line2(dot3, dot4);
			if (line1.IsCross(line2, tempDot))
			{
				//tempDotArray.InsertAt(index1+1, tempDot);
				dstDotArray.RemoveAll();
				dstDotArray.Copy(tempDotArray);
				DSF_TRACE("CheckCrossBefore TRUE");
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CExternStroke::AddExcludeIndex(CList<int,int> &indexList, int indexUp, int index, int indexDown, 
							   double dbSpace, const CArray<CDot, CDot> &dotArray)
{
	int nIndexCount = (index-indexUp)+(indexDown-index)+1;
	int nSize = dotArray.GetSize()-1;
	if (nIndexCount<nSize)
	{
		if(indexUp<0) indexUp+=nSize;
		if(indexDown>nSize-1) indexDown-=nSize;
		int tempIndexUp = indexUp;
		int tempIndexDown = indexDown;
		while (indexUp != indexDown)
		{
			if(MidAngleLineCrossShort(indexDown, indexUp, dbSpace, dotArray))
			{
				tempIndexUp = indexUp;
				AddToIndexList(indexList, indexUp);
				indexUp--;
				if(indexUp<0) indexUp+=nSize;
			}
			else
			{
				break;
			}
		}
		while (indexDown != tempIndexUp)
		{
			if (MidAngleLineCrossShort(tempIndexUp, indexDown, dbSpace, dotArray))
			{
				AddToIndexList(indexList, indexDown);
				indexDown++;
				if(indexDown>nSize-1) indexDown-=nSize;
			}
			else
			{
				break;
			}
		}
	}
}

BOOL CExternStroke::CSegmentLine::IncludeDot(const CDot &dot)
{
	if(SAMEVALUE(dot1.x, dot.x) && SAMEVALUE(dot1.y, dot.y)) return FALSE;
	if(SAMEVALUE(dot2.x, dot.x) && SAMEVALUE(dot2.y, dot.y)) return FALSE;
	if (this->isEqualX)
	{
		return ((dot.y>this->minY)&&(dot.y<this->maxY));
	}
	else
	{
		return ((dot.x>this->minX)&&(dot.x<this->maxX));
	}
}


BOOL CExternStroke::CRadialLine::IsInOneSide(const CDot &dot1, const CDot &dot2)
{
	CSegmentLine segment(dot1, dot2);
	if (this->isEqualX)
	{
		if (segment.isEqualX)
		{
			return TRUE;
		}
		else
		{
			CDot tempDot;
			tempDot.x = this->b;
			tempDot.y = segment.k*tempDot.x + segment.b;
			return (!(segment.DotInLine(tempDot)))||segment.isCrossAtDot;
		}
	}
	else
	{
		if (segment.isEqualX)
		{
			CDot tempDot;
			tempDot.x = segment.b;
			tempDot.y = this->k*tempDot.x + this->b;
			return (!segment.DotInLine(tempDot)||segment.isCrossAtDot);
		}
		else
		{
			if (SAMEVALUE(this->k, segment.k))
			{
				return TRUE;
			}
			else
			{
				CDot tempDot;
				tempDot.x = (segment.b-this->b)/(this->k-segment.k);
				tempDot.y = this->k*tempDot.x + this->b;
				return (!segment.DotInLine(tempDot))||segment.isCrossAtDot;
			}
		}
	}
}
