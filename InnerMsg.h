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

// ���뷽ʽ
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

//�������༭����ò���
typedef struct tagSysCfg
{
	tagSysCfg()
	{
		memset(this, 0, sizeof(tagSysCfg));
	};

	double m_dbDotMoveStepLen;  // �㾫ȷ�ƶ�����
    double m_dbDotFilterPrecis; // �˵㾫ȷ
    double m_dbDotFilterAngle;  // �˵�Ƕ�
	double m_dbMicro;           // ΢��
	double m_dbClick;           // ʰȡ����
	double m_dbOneToTwo;        // �ϵ㾫��
	int    m_nUndoCount;        // �ع�����

	double m_dbExtendSpace;     // �������
	double m_dbKillAngleSpace;  // ����-���㽻���֮��ļ��

	double m_dbGraphImportPrics;    // ͼ�ε��뾫��

	double m_dbDrawEllipSet;    // ��Բ����
	int    m_nGridCol;         // ������
    int    m_nGridRow;         // ������
	
	int   m_nPolygon;          // ������
    int   m_nPolyNum;          // ������
	int   m_nPolyCross;        // ������
    int   m_nPolyStar;         // ������

	int   m_nLayerCnt;         // ������

	double m_dbPreViewZoomScale; // Ԥ����껬������

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
	

	CDot    m_SPt;       // ��ʾ��Χ���
	DWORD   m_pTree;     // �������ָ��
	int     m_nSort;     // ��ǰ���
	int     m_MNum;      // ��ע�����, ���ص�ʱ����ǰ������
	COLORREF m_Color;    // ��ע����ɫ
	int     m_nNewSort;  // ����

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
	
	CDot    m_SDt;       // ��ʾ��Χ���
	CDot    m_EDt;       // ��ʾ��Χ�յ�
	DWORD   m_pTree;     // �������ָ��
	int     m_nSort;     // ��ǰ���

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
	
	CDot    m_SDt;       // ��ʾ��Χ���
	CDot    m_EDt;       // ��ʾ��Χ�յ�

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
	double  m_dbDistanc; // ���߶�������
	QUADSG  m_rt;        // ��ʾ��Χ���

} GNODE;

typedef struct tagGroupInfo
{
	tagGroupInfo()
	{ 
		m_nNum      = -1;
		m_pTreeList = NULL;
	}

	int    m_nNum;      // �������
	CDot   m_CDot;      // �������ĵ�����
	int    m_nCount;    // ����Tree����Ŀ
	DWORD *m_pTreeList; // �����е�Tree�б�
} GROUPINFO;

typedef struct SCANER_CONTROL_PARA
{
	//PCI3000���񾵿������Բ���
	unsigned short MaxStep;
	unsigned short MaxBStep;
	unsigned short XGoto;
	unsigned short YGoto;
}SCANCTRLPARA;

//�������ṹ
typedef struct tagLaserPara
{
	tagLaserPara()
	{
		memset(this, 0, sizeof(tagLaserPara));
	};

	DWORD dwSize;         // ���ṹ���С
	DWORD dwEver;         // �汾��-���汾Ϊ100
	int   nCount;         // ������
	int   nLaserOffDelay; // �������ʱ
	int   nLaserOnDelay;  // ���⿪��ʱ
	BOOL  bCW;            // �Ƿ���������
	int   nAvailDelay;    // ��Чʸ��������ʱ
	int   nEmptyDelay;    // ��ʸ��������ʱ
	int   nMarkDelay;     // �񾵵�λ��ʱ
	int   nJumpDelay;     // ��ת��ʱ
	int   nStrokeDelay;   // ������ʱ
	double dbQFreq;       // �����ͷ�Ƶ��
	double dbQRelease;    // �����ͷ�ʱ��
	double dbAvailStep;   // ��Чʸ������
	double dbEmptyStep;   // ��ʸ������
	double dbCurrent;     // ���������
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
	// Bit 0:����Ա;   1:ͨ�ô��; 2:���ĵ����; 3:��������; 4:����༭
	//     5:ϵͳ����; 6:У������; 7:�ļ�����;   8:���ĵ�; 9:���������
	//    10:�߼�����
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
			nMarkCount = OBJ_MAX_MARKCOUNT; // ���ֻ�ܴ�����
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