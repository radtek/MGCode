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
	//正值外扩，负值内缩
	m_dbOffset = dbOffset;
}

COffsetReg::~COffsetReg()
{
}

//plistData:原始线条数据
//listOffSetData:最后生成的数据
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
	BOOL* pbOutLine=new BOOL[nSize]; //用来保存看List是否为外轮廓
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
			return; //存在非封闭曲线，返回
		}
		pbOutLine[i]=pStroke->ISOutLineStroke(plistData); //pStroke内找一线条的中点做一水平线做扫描线，找扫pListData来判断是否为外轮廓
	}



	pos=plistData->GetHeadPosition();
	
	CProperty pty;
	i=-1;
	while(pos!=NULL){
		//按线条处理数据
		pStroke=plistData->GetNext(pos);
		i++;
		pty = pStroke->m_property;

		if(pStroke->IsRegion())
		{
			//是封闭曲线,ELSE,非封闭曲线,continue;
			StrokeList tmp;
			CStroke StrokeTmp(pty);

// 			//顺时针处理
//  			pStroke->ClockWise(TRUE);
			//计算平等线
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
	BOOL* pbOutLine=new BOOL[nSize]; //用来保存看List是否为外轮廓
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
			return; //存在非封闭曲线，返回
		}
		//pbOutLine[i]=externAdmin.IsOutStroke(*pStroke, *plistData, pos);
		pbOutLine[i]=pStroke->ISOutLineStroke(plistData);
		plistData->GetNext(pos);
	}
	
	
	
	pos=plistData->GetHeadPosition();
	
	CProperty pty;
	i=-1;
	while(pos!=NULL){
		//按线条处理数据
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

