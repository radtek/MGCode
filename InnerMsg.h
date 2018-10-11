#ifndef		__INNERMSG_H__
#define		__INNERMSG_H__

#include "Dot.h"

typedef struct tagQuadSG
{
	double left;
	double right;
	double top;
	double bottom;

	tagQuadSG()
	{
		memset(this, 0, sizeof(tagQuadSG));
	};

	double Width()
	{
		return right - left;
	}

	double Height()
	{
		return top - bottom;
	}

	CDot Center()
	{
		CDot dot;
		dot.x = (left + right)  / 2;
		dot.y = (top  + bottom) / 2;
		return dot;
	}

	void operator = ( const tagQuadSG & src )
	{
		if( &src == this )	
		{
			return;
		}

		left = src.left;
		right = src.right;
		top = src.top;
		bottom = src.bottom;
	}
}QUADSG;

// 对齐方式
enum ENU_ALIGNTYPE
{
	CmdEqualWidth,
	CmdEqualHeight,
	CmdEqualWh,
	CmdDistanceX,
	CmdDistanceY,
	CmdAlignLeft,
	CmdAlignRight,
	CmdAlignTop,
	CmdAlignBottom
};


enum ENU_HLMARKMODE
{
	ENU_MARKNORMALL,
	ENU_MARKSPECPARASET,
	ENU_MARKSEL,

	ENU_MARKFASTFPKNORMALL,
	ENU_MARKFASTFPKLINE
};


typedef struct DOT6
{
	double x;
	double y;
	float  z;
	float  i;
	float  b1;
	float  b2;

	DOT6()
	{
		memset(this, 0, sizeof(DOT6));
	};

	void operator = ( const DOT6 & src )
	{
		if( &src == this )	
		{
			return;
		}

/*		x = src.x;
		y = src.y;
		z = src.z;
		i = src.i;
		b1  = src.b1;
		b2  = src.b2;
*/
		memcpy(this, &src, sizeof(DOT6));
	}

} DOT6;

//仅包含编辑软件用参数
typedef struct tagSysCfg
{
	tagSysCfg()
	{
		memset(this, 0, sizeof(tagSysCfg));
	};

	double m_dbDotMoveStepLen;  // 点精确移动步长
    double m_dbDotFilterPrecis; // 滤点精确
    double m_dbDotFilterAngle;  // 滤点角度
	double m_dbMicro;           // 微调
	double m_dbClick;           // 拾取精度
	double m_dbOneToTwo;        // 断点精度
	int    m_nUndoCount;        // 回滚次数

	double m_dbExtendSpace;     // 缩扩间距
	double m_dbKillAngleSpace;  // 导角-定点交叉点之间的间距

	double m_dbGraphImportPrics;    // 图形导入精度

	double m_dbDrawEllipSet;    // 画圆精度
	int    m_nGridCol;         // 网格行
    int    m_nGridRow;         // 网格列
	
	int   m_nPolygon;          // 网格行
    int   m_nPolyNum;          // 网格列
	int   m_nPolyCross;        // 网格行
    int   m_nPolyStar;         // 网格列

	int   m_nLayerCnt;         // 层数量

	double m_dbPreViewZoomScale; // 预览鼠标滑动比例

	double m_dbJoinDistance;
} SYSCFG;

typedef struct tagSortNode
{
	tagSortNode()
	{
		this->m_pTree = NULL;
		this->m_nSort = 0;
		this->m_MNum = 0;
		this->m_Color = 0;
		this->m_nNewSort = 0;
	}

	~tagSortNode()
	{
		this->m_pTree = NULL;
		this->m_nSort = 0;
		this->m_MNum = 0;
		this->m_Color = 0;
		this->m_nNewSort = 0;
	}

	void operator = ( const tagSortNode & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		m_SPt.x = src.m_SPt.x;
		m_SPt.y = src.m_SPt.y;
		m_pTree = src.m_pTree;
		m_nSort = src.m_nSort;
		m_MNum  = src.m_MNum;
		m_Color = src.m_Color;
		m_nNewSort = src.m_nNewSort;
	}
	

	CDot    m_SPt;       // 标示范围起点
	DWORD   m_pTree;     // 排序对象指针
	int     m_nSort;     // 当前序号
	int     m_MNum;      // 标注点个数, 有重点时，当前点的序号
	COLORREF m_Color;    // 标注框颜色
	int     m_nNewSort;  // 新序

} SORTNODE;


typedef struct tagASNode
{
	tagASNode()
	{
		this->m_pTree = NULL;
		this->m_nSort = 0;
	}

	~tagASNode()
	{
		this->m_pTree = NULL;
		this->m_nSort = 0;
	}

	void operator = ( const tagASNode & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		m_SDt   = src.m_SDt;
		m_EDt   = src.m_EDt;
		m_pTree = src.m_pTree;
		m_nSort = src.m_nSort;
	}
	
	CDot    m_SDt;       // 标示范围起点
	CDot    m_EDt;       // 标示范围终点
	DWORD   m_pTree;     // 排序对象指针
	int     m_nSort;     // 当前序号

} ASNODE;

typedef struct tagADrawNode
{
	tagADrawNode()
	{
	}

	~tagADrawNode()
	{
	}

	void operator = ( const tagADrawNode & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		m_SDt   = src.m_SDt;
		m_EDt   = src.m_EDt;
	}
	
	CDot    m_SDt;       // 标示范围起点
	CDot    m_EDt;       // 标示范围终点

} ADRAWNODE;

typedef struct tagGNode
{
	tagGNode()
	{
		m_nNum  = 0;
		m_pGroupObj = 0;
		m_dbDistanc = -1;
		m_bIsSort   = FALSE;
	}

	~tagGNode()
	{
	}

	void operator = ( const tagGNode & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		m_nNum   = src.m_nNum;
		m_pGroupObj  = src.m_pGroupObj;
        m_rt     = src.m_rt;
		m_bIsSort= src.m_bIsSort;
		m_dbDistanc = src.m_dbDistanc;
	}
	
	int     m_nNum;
	BOOL    m_bIsSort;
	DWORD   m_pGroupObj;
	double  m_dbDistanc; // 与线段起点距离
	QUADSG  m_rt;        // 标示范围起点

} GNODE;

typedef struct tagGroupInfo
{
	tagGroupInfo()
	{ 
		m_nNum      = -1;
		m_pTreeList = NULL;
	}

	int    m_nNum;      // 分区序号
	CDot   m_CDot;      // 分区中心点坐标
	int    m_nCount;    // 分区Tree的数目
	DWORD *m_pTreeList; // 分区中的Tree列表
} GROUPINFO;

typedef struct SCANER_CONTROL_PARA
{
	//PCI3000卡振镜控制特性参数
	unsigned short MaxStep;
	unsigned short MaxBStep;
	unsigned short XGoto;
	unsigned short YGoto;
}SCANCTRLPARA;

//打标参数结构
typedef struct tagLaserPara
{
	tagLaserPara()
	{
		memset(this, 0, sizeof(tagLaserPara));
	};

	DWORD dwSize;         // 本结构体大小
	DWORD dwEver;         // 版本号-本版本为100
	int   nCount;         // 打标次数
	int   nLaserOffDelay; // 激光关延时
	int   nLaserOnDelay;  // 激光开延时
	BOOL  bCW;            // 是否连续激光
	int   nAvailDelay;    // 有效矢量步间延时
	int   nEmptyDelay;    // 空矢量步间延时
	int   nMarkDelay;     // 振镜到位延时
	int   nJumpDelay;     // 跳转延时
	int   nStrokeDelay;   // 拐弯延时
	double dbQFreq;       // 激光释放频率
	double dbQRelease;    // 激光释放时间
	double dbAvailStep;   // 有效矢量步长
	double dbEmptyStep;   // 空矢量步长
	double dbCurrent;     // 层电流参数
} LASERPARA;


typedef struct tagDOUBLEOUTPARA
{	
	tagDOUBLEOUTPARA()
	{
		memset(this,0,sizeof(tagDOUBLEOUTPARA));
	    dbLineWidth=0.1;
	};

	void operator = ( const tagDOUBLEOUTPARA & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		dbLeftOutAliX   = src.dbLeftOutAliX;
		dbLeftOutAliY   = src.dbLeftOutAliY;
        dbRightOutAliX  = src.dbRightOutAliX;
		dbRightOutAliY  = src.dbRightOutAliY;
		bDWFlag         = src.bDWFlag;
		dbLineWidth     = src.dbLineWidth;
		pos             = src.pos;
	}

	double	dbLeftOutAliX;
	double	dbLeftOutAliY;
	double	dbRightOutAliX;
	double	dbRightOutAliY;
	BOOL    bDWFlag;
	double dbLineWidth;

	CDot pos;
}DOUBLEOUTPARA;

typedef struct tagRightInfo
{	
	tagRightInfo()
	{
		memset(this,0,sizeof(tagRightInfo));
	};
	
	DWORD   dwLength;
	int     nUserCount;
} RIGHTINFO;

typedef struct tagUserInfo
{	
	tagUserInfo()
	{
		memset(this,0,sizeof(tagUserInfo));
	};

	void operator = ( const tagUserInfo & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		dwRight1   = src.dwRight1;
		dwRight2   = src.dwRight2;
		bIsDefaultUser = src.bIsDefaultUser;
		strncpy_s(szUserName, 64, src.szUserName, 64);
		strncpy_s(szPassWord, 64, src.szPassWord, 64);
	}

	char    szUserName[64];
	char    szPassWord[64];
	BOOL    bIsDefaultUser;
	// Bit 0:管理员;   1:通用打标; 2:多文档打标; 3:创建对象; 4:对象编辑
	//     5:系统设置; 6:校正设置; 7:文件保存;   8:打开文档; 9:层参数设置
	//    10:高级参数
	DWORD   dwRight1;
	DWORD   dwRight2;
} USERINFO;


typedef struct tagWToC
{	
	double   dbW;
	double   dbC;

	tagWToC()
	{
		memset(this,0,sizeof(tagWToC));
	};

	void operator = ( const tagWToC & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		dbW   = src.dbW;
		dbC   = src.dbC;
	}

} WTOC;


typedef struct tagSubObjMarkSet
{	
	tagSubObjMarkSet()
	{
		bIsMarkFram   = FALSE;
		bIsMarkFill   = FALSE;
        bIsFillFirst  = FALSE;
		nLayer        = 0;
		nSubCnt       = 1;
		bIsResMark    = FALSE;
	};

	void operator = ( const tagSubObjMarkSet & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		bIsMarkFram   = src.bIsMarkFram;
		bIsMarkFill   = src.bIsMarkFill;
        bIsFillFirst  = src.bIsFillFirst;
		nLayer        = src.nLayer;
		nSubCnt       = src.nSubCnt;
		bIsResMark    = src.bIsResMark;
	}

	int 	nSubCnt;
	int     nLayer;
	BOOL    bIsMarkFram;
	BOOL 	bIsMarkFill;
	BOOL 	bIsFillFirst;
	BOOL 	bIsResMark;
} SUBOBJMARKSET;

typedef struct tagObjMarkSet
{	
	tagObjMarkSet()
	{
		nMarkCount = 0;
	};

	void operator = ( const tagObjMarkSet & src )
	{
		if( &src == this )	
		{
			return;
		}
		
		nMarkCount   = src.nMarkCount;
		memcpy(stSub, src.stSub, sizeof(SUBOBJMARKSET) * OBJ_MAX_MARKCOUNT);
		if (nMarkCount > OBJ_MAX_MARKCOUNT) 
		{
			nMarkCount = OBJ_MAX_MARKCOUNT; // 最多只能打六遍
		}
	}

	int           nMarkCount;
	SUBOBJMARKSET stSub[OBJ_MAX_MARKCOUNT];
	
}OBJMARKSET;

typedef struct tagTextInfo
{	
	tagTextInfo()
	{
		nType = 0;
        nTexID = 0;
	};

	void operator = ( const tagTextInfo & src )
	{
		if( &src == this )	
		{
			return;
		}
		nType = src.nType;
        nTexID = src.nTexID;
	}

	UINT nType;
	UINT nTexID;
	
} TEXTINFO;

typedef struct objPos
{
	double x;
	double y;
	double z;
	CDot   Lf;
	CDot   Rf;

	objPos()
	{
		memset(this, 0, sizeof(objPos));
	};

	void operator = ( const objPos & src )
	{
		if( &src == this )	
		{
			return;
		}

		memcpy(this, &src, sizeof(objPos));
	}

} OBJPOS;

typedef struct ArrayMark
{
	QUADSG qd;
	CDot   Lf;
	CDot   Rf;
	double rota;

	ArrayMark()
	{
		memset(this, 0, sizeof(ArrayMark));
	};

	void operator = ( const ArrayMark & src )
	{
		if( &src == this )	
		{
			return;
		}

		memcpy(this, &src, sizeof(ArrayMark));
	}

} ARRAYMARK;


#endif