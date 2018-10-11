// Fnt.h: interface for the CFnt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FNT_H__DB4DFD01_FEA4_11D4_A486_00105A615525__INCLUDED_)
#define AFX_FNT_H__DB4DFD01_FEA4_11D4_A486_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stroke.h"
class CFnt : public CObject  
{
	DECLARE_SERIAL(CFnt)

public:
	CFnt();
	virtual ~CFnt();
	virtual void Serialize( CArchive& ar );

	StrokeList m_list;
	CQuad m_quadInit;			//初始时边框大小
	CQuad m_quadLimit;			//实际边框
	CProperty m_property;

	CDot m_dotBase;				//插入点
	CString m_strText;			//字符串

	double m_dbHeight;			//字高
	double m_dbCharSpace;		//字间距
	double m_dbLineSpace;		//行间距
	double m_dbWidthFactor;		//宽度系数

	int m_nPath;				//路径方式
	double m_dbDirection;		//方向
	int m_nAlign;				//对齐方式
	double m_dbRadius;			//半径
	double m_dbStart;			//开始角
	BOOL m_bCCW;				//是否逆时针
	BOOL m_bInside;				//字符朝内
	BOOL m_bFixSpace;			//固定间距设置
	CString m_strPrecisK;       //字体精度因子

public:
	void MarkPreview(CDC *pDC, const int &nPreviewSpeed);
	CQuad CalLimit();
	CQuad GetMarkRange();

	void MoveStroke(double moveX, double moveY);
		
	void SetProperty(int mode,CProperty pty);
	void Effect();
	int GetMarkUnit();
	UINT Mark(HWND hWnd);
	UINT SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsResMark = FALSE);
	void Draw(XDC *pDC, BOOL bSort);
	void Rotate(CDot dot,double angle);
	void Scale(CDot dot,double ratioX,double ratioY);
	void Move(double moveX,double moveY);
	void Scan(StrokeList *pList);
	void Delete();
	BOOL CreateStrokeList();
	virtual BOOL MakeStroke(){return FALSE;};
	void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	CQuad CalLimit(CDot dot,double dbXScale=1.0,double dbYScale=1.0);
};

#endif // !defined(AFX_FNT_H__DB4DFD01_FEA4_11D4_A486_00105A615525__INCLUDED_)
