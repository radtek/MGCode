// FntHzk.h: interface for the CFntHzk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FNTHZK_H__8096AEF0_EF47_4F8E_ABD9_304C971C269C__INCLUDED_)
#define AFX_FNTHZK_H__8096AEF0_EF47_4F8E_ABD9_304C971C269C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stroke.h"
#include "Fnt.h"
// #include "IndexFile.h"

class CFntHzk  : public CFnt
{
	DECLARE_SERIAL(CFntHzk)

public:
	void SortDotList_V();
	//void Delete();
	CFntHzk();
	virtual ~CFntHzk();
	virtual void Serialize( CArchive& ar );
	virtual BOOL MakeStroke();

	BOOL OpenHzk(int type, int AscType);
	int	 PutHz(int x, int y, int z, char *p, int type, int AscType);
	int  GetBit(unsigned char c, int n);
	void AddtoDotList(double x, double y, BOOL bRollBack = TRUE);
	void SaveInitDotList(double x, double y);

	void ChangeWorkDirect(int nDir);
	void ChangeFlyDirect(int nDir);

	int		m_Handle;
	int		m_HandleASC;
	BOOL	m_bFirst;
	CStroke *m_pStrokeCur;		//��ǰ��stroke

	int		m_HzkType;
	int		m_AscType;

	StrokeList	m_InitDotList;	//�����ʼ���ɵĵ��б�
	int			m_curWorkDir;	//��ǰ����̨����
	int			m_curFlyDir;	//��ǰ���з���

	int			m_DotLeft, m_DotRight;
	int			m_DotTop;
	int			m_DotBottom;
	BOOL		m_bSaveFirst;

	//��������ֿ�
//	CUserFontFile *m_pUserFile;

};

#endif // !defined(AFX_FNTHZK_H__8096AEF0_EF47_4F8E_ABD9_304C971C269C__INCLUDED_)
