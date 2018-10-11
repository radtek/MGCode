///////////////////////////////////////////////////////////////////////////////
// Led.cpp : implementation file
///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-1999 Monte Variakojis ( monte@apollocom.com )
// All rights reserved - not to be sold.
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Led.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLed
///////////////////////////////////////////////////////////////////////////////
CLed::CLed()
{
	m_nLedMode = LED_OFF;
}

CLed::~CLed()
{
}


BEGIN_MESSAGE_MAP(CLed, CStatic)
	//{{AFX_MSG_MAP(CLed)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLed message handlers
///////////////////////////////////////////////////////////////////////////////
void CLed::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawLed(&dc,m_nLedMode);
}

void CLed::DrawLed(CDC *pDC, int nMode)
{
	CRect rect;
	GetClientRect(&rect);

	CBrush ActBrush,NoBrush;
	ActBrush.CreateSolidBrush(RED);
	NoBrush.CreateSolidBrush(GREEN);

	switch(nMode)
	{
	case CLed::LED_ON:
		pDC->FillRect(&rect,&ActBrush);
		break;
	case CLed::LED_OFF:
		pDC->FillRect(&rect,&NoBrush);
		break;
	}

	pDC->DrawEdge(&rect,BDR_SUNKENINNER|EDGE_SUNKEN,BF_RECT);
	ActBrush.DeleteObject();
	NoBrush.DeleteObject();
}

void CLed::SetLed(int nMode)
{
	m_nLedMode = nMode;
	Invalidate();
}

int CLed::GetLed()
{
	return m_nLedMode;
}