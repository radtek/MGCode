/********************************************************************
* 	Project  : NetMonitor
* 	FileName : ETListCtrl.h
* 	Change   : 	
* 	Brief    :  	
* 	Author   : Chen Jun ( chenjun@3cis.com.cn )
* 	Copyright ( c ) 2007-2008 3cis 
* 	All Right Reserved
*********************************************************************/

#if !defined( AFX_LISTCTRLEX_H__3D2C6B4A_4031_48EF_8162_492882D99D43__INCLUDED_ )
#define AFX_LISTCTRLEX_H__3D2C6B4A_4031_48EF_8162_492882D99D43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#include "ETButton.h"
#include <map>
using namespace std;

typedef map<int,CETButton*>button_map;


/************************************************************************/
/* 
* this CETListCtrl class inherits from ListCtrl
* it display the terminal list and record interrelated infomation
*/ 
/************************************************************************/
class CETListCtrl : public CListCtrl
{
// �R���X�g���N�V����
public:
	CETListCtrl();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL( CETListCtrl )
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:

	virtual ~CETListCtrl();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG( CETListCtrl )
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void createItemButton( int nItem, int nSubItem, HWND hMain ,CString strCaption);
	void release();
	void deleteItemEx( int nItem );
	button_map m_mButton;

public:
	afx_msg void OnLvnEndScroll(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	UINT m_uID;
	void updateListCtrlButtonPos();
	void enableButton( BOOL bFlag, int iItem );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif  // !defined( AFX_LISTCTRLEX_H__3D2C6B4A_4031_48EF_8162_492882D99D43__INCLUDED_ )
