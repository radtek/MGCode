// Text.h: interface for the CText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT_H__445AF437_EBB6_49DC_9087_48900DC5F3C1__INCLUDED_)
#define AFX_TEXT_H__445AF437_EBB6_49DC_9087_48900DC5F3C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CText : public CObject  
{
public:
	CText(UINT nTextID);
	virtual ~CText();
	void operator=(const CText& src);	

public:
	CString GetBarString();
	void GenString(DWORD nStyle);
	BOOL GenBarString(UINT nStyle);
	BOOL FlashNextText(DWORD nStyle);
	void Serialize(FILE* fp,BOOL bFlag);
	CString GetInfo();
	CString GetString(DWORD nStyle);
	void CopyToMe(CText src);
	int     m_nMarkTimes;	//打标次数
	BOOL	m_bHex;			//十六进制标识
	BOOL	m_bRand;
	int		m_nJumpDis;
	int		m_iDataset;
	CString m_strText;		//当前文本
	
	int		m_nText;		//文本方式，如固定文本，日期文本，跳号文本
	CString m_strDate;
	CString m_strFix;
	CStringArray m_saSerial;//跳号序列数据
	
	//跳号文本
	BOOL m_bFill;			//是否补齐
	CString m_strPrefix;	//补齐位数字符
	CString m_strFromFile;	//跳号取自于文件之名字
	int m_nJumpStart;		//数字开始
	int m_nJumpCurrent;		//跳号当前值
	int m_nJumpEnd;			//数字结束
	int m_nJumpLen;			//跳号部分长度
	int m_nJumpStep;		//跳号时步长
	//int m_nJumpInterval;	//跳号间隔
	int m_nFromFile;		//是否跳号取自于文件
	CString m_strLeft;      //跳号前置字符串
	CString m_strRight;     //跳号后置字符串
	BOOL m_bUse;			//用于在保存时去除多的无用的文本信息

	UINT m_nTextID;
	CString m_strBinText;	//进制文本内容

	//由于条码文本的内容在选中文本时不能对其进行修改只能修改其文本参数
	//只有在选中条码时才能对本参数进行修改
	BOOL m_bBarCode;		//是否是条码文本
	BYTE* m_pBarCodeBin;	//条码二进制数据
	UINT m_nBarType;		//条码类型
	CString m_strBarString;	//条码字串

	BOOL m_bOverMark;
	BOOL m_Changed;
private:
	BOOL GenBarCode();
	void CaseNum(CString& str);
	CString FormatString(int nVaL);

public:
	BOOL m_bChangeFlag;	
	BOOL m_bModel;
	CString m_strModelNo;
	BOOL ChangeText(CString strTxt,CString ModelNo);
};

#endif // !defined(AFX_TEXT_H__445AF437_EBB6_49DC_9087_48900DC5F3C1__INCLUDED_)
