// DsfDlgInsertObj.cpp : implementation file
//

#include "stdafx.h"
#include "HL.h"
#include "DsfDlgInsertObj.h"
#include "afxdialogex.h"
#include "DsfDlgEditIO.h"


// CDsfDlgInsertObj dialog

IMPLEMENT_DYNAMIC(CDsfDlgInsertObj, CDialogEx)

CDsfDlgInsertObj::CDsfDlgInsertObj(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDsfDlgInsertObj::IDD, pParent)
	, m_nObjType(0)
	, m_nSortType(0)
	, m_nTime(0)
	, m_strInput(_T(""))
	, m_strItMask(_T(""))
	, m_strOtMask(_T(""))
	, m_strOutput(_T(""))
	, m_nInput(0)
	, m_nItMask(0)
	, m_nOutput(0)
	, m_nOtMask(0)
{
	m_strInput.Format("0x%08X", m_nInput);
	m_strItMask.Format("Mask 0x%08X", m_nItMask);
	m_strOutput.Format("0x%08X", m_nOutput);
	m_strOtMask.Format("Mask 0x%08X", m_nOtMask);
}

CDsfDlgInsertObj::~CDsfDlgInsertObj()
{
}

void CDsfDlgInsertObj::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_OBJTYPE, m_nObjType);
	DDX_Radio(pDX, IDC_RADIO_SORTTYPE, m_nSortType);
	DDX_Text(pDX, IDC_EDIT_TIME, m_nTime);
	DDX_Text(pDX, IDC_STATIC_INPUT, m_strInput);
	DDX_Text(pDX, IDC_STATIC_ITMASK, m_strItMask);
	DDX_Text(pDX, IDC_STATIC_OTMASK, m_strOtMask);
	DDX_Text(pDX, IDC_STATIC_OUPUT, m_strOutput);
}


BEGIN_MESSAGE_MAP(CDsfDlgInsertObj, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_EDITINPUT, &CDsfDlgInsertObj::OnBnClickedBtnEditinput)
	ON_BN_CLICKED(IDC_BTN_EDITOUTPUT, &CDsfDlgInsertObj::OnBnClickedBtnEditoutput)
	ON_BN_CLICKED(IDOK, &CDsfDlgInsertObj::OnBnClickedOk)
END_MESSAGE_MAP()


// CDsfDlgInsertObj message handlers


void CDsfDlgInsertObj::OnBnClickedBtnEditinput()
{
	UpdateData(TRUE);
	CDsfDlgEditIO dlg;
	dlg.m_strTitle.LoadString(IDS_DSFOBJ_WAITINPUT);
	dlg.m_nValue = m_nInput;
	dlg.m_nMask = m_nItMask;
	if (dlg.DoModal() == IDOK)
	{
		m_nInput = dlg.m_nValue;
		m_nItMask = dlg.m_nMask;
		m_strInput.Format("0x%08X", m_nInput);
		m_strItMask.Format("Mask 0x%08X", m_nItMask);
		UpdateData(FALSE);
	}
}


void CDsfDlgInsertObj::OnBnClickedBtnEditoutput()
{
	UpdateData(TRUE);
	CDsfDlgEditIO dlg;
	dlg.m_strTitle.LoadString(IDS_DSFOBJ_DOOUTPUT);
	dlg.m_nValue = m_nOutput;
	dlg.m_nMask = m_nOtMask;
	if (dlg.DoModal() == IDOK)
	{
		m_nOutput = dlg.m_nValue;
		m_nOtMask = dlg.m_nMask;
		m_strOutput.Format("0x%08X", m_nOutput);
		m_strOtMask.Format("Mask 0x%08X", m_nOtMask);
		UpdateData(FALSE);
	}
}


void CDsfDlgInsertObj::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	CDialogEx::OnOK();
}

void CDsfDlgInsertObj::Check()
{
	switch(m_nSortType)
	{
	case 0:
		m_nSortType = DSFST_OBJ_BEGIN;
		break;
	case 1:
		m_nSortType = DSFST_OBJ_END;
		break;
	case 2:
		m_nSortType = DSFST_DOC_BEGIN;
		break;
	case 3:
		m_nSortType = DSFST_DOC_END;
		break;
	default:
		m_nSortType = DSFST_OBJ_BEGIN;
		break;
	}
	switch(m_nObjType)
	{
	case 0:
		m_nObjType = DSFOBJ_TIMER;
		break;
	case 1:
		m_nObjType = DSFOBJ_DOOUPUT;
		break;
	case 2:
		m_nObjType = DSFOBJ_WAITINPUT;
		break;
	default:
		m_nObjType = DSFOBJ_TIMER;
		break;
	}
}
