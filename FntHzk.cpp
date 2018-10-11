// FntHzk.cpp: implementation of the CFntHzk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "FntHzk.h"

#include<stdlib.h>
#include<io.h>
#include<fcntl.h>
#include <conio.h>
#include <ctype.h>

#include "Dot.h"
#include "Stroke.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define HZK_PES		10		//以pes个逻辑单位为基准画文本
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CFntHzk,CObject,2000)

CFntHzk::CFntHzk()
{
//	m_pUserFile = new CUserFontFile;
//
//	m_pUserFile->OpenFile(1);
//	m_pUserFile->LoadFile();
//	m_pUserFile->CloseFile();
}

CFntHzk::~CFntHzk()
{
/*
	if(m_pUserFile != NULL)
		delete m_pUserFile;
	m_pUserFile = NULL;
	POSITION prev;
	POSITION pos=m_InitDotList.GetHeadPosition();
	CStroke *pStroke;
	while(pos!=NULL)
	{	
		prev=pos;
		pStroke=m_InitDotList.GetNext(pos);

		m_InitDotList.RemoveAt(prev);
		delete pStroke;
	}
//*/
}

//
//Open the Hzk 12 or 14 or 16
//
BOOL CFntHzk::OpenHzk(int type, int AscType)
{
	CHLApp *pApp = (CHLApp *)AfxGetApp();
	CString path = pApp->GetInstallPath();

	switch (type)
	{
	case FONT_HZK_12:
		m_Handle = _open(path+"\\DFonts\\hzk12", O_RDONLY|O_BINARY);
		break;

	case FONT_HZK_14:
		m_Handle = _open(path+"\\DFonts\\hzk14", O_RDONLY|O_BINARY);
		break;

	case FONT_HZK_16:
		m_Handle = _open(path+"\\DFonts\\hzk16", O_RDONLY|O_BINARY);
		break;
	}

	if(m_Handle == -1)
		return FALSE;

	if(AscType == FONT_ASC_8_16)
	{
		m_HandleASC = _open(path+"\\DFonts\\ASC16", O_RDONLY|O_BINARY);
		if(m_HandleASC == -1)
			return FALSE;
	}

	return TRUE;
}

//
//读取点阵字库，获取文本点阵信息
//
int CFntHzk::PutHz(int x, int y, int z, char *p, int type, int AscType)
{
	unsigned int i,c1,c2,f=0;
	int i1,i2,i3,i4,rec;
	long l;
	char by[32];

	//字间距
	int cs = (int)(HZK_PES*m_dbCharSpace/m_dbWidthFactor/m_dbHeight);

	while((i = *p++)!=0)
	{
		if(i > 0xa1)		//中文
		{
			if(f == 0)
			{
				c1 = (i-0xa1)&0x7f;
				f = 1;
			}
			else
			{
				c2 = (i-0xa1)&0x7f;
				f = 0;
				rec = c1*94+c2;
				l = rec*type*2L;
				_lseek(m_Handle, l, SEEK_SET);
				_read(m_Handle, by, 32);

				////////////////////////////////////////
				//垂直读取
				for(i2=0; i2<2; i2++)	//每一行所有的列分为两组, 
				for(i3=0; i3<4; i3++)	//每一字节8列分为4个来回
				for(i4=0; i4<2; i4++)	//一个来回完成两列
				{
					for(i1=0; i1<type; i1++)	//每列有type行
					{
						if(GetBit(by[i1*2+i2], 7-(i3*2+i4)))
						{
							SaveInitDotList(x+i2*8+(i3*2+i4), y+i1);
							//AddtoDotList(x+i2*8+(i3*2+i4), y+i1);
						}
					}
				}
				x=x+type+z+cs;

				///////////////////////////////////////
/*				//按字库顺序读取
				for(i1=0; i1<type; i1++)
				for(i2=0; i2<2; i2++)
				for(i3=0; i3<8; i3++)
				{
					if(GetBit(by[i1*2+i2], 7-i3))
						AddtoDotList(x+i2*8+i3, y+i1);
				}
				x=x+type+z+cs;
*/				
			}
		}
		else if(i == 0x0a || i == 0x0d)	//回车换行的处理
		{
			x = 0;
			y = (int)(m_dbLineSpace+m_DotBottom);
		}
		else	//英文和数字
		{
			if(AscType == FONT_ASC_5_7)
			{
//				//5*7点阵
//				USERFONTFILE * pFont = GetDataOfChar(i);
//				if(pFont == NULL)
//				{
//					x=x+7+z+cs;
//					continue;
//				}
//
//				//垂直读取
//				for(int i=0; i<5; i++)
//				{
//					int j=0;
//					StrokeList	m_tempList;
//					//垂直向上
//					for(j=6; j>=0; j--)
//					{
//						for(i3=0; i3<pFont->nPointNumber; i3++)
//						{
//							if(pFont->point[i3].x == i && pFont->point[i3].y ==j)
//							{
//								SaveInitDotList(x+pFont->point[i3].x, y+7-pFont->point[i3].y);
//							}
//						}
//					}
//
//				}
//				x=x+7+z+cs;
				//笔画顺序读取
				/*for(i3=0; i3<pFont->nPointNumber; i3++)
				{
					AddtoDotList(x+pFont->point[i3].x, y+pFont->point[i3].y);
				}
				x=x+7+z+cs;*/
			}
			else if(AscType == FONT_ASC_8_16)
			{
				//8*16点阵

				//垂直读取
				_lseek(m_HandleASC, i*16, SEEK_SET);
				_read(m_HandleASC, by, 16); // 读ASCII码点阵 
				for(i3=0; i3<4; i3++)
				for(i4=0; i4<2; i4++)
				{
					for(i1=0; i1<16; i1++)
					{
						if(GetBit(by[i1], 7-(i3*2+i4)))
						{
							SaveInitDotList(x+(i3*2+i4), y+i1);
							//AddtoDotList(x+(i3*2+i4), y+i1);
						}
					}
				}
				x=x+8+z+cs;

				/*
				//按字库顺序读取
				lseek(m_HandleASC, i*16, SEEK_SET);
				read(m_HandleASC, by, 16); // 读ASCII码点阵 
				for(i1=0; i1<16; i1++)
				for(i3=0; i3<8; i3++)
				{
					if(GetBit(by[i1], 7-i3))
						AddtoDotList(x+i3, y+i1);
				}
				x=x+8+z+cs;*/
			}
		}
	}
	//整体垂直回转排序
	SortDotList_V();

	return(x);
}

int CFntHzk::GetBit(unsigned char c, int n)
{
	return((c>>n)&1);
}


BOOL CFntHzk::MakeStroke()
{
	m_InitDotList.RemoveAll();

	if(!OpenHzk(m_HzkType, m_AscType))
	{
		CString csMsg;
		csMsg.LoadString(IDS_HZK_OPENERROR);
		AfxMessageBox(csMsg);
		return FALSE;
	}

	m_bSaveFirst = TRUE;
	m_curWorkDir = 0;
	m_curFlyDir = 0;
	PutHz(0, 0, 0, (LPTSTR)(LPCTSTR)m_strText, m_HzkType, m_AscType);

	_getch();
	_close(m_Handle);
	_close(m_HandleASC);

	return TRUE;
}

void CFntHzk::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		//ar<<m_lf;
	}
	else
	{
		// TODO: add loading code here
		//ar>>m_lf;
	}
}

//
//原始字库坐标信息的备份
void CFntHzk::SaveInitDotList(double x, double y)
{
	CStroke *pStroke = NULL;
	pStroke = new CStroke(m_property); //????

	pStroke->Add(CDot(x,y));
	m_InitDotList.AddTail(pStroke);

	if(m_bSaveFirst)
	{
		m_bSaveFirst = FALSE;
		m_DotTop = m_DotBottom = (int)y;
		m_DotRight = m_DotLeft = (int)x;
	}
	else
	{
		if(x < m_DotLeft)	m_DotLeft = (int)x;
		if(x > m_DotRight)	m_DotRight = (int)x;
		if(y < m_DotTop)	m_DotTop = (int)y;
		if(y > m_DotBottom)	m_DotBottom = (int)y;
	}
}

void CFntHzk::SortDotList_V()
{
	StrokeList	pNewList;
	POSITION pos;
	CStroke* pStroke;
	CDot dot;
	BOOL bFind, bOrder = TRUE;

	//整体回转排序
	for(int i=m_DotLeft; i<=m_DotRight; i++)
	{
		bFind = FALSE;
		if(bOrder)	//从上到下,正序排列
		{
			pos = m_InitDotList.GetHeadPosition();
			while(pos != NULL)
			{	
				pStroke = m_InitDotList.GetNext(pos);
				dot = pStroke->GetAt(0);
				if(dot.x == i)
				{
					bFind = TRUE;
					pNewList.AddTail(pStroke);
				}
			}
		}
		else
		{
			pos = m_InitDotList.GetTailPosition();
			while(pos != NULL)
			{	
				pStroke = m_InitDotList.GetPrev(pos);
				dot = pStroke->GetAt(0);
				if(dot.x == i)
				{
					bFind = TRUE;
					pNewList.AddTail(pStroke);
				}
			}
		}
		
		if(bFind)
			bOrder = !bOrder;
	}

	//加到m_list中
	pos = pNewList.GetHeadPosition();
	while(pos != NULL)
	{	
		pStroke = pNewList.GetNext(pos);
		dot = pStroke->GetAt(0);
		AddtoDotList(dot.x, dot.y);
	}

	//
	POSITION prev;
	pos=m_InitDotList.GetHeadPosition();
	while(pos!=NULL)
	{	
		prev=pos;
		pStroke=m_InitDotList.GetNext(pos);
		m_InitDotList.RemoveAt(prev);
		delete pStroke;
	}
}

void CFntHzk::AddtoDotList(double x, double y, BOOL bRollBack)
{
	CStroke *pStroke = NULL;
	pStroke = new CStroke(m_property);
	

	pStroke->Add(CDot(x, y));
	m_list.AddTail(pStroke);


	//座标反转
	//还原为设定高度,宽度系数
	double ratioY, ratioX;
	
	ratioY=-m_dbHeight/HZK_PES;
	ratioX=-ratioY*m_dbWidthFactor;
	pStroke->Scale(CDot(0, 0), ratioX, ratioY);
}

//
//改变点阵列表的工作台方向
//nDir == 0: Y->X(即将水平排列的点改为垂直排列)
//nDir == 1: X->Y(即将垂直排列的点改为水平排列)
void CFntHzk::ChangeWorkDirect(int nDir)
{
	if(m_curWorkDir == nDir)
		return;

	if(nDir == 0)
	{
		//Copy m_InitDotList to m_list;
		m_list.RemoveAll();

		CStroke *pStroke = NULL;
		CDot dot;

		POSITION pos = m_InitDotList.GetHeadPosition();
		while(pos!=NULL)
		{	
			pStroke = m_InitDotList.GetNext(pos);
			dot = pStroke->GetAt(0);
			AddtoDotList(dot.x, dot.y);
		}
	}
	else
	{
		//Change the Direct
		m_list.RemoveAll();

		StrokeList	pStrokeList;
		CStroke *pStroke = NULL, *pTmp = NULL;
		POSITION pos, pos1;
		int i;
		CDot	pDot;
		BOOL	bOrder = TRUE, bFind;

		for(i=m_DotTop; i<=m_DotBottom; i++)
		{
			bFind = FALSE;
			pos = m_InitDotList.GetHeadPosition();
			pos1 = pStrokeList.GetTailPosition();
			while(pos!=NULL)
			{	
				pStroke = m_InitDotList.GetNext(pos);
				pDot = pStroke->GetAt(0);

				if(pDot.y == i)
				{
					bFind = TRUE;
					pTmp = new CStroke(m_property);
					pTmp->Add(pDot);

					if(bOrder)
						pStrokeList.AddTail(pTmp);
					else
						pStrokeList.InsertAfter(pos1, pTmp);

				}
			}
			if(bFind)
				bOrder = !bOrder;
		}

		pos = pStrokeList.GetHeadPosition();
		while(pos!=NULL)
		{	
			pStroke = pStrokeList.GetNext(pos);
			pDot = pStroke->GetAt(0);
			AddtoDotList(pDot.x, pDot.y);
		}
	}

	m_curWorkDir = nDir;
}

//
//改变点阵列表的飞行运动方向
//nDir == 0: 从左－>右,点阵正向排列
//nDir == 1: 从右->左,点阵逆向排列
void CFntHzk::ChangeFlyDirect(int nDir)
{
	if(m_curFlyDir == nDir)
		return;

	CStroke *pStroke = NULL, *pTmp = NULL;
	StrokeList	pStrokeList;
	CDot	pDot;

	POSITION pos = m_list.GetHeadPosition();
	while(pos != NULL)
	{
		pStroke = m_list.GetNext(pos);
		pDot = pStroke->GetAt(0);
		pTmp = new CStroke(m_property);
		pTmp->Add(pDot);
		pStrokeList.AddHead(pTmp);
	}

	m_list.RemoveAll();

	pos = pStrokeList.GetHeadPosition();
	while(pos != NULL)
	{
		pStroke = pStrokeList.GetNext(pos);
		pDot = pStroke->GetAt(0);
		pTmp = new CStroke(m_property);
		pTmp->Add(pDot);
		m_list.AddTail(pTmp);
	}

	m_curFlyDir = nDir;
}
