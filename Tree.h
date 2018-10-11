// Tree.h: interface for the CTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREE_H__F21BEEC1_E087_11D3_845D_444553540001__INCLUDED_)
#define AFX_TREE_H__F21BEEC1_E087_11D3_845D_444553540001__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Quad.h"	// Added by ClassView
#include "Stroke.h"
#include "Property.h"
#include "Fill.h"
#include "XDC.h"
#include "OffsetReg.h"
#include "DsfObj.h"
#include "StrokeList.h"


typedef struct SericaQuadDot
{
	CQuad quad;
	CDot dot;
	int nNum;
}SeriQuDot;

class CTree : public CObject
{
	//DECLARE_SERIAL(CTree)

public:
	//////////////////////////////////////////////
	//���켰����
	CTree();
	CTree(CNode *pNode);
//	CTree(const CTree *src, UINT nStyle=0x00);
	CTree(const CTree *src, UINT nStyle=0x00, BOOL bFill = TRUE);
	virtual ~CTree();
	
	//���ô�����
	void SetProperty(int mode,CProperty pty);
	void SetProperty(CNode *pNode,int mode,CProperty pty);
	
	//����
	void Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY);
	void Drag(CNode *pNode,CDot dot,double ratioX,double ratioY,double moveX,double moveY);

	//���ǰԤ����
	BOOL PreprocessMark(DWORD nStyle);
	BOOL PreprocessMark(CNode *pNode,DWORD nStyle);

	//���
	void Mark(CNode *pNode,HWND hWnd);
	void Mark(HWND hWnd);

	//ɾ��
	void Delete();
	void Delete(CNode *pNode);

	//��ת
	void Rotate(CDot dot,double angle);
	void Rotate(CNode *pNode,CDot dot,double angle);

	//�ƶ�
	void Move(double moveX,double moveY);
	void Move(CNode *pNode,double moveX,double moveY);

	//����
	void Scale(CDot dot,double ratioX,double ratioY);
	void Scale(CNode *pNode,CDot dot,double ratioX,double ratioY);

	//������Ч��Χ����ȡ
	CQuad CalLimit();
	void CalLimit(CNode *pNode);
	CQuad GetLimit();

	//��ͼ��ʾ
	void Draw(XDC *pDC);
	void Draw(CNode *pNode,XDC *pDC);

	//���л�����
	void Serialize(FILE* fp,BOOL bFlag);
	void Save(CNode *pNode,FILE* fp,BOOL bLeft);
	CNode * Load(FILE* fp);

	//�Ƿ������ָ���ķ�Χ��
	BOOL Include(CQuad quad);
	BOOL Include(CNode *pNode,CQuad quad);

	//��ȡ
	BOOL Click(CPoint &point,CWnd *pWnd);
	BOOL Click(CNode *pNode,CPoint &point,CWnd *pWnd);

	//��ѡ����
	void SetSelect(BOOL bFlag=TRUE);
	void SetSelect(CNode *pNode,BOOL bFlag=TRUE);
	BOOL IsSelect();
	BOOL IsSelect(CNode *pNode);
	void InvertSelect();
	void InvertSelect(CNode *pNode);

	//�����
	CTree *UnGroup();
	BOOL IsGroup();
	void Group(CTree *pTree);
	BOOL CanGroup();

	//���
	void Fill();
	void Scan(CNode *pNode,StrokeList* listTemp);
	void GetFillStyle(FILLDATA& fill);
	void SetFillStyle(FILLDATA& fill);
	void DeleteFill();

	//����
	CNode * CopyFrom(CNode *pNode,UINT nStyle=0x00);

	//���ӽڵ�
	void Add(CNode *pNode);

	//ȡ���ڵ�
	CNode *GetRoot(){return m_pRoot;};
	int GetMarkUnit(){return m_nMarkUnit;};
	void AddRect(CWnd *pWnd);
	void AddRect(CNode *pNode,CWnd *pWnd);

	BOOL GetLockFlg();

	BOOL IsBmpNode();
	BOOL IsBmpNode(CNode *pNode);

protected:
	CQuad m_quadLimit;
	BOOL  m_bSelect;     // ѡ�б�־ TRUE: ѡ�У�FALSE: ��ѡ��
	BOOL  m_bMainSelect; //
	int   m_nMarkUnit;

	//�������
	StrokeList m_listFill;
	FILLDATA m_Fill;

	CProperty m_property;	//����岿�ֵĴ������

public:
	BOOL m_bIsCurSortTree;  // �Ƿ�Ϊ��ǰ����������ѡ�е�TREE
	BOOL m_bIsUpdate;   // �ñ���ֻ����Do��Undo����ʱ���ã�TRUE:��Do��Undo����ʱ���Tree��FALSE:�����
	DWORD64 m_ID;
	int  m_nGroupNum;

	OBJMARKSET m_MarkSet;

#ifdef _DOUBLE_HEAD_MARK
	DOUBLEOUTPARA m_DoubleOutParaIns;
#endif

	CNode *m_pRoot;
	BOOL m_bUnGroupOnlyRoot;

public:
	void MarkPreview(CNode *pNode, CDC *pDC, const int &nPreviewSpeed);
	void MarkPreview(CDC *pDC, const int &nPreviewSpeed);
	WORD m_nGroup;	//����,���ھɰ汾����
	int GetType();

	void MarkNormal(HWND hWnd);
	void MarkSpecParaSet(HWND hWnd);
	void MarkFill(HWND hWnd);

	void SelMark(HWND hWnd, HANDLE hStop, SUBOBJMARKSET &stSub);
	void SelMark(CNode *pNode, int nLayer, HWND hWnd, HANDLE hStop, BOOL bIsResMark = FALSE);
	void SelMarkFill(HWND hWnd, int nLayer, HANDLE hStop, BOOL bIsResMark = FALSE);

public:
	virtual void AddToPlt(CNode *pNode,CNode *pNodeplt);

	CTree* ConvertPLT(UINT nRes);
	CTree* ConvertFillToPLT();
	CTree* ConvertFramToPLT(UINT nResult);

	BOOL IsText(CNode *pNode);
	BOOL IsCode(CNode *pNode);
	UINT CanConvert();
	void AddFillToStrokeList(StrokeList *pStrokeList);
	void CaptureObject(CQuad &quad,StrokeList *pStrokeList);
	void SetStrokeList(CNode *pNode,StrokeList *pStrokeList,BOOL bFlag=FALSE);
	void SetStrokeList(StrokeList *pStrokeList,BOOL bFlag=FALSE);
	BOOL IsEmpty(CNode *pNode);
	BOOL IsEmpty();
	void DelRect(CNode *pNode,CWnd *pWnd);
	void DelRect(CWnd *pWnd);
	BOOL PtInTree(CDot &dot);
	BOOL PtInTree(CPoint &point,CWnd *pWnd);
	BOOL SelectBarCode(CUIntArray& arrayTextID);
	BOOL CheckBarCode(CUIntArray& arrayTextID);
	void GetMarkRange(CNode *pNode, CQuad &quad);
	CQuad GetMarkRange();
	void GetStrokeList(CNode *pNode,STROKELIST *pStrokeList);//zg add
	void GetStrokeList(STROKELIST *pStrokeList);//zg add
	void SetLayColor(int nLayer,int nFillLayer);
	void SetLayColor(CNode *pNode,int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill);
	void GetLayColor(int &nLayer,int &nFillLayer);
	void GetLayColor(CNode *pNode,int &nLayer,int &nFillLayer);

	void SetLayGroupColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill);


	void SetObjColor(int nObjIndex, int nObjColor, int nFillIndex, int nFillColor);

	BOOL  GetBarcode();
	CQuad GetReFurbishQuad();

	void SetGroup(BOOL bGroup);
	void SetGroup(CNode *pNode,BOOL bGroup);

	BOOL GetFirstMarkCurrent(double & dbCurrent);
	BOOL ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	void ExportPlt(CNode *pNode,CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);

	void AddFillToPlt(CNode *pNode, CNode *pNodeplt);

	void LockLayer(int nLayer);
	void UnLockLayer(int nLayer);
	void LockLayer(CNode *pNode, int nLayer);
	void UnLockLayer(CNode *pNode, int nLayer);

	void FilterPoint(double dbPrecis, double dbAngle);
	void FilterPoint(CNode *pNode, double dbPrecis, double dbAngle);

	BOOL GetFirstDot(CDot &dt);
	BOOL GetTreeFirstdot(CNode *pNode, CDot &dt);

	BOOL GetLastDot(CDot &dt);
	BOOL GetNodeLastDot(CNode *pNode, CDot &dt);

	void SetSortStatus(BOOL bIsSort = FALSE);
	void SetSortStatus(CNode *pNode, BOOL bIsSort);

	CTree*  ExtendObj(double dbSpace, BOOL bOpt=TRUE);

	CStroke* GetGoLineStroke();
	CStroke* GetGoLineStroke(CNode *pNode);

	void SetGroupNum(int nNum);
	int  GetGroupNum();

	void ComToOneNode(void *pNode);
	void AddToOneNode(void *pNode);

	BOOL IsMainSelect();
	void SetMainSelect(BOOL bSelect=TRUE);
	double GetLength(CNode* pNode);
	double GetLength();

	BOOL IsHaveTxtOrCodeBar();
	BOOL IsHaveTxtOrCodeBar(CNode *pNode);

	BOOL IsHaveTxt(CNode *pNode);
	BOOL IsHaveCodeBar(CNode *pNode);

	BOOL GetBarcodeLength(CNode *pNode,SeriQuDot *QuadDot,int &nCodeId);
	BOOL SetBarcodeLength(CNode *pNode,SeriQuDot *QuadDot,int &nCodeId);


	BOOL GetTextBaseDot(CNode *pNode,CDot *QuadDot,int &nTextId);
	BOOL MoveOldBaseDot(CNode *pNode,CDot *QuadDot,int &nTextId);



	void TxtAddNo(CNode *pNode,CString &strNo);
	void CodeBarAddNo(CNode *pNode,CString &strNo);

	void DrawExtern(CNode *pNode, XDC *pDC);
	void DrawExtern(XDC *pDC);

	CQuad& RetLimit();

	void JoinStroke(double dbMinDistance);
	void JoinStroke(CNode *pNode,double dbMinDistance);

	//Wafer
	BOOL m_bMarked;	//�Ƿ��ǹ�
	void GetStrokeInQuad(CQuad &quad, StrokeList &strokeList);
	void GetStrokeInQuad(CNode *pNode, CQuad &quad, StrokeList &strokeList);

	int GetStrokeNum(int nFocusType);
	int GetStrokeNum(CNode *pNode, int nFocusType);

	void SetResinRect(CNode *pNode,int nRect);
	void GetResinRect(CNode *pNode,int aResinRect[],int &nId);
	int m_nResinRect;

	void SetStrokeLayFreq(CNode *pNode,double dbQFreq);
};

#endif // !defined(AFX_TREE_H__F21BEEC1_E087_11D3_845D_444553540001__INCLUDED_)






















