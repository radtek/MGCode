// BarcodeSetLen.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HL.h"
#include "BarcodeSetLen.h"
#include "afxdialogex.h"


// CBarcodeSetLen �Ի���

IMPLEMENT_DYNAMIC(CBarcodeSetLen, CDialogEx)

CBarcodeSetLen::CBarcodeSetLen(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBarcodeSetLen::IDD, pParent)
	, m_dbBarcodeLen(0)
	, m_dbBarcodeWid(0)
	, m_bSameLen(TRUE)
{

}

CBarcodeSetLen::~CBarcodeSetLen()
{
}

void CBarcodeSetLen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LENGTH, m_dbBarcodeLen);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_dbBarcodeWid);
	DDX_Check(pDX, IDC_CHECK_SAMELEN, m_bSameLen);
}


BEGIN_MESSAGE_MAP(CBarcodeSetLen, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBarcodeSetLen::OnBnClickedOk)
END_MESSAGE_MAP()


// CBarcodeSetLen ��Ϣ�������


void CBarcodeSetLen::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
		
	CDialogEx::OnOK();
}


BOOL CBarcodeSetLen::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
