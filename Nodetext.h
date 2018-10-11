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

	CUIntArray	m_ArrayTextID;		//�ı�ID
	//�����ı�
	int		m_nAlign;		//���뷽ʽ
	int		m_nPath;		//���з�ʽ���绡�Σ�ֱ��
	double	m_dbDirection;	//ֱ�߷�ʽ�µķ���
	double	m_dbRadius;		//���η�ʽ�µİ뾶
	BOOL	m_bCCW;			//���η�ʽ�µ�ʱ�뷽��
	double	m_dbStart;		//���η�ʽ�µĿ�ʼ�Ƕ�
	BOOL	m_bInside;		//���η�ʽ�µ��ַ�����

	BOOL	m_bCustomFormat;//�Ƿ����ָ���ı�����
	CString m_strFormat;	//���ָ���ı����ݵĸ�ʽ
	CString m_strPrecisK;	//���徫������

	double	m_dbCharSpace;	//�ּ��
	double	m_dbWidthFactor;//���ϵ��
	double	m_dbHeight;		//�ָ߶�
	double	m_dbLineSpace;	//�м��
	
	int		m_nFont;		//���巽ʽ
	CString m_strFontCHS;	//SHX��������
	CString m_strFontEN;	//SHXӢ������
	CString m_strFontNUM;	//SHX��������
	LOGFONT m_lf;			//TTF����
	int		m_HzkType;		//��ǰѡ�еĵ����ֿ�����
	int		m_AscType;		//��ǰѡ�еĵ���Ӣ���ֿ�

	CString m_strText;		//��Ҫ��ʾ���ı�����

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
	
	//����ͨ��NodeText������ȫ�ֵ�Ctext	�ܻ�	20180625
	void UpadateMgrText();

private:
	void FormatString(CString& strText);
	BOOL CheckString();
};

#endif // !defined(AFX_NODETEXT_H__EE3D19A2_41FD_11D4_A483_00105A615525__INCLUDED_)
