// Layer.h: interface for the CLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYER_H__3BF58B01_85A6_11D4_A484_00105A615525__INCLUDED_)
#define AFX_LAYER_H__3BF58B01_85A6_11D4_A484_00105A615525__INCLUDED_

#include "Property.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//��꼤����Ʋ�����
typedef struct LaserPara
{	
	double dbCount;		//������
	double dbJumpSpeed; //�����ٶ�(mm/s)
	double dbMarkSpeed; //����ٶ�(mm/s)

	double nLaserOnDelay;  //���⿪��ʱ(us)
	double nLaserOffDelay; //�������ʱ(us)

	double dbJumpDelay;		//��ת��ʱ(us)
	double dbMarkDelay;		//�߱���ʱ(us)
	double dbPolygonDelay;	//������ʱ(us)
	double dbDotDelay;		//�������ʱ(us)

	double dbQFreq;		//QƵ(KHz)
	double dbQRelease;	//Q�ͷ�(us)
	double dbCurrent;	//����(A)
	double dbFpkTime;	//����������ʱ��(us)

	double dbFoucusType;	//��������
	double dbCountDelay;	//������ʱ
	double dbTmp[128-15];

	LaserPara()
	{
		memset(this,0,sizeof(LaserPara));
	}
	~LaserPara()
	{
	}
	void operator=(const LaserPara& src)
	{
		if (&src == this)
		{
			return;
		}
		memcpy(this, &src, sizeof(LaserPara));
	}

	BOOL operator == ( const LaserPara & src )
	{
		if( &src == this )	
		{
			return TRUE;
		}
		if (memcmp(this, &src, sizeof(LaserPara)) == 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}PARA;

class CLayer : public CObject  
{
	DECLARE_SERIAL(CLayer)

public:
	CLayer();
	CLayer(int nType);
	virtual ~CLayer();
	virtual void Serialize( FILE* fp,BOOL bFlag );
	CLayer(const CLayer& src);
	void operator=(const CLayer& src);

public:
	void UpdatePara();
	CProperty GetProperty();

	void SetColor(COLORREF color);

	int	 m_nLayer;
	COLORREF m_color;
	BOOL m_bDefault;
	BOOL m_bLockFlag;
	PARA m_para;
//	int m_nCount;//������
};

#endif // !defined(AFX_LAYER_H__3BF58B01_85A6_11D4_A484_00105A615525__INCLUDED_)
