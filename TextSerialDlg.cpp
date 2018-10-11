// TextSerialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "TextSerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextSerialDlg dialog


CTextSerialDlg::CTextSerialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextSerialDlg::IDD, pParent)
	, m_bChageMarkCnt(FALSE)
{
	//{{AFX_DATA_INIT(CTextSerialDlg)
	m_strPrefix = _T("");
	m_strLeft = _T("");
	m_strRight = _T("");
	m_nJumpLen = 0;
	m_bFill = FALSE;
	m_nJumpStart = 0;
	//m_nJumpCurrent=0;
	m_nJumpEnd = 0;
	m_nFromFile = -1;
	m_nJumpStep = 0;
	m_nJumpInterval=1;
	m_strBinText = _T("");
	m_bChageMarkCnt=FALSE;
	//}}AFX_DATA_INIT
}


void CTextSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextSerialDlg)
	DDX_Control(pDX, IDC_COMBO_SERIAL, m_comboSerial);
	DDX_Control(pDX, IDC_EDIT_PREFIX, m_editPrefix);
	DDX_Text(pDX, IDC_EDIT_PREFIX, m_strPrefix);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_strLeft);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_strRight);
	DDX_Text(pDX, IDC_EDIT_LEN, m_nJumpLen);
	DDX_Check(pDX, IDC_CHECK_FILL, m_bFill);
	DDX_Text(pDX, IDC_EDIT_START, m_nJumpStart);
	DDX_Text(pDX,IDC_EDIT_CURRENT,*m_pCur);
	DDX_Text(pDX, IDC_EDIT_END, m_nJumpEnd);
	DDX_Radio(pDX, IDC_RADIO_CUSTOM, m_nFromFile);
	DDX_Text(pDX, IDC_EDIT_STEP, m_nJumpStep);
	DDX_Text(pDX,IDC_EDIT_INTERVAL,m_nJumpInterval);
	DDX_Text(pDX, IDC_EDIT_BINTEXT, m_strBinText);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK1, m_bChageMarkCnt);
}


BEGIN_MESSAGE_MAP(CTextSerialDlg, CDialog)
	//{{AFX_MSG_MAP(CTextSerialDlg)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, OnRadioCustom)
	ON_BN_CLICKED(IDC_RADIO_FROM, OnRadioFrom)
	ON_BN_CLICKED(IDC_CHECK_FILL, OnCheckFill)
	ON_EN_KILLFOCUS(IDC_EDIT_LEN, OnKillfocusEditLen)
	ON_EN_KILLFOCUS(IDC_EDIT_START, OnKillfocusEditStart)
	ON_EN_KILLFOCUS(IDC_EDIT_END, OnKillfocusEditEnd)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_CBN_SELCHANGE(IDC_COMBO_SERIAL, OnSelchangeComboSerial)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextSerialDlg message handlers

void CTextSerialDlg::OnRadioCustom() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateView();
	
}

void CTextSerialDlg::OnRadioFrom() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UpdateView();
	
}

void CTextSerialDlg::UpdateView()
{
	if(m_nFromFile)
	{
		GetDlgItem(IDC_COMBO_SERIAL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_RIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STEP)->EnableWindow(FALSE);
		
		GetDlgItem(IDC_CHECK_FILL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PREFIX)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_SERIAL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_END)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LEFT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_RIGHT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STEP)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FILL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PREFIX)->EnableWindow(TRUE);

		OnCheckFill();
	}
}

void CTextSerialDlg::OnCheckFill() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetDlgItem(IDC_EDIT_PREFIX)->EnableWindow(m_bFill);
	GetDlgItem(IDC_EDIT_LEN)->EnableWindow(m_bFill);
	UpdateData(FALSE);	
}

BOOL CTextSerialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_editPrefix.SetLimitText(1);
	SetSerial();
	UpdateView();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTextSerialDlg::OnKillfocusEditLen() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UINT lenMax=max(GetLength(m_nJumpStart),GetLength(m_nJumpEnd));
	if(m_nJumpLen<lenMax)
	{
		m_nJumpLen=lenMax;
		UpdateData(FALSE);
	}	
}

void CTextSerialDlg::OnKillfocusEditStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UINT lenMax=max(GetLength(m_nJumpStart),GetLength(m_nJumpEnd));
	if(m_nJumpLen<lenMax)
	{
		m_nJumpLen=lenMax;
		UpdateData(FALSE);
	}	
}

void CTextSerialDlg::OnKillfocusEditEnd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	UINT lenMax=max(GetLength(m_nJumpStart),GetLength(m_nJumpEnd));
	if(m_nJumpLen<lenMax)
	{
		m_nJumpLen=lenMax;
		UpdateData(FALSE);
	}	
}

int CTextSerialDlg::GetLength(int value)
{
	int i=1;
	while(value/=10)i++;
	return i;

}

void CTextSerialDlg::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	static char szFilter[]="Text File(*.txt)|*.txt|All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE,NULL,m_strFromFile,OFN_HIDEREADONLY,szFilter);
	
	dlg.m_ofn.lpstrInitialDir=m_strFromFile;
	if(dlg.DoModal()==IDOK)
	{	
		m_strFromFile=dlg.GetPathName();

		//加入到列表中
		m_comboSerial.ResetContent();
		CString str;
		int nItems;

		if(m_nFromFile)
		{	
			//取自于文件
			CStdioFile f;
			if(f.Open(m_strFromFile,CFile::modeRead|CFile::typeText))
			{
				//文件行数
				nItems=0;
				while(f.ReadString(str))nItems++;
				if(nItems>32767)
					nItems=32767;
				m_pSerial->SetSize(nItems);

				f.SeekToBegin(); 
				while(f.ReadString(str))GetSerial(str);
			}
			f.Close();
		}

		m_comboSerial.SetCurSel(0);	
		m_nJumpEnd = m_pSerial->GetSize();
		*m_pCur=m_comboSerial.GetCurSel();

//		//显示提示信息
//		nItems=m_comboSerial.GetCount();
//		CSerialProcessDlg dlg;
//		dlg.m_strCount.Format("%d",nItems);
//		m_comboSerial.GetLBText(0,dlg.m_strStart);
//		m_comboSerial.GetLBText(nItems-1,dlg.m_strEnd);
//		dlg.DoModal();

		//整理序列数据
		m_pSerial->SetSize(nItems);
	}		
	UpdateData(FALSE);
}

void CTextSerialDlg::GetSerial(LPCTSTR lpszString)
{
	int index;
	index=m_comboSerial.AddString(lpszString);
	if(index>=0)
	{
		m_comboSerial.SetCurSel(index);
		m_pSerial->SetAt(index,lpszString);
	}
}

void CTextSerialDlg::SetSerial()
{
	int count;
	count=m_pSerial->GetSize();
	for(int i=0;i<count;i++)
	{
		m_comboSerial.AddString(m_pSerial->GetAt(i));
		m_comboSerial.SetCurSel(i);
	}

	m_comboSerial.SetCurSel(*m_pCur);
}

void CTextSerialDlg::OnSelchangeComboSerial() 
{
	// TODO: Add your control notification handler code here
	*m_pCur=m_comboSerial.GetCurSel();
}

//允许改变markcnt?
void CTextSerialDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	GetDlgItem(IDC_EDIT_CURRENT)->EnableWindow(m_bChageMarkCnt);
}
