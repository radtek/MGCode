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
	CQuad m_quadInit;			//��ʼʱ�߿��С
	CQuad m_quadLimit;			//ʵ�ʱ߿�
	CProperty m_property;

	CDot m_dotBase;				//�����
	CString m_strText;			//�ַ���

	double m_dbHeight;			//�ָ�
	double m_dbCharSpace;		//�ּ��
	double m_dbLineSpace;		//�м��
	double m_dbWidthFactor;		//���ϵ��

	int m_nPath;				//·����ʽ
	double m_dbDirection;		//����
	int m_nAlign;				//���뷽ʽ
	double m_dbRadius;			//�뾶
	double m_dbStart;			//��ʼ��
	BOOL m_bCCW;				//�Ƿ���ʱ��
	BOOL m_bInside;				//�ַ�����
	BOOL m_bFixSpace;			//�̶��������
	CString m_strPrecisK;       //���徫������

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
