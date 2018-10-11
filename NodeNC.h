// NodeNC.h: interface for the CNodeNC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODENC_H__3521A44_167_19D4_A425_00FD5A75CEQ__INCLUDED_)
#define AFX_NODENC_H__3521A44_167_19D4_A425_00FD5A75CEQ__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Node.h"
#include "Nodeline.h"
#include "Property.h"	// Added by ClassView

class CNodeNC : public CNode  
{
public:
	CNodeNC();
	virtual ~CNodeNC();

public:
	virtual BOOL SetLayerSelect(int nItem, BOOL bFlag);
	virtual void Serialize(FILE* fp,BOOL bFlag);
	virtual BOOL CanUnGroup();
	virtual void LoadWheelList(int nType, StrokeList *pListStroke);
	virtual CTree * UnGroup();
	BOOL    DeCode(CString file, BOOL bCenter, BOOL bBetter);
	CNodeLine* UnGroupplt(); 

private:
	void CreateCircle(CDot dotCenter, double dbRadius);
	void Process(CString strData);					//参数处理
	void CreatePen(int pen);

	CStdioFile		m_file;				//指向文件的指针
	BOOL			m_bFirst;			//换笔首笔
	CDot			m_dotCur;
	CStroke*		m_pStroke;			//当前笔画

	BOOL        m_bPenUp;			//笔抬起来，空走
	BOOL        m_bCoorA;			//绝对坐标标识
	BOOL        m_bCircle;			//画圆标识
	BOOL        m_bMMUint;          //公制尺寸 mm

};

#endif // !defined(AFX_NODENC_H__3521A44_167_19D4_A425_00FD5A75CEQ__INCLUDED_)
