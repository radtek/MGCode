// TextObject.h: interface for the CTextObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTOBJECT_H__8BCD93FF_5479_494B_8727_968BBAD39E7E__INCLUDED_)
#define AFX_TEXTOBJECT_H__8BCD93FF_5479_494B_8727_968BBAD39E7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//用于做介于文本内容与字体属性的集合，只用于做文本设置时用到
//层次属性
//CTextObject 
// |____________________| ......
//CTextSet             CTextSet  此时我们看成一个整体，出了对话框后设置成各自独立的对象
// |_________|          |_________|
//CNodeText CText      CNodeText CText

#include <afxtempl.h>
#include "TextSet.h"

class CTextObject  
{
public:
	CTextObject();
	virtual ~CTextObject();

public:
	CTextSet* GetItem(UINT nIndex);
	void Delete();
	void DeleteItem(UINT nIndex);
	void AddText(CTextSet* pTextSet);
	CTypedPtrList<CObList,CTextSet*>m_list;		//文本数据列表
};

#endif // !defined(AFX_TEXTOBJECT_H__8BCD93FF_5479_494B_8727_968BBAD39E7E__INCLUDED_)
