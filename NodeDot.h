// NodeDib.h: interface for the CNodeDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(CNODEDOT_HEAD)
#define CNODEDOT_HEAD

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"

class CNodeDot : public CNode  //�߾�У�����ר����
{
public: 
	CNodeDot();
	CNodeDot(double dbBox,int row,int column,int iDotTime);
	virtual ~CNodeDot();	
public:
	virtual BOOL Track(CWnd* pWnd, CPoint ptOrg);
	virtual CQuad CalLimit();
	virtual void Draw(CDC *pDC);
	virtual void Mark(HWND hWnd);
	virtual void Serialize(FILE* fp,BOOL bFlag);

protected:
	double m_dbBox; //Box��С
	int m_iRow; //����
	int m_iCol; //����
	int m_iDotTime;//����ͣ��ʱ��
};

#endif // !defined(AFX_NODEDIB_H__3083ECE1_1673_11D5_8738_80564FC10108__INCLUDED_)
