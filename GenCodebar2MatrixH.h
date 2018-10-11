#ifndef __GEN_CODEBAR_2MATRIX_H_H__
#define __GEN_CODEBAR_2MATRIX_H_H__
/////////////////////////////////////////////////////////////
/*////////////////////////////////////////////////////////////
		GenCodebar2Matrix.dll 用于对常用2维码生成进行封装，
	以便简化调用者。
	Designed by Vinca		-------2006/7/28
////////////////////////////////////////////////////////////*/
/*///////////////////////////////////////////////////////////
////////约定：所有接口参数：内存均由函数调用者进行分配与释放
////////导出函数返回值意义：
	0		——	操作成功
	////
	-1		——	未知错误
	-2		——	入口参数指针非法
	-3		——	条码类型不支持
	-4		——	入口参数非法
	-5		——	条码库初始化失败
	-6		——	条码生成失败，未知错误
	-7		——	条码生成失败，入口参数部分不匹配条码生成规则，具体参考出口参数中某些值意义
	////
	1		——	条码生成失败，预分配内存不够，具体参考出口参数中某些值意义
///////////////////////////////////////////////////////////*/
//结构体定义
//QR条码入口、出口参数定义
typedef struct IQR{
	////输入参数
	struct{
		ULONG _versionType : 5;//版本类型：1~~20
		ULONG _levelType : 3;//纠错级别：1~~4
		ULONG _bConver : 1;//是否反填(1表示反填，0表示不反填)
		ULONG _countsConver : 10;//反填时，周边外扩最小单元个数
		ULONG _reserved : 13;//保留
	} _bits;
	BYTE* _lpszSrc;//条码生成数据
	long _sizeSrcBytes;//条码生成数据字节长度
	////输出值，生成函数返回值意义：
				//-7：表示生成失败，条码生成数据长度大于当前设置的版本类型与纠错级别所允许的最大长度，
					//此时：_sizeRetMaxSrcBytes中为最大允许的条码生成数据的字节个数
				//1：_lpszRetBuffer字节容量不够，此时，_sizeRetBytes为必须分配的字节大小
	long _sizeRetMaxSrcBytes;//当生成函数返回0或-7时，表示与版本类型、纠错级别对应的最大允许的条码生成数据的字节个数
	long _sizeRetBytes;//返回生成的数据字节长度
	long _sizeRetRowsOrCols;//返回矩阵的行数或列数(行、列相同)
	long _sizeRetBuffer;//缓冲区字节长度
	BYTE* _lpszRetBuffer;//缓冲区
} IQR;

//DataMatrix条码入口、出口参数定义
typedef struct IDATAMATRIX{
	////输入参数
	struct{
		ULONG _eccType : 3;//纠错类型，默认：ECC--200(5)，取值范围：ECC--000(0)直到ECC--200(5)
		ULONG _outStyle : 1;//条码外型：0表示正方形，1表示长方形
		ULONG _bUserDefined : 1;//用户自定义长宽模块数(1时，_widthNums和_heightNums有效)
		ULONG _bConver : 1;//是否反填(1表示反填，0表示不反填)
		ULONG _countsConver : 10;//反填时，周边外扩最小单元个数
		ULONG _reserved : 16;//保留
	} _bits;
	long _widthNums;//自定义模块宽度
	long _heightNums;//自定义模块高度
	BYTE* _lpszSrc;//条码生成数据
	long _sizeSrcBytes;//条码生成数据字节长度
	////输出值，生成函数返回值意义：
				//-7：表示生成失败，此时：_sizeRetRows、_sizeRetCols为自定义条码高度、宽度必须设定的值
				//1：_lpszRetBuffer字节容量不够，此时，_sizeRetBytes为必须分配的字节大小
	long _sizeRetBytes;//返回生成的数据字节长度
	long _sizeRetRows;//返回矩阵的行数
	long _sizeRetCols;//返回矩阵的列数
	long _sizeRetBuffer;//缓冲区字节长度
	BYTE* _lpszRetBuffer;//缓冲区
} IDATAMATRIX;

//PDF417条码入口、出口参数定义
typedef struct IPDF417{
	////输入参数
	struct{
		ULONG _eccLevel : 4;//纠错级别(0~8)
		ULONG _nRows : 7;//417条码行数(0或3~~90)
		ULONG _nCols : 5;//417条码列数(0或1~30)
		ULONG _bConver : 1;//是否反填(1表示反填，0表示不反填)
		ULONG _countsConver : 10;//反填时，周边外扩最小单元个数
		ULONG _reserved : 5;//保留
	} _bits;
	long _xScale;//X模块宽度个数
	long _yScale;//Y模块高度个数
	BYTE* _lpszSrc;//条码生成数据
	long _sizeSrcBytes;//条码生成数据字节长度
	////输出值，生成函数返回值意义：
	//1：_lpszRetBuffer字节容量不够，此时，_sizeRetBytes为必须分配的字节大小
	long _sizeRetBytes;//返回生成的数据字节长度
	long _sizeRetRows;//返回矩阵的行数
	long _sizeRetCols;//返回矩阵的列数
	long _sizeRetBuffer;//缓冲区字节长度
	BYTE* _lpszRetBuffer;//缓冲区
} IPDF417;

//导出条码生成函数接口入口、出口参数定义
typedef struct IMATRIXBAR{
	long _type;//1QR码，2DataMatrix码，3Pdf417码
	union{
		IQR _qr;
		IDATAMATRIX _dataMatrix;
		IPDF417 _pdf417;
	} _u_value;
} IMATRIXBAR;

//////导出接口申明
class IGenCodebar2Matrix{
public:
	////获取错误信息
	LPCSTR GetErrInfo(long errNo, long nResourceType){
		if( !LoadLib() )
			return (LPCSTR) NULL;
		return (((LPCSTR (__stdcall*)(long,long)) m_pFunc[0])(errNo,nResourceType));
	}
	////生成条码
	long GenCodebar(LPVOID lpValueSrcRet){
		if( !LoadLib() )
			return -1;
		return (((long (__stdcall*)(LPVOID)) m_pFunc[1])(lpValueSrcRet));
	}
	////
	IGenCodebar2Matrix(){
		Clear();
	}
	~IGenCodebar2Matrix(){
		ReleaseLib();
	}
	void ReleaseLib(){
		if( m_hLib ) ::FreeLibrary(m_hLib);
		Clear();
	}

private:
	void Clear(){
		memset(this,0,sizeof(*this));
	}
	BOOL LoadLib(){
		if( m_hLib )
			return TRUE;
		m_hLib=::LoadLibrary(TEXT("GenCodebar2Matrix.dll"));
		if( !m_hLib ){
			OutputDebugString(TEXT("Can't find GenCodebar2Matrix.dll!\n"));
			return FALSE;
		}
		BOOL bOk=TRUE;
		for( int i=0;i<2;i++ ){
			m_pFunc[i]=(DWORD) ::GetProcAddress((HMODULE) m_hLib,(LPCSTR) (MAKELONG(i+1,0)));
			if( 0==m_pFunc[i] ) bOk=FALSE;
		}
		if( !bOk ){
			OutputDebugString(TEXT("Incorrect version of GenCodebar2Matrix.dll!\n"));
			ReleaseLib();
			return FALSE;
		}
		return TRUE;
	}
	
	////////////////
	DWORD m_pFunc[2];
	HINSTANCE m_hLib;
};
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
#endif
