#pragma once

enum DSFOBJTYPE
{
	DSFOBJ_TIMER = 13,
	DSFOBJ_DOOUPUT,
	DSFOBJ_WAITINPUT
};

enum DSFOBJSORTTYPE
{
	DSFST_DOC_BEGIN = 0,
	DSFST_DOC_END,
	DSFST_OBJ_BEGIN,
	DSFST_OBJ_END
};

class IListType
{
public:
	enum DSFLISTTYPE
	{
		LT_TREE,
		LT_DSFOBJ
	};
	virtual UINT GetListType() = 0;
	virtual void Mark(HWND hWnd) = 0;
	virtual void Serialize(FILE* fp,BOOL bSave) = 0;
};

class CDsfObj : public IListType
{
public:
	CDsfObj(void){}
	virtual ~CDsfObj(void){}
	virtual UINT GetDsfObjType(){return m_nDsfObjType;}
	virtual UINT GetListType(){return LT_DSFOBJ;}
	virtual void Mark(HWND hWnd) = 0;
	virtual void Serialize(FILE* fp,BOOL bSave) = 0;
protected:
	UINT m_nDsfObjType;
};

class CDsfTimer : public CDsfObj
{
public:
	CDsfTimer(void);
	virtual ~CDsfTimer(void);
	virtual void Mark(HWND hWnd);
	virtual void Serialize(FILE* fp,BOOL bSave);
	UINT GetTime(){return m_nTime;}
	void SetTime(UINT nTime){m_nTime=nTime;}
protected:
	UINT m_nTime;
};

class CDsfDoOutput : public CDsfObj
{
public:
	CDsfDoOutput(void);
	virtual ~CDsfDoOutput(void);
	virtual void Mark(HWND hWnd);
	virtual void Serialize(FILE* fp,BOOL bSave);
	void GetOutput(UINT &nOutput, UINT &nMask){nOutput=m_nOutPut;nMask=m_nMask;}
	void SetOutput(UINT nOutput, UINT nMask){m_nOutPut=nOutput;m_nMask=nMask;}
protected:
	UINT m_nOutPut;
	UINT m_nMask;
};

class CDsfWaitInput : public CDsfObj
{
public:
	CDsfWaitInput(void);
	virtual ~CDsfWaitInput(void);
	virtual void Mark(HWND hWnd);
	virtual void Serialize(FILE* fp,BOOL bSave);
	void GetOutput(UINT &nInput, UINT &nMask){nInput=m_nInput;nMask=m_nMask;}
	void SetOutput(UINT nInput, UINT nMask){m_nInput=nInput;m_nMask=nMask;}
protected:
	UINT m_nInput;
	UINT m_nMask;
};