#include "stdafx.h"
#include "StrokeList.h"
#include "HLDoc.h"
#include "HLMotion.h"
#include "hl.h"

CStrokeList::CStrokeList(BOOL bAutoDeleteStroke)
{
	m_pArrayMarkFocus = NULL;
	m_bAutoDeleteStroke = bAutoDeleteStroke;
}


CStrokeList::~CStrokeList(void)
{
	if (m_bAutoDeleteStroke)
	{
		CStroke *pStroke;
		POSITION pos=m_list.GetHeadPosition();
		while(pos!=NULL)
		{	
			pStroke=m_list.GetNext(pos);
			delete pStroke;
		}
	}

	m_list.RemoveAll();
}

double  g_nFreq = 0.0;

BOOL CStrokeList::MarkNormal()
{
	WriteRunLog("mark strokelist", eLogTime);

	if (m_list.IsEmpty())
		return TRUE;

	g_DAT.MarkStart();

	CStroke *pStroke;
	POSITION pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);


		/*if (g_sys.m_bUseOpcLaser)
		{
			if (pStroke->m_dbQFre != g_nFreq)
			{
				BOOL bRet = g_OPCUaClient->writeFreq(pStroke->m_dbQFre);
				if (bRet)
				{
					g_nFreq = pStroke->m_dbQFre;
					WriteRunLog("g_OPCUaClient->writeFreq(pStroke->m_dbQFre);");
				}
				else
				{
					WriteRunLog("g_OPCUaClient->writeFreq Fail;");
				}
			}
		}*/
		if (!g_DAT.MarkStroke(*pStroke))
			return FALSE;
	}

	g_DAT.MarkEnd();

	return TRUE;
}


BOOL CStrokeList::MarkWithFocusType()
{
	if (m_list.IsEmpty())
		return TRUE;

	double dbZFocus;

	int nFocusType = m_list.GetHead()->m_nFocusType;
	CMarkFocus markFocus = m_pArrayMarkFocus->GetAt(nFocusType);

	for (int j=0; j<=markFocus.nMoveNum; j++)
	{
		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
			return FALSE;

		WriteRunLog("AMove eMaskZ to MarkFocus");
		dbZFocus = markFocus.dbZPos + markFocus.dbMoveDis*j;
		g_pMotion->AMove(eMaskZ, dbZFocus);

		MarkNormal();
	}
	return TRUE;
}

void CStrokeList::MarkByStrokeOrder()
{
	int nFocusType = -1;
	CStrokeList orderStrokeList;
	orderStrokeList.m_pArrayMarkFocus = m_pArrayMarkFocus;

	CStroke *pStroke;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pStroke = m_list.GetNext(pos);
		if (pStroke->m_nFocusType != -1)
		{
			if (nFocusType != pStroke->m_nFocusType)
			{
				if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
					return;

				orderStrokeList.MarkWithFocusType();

				orderStrokeList.m_list.RemoveAll();
				nFocusType = pStroke->m_nFocusType;
			}

			orderStrokeList.m_list.AddTail(pStroke);
		}
	}
	orderStrokeList.MarkWithFocusType();
}

void CStrokeList::MarkByFocusType()
{
	int nFocusType = -1;
	for (int i=0; i<m_pArrayMarkFocus->GetSize(); i++)
	{
		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
			return;

		CStrokeList tempStrokeList(FALSE);
		tempStrokeList.m_pArrayMarkFocus = m_pArrayMarkFocus;

		CStroke *pStroke;
		POSITION pos=m_list.GetHeadPosition();
		while(pos)
		{
			pStroke = m_list.GetNext(pos);
			if (pStroke->m_nFocusType==i)
				tempStrokeList.m_list.AddTail(pStroke);
		}

		tempStrokeList.MarkWithFocusType();
	}
}

BOOL CStrokeList::IsEmpty()
{
	return m_list.IsEmpty();
}

void CStrokeList::Empty()
{
	CStroke *pStroke;
	POSITION pos=m_list.GetHeadPosition();
	while(pos)
	{
		pStroke = m_list.GetNext(pos);
		delete pStroke;
	}
	m_list.RemoveAll();
}

BOOL CStrokeList::Move(double moveX,double moveY)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
			pStroke->Move(moveX,moveY);
		}
	}// end While

	return TRUE;
}

void CStrokeList::Rotate(CDot dot,double angle)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
			pStroke->Rotate(dot,angle);
		}
	}
}

void CStrokeList::Scale(CDot dot,double ratioX,double ratioY)
{
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		if (pStroke)
		{
			pStroke->Scale(dot,ratioX,ratioY);
		}
	}// end while
}


CQuad CStrokeList::CalLimit()
{
	m_quadLimit.left = m_quadLimit.right = m_quadLimit.bottom = m_quadLimit.top = 0.0;
	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		m_quadLimit.Compose(pStroke->CalLimit());
	}
	return m_quadLimit;
}