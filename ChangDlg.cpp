// CChangDlg.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ChangDlg.h"
#include "SelectCoordinary.h"
#include "HLView.h"
#include "ExtendObjDlg.h"
#include "ExternViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangDlg dialog


CChangDlg::CChangDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangDlg)
	m_dbX = 0.0;
	m_dbY = 0.0;
	m_dbRotate = 0.0;
	m_dbPropX = 100.0;
	m_dbPropY = 100.0;
	m_bProrata = TRUE;
	m_nBase = 4;
	m_strSizeX = _T("");
	m_strSizeY = _T("");
	m_dbSizeX  = 0.0;
	m_dbSizeY  = 0.0;
	//}}AFX_DATA_INIT
}


void CChangDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangDlg)
	DDX_Text(pDX, IDC_POSX, m_dbX);
	DDX_Text(pDX, IDC_POSY, m_dbY);
	DDX_Text(pDX, IDC_ROTATE, m_dbRotate);
	DDX_Text(pDX, IDC_PROPEX, m_strPropX);
	DDX_Text(pDX, IDC_PROPEY, m_strPropY);
	DDX_Check(pDX, IDC_CHECK_SCALE, m_bProrata);
	DDX_Text(pDX, IDC_SIZEX, m_strSizeX);
	DDX_Text(pDX, IDC_SIZEY, m_strSizeY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangDlg, CDialog)
	//{{AFX_MSG_MAP(CChangDlg)
	ON_BN_CLICKED(IDOK, OnApply)
	ON_EN_CHANGE(IDC_PROPEX, OnChangePropX)
	ON_EN_CHANGE(IDC_PROPEY, OnChangePropY)
	ON_EN_CHANGE(IDC_ROTATE, OnChangeRotate)
	ON_BN_CLICKED(IDC_BUTT_SELPOS, OnButtSelpos)
	ON_BN_CLICKED(IDC_CENTER, OnCenter)
	ON_BN_CLICKED(IDC_MAKECOPY, OnMakecopy)
	ON_EN_CHANGE(IDC_SIZEX, OnChangeSizeX)
	ON_EN_CHANGE(IDC_SIZEY, OnChangeSizeY)
	ON_BN_CLICKED(IDC_CHECK_SCALE, OnCheckScale)
    ON_BN_CLICKED(IDC_CHECK_CENTERPOS, OnCheckPOS)
	ON_BN_CLICKED(IDC_MAKECOPY_SET, OnMakecopySet)
	ON_BN_CLICKED(IDC_BUTTON_EXTENDOBJ, OnButtonExtendobj)
	ON_EN_CHANGE(IDC_POSX, OnChangePosx)
	ON_EN_CHANGE(IDC_POSY, OnChangePosy)
	ON_EN_SETFOCUS(IDC_POSX, OnSetfocusPosx)
	ON_EN_SETFOCUS(IDC_POSY, OnSetfocusPosy)
	ON_EN_SETFOCUS(IDC_ROTATE, OnSetfocusRotate)
	ON_EN_SETFOCUS(IDC_SIZEX, OnSetfocusSizex)
	ON_EN_SETFOCUS(IDC_SIZEY, OnSetfocusSizey)
	ON_EN_SETFOCUS(IDC_PROPEX, OnSetfocusPropex)
	ON_EN_SETFOCUS(IDC_PROPEY, OnSetfocusPropey)
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CChangDlg::OnNewDocument()
{
	m_dbX      = 0.0;
	m_dbY      = 0.0;
	m_dbRotate = 0.0;
	m_dbPropX  = 100.0;
	m_dbPropY  = 100.0;
	m_dbSizeX  = 0.0;
	m_dbSizeY  = 0.0;
	m_strSizeX = _T("0.0");
	m_strSizeY = _T("0.0");
	m_nBase    = 4;
	m_bProrata = TRUE;

	((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetCheck(TRUE);
	HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON19);
	((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetIcon(hIcon);

	UpdateUIData(FALSE);

	m_strSizeX = _T("0.0");
	m_strSizeY = _T("0.0");
    m_strPropX = _T("0.0");
	m_strPropY = _T("0.0");

	UpdateData(FALSE);

	return TRUE;
}

void CChangDlg::OnApply()
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	CWnd *pWnd=(CWnd *)GetDlgItem(IDOK);
	CButton *pButt=(CButton*)pWnd;

	CQuad oldQuad;
    oldQuad.SetEmpty();
	oldQuad=pDoc->GetTrackerQuad();

	CDot dot;
	dot=oldQuad.GetBaseDot(m_nBase);
	double dbWith   = oldQuad.Width();
	double dbHeight = oldQuad.Height();

	UpdateUIData(TRUE);

	m_dbSizeX = atof(m_strSizeX);
	m_dbSizeY = atof(m_strSizeY);
	
	m_dbPropX = atof(m_strPropX);
	m_dbPropY = atof(m_strPropY);

	if (m_dbSizeX < 0)  m_dbSizeX = dbWith;  else if (fabs(m_dbSizeX) == 0) m_dbSizeX  = 0.000000001;
	if (m_dbSizeY < 0)  m_dbSizeY = dbHeight; else if (fabs(m_dbSizeY) == 0) m_dbSizeY = 0.000000001;
	if (m_dbPropX < 0)  m_dbPropX = 100; else if (fabs(m_dbPropX) == 0) m_dbPropX = 0.000000001;
	if (m_dbPropY < 0)  m_dbPropY = 100; else if (fabs(m_dbPropY) == 0) m_dbPropY = 0.000000001;
	
	switch(m_dwOperate) 
	{
	case ENUM_OBJECT_SIZE:
		{
			if ((fabs(dbWith) > EPSINON) && (fabs(dbHeight) > EPSINON))
			{
				pDoc->Drag(dot, m_dbSizeX/dbWith, m_dbSizeY/dbHeight, 0, 0);
			}
			else if ((fabs(dbWith) <= EPSINON) && (fabs(dbHeight) > EPSINON))
			{
				pDoc->Drag(dot, 1, m_dbSizeY/dbHeight, 0, 0);
			}
			else if ((fabs(dbWith) > EPSINON) && (fabs(dbHeight) <= EPSINON))
			{
				pDoc->Drag(dot, m_dbSizeX/dbWith, 1, 0, 0);
			}
			else
			{
			}

			pView->Invalidate();
			m_dwOperate = ENUM_OBJECT_END;
			break;
		}
	case ENUM_OBJECT_SCALE:
		{
			pDoc->Drag(dot, m_dbPropX/100, m_dbPropY/100, 0, 0);
			pView->Invalidate();
			
			break;
		}
	case ENUM_OBJECT_ROTATE:
		{
			if(m_dbRotate !=0)
			{
				pDoc->Rotate(dot, m_dbRotate);
				pView->Invalidate();
			}
			break;
		}
	case ENUM_OBJECT_POS:
		{
			CQuad quad=pDoc->GetLimit();
			CDot dotOld,dotNew;
			dotOld=quad.GetBaseDot(m_nBase);
			
			BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_CENTERPOS))->GetCheck();
			if (bRet)
			{
				dot = quad.Center();
//				((CButton*)GetDlgItem(IDC_CHECK_CENTERPOS))->SetCheck(FALSE);
				dotNew = CDot(dotOld.x + m_dbX, dotOld.y + m_dbY);
//				GetDlgItem(IDC_STATIC_OBJPOS)->SetWindowText("绝对位置:");
			}
			else
			{
				dotNew = CDot(m_dbX,m_dbY);
			}
			
			double moveX=dotNew.x-dotOld.x;
			double moveY=dotNew.y-dotOld.y;

			pDoc->Drag(dotOld, 1, 1, moveX, moveY);
			pView->Invalidate();
			
			break;
		}
	default:
		break;
	}

	UpdateUIData(FALSE);

	pView->SetFocus();

	pView->DispSelObjProp();

	return;
}

void CChangDlg::OnMakecopy() 
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	CHLDoc *pDoc=GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	pDoc->Copy();

	CQuad oldQuad;
    oldQuad.SetEmpty();
	oldQuad=pDoc->GetTrackerQuad();

	CDot dot;
	dot = oldQuad.GetBaseDot(m_nBase);
	double dbWith   = oldQuad.Width();
	double dbHeight = oldQuad.Height();

	UpdateUIData(TRUE);

	m_dbSizeX = atof(m_strSizeX);
	m_dbSizeY = atof(m_strSizeY);

	m_dbPropX = atof(m_strPropX);
	m_dbPropY = atof(m_strPropY);

	if (m_dbSizeX < 0)  m_dbSizeX = dbWith;  else if (fabs(m_dbSizeX) == 0) m_dbSizeX  = 0.000000001;
	if (m_dbSizeY < 0)  m_dbSizeY = dbHeight; else if (fabs(m_dbSizeY) == 0) m_dbSizeY = 0.000000001;
	if (m_dbPropX < 0)  m_dbPropX = 100; else if (fabs(m_dbPropX) == 0) m_dbPropX = 0.000000001;
	if (m_dbPropY < 0)  m_dbPropY = 100; else if (fabs(m_dbPropY) == 0) m_dbPropY = 0.000000001;
	
	switch(m_dwOperate) 
	{
	case ENUM_OBJECT_SIZE:
		{
			if ((fabs(dbWith) > EPSINON) && (fabs(dbHeight) > EPSINON))
			{
				pDoc->Drag(dot, m_dbSizeX/dbWith, m_dbSizeY/dbHeight, 0, 0);
			}
			else if ((fabs(dbWith) <= EPSINON) && (fabs(dbHeight) > EPSINON))
			{
				pDoc->Drag(dot, 1, m_dbSizeY/dbHeight, 0, 0);
			}
			else if ((fabs(dbWith) > EPSINON) && (fabs(dbHeight) <= EPSINON))
			{
				pDoc->Drag(dot, m_dbSizeX/dbWith, 1, 0, 0);
			}
			else
			{
			}

			pView->Invalidate();
			m_dwOperate = ENUM_OBJECT_END;
			break;
		}
	case ENUM_OBJECT_SCALE:
		{
			pDoc->Drag(dot, m_dbPropX/100, m_dbPropY/100, 0, 0);
			pView->Invalidate();
			break;
		}
	case ENUM_OBJECT_ROTATE:
		{
			if(m_dbRotate !=0)
			{
				pDoc->Rotate(dot, m_dbRotate);
				pView->Invalidate();
			}
			break;
		}
	case ENUM_OBJECT_POS:
		{
			CQuad quad=pDoc->GetLimit();
			CDot dotOld,dotNew;
			dotOld=quad.GetBaseDot(m_nBase);

			BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_CENTERPOS))->GetCheck();
			if (bRet)
			{
				dot = quad.Center();
//				((CButton*)GetDlgItem(IDC_CHECK_CENTERPOS))->SetCheck(FALSE);
				dotNew = CDot(dotOld.x + m_dbX, dotOld.y + m_dbY);
//				GetDlgItem(IDC_STATIC_OBJPOS)->SetWindowText("绝对位置:");
			}
			else
			{
				dotNew = CDot(m_dbX,m_dbY);
			}

			double moveX = dotNew.x - dotOld.x;
			double moveY = dotNew.y - dotOld.y;

			pDoc->Drag(dotOld, 1, 1, moveX, moveY);
			pView->Invalidate();
			
			break;
		}
	default:
		break;
	}

	UpdateUIData(FALSE);

	pView->SetFocus();

	pView->DispSelObjProp();
	
	return;
}

void CChangDlg::UpdateUIData(BOOL bSaveAndValidate)
{
	if (!bSaveAndValidate) 
	{

		m_dbX = MyPrecision(m_dbX,4);
		m_dbY = MyPrecision(m_dbY,4);
		m_dbRotate = MyPrecision(m_dbRotate);
//		m_dbPropX  = MyPrecision(m_dbPropX);
//		m_dbPropY  = MyPrecision(m_dbPropY);

		m_strPropX.Format("%0.4f", m_dbPropX);
		m_strPropY.Format("%0.4f", m_dbPropY);
		
//		m_dbSizeX  = MyPrecision(m_dbSizeX);
//		m_dbSizeY  = MyPrecision(m_dbSizeY);
		m_strSizeX.Format("%0.4f", m_dbSizeX);
		m_strSizeY.Format("%0.4f", m_dbSizeY);
	}

	UpdateData(bSaveAndValidate);
	
	return;
}

void CChangDlg::SetPos(double dbX,double dbY)
{
	BOOL bCheck = ((CButton*)(GetDlgItem(IDC_CHECK_CENTERPOS)))->GetCheck();
	
	if (!bCheck) 
	{
		m_dbX=MyPrecision(dbX);
		m_dbY=MyPrecision(dbY);
	}
	
	UpdateUIData(FALSE);
}

void CChangDlg::SetSize(double dbSizeX,double dbSizeY)
{
//	m_dbSizeX=MyPrecision(dbSizeX);
//	m_dbSizeY=MyPrecision(dbSizeY);

	m_strSizeX.Format("%0.4f", dbSizeX);
	m_strSizeY.Format("%0.4f", dbSizeY);

	m_dbSizeX = dbSizeX;
	m_dbSizeY = dbSizeY;
	UpdateUIData(FALSE);
}

BOOL CChangDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON14);
	((CButton*)GetDlgItem(IDC_BUTT_SELPOS))->SetIcon(hIcon);
	
	hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON22);
	((CButton*)GetDlgItem(IDC_MAKECOPY_SET))->SetIcon(hIcon);
	
	m_bProrata = TRUE;
	((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetCheck(TRUE);
	hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON19);
	((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetIcon(hIcon);
//	UpdateUIData(FALSE);
	UpdateData(FALSE);
	
	return TRUE;
}

void CChangDlg::OnChangeRotate() 
{
	m_dwOperate = ENUM_OBJECT_ROTATE;
}

void CChangDlg::CloseDoor()
{
	(CButton*)GetDlgItem(IDC_POSX)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_POSY)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_PROPEX)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_PROPEY)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_SIZEY)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_SIZEX)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_ROTATE)->EnableWindow(FALSE);
	(CButton*)GetDlgItem(IDC_CENTER)->EnableWindow(FALSE);

	return;
}

void CChangDlg::OpenDoor()
{
	(CButton*)GetDlgItem(IDC_POSX)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_POSY)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_PROPEX)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_PROPEY)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_SIZEY)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_SIZEX)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_ROTATE)->EnableWindow(TRUE);
	(CButton*)GetDlgItem(IDC_CENTER)->EnableWindow(TRUE);

	return;
}

void CChangDlg::OnButtSelpos() 
{
	CFrameWnd *pWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pWnd->GetActiveFrame()->GetActiveView();
	if(pView!=NULL)
	{
		CButton *pButt=(CButton*)GetDlgItem(IDC_BUTT_SELPOS);
		SelectCoordinary dlg(pView,&m_dbX,&m_dbY,&m_dbSizeX,&m_dbSizeY,pButt,&m_nBase);
		if(dlg.DoModal()==IDOK)
		{
			UpdateUIData(FALSE);
		}
	}

	pView->SetFocus();
	return;
}

void CChangDlg::OnCenter() 
{
	CFrameWnd *pFWnd=(CFrameWnd*)AfxGetMainWnd();
	CHLView *pView=(CHLView*)pFWnd->GetActiveFrame()->GetActiveView();
	if(pView == NULL)
	{
		return;
	}

	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if(pDoc->HaveSelectObject())
	{
		CQuad quad=pDoc->GetLimit();
		CDot dot;
		dot.x=(quad.left+quad.right)/2;
		dot.y=(quad.top+quad.bottom)/2;
		pDoc->Drag(dot,1,1,-dot.x,-dot.y);
		pView->Invalidate();
	}

	pView->SetFocus();
	
	return;
}

void CChangDlg::MakeitUse()
{
	return;
}


CHLDoc* CChangDlg::GetDocument()
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return NULL;
	}

	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return NULL;
	}

	return pDoc;
}

void CChangDlg::OnChangeSizeX() 
{
	CHLDoc *pDoc = GetDocument();
	if (!pDoc)  return;

	if (!pDoc->IsSelectedObj())
	{   
		m_dbSizeX = 0;
		UpdateUIData(FALSE);
		return;
	}
	
	m_dwOperate = ENUM_OBJECT_SIZE;

	CQuad oldQuad;
	oldQuad.SetEmpty();
	oldQuad=pDoc->GetTrackerQuad();

	CDot dot;
	dot=oldQuad.GetBaseDot(m_nBase);
	double dbWith   = oldQuad.Width();
	double dbHeight = oldQuad.Height();

    UpdateUIData(TRUE);

	m_dbSizeX = atof(m_strSizeX);
	m_dbSizeY = atof(m_strSizeY);

	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (bRet)
	{
		if (fabs(dbWith) < EPSINON) 
		{
	//		m_dbSizeY = 0;
			m_dbPropX = 100;
		}
		else
		{
			m_dbSizeY = m_dbSizeX  * dbHeight  / dbWith;
		    m_dbPropX = (m_dbSizeX / dbWith)   * 100;
		}

		if (fabs(dbHeight) < EPSINON) 
		{
			m_dbPropY = 100;
		}
		else
		{
			m_dbPropY = (m_dbSizeY / dbHeight) * 100;
		}
	}
	else
	{
		if (fabs(dbWith) < EPSINON) 
		{
			m_dbPropX = 100;
		}
		else
		{
			m_dbPropX = (m_dbSizeX / dbWith)   * 100;
		}
	}

	m_strSizeY.Format("%0.4f", m_dbSizeY);

	m_strPropX.Format("%0.4f", m_dbPropX);
	m_strPropY.Format("%0.4f", m_dbPropY);

//	UpdateUIData(FALSE);
	UpdateData(FALSE);

	return;
}

void CChangDlg::OnChangeSizeY() 
{
	CHLDoc *pDoc = GetDocument();
	if (!pDoc)  return;

	if (!pDoc->IsSelectedObj())
	{   
		m_dbSizeY = 0;
		UpdateUIData(FALSE);
		return;
	}

	m_dwOperate = ENUM_OBJECT_SIZE;

	UpdateUIData(TRUE);

	m_dbSizeX = atof(m_strSizeX);
	m_dbSizeY = atof(m_strSizeY);

	CQuad oldQuad;
	oldQuad.SetEmpty();
	oldQuad=pDoc->GetTrackerQuad();

	double dbWith   = oldQuad.Width();
	double dbHeight = oldQuad.Height();
	
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (bRet)
	{
		if (fabs(dbHeight) < EPSINON) 
		{
//			m_dbSizeX = 0;
		    m_dbPropY = 100;
		}
		else
		{
			m_dbSizeX = m_dbSizeY  * dbWith / dbHeight;
		    m_dbPropY = (m_dbSizeY / dbHeight) * 100;
		}
		
		if (fabs(dbWith) < EPSINON) 
		{
			m_dbPropX = 100;
		}
		else
		{
			m_dbPropX = (m_dbSizeX / dbWith)   * 100;
		}
	}
	else
	{
		if (fabs(dbHeight) < EPSINON) 
		{
			m_dbPropY = 100;
		}
		else
		{
			m_dbPropY = (m_dbSizeY / dbHeight) * 100;
		}
		
	}

	m_strSizeX.Format("%0.4f", m_dbSizeX);

	m_strPropX.Format("%0.4f", m_dbPropX);
	m_strPropY.Format("%0.4f", m_dbPropY);

//	UpdateUIData(FALSE);
	UpdateData(FALSE);

	return;
}

void CChangDlg::OnChangePropX() 
{
	CHLDoc *pDoc = GetDocument();
	if (!pDoc)  return;

	if (!pDoc->IsSelectedObj())
	{   
		m_dbPropX = 100;
		UpdateUIData(FALSE);
		return;
	}
	
	m_dwOperate = ENUM_OBJECT_SCALE;

	UpdateUIData(TRUE);

	m_dbSizeX = atof(m_strSizeX);
	m_dbSizeY = atof(m_strSizeY);

	m_dbPropX = atof(m_strPropX);
	m_dbPropY = atof(m_strPropY);

	CQuad oldQuad;
	oldQuad.SetEmpty();
	oldQuad=pDoc->GetTrackerQuad();

	double dbWidth  = oldQuad.Width();
	double dbHeight = oldQuad.Height();

	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (bRet)
	{
		m_dbPropY = m_dbPropX;
		m_dbSizeX = dbWidth  * (m_dbPropX / 100);
		m_dbSizeY = dbHeight * (m_dbPropY / 100);
	}
	else
	{
		m_dbSizeX = dbWidth  * (m_dbPropX / 100);
	}

	m_strSizeX.Format("%0.4f", m_dbSizeX);
	m_strSizeY.Format("%0.4f", m_dbSizeY);

	m_strPropY.Format("%0.4f", m_dbPropY);

//    UpdateUIData(FALSE);
	UpdateData(FALSE);

	return;
}

void CChangDlg::OnChangePropY() 
{
	CHLDoc *pDoc = GetDocument();
	if (!pDoc)  return;

	if (!pDoc->IsSelectedObj())
	{   
		m_dbPropY = 100;
		UpdateUIData(FALSE);
		return;
	}

	m_dwOperate = ENUM_OBJECT_SCALE;

	UpdateUIData(TRUE);

	m_dbSizeX = atof(m_strSizeX);
	m_dbSizeY = atof(m_strSizeY);

	m_dbPropX = atof(m_strPropX);
	m_dbPropY = atof(m_strPropY);
	
	CQuad oldQuad;
	oldQuad.SetEmpty();
	oldQuad=pDoc->GetTrackerQuad();

	double dbHeight = oldQuad.Height();
	double dbWidth  = oldQuad.Width();

	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (bRet)
	{
		m_dbSizeY = dbHeight * (m_dbPropY / 100);
		m_dbPropX = m_dbPropY;
		m_dbSizeX = dbWidth  * (m_dbPropX / 100);

	}
	else
	{
		m_dbSizeY = dbHeight * (m_dbPropY / 100);
	}

	m_strSizeX.Format("%0.4f", m_dbSizeX);
	m_strSizeY.Format("%0.4f", m_dbSizeY);

	m_strPropX.Format("%0.4f", m_dbPropX);
	
//	UpdateUIData(FALSE);
	UpdateData(FALSE);

	return;
}

void CChangDlg::OnCheckScale() 
{
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->GetCheck();
	if (!bRet)
	{
		HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON20);
	    ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetIcon(hIcon);
		m_bProrata = FALSE;
	}
	else
	{
		HICON hIcon=((CHLApp*)AfxGetApp())->LoadIcon(IDI_ICON19);
	    ((CButton*)GetDlgItem(IDC_CHECK_SCALE))->SetIcon(hIcon);
		m_bProrata = TRUE;
	}

	return;
}

void CChangDlg::OnCheckPOS() 
{
	BOOL bRet = ((CButton*)GetDlgItem(IDC_CHECK_CENTERPOS))->GetCheck();
	if (!bRet)
	{
		GetDlgItem(IDC_STATIC_OBJPOS)->SetWindowText("Abs Pos:");
	}
	else
	{
		GetDlgItem(IDC_STATIC_OBJPOS)->SetWindowText("RelativePos");
		GetDlgItem(IDC_POSX)->SetWindowText("0");
		GetDlgItem(IDC_POSY)->SetWindowText("0");
		m_dbX = 0.0;
		m_dbY = 0.0;
	}

	return;
}

void CChangDlg::OnMakecopySet() 
{
	return;
}

void CChangDlg::OnButtonExtendobj() 
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
	if(NULL == pView)
	{
		return;
	}

	CHLDoc *pDoc=(CHLDoc*)pView->GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	// 	CExtendObjDlg oExtendobj;
// 	oExtendobj.DoModal();
	CExternViewDlg dlg;
	dlg.SetList(pDoc);
	dlg.DoModal();

	pView->SetFocus();
	return;
}

void CChangDlg::OnChangePosx() 
{
	m_dwOperate = ENUM_OBJECT_POS;
}

void CChangDlg::OnChangePosy() 
{
	m_dwOperate = ENUM_OBJECT_POS;
}

void CChangDlg::OnSetfocusPosx() 
{
	m_dwOperate = ENUM_OBJECT_POS;
}

void CChangDlg::OnSetfocusPosy() 
{
	m_dwOperate = ENUM_OBJECT_POS;	
}

void CChangDlg::OnSetfocusRotate() 
{
	m_dwOperate = ENUM_OBJECT_ROTATE;	
}

void CChangDlg::OnSetfocusSizex() 
{
	m_dwOperate = ENUM_OBJECT_SIZE;	
}

void CChangDlg::OnSetfocusSizey() 
{
	m_dwOperate = ENUM_OBJECT_SIZE;
	
}

void CChangDlg::OnSetfocusPropex() 
{
	m_dwOperate = ENUM_OBJECT_SCALE;
}

void CChangDlg::OnSetfocusPropey() 
{
	m_dwOperate = ENUM_OBJECT_SCALE;
}

BOOL CChangDlg::PreTranslateMessage(MSG* pMsg) 
{
	/*
	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F10) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_MARK_NORMAL , NULL);
		return TRUE;
	}

	if (((WM_SYSKEYDOWN  == pMsg->message) || (WM_KEYDOWN  == pMsg->message)) 
		&& (GetKeyState(VK_F7) & 0x8000))
	{
		AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FAST_MARK , NULL);
		return TRUE;
	}
	*/
	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_RETURN) & 0x8000))
	{
		return TRUE;
	}
	
	if ((WM_KEYDOWN  == pMsg->message) && (GetKeyState(VK_ESCAPE) & 0x8000))
	{
		return TRUE;
	}


	return CDialog::PreTranslateMessage(pMsg);
}

void CChangDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CDialog::OnMouseMove(nFlags, point);
}


void CChangDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}


void CChangDlg::UpdateProp(CQuad &quad) 
{
	BOOL bCheck = ((CButton*)(GetDlgItem(IDC_CHECK_CENTERPOS)))->GetCheck();
	
	if (!bCheck) 
	{
		CDot dotCur;
		dotCur = quad.GetBaseDot(m_nBase);
		
		m_dbX = dotCur.x;
		m_dbY = dotCur.y;

		m_dbX = MyPrecision(m_dbX,4);
		m_dbY = MyPrecision(m_dbY,4);
	}
	
	m_dbSizeX = quad.Width();
	m_dbSizeY = quad.Height();
	m_dbPropX = 100;
	m_dbPropY = 100;

	m_dbPropX  = MyPrecision(m_dbPropX);
	m_dbPropY  = MyPrecision(m_dbPropY);
	
	m_strSizeX.Format("%0.4f", m_dbSizeX);
	m_strSizeY.Format("%0.4f", m_dbSizeY);

    UpdateData(FALSE);

	return;	
}















