// NodeText.h: interface for the CNodeText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_)
#define AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Dot.h"
#include "Fnt.h"
#include "Tree.h"
#include "Quad.h"

class CNodeText : public CNode
{
public:
	void AddToPlt(CNode *pNodePlt);
	virtual CNode * Copy(UINT nStyle=0x00);
	virtual BOOL Click(CPoint &point,CWnd *pWnd);
	virtual void SetProperty(int mode,CProperty pty);
	virtual void Scale(CDot dot,double ratioX,double ratioY);
	virtual BOOL Move(double moveX,double moveY);
	virtual void Rotate(CDot dot,double angle);
	virtual void Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY);
	virtual BOOL PreprocessMark(DWORD nStyle);
	virtual void Mark(HWND hWnd, int nFocusType);
	virtual void SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsResMark = FALSE);
	virtual CQuad CalLimit();
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual void Draw(XDC *pDC);
	virtual void ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100);
	virtual BOOL Create(CRect &rect);
	virtual void Scan(StrokeList *pList);		
	virtual BOOL CanGroup();
	virtual CNode *ReDefineCopy();
	virtual CQuad GetMarkRange();//yehq add 2008/04/29 for v5.1.34
	virtual void DrawExtern(XDC *pDC);

	CNodeText();

	virtual ~CNodeText();
	virtual void Serialize(FILE* fp,BOOL bFlag);

public:
	void MarkPreview(CDC *pDC, const int &nPreviewSpeed);
	virtual void SetStrokeList(StrokeList *pStrokeList);
	CDot	m_dotBase;
	CDot	m_dotLT;
	CDot	m_dotRB;

	CUIntArray	m_ArrayTextID;		//文本ID
	//跳号文本
	int		m_nAlign;		//对齐方式
	int		m_nPath;		//排列方式，如弧形，直线
	double	m_dbDirection;	//直线方式下的方向
	double	m_dbRadius;		//弧形方式下的半径
	BOOL	m_bCCW;			//弧形方式下的时针方向
	double	m_dbStart;		//弧形方式下的开始角度
	BOOL	m_bInside;		//弧形方式下的字符朝内

	BOOL	m_bCustomFormat;//是否输出指定文本内容
	CString m_strFormat;	//输出指定文本内容的格式
	CString m_strPrecisK;	//字体精度因子

	double	m_dbCharSpace;	//字间距
	double	m_dbWidthFactor;//宽度系数
	double	m_dbHeight;		//字高度
	double	m_dbLineSpace;	//行间距
	
	int		m_nFont;		//字体方式
	CString m_strFontCHS;	//SHX中文字体
	CString m_strFontEN;	//SHX英文字体
	CString m_strFontNUM;	//SHX数字字体
	LOGFONT m_lf;			//TTF字体
	int		m_HzkType;		//当前选中的点阵字库类型
	int		m_AscType;		//当前选中的点阵英文字库

	CString m_strText;		//需要显示的文本内容

	BOOL	m_bFixSpace;

	CFnt *m_pFnt;
	BOOL CreateString(BOOL bNext);
	CFnt * CreateFnt();
	bool m_needUpdateStroke;

public:
	BOOL IsBarText();
	void CreateFromBar(UINT nText);
	void CopyToMe(CNodeText* pSrc);
	virtual void SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill);
	BOOL GetNodeFirstDot(CDot &dt);
	BOOL GetNodeLastDot(CDot &dt);
	virtual void GetStrokeInQuad(CQuad &quad, StrokeList &strokeList);
	void DelTextByID(UINT nTextID);
	
	//尝试通过NodeText来更新全局的Ctext	周欢	20180625
	void UpadateMgrText();

private:
	void FormatString(CString& strText);
	BOOL CheckString();
};

#endif // !defined(AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_)
