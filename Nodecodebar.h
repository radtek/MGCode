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
	virtual CString GetParitybit(CString markcode){return "";};//�õ�У��λ

	virtual void DeleteFill();
	virtual BOOL GetBarcode();

	virtual void DrawExtern(XDC *pDC);
	
public:
	BOOL GenBarCode(UINT nStyle);
	void AddToPlt(CNode *pNodeplt);
	BARTYPE m_BarType;
	UINT m_nTextID;			//����ID
	double m_dbFillSpace;	//�����
	BOOL m_bRev;			//ȷ�������Ƿ���
    BOOL m_bGoBack;			//������
    BOOL m_bConnect;		//������
	double m_dbRevWidth;	//��������ʱ�Ŀ��
	double m_dbAngle;		//����Ƕ�
	BOOL	m_bTextVis;		//Ŀǰ���õ���

	DWORD m_nSingleLineMarkCount;
	UINT  m_nNarrowLineNum;

	BYTE* m_pByte;			//�����������Ϣ
	UINT m_nCol;			//��������
	UINT m_nRow;			//��������

	double  m_dbshrink;		//����������
	double	m_dbHeightBar;	//����߶�
	int     m_nOutline;		//PDF417��������
	int     m_nErrLev;		//����������QR,DATAMATRIX
	int		m_nHPoint;		//��ֱ��������,����QR,DATAMATRIX
	int		m_nWPoint;		//ˮƽ��������,����DATAMATRIX
	BOOL	m_bAutoSet;		//�������볤������;//true �Զ�����;FALSE �û�����
	int		m_nAlign;		//�������ַ��� 1�����;2�м����;3�Ҷ���
	double  m_narrowW;		//������������ʱ,��¼��ǰ����խ�����

	CDot m_dotBase;			//�����׼��
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

	double m_dbSpace;		//����EAN8��EAN13����˵����ڳ���֮��
							//���ȶ̵ĳ����ټ�Ϊ�˲���,Ŀǰĩ�����л�����
							//���ó��������棬����ԭ����ֵ��0.6MM��
	double m_dbcodeRate;	//��ֵ�̶�Ϊ2.75
};

#endif // !defined(AFX_NODECODEBAR_H__E79C9B81_6EC5_11D4_9565_00105A6155CA__INCLUDED_)

