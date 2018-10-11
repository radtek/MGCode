// NodeDib.cpp: implementation of the CNodeDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "NodeDib.h"
#include  "windowsx.h"	//for GlobalAllocPtr.
#include "DlgFileReplyRename.h"
#include "Error.h"
#include "HLView.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



extern void GenImportFullName(CString&);
extern CString g_OpenFileName;
extern CString g_OldFileName;
extern BOOL g_bClipBoardCopyNode;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNodeDib::CNodeDib()
{
	m_type = NODE_BMP;
	m_movex = m_movey = 0;
	m_bOppsitionCol = 0;//正常
	m_dbSpaceX = 0.05;
	m_dbSpaceY = 0.05;
	m_iWidth = m_iHeight = 0;
	m_strBmpName = "";
	m_pImageSrc = NULL;
	m_dbDotTime=1000;
	m_dbAngle=0.0;
	m_iDispWidth=m_iDispHeight=0;
	m_nDibType=1;
	m_bXFlip=FALSE;
	m_bYFlip=FALSE;
	m_bLoop=FALSE;
	m_dbLoopCompate=0.0;
	m_nAccTime=0;
	m_dbBrightness=0.;
	m_dbContrast=0.0;
	m_pDC = NULL;
	m_dbSpaceXR=0.05;
	m_dbSpaceYR=0.05;
}

CNodeDib::CNodeDib(CString filename)
{
	//GenImportFullName(filename);
	m_type = NODE_BMP;
	m_movex = m_movey = 0;
	m_bOppsitionCol = 0;//正常
	m_strBmpName = filename;
	m_dbSpaceX = 0.05;
	m_dbSpaceY = 0.05;
	m_iWidth = m_iHeight = 0;
	m_pImageSrc = NULL;
	m_dbDotTime=1000;
	m_dbAngle=0.0;
	m_iDispWidth=m_iDispHeight=0;
	m_nDibType=1;
	m_bXFlip=FALSE;
	m_bYFlip=FALSE;
	m_bLoop=FALSE;
	m_dbLoopCompate=0.0;
	m_nAccTime=0;
	m_dbBrightness=0.;
	m_dbContrast=0.0;
	m_pDC = NULL;
	m_dbSpaceXR=0.05;
	m_dbSpaceYR=0.05;
}
                
CNodeDib::~CNodeDib()
{
	Delete();

}                                                         

void CNodeDib::Serialize(FILE* fp,BOOL bFlag)
{
	//本标准采用文本方式进行数据录入
	CMySerialize fs(fp);
	CString str;

	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (pFWnd)
	{
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(pView)
		{
			m_pDC = &(pView->m_DC);
		}
	}

	if(bFlag)
	{
		UpdateName(); //执行图片Copy等操作
		fs<<"NDIB_B";
		fs<<100;
		fs<<m_strBmpName;
		fs<<101;
		fs<<m_movex;
		fs<<102;
		fs<<m_movey;
		fs<<103;
		fs<<m_dbSpaceX;
		fs<<105;
		fs<<m_iHeight;
		fs<<106;
		fs<<m_iWidth;
		fs<<107;
		fs<<m_dbHeight;
		fs<<108;
		fs<<m_dbWidth;
		fs<<113;
		fs<<m_dbDotTime;
		fs<<114;
		fs<<m_bLoop;
		fs<<115;
		fs<<m_dbAngle;
		fs<<116;
		fs<<m_nDibType;
		fs<<117;
		fs<<m_bXFlip;
		fs<<118;
		fs<<m_bYFlip;
		fs<<119;
		fs<<m_dbSpaceY;
		fs<<120;
		fs<<m_dbLoopCompate;
		fs<<121;
		fs<<m_nAccTime;
		fs<<122;
		fs<<m_dbBrightness;
		fs<<123;
		fs<<m_dbContrast;
		//打标属性
		//////////////////////
		fs<<109;
		fs<<m_bOppsitionCol;//下一个节点从112开始
		
		m_property.Serialize(fp,bFlag);

		fs<<"NDIB_E";
	}
	else
	{
		while(fs>>str)
		{
			switch(atoi(str))
			{
			case 0:
				if (str=="PRO_B")
					m_property.Serialize(fp,bFlag);
				else if (str=="NDIB_E")
					goto NODEDIB_SERI_END;
				break;
			case 100:
				fs>>m_strBmpName;
				break;
			case 101:
				fs>>m_movex;
				break;
			case 102:
				fs>>m_movey;
				break;
			case 103:
				fs>>m_dbSpaceX;
				break;
			case 105:
				fs>>m_iHeight;
				break;
			case 106:
				fs>>m_iWidth;
				break;
			case 107:
				fs>>m_dbHeight;
				break;
			case 108:
				fs>>m_dbWidth;
				break;
			case 109:
				fs>>m_bOppsitionCol;//下一个节点从112开始
				break;
			case 113:
				fs>>m_dbDotTime;
				break;
			case 114:
				fs>>m_bLoop;
				break;
				//add by henry_ye
			case 115:
				fs>>m_dbAngle;
				break;
			case 116:
				fs>>m_nDibType;
				break;
			case 117:
				fs>>m_bXFlip;
				break;
			case 118:
				fs>>m_bYFlip;
				break;
			case 119:
				fs>>m_dbSpaceY;
				break;
			case 120:
				fs>>m_dbLoopCompate;
				break;
			case 121:
				fs>>m_nAccTime;
				break;
			case 122:
				fs>>m_dbBrightness;
				break;
			case 123:
				fs>>m_dbContrast;
				break;
			}
		}
NODEDIB_SERI_END:
		m_property.Serialize(fp,bFlag);
		m_dbSpaceXR= m_dbWidth/(m_iWidth-1);
		m_dbSpaceYR= m_dbHeight/(m_iHeight-1);
		DataProc();
	}
}

BOOL CNodeDib::CanGroup()
{
	//位图不允许组合
	return FALSE;
}

void CNodeDib::ProcMinusRatioX()
{
	int angle=Pai2Deg(m_dbAngle);
	angle%=360;
	while (angle<0)
	{
		angle+=360;
	}
	while (angle>=360)
	{
		angle-=360;
	}
	m_dbAngle=Deg2Pai(angle);
	if ((angle>=315||angle<=45)||(angle>=135&&angle<=225))
	{
			m_bXFlip=!m_bXFlip;	
			m_dbAngle=PAI*2-m_dbAngle;	
	}
	else if ((angle>=45&&angle<=135)||(angle<=315&&angle>=225))
	{
			m_bYFlip=!m_bYFlip;
			if (m_dbAngle<PAI)
			{
				m_dbAngle=PAI-m_dbAngle;
			}
			else
			{
				m_dbAngle=3.0*PAI-m_dbAngle;
			}
	}
}

void CNodeDib::ProcMinusRatioY()
{
	int angle=Pai2Deg(m_dbAngle);
	angle%=360;
	while (angle<0)
	{
		angle+=360;
	}
	while (angle>=360)
	{
		angle-=360;
	}
	m_dbAngle=Deg2Pai(angle);
	if ((angle>=315||angle<=45)||(angle>=135&&angle<=225))
	{
		m_bYFlip=!m_bYFlip;	
		m_dbAngle=PAI*2-m_dbAngle;	
	}
	else if ((angle>=45&&angle<=135)||(angle<=315&&angle>=225))
	{
		m_bXFlip=!m_bXFlip;
		if (m_dbAngle<PAI)
		{
			m_dbAngle=PAI-m_dbAngle;
		}
		else
		{
			m_dbAngle=3.0*PAI-m_dbAngle;
		}
	}
}

void CNodeDib::Scale(CDot dot,double ratioX,double ratioY)
{
	int iBckHeight=m_iHeight;
	int iBckWidth=m_iWidth;
	double dbBckAngle=m_dbAngle;
	double dbMovex=m_movex;
	double dbMovey=m_movey;

	if((fabs(ratioX-1.0)>0.00001)||/*&&*/(fabs(ratioY-1.0)>0.00001))
	{
		//proc angle		
		if (ratioX<0.0)
		{
			ProcMinusRatioX();
		}
		if (ratioY<0.0)
		{
			ProcMinusRatioY();
		}

		//proc center& weigth
		double tx,ty;
		tx=m_dbSpaceXR*(m_iWidth-1)*ratioX;
		ty=m_dbSpaceYR*(m_iHeight-1)*ratioY;

		m_iWidth=int(tx/m_dbSpaceX+0.5)+1;
		m_iHeight=int(ty/m_dbSpaceY+0.5)+1;
		
		m_dbSpaceXR= tx/(m_iWidth-1);
		m_dbSpaceYR= ty/(m_iHeight-1);
		
//		m_iHeight = (int)(fabs(m_iHeight*ratioY));
//		m_iWidth = (int)(fabs(m_iWidth*ratioX));
		
		tx=m_movex-dot.x;
		m_movex=ratioX*tx+dot.x;	
		ty=m_movey-dot.y;
		m_movey=ratioY*ty+dot.y;


		if (m_iHeight <= 2 || m_iWidth <= 2)
		{
			m_iHeight=iBckHeight;
			m_iWidth=iBckWidth;
			m_dbAngle=dbBckAngle;
			m_movex=dbMovex;
			m_movey=dbMovey;
			// 			CString str;
			// 			str.LoadString(IDS_PROMPT_SCALE_MINIMUM);
			AfxMessageBox(IDS_PROMPT_SCALE_MINIMUM);
			return;
		}
		DeCode(m_strBmpName);
		return;
	}
	else
	{
		return;
	}
}

void CNodeDib::Rotate(CDot dot,double angle)
{
	angle = angle*PAI/180;
	double dbSine=sin(angle);
	double dbCos=cos(angle);
	double a,b,newx,newy;	
 	a=m_movex-dot.x;
 	b=m_movey-dot.y;
	
	newx=a*dbCos+b*dbSine;
	newy=b*dbCos-a*dbSine;

	m_movex=dot.x+newx;
 	m_movey=dot.y+newy;
	//test for ????????????????关闭旋转功能
// 	int nAngle=Pai2Deg(angle);
// 	while (nAngle<0)
// 	{
// 		nAngle+=360;
// 	}
// 	while (nAngle>=360)
// 	{
// 		nAngle-=360;
// 	}
// 	if (nAngle>=45&&nAngle<135)
// 	{
// 		m_dbAngle-=(PAI/2);
// 	} 
// 	else if (nAngle>=135&&nAngle<225)
// 	{
// 		m_dbAngle-=PAI;
// 	}
// 	else if (nAngle>=225&&nAngle<315)
// 	{
// 		m_dbAngle-=(PAI*1.5);
// 	}
// 	else
// 		return;
	
	m_dbAngle-=angle;
	int iangle=Pai2Deg(m_dbAngle);
	iangle%=360;
	while (iangle<0)
	{
		iangle+=360;
	}
	while (iangle>=360)
	{
		iangle-=360;
	}
	m_dbAngle=Deg2Pai(iangle);
	DeCode(m_strBmpName);
	return;
}

BOOL CNodeDib::Move(double moveX,double moveY)
{
	double dbscale= DPMM;

//	dbscale = 1/dbscale;
//	moveX *= dbscale;
//	moveY *= -dbscale;
	m_movex += moveX;
	m_movey += moveY;
	//DeCode(m_strBmpName); //阵列时要调用，因为CTree::Move(会调用) 因为父类CNode会调用scasle(),scale()会调用Decode();
	CalLimit();
	return TRUE;
}

void CNodeDib::Draw(XDC *pDC)
{
	m_pDC = pDC;
	if(m_pImageSrc)
	{
		if (pDC->IsPrinting())
		{
			//BOOL bDraw=FALSE;
			double x,y;
			BYTE tmp;
			double dbCenterX, dbCenterY;
			CDot tmpDot;
			dbCenterX = m_dbSpaceXR*(m_iWidth-1)/2.0;
			dbCenterY = m_dbSpaceYR*(m_iHeight-1)/2.0;
			double dSin=sin(m_dbAngle);
			double dCos=cos(m_dbAngle);	
			BOOL bDraw=FALSE;
			for(int j=0;j<m_iHeight;j++){
				//每行
				for(int i=0;i<m_iWidth;i++){
					//每列
					if( 0==(j%2) ){//正向扫描
						tmp = *(m_pImageSrc+j*m_iWidth+i);
						x = i*m_dbSpaceXR-dbCenterX;//+m_movex;
						y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
					}
					else{//逆向扫描
						tmp = *(m_pImageSrc+j*m_iWidth+((m_iWidth-1-i)));
						x = (m_iWidth-1-i)*m_dbSpaceXR-dbCenterX;//+m_movex;
						y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
					}
					if (m_nDibType==0)
					{
						tmp = m_bOppsitionCol?~tmp:tmp;
						{
							tmpDot.x=x*dCos-y*dSin+m_movex;
							tmpDot.y=x*dSin+y*dCos+m_movey;
							//tmpDot.TrueToLog();
							pDC->SetPixelV((int)tmpDot.x,(int)tmpDot.y,RGB(tmp,tmp,tmp));
						}
					} 
					else
					{
						bDraw = m_bOppsitionCol?tmp:!tmp;
						if( bDraw ){
							tmpDot.x=x*dCos-y*dSin+m_movex;
							tmpDot.y=x*dSin+y*dCos+m_movey;
							//tmpDot.TrueToLog();
							pDC->SetPixelV((int)tmpDot.x,(int)tmpDot.y,m_property.m_color);
						}
					}
					
				}
			}
		}
		else
		{
			//char pbuf[256];
			BITMAP   bm;
			m_bmp.GetBitmap(&bm); 
			CDC   dcCompatible; 
			dcCompatible.CreateCompatibleDC(pDC->m_pDC); 
			dcCompatible.SelectObject(&m_bmp);

			CQuad quad;
			CRgn cRgn;
			CPoint point[4];
			CalDibRgn(quad,point,4);
			pDC->m_pDC->LPtoDP(point,4);
			VERIFY(cRgn.CreatePolygonRgn(point,4,ALTERNATE));
			pDC->m_pDC->SelectClipRgn(&cRgn,RGN_COPY);
			SetStretchBltMode(pDC->m_pDC->m_hDC,COLORONCOLOR);	
			pDC->m_pDC->StretchBlt((int)quad.left,(int)quad.bottom,int(quad.right-quad.left),int(quad.top-quad.bottom),&dcCompatible,0,0, bm.bmWidth,bm.bmHeight,SRCCOPY); 
			cRgn.DeleteObject();
			pDC->m_pDC->SelectClipRgn(NULL,RGN_COPY);
		}
	}	
}

BOOL CNodeDib::Track(CWnd* pWnd, CPoint ptOrg)
{
	return DeCode(m_strBmpName);
}

void CNodeDib::Mark(HWND hWnd)
{
	int n =  g_DAT.GetMarkCount(m_property.m_nLayer);	
	BOOL bStop = FALSE;	

	//dsf 2010.04.06
	//BMPSPEC bmpSpec;
	//bmpSpec.dbDotSpace=m_dbSpaceX;
	//PARA para=m_play->GetPara(m_property.m_nLayer);
	//double dbMarkSpeed=para.dbMarkSpeed; /*m_property.m_nLayer*/
	//bmpSpec.dbQFreq=dbMarkSpeed/m_dbSpaceX/1000;
	//bmpSpec.dbQRelease=para.dbQRelease;
	//bmpSpec.dbDotTime=int(m_dbDotTime);
	//g_DAT.BmpStart(&bmpSpec);

	
	for(int i= 0;i<n;i++)
	{		
		if(m_pImageSrc)
		{
//			MARK_BMP mark_bmp;
			double x,y;
			BYTE tmp;
			double dbCenterX, dbCenterY;		
			//CDot tmpDot;
			dbCenterX = m_dbSpaceXR*(m_iWidth-1)/2.0;
			dbCenterY = m_dbSpaceYR*(m_iHeight-1)/2.0;
			double dSin=sin(m_dbAngle);
			double dCos=cos(m_dbAngle);
			UINT nType=(m_nDibType==0?3:1);
			DOT6* pdot6=new DOT6[m_iWidth+m_nAccTime];
// 			double* pdbX=new double[m_iWidth+m_nAccTime];
// 			double* pdbY=new double[m_iWidth+m_nAccTime];
			int index=0;
// 			mark_bmp.nType=nType;
// 			mark_bmp.pdbX=pdbX;
// 			mark_bmp.pdbY=pdbY;
	
			if (m_nDibType==0)
			{
				//灰度打标
//				BYTE* pbGray=new BYTE[m_iWidth+m_nAccTime];
//				mark_bmp.g.pbGray=pbGray;
//				mark_bmp.g.dbSpace=m_dbSpaceX;
//				mark_bmp.g.nAtu_time=g_sys.m_nAtu_time;
//				mark_bmp.g.dbAtu_tag=g_sys.m_dbAtu_tag;
//				mark_bmp.g.dbPlus_cof=g_sys.m_dbPlus_cof;

				for(int j=0;j<m_iHeight;j++)
				{
					//每行
					index=0;
					//补点测试
					tmp = m_bOppsitionCol?255:0;
					for (int k=0;k<m_nAccTime;k++)
					{										
						if (m_bLoop)
						{
							x = ((j%2==0)?k-m_nAccTime:m_iWidth-1+m_nAccTime-k+m_dbLoopCompate)*m_dbSpaceXR-dbCenterX;//+m_movex;
							y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
						}
						else
						{
							x = (k-m_nAccTime)*m_dbSpaceXR-dbCenterX;//+m_movex;
							y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
						}
						pdot6[k].x=x*dCos-y*dSin+m_movex;
						pdot6[k].y=x*dSin+y*dCos+m_movey;
						pdot6[k].i=tmp;				
					}
					for(int i=0;i<m_iWidth;i++)
					{
						if(m_bLoop)
						{
							if((j%2)==0)
							{
								//正向
								tmp = *(m_pImageSrc+j*m_iWidth+i);
								x = i*m_dbSpaceXR-dbCenterX;//+m_movex;
								y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
							}
							else
							{
								//反向
								//test
								tmp = *(m_pImageSrc+(j-1)*m_iWidth+(m_iWidth-i-1));
								//tmp = *(m_pImageSrc+j*m_iWidth+(m_iWidth-i-1));//test
								x = (m_iWidth-i-1+m_dbLoopCompate/*test*/)*m_dbSpaceXR-dbCenterX;//+m_movex;
								y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
							}
						}
						else
						{
							//每列
							tmp = *(m_pImageSrc+j*m_iWidth+i);
							x = i*m_dbSpaceXR-dbCenterX;//+m_movex;
							y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
						}
						/*tmp = m_bOppsitionCol?tmp:~tmp;*/ //屏幕逻辑跟电流逻辑相反						
						{
							pdot6[index+m_nAccTime].x=x*dCos-y*dSin+m_movex;
							pdot6[index+m_nAccTime].y=x*dSin+y*dCos+m_movey;
							/*pbGray[index+m_nAccTime]=tmp;*///以为灰度补偿
							tmp=g_sys.m_GrayCompensate[tmp];
							tmp = m_bOppsitionCol?tmp:~tmp; //灰度补偿相反
							pdot6[index+m_nAccTime].i=tmp;
							index++;
						}
					}
					if(index>0)
					{
						if (!g_DAT.MarkStroke(nType, LPVOID(pdot6),index+m_nAccTime,m_property.m_nLayer))
						{
								bStop=TRUE;
								break;
						}
					}			
				}
				
			} 
			else
			{
				//二值图打标
				BOOL bDraw=FALSE;
				for(int j=0;j<m_iHeight;j++)
				{
					//每行
					index=0;
					for(int i=0;i<m_iWidth;i++){
						if(m_bLoop)
						{
							if((j%2)==0)
							{
								//正向
								tmp = *(m_pImageSrc+j*m_iWidth+i);
								x = i*m_dbSpaceXR-dbCenterX;//+m_movex;
								y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
							}
							else
							{
								//反向
								tmp = *(m_pImageSrc+j*m_iWidth+(m_iWidth-i-1));
								x = (m_iWidth-i-1)*m_dbSpaceXR-dbCenterX;//+m_movex;
								y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
							}
						}
						else
						{
							//每列
							tmp = *(m_pImageSrc+j*m_iWidth+i);
							x = i*m_dbSpaceXR-dbCenterX;//+m_movex;
							y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
						}
						bDraw = m_bOppsitionCol?tmp:!tmp;
						if( bDraw ){
							pdot6[index].x=x*dCos-y*dSin+m_movex;
							pdot6[index].y=x*dSin+y*dCos+m_movey;
							index++;
						}
					}
					if(index>0)
					{
						if (!g_DAT.MarkStroke(nType, LPVOID(pdot6),index,m_property.m_nLayer))
						{
							bStop=TRUE;
							break;
						}
					}			
				}		
			}
			delete[] pdot6;
		}
		if(bStop)
			break;
	}
	//g_DAT.BmpEnd();
	return /*iMarkrtn*/;
}

BOOL CNodeDib::Create(CRect &rect)
{
	return TRUE;
}

BOOL CNodeDib::DataChange()
{
	//建立打标数据
	double tx,ty;	
	tx=m_dbWidth;
	ty=m_dbHeight;

	//cal m_dbSpaceX&m_dbSpaceY	
	m_iWidth=int(tx/m_dbSpaceX+0.5)+1;
	m_iHeight=int(ty/m_dbSpaceY+0.5)+1;
	
	
	m_dbSpaceXR= tx/(m_iWidth-1);
	m_dbSpaceYR= ty/(m_iHeight-1);

	return DeCode(m_strBmpName);
}

BOOL CNodeDib::DataProc()
{
	return DeCode(m_strBmpName);
}

BOOL CNodeDib::Click(CPoint &point, CWnd *pWnd)
{	
	CPoint pCPoint[4];
	CQuad quad;
//	CRect rect;
	CalDibRgn(quad,pCPoint, 4);
	CDot dot;
	dot.x = point.x;
	dot.y = point.y;
	return quad.DotIn(dot);
//	CRgn rgn;
//	rgn.CreatePolygonRgn(pCPoint,4,ALTERNATE);
//	quad=CQuad(dot,dbR,dbR);
//	rect=quad;
//	BOOL bRtn=rgn.RectInRegion(&rect);
//	rgn.DeleteObject();
//	return bRtn;

	/*return m_quadLimit.Click(dot,dbR);*/
}

CNode * CNodeDib::Copy(UINT nStyle)
{
	CNodeDib *tmp=new CNodeDib();
	
	tmp->m_bLoop=m_bLoop;
	tmp->m_property=m_property;
	tmp->m_bSelect = m_bSelect;
	tmp->m_strBmpName=m_strBmpName;
	tmp->m_bOppsitionCol = m_bOppsitionCol;
	tmp->m_dbDotTime=m_dbDotTime;
	tmp->m_dbSpaceX=m_dbSpaceX;
	tmp->m_dbSpaceY=m_dbSpaceY;
	tmp->m_dbHeight=m_dbHeight;
	tmp->m_dbWidth=m_dbWidth;
	tmp->m_iHeight=m_iHeight;
	tmp->m_iWidth=m_iWidth;
	tmp->m_property=m_property;
	tmp->m_movex=m_movex;
	tmp->m_movey=m_movey;
	
	tmp->m_dbAngle=m_dbAngle;
	tmp->m_nDibType=m_nDibType;
	tmp->m_bXFlip=m_bXFlip;
	tmp->m_bYFlip=m_bYFlip;
	tmp->m_bSelect=m_bSelect; //for 阵列
	tmp->m_iDispHeight=m_iDispHeight;
	tmp->m_iDispWidth=m_iDispWidth;
	tmp->m_nAccTime=m_nAccTime;
	tmp->m_dbLoopCompate=m_dbLoopCompate;
	tmp->m_dbBrightness=m_dbBrightness;
	tmp->m_dbContrast=m_dbContrast;
	tmp->m_pDC =m_pDC;

	tmp->m_dbSpaceXR=m_dbSpaceXR;
	tmp->m_dbSpaceYR=m_dbSpaceYR;

	tmp->DataProc();
	return tmp;
}

void CNodeDib::SetProperty(int mode,CProperty pty ,XDC* pDC)
{	
	m_pDC = pDC;
	m_property.Set(mode,pty);
}

void CNodeDib::MarkPreview(CDC *pDC, const int &nPreviewSpeed)
{
	if(m_pImageSrc)
	{
		CStroke strokeTmp;
		BOOL bDraw=FALSE;
		double x,y;
		BYTE tmp;
		double dbCenterX, dbCenterY;
		double dSin=sin(m_dbAngle);
		double dCos=cos(m_dbAngle);
		CDot tmpDot;
		dbCenterX = m_dbSpaceXR*(m_iWidth-1)/2.0;
		dbCenterY = m_dbSpaceYR*(m_iHeight-1)/2.0;
		int index=0;
		if (m_nDibType==0)
		{
			
			for(int j=0;j<m_iHeight;j++){
				//每行
				for(int i=0;i<m_iWidth;i++){
					//每列
					if (m_bLoop)
					{
						if( 0==(j%2) ){//正向扫描
							tmp = *(m_pImageSrc+j*m_iWidth+i);
							x = i*m_dbSpaceXR-dbCenterX;//+m_movex;
							y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
						}
						else{//逆向扫描
							tmp = *(m_pImageSrc+j*m_iWidth+((m_iWidth-1-i)));
							x = (m_iWidth-1-i)*m_dbSpaceXR-dbCenterX;//+m_movex;
							y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
						}
					} 
					else
					{
						tmp = *(m_pImageSrc+j*m_iWidth+i);
						x = i*m_dbSpaceXR-dbCenterX;//+m_movex;
						y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
					}
					
					tmp = m_bOppsitionCol?~tmp:tmp;
					{
						tmpDot.x=x*dCos-y*dSin+m_movex;
						tmpDot.y=x*dSin+y*dCos+m_movey;
						tmpDot.TrueToLog();
						pDC->SetPixelV((int)tmpDot.x,(int)tmpDot.y,RGB(tmp,tmp,tmp));
						index++;				
						
					}
					if (index>=20)
					{
						index=0;
						Sleep(MARKPREVIEW_SPEEDMAX-nPreviewSpeed+1);
						if(g_sys.m_bStopPreview)
						{
							AfxEndThread(0);
						}
					}
				}
			}
		} 
		else
		{
			for(int j=0;j<m_iHeight;j++){
				//每行
				for(int i=0;i<m_iWidth;i++){
					//每列
					if(m_bLoop)
					{
						if((j%2)==0)
						{
							//正向
							tmp = *(m_pImageSrc+j*m_iWidth+i);
							x = i*m_dbSpaceXR-dbCenterX;//+m_movex;
							y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
						}
						else
						{
							//反向
							tmp = *(m_pImageSrc+j*m_iWidth+(m_iWidth-i-1));
							x = (m_iWidth-i-1)*m_dbSpaceXR-dbCenterX;//+m_movex;
							y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
						}
					}
					else
					{
						//每列
						tmp = *(m_pImageSrc+j*m_iWidth+i);
						x = i*m_dbSpaceXR-dbCenterX;//+m_movex;
						y = j*m_dbSpaceYR-dbCenterY;//+m_movey;
					}
					bDraw = m_bOppsitionCol?tmp:!tmp;
					if( bDraw ){
						index++;
						tmpDot.x=x*dCos-y*dSin+m_movex;
						tmpDot.y=x*dSin+y*dCos+m_movey;
						pDC->SetPixelV((int)tmpDot.x,(int)tmpDot.y,m_property.m_color);
						//strokeTmp.Add(tmpDot);
					}
					if (index>=20)
					{
						index=0;
						Sleep(MARKPREVIEW_SPEEDMAX-nPreviewSpeed+1);
						if(g_sys.m_bStopPreview)
						{
							AfxEndThread(0);
						}
					}
				}
			}
		}
	}
	
}

BOOL CNodeDib::DeCode(CString lpszPathName)
{
	CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
	if (pFWnd)
	{
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		if(pView)
		{
			m_pDC = &(pView->m_DC);
		}
	}
	
	GenImportFullName(lpszPathName);
	AfxGetApp()->BeginWaitCursor();
	BOOL bFlag = FALSE;
	double x,y;
	g_DAT.GetMarkRange(x,y);
	double dbBoxX=x;
	double dbBoxY=y;
	delete []m_pImageSrc;
	m_pImageSrc = NULL;
	{
		int nWidth,nHeight;
		if(LoadDib((char*)(LPCTSTR) lpszPathName,nWidth,nHeight,m_bXFlip,m_bYFlip)==0)
		{
			if((m_iWidth==0)||(m_iHeight==0))
			{
				//按X拉伸，保持显示比例不变
				int nMax = nHeight>nWidth?nHeight:nWidth;
				double dbMax = nMax*m_dbSpaceXR;
				dbMax = dbMax>dbBoxX?dbBoxX:dbMax;
				dbMax = dbMax>dbBoxY?dbBoxY:dbMax;
				nMax = (int)(dbMax/m_dbSpaceXR);
				if(nHeight>nWidth)
				{
					nWidth = (nWidth*nMax/nHeight);
					nHeight = nMax;
				}
				else
				{
					nHeight = (nHeight*nMax/nWidth);
					nWidth = nMax;
				}
				m_iWidth = nWidth;
				m_iHeight =int(m_dbSpaceXR*nHeight/m_dbSpaceYR);
			}

			m_pImageSrc = new BYTE[m_iWidth*m_iHeight]; //100段线有101个端点
			int iDllRtn=0;
			long brightness=long(m_dbBrightness*255);
			brightness=brightness>255?255:brightness;
			brightness=brightness<(-255)?(-255):brightness;
			long contrast=long(m_dbContrast*100);
			contrast=contrast>100?100:contrast;
			contrast=contrast<(-100)?(-100):contrast;
			if (m_nDibType==0)
			{
				iDllRtn = ProcGrayDib(m_pImageSrc,m_iWidth,m_iHeight,m_iWidth,int(m_dbSpaceYR*m_iHeight/m_dbSpaceXR),g_sys.m_nDibSampleMode,brightness,contrast) ;
			} 
			else
			{
				iDllRtn= ProcDib(m_pImageSrc,m_iWidth,m_iHeight,m_iWidth,int(m_dbSpaceYR*m_iHeight/m_dbSpaceXR),g_sys.m_nDibSampleMode,brightness,contrast);
			}
			if(iDllRtn== 0)
			{
				m_dbWidth =(m_iWidth-1)*m_dbSpaceXR;
				m_dbHeight =(m_iHeight-1)*m_dbSpaceYR;
				char pbuf[256];
				int iPData;
				iPData=GetTempPath(256,pbuf);
				sprintf(pbuf+iPData,"HLBMP.bmp");// 
				(m_bmp).DeleteObject();						
				HBITMAP hbmp=(HBITMAP)::LoadImage(AfxGetInstanceHandle(),
					pbuf,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);   

				//for rotate;
				hbmp=GetRotatedBitmapNT(hbmp,m_dbAngle,RGB(255,255,255));
				if(hbmp==NULL)    
					return bFlag;
				m_bmp.DeleteObject();
				m_bmp.Attach(hbmp); 
				bFlag = TRUE;
			}
			DelDib();
		}
		else
		{
			CString str;
			str.LoadString(IDS_PROMPT_CANNOTFIND);
			str+=lpszPathName;
			AfxMessageBox(str);
		}
		
	}
	CalLimit();
	AfxGetApp()->EndWaitCursor();
	return bFlag;
}

void CNodeDib::Delete()
{
	delete []m_pImageSrc;
	m_pImageSrc=NULL;
	m_bmp.DeleteObject();		
}
//计算位图外包矩阵大小
CQuad CNodeDib::CalLimit()
{
//	CPoint pCPoint[4];
	CDot pCPoint[4];
	//计算画图多边形区域
	{
		double cosine   = cos(m_dbAngle);
		double sine     = sin(m_dbAngle);
		double const fX = (m_dbWidth/2.0);
		double const fY = (m_dbHeight/2.0);

		CDot cDot[4];
		cDot[0].x = fX;cDot[0].y = -fY;
		cDot[1].x = fX;cDot[1].y =  fY;
		cDot[2].x =-fX;cDot[2].y =  fY;
		cDot[3].x =-fX;cDot[3].y = -fY;
		
		double x(0.0),y(0.0);
		for (int i = 0; i < 4; i++)
		{
			x = cDot[i].x * cosine - cDot[i].y * sine;
			y = cDot[i].x * sine   + cDot[i].y * cosine;
			x = x + m_movex;
			y = y + m_movey;
			pCPoint[i].x = x;			
			pCPoint[i].y = y;
		}		
	}
	CQuad quad1;
	quad1.left=min(pCPoint[3].x,min(pCPoint[2].x,min(pCPoint[0].x,pCPoint[1].x)));
	quad1.right=max(pCPoint[3].x,max(pCPoint[2].x,max(pCPoint[0].x,pCPoint[1].x)));
	quad1.bottom=min(pCPoint[3].y,min(pCPoint[2].y,min(pCPoint[0].y,pCPoint[1].y)));
	quad1.top=max(pCPoint[3].y,max(pCPoint[2].y,max(pCPoint[0].y,pCPoint[1].y)));
	m_quadLimit=quad1;
	return m_quadLimit;

}

void CNodeDib::SetLayColor(int nLayer,COLORREF color,int nLayerFill,COLORREF colorFill)
{
	m_property.m_color  = color;
	m_property.m_nLayer  = nLayer;
}

// GetRotatedBitmapNT	- Create a new bitmap with rotated image
// Returns		- Returns new bitmap with rotated image
// hBitmap		- Bitmap to rotate
// radians		- Angle of rotation in radians
// clrBack		- Color of pixels in the resulting bitmap that do
//			  not get covered by source pixels
HBITMAP CNodeDib::GetRotatedBitmapNT( HBITMAP hBitmap, double radians, COLORREF clrBack )
{
	// Create a memory DC compatible with the display
	CDC sourceDC, destDC;
	sourceDC.CreateCompatibleDC( NULL );
	destDC.CreateCompatibleDC( NULL );

	// Get logical coordinates
	BITMAP bm;
	::GetObject( hBitmap, sizeof( bm ), &bm );

	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);

	int x1 = (int)(bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(-bm.bmWidth * sine);

	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(0,max(x1, max(x2,x3)));
	int maxy = max(0,max(y1, max(y2,y3)));

	int w = maxx - minx;
	int h = maxy - miny;

	// Create a bitmap to hold the result
	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);
	HBITMAP hbmOldSource = (HBITMAP)::SelectObject( sourceDC.m_hDC, hBitmap );
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC.m_hDC, hbmResult );
	// Draw the background color before we change mapping mode
	HBRUSH hbrBack = CreateSolidBrush( clrBack );
	HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC.m_hDC, hbrBack );
	destDC.PatBlt( 0, 0, w, h, PATCOPY );
	::DeleteObject( ::SelectObject( destDC.m_hDC, hbrOld ) );

	// We will use world transform to rotate the bitmap
	SetGraphicsMode(destDC.m_hDC, GM_ADVANCED);
	

	XFORM xform;
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = (float)-minx;
	xform.eDy = (float)-miny;

	SetWorldTransform( destDC.m_hDC, &xform );

	if (m_bOppsitionCol)
	{
		destDC.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, NOTSRCCOPY );
	}
	else
	{
		destDC.BitBlt(0,0,bm.bmWidth, bm.bmHeight, &sourceDC, 0, 0, SRCCOPY );		
	}
	::SelectObject( sourceDC.m_hDC, hbmOldSource );
	::SelectObject( destDC.m_hDC, hbmOldDest );
	return hbmResult;
}
//计算绘图的矩形框CQuad& quad，CRgn clip窗大小(CPoint* pCPoint,int iCnt)。
void CNodeDib::CalDibRgn(CQuad& quad,CPoint* pCPoint,int iCnt)
{
	if (iCnt!=4)
	{
		return;
	}
	//计算画图多边形区域
	{
		double cosine = cos(m_dbAngle);
		double sine = sin(m_dbAngle);
		double const fX=(m_dbWidth/2.0);
		double const fY=(m_dbHeight/2.0);
		CDot cDot[4];
		cDot[0].x=fX;cDot[0].y=-fY;
		cDot[1].x=fX;cDot[1].y=fY;
		cDot[2].x=-fX;cDot[2].y=fY;
		cDot[3].x=-fX;cDot[3].y=-fY;	
		double x,y;
		for (int i=0;i<4;i++)
		{
			x=cDot[i].x*cosine-cDot[i].y*sine;
			y=cDot[i].x*sine+cDot[i].y*cosine;
			x=x+m_movex;
			y=y+m_movey;
			//实际坐标转化为log坐标
//			pCPoint[i].x=LONG(x*DPMM+CENTER);			
//			pCPoint[i].y=LONG(CENTER-y*DPMM);
			m_pDC->PhysicToLogic(x, y);
			pCPoint[i].x=(long)x;			
			pCPoint[i].y=(long)y;
		}		
	}
	quad.left=min(pCPoint[3].x,min(pCPoint[2].x,min(pCPoint[0].x,pCPoint[1].x)));
	quad.right=max(pCPoint[3].x,max(pCPoint[2].x,max(pCPoint[0].x,pCPoint[1].x)));
	quad.bottom=min(pCPoint[3].y,min(pCPoint[2].y,min(pCPoint[0].y,pCPoint[1].y)));
	quad.top=max(pCPoint[3].y,max(pCPoint[2].y,max(pCPoint[0].y,pCPoint[1].y)));
	return;
}


void CNodeDib::UpdateName()
{	

	if (g_bClipBoardCopyNode)
	{			
		//复制文件操作
		//如果为绝对路径，返回，否则改为绝对路径。
		if (-1==m_strBmpName.Find(':'))
		{
			//还原路径
			CString strHLFileName;
			CString strHLFilePath;
			CString strHLImportFilePath;	
			if(!g_OpenFileName.IsEmpty())
			{		
				strHLFileName=g_OpenFileName;
				strHLFilePath=strHLFileName.Left(strHLFileName.ReverseFind('\\'));
				strHLImportFilePath=strHLFilePath+strHLFileName.Mid(strHLFileName.ReverseFind('\\'),strHLFileName.ReverseFind('.')-strHLFileName.ReverseFind('\\'));
				m_strBmpName=strHLImportFilePath+"\\"+m_strBmpName;
			}
			else if (!g_OldFileName.IsEmpty())
			{
				strHLFileName=g_OldFileName;
				strHLFilePath=strHLFileName.Left(strHLFileName.ReverseFind('\\'));
				strHLImportFilePath=strHLFilePath+strHLFileName.Mid(strHLFileName.ReverseFind('\\'),strHLFileName.ReverseFind('.')-strHLFileName.ReverseFind('\\'));
				m_strBmpName=strHLImportFilePath+"\\"+m_strBmpName;
			}
		}
		return;
	}

	if (0!=g_OldFileName.Compare(g_OpenFileName)&&(!g_OldFileName.IsEmpty()))
	{
		//为SAve as
		//do copy 文件夹
		//复原BMP路径 利用g_OldFileName;
		CString str=g_OldFileName.Left(g_OldFileName.ReverseFind('.'));
		m_strBmpName=str+'\\'+m_strBmpName;
	}
	else if (-1==m_strBmpName.Find(':'))
	{
		return; //已经是相对路径了
	}	
	CString strHLFileName=g_OpenFileName;
	CString strHLFilePath;
	CString strHLImportFilePath;
	CString strDestFile;
	CString strSrcFile=m_strBmpName;
	strHLFilePath=strHLFileName.Left(strHLFileName.ReverseFind('\\'));
	strHLImportFilePath=strHLFilePath+strHLFileName.Mid(strHLFileName.ReverseFind('\\'),strHLFileName.ReverseFind('.')-strHLFileName.ReverseFind('\\'));
	m_strBmpName=strSrcFile.Mid(strSrcFile.ReverseFind('\\')+1);
	strDestFile=strHLImportFilePath+"\\"+m_strBmpName;
	if (0==strSrcFile.Compare(strDestFile))
	{
		return;
	}
	//1.能否找到文件夹,不能则新建
	BOOL bHLImportFilePathExist=FALSE;
	CFileFind finder;
	bHLImportFilePathExist = finder.FindFile(strHLImportFilePath);		
	finder.Close();
	if (!bHLImportFilePathExist)
	{
		if(!::CreateDirectory(strHLImportFilePath,NULL))
		{
			AfxMessageBox(IDS_PROMPT_CREATEINPORTFOLDER_FAIL);
			m_strBmpName=strSrcFile;
			return;
		}		
	}
	//2.Copy 导入文件到文件夹
	if(!CopyFile(strSrcFile,strDestFile,TRUE))
	{
		CString	str;
		CString strTemp;
		str.LoadString(IDS_PROMPT_INPORTFILE1);//="导入文件夹已有文件:";
		str+=m_strBmpName;
		strTemp.LoadString(IDS_PROMPT_INPORTFILE2);
		str+=strTemp+strSrcFile; //"，\n原文件:"+strSrcFile+"\n替换或自动重命名?";
		strTemp.LoadString(IDS_PROMPT_INPORTFILE3);
		str+=strTemp;
		CDlgFileReplyRename dlg;
		dlg.m_strPromptMessage=str;
		if(IDOK==dlg.DoModal())
			CopyFile(strSrcFile,strDestFile,FALSE);
		else
		{
			//自动进行加序号处理
			str=m_strBmpName.Left(m_strBmpName.ReverseFind('.'));
			CString bmpExt;
			int len = m_strBmpName.GetLength();
			int j;
			for (j = len-1; j >= 0; j--){
				if (m_strBmpName[j] == '.'){
					bmpExt=m_strBmpName.Mid(j+1);
					break;
				}
			}
			if (j<0)
			{
				bmpExt="";
			}

			CString newName;
			int i=1;
			newName.Format("_%d.",i);
			newName+=bmpExt;
			m_strBmpName=str+newName;
			strDestFile=strHLImportFilePath+"\\"+m_strBmpName;
			while (!CopyFile(strSrcFile,strDestFile,TRUE))
			{
				i++;
				newName.Format("_%d.",i);
				newName+=bmpExt;
				m_strBmpName=str+newName;
				strDestFile=strHLImportFilePath+"\\"+m_strBmpName;
			}
			//m_strBmpName=strSrcFile;
			return;
		}
	}	
}

int CNodeDib::Pai2Deg(double f)
{
	return int(/*fabs*/f*180/PAI);
}
double CNodeDib::Deg2Pai(int a)
{
	return double(a)*PAI/180;
}

void CNodeDib::Drag(CDot dot,double ratioX,double ratioY,double moveX,double moveY)
{
	Move(moveX,moveY);

	Scale(dot,ratioX,ratioY);
	
	return;
}
