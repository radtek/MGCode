/* 
填充类:本类专为给矢量图填充用
	作    者:曹锋
	更新时间:20060902
	版 本 号:1.1
//*/

#include "stdafx.h"
#include "hl.h"
#include "Fill.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CFill::CFill(double space,double dbBorderSpace,double dbAngle,
			 UINT style,CProperty property,double dbPtp,double dbCircle,double dbHeadKillLen,double dbTailKillLen)
{
	m_dbSpace = space;
	m_dbBorderSpace = dbBorderSpace;
	m_nStyle = style;
	m_dbAngle = dbAngle;
	m_property = property;
	m_dbPtp = dbPtp;
    m_dbCircle = dbCircle;
	m_dbHeadKillLen = -dbHeadKillLen;
	m_dbTailKillLen = -dbTailKillLen;
}

CFill::~CFill()
{
}


//plistData:线条数据，不产生实体只是从TREE中扫出所有的线条指针
//listFillData:填充后的数据用于输出，在此函数中只会往里加数据
void CFill::Proc(StrokeList *plistData,StrokeList &listFillData)
{
	m_plist = plistData;

	CArray<StrokeList*, StrokeList*> FillList;
	StrokeList FillStroke;

	CQuad quad;
	CDot dotCenter;
	quad = CalLimit();
	dotCenter = quad.GetBaseDot();

	//////////////////////////////////////////////////////////////////////////
	//数据转动到一定角度值
	double angle = m_dbAngle;
	Rotate(m_plist,dotCenter,angle);
	//////////////////////////////////////////////////////////////////////////

	int nNum = 0;	//划分成几个层
	double dbFillBorderSpace=m_dbBorderSpace;			//填充边框边距
	double dbfillSpace = m_dbSpace;
	double curpos = 0;

	quad = CalLimit();
	nNum = (int)((quad.Height()-dbFillBorderSpace*2)/dbfillSpace);

	if(m_nStyle&FILL_EVEN){
		//使用平均化处理，即点间距一致
		dbfillSpace = (quad.Height()-dbFillBorderSpace*2)/nNum;
	}

	curpos = quad.bottom+dbFillBorderSpace;
	
	BOOL bBackFillDirect=FALSE;			//扫描线是否反向
	CStroke* pStroke;
	DotList listInfo[3];				//标准线条数据，与老版本方式一样
	CDot dottmp;
	double str,end;
	nNum++;

//	dbFillBorderSpace = dbFillBorderSpace>dbfillSpace?dbfillSpace:dbFillBorderSpace;

	while( --nNum>=0 ){
		//扫描相近的3条线确定出来的LISTINFO数据表
//		double sstr = curpos+dbFillBorderSpace;
//		double send = curpos-dbFillBorderSpace;
//		double tmp=MyTrueToLog(0.01);			//填充边框边距
//		str = curpos+tmp;
//		end = curpos-tmp;
//		if(send<quad.bottom){
//			curpos += dbfillSpace;
//			continue;
//		}
//		if(sstr>quad.top){
//			curpos += dbfillSpace;
//			continue;
//		}
// 		end = end<quad.bottom?quad.bottom:end;
// 		str = str>quad.top?quad.top:str;
		str = curpos+dbFillBorderSpace;
		end = curpos-dbFillBorderSpace;
		end = end<quad.bottom?quad.bottom+0.002:end+0.002;
		str = str>quad.top?quad.top-0.002:str-0.002;
		
 		ScanFill(listInfo,curpos,str,end);

		DotList dotArray;
		//将所有的上下结点数据整合成一个数组放入DOTARRAY中
		CreateDotArray(listInfo,dotArray);
		double* pList;

		//排列个数
		int nCount = dotArray.GetSize();
		pList = new double[nCount];

		for(int i=0;i<nCount;i++)
		{
			dottmp = dotArray.GetAt(i);
			pList[i] = dottmp.x;
		}

		//将划线区域重新按指定方式进行从大到小或从小到大排列
		sort_bubble(pList,nCount,bBackFillDirect);
		Sortlist(listInfo[0],bBackFillDirect);

		StrokeList *pstrList = new StrokeList;
		
		int nSize = listInfo[0].GetSize();
		for(int i=0;i<nSize/2;i++)
		{
			//标准线取出有效区
			double ox1,ox2,sx1,sx2;
			sx1 = (listInfo[0].GetAt(i*2)).x;
			sx2 = (listInfo[0].GetAt(i*2+1)).x;
			for(int j=0;j<nCount/2;j++)
			{
				if( (j*2+1<nCount)&&(j*2+2<nCount) )
				{
					ox1=sx1;
					ox2=sx2;
					if( IsValide(pList[j*2+1],pList[j*2+2],ox1,ox2) ){
						//确定是不是数据区域如果是则加入线条
//						if( fabs(ox1-ox2)<=dbFillBorderSpace*2 )
//							continue;
						pStroke = new CStroke(m_property);
						pstrList->AddTail(pStroke);

						if(bBackFillDirect)
						{
							dottmp.y = curpos;
							dottmp.x = ox1;
							pStroke->Add(dottmp);
							dottmp.x = ox2;
							pStroke->Add(dottmp);
						}
						else
						{
							dottmp.y = curpos;
							dottmp.x = ox1;
							pStroke->Add(dottmp);
							dottmp.x = ox2;
							pStroke->Add(dottmp);
						}

						ZoomStroke(pStroke);
					}
				}
			}
		}

		FillList.Add(pstrList);

		//进入下一个数据切分
		delete []pList;
		listInfo[0].RemoveAll();
		listInfo[1].RemoveAll();
		listInfo[2].RemoveAll();
		curpos += dbfillSpace;
		if( m_nStyle&FILL_BACK )
			bBackFillDirect=!bBackFillDirect;
	}


    CArray<StrokeList*, StrokeList*> RetList;
	if( m_nStyle&FILL_BETTER )
	{
		CArray<StrokeList*, StrokeList*> ListStrk;
		BetterProc(FillList, ListStrk);
		ListSortProc(ListStrk, RetList);

		CirclLinePro(RetList, listFillData);
	}
	else
	{
		AddToOneList(FillList, RetList);
		CirclLinePro(RetList, listFillData);
		//NobetterPro(FillList, listFillData);
	}

	//点间距设置
	if( m_nStyle&FILL_PTP )
	{
		PtpProc(&listFillData);
	}

	//////////////////////////////////////////////////////////////////////////
	//数据线条还原到原始位置
	Rotate(m_plist,dotCenter,-angle);
	//填充线条还原到原始位置
	Rotate(&listFillData,dotCenter,-angle);
	/////////////////////////////////////////////////////////////////////////

	return;
}


void CFill::AddToOneList(CArray<StrokeList*, StrokeList*> &FillList, CArray<StrokeList*, StrokeList*> &RetList)
{
	StrokeList *plistData = NULL;
	int nCount = FillList.GetSize();
	if (nCount > 0)
	{
		plistData = new StrokeList;
	}
	for (int i = 0; i < nCount; i++)
	{
		StrokeList* pStrkList = FillList.GetAt(i);
		POSITION pos = pStrkList->GetHeadPosition();
		while (pos != NULL)
		{
			CStroke *pStrk = pStrkList->GetNext(pos);

			plistData->AddTail(pStrk);
			
		}// end while
	}// end for (

	if (plistData) 
	{
		RetList.Add(plistData);
	}


	int nFilllistCount = FillList.GetSize();
	for (int j = 0; j < nFilllistCount; j++)
	{
		StrokeList* pStrokeList = FillList.GetAt(j);
		if (NULL == pStrokeList) 
		{
			continue;
		}
		pStrokeList->RemoveAll();
		delete pStrokeList;
		pStrokeList = NULL;
	}
	FillList.RemoveAll();

	return;
}


void CFill::NobetterPro(CArray<StrokeList*, StrokeList*> &FillList, StrokeList& listFillData)
{
	int nCount = FillList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		StrokeList* pStrkList = FillList.GetAt(i);
		POSITION pos = pStrkList->GetHeadPosition();
		while (pos != NULL)
		{
			CStroke *pStrk = pStrkList->GetNext(pos);

			listFillData.AddTail(pStrk);
			
		}// end while
	}// end for (


	int nFilllistCount = FillList.GetSize();
	for (int j = 0; j < nFilllistCount; j++)
	{
		StrokeList* pStrokeList = FillList.GetAt(j);
		if (NULL == pStrokeList) 
		{
			continue;
		}
		pStrokeList->RemoveAll();
		delete pStrokeList;
		pStrokeList = NULL;
	}
	FillList.RemoveAll();

	return;
}

void CFill::CirclLinePro(CArray<StrokeList*, StrokeList*> &RetList, StrokeList& listFillData)
{
	if( m_nStyle&FILL_CIRCLE )
	{
		int nlistCount = RetList.GetSize();
		for (int i = 0; i < nlistCount; i++)
		{
			StrokeList* pStrokeList = RetList.GetAt(i);
			if (NULL == pStrokeList) 
			{
				continue;
			}

			CirclProc(pStrokeList, &listFillData, m_dbCircle);
			DeletStrokeList(pStrokeList);
			pStrokeList->RemoveAll();
			delete pStrokeList;
			pStrokeList = NULL;
		}
	}
	else
	{
		int nlistCount = RetList.GetSize();
		for (int i = 0; i < nlistCount; i++)
		{
			StrokeList* pStrokeList = RetList.GetAt(i);
			if (NULL == pStrokeList) 
			{
				continue;
			}

			POSITION posl = pStrokeList->GetHeadPosition();
			while(posl!=NULL)
			{
				CStroke *pStroke = pStrokeList->GetNext(posl);
				listFillData.AddTail(pStroke);
			}

			pStrokeList->RemoveAll();
			delete pStrokeList;
			pStrokeList = NULL;
		}
	}

	return;
}

void CFill::ReleaseFillList(CArray<StrokeList*, StrokeList*> &FillList)
{
	int nFilllistCount = FillList.GetSize();
	for (int j = 0; j < nFilllistCount; j++)
	{
		StrokeList* pStrokeList = FillList.GetAt(j);
		if (NULL == pStrokeList) 
		{
			continue;
		}
		pStrokeList->RemoveAll();
		delete pStrokeList;
		pStrokeList = NULL;
	}
	FillList.RemoveAll();

	return;
}

//bFlag:FALSE是从小到大TRUE是从大到小
void CFill::sort_bubble(double *plist, int size,BOOL bFlag)
{
	double tmp;
	int i,j;
	
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			if(!bFlag){
				if(plist[i]<plist[j]){
					tmp = plist[i];
					plist[i] = plist[j];
					plist[j] = tmp;
				}
			}
			if(bFlag){
				if(plist[i]>plist[j]){
					tmp = plist[i];
					plist[i] = plist[j];
					plist[j] = tmp;
				}
			}
		}
	}
}

void CFill::CreateDotArray(DotList *pList, DotList &dotArray)
{
	CDot dot;
	int nCount;

	nCount = pList[1].GetSize();
	for(int i=0;i<nCount;i++)
	{
		dot = pList[1].GetAt(i);
		dotArray.Add(dot);
	}

	nCount = pList[2].GetSize();
	for(int i=0;i<nCount;i++)
	{
		dot = pList[2].GetAt(i);
		dotArray.Add(dot);
	}
}

void CFill::ScanFill(DotList* listInfo, double scan,double str,double end)
{
	POSITION pos=m_plist->GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL){	
		pStroke=m_plist->GetNext(pos);
 		pStroke->ScanFill(listInfo,scan,str,end);
	}
}

//plist:转动数据
//angle:弧度值
//dot:中心点坐标
void CFill::Rotate(StrokeList* plist,CDot dot, double angle)
{
	POSITION pos=plist->GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL){	
		pStroke=plist->GetNext(pos);
		pStroke->Rotate(dot,angle);
	}
}

CQuad CFill::CalLimit()
{
	m_quadLimit.SetEmpty();

	CQuad quad;
	POSITION pos = m_plist->GetHeadPosition();
	CStroke *pStroke;

	while(pos!=NULL){
		pStroke=m_plist->GetNext(pos);
		quad = pStroke->CalLimit();
		m_quadLimit.Union(quad);
	}
	m_quadLimit.Normal();

	return m_quadLimit;
}

void CFill::Sortlist(DotList& dotArray,BOOL bFlag)
{
	//交叉点排序
	CDot doti,dotj;
	
	int m = dotArray.GetSize();
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<m;j++)
		{
			doti = dotArray[i];
			dotj = dotArray[j];

			if(!bFlag){
				if(doti.x<dotj.x){
					dotArray.SetAt(i,dotj);
					dotArray.SetAt(j,doti);
				}
			}
			if(bFlag){
				if(doti.x>dotj.x){
					dotArray.SetAt(i,dotj);
					dotArray.SetAt(j,doti);
				}
			}
		}
	}
}

void CFill::DeleteTmpList(StrokeList *list)
{
	POSITION prev;
	POSITION pos;
	CStroke *pStroke;

	pos=list->GetHeadPosition();
	while(pos!=NULL)
	{
		prev=pos;
		pStroke=list->GetNext(pos);
		
		list->RemoveAt(prev);
		delete pStroke;
	}
}

/*
x1,x2为限制点，如果ox1,ox2(标准线)减去边界值仍不到限制点，则以限制点算
*/
BOOL CFill::IsValide(double x1,double x2,double &ox1,double &ox2)
{
	double angle = m_dbAngle;
	double dbBorderSpace = m_dbBorderSpace;
// 	dbBorderSpace = (m_dbAngle<45)?m_dbBorderSpace/cos(angle):m_dbBorderSpace/sin(angle);
	double dbFillBorderSpace=MyTrueToLog(dbBorderSpace);			//填充边框边距
	double tx1,tx2,tox1,tox2;
	BOOL bRev=FALSE;

//	if(fabs(x1-x2)<2*dbFillBorderSpace)
//		return FALSE;

	if(x1>x2){
		tx1=x2;
		tx2=x1;
		tox1=ox2;
		tox2=ox1;
		bRev = TRUE;
	}
	else{
		tx1=x1;
		tx2=x2;
		tox1=ox1;
		tox2=ox2;
	}
	//确立TX1<TX2,TOX1<TOX2
	if(tox2<=tx1){
		//去除以下情况
		//              X   X            tx1     tx2
		// ------------                 tox1    tox2
		return FALSE;
	}
	else if (tox1>=tx2){
		// X   X               断点
		//       ------------  标准线
		return FALSE;
	}
	else if(tx1<=tox1&&tx2>=tox2){
		//X              X
		//  ------------ 
		tox1+=dbFillBorderSpace;
		tox2-=dbFillBorderSpace;
	}
	else if(tx1<=tox1&&tx2>=tox1){
		//X    X
		//  ------------ 
		if(fabs(tx2-tox1)<=2*dbFillBorderSpace)
			return FALSE;
		tox1 += dbFillBorderSpace;
		tox2 = fabs(tox2-tx2)>dbFillBorderSpace?tx2-dbFillBorderSpace:tox2-dbFillBorderSpace;
	}
	else if (tx1<=tox2&&tx2>=tox2){
		//           X    X
		//  ------------ 
		if(fabs(tx1-tox2)<=2*dbFillBorderSpace)
			return FALSE;
		tox1 = fabs(tox1-tx1)>dbFillBorderSpace?tx1+dbFillBorderSpace:tox1+dbFillBorderSpace;
		tox2 -= dbFillBorderSpace;
	}
	else if (tx1>=tox1&&tx2<=tox2){
		//     X    X
		//  ------------ 
		tox1 = fabs(tox1-tx1)>dbFillBorderSpace?tx1+dbFillBorderSpace:tox1+dbFillBorderSpace;
		tox2 = fabs(tox2-tx2)>dbFillBorderSpace?tx2-dbFillBorderSpace:tox2-dbFillBorderSpace;
	}
	else{
		int a=0;//for debug;
	}

	if((tox2-tox1)<=0)
		return FALSE;

	if(bRev){
		ox1=tox2;
		ox2=tox1;
	}
	else{
		ox1=tox1;
		ox2=tox2;
	}
	return TRUE;
}

void CFill::PtpProc(StrokeList *pList)
{
	//以X方向上的零点为基准点进行处理
	POSITION pos=pList->GetHeadPosition();
	CStroke *pStroke;
	CDot dotStart,dotEnd,dotTmp;

	BOOL bStart=TRUE;
	BOOL bDir = TRUE;	//错点处理
	double tmpy,tmpx;
	int nCount;

	while(pos!=NULL){
		pStroke=pList->GetNext(pos);
		dotStart = pStroke->m_dotArray.GetAt(0);
		dotEnd = pStroke->m_dotArray.GetAt(1);
//		dotStart.LogToTrue();
//		dotEnd.LogToTrue();
		dotTmp = dotStart;
		if(bStart){
			bStart = FALSE;
			tmpy = dotTmp.y;
		}
		if(fabs(tmpy-dotTmp.y)>0.0002){
			tmpy = dotTmp.y;
			bDir = !bDir;
		}
		if(bDir){
			tmpx = (dotStart.x+dotEnd.x)/2;
			nCount = (int)(fabs(dotStart.x-tmpx)/m_dbPtp);
			if(dotStart.x>tmpx){
				dotTmp.x = tmpx+nCount*m_dbPtp;
			}
			else{
				dotTmp.x = tmpx-nCount*m_dbPtp;
			}
		}
		else{
			tmpx = (dotStart.x+dotEnd.x)/2+m_dbPtp/2;
			nCount = (int)(fabs(dotStart.x-tmpx)/m_dbPtp);
			if(dotStart.x>tmpx){
				dotTmp.x = tmpx+nCount*m_dbPtp;
			}
			else{
				dotTmp.x = tmpx-nCount*m_dbPtp;
			}
		}
//		dotTmp.TrueToLog();
 		pStroke->m_dotArray.InsertAt(1,dotTmp);
// 		pStroke->m_dotArray.SetAt(0,dotTmp);
	}
}

void CFill::CirclProc(StrokeList *pList, StrokeList *pRetList, double dbLen)
{
	CStroke *pStrTemp = NULL;
	CDot dotS,dotE;

	BOOL bFirst=TRUE;
	StrokeList strList;

	POSITION pos=pList->GetHeadPosition();
	while(pos!=NULL)
	{
		CStroke *pStroke = pList->GetNext(pos);
		if (bFirst)
		{
			pStrTemp  =  pStroke->Copy();
			strList.AddTail(pStrTemp);
			bFirst = FALSE;
			int nCount = pStrTemp->GetSize();
			if (nCount > 0) 
			{
				dotE = pStrTemp->GetAt(nCount-1);
			}
			continue;
		}

		int nCount = pStroke->GetSize();
		if (nCount > 0) 
		{
			 dotS = pStroke->GetAt(0);
			 double dbD = Distance(dotE, dotS);
			 if (fabs(dbD) <= dbLen) 
			 {
				 for (int i = 0; i < nCount; i ++)
				 {
					 pStrTemp->Add(pStroke->m_dotArray.GetAt(i));
				 }
				 dotE = pStroke->m_dotArray.GetAt(nCount - 1);
			 }
			 else
			 {
				 pStrTemp  =  pStroke->Copy();
			 	 strList.AddTail(pStrTemp);
				 int nCount = pStrTemp->GetSize();
				 if (nCount > 0) 
				 {
					 dotE = pStrTemp->GetAt(nCount-1);
				 }
			 }
		} // if (nCount > 0) 
	}

	DeletStrokeList(pList);

	POSITION posl = strList.GetHeadPosition();
	while(posl!=NULL)
	{
		CStroke *pStroke = strList.GetNext(posl);
		pRetList->AddTail(pStroke);
	}

	strList.RemoveAll();

	return;
}

BOOL CFill::DeletStrokeList(StrokeList* pList)
{
	if (NULL == pList)
	{
		return FALSE;
	}

	POSITION prev;
	CStroke *pStroke = NULL;

	POSITION posd = pList->GetHeadPosition();
	while(posd!=NULL)
	{	
		prev=posd;
		pStroke=pList->GetNext(posd);
		pList->RemoveAt(prev);
		if (pStroke)
		{
			delete pStroke;
		    pStroke = NULL;
		}
	}// end while

	pList->RemoveAll();

	return TRUE;
}

double CFill::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return dbD;
}

void CFill::BetterProc(CArray<StrokeList*, StrokeList*> &FillList
					   , CArray<StrokeList*, StrokeList*> &RetList)
{
	int nCount = FillList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		StrokeList* pStrkList = FillList.GetAt(i);
		POSITION pos = pStrkList->GetHeadPosition();
		while (pos != NULL)
		{
			CStroke *pStrk = pStrkList->GetNext(pos);

			int nDtCount = pStrk->GetSize();
			if (nDtCount <= 0)
			{
				continue;
			}
			
			BOOL bRet = StrokeProc(pStrk, RetList);
			if (!bRet)
			{
				delete pStrk;
				pStrk = NULL;
				continue;
			}
		}// end while
	}// end for (

	ReleaseFillList(FillList);
	
	return;
}

/*
void CFill::ListSortProc(CArray<StrokeList*, StrokeList*> &FillList
					   , CArray<StrokeList*, StrokeList*> &RetList)
{
	int nCount = FillList.GetSize();
	if (nCount <= 0)
	{
		return;
	}

	StrokeList *pFList = FillList.GetAt(0);
	RetList.Add(pFList);
	FillList.RemoveAt(0);

	CDot dotT;
	POSITION pos = pFList->GetTailPosition();
	CStroke * pStroke = NULL;
	if (pos != NULL)
	{
		pStroke = pFList->GetNext(pos);
	}

	int nStrkCount = pStroke->GetSize();
	if (nStrkCount <= 0)
	{
		return;
	}

	// 第一个StrokeList的最后一个点
	dotT = pStroke->GetAt(nStrkCount - 1);

	int RCount = RetList.GetSize();
	while ( RCount < nCount)
	{
		CDot dot_H;
		int nPosH = -1;
		double dbMinDsH = 99999999999999;
/////////////////////////////////////////////
		// 尾与头连接
		for (int i = 0; i < FillList.GetSize(); i++)
		{
			StrokeList *pSList = FillList.GetAt(i);
			if (NULL == pSList)
			{
				continue;
			}

			CDot dotH;
			POSITION pos = pSList->GetHeadPosition();
			CStroke * pStroke = NULL;
			if (pos != NULL)
			{
				pStroke = pSList->GetNext(pos);
			}

			int nStrkCount = pStroke->GetSize();
			if (nStrkCount <= 0)
			{
				return;
			}

			CDot dot = pStroke->GetAt(0);
			double dbD = Distance(dotT, dot);
			if (fabs(dbD) <= dbMinDsH)
			{
				dbMinDsH   = fabs(dbD);
				nPosH = i;

				POSITION pos = pSList->GetTailPosition();
				CStroke * pStroke = NULL;
				if (pos != NULL)
				{
					pStroke = pSList->GetNext(pos);
				}

				int nStrkCount = pStroke->GetSize();
				if (nStrkCount <= 0)
				{
					return;
				}
				dot_H = pStroke->GetAt(nStrkCount - 1);
			}
		}// end for
///////////////////////////////////////////////////////////

///////////////--------------------------
		CDot dot_T;
		int nPosT = -1;
		double dbMinDsT = 99999999999999;
		// 尾与尾连接
		for (int j = 0; j < FillList.GetSize(); j++)
		{
			StrokeList *pSList = FillList.GetAt(j);
			if (NULL == pSList)
			{
				continue;
			}

			POSITION pos = pSList->GetTailPosition();
			CStroke * pStroke = NULL;
			if (pos != NULL)
			{
				pStroke = pSList->GetNext(pos);
			}

			int nStrkCount = pStroke->GetSize();
			if (nStrkCount <= 0)
			{
				return;
			}

			CDot dot = pStroke->GetAt(0);
			double dbD = Distance(dotT, dot);
			if (fabs(dbD) <= dbMinDsT)
			{
				dbMinDsT   = fabs(dbD);
				nPosT = j;

				POSITION pos = pSList->GetHeadPosition();
				CStroke * pStroke = NULL;
				if (pos != NULL)
				{
					pStroke = pSList->GetNext(pos);
				}

				int nStrkCount = pStroke->GetSize();
				if (nStrkCount <= 0)
				{
					return;
				}
				dot_T = pStroke->GetAt(nStrkCount - 1);
			}
		}// end for

		// 如果尾头相连的距离小于尾尾相连的距离
		if (dbMinDsH <= dbMinDsT)
		{
			dotT = dot_H;
			StrokeList *pRList = FillList.GetAt(nPosH);
			RetList.Add(pRList);
			FillList.RemoveAt(nPosH);
		}
		else
		{
			dotT = dot_T;
			StrokeList *pRList = FillList.GetAt(nPosT);
			ReverStrkList(pRList);
			RetList.Add(pRList);
			FillList.RemoveAt(nPosT);
		}
///////////////--------------------------

		RCount = RetList.GetSize();
	}// end while
	
	
	return;
}
*/


void CFill::ListSortProc(CArray<StrokeList*, StrokeList*> &FillList
					   , CArray<StrokeList*, StrokeList*> &RetList)
{
	int nCount = FillList.GetSize();
	if (nCount <= 0)
	{
		return;
	}

	StrokeList *pFList = FillList.GetAt(0);
	RetList.Add(pFList);
	FillList.RemoveAt(0);

	CDot dotT;
	POSITION pos = pFList->GetTailPosition();
	CStroke * pStroke = NULL;
	if (pos != NULL)
	{
		pStroke = pFList->GetNext(pos);
	}

	int nStrkCount = pStroke->GetSize();
	if (nStrkCount <= 0)
	{
		return;
	}

	// 第一个StrokeList的最后一个点
	dotT = pStroke->GetAt(nStrkCount - 1);

	int RCount = RetList.GetSize();
	while ( RCount < nCount)
	{
		CDot dot_H;
		int nPosH = -1;
		double dbMinDsH = 99999999999999;
/////////////////////////////////////////////
		// 尾与头连接
		for (int i = 0; i < FillList.GetSize(); i++)
		{
			StrokeList *pSList = FillList.GetAt(i);
			if (NULL == pSList)
			{
				continue;
			}

			CDot dotH;
			POSITION pos = pSList->GetHeadPosition();
			CStroke * pStroke = NULL;
			if (pos != NULL)
			{
				pStroke = pSList->GetNext(pos);
			}

			int nStrkCount = pStroke->GetSize();
			if (nStrkCount <= 0)
			{
				return;
			}

			CDot dot = pStroke->GetAt(0);
			double dbD = Distance(dotT, dot);
			if (fabs(dbD) <= dbMinDsH)
			{
				dbMinDsH   = fabs(dbD);
				nPosH = i;
			}
		}// end for
///////////////////////////////////////////////////////////

///////////////--------------------------
		CDot dot_T;
		int nPosT = -1;
		double dbMinDsT = 99999999999999;
		// 尾与尾连接
		for (int j = 0; j < FillList.GetSize(); j++)
		{
			StrokeList *pSList = FillList.GetAt(j);
			if (NULL == pSList)
			{
				continue;
			}

			POSITION pos = pSList->GetTailPosition();
			CStroke * pStroke = NULL;
			if (pos != NULL)
			{
				pStroke = pSList->GetNext(pos);
			}

			int nStrkCount = pStroke->GetSize();
			if (nStrkCount <= 0)
			{
				return;
			}

			CDot dot = pStroke->GetAt(nStrkCount-1);
			double dbD = Distance(dotT, dot);
			if (fabs(dbD) <= dbMinDsT)
			{
				dbMinDsT   = fabs(dbD);
				nPosT = j;
			}
		}// end for

		StrokeList *pRList = NULL;
		// 如果尾头相连的距离小于尾尾相连的距离
		if (dbMinDsH <= dbMinDsT)
		{
			pRList = FillList.GetAt(nPosH);
			RetList.Add(pRList);
			FillList.RemoveAt(nPosH);
		}
		else
		{
			pRList = FillList.GetAt(nPosT);
			ReverStrkList(pRList);
			RetList.Add(pRList);
			FillList.RemoveAt(nPosT);
		}
		if(pRList==NULL)
		{
			RCount = RetList.GetSize()+1;
			continue;
		}

		POSITION pos = pRList->GetTailPosition();
		CStroke * pStroke = NULL;
		if (pos != NULL)
		{
			pStroke = pRList->GetNext(pos);
		}

		int nStrkCount = pStroke->GetSize();
		if (nStrkCount <= 0)
		{
			return;
		}
		dotT = pStroke->GetAt(nStrkCount-1);
///////////////--------------------------

		RCount = RetList.GetSize();
	}// end while
	
	
	return;
}

void CFill::ReverStrkList(StrokeList *pStrkList)
{
	if (NULL == pStrkList)
	{
		return;
	}

	int nCount = pStrkList->GetCount();
	if (nCount <= 0 )
	{
		return;
	}

	DWORD *pStrkArray = new DWORD[nCount];
	if (NULL == pStrkArray)
	{
		return;
	}
	memset(pStrkArray, 0, sizeof(DWORD) * nCount);

	POSITION pos = pStrkList->GetHeadPosition();
	int i = 0;
	while (pos != NULL)
	{
		CStroke * pStroke = pStrkList->GetNext(pos);
		pStrkArray[i++] = (DWORD)(pStroke);
	}

	pStrkList->RemoveAll();
	for (int j = (nCount - 1); j >= 0; j--)
	{
		pStrkList->AddTail((CStroke*)(pStrkArray[j]));
	}

	delete []pStrkArray;
	pStrkArray = NULL;

	return;
}

BOOL CFill::StrokeProc(CStroke *pStrk, CArray<StrokeList*, StrokeList*> &RetList)
{
	if (NULL == pStrk)
	{
		return FALSE;
	}
	
	int nDtCount = pStrk->GetSize();
	if (nDtCount <= 0)
	{
		return FALSE;
	}
	CDot dotH = pStrk->GetAt(0);

	int nInserPos = -1;
	double dbMinLen = m_dbCircle;

	int nLisCount = RetList.GetSize();
	for (int i = 0; i < nLisCount; i++)
	{
		StrokeList *pStrkList = RetList.GetAt(i);
		int nStrkCt = pStrkList->GetCount();
		if (nStrkCt <= 0)
		{
			continue;
		}

		POSITION pos = pStrkList->GetTailPosition();
		CStroke * pStroke = NULL;
		while (pos != NULL)
		{
			pStroke = pStrkList->GetNext(pos);
		}

		int nDtCt = pStroke->GetSize();
		if (nDtCt <= 0)
		{
			continue;
		}
		
		CDot dotT = pStroke->GetAt(nDtCt - 1);
		if ((fabs(dotT.y - dotH.y)) < EPSINON)
		{
			continue;
		}
		double dbD = Distance(dotT, dotH);
		if (fabs(dbD) <= dbMinLen)
		{
			dbMinLen  = fabs(dbD);
			nInserPos = i;
		}
	}

	if (nInserPos != -1)
	{
		StrokeList *pStrokeList = RetList.GetAt(nInserPos);
		pStrokeList->AddTail(pStrk);
	}
	else
	{
		StrokeList *pStrokeList = new StrokeList;
		if (NULL == pStrokeList)
		{
			return FALSE;
		}

		pStrokeList->AddTail(pStrk);

		RetList.Add(pStrokeList);
	}

	return TRUE;
}

BOOL CFill::ZoomStroke(CStroke* pStroke)
{
	int nDtCt = pStroke->GetSize();
	if (nDtCt < 2)
	{
		return FALSE;
	}
	
	CDot dotH = pStroke->GetAt(0);
	CDot dot2 = pStroke->GetAt(1);
	double dbSourLineLengh = Distance(dotH, dot2);
	pStroke->m_dotArray[0].x = (m_dbHeadKillLen/dbSourLineLengh) * ((dotH.x-dot2.x)) + dotH.x;
	pStroke->m_dotArray[0].y = (m_dbHeadKillLen/dbSourLineLengh) * ((dotH.y-dot2.y)) + dotH.y;

	CDot dotT = pStroke->GetAt(nDtCt - 1);
	dot2 = pStroke->GetAt(nDtCt - 2);
	dbSourLineLengh = Distance(dotT, dot2);
	pStroke->m_dotArray[nDtCt - 1].x = (m_dbTailKillLen/dbSourLineLengh) * ((dotT.x-dot2.x)) + dotT.x;
	pStroke->m_dotArray[nDtCt - 1].y = (m_dbTailKillLen/dbSourLineLengh) * ((dotT.y-dot2.y)) + dotT.y;

	return TRUE;
}
