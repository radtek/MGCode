// ExternStroke.h: interface for the CExternStroke class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXTERNSTROKE_H__29EDEBDE_4EB5_4502_B3C8_D38F7FA4F904__INCLUDED_)
#define AFX_EXTERNSTROKE_H__29EDEBDE_4EB5_4502_B3C8_D38F7FA4F904__INCLUDED_

#include "Dot.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExternStroke  
{
	class CSegmentLine
	{
	public:
		BOOL IncludeDot(const CDot &dot);
		BOOL IsCross(CSegmentLine segment, CDot &dot);
		BOOL DotInLine(const CDot &dot);
		CSegmentLine(const CDot &dot1, const CDot &dot2);
		~CSegmentLine();
		double minX;
		double maxX;
		double minY;
		double maxY;
		double k;
		double b;
		BOOL isEqualX;
		CDot dot1;
		CDot dot2;
		BOOL isCrossAtDot;
	};
	class CLine
	{
	public:
		CDot GetCrossDot(const CLine &line);
		~CLine();
		CLine(const CDot &dot1, const CDot &dot2);
		double k;
		double b;
		BOOL isEqualX;
		CDot dot1;
		CDot dot2;
	};
	class CRadialLine
	{
	public:
		BOOL IsCross(const CLine &line);
		BOOL DotInLine(const CDot &dot);
		BOOL IsCross(CSegmentLine &segment);
		BOOL IsInOneSide(const CDot &dot1, const CDot &dot2);
		CRadialLine(const CDot &beginDot, const CDot &dot2, int nDirection);
		~CRadialLine();
		double beginX;
		double beginY;
		BOOL isXbig;
		BOOL isYbig;
		double k;
		double b;
		BOOL isEqualX;
	};
	
public:
	void AddExcludeIndex(CList<int,int> &indexList, int indexUp, int index, int indexDown, 
							   double dbSpace, const CArray<CDot, CDot> &dotArray);
	BOOL CheckCrossBefore(int index1, int index2, CArray<CDot, CDot> &dstDotArray);
	int DotInRegion2(const CArray<CDot, CDot> &dotArray, CDot dot);
	BOOL CheckDistance(int index1, int index2, double dbspace, const CArray<CDot, CDot> &dotArray);
	void SortIndexList(CList<int,int> &indexList);
	int AddDownIndex(CList<int,int> &indexList, int index, 
					double dbSpace, const CArray<CDot, CDot> &dotArray);
	int AddUpIndex(CList<int,int> &indexList, int index, 
					double dbSpace, const CArray<CDot, CDot> &dotArray);
	void AddToIndexList(CList<int,int> &indexList, int index);
	void TrimDot(const CArray<CDot, CDot> &dotArray, CArray<CDot, CDot> &dstDotArray, double dbSpace);
	void ClearExceptionalDot(const CArray<CDot, CDot> &dotArray, CArray<CDot, CDot> &dstDotArray, double dbSpace);
	BOOL MidAngleLineCrossShort(int index1, int index2, double dbspace, const CArray<CDot, CDot> &dotArray);
	CDot GetCrossDot(CDot dotA, CDot dotB, CDot dotC, CDot dotD);
	void ClearSameIndex(CArray<int, int> &indexArray, CArray<CDot, CDot> &dstDotArray);
	int DotInRegion(const CArray<CDot, CDot> &dotArray, CDot dot);
	BOOL IsStrokeInStroke(const CStroke &stroke1, const CStroke &stroke2);
	BOOL IsInAngle(CDot &dotLeft, const CDot &dotMid, CDot &dotRight, 
					const CArray<CDot, CDot> &dotArray);
	BOOL ClearAngleShort(const CArray<CDot, CDot> &dotArray, CArray<CDot, CDot> &dstDotArray, double dbSpace);
	inline void SetDotToCenter(CDot &dot1, CDot &dot2);
	void ClearDifferent(const CStroke &strokeIn, CStroke &strokeOut);
	int GetCrossCount(CRadialLine radialLine, const CArray<CDot, CDot> &dotArray);
	inline void GetCxCy(double &cx, double &cy, double c1, const CDot &dot1, const CDot &dot2);
	inline void GetEqualLenAngle(CDot &dotLeft, const CDot &dotMid, CDot &dotRight);
	inline double Distance(const CDot dot1, const CDot dot2);
	void RemoveMidDotInLine(CArray<CDot, CDot> &dotArray);
	inline BOOL IsLine(const CDot &dotLeft, const CDot &dotMid, const CDot &dotRight);
	void RemoveSameDot(CArray<CDot, CDot> &dotArray);
	CExternStroke();
	virtual ~CExternStroke();
	BOOL IsOutStroke(const CStroke &stroke, const StrokeList &strokeList, POSITION indexPos);
	void DoExtern(const CStroke &strokeIn, CStroke &strokeOut, double dbSpace, BOOL bOpt);
	void GetExternDot(const CDot &dotLeft, const CDot &dotMid, const CDot &dotRight, 
			CDot &dotOut, double dbSpace, const CArray<CDot, CDot> &dotArray);

	CArray<CDot, CDot> m_arrayExceptDot;
};

#endif // !defined(AFX_EXTERNSTROKE_H__29EDEBDE_4EB5_4502_B3C8_D38F7FA4F904__INCLUDED_)
