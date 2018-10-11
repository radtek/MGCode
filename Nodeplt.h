// NodePlt.h: interface for the CNodePlt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODEPLT_H__82862A44_4819_11D4_A483_00105A615525__INCLUDED_)
#define AFX_NODEPLT_H__82862A44_4819_11D4_A483_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Property.h"	// Added by ClassView

class CNodePlt : public CNode  
{
public:
	CNodePlt();
	virtual ~CNodePlt();

public:
	virtual void Serialize(FILE* fp,BOOL bFlag);
	virtual BOOL CanUnGroup(){return TRUE;};
	virtual CTree * UnGroup();
	BOOL DeCode(CString file,BOOL bCenter,BOOL bBetter);
	CNodePlt* UnGroupplt(); 

private:
	void CreateCircle(CDot dotCenter, double dbRadius);
	void ReadCommand(char *cmd);	//ȡ�����֣��絽�ļ�β���򷵻�Ϊ"00"
	int ReadValue(double &Value);	//ȡ����ֵ������" ", ",",";"����ĸ����
	void Process();					//��������
	void CreatePen(int pen);

	CFile		*m_pFile;			//ָ���ļ���ָ��
	BOOL		m_bPen;				//���ʱ�ʶ
	CDot		m_dotCur;
	CStroke		*m_pStroke;			//��ǰ�ʻ�

	BOOL        m_bPenUp;			//��ʱ�ʶ
	BOOL        m_bCoorA;			//���������ʶ
	BOOL        m_bCircle;			//��Բ��ʶ

};

#endif // !defined(AFX_NODEPLT_H__82862A44_4819_11D4_A483_00105A615525__INCLUDED_)
