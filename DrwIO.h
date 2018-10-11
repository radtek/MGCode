
#ifdef DRWIO_H
#else
#define DRWIO_H extern "C" __declspec(dllexport)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h> 
#define PI	3.1415926535

#define CAOUT_POINT		0
#define CAOUT_LINE		1
#define CAOUT_CIRCLE	2
#define CAOUT_ARC		3  


/////////////////////////////////////////
typedef struct tag_POINTOUT
{
	double  x;
	double  y;
}POINTOUT, *PPOINTOUT;

struct LINEARC : public CObject
{
	int       nObjType;
	POINTOUT  pointstart;
	POINTOUT  pointend;
	POINTOUT  centerpoint;
	double    radius;
	int       nDir;  //·½Ïò:0-Ë³;1-Äæ
};

struct ENT : public CObject
{
	char        layername[64];
	CArray<LINEARC*, LINEARC*> linearcList;
};

typedef CTypedPtrList<CObList, ENT *> DXFOBJOUTARC_LIST;


DRWIO_H BOOL TranslateDXFObj( CString strFileName, double dbprecision, DXFOBJOUTARC_LIST &entlist, CString strFontPath);
DRWIO_H void  DeleteOutObj( DXFOBJOUTARC_LIST* pentlist );
