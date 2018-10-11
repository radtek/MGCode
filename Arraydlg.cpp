// ArrayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ArrayDlg.h"
#include "ArrayposDlg.h"
#include "HLDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrayDlg dialog

#define HITBORDER 3
extern HICON g_hIcoList[20];

extern CHLApp theApp;

CArrayDlg::CArrayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArrayDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CArrayDlg)
	m_nArray   = 0;
	m_nRedMark =0;
	m_nColumn = 0;
	m_dbColumnDistance = 0.0;
	m_nRow = 0;
	m_dbRowDistance = 0.0;
	m_bAuto = FALSE;
	m_nRowFirst = 0;
	m_nRoundType = 1;
	m_dbRotate = 0.0;
	m_dbLX = 0.0;
	m_dbLY = 0.0;
	m_dbRX = 0.0;
	m_dbRY = 0.0;
	//}}AFX_DATA_INIT
}


void CArrayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CArrayDlg)
	DDX_Control(pDX, IDC_STATIC_OBJVIEW, m_objView);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Radio(pDX, IDC_RADIO_STANDARD, m_nArray);
	DDX_Text(pDX, IDC_EDIT_COLUMN, m_nColumn);
	DDX_Text(pDX, IDC_EDIT_COLUMN_DISTANCE, m_dbColumnDistance);
	DDX_Text(pDX, IDC_EDIT_ROW, m_nRow);
	DDX_Text(pDX, IDC_EDIT_ROW_DISTANCE, m_dbRowDistance);
	DDX_Check(pDX, IDC_CHECK_AUTO, m_bAuto);
	DDX_Radio(pDX, IDC_RADIO_ROW, m_nRowFirst);
	DDX_Radio(pDX, IDC_RADIO_TYPELINE, m_nRoundType);
	DDX_Text(pDX, IDC_EDIT_ROTATE, m_dbRotate);
	DDX_Text(pDX, IDC_EDIT_LX, m_dbLX);
	DDX_Text(pDX, IDC_EDIT_LY, m_dbLY);
	DDX_Text(pDX, IDC_EDIT_RX, m_dbRX);
	DDX_Text(pDX, IDC_EDIT_RY, m_dbRY);
	DDX_Radio(pDX, IDC_RADIO_RED, m_nRedMark);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CArrayDlg, CDialog)
	//{{AFX_MSG_MAP(CArrayDlg)
	ON_BN_CLICKED(IDC_RADIO_CUSTOM, OnRadioCustom)
	ON_BN_CLICKED(IDC_RADIO_STANDARD, OnRadioStandard)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_REFURBISH, OnButtonRefurbish)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, OnButtonRotate)
	ON_BN_CLICKED(IDC_BUTTON_IMPORTTEMPLATE, OnButtonImporttemplate)
	ON_BN_CLICKED(IDC_BUTTON_EXPORTTEMPLATE, OnButtonExporttemplate)
	ON_BN_CLICKED(IDC_RADIO_TEMPLATE, OnRadioTemplate)
	ON_BN_CLICKED(IDC_BUTTON_OFFSET, OnButtonOffset)
	ON_BN_CLICKED(IDC_BUTTON_MARK, OnButtonMark)
	ON_BN_CLICKED(IDC_RADIO_RED, OnRadioRed)
	ON_BN_CLICKED(IDC_RADIO_LASER, OnRadioLaser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CArrayDlg message handlers

void CArrayDlg::OnRadioCustom() 
{
	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	m_dtArray.RemoveAll();
	m_QuadList.RemoveAll();
	m_xzoom.m_wndView->DelAllStrk();
	m_xzoom.m_wndView->UpdateSelQuad();
	m_xzoom.m_wndView->Invalidate();

	UpdateData(TRUE);
	UpdateView();

	return;
}

void CArrayDlg::OnRadioStandard() 
{
	UpdateData(TRUE);
	UpdateView();

	return;
}

void CArrayDlg::OnRadioTemplate() 
{
	UpdateData(TRUE);
	UpdateView();

	return;
}

void CArrayDlg::OnButtonAdd() 
{
	int nItem = 0;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		nItem=m_list.GetItemCount();
	}
	else
	{
		nItem = m_list.GetNextSelectedItem(pos);
	}


	CArrayposDlg dlg;
	if(dlg.DoModal()==IDOK)
	{	
		CString str;
		str.Format("%f",dlg.m_dbX);
		m_list.InsertItem(nItem,str);

		str.Format("%f",dlg.m_dbY);
		m_list.SetItemText(nItem,1,str);

		str.Format("%f",dlg.m_dbR);
		m_list.SetItemText(nItem,2,str);
	}

	return;
}


void CArrayDlg::OnButtonDelete() 
{
	int nItem;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		nItem = m_list.GetNextSelectedItem(pos);
	}

	m_list.DeleteItem(nItem);
}

void CArrayDlg::OnButtonEdit() 
{
	int nItem = 0;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		return;
	}
	else
	{
		nItem = m_list.GetNextSelectedItem(pos);
	}
		
	CArrayposDlg dlg;
	CString str("");

	str=m_list.GetItemText(nItem,0);
	dlg.m_dbX=atof(str);
	str=m_list.GetItemText(nItem,1);
	dlg.m_dbY=atof(str);
	str=m_list.GetItemText(nItem,2);
	dlg.m_dbR=atof(str);

	if(dlg.DoModal()==IDOK)
	{
		str.Format("%f",dlg.m_dbX);
		m_list.SetItemText(nItem,0,str);
		str.Format("%f",dlg.m_dbY);
		m_list.SetItemText(nItem,1,str);
		str.Format("%f",dlg.m_dbR);
		m_list.SetItemText(nItem,2,str);
	}

	return;
}

void CArrayDlg::OnButtonLoad() 
{
	CString file;
	CHLApp *pApp=(CHLApp *)AfxGetApp();
	CString str;
	
	static char szFilter[]="Pos File(*.pos)|*.pos||";
	CFileDialog dlg(TRUE,"pos",NULL,OFN_HIDEREADONLY,szFilter);
	
	file=pApp->GetInstallPath();
	str.LoadString(IDS_ARRAY_LOAD);
	dlg.m_ofn.lpstrTitle=str;
	dlg.m_ofn.lpstrInitialDir=file;
	if(dlg.DoModal()==IDOK)
	{
		file=dlg.GetPathName();
		Load(file);
	}
}

void CArrayDlg::OnButtonRefurbish() 
{
	CHLDoc *pDoc = GetDocument();
	if (!pDoc)
	{
		return;
	}

//	CArray<DOT6, DOT6> dtArray;
//	dtArray.SetSize(50000, 10000);
//	pDoc->GetArrayDot(dtArray);

	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	m_dtArray.RemoveAll();
	m_QuadList.RemoveAll();
	pDoc->GetArrayDot(m_dtArray);
	m_xzoom.m_wndView->DelAllStrk();
	
	m_list.DeleteAllItems();
	CString str("");
	OBJPOS dt;
	int nSize = m_dtArray.GetSize();
	for(int i = 0; i < nSize; i++)
	{
		dt=m_dtArray[i];
		str.Format("%f",dt.x);
		m_list.InsertItem(i,str);
		str.Format("%f",dt.y);
		m_list.SetItemText(i,1,str);
		str.Format("%f",dt.z);
		m_list.SetItemText(i,2,str);
	}
//	dtArray.RemoveAll();

	return;
}

void CArrayDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString file;
	CHLApp *pApp=(CHLApp *)AfxGetApp();
	CString str;
	
	static char szFilter[]="Pos File(*.pos)|*.pos||";
	CFileDialog dlg(FALSE,"pos",NULL,OFN_OVERWRITEPROMPT,szFilter);
	
	file=pApp->GetInstallPath();
	str.LoadString(IDS_ARRAY_SAVE);
	dlg.m_ofn.lpstrTitle=str;
	dlg.m_ofn.lpstrInitialDir=file;
	if(dlg.DoModal()==IDOK)
	{
		file=dlg.GetPathName();
		Save(file);
	}
}

void CArrayDlg::UpdateView()
{
	switch(m_nArray)
	{
	case 0:
		{
			GetDlgItem(IDC_EDIT_ROW)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_ROW_DISTANCE)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_COLUMN_DISTANCE)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_TYPELINE)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_TYPES)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_ROW)->EnableWindow(TRUE);
			GetDlgItem(IDC_RADIO_COL)->EnableWindow(TRUE);

			GetDlgItem(IDC_LIST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_REFURBISH)->EnableWindow(FALSE);

			GetDlgItem(IDC_BUTTON_IMPORTTEMPLATE)->EnableWindow(FALSE);
			break;
		}
	case 1:
		{
			GetDlgItem(IDC_EDIT_ROW)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ROW_DISTANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COLUMN_DISTANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_TYPELINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_TYPES)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_ROW)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_COL)->EnableWindow(FALSE);

			GetDlgItem(IDC_LIST)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_REFURBISH)->EnableWindow(TRUE);

			GetDlgItem(IDC_BUTTON_IMPORTTEMPLATE)->EnableWindow(FALSE);
			break;
		}
	case 2:
		{
			GetDlgItem(IDC_EDIT_ROW)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ROW_DISTANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COLUMN)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COLUMN_DISTANCE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_TYPELINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_TYPES)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_ROW)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO_COL)->EnableWindow(FALSE);

			GetDlgItem(IDC_LIST)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_REFURBISH)->EnableWindow(FALSE);

			GetDlgItem(IDC_BUTTON_IMPORTTEMPLATE)->EnableWindow(TRUE);
			break;
		}
	default:
		break;
	}

	UpdateData(FALSE);

}

BOOL CArrayDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CHLApp *pApp=(CHLApp *)AfxGetApp();
	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		pApp->m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		pApp->m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		pApp->m_FastMark.StopFastMark();
	}
	

	//列表处理
	LV_COLUMN lvc;
	
	lvc.mask=
		LVCF_FMT|
		LVCF_WIDTH|
		LVCF_TEXT|
		LVCF_SUBITEM;

	lvc.fmt=LVCFMT_LEFT;

	lvc.cx=80;
	lvc.iSubItem=0;
	lvc.pszText="X(mm)";
	m_list.InsertColumn(0,&lvc);
	lvc.iSubItem=1;
	lvc.pszText="Y(mm)";
	m_list.InsertColumn(1,&lvc);

	lvc.iSubItem=2;
	lvc.pszText="R(o)";
	m_list.InsertColumn(2,&lvc);

	CString str=pApp->GetInstallPath();
	str+="default.pos";
	Load(str);

	CRect rt;
	m_objView.GetClientRect(&rt);
	double x(0.0), y(0.0);
	g_DAT.GetMarkRange(x,y);

	m_xzoom.CreateZoomEx(rt,this,ID_ZOOM_WND,x,y);
 	m_xzoom.SetZoomScale(g_SysCfg.m_dbPreViewZoomScale);

	m_xzoom.m_wndView->SetArrayDlg(this);

#ifdef _DOUBLE_HEAD_MARK
	GetDlgItem(IDC_EDIT_LX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_LY)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RX)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RY)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OFFSET)->EnableWindow(TRUE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CArrayDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_xzoom.m_wndView->SetArrayDlg(NULL);
    m_xzoom.m_wndView->DelAllStrk();

	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	return;
}

void CArrayDlg::GetArrayDot()
{
	UpdateData(TRUE);
	m_dtArray.RemoveAll();

	if(m_nArray)
	{
		CString str("");
		int count=m_list.GetItemCount();
		for(int i=0;i<count;i++)
		{
			OBJPOS dt;
			str  = m_list.GetItemText(i,0);
			dt.x = atof(str);

			str  = m_list.GetItemText(i,1);
			dt.y = atof(str);

			str  = m_list.GetItemText(i,2);
			dt.z = (atof(str));

			m_dtArray.Add(dt);
		}
	}
	else
	{
		BOOL bS = (m_nRoundType==1);
		BOOL bF = TRUE;
		if(m_nRowFirst==1)
		{
			for(int i=0;i<m_nRow;i++)
			{
				for(int j=0;j<m_nColumn;j++)
				{
					OBJPOS dt;
					dt.x=m_dotBase.x+j*m_dbColumnDistance;
					dt.y=m_dotBase.y+i*m_dbRowDistance;
					if(bS&&!bF)
					{
						dt.x=m_dotBase.x+(m_nColumn-1-j)*m_dbColumnDistance;
						dt.y=m_dotBase.y+i*m_dbRowDistance;
					}
					m_dtArray.Add(dt);
				}
				bF = !bF;
			}
		}
		else
		{
			for(int i=0;i<m_nColumn ;i++)
			{
				for(int j=0;j<m_nRow;j++)
				{
					OBJPOS dt;
					dt.x=m_dotBase.x+i*m_dbColumnDistance;
					dt.y=m_dotBase.y+j*m_dbRowDistance;
					if(bS&&!bF)
					{
						dt.x=m_dotBase.x+i*m_dbColumnDistance;
						dt.y=m_dotBase.y+(m_nRow-1-j)*m_dbRowDistance;
					}
					m_dtArray.Add(dt);
				}
				bF = !bF;
			}
		}
	}

	return;
}

void CArrayDlg::Load(CString file)
{
	CFile f;
	
	if(!f.Open(file,CFile::modeRead))
	{
		return;
	}
		
	CArchive ar(&f,CArchive::load);

	//贮存数据
	CString str("");
	ar>>str;
	ar>>m_nArray;
	if(m_nArray==1)
	{
		m_list.DeleteAllItems();
		int count;

		ar>>count;
		double x(0.0), y(0.0), z(0.0);
		for(int i=0;i<count;i++)
		{
			ar>>x;
			ar>>y;
			ar>>z;
			str.Format("%f",x);
			m_list.InsertItem(i,str);
			str.Format("%f",y);
			m_list.SetItemText(i,1,str);
			str.Format("%f",z);
			m_list.SetItemText(i,2,str);
		}
	}
	else if(m_nArray==0)
	{
		ar>>m_nRow;
		ar>>m_nColumn;
		ar>>m_dbRowDistance;
		ar>>m_dbColumnDistance;
	}
	
	ar.Close();
	f.Close();
	
	UpdateView();
}

void CArrayDlg::Save(CString file)
{
	CFile f;
	f.Open(file,CFile::modeWrite|CFile::modeCreate );
	CArchive ar(&f,CArchive::store);

	//贮存数据
	CString str;
	str="2000.12.11";
	ar<<str;
	ar<<m_nArray;
	if(m_nArray==1)
	{
		int count=m_list.GetItemCount();
		ar<<count;
		double x(0.0), y(0.0), z(0.0);
		for(int i=0;i<count;i++)
		{
			str=m_list.GetItemText(i,0);
			x=atof(str);

			str=m_list.GetItemText(i,1);
			y=atof(str);

			str=m_list.GetItemText(i,2);
			z=atof(str);

			ar<<x;
			ar<<y;
			ar<<z;
		}
	}
	else if(m_nArray==0)
	{
		ar<<m_nRow;
		ar<<m_nColumn;
		ar<<m_dbRowDistance;
		ar<<m_dbColumnDistance;
	}
	
	ar.Close();
	f.Close();
}

void CArrayDlg::OnOK() 
{
	int nCnt = m_dtArray.GetSize();
	if (0 == nCnt)
	{
		GetArrayDot();
	}
	
	CHLApp *pApp=(CHLApp *)AfxGetApp();
	CString str=pApp->GetInstallPath();
	str+="default.pos";
	Save(str);

	CDialog::OnOK();
}


BOOL CArrayDlg::MulObjSel(CPoint &point, CRect &rect, CWnd *pWnd)//是否选中对象
{
	BOOL bFlag=FALSE;

	AfxLockTempMaps();
	pWnd->SetCapture();

	if (CWnd::GetCapture() != pWnd)
	{
		return FALSE;
	}
	
	CPoint ptthis    = point;
	       m_ptSPrev  = point;
	       m_ptSNow   = m_ptSPrev;

	MSG msg;
	for(;;)
	{
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		switch (msg.message)
		{
		case WM_LBUTTONUP:
			{
				if(bFlag)
				{
					LastBrush(pWnd, m_ptSPrev, ptthis);
				}

				rect.left   = point.x;
				rect.right  = ptthis.x;
				rect.top    = point.y;
				rect.bottom = ptthis.y;

				goto ExitLoop;
			}
		case WM_MOUSEMOVE:
			{
				bFlag    = TRUE;
				ptthis.x = (int)(short)LOWORD(msg.lParam);
				ptthis.y = (int)(short)HIWORD(msg.lParam);
				DrawRect(pWnd,m_ptSPrev,m_ptSNow,ptthis);
				m_ptSNow = ptthis;
				
				break;
			}
		default:
			{
				break;
			}
		} // end switch
	} // end for

ExitLoop:
	{
		ReleaseCapture();
		AfxUnlockTempMaps(FALSE);	
		return TRUE;
	}
	
}

void CArrayDlg::LastBrush(CWnd *pWnd, CPoint &ptf, CPoint &ptl)
{
	CDC *pDC=pWnd->GetDC();
	CPen *OldPen = NULL;

	int mod=pDC->SetROP2(R2_NOTXORPEN);//反色掉

	CPen BluePen(PS_DOT,1,RGB(0,0,255));
	OldPen=pDC->SelectObject(&BluePen);

	pDC->MoveTo(ptf);
	pDC->LineTo(ptl.x,ptf.y);
	pDC->LineTo(ptl);
	pDC->LineTo(ptf.x,ptl.y);
	pDC->LineTo(ptf);
	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();

	
	 pWnd->ReleaseDC(pDC);
	return;
}

void CArrayDlg::DrawRect(CWnd *pWnd,CPoint &ptf,CPoint &ptl,CPoint &point)
{
   	CDC  *pDC = pWnd->GetDC();
	CPen *OldPen = NULL;

	int mod=pDC->SetROP2(R2_NOTXORPEN);//反色掉
	CPen BluePen(PS_DOT,1,RGB(0,0,255));
	OldPen=pDC->SelectObject(&BluePen);
	
	pDC->MoveTo(ptf);
	pDC->LineTo(ptl.x,ptf.y);
	pDC->LineTo(ptl);
	pDC->LineTo(ptf.x,ptl.y);
	pDC->LineTo(ptf);

	pDC->SelectObject(OldPen);
	OldPen=pDC->SelectObject(&BluePen);

	pDC->MoveTo(ptf);
	pDC->LineTo(point.x,ptf.y);
	pDC->LineTo(point);
	pDC->LineTo(ptf.x,point.y);
	pDC->LineTo(ptf);

	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();
	pDC->SetROP2(mod);
	pWnd->ReleaseDC(pDC);

	return;
}


CHLDoc * CArrayDlg::GetDocument()
{
	CMDIFrameWnd *pMainWnd=(CMDIFrameWnd *)AfxGetMainWnd();
	CMDIChildWnd *pChildWnd=pMainWnd->MDIGetActive();
	if(pChildWnd)
	{
		CHLDoc *pDoc=(CHLDoc *)pChildWnd->GetActiveDocument();
		if (pDoc)
		{
			return pDoc;
		}

		return NULL;
	}

	return NULL;
}


void CArrayDlg::OnButtonApply() 
{
	CHLDoc *pDoc = GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	if (g_bIsRedPreview)
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("停止");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("开始");
	}

	UpdateData(TRUE);

//	int nCnt = m_dtArray.GetSize();
//	if (0 == nCnt)
//	{
//		GetArrayDot();
//	}

	switch(m_nArray)
	{
	case 0:
	case 1:
		{
			GetArrayDot();
			break;
		}
	case 2:
		{
			break;
		}
	default:
		break;
	}
	
	CQuad quad = pDoc->GetLimit();
	if (quad.IsEmpty())
	{
		quad.top = 3;
		quad.bottom = -3;
		quad.left = -3;
		quad.right = 3;
	}
	
	m_xzoom.m_wndView->DrawArray(quad, m_dtArray, m_QuadList);
	m_xzoom.m_wndView->Invalidate();

	return;
}

/////////
void CArrayDlg::DrawSelTraker(XDC *pDC, CQuad &Quad,CWnd *pWnd, CArray<SELQUAD, SELQUAD> &QuadList)
{
	DrawTracker(pDC,Quad,pWnd);

	int nCount = QuadList.GetSize();
	if (nCount <= 1)
	{
		return;
	}

	BOOL bFill  = FALSE;
	int  nIndex = -1;
	
	for (int i =0; i < nCount; i++) 
	{
		if (!QuadList[i].m_bSel)
		{
			continue;
		}

		if (QuadList[i].m_bFill) 
		{
			DrawSelObjTracker(pDC, QuadList[i].m_QD, TRUE, pWnd);
			bFill = TRUE;
		}
		else
		{
			DrawSelObjTracker(pDC, QuadList[i].m_QD, FALSE, pWnd);
		}

		nIndex = i;
	}


	return;
}

void CArrayDlg::DrawSelObjTracker(XDC *pDC, CQuad &Quad, BOOL bFillQuad, CWnd *pWnd)
{
	CPen BluePen(PS_SOLID, 0, BLUE);
	CPen *OldPen=pDC->SelectObject(&BluePen);
	double x(1.5), y(1.5);
	CZoomView *pView=(CZoomView*)pWnd;
	CQuad quad=Quad;
	pView->ActualLen(x);
	pView->ActualLen(y);
	quad.Inflate(x,y);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);
	//draw handle
//	m_quadSave=quad;
	m_HITBORDER=HITBORDER;
	m_xzoom.m_wndView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();
	for(int i=0;i<8;i++)
	{
		if ((7 == i) || (6 == i) || (5 == i) || (8 == i) || (4 == i))
		{
			continue;
		}
		pDC->MoveTo(m_quad[i].left,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].top);
		pDC->LineTo(m_quad[i].right,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].bottom);
		pDC->LineTo(m_quad[i].left,m_quad[i].top);
		if (bFillQuad)
		{
			pDC->FillSolidQuad(m_quad[i],BLUE);
		}
	}
	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();

	return;
}


void CArrayDlg::DrawTracker(XDC *pDC, CQuad &Quad,CWnd *pWnd)
{
	//draw outline
	CPen BluePen(PS_DOT,0,BLUE);
	CPen *OldPen=pDC->SelectObject(&BluePen);
	double x(1.5),y(1.5);
	CZoomView *pView=(CZoomView*)pWnd;
	CQuad quad=Quad;
	pView->ActualLen(x);
	pView->ActualLen(y);
	quad.Inflate(x,y);
	pDC->MoveTo(quad.left,quad.top);
	pDC->LineTo(quad.right,quad.top);
	pDC->LineTo(quad.right,quad.bottom);
	pDC->LineTo(quad.left,quad.bottom);
	pDC->LineTo(quad.left,quad.top);
	//draw handle
	m_quadSave=quad;
	m_HITBORDER=HITBORDER;
	pView->ActualLen(m_HITBORDER);
	UpdateRectCtrl();

	CDC *pMDC = pView->GetDC();
	for(int i=0; i<8; i++)
	{
		CQuad  mQuad = m_quad[i];
		switch(i)
		{
		case 0:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(-10, -10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[3]);
				break;
			}
		case 1:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(10, -10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[2]);
				break;
			}
		case 2:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(10, 10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[3]);
				break;
			}
		case 3:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(-10, 10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[2]);
				break;
			}
		case 4:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(0, -10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[1]);
				break;
			}
		case 5:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(10, 0);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[4]);
				break;
			}
		case 6:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(0, 10);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[1]);
				break;
			}
		case 7:
			{
				CDot dot  = mQuad.Center();
				pView->PhysicToLogic(dot.x, dot.y);
				dot.SetOffset(-10, 0);
				pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[4]);
				break;
			}
		default:
			{
				break;
			}
		}

	}

	CDot dot  = Quad.Center();
	pView->PhysicToLogic(dot.x, dot.y);
	pMDC->DrawIcon((int)(dot.x) - 16, (int)(dot.y) - 16, g_hIcoList[0]);

	pView->ReleaseDC(pMDC);

	pDC->SelectObject(OldPen);
	BluePen.DeleteObject();
}



void CArrayDlg::UpdateRectCtrl()
{
	(m_quad[0]).left  = m_quadSave.left-2*m_HITBORDER;
	(m_quad[0]).right = m_quadSave.left;
	(m_quad[0]).top   = m_quadSave.top+2*m_HITBORDER;
	(m_quad[0]).bottom= m_quadSave.top;

	(m_quad[1]).left  = m_quadSave.right;
	(m_quad[1]).right = m_quadSave.right+2*m_HITBORDER;
	(m_quad[1]).top   = m_quadSave.top+2*m_HITBORDER;
	(m_quad[1]).bottom= m_quadSave.top;

	(m_quad[2]).left  = m_quadSave.right;
	(m_quad[2]).right = m_quadSave.right+2*m_HITBORDER;
	(m_quad[2]).top   = m_quadSave.bottom;
	(m_quad[2]).bottom= m_quadSave.bottom-2*m_HITBORDER;

	(m_quad[3]).left  = m_quadSave.left-2*m_HITBORDER;
	(m_quad[3]).right = m_quadSave.left;
	(m_quad[3]).top   = m_quadSave.bottom;
	(m_quad[3]).bottom= m_quadSave.bottom-2*m_HITBORDER;

	(m_quad[4]).left  = (m_quadSave.left+m_quadSave.right)/2-m_HITBORDER;
	(m_quad[4]).right = (m_quadSave.left+m_quadSave.right)/2+m_HITBORDER;
	(m_quad[4]).top   = m_quadSave.top+2*m_HITBORDER;
	(m_quad[4]).bottom= m_quadSave.top;

	(m_quad[5]).left  = m_quadSave.right;
	(m_quad[5]).right = m_quadSave.right+2*m_HITBORDER;
	(m_quad[5]).top   = (m_quadSave.top+m_quadSave.bottom)/2+m_HITBORDER;
	(m_quad[5]).bottom= (m_quadSave.top+m_quadSave.bottom)/2-m_HITBORDER;

	(m_quad[6]).left  = (m_quadSave.left+m_quadSave.right)/2-m_HITBORDER;
	(m_quad[6]).right = (m_quadSave.left+m_quadSave.right)/2+m_HITBORDER;
	(m_quad[6]).top   = m_quadSave.bottom;
	(m_quad[6]).bottom= m_quadSave.bottom-2*m_HITBORDER;

	(m_quad[7]).left  = m_quadSave.left-2*m_HITBORDER;
	(m_quad[7]).right = m_quadSave.left;
	(m_quad[7]).top   = (m_quadSave.top+m_quadSave.bottom)/2+m_HITBORDER;
	(m_quad[7]).bottom= (m_quadSave.top+m_quadSave.bottom)/2-m_HITBORDER;

	return;
}

void CArrayDlg::OnButtonRotate() 
{
	UpdateData(TRUE);
	
	int nCnt = m_QuadList.GetSize();
	for (int i = 0; i < nCnt; i++)
	{
		if (m_QuadList[i].m_bSel)
		{
			double dbRotate = m_dtArray[i].z;
			dbRotate += m_dbRotate;
			dbRotate = fmod(dbRotate, 360);
			m_dtArray[i].z = (float)dbRotate;
		}
	}

	CHLDoc* pDoc = GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	CQuad quad = pDoc->GetLimit();
	if (quad.IsEmpty())
	{
		quad.top = 3;
		quad.bottom = -3;
		quad.left = -3;
		quad.right = 3;
	}
	
	m_xzoom.m_wndView->UpdateDrawArray(quad, m_dtArray, m_QuadList);
	m_xzoom.m_wndView->UpdateSelQuad();
	m_xzoom.m_wndView->Invalidate();

	SetRedMarkQuadlist(m_QuadList);

	return;
}

void CArrayDlg::OnButtonImporttemplate() 
{
	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	if (g_bIsRedPreview)
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("停止");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("开始");
	}

	CString file("");
	CHLApp *pApp=(CHLApp *)AfxGetApp();
	CString str("");
	
	static char szFilter[]="TPos File(*.tpos)|*.tpos||";
	CFileDialog dlg(TRUE,"tpos",NULL,OFN_HIDEREADONLY,szFilter);
	
	file=pApp->GetInstallPath();
	str.LoadString(IDS_TEMPLATEARRAY_LOAD);
	dlg.m_ofn.lpstrTitle=str;
	dlg.m_ofn.lpstrInitialDir=file;
	if(dlg.DoModal()==IDOK)
	{
		file=dlg.GetPathName();
		LoadTemlate(file, m_dtArray);
	}

	CHLDoc* pDoc = GetDocument();
	if (!pDoc)
	{
		return;
	}

	CQuad quad = pDoc->GetLimit();
	if (quad.IsEmpty())
	{
		quad.top = 3;
		quad.bottom = -3;
		quad.left = -3;
		quad.right = 3;
	}
	
	m_xzoom.m_wndView->DrawArray(quad, m_dtArray, m_QuadList);
	m_xzoom.m_wndView->Invalidate();

	return;
}

void CArrayDlg::OnButtonExporttemplate() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString file("");
	CHLApp *pApp=(CHLApp *)AfxGetApp();
	CString str;
	
	static char szFilter[]="TPos File(*.tpos)|*.tpos||";
	CFileDialog dlg(FALSE,"tpos",NULL,OFN_OVERWRITEPROMPT,szFilter);
	
	file=pApp->GetInstallPath();
	str.LoadString(IDS_TEMPLATEARRAY_SAVE);
	dlg.m_ofn.lpstrTitle=str;
	dlg.m_ofn.lpstrInitialDir=file;
	if(dlg.DoModal()==IDOK)
	{
		file=dlg.GetPathName();
		SaveTemlate(file, m_dtArray);
	}

	return;
}


void CArrayDlg::LoadTemlate(CString &fileName, CArray<OBJPOS, OBJPOS> &dtArray)
{
	FILE *fp;
	if(fopen_s(&fp, fileName,"rb") != 0)
	{
		return;
	}

	CMySerialize fs(fp);
	dtArray.RemoveAll();
	int nCnt = dtArray.GetSize();

	//贮存数据
	fs>>nCnt;
	dtArray.SetSize(nCnt);
	for(int i = 0; i < nCnt; i++)
	{
		fs>>dtArray[i].x;
		fs>>dtArray[i].y;
		fs>>dtArray[i].z;

		fs>>dtArray[i].Lf.x;
		fs>>dtArray[i].Lf.y;
		fs>>dtArray[i].Rf.x;
		fs>>dtArray[i].Rf.y;
	}
	
	fclose(fp);

	return;
}

void CArrayDlg::SaveTemlate(CString &fileName, CArray<OBJPOS, OBJPOS> &dtArray)
{
	FILE *fp;
	if(fopen_s(&fp, fileName,"wb")!=0)
	{
		return;
	}

	CMySerialize fs(fp);

	//贮存数据
	int nCnt = dtArray.GetSize();
	fs<< nCnt;
	for(int i = 0; i < nCnt; i++)
	{
		fs<<dtArray[i].x;
		fs<<dtArray[i].y;
		fs<<dtArray[i].z;

		fs<<dtArray[i].Lf.x;
		fs<<dtArray[i].Lf.y;
		fs<<dtArray[i].Rf.x;
		fs<<dtArray[i].Rf.y;
	}

	fclose(fp);

	return;
}

void CArrayDlg::OnButtonOffset() 
{
	UpdateData(TRUE);
	
	int nCnt = m_QuadList.GetSize();
	for (int i = 0; i < nCnt; i++)
	{
		if (m_QuadList[i].m_bSel)
		{
			m_dtArray[i].Lf.x = m_dbLX;
			m_dtArray[i].Lf.y = m_dbLY;

			m_dtArray[i].Rf.x = m_dbRX;
			m_dtArray[i].Rf.y = m_dbRY;
		}
	}

	SetRedMarkQuadlist(m_QuadList);

	return;
}

void CArrayDlg::DispSelObjProp(int nNum) 
{
	int nCnt = m_QuadList.GetSize();

	if ((nNum < 0) || (nNum >= nCnt))
	{
		return;
	}
	
	for (int i = 0; i < nCnt; i++)
	{
		if (m_QuadList[i].m_bSel)
		{
			OBJPOS Pos = m_dtArray[i];
			m_dbLX = Pos.Lf.x;
			m_dbLY = Pos.Lf.y;

			m_dbRX = Pos.Rf.x;
			m_dbRY = Pos.Rf.y;

			m_dbRotate = Pos.z;

			break;
		}
	}

	UpdateData(FALSE);

	return;
}

void CArrayDlg::SetRedMarkQuadlist(CArray<SELQUAD, SELQUAD> &QuadList) 
{
	CHLDoc* pDoc = GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	CQuad quad = pDoc->GetLimit();
	if (quad.IsEmpty())
	{
		quad.top = 3;
		quad.bottom = -3;
		quad.left = -3;
		quad.right = 3;
	}

	CArray<ARRAYMARK, ARRAYMARK> ArrayMark;
	int nCnt = QuadList.GetSize();
	for (int i = 0; i < nCnt; i++)
	{
		SELQUAD selQuad = QuadList.GetAt(i);
		if (TRUE == selQuad.m_bSel) 
		{
			ARRAYMARK Pos;

			CDot cDt   = quad.Center();
			OBJPOS ObjPos = m_dtArray.GetAt(i);
			double dbx = ObjPos.x - cDt.x;
			double dby = ObjPos.y - cDt.y;
			quad.SetOffset(dbx, dby);
			
			Pos.qd.left   = quad.left;
			Pos.qd.right  = quad.right;
			Pos.qd.top    = quad.top;
			Pos.qd.bottom = quad.bottom;

			Pos.Lf   = ObjPos.Lf;
			Pos.Rf   = ObjPos.Rf;
			Pos.rota = ObjPos.z;

			ArrayMark.Add(Pos);
		}
		
	}
	theApp.m_RedPreView.SetMarkStrk(ArrayMark);

	return;
}


void CArrayDlg::OnButtonMark() 
{
	CHLDoc* pDoc = GetDocument();
	if (!pDoc)
	{
		return;
	}

	UpdateData(TRUE);

	switch(m_nRedMark)
	{
	case 0:
		{
			SetRedMarkQuadlist(m_QuadList);
			pDoc->RedPreview(TRUE,TRUE);
			break;
		}
	case 1:
		{
			SetRedMarkQuadlist(m_QuadList);
			pDoc->RedPreview(FALSE, TRUE);
			break;
		}
	default:
		{
			break;
		}
	}

	if (g_bIsRedPreview)
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("停止");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("开始");
	}

	Sleep(100);

	return;
}

void CArrayDlg::OnRadioRed() 
{
	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	if (g_bIsRedPreview)
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("停止");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("开始");
	}

	return;
}

void CArrayDlg::OnRadioLaser() 
{
	if (g_bIsRedPreview)
	{
		g_bIsRedPreview = FALSE;
		theApp.m_RedPreView.StopPreview();
	}

	if (g_bIsSelMark) 
	{
		theApp.m_SelMark.StopSelMark();
	}
	
	if (g_bIsFastMark) 
	{
		theApp.m_FastMark.StopFastMark();
	}

	if (g_bIsRedPreview)
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("停止");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_MARK)->SetWindowText("开始");
	}

	return;
}
