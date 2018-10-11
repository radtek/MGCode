// VectorMgr.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "VectorMgr.h"

#include "Nodeplt.h"
#include "NodeDxf_GP.h"
#include "NodeNC.h"


CVectorMgr g_VectorMgr;
// CVectorMgr
CVectorMgr::CVectorMgr()
{
}



CVectorMgr::~CVectorMgr()
{
}




// CVectorMgr 成员函数
BOOL CVectorMgr::Decode(CString strFileName, CString strFileExt, CTree *pTree, CProperty pty, BOOL bCenter, BOOL bBetter)
{
	if( pTree == NULL )
		return FALSE;

	CArray<CNodeLine *, CNodeLine *>  arrayNode;
	if( FALSE == Decode(strFileName, strFileExt, arrayNode, pty, bCenter, bBetter) )
		return FALSE;

	CNodeLine *pNode = NULL;
	for(int i=0; i<arrayNode.GetSize(); i++)
	{
		pNode = arrayNode.GetAt(i);
		
		if( pNode != NULL )
		{
			CTree treeTemp(pNode);
			pTree->Group(&treeTemp);
			pNode = NULL;
		}
	}
	
	return TRUE;
}





BOOL CVectorMgr::Decode(CString strFileName, CString strFileExt, CArray<CNodeLine *, CNodeLine *> &arrayNode, CProperty pty, BOOL bCenter, BOOL bBetter)
{
	BOOL bRet = TRUE;
	strFileExt.MakeUpper();
	
	CNodeLine *pNodeTemp = NULL;
	if( 0 == strFileExt.CompareNoCase(_T("DXF") ) )
	{
		CNodeDxf_GP *pNode = new CNodeDxf_GP();
		if( NULL == pNode )
			return FALSE;
		
		pNode->SetProperty(PTY_INIT,pty);
		if( pNode->DeCode(strFileName, bCenter, bBetter))
		{
			while( 1 )
			{
				pNodeTemp =	pNode->UnGroupDxf_GP();
				if( pNodeTemp == NULL )
				{
					break;
				}
				else
				{
					//pNodeTemp->InitQuadRange();
					arrayNode.Add(pNodeTemp);
				}
			}
			delete pNode;
		}
		else
		{
			delete pNode;
		}
	}
	else 
	{
		bRet = FALSE;
	}
	
	return bRet;
}





BOOL CVectorMgr::SaveAs(CChain *pChain, int nVectorType, int nFlag )
{
	return TRUE;
}