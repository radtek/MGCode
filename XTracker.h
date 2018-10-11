// XTracker.h: interface for the XTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTRACKER_H__D814A30B_F9F7_4FF1_BC2D_CA3850434BCA__INCLUDED_)
#define AFX_XTRACKER_H__D814A30B_F9F7_4FF1_BC2D_CA3850434BCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stroke.h"
#include "Quad.h"	// Added by ClassView

class XTracker
{
public:
	class DotSelectList  
	{
	public:
		DotSelectList()
		{
			m_pNext=m_pFront=NULL;
			m_Node.x=m_Node.y=0;
			m_nNum=-1;
			m_pLeft=m_pRight=NULL;
			bLeft=bRight=TRUE;
			bLeftDraw=bRightDraw=FALSE;
		}
		~DotSelectList()
		{
		};
		
	public:
		
		CDot m_Node;
		DotSelectList *m_pNext; //前一个指针
		DotSelectList *m_pFront;//后一个指针
		CDot *m_pLeft;          //左结点指针
		CDot *m_pRight;         //右结点指针
		BOOL bLeft;             //左结点是否为空
		BOOL bRight;            //右结点是否为空
		BOOL bLeftDraw;         //是否向左边画线
		BOOL bRightDraw;        //是否向右边画线
		int  m_nNum;            //点的号码,用于点的定位，编号从0开始
	};
	class DotHead
	{
	public:
		DotHead()
		{
			m_pAdd   = NULL;
			m_pHead  = NULL;
			m_pNext  = NULL;
			m_pTail  = NULL;
			m_nCount = 0;
		}
		~DotHead()
		{
		}

	public:
		void InverDrawDot(CWnd *pWnd);//反画
		void AddSelectDot(CDot &dot,CDot &left,CDot &right,int nNum,BOOL bLeft,BOOL bRight)//构作双向链表
		{
			DotSelectList *pAdd = new DotSelectList;
			pAdd->m_Node        = dot;
			
			pAdd->m_pLeft       = new CDot;
			pAdd->m_pRight      = new CDot;
			*(pAdd->m_pLeft)    = left;
			*(pAdd->m_pRight)   = right;

			pAdd->bLeftDraw     = bLeft;
			pAdd->bRightDraw    = bRight;

			pAdd->m_nNum        = nNum;
			
			if(m_pHead==NULL)
			{
				m_pHead=pAdd;
				m_pTail=pAdd;
			}
			else
			{
				m_pTail->m_pNext=pAdd;
				pAdd->m_pFront=m_pTail;
				m_pTail=pAdd;
			}
		}

		void DelSelectDot()//删除一支点
		{
			DotSelectList *plist=m_pHead;
			while(plist)
			{
				m_pHead=m_pHead->m_pNext;
				if(plist->bLeft)
				{
					delete plist->m_pLeft;
					plist->bLeft=FALSE;
				}
				if(plist->bRight)
				{
					delete plist->m_pRight;
					plist->bRight=FALSE;
				}
				delete plist;
				plist=m_pHead;
			}
			m_pTail=NULL;
		}

		void DataExChange()//排序，至少要有两个数据才进行交换
		{
			if(m_pTail==m_pHead)
			{
				return;
			}
			
			DotSelectList *pList=m_pHead;
			DotSelectList *pTail=pList->m_pNext;
			int i,j;
			while(pTail)//从前往后
			{
				i=pList->m_nNum;
				j=pTail->m_nNum;
				if(i+1==j)
				{
					delete pList->m_pRight;
					pList->bRight=FALSE;
					pList->m_pRight=&pTail->m_Node;
				}
				pList=pList->m_pNext;
				pTail=pTail->m_pNext;
			}
			pTail=pList;
			i=pList->m_nNum;
			j=m_pHead->m_nNum;
			if((i+1)%m_nCount==j)
			{
				delete pList->m_pRight;
				pList->bRight=FALSE;
				pList->m_pRight=&m_pHead->m_Node;
			}
			
			//从后往前
			pList=pTail->m_pFront;
			while(pList)
			{
				i=pTail->m_nNum;
				j=pList->m_nNum;
				if(i-1==j)
				{
					delete pTail->m_pLeft;
					pTail->bLeft=FALSE;
					pTail->m_pLeft=&pList->m_Node;
				}
				pTail=pTail->m_pFront;
				pList=pList->m_pFront;
			}

			i=pTail->m_nNum;
			j=m_pTail->m_nNum;
			if((j+1)%m_nCount==i)
			{
				delete pTail->m_pLeft;
				pTail->bLeft=FALSE;
				pTail->m_pLeft=&m_pTail->m_Node;
			}
		}
		
		void DeleteDot(CDot &dot)//删除结点的同时修改号码
		{
			if(m_pHead==NULL)	
				return;
			DotSelectList *plist=m_pHead;
			while(plist)
			{
				if(plist->m_Node==dot)
					break;
				plist=plist->m_pNext;
			}
			if(plist==m_pHead)
			{
				m_pHead=plist->m_pNext;
			}
			if(plist==m_pTail)
			{
				m_pTail=plist->m_pFront;
			}
			if(plist->m_pFront)
				plist->m_pFront->m_pNext=plist->m_pNext;
			if(plist->m_pNext)
				plist->m_pNext->m_pFront=plist->m_pFront;
			if(plist->bLeft)
				delete plist->m_pLeft;
			if(plist->bRight)
				delete plist->m_pRight;
			delete plist;
			plist=m_pHead;
			while(plist)
			{
				plist->m_nNum--;
				plist=plist->m_pNext;
			}
		}
		
		void operator =(DotHead &head)
		{
			m_pAdd=head.m_pAdd;
			m_pHead=head.m_pHead;
			m_pNext=head.m_pNext;
		}

		DotSelectList *m_pHead;
		DotSelectList *m_pTail;
		CStroke *m_pAdd;//Stroke的地址,动态修改
		DotHead *m_pNext;

		int m_nCount;
	};
public:
	XTracker();
	~XTracker();

public:
	void MoveDot(double MoveX,double MoveY);
	BOOL IsSameNode(CStroke *pStroke1,CStroke *pStroke2);
	void JoinPoint();
	void SplitPoint(double dbPoint);
	void ClearAll();
	BOOL HitValidPoint(CPoint &point,CWnd *pWnd);
	void ModifyStrokeList(CStroke *pStroke);
	void DelDot();
	void AddPoint(CPoint &point,CWnd *pWnd);
	void AddRect(CWnd *pWnd);
	void Edit(CQuad &quad,CWnd *pWnd);
	void SetFlag(BOOL bflag);
	void Edit(CPoint &point,CWnd *pWnd);
	BOOL IsOnDot(CPoint &point,CWnd *pWnd);
	void DelRect(CWnd *pWnd);
	void ClearAndStore(CQuad &quad,CWnd *pWnd);
	BOOL HasDot(CQuad &quad);
	void ClearAndStore(CPoint &point,CWnd *pWnd);
	DWORD FillRect(CWnd *pWnd);
	void SearchDot(CPoint &point,CWnd *pWnd);
	void SearchDot(CQuad &quad);
	void GetDot(CQuad &quad);
	void GetDot(CPoint &point,CWnd *pWnd);
	BOOL HaveObject();
	BOOL HitOtherDot(CPoint &point,CWnd *pWnd);
	BOOL HitSelectDot(CPoint &point,CWnd *pWnd);
	void ClearStrokeList();
	void DrawNode(CPoint &pt1,CPoint &pt2,CWnd *pWnd);
	void SingleModify(CPoint &point,CWnd *pWnd);
	void SetPhysicQuad(CWnd *pWnd,CQuad &quad);
	void GetTrueRect(CRect &rect);
	void InverDraw(CQuad &quad,CWnd *pWnd);
	void Draw(CQuad &quad,CWnd*pWnd);
	void DrawTracker(XDC *pDC,CQuad &quad,CWnd *pWnd);
	int  GetResult(CPoint &point ,CQuad &quad,CWnd *pWnd);
	void Handle(int nResult,double &ratiox,double &ratioy,double &MoveX,double &MoveY,CPoint dot,CPoint &point,CQuad &quad,CWnd *pWnd);
	BOOL GetDragPara(int &nResult,CQuad &quad,CDot &dot,double &ratioX,double &ratioY,double &MoveX,double &MoveY,CWnd *pWnd);
	int  PtHitQuad(CPoint &pt,CQuad &quad,CWnd *pWnd);
	
	void AddDotHead(CStroke *pStroke,CDot &dotIndex,CWnd *pWnd);
	void FromHandle(int nResult,double &ratioX,double &ratioY,double &MoveX,double &MoveY, CPoint pointOrg,CPoint &point,CQuad &quad,CDot &dot,CWnd *pWnd);
	
	BOOL IsPtInRect(int i,CPoint &point,CTree *pTree,CWnd *pWnd);
	BOOL CtrlPoint(int i,CPoint &point,CTree* pTree,CWnd *pWnd);
	int  HitPoint(CTree* pTree,CPoint &point,CWnd *pWnd);
	BOOL InRect(CPoint &point,CTree *pTree,CWnd *pWnd);
	void UpdateRectCtrl();
	void InverDrawDot(CWnd *pWnd);

	// 查找选中列表中第一个点的序号
	int FindSelListFirstDotNum(int &nNum);
	
	CStroke* FindSelListFirstDot(int &nNum, int &nCount);
	// 通过一个选中的点选中整个笔画
	void SelectStrokeBySelPt(CWnd *pWnd);

	void SetDotToStart(CWnd *pWnd);

	void FilterPoint(CWnd *pWnd, double dbPresic, double dbAngle);

	void KillAngle(CWnd *pWnd);

	void ReverseStroke(CWnd *pWnd);

	void DrawSelTraker(XDC *pDC, CQuad &Quad,CWnd *pWnd, CArray<SELQUAD, SELQUAD> &QuadList);
	void DrawSelObjTracker(XDC *pDC, CQuad &Quad,BOOL bFillQuad,CWnd *pWnd);

	CQuad GetQuadSave()
	{
		return m_quadSave;
	}
	void SetQuadSave(CQuad &quad)
	{
		m_quadSave=quad;
	}

	CQuad m_quadOrg;			
	CQuad m_quad[8];
	StrokeList m_StrokeList;//用来放置笔划


private:
	void ModifyDot(int nResult,CDot &dot,CQuad &quad);
	void DrawQuad(int nResult,CDot &dot,double &ratioX,double &ratioY,double &MoveX,double &MoveY,CQuad &quad1,CQuad &quad2,CWnd *pWnd);
	//8个控点位置 按0，1，2，3为四周控点，4，5，6，7为边上控点(clock-wise)

public:
	// 查找选中列表中的第一个点
	int FindSelListFirstPt(CDot &dot);
	// 增加指定Stroke到可编辑列表中
	void AddStrokeToEditList(CStroke *pStroke);
	
	BOOL Track(CPoint &point,CWnd *pWnd);
	void LastBrush(CWnd *pWnd,CPoint &ptf,CPoint &ptl);
	void DrawRect(CWnd *pWnd,CPoint &ptf,CPoint &ptl,CPoint &point);
	BOOL IsFillStyle(CStroke *pStroke);
	BOOL IsDiffStroke();
	int  HeadOrTail(DotHead *pHead);
	BOOL DotListSub(DotHead **plist,int &nCount);
	void DataEXChange();
	double Distance(CDot &sDot, CDot &eDot);
	void ReplaceDot();
	void AddDotHeadB(CStroke *pStroke,CQuad &quad);
	void ClearDot();
	BOOL ListEmpty();
	void AddDotToHead(CStroke *pStroke,CDot &dotIndex, double dbdata);
	BOOL HitSelectDotB(CDot &dotTest, double dbdata);

private:
	CQuad m_NewQuad;
	CQuad m_OldQuad;
	CQuad m_quadSave;

public:
	DotHead *m_pDotHead;
	DotHead *m_pDotTail;
	CPoint m_ptMeasureLine;
	CPoint m_ptPrev;
	CPoint m_ptNow;
	double m_HITBORDER;
	CPoint m_ptDown;
	
	//当选中矩形中心控制点时，记录矩形中心的物理位置
	CDot m_dtQuadCenter;
	CRect m_rect;
	CRect m_rectOut;
};

#endif // !defined(AFX_XTRACKER_H__D814A30B_F9F7_4FF1_BC2D_CA3850434BCA__INCLUDED_)
