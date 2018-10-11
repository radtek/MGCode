#ifndef __GEN_CODEBAR_1_DIM_H_H__
#define __GEN_CODEBAR_1_DIM_H_H__
//////////////////////////////////////////////////////////////
/*////////////////////////////////////////////////////////////
		GenCodebar1Dim.dll 用于对常用1维条码生成进行封装，
	以便简化调用者。
	Designed by Vinca		-------2006/9/05
////////////////////////////////////////////////////////////*/
/*////////////////////////////////////////////////////////////
////////约定：所有接口参数：内存均由函数调用者进行分配与释放
//////////////二进制字节中，bit7：1表示条，0表示空
//							bit6：1表示需加长处理，0表示不需加长
//					--------------在Code39码、Code39扩展码中其它Bit意义
//					------------------Bit1、Bit0意义：
//					-----------------------------00表示位空(即条码字符间隙)
//					-----------------------------01表示笮
//					-----------------------------10表示宽
//					--------------在Inter25码中其它Bit意义
//					------------------Bit1、Bit0意义：
//					-----------------------------01表示笮
//					-----------------------------10表示宽
//39码(包括扩展39码)资源文件说明：0笮空、1宽空、2笮条、3宽条
////////导出函数返回值意义：
//0		——	操作成功
//////
//-1		——	未知错误
//-2		——	入口参数指针非法
//-3		——	条码类型不支持
//-4		——	入口参数非法
//-5		——	条码库初始化失败，资源载入失败
//-6		——	条码生成失败，未知错误
//-7		——	条码生成失败，入口参数部分不匹配条码生成规则
//-8		——	条码资源错误
//-9		——	条码生成失败，入口参数长度不匹配(具体见各条码相关参数定义)
//////
//1			——	条码生成失败，预分配内存不够
////////////////////////////////////////////////////////////*/
//////////////////////////////////////////////////////////////
////Code128码、Ean128码说明：
////	4个转义字符：0xF1、0xF2、0xF3、0xF4、0xF5、0xF6、0xF7
////	0xF1表示CODEA，0xF2表示CODEB，0xF3表示CODEC，0xF4表示SHIFT
////	0xF5表示FNC1，0xF6表示FNC2，0xF7表示FNC3，0xF8表示FNC4
////注：首字符为转义字符，意义：0xF1表示STARTA，0xF2表示STARTB，0xF3表示STARTC
////	首字符不为转义字符，则采用Code128 Auto的规则决定开始字符：STARTA或STARTB或STARTC
////Code128 Auto码中不允许出现转义字符	
//////////////////////////////////////////////////////////////
//支持的一维条码类型
enum{
	eCodebar1Dim_UpcA=1,//UpcA
	eCodebar1Dim_UpcE,//UpcE
	eCodebar1Dim_Ean13,//EAN13
	eCodebar1Dim_Ean8,//EAN8
	eCodebar1Dim_Code128,//Code128
	eCodebar1Dim_Ean128,//EAN128
	eCodebar1Dim_Code128Auto,//Code128Auto
	eCodebar1Dim_Code39,//Code39
	eCodebar1Dim_Code39Ex,//Code39Ex
	eCodebar1Dim_Inter25,//Inter25
};

//导出条码生成函数接口入口、出口参数定义
typedef struct ICODEBAR1{
	long _type;//一维条码类型
	struct{
		////入口参数
		long _i_nSrcSize;//原字符串字符个数
		BYTE* _i_lpcbSrc;//原字符串内容
		long _i_nRetBinaryBuffSize;//二进制数据内存的字节大小
		long _i_nRetStrBuffSize;//完整字符串内存的字节大小
		union{//某些条码的额外附加入口参数
			DWORD _dw39;//39码附加参数，Bit0为1表示需要校验，为0则表示不需校验
			DWORD _dw25;//25码附加参数，Bit0为1表示需要校验，为0则表示不需校验
		} _uExtraInput;
		////出口参数(当返回-9错误时，_nRltBinarySize表示实际所需的入口条码字符串长度)
		long _o_nRltBinarySize;//返回生成的二进制字串的字节大小
		BYTE* _i_lpcbRetBinaryBuff;//返回生成的二进制字串
		long _o_nRltIndexStart;//指示_lpszSrc在_lpszRetStrBuff中的起始索引(基值0)
		long _o_nRltStrSize;//返回生成的完整字符串的字节大小
		BYTE* _i_lpcbRetStrBuff;//返回生成的完整字符串(只有在校验位时才有用否则不用)
	} _s_value;
} ICODEBAR1;

//////导出接口申明
class IGenCodebar1Dim{
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
	IGenCodebar1Dim(){
		Clear();
	}
	~IGenCodebar1Dim(){
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
		m_hLib=::LoadLibrary(TEXT("GenCodebar1Dim.dll"));
		if( !m_hLib ){
			OutputDebugString(TEXT("Can't find GenCodebar1Dim.dll!\n"));
			return FALSE;
		}
		BOOL bOk=TRUE;
		for( int i=0;i<2;i++ ){
			m_pFunc[i]=(DWORD) ::GetProcAddress((HMODULE) m_hLib,(LPCSTR) (MAKELONG(i+1,0)));
			if( 0==m_pFunc[i] ) bOk=FALSE;
		}
		if( !bOk ){
			OutputDebugString(TEXT("Incorrect version of GenCodebar1Dim.dll!\n"));
			ReleaseLib();
			return FALSE;
		}
		return TRUE;
	}
	
	////////////////
	DWORD m_pFunc[2];
	HINSTANCE m_hLib;
};
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
#endif
