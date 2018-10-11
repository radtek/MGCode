// Layer.h: interface for the CLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAYER_H__3BF58B01_85A6_11D4_A484_00105A615525__INCLUDED_)
#define AFX_LAYER_H__3BF58B01_85A6_11D4_A484_00105A615525__INCLUDED_

#include "Property.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//打标激光控制参数集
typedef struct LaserPara
{	
	double dbCount;		//打标次数
	double dbJumpSpeed; //空跳速度(mm/s)
	double dbMarkSpeed; //打标速度(mm/s)

	double nLaserOnDelay;  //激光开延时(us)
	double nLaserOffDelay; //激光关延时(us)

	double dbJumpDelay;		//跳转延时(us)
	double dbMarkDelay;		//走笔延时(us)
	double dbPolygonDelay;	//拐弯延时(us)
	double dbDotDelay;		//点出光延时(us)

	double dbQFreq;		//Q频(KHz)
	double dbQRelease;	//Q释放(us)
	double dbCurrent;	//电流(A)
	double dbFpkTime;	//首脉冲抑制时间(us)

	double dbFoucusType;	//焦点类型
	double dbCountDelay;	//次数延时
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
//	int m_nCount;//打标次数
};

#endif // !defined(AFX_LAYER_H__3BF58B01_85A6_11D4_A484_00105A615525__INCLUDED_)
