// DsfDlgEditIO.cpp : implementation file
//

#include "stdafx.h"
#include "HL.h"
#include "DsfDlgEditIO.h"
#include "afxdialogex.h"


// CDsfDlgEditIO dialog

IMPLEMENT_DYNAMIC(CDsfDlgEditIO, CDialogEx)

CDsfDlgEditIO::CDsfDlgEditIO(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDsfDlgEditIO::IDD, pParent)
	, m_strTitle(_T(""))
	, m_strValue1(_T(" 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15"))
	, m_strValue2(_T("16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31"))
	, m_strMask1(_T(" 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15"))
	, m_strMask2(_T("16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31"))
	, m_strValue(_T(""))
	, m_strMask(_T(""))
{
	memset(m_bMasks, 0, 32);
	memset(m_bValues, 0, 32);
}

CDsfDlgEditIO::~CDsfDlgEditIO()
{
}

void CDsfDlgEditIO::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TITLE, m_strTitle);
	DDX_Text(pDX, IDC_STATIC_VALUE1, m_strValue1);
	DDX_Text(pDX, IDC_STATIC_VALUE2, m_strValue2);
	DDX_Text(pDX, IDC_STATIC_MASK1, m_strMask1);
	DDX_Text(pDX, IDC_STATIC_MASK2, m_strMask2);
	DDX_Check(pDX, IDC_CHECK1, m_bValues[0]);
	DDX_Check(pDX, IDC_CHECK2, m_bValues[1]);
	DDX_Check(pDX, IDC_CHECK3, m_bValues[2]);
	DDX_Check(pDX, IDC_CHECK4, m_bValues[3]);
	DDX_Check(pDX, IDC_CHECK5, m_bValues[4]);
	DDX_Check(pDX, IDC_CHECK6, m_bValues[5]);
	DDX_Check(pDX, IDC_CHECK7, m_bValues[6]);
	DDX_Check(pDX, IDC_CHECK8, m_bValues[7]);
	DDX_Check(pDX, IDC_CHECK9, m_bValues[8]);
	DDX_Check(pDX, IDC_CHECK10, m_bValues[9]);
	DDX_Check(pDX, IDC_CHECK11, m_bValues[10]);
	DDX_Check(pDX, IDC_CHECK12, m_bValues[11]);
	DDX_Check(pDX, IDC_CHECK13, m_bValues[12]);
	DDX_Check(pDX, IDC_CHECK14, m_bValues[13]);
	DDX_Check(pDX, IDC_CHECK15, m_bValues[14]);
	DDX_Check(pDX, IDC_CHECK16, m_bValues[15]);
	DDX_Check(pDX, IDC_CHECK17, m_bValues[16]);
	DDX_Check(pDX, IDC_CHECK18, m_bValues[17]);
	DDX_Check(pDX, IDC_CHECK19, m_bValues[18]);
	DDX_Check(pDX, IDC_CHECK20, m_bValues[19]);
	DDX_Check(pDX, IDC_CHECK21, m_bValues[20]);
	DDX_Check(pDX, IDC_CHECK22, m_bValues[21]);
	DDX_Check(pDX, IDC_CHECK23, m_bValues[22]);
	DDX_Check(pDX, IDC_CHECK24, m_bValues[23]);
	DDX_Check(pDX, IDC_CHECK25, m_bValues[24]);
	DDX_Check(pDX, IDC_CHECK26, m_bValues[25]);
	DDX_Check(pDX, IDC_CHECK27, m_bValues[26]);
	DDX_Check(pDX, IDC_CHECK28, m_bValues[27]);
	DDX_Check(pDX, IDC_CHECK29, m_bValues[28]);
	DDX_Check(pDX, IDC_CHECK30, m_bValues[29]);
	DDX_Check(pDX, IDC_CHECK31, m_bValues[30]);
	DDX_Check(pDX, IDC_CHECK32, m_bValues[31]);
	DDX_Check(pDX, IDC_CHECK33, m_bMasks[0]);
	DDX_Check(pDX, IDC_CHECK34, m_bMasks[1]);
	DDX_Check(pDX, IDC_CHECK35, m_bMasks[2]);
	DDX_Check(pDX, IDC_CHECK36, m_bMasks[3]);
	DDX_Check(pDX, IDC_CHECK37, m_bMasks[4]);
	DDX_Check(pDX, IDC_CHECK38, m_bMasks[5]);
	DDX_Check(pDX, IDC_CHECK39, m_bMasks[6]);
	DDX_Check(pDX, IDC_CHECK40, m_bMasks[7]);
	DDX_Check(pDX, IDC_CHECK41, m_bMasks[8]);
	DDX_Check(pDX, IDC_CHECK42, m_bMasks[9]);
	DDX_Check(pDX, IDC_CHECK43, m_bMasks[10]);
	DDX_Check(pDX, IDC_CHECK44, m_bMasks[11]);
	DDX_Check(pDX, IDC_CHECK45, m_bMasks[12]);
	DDX_Check(pDX, IDC_CHECK46, m_bMasks[13]);
	DDX_Check(pDX, IDC_CHECK47, m_bMasks[14]);
	DDX_Check(pDX, IDC_CHECK48, m_bMasks[15]);
	DDX_Check(pDX, IDC_CHECK49, m_bMasks[16]);
	DDX_Check(pDX, IDC_CHECK50, m_bMasks[17]);
	DDX_Check(pDX, IDC_CHECK51, m_bMasks[18]);
	DDX_Check(pDX, IDC_CHECK52, m_bMasks[19]);
	DDX_Check(pDX, IDC_CHECK53, m_bMasks[20]);
	DDX_Check(pDX, IDC_CHECK54, m_bMasks[21]);
	DDX_Check(pDX, IDC_CHECK55, m_bMasks[22]);
	DDX_Check(pDX, IDC_CHECK56, m_bMasks[23]);
	DDX_Check(pDX, IDC_CHECK57, m_bMasks[24]);
	DDX_Check(pDX, IDC_CHECK58, m_bMasks[25]);
	DDX_Check(pDX, IDC_CHECK59, m_bMasks[26]);
	DDX_Check(pDX, IDC_CHECK60, m_bMasks[27]);
	DDX_Check(pDX, IDC_CHECK61, m_bMasks[28]);
	DDX_Check(pDX, IDC_CHECK62, m_bMasks[29]);
	DDX_Check(pDX, IDC_CHECK63, m_bMasks[30]);
	DDX_Check(pDX, IDC_CHECK64, m_bMasks[31]);
	DDX_Text(pDX, IDC_STATIC_VALUE, m_strValue);
	DDX_Text(pDX, IDC_STATIC_MASK, m_strMask);
}


BEGIN_MESSAGE_MAP(CDsfDlgEditIO, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDsfDlgEditIO::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK3, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK4, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK5, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK6, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK7, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK8, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK9, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK10, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK11, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK12, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK13, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK14, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK15, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK16, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK17, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK18, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK19, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK20, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK21, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK22, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK23, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK24, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK25, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK26, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK27, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK28, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK29, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK30, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK31, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK32, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK33, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK34, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK35, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK36, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK37, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK38, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK39, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK40, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK41, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK42, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK43, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK44, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK45, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK46, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK47, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK48, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK49, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK50, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK51, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK52, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK53, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK54, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK55, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK56, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK57, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK58, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK59, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK60, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK61, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK62, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK63, &CDsfDlgEditIO::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK64, &CDsfDlgEditIO::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CDsfDlgEditIO message handlers


BOOL CDsfDlgEditIO::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i=0; i<32; i++)
	{
		m_bValues[i] = (m_nValue&(1<<i))>>i;
	}
	for (int i=0; i<32; i++)
	{
		m_bMasks[i] = (m_nMask&(1<<i))>>i;
	}
	m_strValue.Format("0x%08X", m_nValue);
	m_strMask.Format("0x%08X", m_nMask);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDsfDlgEditIO::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_nValue = 0;
	for (int i=0; i<32; i++)
	{
		m_nValue |= m_bValues[i]<<i;
	}
	m_nMask = 0;
	for (int i=0; i<32; i++)
	{
		m_nMask |= m_bMasks[i]<<i;
	}
	CDialogEx::OnOK();
}


void CDsfDlgEditIO::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	m_nValue = 0;
	for (int i=0; i<32; i++)
	{
		m_nValue |= m_bValues[i]<<i;
	}
	m_nMask = 0;
	for (int i=0; i<32; i++)
	{
		m_nMask |= m_bMasks[i]<<i;
	}
	m_strValue.Format("0x%08X", m_nValue);
	m_strMask.Format("0x%08X", m_nMask);
	UpdateData(FALSE);
}
