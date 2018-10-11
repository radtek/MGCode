// NodeCodebar.h: interface for the CNodeCodebar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_)
#define AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Quad.h"
#include "XDC.h"
class CNodeCodebar : public CNode  
{
public:
	CNodeCodebar();
	virtual ~CNodeCodebar();
	virtual void Serialize(FILE* fp,BOOL bFlag);

public:
	virtual BOOL PreprocessMark(DWORD nStyle);
	virtual BOOL CanGroup();
	CQuad GetMarkRange();
	
	virtual BOOL Create(CRect &rect);
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Draw(XDC *pDC);
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);

	virtual void Rotate(CDot dot,double angle);
	virtual BOOL Move(double moveX,double moveY);
	virtual void Scale(CDot dot,double ratioX,double ratioY);
	virtual CNode * Copy(UINT nStyle);


	virtual BOOL CreateStrokeList();
	virtual CString GetParitybit(CString markcode){return "";};//得到校验位

	virtual void DeleteFill();
	virtual BOOL GetBarcode();

	virtual void DrawExtern(XDC *pDC);
	
public:
	BOOL GenBarCode(UINT nStyle);
	void AddToPlt(CNode *pNodeplt);
	BARTYPE m_BarType;
	UINT m_nTextID;			//条码ID
	double m_dbFillSpace;	//填充间隔
	BOOL m_bRev;			//确定条码是否反填
    BOOL m_bGoBack;			//块往复
    BOOL m_bConnect;		//块连接
	double m_dbRevWidth;	//反相条码时的宽度
	double m_dbAngle;		//条码角度
	BOOL	m_bTextVis;		//目前不用到，

	DWORD m_nSingleLineMarkCount;
	UINT  m_nNarrowLineNum;

	BYTE* m_pByte;			//条码二进制信息
	UINT m_nCol;			//条码行数
	UINT m_nRow;			//条码列数

	double  m_dbshrink;		//条码缩进量
	double	m_dbHeightBar;	//条码高度
	int     m_nOutline;		//PDF417条码行数
	int     m_nErrLev;		//纠错级别，用于QR,DATAMATRIX
	int		m_nHPoint;		//垂直方向点个数,用于QR,DATAMATRIX
	int		m_nWPoint;		//水平方向点个数,用于DATAMATRIX
	BOOL	m_bAutoSet;		//设置条码长宽属性;//true 自动设置;FALSE 用户设置
	int		m_nAlign;		//条码与字符号 1左对齐;2中间对齐;3右对齐
	double  m_narrowW;		//用于条码拉变时,记录当前条码窄条宽度

	CDot m_dotBase;			//条码基准点
	StrokeList m_listFill;

	CArray<StrokeList*, StrokeList*> m_CodeBarList;

	BOOL m_bPointDataMatrix;
	int m_nPointRadiusPercent;
	BOOL CreateStrokeListPoint();

private:
	BOOL CreateStrokeList2();
	BOOL CreateStrokeList1();
	void initCodebar();

	BOOL   AddToList();

	BOOL   ClearBarList();
	BOOL   BetterOneCodeBar();
	BOOL   ConnectOneCodeBar();
	double Distance(CDot &sDot, CDot &eDot);
	BOOL   ProOneStroke(CDot &dt, CStroke *pStrk);
	BOOL   MinDsIsFirstDot(CDot &dt, CStroke *pStrk);

	double m_dbSpace;		//对于EAN8和EAN13码来说会存在长短之分
							//长比短的长多少即为此参数,目前末做序列化操作
							//现用常量来代替，沿用原来的值（0.6MM）
	double m_dbcodeRate;	//此值固定为2.75
};

#endif // !defined(AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_)

