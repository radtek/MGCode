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
	int     m_nMarkTimes;	//������
	BOOL	m_bHex;			//ʮ�����Ʊ�ʶ
	BOOL	m_bRand;
	int		m_nJumpDis;
	int		m_iDataset;
	CString m_strText;		//��ǰ�ı�
	
	int		m_nText;		//�ı���ʽ����̶��ı��������ı��������ı�
	CString m_strDate;
	CString m_strFix;
	CStringArray m_saSerial;//������������
	
	//�����ı�
	BOOL m_bFill;			//�Ƿ���
	CString m_strPrefix;	//����λ���ַ�
	CString m_strFromFile;	//����ȡ�����ļ�֮����
	int m_nJumpStart;		//���ֿ�ʼ
	int m_nJumpCurrent;		//���ŵ�ǰֵ
	int m_nJumpEnd;			//���ֽ���
	int m_nJumpLen;			//���Ų��ֳ���
	int m_nJumpStep;		//����ʱ����
	//int m_nJumpInterval;	//���ż��
	int m_nFromFile;		//�Ƿ�����ȡ�����ļ�
	CString m_strLeft;      //����ǰ���ַ���
	CString m_strRight;     //���ź����ַ���
	BOOL m_bUse;			//�����ڱ���ʱȥ��������õ��ı���Ϣ

	UINT m_nTextID;
	CString m_strBinText;	//�����ı�����

	//���������ı���������ѡ���ı�ʱ���ܶ�������޸�ֻ���޸����ı�����
	//ֻ����ѡ������ʱ���ܶԱ����������޸�
	BOOL m_bBarCode;		//�Ƿ��������ı�
	BYTE* m_pBarCodeBin;	//�������������
	UINT m_nBarType;		//��������
	CString m_strBarString;	//�����ִ�

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
