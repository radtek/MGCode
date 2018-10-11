// Lay.h: interface for the CLay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAY_H__F86EEAE1_863E_11D4_A484_00105A615525__INCLUDED_)
#define AFX_LAY_H__F86EEAE1_863E_11D4_A484_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Layer.h"

class CLay: public CObject  //Lay>Layer
{
	DECLARE_SERIAL(CLay)
  
public:
	void UpdateList();
	CLayer GetLayer(COLORREF color);

	int GetCountDelay(int nLay);
	int GetFocusType(int nLay);
	int GetRepeat(int nLay);
	CLayer GetCurLayer();

	CLayer GetLayer(int index);
	void   SetLayer(CLayer &layer);

	CLayer GetLayer(CProperty pty);
	int GetSize();
	int GetCur();
	void SetCur(int cur);

	void SetLayerColor(int index, COLORREF color);

	//Updated by AndyF.Shelby on 2 Mar. 2006 for SleepMode
	CLayer& GetDefaultLayer();

	void Set(int mode,CLayer ly);
//	void Del(int index);
	void Del(int nLayer);
	void AddNew();
	void Add(CLayer ly);
	void Serialize(FILE* fp,BOOL bFlag);

	BOOL GetLockFlg(int index);
	void SetLockFlg(int index, BOOL bLockFlg);

	CLay();
	virtual ~CLay();
	void operator=(const CLay& src);
	CLay(const CLay& src);

protected:
	CArray<CLayer,CLayer>m_list;		//层列表
	int m_nNext;						//增加时的下一层号
	int m_nCur;							//当前层

	//cyadd
public:
	int GetLayerNum(CProperty pty);
	PARA GetPara(int nLay);
};

#endif // !defined(AFX_LAY_H__F86EEAE1_863E_11D4_A484_00105A615525__INCLUDED_)
