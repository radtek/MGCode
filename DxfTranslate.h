// DxfTranslate.h: interface for the CDxfTranslate class.
//
//////////////////////////////////////////////////////////////////////

//*******************************************************************//
//Class interface for user interface of decoder v1.0,designed by 
//PengZili,Feb. 2005.
//phone:0755-86161316;E-mail:pzili@21cn.com
//copyright reserved by HAN'S LASER co. ltd.(Shen Zhen)
//*******************************************************************//

#if !defined(AFX_DXFTRANSLATE_H__1049F708_A23C_4EF5_9959_10ED958D1122__INCLUDED_)
#define AFX_DXFTRANSLATE_H__1049F708_A23C_4EF5_9959_10ED958D1122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//++++++++++++++++++++tagPOINTOUT+++++++++++++++++++++++++++++++++++++++++++++++//
//						输出点的表示											//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
typedef struct tagPOINTOUT
{
	DOUBLE x;
	DOUBLE y;
	DOUBLE z;
}POINTOUT;

//++++++++++++++++++++ 类CDxfStroke+++++++++++++++++++++++++++++++++++++++++++++++//
//						点的序列	   											  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class  CDxfStroke : public CObject
{
public:
	CArray<POINTOUT,POINTOUT>m_pointArray;
	CString m_nLayerName;
};


//++++++++++++++++++++ 类CDxfStroke3D+++++++++++++++++++++++++++++++++++++++++++++++//
//						点的序列	   											  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#define OUT_3DOUTLINE	0					// 输出外框
#define OUT_3DFACE		1					// 输出3dface, polyline face
class  CDxfStroke3D : public CObject
{
public:
	CString m_nLayerName;
	CArray<POINTOUT,POINTOUT>m_pointArray;	//点的序列
	int m_nType;							// 0:外框 1:3dface(由四个点组成)
};

// 3DFace
class CDxfStroke3D_3DFace : public CDxfStroke3D
{
public:										
	int m_nInvisEdgeFlag;					// 1:第一条边不可见 2:第二条边不可见 4:第三条边不可见 8:第四条边不可见
};

//++++++++++++++++++++ 类CDxfArcCirOut+++++++++++++++++++++++++++++++++++++++++++++++//
//						输出圆弧和圆	   											  //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#define CAOUT_BASE		0
#define CAOUT_LINE		1
#define CAOUT_CIRCLE	2
#define CAOUT_ARC		3

class CDxfArcCirOut : public CObject//bass class
{
public:
	CString m_nLayerName;				  //层号
	int m_nObjType;						  //类型
};

class CDxfArcCirOut_LINE: public CDxfArcCirOut//直线
{
public:
	CArray<POINTOUT,POINTOUT>m_pointArray;//点的序列
};

class CDxfArcCirOut_ARC : public CDxfArcCirOut//圆弧
{
public:
	DOUBLE	m_dbRadius;	//半径
	POINTOUT m_centerP;	//圆心
	POINTOUT m_startP;	//起点
	POINTOUT m_endP;	//终点
	INT m_nDire;		//方向:0-顺;1-逆	
};

class CDxfArcCirOut_CIRCLE : public CDxfArcCirOut//圆
{
public:
	DOUBLE	m_dbRadius;	//半径
	POINTOUT m_centerP;	//圆心
};

//++++++++++++++++++++输出接口1: 类CDxfTranslate+++++++++++++++++++++++++++++++++++++++++++++++//
//      接口1： Translate(CString flie,                //待解码的文件路径					//
//						  DOUBLE precision,			   //精度值	                            //
//						  CString strFontPath,		   //字库路径							//  
//						  )                                                                 //
//      接口2：CTypedPtrList<CObList,CDxfStroke *>m_DxfStrokeList 保存输出的Stroke序列      //
//      接口3：CTypedPtrList<CObList,CDxfArcCirOut *>m_DxfObjList 保存输出的Stroke序列       //
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
typedef CTypedPtrList<CObList,CDxfStroke *> DXFSTROKE_LIST;
typedef CTypedPtrList<CObList,CDxfArcCirOut *> DXFACOUT_LIST;
typedef CTypedPtrList<CObList,CDxfStroke3D *> DXFSTROKE_LIST3D; // 包括边框和3dface等模型
class AFX_EXT_CLASS CDxfTranslate  
{
public:
	CDxfTranslate();
	virtual ~CDxfTranslate(); 

	BOOL Translate(CString flie,DOUBLE precision,CString strFontPath);

	// nOutType:1只导外框 2:只导3DFace 3:导外框和3DFace
	// 导出的数据在m_DxfStrokeList3D中，类型以CDxfStroke3D中的m_nType区分
	BOOL Translate3D( CString flie,DOUBLE precision, CString strFontPath,int nOutType);

	DXFSTROKE_LIST m_DxfStrokeList;

	DXFACOUT_LIST m_DxfObjList;

	DXFSTROKE_LIST3D m_DxfStrokeList3D;
};

//++++++++++++++++++++输出接口2: TranslateDXF/DeleteObjStk+++++++++++++++++++++++++++++++++++++++++++++//
//	(Added by peng zili, 2006-08-21, phone: 86161387)
//	说明: 调用	TranslateDXF 进行Dxf文档解码, 确保数据使用完后调用(必须)DeleteObj删除对象
//	1. TranslateDXF
//	flie -		[IN] 待解码的文件名(路径)
//	precision -	[IN] 生成线条精度值			
//	dxfObjOut - [OUT]输出对象(以线条形式)
//	strFontPath-[IN] SHX字库路径(为空不能正确导入SHX字体)
//	RET - TRUE: 解码成功; FALSE - 解码失败
//	2. DeleteObjStk
//	dxfObjDel - [IN] 输出对象(以线条形式), TranslateDXF的输出(dxfObjOut)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
struct OBJ_OUTSTROKE : public CObject
{
	OBJ_OUTSTROKE()
	{
		m_bContinueStroke = TRUE;
	}
	DXFSTROKE_LIST lststkOut;	
	CString m_nLayerName;
	BOOL	m_bContinueStroke;
};

typedef CTypedPtrList<CObList, OBJ_OUTSTROKE*> DXFOBJOUTLIST_STROKE;

extern "C"
AFX_EXT_API BOOL TranslateDXF( CString flie, DOUBLE precision, DXFOBJOUTLIST_STROKE& dxfObjOut, CString strFontPath = _T("") );
extern "C"
AFX_EXT_API	void DeleteObjStk( const DXFOBJOUTLIST_STROKE& dxfObjDel );

//++++++++++++++++END 输出接口2+++++++++++++++++++++++++++++++++++

//++++++++++++++++++++输出接口3: TranslateDXF/DeleteOutObj+++++++++++++++++++++++++++++++++++++++++++++//
//	(Added by peng zili, 2007-04-10, phone: 86161387)
//	说明: 调用	TranslateDXFObj 进行Dxf文档解码, 以对象方式组织解码结果, 确保数据使用完后调用(必须)DeleteObj删除对象
//	1. TranslateDXFObj
//	flie -		[IN] 待解码的文件名(路径)
//	precision -	[IN] 生成线条精度值			
//	dxfObjOut - [OUT]输出对象(以线条形式)
//	strFontPath-[IN] SHX字库路径(为空不能正确导入SHX字体)
//	RET - TRUE: 解码成功; FALSE - 解码失败
//	2. DeleteOutObj
//	dxfObjDel - [IN] 输出对象(以线条形式), TranslateDXF的输出(dxfObjOut)
//
//	NOTE: 块等插入对象不保留其层次结构, 使用时需要打散
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
struct OBJ_OUTARC : public CObject
{
	DXFACOUT_LIST m_lstObjOutArc;
	CString m_strLayerName;
};
typedef CTypedPtrList<CObList, OBJ_OUTARC *> DXFOBJOUTARC_LIST;

extern "C"
AFX_EXT_API BOOL TranslateDXFObj( CString flie, DOUBLE precision, DXFOBJOUTARC_LIST& dxfObjOut, CString strFontPath = _T("") );
extern "C"
AFX_EXT_API	void DeleteOutObj( const DXFOBJOUTARC_LIST& dxfObjDel );

//++++++++++++++++END 输出接口3+++++++++++++++++++++++++++++++++++


#endif // !defined(AFX_DXFTRANSLATE_H__1049F708_A23C_4EF5_9959_10ED958D1122__INCLUDED_)
