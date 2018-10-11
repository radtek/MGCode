// ETButton.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "ETButton.h"


// CButtonEx

IMPLEMENT_DYNAMIC(CETButton, CButton)

CETButton::CETButton()
{

}

CETButton::CETButton( int nItem, int nSubItem, CRect rect, HWND hParent )
{
	m_inItem = nItem;
	m_inSubItem = nSubItem;
	m_rect = rect;
	m_hParent = hParent;
	bEnable = TRUE;
}

CETButton::~CETButton()
{
}


BEGIN_MESSAGE_MAP(CETButton, CButton)
	ON_CONTROL_REFLECT(BN_CLICKED, &CETButton::OnBnClicked)
END_MESSAGE_MAP()



// CETButton ���b�Z�[�W �n���h��



void CETButton::OnBnClicked()
{
	::SendMessage( m_hParent, WM_BN_CLICK, m_inItem, m_inSubItem );
}
