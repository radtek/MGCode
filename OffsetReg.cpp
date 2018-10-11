// OffsetReg.cpp: implementation of the COffsetReg class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "hl.h"
#include "OffsetReg.h"
#include "ExternStroke.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COffsetReg::COffsetReg(double dbOffset)
{
	//��ֵ��������ֵ����
	m_dbOffset = dbOffset;
}

COffsetReg::~COffsetReg()
{
}

//plistData:ԭʼ��������
//listOffSetData:������ɵ�����
void COffsetReg::Proc(StrokeList *plistData, StrokeList &listOffSetData)
{
	if(fabs(m_dbOffset)<0.0001)
		return;
	
	m_pList = plistData;
	
	int nSize=plistData->GetCount();
	if (nSize<=0)
	{
		return;
	}
	CStroke* pStroke;
	BOOL* pbOutLine=new BOOL[nSize]; //�������濴List�Ƿ�Ϊ������
	if (pbOutLine==NULL)
	{
		return;
	}


	int i=-1;
	POSITION pos=plistData->GetHeadPosition();
	while (pos!=NULL)
	{
		pStroke=plistData->GetNext(pos);
		i++;
		if (!pStroke->IsRegion())
		{
			if (pbOutLine)
			{
				delete pbOutLine;
				pbOutLine=NULL;
			}
			DSF_TRACE("\nOffSet Err:Stroke isn't closed!");
			return; //���ڷǷ�����ߣ�����
		}
		pbOutLine[i]=pStroke->ISOutLineStroke(plistData); //pStroke����һ�������е���һˮƽ����ɨ���ߣ���ɨpListData���ж��Ƿ�Ϊ������
	}



	pos=plistData->GetHeadPosition();
	
	CProperty pty;
	i=-1;
	while(pos!=NULL){
		//��������������
		pStroke=plistData->GetNext(pos);
		i++;
		pty = pStroke->m_property;

		if(pStroke->IsRegion())
		{
			//�Ƿ������,ELSE,�Ƿ������,continue;
			StrokeList tmp;
			CStroke StrokeTmp(pty);

// 			//˳ʱ�봦��
//  			pStroke->ClockWise(TRUE);
			//����ƽ����
			pStroke->Excursion(&StrokeTmp,(pbOutLine[i]?m_dbOffset:m_dbOffset*(-1)));
			StrokeTmp.FilterExcursion(tmp);
			
			POSITION poslist = tmp.GetHeadPosition();
			CStroke* pTmp;

			while(poslist!=NULL){
				pTmp = tmp.GetNext(poslist);
				listOffSetData.AddTail(pTmp);
			}

			tmp.RemoveAll();
		}
	}
	if (pbOutLine)
	{
		delete pbOutLine;
		pbOutLine=NULL;
	}
}


void COffsetReg::DsfProc(StrokeList *plistData, StrokeList &listOffSetData, BOOL bOpt)
{
	CExternStroke externAdmin;
	if(fabs(m_dbOffset)<0.0001)
		return;
	
	m_pList = plistData;
	
	int nSize=plistData->GetCount();
	if (nSize<=0)
	{
		return;
	}
	CStroke* pStroke;
	BOOL* pbOutLine=new BOOL[nSize]; //�������濴List�Ƿ�Ϊ������
	if (pbOutLine==NULL)
	{
		return;
	}
	
	
	int i=-1;
	POSITION pos=plistData->GetHeadPosition();
	while (pos!=NULL)
	{
		pStroke=plistData->GetAt(pos);
		i++;
		if (!pStroke->IsRegion())
		{
			if (pbOutLine)
			{
				delete pbOutLine;
				pbOutLine=NULL;
			}
			DSF_TRACE("\nOffSet Err:Stroke isn't closed!");
			return; //���ڷǷ�����ߣ�����
		}
		//pbOutLine[i]=externAdmin.IsOutStroke(*pStroke, *plistData, pos);
		pbOutLine[i]=pStroke->ISOutLineStroke(plistData);
		plistData->GetNext(pos);
	}
	
	
	
	pos=plistData->GetHeadPosition();
	
	CProperty pty;
	i=-1;
	while(pos!=NULL){
		//��������������
		pStroke=plistData->GetNext(pos);
		i++;
		pty = pStroke->m_property;
		
		if(pStroke->IsRegion())
		{
			CStroke *pStrokeTemp = new CStroke(pty);
			externAdmin.DoExtern(*pStroke, *pStrokeTemp, (pbOutLine[i]?m_dbOffset:m_dbOffset*(-1)), bOpt);

			StrokeList tmp;
			pStrokeTemp->FilterExcursion(tmp);
			BOOL bExceptFlag = FALSE;
			BOOL bFindFlag = FALSE;
			if(tmp.GetCount()>1) bExceptFlag=TRUE;
			POSITION poslist = tmp.GetHeadPosition();
			CStroke* pTmp;
			while(poslist!=NULL){
				pTmp = tmp.GetNext(poslist);
				if (bExceptFlag)
				{
					int dotCount = pTmp->m_dotArray.GetSize()-1;
					for (int i=0; i<dotCount; i++)
					{
						CDot tDot = pTmp->m_dotArray.GetAt(i);
						for (int j=0; j<externAdmin.m_arrayExceptDot.GetSize(); j++)
						{
							CDot fDot = externAdmin.m_arrayExceptDot.GetAt(j);
							if(tDot==fDot)
							{
								bFindFlag=TRUE;
								break;
							}
						}
						if(bFindFlag) break;
					}
					if(bFindFlag) 
					{
						delete pTmp;
						continue;
					}
				}
				listOffSetData.AddTail(pTmp);
			}
			tmp.RemoveAll();

			delete pStrokeTemp;

// 			if (pStrokeTemp->m_dotArray.GetSize()>0)
// 			{
// 				listOffSetData.AddTail(pStrokeTemp);
// 			}
		}
	}
	if (pbOutLine)
	{
		delete pbOutLine;
		pbOutLine=NULL;
	}
}

