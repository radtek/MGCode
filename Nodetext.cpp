// NodeText.cpp: implementation of the CNodeText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeText.h"
#include "Text.h"
#include "HLView.h"
#include "FntShx.h"
#include "FntTTF.h"
#include "TextBaseDlg.h"
#include "Mystruct.h"
#include "NodeLine.h"
#include "FntHzk.h"
#include "SetBarTextDlg.h"

#include"cpp11_std_files.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodeText::CNodeText()
{
	m_type=NODE_TEXT_DSF;
	//初始化值
	m_dotBase=CDot(0,0);
	m_nPath=PATH_LINE;
	m_dbDirection=0;
	m_dbRadius=20;
	m_bCCW=FALSE;
	m_bInside=FALSE;
	m_dbStart=0;
	
	m_nFont=FONT_TTF;
	m_HzkType = FONT_HZK_12;
	m_AscType = FONT_ASC_5_7;

	m_dbCharSpace=0;
	m_dbWidthFactor=1;
	m_dbHeight=5;
	m_dbLineSpace=0;

	m_pFnt=NULL;

	m_nAlign = ALIGN_LEFT;

	m_ArrayTextID.RemoveAll();
	m_strText = "";

	m_pFnt=NULL;
	m_strFontCHS="简体宋体";
	m_strFontEN="romans";
	m_strFontNUM="romans";

	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight=-24;
	strcpy(m_lf.lfFaceName,"Arial");

	m_bCustomFormat = FALSE;
	m_strFormat  = "XXX-XXX";
	m_strPrecisK = "500";
	m_bFixSpace = FALSE;
}

CNodeText::~CNodeText()
{
	if(m_pFnt)
	{
		delete m_pFnt;
	}

	return;
}

BOOL CNodeText::Track(CWnd* pWnd, CPoint ptOrg)
{
	return Create(CRect(0,0,0,0));
}

void CNodeText::Draw(XDC *pDC)
{
	//此处对数据层做查询判断是否需要重新生成stroke
	bool needUpdate = false;
	needUpdate = CheckString();
	if (needUpdate) {
		CreateString(true);
	}

	CStroke::m_bRedColorDraw = m_bSelect;
	if(m_pFnt!=NULL)
		m_pFnt->Draw(pDC, m_bIsCurSortTree);

	return;
}

CQuad CNodeText::CalLimit()
{
	m_quadLimit.SetEmpty();
	if(m_pFnt)
	{
		m_quadLimit.Union(m_pFnt->CalLimit());
	}
	return m_quadLimit;
}

CQuad CNodeText::GetMarkRange()
{
	CQuad quad;
	quad.SetEmpty();
	if(m_pFnt)
	{
		quad.Union(m_pFnt->GetMarkRange());
	}
	return quad;
}
void CNodeText::SetProperty(int mode,CProperty pty)
{
	m_property.Set(mode,pty);
}

BOOL CNodeText::Click(CPoint &point,CWnd *pWnd)
{
	return m_quadLimit.Include(point,pWnd);
}

BOOL CNodeText::CanGroup()
{
	int nCount = m_ArrayTextID.GetSize();
	UINT nTextID;
	CText* pText;
	BOOL bBarText=FALSE;
	
	for(int i=0;i<nCount;i++)
	{
		nTextID = m_ArrayTextID.GetAt(i);
		pText = g_MgrText->GetAt(nTextID);
		if(pText->m_bBarCode){
			//是条码所属文本
			bBarText=TRUE;
		}
	}
	return !bBarText;
}

CNode * CNodeText::Copy(UINT nStyle)
{	
 	CNodeText *tmp=new CNodeText();
	tmp->m_property=m_property;
	tmp->m_bSelect = m_bSelect;
	tmp->m_nXPos		= m_nXPos;
	tmp->m_nAlign		= m_nAlign;

	tmp->m_dotBase		=m_dotBase;
	tmp->m_nPath		=m_nPath;
	tmp->m_dbDirection	=m_dbDirection;
	tmp->m_dbRadius		=m_dbRadius;
	tmp->m_bCCW			=m_bCCW;
	tmp->m_bInside		=m_bInside;
	tmp->m_dbStart		=m_dbStart;
	
	tmp->m_nFont		=m_nFont;
	tmp->m_strFontCHS	=m_strFontCHS;
	tmp->m_strFontEN	=m_strFontEN;
	tmp->m_strFontNUM	=m_strFontNUM;
	tmp->m_lf			=m_lf;

	tmp->m_dbCharSpace	=m_dbCharSpace;
	tmp->m_dbWidthFactor=m_dbWidthFactor;
	tmp->m_dbHeight		=m_dbHeight;
	tmp->m_dbLineSpace	=m_dbLineSpace;

	tmp->m_HzkType=m_HzkType;
	tmp->m_AscType=m_AscType;

	tmp->m_bFixSpace = m_bFixSpace;

	//////////////////陈列时自动增加
	if(0x00 == nStyle)
	{
		CText* pText;
		int nIndex;
		int nCount = m_ArrayTextID.GetSize();
		for(int i=0;i<nCount;i++)
		{
			UINT nID = g_MgrText->GetNextID();
			pText = new CText(0);
			nIndex = m_ArrayTextID.GetAt(i);
			*pText = *(g_MgrText->GetAt(nIndex));
			pText->m_nTextID = nID;
			if(g_dwJumpCnt != -1)
			{
				pText->m_nJumpStart += g_dwJumpCnt;
			}
			g_MgrText->AddText(pText);
			tmp->m_ArrayTextID.Add(nID);
		}
	}
	else
	{
		int nIndex;
		int nCount = m_ArrayTextID.GetSize();
		for(int i=0;i<nCount;i++)
		{
			nIndex = m_ArrayTextID.GetAt(i);
			tmp->m_ArrayTextID.Add(nIndex);
		}
	}
	tmp->m_strText=m_strText;
	tmp->m_bCustomFormat = m_bCustomFormat;
	tmp->m_strFormat  = m_strFormat;
    tmp->m_strPrecisK = m_strPrecisK;
	tmp->CreateString(FALSE);
	
 	return tmp;
}

void CNodeText::CopyToMe(CNodeText *pSrc)
{
	//COPY相关属性，不COPY文本ID
	m_property		= pSrc->m_property;
	m_bSelect		= pSrc->m_bSelect;
	m_nXPos			= pSrc->m_nXPos;
	m_nAlign		= pSrc->m_nAlign;
	m_dotBase		= pSrc->m_dotBase;
	m_nPath			= pSrc->m_nPath;
	m_dbDirection	= pSrc->m_dbDirection;
	m_dbRadius		= pSrc->m_dbRadius;
	m_bCCW			= pSrc->m_bCCW;
	m_bInside		= pSrc->m_bInside;
	m_dbStart		= pSrc->m_dbStart;
	m_nFont			= pSrc->m_nFont;
	m_strFontCHS	= pSrc->m_strFontCHS	;
	m_strFontEN		= pSrc->m_strFontEN	;
	m_strFontNUM	= pSrc->m_strFontNUM	;
	m_lf			= pSrc->m_lf			;
	m_dbCharSpace	= pSrc->m_dbCharSpace;

	m_dbWidthFactor		= pSrc->m_dbWidthFactor	;
	m_dbHeight			= pSrc->m_dbHeight		;
	m_dbLineSpace		= pSrc->m_dbLineSpace		;
	m_HzkType			= pSrc->m_HzkType			;
	m_AscType			= pSrc->m_AscType			;

	m_bCustomFormat = pSrc->m_bCustomFormat;
	m_strFormat  = pSrc->m_strFormat;
    m_strPrecisK = pSrc->m_strPrecisK;
	m_bFixSpace  = pSrc->m_bFixSpace;

	return;
}

void CNodeText::Rotate(CDot dot,double angle)
{

	m_pFnt->Rotate(dot, angle);
	m_pFnt->CreateStrokeList();
	CalLimit();

	m_dotBase=m_pFnt->m_dotBase;
	m_dbDirection=m_pFnt->m_dbDirection;
	m_dbStart=m_pFnt->m_dbStart;
	
	return;
}

BOOL CNodeText::Move(double moveX, double moveY)
{
	m_pFnt->Move(moveX,moveY);
	m_pFnt->CreateStrokeList();
	CalLimit();
	m_dotBase	=m_pFnt->m_dotBase;

	return TRUE;
}

void CNodeText::Scale(CDot dot, double ratioX, double ratioY)
{	
	int n=(int)(m_dbDirection/90);
	int ang =(int ) m_dbDirection;
	if((m_dbDirection!=n*90 && (ratioX==1 && ratioY==1))||(ang%180==0))
	{

		m_pFnt->m_dbCharSpace = m_pFnt->m_dbCharSpace*ratioX;	//LZQ,20131220	

		dot.LogToTrue();
		m_pFnt->Scale(dot,ratioX,ratioY);
		
		m_pFnt->CreateStrokeList();
		CalLimit();
		
		//刷新字形值
		m_dbCharSpace	=m_pFnt->m_dbCharSpace;
		m_dbHeight		=m_pFnt->m_dbHeight;
		m_dbLineSpace	=m_pFnt->m_dbLineSpace;
		m_dbWidthFactor	=m_pFnt->m_dbWidthFactor;
		m_dotBase		=m_pFnt->m_dotBase;
	}

	return;
}

void CNodeText::Drag(CDot dot, double ratioX, double ratioY,double moveX,double moveY)
{	
	if (fabs(ratioX) < EPSINON) ratioX = 0.000000001;
	if (fabs(ratioY) < EPSINON) ratioY = 0.000000001;

	if(m_nPath)
	{
		m_pFnt->Scale(dot,ratioX,ratioY);
		m_pFnt->Move(moveX,moveY);
		
		m_pFnt->m_dbStart = m_dbStart;
		m_pFnt->CreateStrokeList();
		CalLimit();
		
	}
	else
	{
		BOOL flag=FALSE;
		int n=(int)(m_dbDirection/90);
		int ang =(int ) m_dbDirection;
		if(ang%180==0&&(ratioX!=1 || ratioY!=1))
		{
			flag=TRUE;
		}
		else if(ratioX==1 && ratioY==1)
		{
			Move(moveX,moveY)	;
			return;
		}
		if (!flag)
		{
			return;
		}


		m_pFnt->Scale(dot,ratioX,ratioY);
		m_pFnt->Move(moveX,moveY);

		m_pFnt->CreateStrokeList();
		CalLimit();
	}

	//刷新字形值
	m_dbCharSpace	=m_pFnt->m_dbCharSpace;
	m_dbHeight		=m_pFnt->m_dbHeight;
	m_dbLineSpace	=m_pFnt->m_dbLineSpace;
	m_dbWidthFactor	=m_pFnt->m_dbWidthFactor;
	m_dotBase		=m_pFnt->m_dotBase;
	m_dbRadius      =m_pFnt->m_dbRadius;


	return;
}

void CNodeText::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;
	
	if (bFlag)
	{
		CalLimit();
		CDot dot1,dot2;

		//写入头信息
		fs<<"NTXT_B";
		fs<<100;
		str.Empty();
		CString strTmp;
		int nCount = m_ArrayTextID.GetSize();
		for(int i=0;i<nCount;i++)
		{
			strTmp.Format("%d|",m_ArrayTextID.GetAt(i));
			str+=strTmp;
		}
		fs<<str;		//文本属性ID
		fs<<118;
		fs<<m_nPath;			//路径方式
		fs<<119;
		fs<<m_dbDirection;		//直线方式下的方向
		fs<<120;
		fs<<m_dbRadius;			//半径
		fs<<121;
		fs<<m_bCCW;				//正逆时针方向
		fs<<122;
		fs<<m_dbStart;			//开始角度
		fs<<123;
		fs<<m_bInside;			//文字内侧显示
		fs<<124;
		fs<<m_nFont;			//文字类型shx或ttf
		fs<<125;
		fs<<m_dbCharSpace;		//字间距
		fs<<126;
		fs<<m_dbWidthFactor;	//宽度系数
		fs<<127;
		fs<<m_dbHeight;			//字高
		fs<<128;
		fs<<m_dbLineSpace;		//行间距
		fs<<129;
		fs<<m_strFontCHS;		//SHX字体的中文字体名
		fs<<130;
		fs<<m_strFontEN;
		fs<<131;
		fs<<m_strFontNUM;
		dot1 = m_quadLimit.GetBaseDot(BASE_TL);
		dot1.TrueToLog();
		fs<<132;
		fs<<dot1.x;				//左上点坐标
		fs<<133;
		fs<<dot1.y;
		dot1.LogToTrue();
		dot2 = m_quadLimit.GetBaseDot(BASE_BR);
		dot2.TrueToLog();
		fs<<134;
		fs<<dot2.x;				//右下点坐标
		fs<<135;
		fs<<dot2.y;
		fs<<136;
		dot2.LogToTrue();
		fs<<m_nAlign;			//对齐方式;
		fs<<137;
		//写入文本信息到一行文本中
		CString strFontInfo;
		strFontInfo.Format("%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s",
			m_lf.lfHeight,
			m_lf.lfWeight,
			m_lf.lfEscapement,
			m_lf.lfOrientation,
			m_lf.lfWeight,
			m_lf.lfItalic,
			m_lf.lfUnderline,
			m_lf.lfStrikeOut,
			m_lf.lfCharSet,
			m_lf.lfOutPrecision,
			m_lf.lfClipPrecision,
			m_lf.lfQuality,
			m_lf.lfPitchAndFamily,
			m_lf.lfFaceName);
		fs<<strFontInfo;
		m_dotBase.TrueToLog();
		fs<<140;
		fs<<m_dotBase.x;
		fs<<141;
		fs<<m_dotBase.y;
		m_dotBase.LogToTrue();
		fs<<142;
		fs<<m_bFixSpace;
		fs<<143;
		fs<<m_bCustomFormat;
		fs<<144;
		fs<<m_strFormat;
		fs<<145;
		fs<<m_strPrecisK;

		m_property.Serialize(fp,bFlag);

		fs<<"NTXT_E";
	}
	else
	{
		CString strRead;
		while(fs>>str)
		{
			switch(atoi(str))
			{
			case 0:
				if (str=="PRO_B")
				{
					m_property.Serialize(fp,bFlag);
					continue;
				}
				else if (str=="NTXT_E")
				{
					goto NODETEXT_SERI_END;
				}
				break;
			case 100:
				{
					m_ArrayTextID.RemoveAll();
					fs>>strRead;
					int nCount = strRead.Find("|");
					int tmp;
					while(nCount!=-1)
					{
						tmp = atoi(strRead);
						m_ArrayTextID.Add(tmp);
						strRead = strRead.Mid(nCount+1);
						nCount = strRead.Find("|");
					}
					if(g_MgrText->GetCopySts())
					{
						//如果是COPY状态时则修改ID
						UINT nID = g_MgrText->GetBaseID();
						nCount = m_ArrayTextID.GetSize();
						UINT nTxt;
						for(int i=0;i<nCount;i++)
						{
							nTxt = m_ArrayTextID.GetAt(i);
							nTxt = OFFSET_TEXTID(nID,nTxt);
							m_ArrayTextID.SetAt(i,nTxt);
						}
					}
					break;
				}
			case 118:
				fs>>m_nPath;
				break;
			case 119:
				fs>>m_dbDirection;		//直线方式下的方向
				break;
			case 120:
				fs>>m_dbRadius;			//半径
				break;
			case 121:
				fs>>m_bCCW;				//正逆时针方向
				break;
			case 122:
				fs>>m_dbStart;			//开始角度
				break;
			case 123:
				fs>>m_bInside;			//文字内侧显示
				break;
			case 124:
				fs>>m_nFont;			//文字类型shx或ttf
				break;
			case 125:
				fs>>m_dbCharSpace;		//字间距
				break;
			case 126:
				fs>>m_dbWidthFactor;	//宽度系数
				break;
			case 127:
				fs>>m_dbHeight;			//字高
				break;
			case 128:
				fs>>m_dbLineSpace;		//行间距
				break;
			case 129:
				fs>>m_strFontCHS;		//SHX字体的中文字体名
				break;
			case 130:
				fs>>m_strFontEN;
				break;
			case 131:
				fs>>m_strFontNUM;
				break;
			case 132:
				fs>>m_dotLT.x;
				break;
			case 133:
				fs>>m_dotLT.y;
				break;
			case 134:
				fs>>m_dotRB.x;
				break;
			case 135:
				fs>>m_dotRB.y;
				break;
			case 136:
				fs>>m_nAlign;
				break;
			case 137:
				{
					fs>>strRead;
					int nCount;
					m_lf.lfHeight=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);

					m_lf.lfWeight=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					
					m_lf.lfEscapement=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);

					m_lf.lfOrientation=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					m_lf.lfWeight=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					m_lf.lfItalic=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					m_lf.lfUnderline=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					m_lf.lfStrikeOut=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					m_lf.lfCharSet=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					m_lf.lfOutPrecision=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					m_lf.lfClipPrecision=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					m_lf.lfQuality=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					m_lf.lfPitchAndFamily=atoi(strRead);
					nCount = strRead.Find("|");
					strRead=strRead.Mid(nCount+1);
					strRead.TrimRight("\r\n");
					strcpy(m_lf.lfFaceName,strRead.GetBuffer(0));
					strRead.ReleaseBuffer();
					break;
				}
			case 140:
				fs>>m_dotBase.x;
				break;
			case 141:
				fs>>m_dotBase.y;
				break;
			case 142:
				fs>>m_bFixSpace;
				break;
			case 143:
				fs>>m_bCustomFormat;
				break;
			case 144:
				fs>>m_strFormat;
				break;
            case 145:
				fs>>m_strPrecisK;
				break;
			}
		}
NODETEXT_SERI_END:
		m_dotRB.LogToTrue();
		m_dotBase.LogToTrue();
		m_dotLT.LogToTrue();
		CreateString(FALSE);
		CalLimit();
		double moveX,moveY;
		switch(m_nAlign)
		{
		case ALIGN_LEFT:
			moveX=m_dotLT.x-m_quadLimit.left;
			break;
		case ALIGN_RIGHT:
			moveX=m_dotRB.x-m_quadLimit.right;
			break;
		case ALIGN_MID:
			moveX=(m_dotLT.x+m_dotRB.x)/2-(m_quadLimit.left+m_quadLimit.right)/2;
			break;
		}
		moveY = (m_dotLT.y+m_dotRB.y)/2-(m_quadLimit.top+m_quadLimit.bottom)/2;
		Move(moveX,moveY);
	}
}

void CNodeText::Scan(StrokeList *pList)
{
	if(m_pFnt)
	{
		m_pFnt->Scan(pList);
	}

	return;
}

BOOL CNodeText::IsBarText()
{
	int nCount = m_ArrayTextID.GetSize();
	UINT nTextID;
	CText* pText;
	BOOL bBarText=FALSE;
	
	for(int i=0;i<nCount;i++)
	{
		nTextID = m_ArrayTextID.GetAt(i);
		pText = g_MgrText->GetAt(nTextID);
		if(pText->m_bBarCode){
			//是条码所属文本
			bBarText=TRUE;
		}
	}

	return bBarText;
}

BOOL CNodeText::Create(CRect &rect)
{
	int nCount = m_ArrayTextID.GetSize();
	UINT nTextID;
	CText* pText;
	BOOL bBarText=FALSE;
	CString str,strText;
	
	for(int i=0;i<nCount;i++)
	{
		nTextID = m_ArrayTextID.GetAt(i);
		pText = g_MgrText->GetAt(nTextID);
		if(pText->m_bBarCode)
		{
			//是条码所属文本
			bBarText=TRUE;
		}
		g_MgrText->GetString(nTextID,str);
		strText+=str;
	}

	if(bBarText)
	{
		CSetBarTextDlg dlg;
		dlg.m_strInfo = strText;
		//字体设置部分
		dlg.m_PageFnt.m_nFont			= m_nFont				;
		dlg.m_PageFnt.m_dbCharSpace		= m_dbCharSpace		;
		dlg.m_PageFnt.m_dbWidthFactor	= m_dbWidthFactor*100 	;
		dlg.m_PageFnt.m_dbHeight		= m_dbHeight			;
		dlg.m_PageFnt.m_dbLineSpace		= m_dbLineSpace		;
		dlg.m_PageFnt.m_strFontCHS		= m_strFontCHS		;
		dlg.m_PageFnt.m_strFontEN		= m_strFontEN			;
		dlg.m_PageFnt.m_strFontNUM		= m_strFontNUM		;
		dlg.m_PageFnt.m_lf				= m_lf				;
		dlg.m_PageFnt.m_bCustomFormat	= m_bCustomFormat		;
		dlg.m_PageFnt.m_strFormat		= m_strFormat			;
        dlg.m_PageFnt.m_strPrecisK		= m_strPrecisK			;
		//排布设置部分
		dlg.m_PageRound.m_nPath			= m_nPath		;
		dlg.m_PageRound.m_dbRadius		= m_dbRadius	;
		dlg.m_PageRound.m_dbStart		= m_dbStart		;
		dlg.m_PageRound.m_bCCW			= m_bCCW		;
		dlg.m_PageRound.m_bInside		= m_bInside		;
		dlg.m_PageRound.m_nAlign		= m_nAlign		;
		dlg.m_PageRound.m_dbDirection	= m_dbDirection	;
		dlg.m_PageRound.m_bFixSpace		= m_bFixSpace;
		
		if(dlg.DoModal()==IDCANCEL)
		{
			return FALSE;
		}
		
		//刷新文本
		//字体设置部分
		m_nFont			= dlg.m_PageFnt.m_nFont;
		m_dbCharSpace	= dlg.m_PageFnt.m_dbCharSpace;
		m_dbWidthFactor = dlg.m_PageFnt.m_dbWidthFactor/100;
		m_dbHeight		= dlg.m_PageFnt.m_dbHeight;
		m_dbLineSpace	= dlg.m_PageFnt.m_dbLineSpace;
		m_strFontCHS	= dlg.m_PageFnt.m_strFontCHS;
		m_strFontEN		= dlg.m_PageFnt.m_strFontEN;	
		m_strFontNUM	= dlg.m_PageFnt.m_strFontNUM;
		m_lf			= dlg.m_PageFnt.m_lf;
		m_bCustomFormat	= dlg.m_PageFnt.m_bCustomFormat;
		m_strFormat		= dlg.m_PageFnt.m_strFormat;
        m_strPrecisK	= dlg.m_PageFnt.m_strPrecisK;
		//排布设置部分
		m_nPath			= dlg.m_PageRound.m_nPath		;
		m_dbRadius		= dlg.m_PageRound.m_dbRadius	;
		m_dbStart		= dlg.m_PageRound.m_dbStart		;
		m_bCCW			= dlg.m_PageRound.m_bCCW		;
		m_bInside		= dlg.m_PageRound.m_bInside		;
		m_nAlign		= dlg.m_PageRound.m_nAlign		;
		m_dbDirection	= dlg.m_PageRound.m_dbDirection	;
		m_bFixSpace		= dlg.m_PageRound.m_bFixSpace;
	}
	else
	{
		CTextBaseDlg dlg;
		dlg.m_bCreate=FALSE;
		dlg.m_Nodelist.AddTail(this);
		dlg.DoModal();
	}

	CreateString(FALSE);

	return TRUE;
}

void CNodeText::Mark(HWND hWnd, int nFocusType)
{
	int n  =  g_DAT.GetMarkCount(m_property.m_nLayer);
	if (ENU_MARKSPECPARASET == m_nMarkMode) 
	{
		n = 1;
	}
	
	BOOL bStop = FALSE;
	for(int i= 0;i<n;i++)
	{
		bStop = !(m_pFnt->Mark(hWnd));
		if(bStop)
			break;
	}
}


void CNodeText::SelMark(HWND hWnd, HANDLE hSelStop, int nLayer, BOOL bIsResMark)
{
	BOOL bStop = FALSE;
	bStop = m_pFnt->SelMark(hWnd,  hSelStop, nLayer, bIsResMark);

	return;
}


void CNodeText::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	m_pFnt->ExportPlt(file,bUnExportFill,bScall100);	
}

BOOL CNodeText::CreateString(BOOL bNext)
{
	//建立数据的入口
	CString str;
	m_strText.Empty();
	int nCount = m_ArrayTextID.GetSize();
	UINT nTextID;

	for(int i=0;i<nCount;i++)
	{
		nTextID = m_ArrayTextID.GetAt(i);
		g_MgrText->FlashText(nTextID,0x00);
		g_MgrText->GetString(nTextID,str);
		m_strText+=str;
	}
	FormatString(m_strText);

	delete m_pFnt;
	m_pFnt=CreateFnt();
 	CalLimit();
	return TRUE;
}

BOOL CNodeText::CheckString()
{
	//建立数据的入口
	CString str;
	CString tempStr;

	//m_strText.Empty();

	int nCount = m_ArrayTextID.GetSize();
	UINT nTextID;

	for (int i = 0; i<nCount; i++)
	{
		nTextID = m_ArrayTextID.GetAt(i);
		//g_MgrText->FlashText(nTextID, 0x00); 
		g_MgrText->GetString(nTextID, str);
		tempStr += str;
	}
	FormatString(m_strText);

	if (m_strText == tempStr) {
		return false;
	}
	else {
		return true;
	}
}



CFnt * CNodeText::CreateFnt()
{
	CFnt *p;
	if(m_nFont==FONT_SHX)
	{
		CFntShx *tmp=new CFntShx;
		p=tmp;
		
		tmp->m_strChineseFont=m_strFontCHS;
		tmp->m_strEnglishFont=m_strFontEN;
		tmp->m_strNumberFont =m_strFontNUM;
	}
	else if(m_nFont == FONT_TTF)
	{
		CFntTTF *tmp=new CFntTTF;
		p=tmp;
		tmp->m_lf=m_lf;
	}
	else if(m_nFont == FONT_MATRIX)
	{
		CFntHzk *tmp=new CFntHzk;
		p=tmp;

		tmp->m_HzkType = m_HzkType;
		tmp->m_AscType = m_AscType;
	}
	else
	{
		return NULL;
	}
	p->SetProperty(PTY_INIT,m_property);

	p->m_strText		=m_strText;
	p->m_dotBase		=m_dotBase;
	p->m_nPath			=m_nPath;
	p->m_nAlign			=m_nAlign;
	
	//直线方式
	p->m_dbDirection	=m_dbDirection;

	//圆弧方式
	p->m_dbRadius		=m_dbRadius;
	p->m_dbStart		=m_dbStart;
	p->m_bCCW			=m_bCCW;
	p->m_bInside		=m_bInside;

	//字形
	p->m_dbCharSpace	=m_dbCharSpace;
	p->m_dbHeight		=m_dbHeight;
	p->m_dbLineSpace	=m_dbLineSpace;
	p->m_dbWidthFactor	=m_dbWidthFactor;

	p->m_bFixSpace		=m_bFixSpace;
    p->m_strPrecisK		=m_strPrecisK;

	p->CreateStrokeList();

	return p;
}

BOOL CNodeText::PreprocessMark(DWORD nStyle)
{
	//nStyle暂时没用到，以备以后扩展
	BOOL bFlag = FALSE;
	if(m_pFnt)
	{
		CString str;
		CString strText;
		int nCount = m_ArrayTextID.GetSize();
		UINT nTextID;
		BOOL bOverMark=FALSE;
		for(int i=0;i<nCount;i++)
		{
			nTextID = m_ArrayTextID.GetAt(i);
			g_MgrText->GetString(nTextID,str,0x00);	
			if (g_MgrText->IsOverMark(nTextID,0x00))
			{
				bOverMark=TRUE;
			}
			strText+=str;
		}
		FormatString(strText);
		
		if(strText == m_strText)
		{
			bFlag = FALSE;
			if (bOverMark)
			{
				CWnd *pWin = ((CWinApp *) AfxGetApp())->m_pMainWnd;
				CString str;
				str.LoadString(IDS_PROMPT_JUMP_OVERLAP);
				pWin->SendMessage(WM_SETMESSAGESTRING, 0,(LPARAM)(LPCSTR)str);
				Beep(415,50);
			}
			
		}
		else{
			bFlag = TRUE;
			m_strText = strText;
			delete m_pFnt;
			m_pFnt=CreateFnt();
			CalLimit();
		}
	}
	return bFlag;
}

void CNodeText::MarkPreview(CDC *pDC, const int &nPreviewSpeed)
{
	m_pFnt->MarkPreview(pDC, nPreviewSpeed);
}

void CNodeText::SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{
	m_property.m_color  = color;
	m_property.m_nLayer  = nLayer;
	
	delete m_pFnt;
	m_pFnt=CreateFnt();
	CalLimit();

	return;
}

void CNodeText::FormatString(CString &strText)
{
	if(!m_bCustomFormat)
	{
		return;
	}
	
	//格式化文本输出
	CString strNext;

	int nSize = strText.GetLength();
	char tmp;
	int pos=0;
	int nCount = m_strFormat.GetLength();

	strNext.Empty();

	for(int i=0;i<nCount;i++)
	{
		tmp = m_strFormat.GetAt(i);
		if(tmp == 'X')
		{
			if(nSize>pos){
				tmp = strText.GetAt(pos);
			}
			else{
				//超出信息量的范围则不用进行相应处理直接退出
				break;
			}
			pos++;
		}
		else if(tmp == 'N')
		{
			pos++;
			continue;
		}
		strNext += tmp;
	}

	strText = strNext;

	return;
}

void CNodeText::CreateFromBar(UINT nText)
{
	m_ArrayTextID.Add(nText);
}

void CNodeText::SetStrokeList(StrokeList *pStrokeList)
{
	POSITION pos=m_pFnt->m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_pFnt->m_list.GetNext(pos);
		pStrokeList->AddTail(pStroke);
	}

	return;
}

CNode * CNodeText::ReDefineCopy()
{
	POSITION pos=m_pFnt->m_list.GetHeadPosition();
	CStroke *pStroke,*pNew;
	CNodeText *tmp=new CNodeText;
	tmp->m_pFnt=new CFnt;
	tmp->m_property=m_property;
	tmp->m_bSelect = m_bSelect;
	while(pos)
	{
		pStroke=m_pFnt->m_list.GetNext(pos);
		pNew=pStroke->Copy();
		tmp->m_pFnt->m_list.AddTail(pNew);
	}
	return tmp;
}

void CNodeText::AddToPlt(CNode *pNodePlt)
{
	if (NULL == pNodePlt) 
	{
		return;
	}

	POSITION pos=m_pFnt->m_list.GetHeadPosition();
	CStroke *pStroke,*pStrokeNew;
	while(pos)
	{
		pStroke=m_pFnt->m_list.GetNext(pos);
		pStrokeNew=pStroke->Copy();
		pNodePlt->GetList().AddTail(pStrokeNew);
	}

	pNodePlt->m_property = m_property;

	return;
}

BOOL CNodeText::GetNodeFirstDot(CDot &dt)
{
	int nCount = m_pFnt->m_list.GetCount();
	if(nCount <= 0)
	{
		return FALSE;
	}

	CStroke *pStroke = NULL;
	POSITION pos = m_pFnt->m_list.GetHeadPosition();
	while(pos != NULL)
	{
		pStroke = m_list.GetNext(pos);
		if (pStroke != NULL)
		{
			if (pStroke->m_dotArray.GetSize() > 0)
			{
				dt = pStroke->m_dotArray.GetAt(0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CNodeText::GetNodeLastDot(CDot &dt)
{
	int nCount = m_pFnt->m_list.GetCount();
	if(nCount <= 0)
	{
		return FALSE;
	}

	POSITION pos=m_pFnt->m_list.GetTailPosition();
	if (pos != NULL)
	{
		CStroke *pStroke = m_pFnt->m_list.GetNext(pos);
		if (pStroke != NULL)
		{
			int nDCount = pStroke->m_dotArray.GetSize();
			if (nDCount > 0)
			{
				dt = pStroke->m_dotArray.GetAt(nDCount - 1);
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CNodeText::DrawExtern(XDC *pDC)
{
	Draw(pDC);
} 


void CNodeText::DelTextByID(UINT nTextID)
{
	UINT nIndex;
	int nCount = m_ArrayTextID.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		nIndex = m_ArrayTextID.GetAt(i);
		if (nTextID == nIndex) 
		{
			m_ArrayTextID.RemoveAt(i);
			break;
		}
	}

	g_MgrText->DeleteByID(nTextID);

	return;
}


void CNodeText::GetStrokeInQuad(CQuad &quad, StrokeList &strokeList)
{
	CStroke::GetStrokeInQuadFromStrokeList(m_pFnt->m_list, strokeList, quad, m_property, m_nResinRect);
}


void CNodeText::UpadateMgrText(){

	int nCount = this->m_ArrayTextID.GetSize();
	for(int i=0;i<nCount;i++){
		g_MgrText->ChangeText(this->m_strText,this->m_ArrayTextID.GetAt(i));
	}

	this->CreateString(false);
}