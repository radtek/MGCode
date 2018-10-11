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
	void ReadCommand(char *cmd);	//取命令字，如到文件尾，则返回为"00"
	int ReadValue(double &Value);	//取参数值，遇到" ", ",",";"或字母结束
	void Process();					//参数处理
	void CreatePen(int pen);

	CFile		*m_pFile;			//指向文件的指针
	BOOL		m_bPen;				//换笔标识
	CDot		m_dotCur;
	CStroke		*m_pStroke;			//当前笔画

	BOOL        m_bPenUp;			//起笔标识
	BOOL        m_bCoorA;			//绝对坐标标识
	BOOL        m_bCircle;			//画圆标识

};

#endif // !defined(AFX_NODEPLT_H__82862A44_4819_11D4_A483_00105A615525__INCLUDED_)
