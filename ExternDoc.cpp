// ExternDoc.cpp : implementation file
//

#include "stdafx.h"
#include "hl.h"
#include "ExternDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExternDoc

IMPLEMENT_DYNCREATE(CExternDoc, CDocument)

CExternDoc::CExternDoc()
{
}

BOOL CExternDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CExternDoc::~CExternDoc()
{
// 	while (!m_list.IsEmpty())
// 	{
// 		delete m_list.RemoveHead();
// 	}
	while (!m_alterList.IsEmpty())
	{
		delete m_alterList.RemoveHead();
	}
}


BEGIN_MESSAGE_MAP(CExternDoc, CDocument)
	//{{AFX_MSG_MAP(CExternDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExternDoc diagnostics

#ifdef _DEBUG
void CExternDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CExternDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExternDoc serialization

void CExternDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CExternDoc commands

void CExternDoc::DrawList(XDC *pDC)
{
	CPen pen(PS_SOLID, 1, GRAY);
	CPen *pOldPen = pDC->m_pDC->SelectObject(&pen);
	POSITION pos = m_list.GetHeadPosition();
	while (pos != NULL)
	{
		CGroupObj *pGroupObj = m_list.GetNext(pos);
		pGroupObj->DrawExtern(pDC);
	}
	pDC->m_pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void CExternDoc::DrawAlterList(XDC *pDC)
{
	CPen pen(PS_SOLID, 1, RED);
	CPen *pOldPen = pDC->m_pDC->SelectObject(&pen);
	POSITION pos = m_alterList.GetHeadPosition();
	while (pos != NULL)
	{
		CGroupObj *pGroupObj = m_alterList.GetNext(pos);
		pGroupObj->DrawExtern(pDC);
	}
	pDC->m_pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void CExternDoc::ApplyExtern(double dbSpace, BOOL bOpt)
{
	DeleteAlterList();
	POSITION pos = m_list.GetHeadPosition();
	while (pos != NULL)
	{
		CGroupObj *pGroupObj = m_list.GetNext(pos);
		CGroupObj *pAlterGroupObj = pGroupObj->ExtendObj(dbSpace, bOpt);
		if(pAlterGroupObj != NULL)
		{
			m_alterList.AddTail(pAlterGroupObj);
		}
	}
	UpdateAllViews(NULL);
}

void CExternDoc::DeleteList()
{
	while (!m_list.IsEmpty())
	{
		delete m_list.RemoveHead();
	}
}

void CExternDoc::DeleteAlterList()
{
	while (!m_alterList.IsEmpty())
	{
		delete m_alterList.RemoveHead();
	}
}

CQuad CExternDoc::CalLimit()
{
	CQuad quad;
	quad.SetEmpty();

	POSITION pos = m_list.GetHeadPosition();
	while (pos != NULL)
	{
		CGroupObj *pGroupObj = m_list.GetNext(pos);
		quad.Union(pGroupObj->GetLimit());
	}
	return quad;
}
