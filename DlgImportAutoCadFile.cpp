// DlgImportAutoCadFile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "DlgImportAutoCadFile.h"
#include "afxdialogex.h"


// CDlgImportAutoCadFile �Ի���

IMPLEMENT_DYNAMIC(CDlgImportAutoCadFile, CDialogEx)

CDlgImportAutoCadFile::CDlgImportAutoCadFile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgImportAutoCadFile::IDD, pParent)
	, m_bFilterText(TRUE)
{

	m_nCadUnit = 0;
}

CDlgImportAutoCadFile::~CDlgImportAutoCadFile()
{
}

void CDlgImportAutoCadFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_CAD_UNIT, m_nCadUnit);
	DDX_Check(pDX, IDC_CHECK_FILTER_TEXT, m_bFilterText);
}


BEGIN_MESSAGE_MAP(CDlgImportAutoCadFile, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgImportAutoCadFile::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgImportAutoCadFile ��Ϣ�������


void CDlgImportAutoCadFile::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!UpdateData())
		return;

	switch (m_nCadUnit)
	{
	case 1:
		m_dbUnitTrans = 25.4;
		break;
	case 2:
		m_dbUnitTrans = 1;
		break;
	default:
		m_dbUnitTrans = 0;
	}


	CDialogEx::OnOK();
}
