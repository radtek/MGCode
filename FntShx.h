// FntShx.h: interface for the CFntShx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FNTSHX_H__9A216A01_FEAF_11D4_A486_00105A615525__INCLUDED_)
#define AFX_FNTSHX_H__9A216A01_FEAF_11D4_A486_00105A615525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Fnt.h"

const int cad_vec[][2]={
	{2,0},{2,1},{2,2},{1,2},{0,2},{-1,2},
	{-2,2},{-2,1},{-2,0},{-2,-1},{-2,-2},
	{-1,-2},{0,-2},{1,-2},{2,-2},{2,-1},
};	//every dirction of acad shx code

class CFntShx : public CFnt  
{
	struct ENSHX_HEAD
	{
		char c1[24];
		unsigned __int16 begin_ch;
		unsigned __int16 end_ch;
		unsigned __int16 total_ch;
	};
	
	struct ENSHX_INDEX
	{
		unsigned __int16 ch_asc;
		unsigned __int16 ch_bytes;
	};
	
	struct FONT_INFO
	{
		unsigned __int8 ascent;
		unsigned __int8 descent;
		unsigned __int16 style;
	};
	
	struct BIGSHX_HEAD
	{
		unsigned __int16 number_index;
		unsigned __int16 number_esc;
	};
	
	struct BIGSHX_INDEX
	{
		unsigned __int16 code;	//��������
		unsigned __int16 len;	//����
		__int32 pos;			//λ��
	};

	DECLARE_SERIAL(CFntShx)

public:
	CFntShx();
	virtual ~CFntShx();
	virtual void Serialize( CArchive& ar );

public:
	CString m_strChineseFont;	//��������
	CString m_strEnglishFont;	//Ӣ������
	CString m_strNumberFont;	//��������
	virtual BOOL MakeStroke();
	BOOL MakeFixStroke();		//�̶����
	BOOL MakeOldStroke();		//�Ϸ�ʽ�����ȼ��

protected:
	double m_dbUnitEnglish;
	double m_dbUnitNumber;
	double m_dbUnitEnshx;
	double m_dbUnitBigshx;

	CStroke *m_pStrokeCur;		//��ǰ��stroke

	CDot m_dotCur;			//�ʵ�ǰͣ��λ��
	CDot m_dotPrev;			//����һλ��
	BOOL m_bFirst;
	ENSHX_HEAD m_EnshxHeadEnglish;
	ENSHX_HEAD m_EnshxHeadNumber;
	ENSHX_HEAD m_EnshxHead;
	ENSHX_INDEX * m_pIndexEnglish;
	ENSHX_INDEX * m_pIndexNumber;
	ENSHX_INDEX * m_pIndexEnshx;

	BIGSHX_HEAD m_BigshxHead;
	DWORD m_posBigshxIndexStart;

	CFile *m_pFileChinese;
	CFile *m_pFileNumber;
	CFile *m_pFileEnglish;
	CFile *m_pFile;

	void process(unsigned __int8 letter);	//Ӣ�Ļ����ֵĴ���
	void process(unsigned __int16 letter);	//���Ļ���ַ��Ĵ���
	void CreateArc(CDot dotCenter,double r,double angleStart,double angle);
	void CreateLine();
	__int8 decode(unsigned __int8 code);
};

#endif // !defined(AFX_FNTSHX_H__9A216A01_FEAF_11D4_A486_00105A615525__INCLUDED_)
