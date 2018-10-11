// NodeDib.cpp: implementation of the CNodeDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
/*#include "hl.h"*/
#include "NodeDot.h"
#include "NodeDib.h"
#include "LocNukeForHL.h"
#include "StringIDTable.h"
#include "HLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNodeDot::CNodeDot()
{
	m_type=NODE_DOT;
}

CNodeDot::CNodeDot(double dbBox,int row,int column,int iDotTime)
{
	m_type=NODE_DOT;
	
	m_dbBox=dbBox;
	m_iRow=row;
	m_iCol=column;
	m_iDotTime=iDotTime;
}
                
CNodeDot::~CNodeDot()
{

}                                                         

void CNodeDot::Mark(HWND hWnd)
{

	//整个函数未使用

	//打未纠正数据
	MARK_BMP mark_bmp;
	double* pdbX=new double[m_iCol];
	double* pdbY=new double[m_iCol];
	int index=0;
	mark_bmp.nType=1;
	mark_bmp.pdbX=pdbX;
	mark_bmp.pdbY=pdbY;
	BOOL bStop=FALSE;
	UINT iMarkrtn=0;			
	{ 
		CStdioFile fx,fy;
		CString str;
		CString strFileName1;
		CString strFileName2;
		strFileName1=g_sys.GetInstallPath();			
		strFileName2=g_sys.GetInstallPath();
		strFileName1+="MarkX.dat";
		strFileName2+="MarkY.dat";
		if (fx.Open(strFileName1,CFile::modeCreate|CFile::modeWrite)&&fy.Open(strFileName2,CFile::modeCreate|CFile::modeWrite))
		{
			double ssx,ssy;
//			int outx,outy;	
			for(int j=0;j<m_iRow;j++)
			{ 
				//每行
				index=0;
				for(int i=0;i<m_iCol;i++)
				{						
					{
						//每列
						ssx=m_dbBox/(m_iCol-1)*i-m_dbBox/2;
						ssy=m_dbBox/(m_iRow-1)*(m_iRow-j-1)-m_dbBox/2;						
						pdbX[index]=ssx;
						pdbY[index]=ssy;
//						DoCali(0,ssx,ssy,outx,outy,FALSE);
//						str.Format("%d	",outx);
						fx.WriteString(str);			
//						str.Format("%d	",outy);
						fy.WriteString(str);
						index++;
					}
				}
				if(index>0)
				{

					int nPos =int(double(j)/m_iRow*10.+0.5);
					::SendMessage(hWnd,WM_MARK_END,MARK_INFO,nPos);			

					mark_bmp.d.nDelay=m_iDotTime;
					//iMarkrtn=g_DAT.Mark_BMP(1, LPVOID(&mark_bmp),index,m_property.m_nLayer);
					if (iMarkrtn!=0)
					{
						bStop=TRUE;
						break;
					}
				}
				str.Format("\n");
				fx.WriteString(str);
				fy.WriteString(str);
			}
			fx.Close();
			fy.Close();
		}
		else
		{
			delete[] pdbX;
			delete[] pdbY;
			CString strError;
			//LcnLoadString(IDS_SAVE_DOT_FAIL,strError,NAME_RES_MODULE,_T("保存点阵文件失败!"));
			AfxMessageBox(strError);
			return;
		}
	}	
	delete[] pdbX;
	delete[] pdbY;
	return;
}


BOOL CNodeDot::Track(CWnd* pWnd, CPoint ptOrg)
{
	CHLView *pView = (CHLView *)pWnd;
	pView->ViewDPtoLP(&ptOrg);

	//删除可能存在的数据
	Delete();

	CStroke *pStroke;
	pStroke=new CStroke(m_property);
	pStroke->Add(ptOrg);

	m_list.AddTail(pStroke);

	m_quadLimit.left   = ptOrg.x - 0.1;
	m_quadLimit.right  = ptOrg.x + 0.1;
	m_quadLimit.top    = ptOrg.y + 0.1;
	m_quadLimit.bottom = ptOrg.y - 0.1;


	//CDC* pDC;
	//pDC = pWnd->GetDC();
	//ASSERT_VALID(pDC);
	//CPen pen(PS_SOLID,3,RGB(0,0,0));  
	//CPen *oldPen=pDC->SelectObject(&pen);


	//CRect rect(int(m_quadLimit.left), int(m_quadLimit.top), int(m_quadLimit.right), int(m_quadLimit.bottom));

	//HGDIOBJ pOld = pDC->SelectObject(GetStockObject(BLACK_BRUSH));
	//pDC->Ellipse(rect);
	//pDC->SelectObject(pOld);



	return TRUE;
}

CQuad CNodeDot::CalLimit()
{
	CNode::CalLimit();

	//left<right, top>bottom
	CDot dotCenter = m_quadLimit.GetBaseDot();
	m_quadLimit.left   = dotCenter.x - 0.1;
	m_quadLimit.right  = dotCenter.x + 0.1;
	m_quadLimit.top    = dotCenter.y + 0.1;
	m_quadLimit.bottom = dotCenter.y - 0.1;

	return m_quadLimit;
}

void CNodeDot::Draw(CDC *pDC)
{
	CRect rect(int(m_quadLimit.left), int(m_quadLimit.top), int(m_quadLimit.right), int(m_quadLimit.bottom));

	HGDIOBJ pOld = pDC->SelectObject(GetStockObject(BLACK_BRUSH));
	pDC->Ellipse(rect);
	pDC->SelectObject(pOld);

}

void CNodeDot::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;
	
	if (bFlag)
	{
		//写入头信息
		fs<<"NDOT_B";
		m_property.Serialize(fp,bFlag);
		Serialize_list(fp,bFlag);
		//写入尾信息
		fs<<"NDOT_E";
	}
	else
	{
		while(fs>>str)
		{
			if(str.Left(5) == "PRO_B"){
				m_property.Serialize(fp,bFlag);
				continue;
			}
			if(str.Left(4) == "SL_B"){
				Serialize_list(fp,bFlag);
				continue;
			}
			if(str.Left(6) == "NDOT_E")
				break;
		}
	}
}