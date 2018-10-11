// NodeCodebar.cpp: implementation of the CNodeCodebar class.
//
/////////////////
#include "stdafx.h"
#include "hl.h"
#include "NodeCodebar.h"
#include "BarCodeDlg.h"
#include "Text.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
CNodeCodebar *g_btmp=NULL;
CNodeCodebar::CNodeCodebar()
{
	m_type=NODE_CODEBAR;
	initCodebar();
}

CNodeCodebar::~CNodeCodebar()
{
	if(m_pByte)
	{
		delete []m_pByte;
	}

	DeleteFill();
	if(g_btmp)
	{
		g_btmp->m_list.RemoveAll();
		g_btmp->m_listFill.RemoveAll();
	}
}

BOOL CNodeCodebar::CanGroup()
{
	return FALSE;
}

void CNodeCodebar::initCodebar()
{
	m_dbSpace = 0.6;
	m_dbcodeRate = 2.75;

	m_pByte = new BYTE[256*256];
	m_bRev = FALSE;
	m_bGoBack  = FALSE;
	m_bConnect = FALSE;
	m_dbRevWidth = 0.5;
	m_BarType = CODE39;
	m_nTextID = 1;
	m_dbFillSpace = 0.05;
	m_dbAngle = 0;
 
	m_nErrLev = 3;
	m_nHPoint = 10;
	m_nWPoint = 10;
	m_nOutline=0;

	m_nSingleLineMarkCount = 1;
	m_nNarrowLineNum = 2;

	m_dbHeightBar = 10.0f;
	m_dbshrink = 0.0f;
	m_bTextVis = TRUE;
	m_bAutoSet= FALSE;

	m_nAlign = 2;
	m_narrowW = (double)basenar;

	m_dotBase = CDot(CENTER,CENTER);

	m_bPointDataMatrix = FALSE;
	m_nPointRadiusPercent = 80;
}

void CNodeCodebar::Serialize(FILE* fp,BOOL bFlag)
{
	CMySerialize fs(fp);
	CString str;
	CString strInfo;
	CDot dot1,dot2;
	
	if (bFlag)
	{
		CQuad quad = CalLimit();
		fs<<"NBAR_B";
		fs<<100;
		fs<<m_nTextID;
		fs<<102;
		fs<<(UINT)m_BarType;		//��������
		fs<<103;
		fs<<m_type;					//�ڵ������Ժ�ɻ�������NODE_BASE���������ƣ��÷�ϰ��̫����
		fs<<104;
		fs<<m_dbHeightBar;			//����߶�
		fs<<106;
		fs<<m_narrowW ;				//�����
		fs<<107;
		fs<<m_dbshrink;				//����������
		fs<<110;
		fs<<m_nAlign;				//���뷽ʽ
		fs<<111;
		fs<<m_nOutline;				//PDF417��ģ�����
		fs<<113;
		fs<<m_nErrLev;				//������
		fs<<115;
		fs<<m_nHPoint;				//��ֱ��������
		fs<<116;
		fs<<m_nWPoint;				//ˮƽ��������
		fs<<117;
		fs<<m_bAutoSet;				//�������볤������;//true �Զ�����;FALSE �û�����
		
		dot1 = quad.GetBaseDot(BASE_BL);
		dot2 = quad.GetBaseDot(BASE_TR);

		dot1.TrueToLog();
		dot2.TrueToLog();
		
		fs<<118;
		fs<<dot1.x;
		fs<<119;
		fs<<dot1.y;
		fs<<120;
		fs<<dot2.x;
		fs<<121;
		fs<<dot2.y;
		fs<<130;

		fs<<m_bRev;				// ȷ�������Ƿ���
		fs<<131;
		fs<<m_dbRevWidth;		// ��������ʱ�Ŀ��
		fs<<132;
		fs<<(m_dbAngle * -1);	// ����Ƕ�
		fs<<133;
		fs<<m_bTextVis;			// �Ƿ���ʾ�ı�

		m_dotBase.TrueToLog();
		fs<<134;
		fs<<m_dotBase.x;
		fs<<135;
		fs<<m_dotBase.y;
		fs<<136;
		fs<<m_dbFillSpace;

		fs<<140;
		fs<<m_dbSpace;
		fs<<141;
		fs<<m_dbcodeRate;

		fs<<142;
		fs<<m_bGoBack;
		fs<<143;
		fs<<m_bConnect;
		fs<<144;
		fs<<m_nSingleLineMarkCount;
        fs<<145;
		fs<<m_nNarrowLineNum;

		fs<<146;
		fs<<m_bPointDataMatrix;
		fs<<147;
		fs<<m_nPointRadiusPercent;

		fs<<"NBAR_E";

		m_dotBase.LogToTrue();

		m_property.Serialize(fp,bFlag);
	}
	else
	{
		int i = 0;
		while(fs>>i)
		{
			switch(i)
			{
			case 100:
				{
					fs>>m_nTextID;
					if(g_MgrText->GetCopySts())
					{
						//�����COPY״̬ʱ���޸�ID
						UINT nID = g_MgrText->GetBaseID();
						m_nTextID= OFFSET_TEXTID(nID,m_nTextID);
					}
					break;
				}
			case 102:
				{
					UINT tmp;
					fs>>tmp;
					m_BarType = (BARTYPE)tmp;
					break;
				}
			case 103:
				fs>>m_type;
				break;
			case 104:
				fs>>m_dbHeightBar;
				break;
			case 106:
				fs>>m_narrowW;
				break;
			case 107:
				fs>>m_dbshrink;				//����������
				break;
			case 110:
				fs>>m_nAlign;
				break;
			case 111:
				fs>>m_nOutline;				//PDF417��ģ�����
				break;
			case 113:
				fs>>m_nErrLev;				//������
				break;
			case 115:
				fs>>m_nHPoint;				//��ֱ��������
				break;
			case 116:
				fs>>m_nWPoint;				//ˮƽ��������
				break;
			case 117:
				fs>>m_bAutoSet;				//�������볤������;//true �Զ�����;FALSE �û�����
				break;
			case 118:
				fs>>dot1.x;
				break;
			case 119:
				fs>>dot1.y;
				break;
			case 120:
				fs>>dot2.x;
				break;
			case 121:
				fs>>dot2.y;
				break;
			case 130:
				fs>>m_bRev;				//ȷ�������Ƿ���
				break;
			case 131:
				fs>>m_dbRevWidth;		//��������ʱ�Ŀ��
				break;
			case 132:
				{
					fs>>m_dbAngle;		//����Ƕ�
					m_dbAngle *= -1;
				    break;
				}
			case 133:
				fs>>m_bTextVis;			//�Ƿ���ʾ�ı�
				break;
			case 134:
				fs>>m_dotBase.x;
				break;
			case 135:
				fs>>m_dotBase.y;
				break;
			case 136:
				fs>>m_dbFillSpace;
				break;
			case 140:
				fs>>m_dbSpace;
				break;
			case 141:
				fs>>m_dbcodeRate;
				break;
            case 142:
				fs>>m_bGoBack;
				break;
			case 143:
				fs>>m_bConnect;
				break;
            case 144:
				fs>>m_nSingleLineMarkCount;
				break;
			case 145:
				fs>>m_nNarrowLineNum;
				break;
			case 146:
				fs>>m_bPointDataMatrix;
				break;
			case 147:
				fs>>m_nPointRadiusPercent;
				break;
			case 0:
				goto NODECODEBAR_SERI_END;
			}
		}
NODECODEBAR_SERI_END:
		dot1.LogToTrue();
		dot2.LogToTrue();
		m_dotBase.LogToTrue();

		m_property.Serialize(fp,bFlag);

		CText* pText = g_MgrText->GetAt(m_nTextID);
		pText->FlashNextText(0x00);	//�õ��ִ�����
		GenBarCode(0x00);
		CalLimit();
		double moveX,moveY;
		switch(m_nAlign)	//Ŀǰ��������
		{
		case ALIGN_LEFT:
			moveX=dot1.x-m_quadLimit.left;
			break;
		case ALIGN_RIGHT:
			moveX=dot2.x-m_quadLimit.right;
			break;
		case ALIGN_MID:
			moveX=(dot1.x+dot2.x)/2-(m_quadLimit.left+m_quadLimit.right)/2;
			break;
		}
		moveY = dot1.y - m_quadLimit.bottom;
		Move(moveX,moveY);
	}

	return;
}

void CNodeCodebar::Rotate(CDot dot,double angle)
{
//	m_dbAngle += angle*180/PAI;
	m_dbAngle += angle;
	CQuad quad = CalLimit();
	CDot tmp = quad.GetBaseDot();
	CNode::Rotate(tmp,angle);

	return;
}

BOOL CNodeCodebar::Move(double moveX,double moveY)
{
/*	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		pStroke->Move(moveX,moveY);
	}

	CalLimit();
//	m_dotBase = m_quadLimit.Center();
*/

	m_dotBase.Move(moveX,moveY);
	GenBarCode(0x00);
	CalLimit();
	return TRUE;
}

void CNodeCodebar::Scale(CDot dot,double ratioX,double ratioY)
{
	if (fabs(ratioX) < 0.0000001) ratioX = 1;
	if (fabs(ratioY) < 0.0000001) ratioY = 1;

	if(m_BarType>=PDF417)
	{
		//��ά��������һ��ҪXY����һ��
		if(fabs(ratioX-1) > 0.001||fabs(ratioY-1) > 0.001)
		{
			ratioX=fabs(ratioY-1)>fabs(ratioX-1)?ratioY:ratioX;
			ratioY=ratioX;
			m_dbHeightBar = (int)(fabs(m_dbHeightBar*ratioX)*1000)/1000.0;
			GenBarCode(0x00);
		}
	}
	else
	{
		//һά�������ű���XY�ɲ�һ��
		m_narrowW = ratioX*m_narrowW;
		m_dbHeightBar = m_dbHeightBar*ratioY;
		GenBarCode(0x00);
/*		POSITION pos=m_list.GetHeadPosition();
		CStroke *pStroke = NULL;
		while(pos)
		{
			pStroke=m_list.GetNext(pos);
			pStroke->Scale(dot,ratioX,ratioY);
		}
		*/
	}

	CalLimit();
	m_dotBase = m_quadLimit.Center();

	return;
}

void CNodeCodebar::Draw(XDC *pDC)
{

	CStroke::m_bRedColorDraw = m_bSelect;

	POSITION pos;
	CStroke *pStroke;

	pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pStroke=m_list.GetNext(pos);
		pStroke->Draw(pDC);
	}

	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_listFill.GetNext(pos);
		pStroke->Draw(pDC);
	}

	return;
}

void CNodeCodebar::ExportPlt(CStdioFile& file,BOOL bUnExportFill,BOOL bScall100)
{
	POSITION pos;
	CStroke *pStroke;
	
	pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{
		pStroke=m_list.GetNext(pos);
		pStroke->ExportPlt(file, bUnExportFill, bScall100);
	}
	
	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_listFill.GetNext(pos);
		pStroke->ExportPlt(file,bUnExportFill, bScall100);
	}

	return;
}

BOOL CNodeCodebar::Track(CWnd* pWnd, CPoint ptOrg)
{
	//��һ�δ���
	BOOL bResult=TRUE;
	CBarCodeDlg dlg;

	dlg.m_BarPage.m_bRev = m_bRev;
    dlg.m_BarPage.m_bGoBack  = m_bGoBack;
    dlg.m_BarPage.m_bConnect = m_bConnect;
// 	dlg.m_BarPage.m_bShowText = m_bTextVis;
	dlg.m_BarPage.m_dbHeight = m_dbHeightBar;
	dlg.m_BarPage.m_dbNarrow = m_narrowW;
	dlg.m_BarPage.m_dbShrink = m_dbshrink;
	dlg.m_BarPage.m_dbWidth = m_dbRevWidth;
	dlg.m_nBarType = (UINT)m_BarType;
	dlg.m_BarPage.m_dbSpace = m_dbFillSpace;
	dlg.m_BarPage.m_dbAngle = m_dbAngle;
	dlg.m_nErrLev = m_nErrLev;
	dlg.m_nHPoint = m_nHPoint;
	dlg.m_nWPoint = m_nWPoint;
	dlg.m_bAutoSet = m_bAutoSet;

	dlg.m_nOutline = m_nOutline;
    dlg.m_nSingleLineMarkCount = m_nSingleLineMarkCount;
    dlg.m_nNarrowLineNum = m_nNarrowLineNum;

	dlg.m_BarPage.m_bPointDataMatrix = m_bPointDataMatrix;
	dlg.m_BarPage.m_nPointRadiusPercent = m_nPointRadiusPercent;

	CText* pText = new CText(1);
	pText->m_bBarCode = TRUE;
	dlg.m_PageText.m_nText		= pText->m_nText;
	dlg.m_PageText.m_strFix		= pText->m_strFix;
	dlg.m_PageText.m_strDate	= pText->m_strDate;
	dlg.m_PageText.m_nJumpStart	= pText->m_nJumpStart;
	dlg.m_PageText.m_nJumpEnd	= pText->m_nJumpEnd;
	dlg.m_PageText.m_strLeft	= pText->m_strLeft;
	dlg.m_PageText.m_strRight	= pText->m_strRight	;
	dlg.m_PageText.m_nJumpLen	= pText->m_nJumpLen	;
	dlg.m_PageText.m_strPrefix	= pText->m_strPrefix;
	dlg.m_PageText.m_nJumpStep	= pText->m_nJumpStep;
	dlg.m_PageText.m_bFill		= pText->m_bFill	;
	dlg.m_PageText.m_nFromFile	= pText->m_nFromFile	;
	dlg.m_PageText.m_strFromFile= pText->m_strFromFile	;
	dlg.m_PageText.m_pSerial	= &(pText->m_saSerial);
	dlg.m_PageText.m_pCur		= &(pText->m_nMarkTimes);
	dlg.m_PageText.m_strBinText	= pText->m_strBinText;

	dlg.m_pText = pText;
	dlg.m_PageText.m_bModel		= pText->m_bModel;
	dlg.m_PageText.m_strModelNo	= pText->m_strModelNo;
	
	if(dlg.DoModal()==IDOK)
	{		
		pText->m_nText			= dlg.m_PageText.m_nText;
		pText->m_strFix			= dlg.m_PageText.m_strFix;
		pText->m_strDate		= dlg.m_PageText.m_strDate;
		pText->m_nJumpStart		= dlg.m_PageText.m_nJumpStart;
		pText->m_nJumpEnd		= dlg.m_PageText.m_nJumpEnd;
		pText->m_strLeft		= dlg.m_PageText.m_strLeft;
		pText->m_strRight		= dlg.m_PageText.m_strRight;
		pText->m_nJumpLen		= dlg.m_PageText.m_nJumpLen;
		pText->m_strPrefix		= dlg.m_PageText.m_strPrefix;
		pText->m_nJumpStep		= dlg.m_PageText.m_nJumpStep;
		pText->m_bFill			= dlg.m_PageText.m_bFill;
		pText->m_nFromFile		= dlg.m_PageText.m_nFromFile;
		pText->m_strFromFile	= dlg.m_PageText.m_strFromFile;
		pText->m_strBinText		= dlg.m_PageText.m_strBinText;

		pText->m_bModel         =dlg.m_PageText.m_bModel;
		pText->m_strModelNo     =dlg.m_PageText.m_strModelNo;
		
		m_bRev			= dlg.m_BarPage.m_bRev		;
        m_nSingleLineMarkCount	= dlg.m_BarPage.m_nSingleLineMarkCount		;
		m_nNarrowLineNum        = dlg.m_BarPage.m_nNarrowLineNum		;
        m_bGoBack		= dlg.m_BarPage.m_bGoBack		;
        m_bConnect		= dlg.m_BarPage.m_bConnect		;
// 		m_bTextVis		= dlg.m_BarPage.m_bShowText ;
		m_dbHeightBar	= dlg.m_BarPage.m_dbHeight		;
		m_narrowW		= dlg.m_BarPage.m_dbNarrow		;
		m_dbshrink		= dlg.m_BarPage.m_dbShrink	;
		m_dbRevWidth	= dlg.m_BarPage.m_dbWidth		;
		m_BarType		= (BARTYPE)dlg.m_nBarType		;
		m_dbFillSpace	= dlg.m_BarPage.m_dbSpace;
		m_dbAngle		= dlg.m_BarPage.m_dbAngle;
		m_nOutline		= dlg.m_nOutline;
		m_nErrLev = dlg.m_nErrLev;
		m_nHPoint = dlg.m_nHPoint;
		m_nWPoint = dlg.m_nWPoint;
		m_bAutoSet = dlg.m_bAutoSet;

		m_bPointDataMatrix = dlg.m_BarPage.m_bPointDataMatrix;
		m_nPointRadiusPercent = dlg.m_BarPage.m_nPointRadiusPercent;
		
		UINT nID = g_MgrText->GetNextID();
		pText->m_nTextID = nID;
		m_nTextID = nID;
		g_MgrText->AddText(pText);

		pText->m_bBarCode = TRUE;
		pText->m_nBarType = m_BarType;		
		pText->FlashNextText(0x00);	//�õ��ִ�����		
		GenBarCode(0x00);		
		CalLimit();
	}
	else
	{
		bResult = FALSE;
		delete pText;
	}
	
	return bResult;
}

BOOL CNodeCodebar::Create(CRect &rect)
{
	CBarCodeDlg dlg;
	
	dlg.m_BarPage.m_bRev = m_bRev;
    dlg.m_BarPage.m_nSingleLineMarkCount = m_nSingleLineMarkCount;
    dlg.m_BarPage.m_nNarrowLineNum = m_nNarrowLineNum;
    dlg.m_BarPage.m_bGoBack = m_bGoBack;
    dlg.m_BarPage.m_bConnect = m_bConnect;
//	dlg.m_BarPage.m_bShowText = m_bTextVis;
	dlg.m_BarPage.m_dbHeight = m_dbHeightBar;
	dlg.m_BarPage.m_dbNarrow = m_narrowW;
	dlg.m_BarPage.m_dbShrink = m_dbshrink;
	dlg.m_BarPage.m_dbWidth = m_dbRevWidth;
	dlg.m_nBarType = (UINT)m_BarType;
	dlg.m_BarPage.m_dbSpace = m_dbFillSpace;
	dlg.m_BarPage.m_dbAngle = m_dbAngle;
	dlg.m_nOutline = m_nOutline;
	dlg.m_nErrLev = m_nErrLev;
	dlg.m_nHPoint = m_nHPoint;
	dlg.m_nWPoint = m_nWPoint;
	dlg.m_bAutoSet = m_bAutoSet;
	
	dlg.m_BarPage.m_bPointDataMatrix = m_bPointDataMatrix;
	dlg.m_BarPage.m_nPointRadiusPercent = m_nPointRadiusPercent;

	CText* pText = g_MgrText->GetAt(m_nTextID);
	dlg.m_PageText.m_nText		= pText->m_nText;
	dlg.m_PageText.m_strFix		= pText->m_strFix;
	dlg.m_PageText.m_strDate	= pText->m_strDate;
	dlg.m_PageText.m_nJumpStart	= pText->m_nJumpStart;
	dlg.m_PageText.m_nJumpEnd	= pText->m_nJumpEnd;
	dlg.m_PageText.m_strLeft	= pText->m_strLeft;
	dlg.m_PageText.m_strRight	= pText->m_strRight	;
	dlg.m_PageText.m_nJumpLen	= pText->m_nJumpLen	;
	dlg.m_PageText.m_strPrefix	= pText->m_strPrefix;
	dlg.m_PageText.m_nJumpStep	= pText->m_nJumpStep;
	dlg.m_PageText.m_bFill		= pText->m_bFill	;
	dlg.m_PageText.m_nFromFile	= pText->m_nFromFile	;
	dlg.m_PageText.m_strFromFile= pText->m_strFromFile	;
	dlg.m_PageText.m_pSerial	= &(pText->m_saSerial);
	dlg.m_PageText.m_pCur		= &(pText->m_nMarkTimes);
	dlg.m_PageText.m_strBinText	= pText->m_strBinText;

	dlg.m_PageText.m_bModel		= pText->m_bModel;
	dlg.m_PageText.m_strModelNo	= pText->m_strModelNo;
	pText->m_bBarCode = TRUE;

	dlg.m_pText = pText;
	
	if(dlg.DoModal()==IDOK)
	{
		pText->m_nText			= dlg.m_PageText.m_nText;
		pText->m_strFix			= dlg.m_PageText.m_strFix;
		pText->m_strDate		= dlg.m_PageText.m_strDate;
		pText->m_nJumpStart		= dlg.m_PageText.m_nJumpStart;
		pText->m_nJumpEnd		= dlg.m_PageText.m_nJumpEnd;
		pText->m_strLeft		= dlg.m_PageText.m_strLeft;
		pText->m_strRight		= dlg.m_PageText.m_strRight;
		pText->m_nJumpLen		= dlg.m_PageText.m_nJumpLen;
		pText->m_strPrefix		= dlg.m_PageText.m_strPrefix;
		pText->m_nJumpStep		= dlg.m_PageText.m_nJumpStep;
		pText->m_bFill			= dlg.m_PageText.m_bFill;
		pText->m_nFromFile		= dlg.m_PageText.m_nFromFile;
		pText->m_strFromFile	= dlg.m_PageText.m_strFromFile;
		pText->m_strBinText		= dlg.m_PageText.m_strBinText;
		pText->m_bBarCode		= TRUE;
		pText->m_bModel         =dlg.m_PageText.m_bModel;
		pText->m_strModelNo     =dlg.m_PageText.m_strModelNo;

		m_bRev			= dlg.m_BarPage.m_bRev		;
        m_nSingleLineMarkCount			= dlg.m_BarPage.m_nSingleLineMarkCount		;
        m_nNarrowLineNum			= dlg.m_BarPage.m_nNarrowLineNum		;
        m_bGoBack		= dlg.m_BarPage.m_bGoBack		;
        m_bConnect		= dlg.m_BarPage.m_bConnect		;
//		m_bTextVis		= dlg.m_BarPage.m_bShowText ;
		m_dbHeightBar	= dlg.m_BarPage.m_dbHeight		;
		m_narrowW		= dlg.m_BarPage.m_dbNarrow		;
		m_dbshrink		= dlg.m_BarPage.m_dbShrink	;
		m_dbRevWidth	= dlg.m_BarPage.m_dbWidth		;
		m_BarType		= (BARTYPE)dlg.m_nBarType		;
		m_dbFillSpace	= dlg.m_BarPage.m_dbSpace;
		m_dbAngle		= dlg.m_BarPage.m_dbAngle;
		m_nOutline		= dlg.m_nOutline;
		m_nErrLev = dlg.m_nErrLev;
		m_nHPoint = dlg.m_nHPoint;
		m_nWPoint = dlg.m_nWPoint;
		m_bAutoSet = dlg.m_bAutoSet;
		
		m_bPointDataMatrix = dlg.m_BarPage.m_bPointDataMatrix;
		m_nPointRadiusPercent = dlg.m_BarPage.m_nPointRadiusPercent;

		pText->m_nBarType = m_BarType;
		pText->FlashNextText(0x00);	//�õ��ִ�����
		GenBarCode(0x00);
	}
	SetSelect(TRUE);
	CalLimit();

	return TRUE;
}

BOOL CNodeCodebar::PreprocessMark(DWORD nStyle)
{
	//����ԭ����NSTYLE�ڴ�ʱ�������ݵ����ţ��ָ�Ϊ
	//��CHAIN��ͳһ�����ı����ݵ�����ת�������ı��������в���仯
	GenBarCode(0x00);
	CalLimit();

	return TRUE;
}

BOOL CNodeCodebar::GetBarcode()
{
	return TRUE;
}

void CNodeCodebar::DeleteFill()
{
	POSITION prev;
	POSITION pos;
	CStroke *pStroke;
	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)
	{	
		prev=pos;
		pStroke=m_listFill.GetNext(pos);

		m_listFill.RemoveAt(prev);
		delete pStroke;
	}
}

CQuad CNodeCodebar::GetMarkRange()
{
	CQuad quad;
    quad.SetEmpty();
	CStroke *pStroke;
	POSITION pos;
	
	pos=m_list.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_list.GetNext(pos);
		quad.Union(pStroke->CalLimit());
	}
	quad.Normal();
	return quad;
}

/*
nStyle=0x00ʱ��������һ������
0x01ʱ������һ����������
*/
BOOL CNodeCodebar::GenBarCode(UINT nStyle)
{
	BOOL bRtn = TRUE;
	switch(m_BarType) 
	{
	case EAN8:
		{
			//���֣�λ��ֻ��Ϊ7λ��1��У��λ
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();

			int nLen = strText.GetLength();
			if(nLen>7){
				strText.SetAt(7,TCHAR(0X00));
			}
			else{
				for(int i=0;i<7-nLen;i++)
					strText.Insert(0,'0');
			}

			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Ean8;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case EAN13:
		{
			//���֣�λ��ֻ��Ϊ12λ��1��У��λ
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();

			int nLen = strText.GetLength();
			if(nLen>7){
				strText.SetAt(12,TCHAR(0X00));
			}
			else{
				for(int i=0;i<12-nLen;i++)
					strText.Insert(0,'0');
			}

			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Ean13;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case EAN128:
		{
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Ean128;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case CODE128A:
		{
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			strText.Insert(0,TCHAR(0xF1));
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case CODE128B:
		{
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			strText.Insert(0,TCHAR(0xF2));
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case CODE128C:
		{
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			strText.Insert(0,TCHAR(0xF3));
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code128;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case CODE128AUTO:
		{
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code128Auto;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case CODE39:
		{
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code39;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			iCodeBar1._s_value._uExtraInput._dw39 = 0;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case INT25:
		{
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Inter25;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			iCodeBar1._s_value._uExtraInput._dw25 = 0;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case EXT39:
		{
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_Code39Ex;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			iCodeBar1._s_value._uExtraInput._dw39 = 0;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case UPCA:
		{
			//���֣�λ��ֻ��Ϊ11λ��1��У��λ
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();

			int nLen = strText.GetLength();
			if(nLen>7){
				strText.SetAt(11,TCHAR(0X00));
			}
			else{
				for(int i=0;i<11-nLen;i++)
					strText.Insert(0,'0');
			}

			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_UpcA;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case UPCE:
		{
			//���֣�λ��ֻ��Ϊ6λ��2��У��λ
			BYTE szOutString[256];
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();

			int nLen = strText.GetLength();
			if(nLen>7){
				strText.SetAt(6,TCHAR(0X00));
			}
			else{
				for(int i=0;i<6-nLen;i++)
					strText.Insert(0,'0');
			}

			ICODEBAR1 iCodeBar1;
			iCodeBar1._type = eCodebar1Dim_UpcE;
			iCodeBar1._s_value._i_lpcbSrc = (BYTE*)((LPCTSTR)strText);
			iCodeBar1._s_value._i_nSrcSize = strText.GetLength();
			iCodeBar1._s_value._i_lpcbRetBinaryBuff = m_pByte;
			iCodeBar1._s_value._i_nRetBinaryBuffSize = 256*256;
			iCodeBar1._s_value._i_lpcbRetStrBuff = szOutString;
			iCodeBar1._s_value._i_nRetStrBuffSize = 256;
			
			int nRtn = g_BarCodeMgr.m_BarCode1.GenCodebar(&iCodeBar1);
			m_nCol = iCodeBar1._s_value._o_nRltBinarySize;
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode1.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = 1;
			break;
		}
	case PDF417:
		{
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			IMATRIXBAR iMatrix;
			iMatrix._type = 3;
 			iMatrix._u_value._pdf417._bits._bConver=0;
			iMatrix._u_value._pdf417._bits._countsConver=0;
			iMatrix._u_value._pdf417._bits._eccLevel=1;
			iMatrix._u_value._pdf417._bits._nCols = 0;		
			iMatrix._u_value._pdf417._bits._nRows = m_nOutline;		//
			iMatrix._u_value._pdf417._xScale = 1;
			iMatrix._u_value._pdf417._yScale = 1;
			iMatrix._u_value._pdf417._sizeSrcBytes = strText.GetLength();
			iMatrix._u_value._pdf417._lpszSrc = (BYTE*)((LPCTSTR)strText);
			iMatrix._u_value._pdf417._lpszRetBuffer = m_pByte;
			iMatrix._u_value._pdf417._sizeRetBuffer = 256*256;

			int nRtn = g_BarCodeMgr.m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode2.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = iMatrix._u_value._pdf417._sizeRetRows;
			m_nCol = iMatrix._u_value._pdf417._sizeRetCols;
			break;
		}
	case DATAMATRIX:
		{
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			IMATRIXBAR iMatrix;
			iMatrix._type = 2;
			iMatrix._u_value._dataMatrix._bits._bConver =0;
			iMatrix._u_value._dataMatrix._bits._countsConver = 0;
			iMatrix._u_value._dataMatrix._bits._bUserDefined=m_bAutoSet;	//
			iMatrix._u_value._dataMatrix._bits._eccType =m_nErrLev;			//
			iMatrix._u_value._dataMatrix._bits._outStyle=m_nOutline;		//
			iMatrix._u_value._dataMatrix._heightNums = m_nHPoint;			//
			iMatrix._u_value._dataMatrix._widthNums = m_nWPoint;			//
			iMatrix._u_value._dataMatrix._lpszSrc = (BYTE*)((LPCTSTR)strText);
			iMatrix._u_value._dataMatrix._sizeSrcBytes = strText.GetLength();
			iMatrix._u_value._dataMatrix._lpszRetBuffer = m_pByte;
			iMatrix._u_value._dataMatrix._sizeRetBuffer = 256*256;
			
			int nRtn = g_BarCodeMgr.m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if(nRtn!=0){
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode2.GetErrInfo(nRtn,g_LangID));
			}
			m_nRow = iMatrix._u_value._dataMatrix._sizeRetRows;
			m_nCol = iMatrix._u_value._dataMatrix._sizeRetCols;
			break;
		}
	case QR:
		{
			CText* pText = g_MgrText->GetAt(m_nTextID);
			CString strText = pText->GetBarString();
			IMATRIXBAR iMatrix;
			iMatrix._type = 1;
			iMatrix._u_value._qr._bits._bConver=0;
			iMatrix._u_value._qr._bits._countsConver =0;
			iMatrix._u_value._qr._bits._levelType=m_nErrLev;	//
			iMatrix._u_value._qr._bits._versionType=m_nHPoint;	//
			iMatrix._u_value._qr._lpszRetBuffer = m_pByte;
			iMatrix._u_value._qr._lpszSrc = (BYTE*)((LPCTSTR)strText);
			iMatrix._u_value._qr._sizeRetBuffer = 256*256;
			iMatrix._u_value._qr._sizeSrcBytes = strText.GetLength();

			int nRtn = g_BarCodeMgr.m_BarCode2.GenCodebar((LPVOID)(&iMatrix));
			if(nRtn!=0)
			{
				bRtn = FALSE;
				AfxMessageBox(g_BarCodeMgr.m_BarCode2.GetErrInfo(nRtn, g_LangID));
			}
			m_nRow = iMatrix._u_value._qr._sizeRetRowsOrCols;
			m_nCol = iMatrix._u_value._qr._sizeRetRowsOrCols;
			break;
		}
	}
	
	if(bRtn)
	{
		CreateStrokeList();
	}
	
    return bRtn;
}

BOOL CNodeCodebar::CreateStrokeList()
{
	ClearBarList();
	
	BOOL bFlag = FALSE;

	if(m_nRow>1)
	{
		if (m_bPointDataMatrix)
			bFlag = CreateStrokeListPoint();
		else
			bFlag = CreateStrokeList2();
	}
	else
	{
		bFlag = CreateStrokeList1();

		// �Ż�����
		if (m_bGoBack) 
		{
			BetterOneCodeBar();
		}

		// ���Ӵ���
		if (m_bConnect) 
		{
			ConnectOneCodeBar();
		}

		AddToList();
	}
	
	//����ת���Ƕ�
//	double dbAngle = m_dbAngle*PAI/180;
	double dbAngle = m_dbAngle;
	m_dbAngle=0;
	Rotate(m_dotBase,dbAngle);
	CQuad quadNew = CalLimit();
	CDot dotNew = quadNew.GetBaseDot();
	CNode::Move(m_dotBase.x-dotNew.x,m_dotBase.y-dotNew.y);

	ClearBarList();

	return bFlag;
}


BOOL CNodeCodebar::CreateStrokeList1()
{
	Delete();
	DeleteFill();

	//һά���뽨������
	double dbWidth    = m_dbHeightBar;	//��ά������
	double dbOffset   = m_dbFillSpace;
	double dbShrink   = m_dbshrink;	    //������
	BOOL bRev         = m_bRev;			//�Ƿ���
    BOOL bGoBack      = m_bGoBack;		//������
    BOOL bConnect     = m_bConnect;		//������
	double dbRevWidth = m_dbRevWidth;	//������
	double dbNarrow   = m_narrowW;		//�����
	double dbAOffset  = 0;				//����������������ľ��������
	CStroke* pStroke  = NULL;
	UINT nRevNum      = (UINT)(dbRevWidth / dbOffset);
	dbRevWidth        = nRevNum*dbOffset;
	
	switch(m_BarType)
	{
	case EAN8:
	case EAN13:
	case UPCA:
	case UPCE:
	case EAN128:
	case CODE128A:
	case CODE128B:
	case CODE128C:
	case CODE128AUTO:
		{
			BYTE tmp;
			BOOL bBack;
			int  nBack=0;
			double dbCurX;
			BOOL bFirst=TRUE;
			int  nS = 0;
			CDot dottmp,dot;
			BOOL bLong=FALSE;

			if(bRev)
			{
				StrokeList *pStrkList = new StrokeList;
				if (NULL == pStrkList) 
				{
					return FALSE;
				}

				//����ʱ�Ӹ��߽�
				for(UINT n=0;n<nRevNum;n++)
				{
					pStroke = new CStroke(m_property);
					//m_list.AddTail(pStroke);
					pStrkList->AddTail(pStroke);
					
					dottmp.x = -dbRevWidth - dbShrink +n*dbOffset;
					dottmp.y = 0;
					dot = dottmp;
					pStroke->Add(dot);
					dottmp.y = dbWidth;
					dot = dottmp;
					pStroke->Add(dot);
				}

				m_CodeBarList.Add(pStrkList);
			}

			for(UINT i=0;i<m_nCol;i++)
			{
				tmp   = *(m_pByte+i);
				bBack = (tmp&0x80)?TRUE:FALSE;
				bBack = bRev?(!bBack):bBack;
				if(bBack)
				{
					if(bFirst)
					{
						nS = i;
						bFirst = FALSE;
					}
					nBack++;
					bLong = (tmp&0x40);
				}
				else
				{
					if(nBack>1)
					{
						StrokeList *pStrkList = new StrokeList;
						if (NULL == pStrkList) 
						{
							return FALSE;
						}

						//������
						double dbBack = nBack*dbNarrow*1.4;
						int nFillNum = (int)((dbBack-dbShrink*2)/dbOffset + 0.6) + 1;
						nFillNum=nFillNum<=(int)m_nSingleLineMarkCount?(int)m_nSingleLineMarkCount:nFillNum;
						double dbGoStep = (dbBack-dbShrink*2) / (nFillNum - 1);
						if (nFillNum <= 1) {
							dbGoStep = 0;
						}
						dbCurX = nS*dbNarrow*1.4+dbShrink;

						if (nFillNum > 1) 
						{
							for(int j=0; j < nFillNum; j++)
							{
								pStroke = new CStroke(m_property);
								pStrkList->AddTail(pStroke);

								dottmp.x = dbCurX+j*dbGoStep;
								dottmp.y = bLong?-m_dbSpace:0;
								dot = dottmp;
								pStroke->Add(dot);
								dottmp.y = dbWidth;
								dot = dottmp;
								pStroke->Add(dot);
							}
						}
						else
						{
							pStroke = new CStroke(m_property);
							pStrkList->AddTail(pStroke);
							
							dottmp.x = dbCurX;
							dottmp.y = bLong?-m_dbSpace:0;
							dot = dottmp;
							pStroke->Add(dot);
							dottmp.y = dbWidth;
							dot = dottmp;
							pStroke->Add(dot);

							pStroke = new CStroke(m_property);
							pStrkList->AddTail(pStroke);
							
							dottmp.x = (nS + nBack) * dbNarrow * 1.4 - dbShrink;
							dottmp.x = dottmp.x>dbCurX?dbCurX:dottmp.x;
							dottmp.y = bLong?-m_dbSpace:0; 
							dot = dottmp;
							pStroke->Add(dot);
							dottmp.y = dbWidth;
							dot = dottmp;
							pStroke->Add(dot);

							
						}
						m_CodeBarList.Add(pStrkList);
					}
					else if (1 == nBack)
					{
						StrokeList *pStrkList = new StrokeList;
						if (NULL == pStrkList) 
						{
							return FALSE;
						}

						if (m_nNarrowLineNum < 2)
						{
							m_nNarrowLineNum = 2;
						}

						int nNarrowLineCnt = m_nNarrowLineNum;

						//������
						double dbBack = nBack*dbNarrow*1.4;
						int nLineCnt = (int)((dbBack-dbShrink*2) / dbOffset + 0.6) + 1;
						if (nLineCnt >= (int)nNarrowLineCnt ) 
						{
							nNarrowLineCnt = nLineCnt;
						}

						double dbGoStep = ((dbBack-dbShrink*2) / (nNarrowLineCnt - 1) );
						dbCurX = nS*dbNarrow*1.4+dbShrink;
					
						for(int j= 0; j < nNarrowLineCnt; j++)
						{
							pStroke = new CStroke(m_property);
							pStrkList->AddTail(pStroke);

							dottmp.x = dbCurX+j*dbGoStep;
							dottmp.y = bLong?-m_dbSpace:0;
							dot = dottmp;
							pStroke->Add(dot);
							dottmp.y = dbWidth;
							dot = dottmp;
							pStroke->Add(dot);
						}

						m_CodeBarList.Add(pStrkList);
					}
					
					nBack=0;
					bFirst=TRUE;
					bLong=FALSE;
				}
			}
			if(nBack>0)
			{
				//������
				double dbBack = nBack*dbNarrow*1.4;
				int nFillNum  = (int)((dbBack-dbShrink*2)/dbOffset + 0.6) + 1;
				nFillNum=nFillNum<=(int)m_nSingleLineMarkCount?(int)m_nSingleLineMarkCount:nFillNum;
				double dbGoStep = (dbBack-dbShrink*2) / (nFillNum - 1);
				if (nFillNum <= 1) {
					dbGoStep = 0;
				}
//				dbAOffset     = (dbBack-dbShrink*2)/nFillNum;
				dbCurX        = nS*dbNarrow*1.4+dbShrink;

				StrokeList *pStrkList = new StrokeList;
				if (NULL == pStrkList) 
				{
					return FALSE;
				}

				for(int j=0; j < nFillNum; j++)
				{
					pStroke = new CStroke(m_property);
					pStrkList->AddTail(pStroke);

//					dottmp.x = dbCurX+j*dbAOffset;
					dottmp.x = dbCurX+j*dbGoStep;
					dottmp.y = bLong?-m_dbSpace:0;
					dot      = dottmp;
					pStroke->Add(dot);
					dottmp.y = dbWidth;
					dot      = dottmp;
					pStroke->Add(dot);
				}
				
				m_CodeBarList.Add(pStrkList);
			}

			if(bRev)
			{
				StrokeList *pStrkList = new StrokeList;
				if (NULL == pStrkList) 
				{
					return FALSE;
				}
				
				//����ʱ�Ӹ��߽�
				dbCurX = m_nCol*dbNarrow*1.4+dbShrink;

				for(UINT n=0;n<nRevNum;n++)
				{
					pStroke = new CStroke(m_property);
//					m_list.AddTail(pStroke);
                    pStrkList->AddTail(pStroke);

					dottmp.x = dbCurX+n*dbOffset;
					dottmp.y = 0;
					dot      = dottmp;
					pStroke->Add(dot);
					dottmp.y = dbWidth;
					dot      = dottmp;
					pStroke->Add(dot);
				}
				m_CodeBarList.Add(pStrkList);
			}

			break;
		}
	case CODE39:
	case INT25:
	case EXT39:
		{
			BYTE tmp;
			BOOL bBack=TRUE;
			double dbCurX=0;
			CDot dottmp,dot;

			if(bRev)
			{
				StrokeList *pStrkList = new StrokeList;
				if (NULL == pStrkList) 
				{
					return FALSE;
				}

				//����ʱ�Ӹ��߽�
				for(UINT n=0;n<nRevNum;n++)
				{
					pStroke = new CStroke(m_property);
//					m_list.AddTail(pStroke);
					pStrkList->AddTail(pStroke);

					dottmp.x = -dbRevWidth - dbShrink +n*dbOffset;
					dottmp.y = 0;
					dot      = dottmp;
					pStroke->Add(dot);
					dottmp.y = dbWidth;
					dot      = dottmp;
					pStroke->Add(dot);
				}
				m_CodeBarList.Add(pStrkList);
				bBack = FALSE;
			}
			
			for(UINT i=0;i<m_nCol;i++)
			{
				tmp = *(m_pByte+i);
				tmp = tmp&0x0F;
				
				if(tmp==0x01)
				{
					//խ��
					double dbBack = m_narrowW;
					int nFillNum = (int)((dbBack-dbShrink*2)/dbOffset + 0.6);
					//nFillNum=nFillNum>0?nFillNum:1;
					nFillNum=nFillNum<=(int)m_nSingleLineMarkCount?(int)m_nSingleLineMarkCount:nFillNum;
					dbAOffset = (dbBack-dbShrink*2)/nFillNum;

					if(bBack)
					{
						StrokeList *pStrkList = new StrokeList;
						if (NULL == pStrkList) 
						{
							return FALSE;
						}

						for(int j = 0; j < nFillNum; j++)
						{
							pStroke = new CStroke(m_property);
//							m_list.AddTail(pStroke);
							pStrkList->AddTail(pStroke);
							
							dottmp.x = dbCurX+j*dbAOffset;
							dottmp.y = 0;
							dot      = dottmp;
							pStroke->Add(dot);
							dottmp.y = dbWidth;
							dot      = dottmp;
							pStroke->Add(dot);
						}
						m_CodeBarList.Add(pStrkList);
					}
					dbCurX += dbBack;
				}
				else if(tmp==0x00)
				{
					//λ��
					double dbBack = m_narrowW;
					int nFillNum  = (int)((dbBack-dbShrink*2)/dbOffset);
					//nFillNum      = nFillNum>0?nFillNum:1;
					nFillNum=nFillNum<=(int)m_nSingleLineMarkCount?(int)m_nSingleLineMarkCount:nFillNum;
					dbAOffset     = (dbBack-dbShrink*2)/nFillNum;
					if(bBack)
					{
						StrokeList *pStrkList = new StrokeList;
						if (NULL == pStrkList) 
						{
							return FALSE;
						}

						for(int j=0;j<nFillNum;j++)
						{
							pStroke = new CStroke(m_property);
//							m_list.AddTail(pStroke);
							pStrkList->AddTail(pStroke);
							
							dottmp.x = dbCurX+j*dbAOffset;
							dottmp.y = 0;
							dot      = dottmp;
							pStroke->Add(dot);
							dottmp.y = dbWidth;
							dot      = dottmp;
							pStroke->Add(dot);
						}
						m_CodeBarList.Add(pStrkList);
					}
					dbCurX += dbBack;
				}
				else
				{
					//����
					double dbBack = m_dbcodeRate*m_narrowW;
					int nFillNum  = (int)((dbBack-dbShrink*2)/dbOffset);
					//nFillNum      = nFillNum>0?nFillNum:1;
					nFillNum=nFillNum<=(int)m_nSingleLineMarkCount?(int)m_nSingleLineMarkCount:nFillNum;
					dbAOffset     = (dbBack-dbShrink*2)/nFillNum;
					if(bBack)
					{
						StrokeList *pStrkList = new StrokeList;
						if (NULL == pStrkList) 
						{
							return FALSE;
						}

						for(int j=0;j<nFillNum;j++)
						{
							pStroke = new CStroke(m_property);
//							m_list.AddTail(pStroke);
							pStrkList->AddTail(pStroke);
							
							dottmp.x = dbCurX+j*dbAOffset;
							dottmp.y = 0;
							dot      = dottmp;
							pStroke->Add(dot);
							dottmp.y = dbWidth;
							dot      = dottmp;
							pStroke->Add(dot);
						}
						m_CodeBarList.Add(pStrkList);
					}
					dbCurX += dbBack;
				}
				
				//��һ�����෴
				bBack=!bBack;
			}

			if(bRev)
			{
				StrokeList *pStrkList = new StrokeList;
				if (NULL == pStrkList) 
				{
					return FALSE;
				}
				//����ʱ�Ӹ��߽�
				for(UINT n=0;n<nRevNum;n++)
				{
					pStroke = new CStroke(m_property);
//					m_list.AddTail(pStroke);
					pStrkList->AddTail(pStroke);

					dottmp.x = dbCurX+n*dbOffset;
					dottmp.y = 0;
					dot      = dottmp;
					pStroke->Add(dot);
					dottmp.y = dbWidth;
					dot      = dottmp;
					pStroke->Add(dot);
				}
				m_CodeBarList.Add(pStrkList);
			}
			
			break;
		}
	}
	
	return TRUE;
}

BOOL CNodeCodebar::CreateStrokeList2()
{
	//��ά���뽨������
	//ɾ����Ч����
	Delete();
	DeleteFill();
	
	BYTE byte;
	BOOL bEnter;
	UINT nS,nE;
	CUIntArray nArray;
	CDot dot,tmp;
	CStroke* pStroke;

	double dbBlockH = m_dbHeightBar/m_nRow;	//��ά������
	double dbOffset = m_dbFillSpace;
	double dbShrink = m_dbshrink;	//������
	BOOL bRev = m_bRev;				//�Ƿ���
    BOOL bGoBack    = m_bGoBack;	//������
    BOOL bConnect   = m_bConnect;	//������
	BOOL bLoop = TRUE;				//�������
	double dbRevWidth = m_dbRevWidth;	//������
	BYTE s,e;	//��׼�㣬���ڷ���ʱȷ��λ��
	s=bRev?0:1;
	e=bRev?1:0;

	UINT   nFillNum = (UINT)((dbBlockH-m_dbshrink*2)/dbOffset+0.5);	
	nFillNum = max(1,nFillNum);
	dbOffset = (dbBlockH-m_dbshrink*2)/nFillNum;

	UINT nRevNum = (UINT)((dbRevWidth-m_dbshrink*2)/dbOffset+0.5);
	nRevNum = max(1,nRevNum);
	dbRevWidth   = nRevNum*dbOffset+2*m_dbshrink;

	//DEBUG START
	{
		DSF_TRACE("codebar start\n");
		CString str;

		for(UINT i=0;i<m_nCol;i++)
		{
			for(UINT j=0;j<m_nRow;j++)
			{
				byte = *(m_pByte+j*m_nCol+i);
				if(byte == 0)
					str += "0";
				else
					str += "1";
			}
			str += "\n";
			DSF_TRACE(str);
		}
		DSF_TRACE("codebar end\n");
	}
	//DEBUG END

	for(UINT i=0;i<m_nCol;i++)
	{
		BOOL bS,bE;
		bE = bS = FALSE;
		bEnter=FALSE;
		nArray.RemoveAll();
		for(UINT j=0;j<m_nRow;j++)
		{
			byte = *(m_pByte+j*m_nCol+i);
			if((s==byte)&&(FALSE==bEnter))
			{
				bEnter=TRUE;
				nS = j;
			}
			else if((e==byte)&&bEnter)
			{
				bEnter=FALSE;
				nE = j;
				nArray.Add(nS);
				nArray.Add(nE);
			}
		}
		
		if(bEnter)
		{
			nE = m_nRow;
			nArray.Add(nS);
			nArray.Add(nE);
		}
		//ͳһ������׼����
		//��������ʱ��0,0��Ϊ��׼��

		if(bRev && i==0)
		{
			//�������ͼ
			for(UINT n=0;n<nRevNum;n++)
			{
				pStroke = new CStroke(m_property);
				m_list.AddTail(pStroke);
				
				if(bLoop&&(n%2))
				{
					tmp.x = -dbRevWidth+n*dbOffset;
					tmp.y = -(m_nRow*dbBlockH+dbRevWidth);
					dot = tmp;
					pStroke->Add(dot);
					tmp.y = dbRevWidth;
					dot = tmp;
					pStroke->Add(dot);
				}
				else
				{
					tmp.x = -dbRevWidth+n*dbOffset;
					tmp.y = dbRevWidth;
					dot = tmp;
					pStroke->Add(dot);
					tmp.y = -(m_nRow*dbBlockH+dbRevWidth);
					dot = tmp;
					pStroke->Add(dot);
				}
			}
		}

		if(bRev&&nArray.GetSize()==0)
		{
			for(UINT n=0;n<nFillNum;n++)
			{
				pStroke = new CStroke(m_property);
				m_list.AddTail(pStroke);
				
				if(bLoop&&(n%2)){
					tmp.x = i*dbBlockH+dbShrink+n*dbOffset;
					tmp.y = 0;
					dot = tmp;
					pStroke->Add(dot);
					tmp.y = dbRevWidth;
					dot = tmp;
					pStroke->Add(dot);
				}
				else{
					tmp.x = i*dbBlockH+dbShrink+n*dbOffset;
					tmp.y = dbRevWidth;
					dot = tmp;
					pStroke->Add(dot);
					tmp.y = 0;
					dot = tmp;
					pStroke->Add(dot);
				}
			}
		}
		
		int nSize = nArray.GetSize();
		for(int m=0;m<nSize/2;m++)
		{
			nS = nArray.GetAt(m*2);
			nE = nArray.GetAt(m*2+1);
			if(m==0&&nS!=0&&bRev){
				//���ϱ߿�
				for(UINT n=0;n<nFillNum;n++)
				{
					pStroke = new CStroke(m_property);
					m_list.AddTail(pStroke);

					if(bLoop&&(n%2)){
						tmp.x = i*dbBlockH+dbShrink+n*dbOffset;
						tmp.y = 0;
						dot = tmp;
						pStroke->Add(dot);
						tmp.y = dbRevWidth;
						dot = tmp;
						pStroke->Add(dot);
					}
					else{
						tmp.x = i*dbBlockH+dbShrink+n*dbOffset;
						tmp.y = dbRevWidth;
						dot = tmp;
						pStroke->Add(dot);
						tmp.y = 0;
						dot = tmp;
						pStroke->Add(dot);
					}
				}
				bS = TRUE;
			}
			
			for(UINT n=0;n<nFillNum;n++)
			{
				pStroke = new CStroke(m_property);
				m_list.AddTail(pStroke);

				if(bLoop&&(n%2)){
					tmp.x = i*dbBlockH+dbShrink+n*dbOffset;
					if(nE==m_nRow&&bRev){
						tmp.y = -(m_nRow*dbBlockH+dbRevWidth);
						bE = TRUE;
					}
					else
						tmp.y = -(nE*dbBlockH-dbShrink);
					dot = tmp;
					pStroke->Add(dot);

					if(nS==0&&bRev){
						tmp.y = dbRevWidth;
					}
					else
						tmp.y = -(nS*dbBlockH+dbShrink);
					dot = tmp;
					pStroke->Add(dot);
				}
				else
				{
					tmp.x = i*dbBlockH+dbShrink+n*dbOffset;
					if(nS==0&&bRev){
						tmp.y = dbRevWidth;
					}
					else
						tmp.y = -(nS*dbBlockH+dbShrink);
					dot = tmp;
					pStroke->Add(dot);
					if(nE==m_nRow&&bRev){
						tmp.y = -(m_nRow*dbBlockH+dbRevWidth);
						bE = TRUE;
					}
					else
						tmp.y = -(nE*dbBlockH-dbShrink);
					dot = tmp;
					pStroke->Add(dot);
				}
			}
		}

		if(bE==FALSE&&bRev)
		{
			for(UINT n=0;n<nFillNum;n++)
			{
				pStroke = new CStroke(m_property);
				m_list.AddTail(pStroke);

				if(bLoop&&(n%2))
				{
					tmp.x = i*dbBlockH+dbShrink+n*dbOffset;
					tmp.y = -(m_nRow*dbBlockH+dbRevWidth);
					dot = tmp;
					pStroke->Add(dot);
					tmp.y = -(m_nRow*dbBlockH+dbShrink);
					dot = tmp;
					pStroke->Add(dot);
				}
				else
				{
					tmp.x = i*dbBlockH+dbShrink+n*dbOffset;
					tmp.y = -(m_nRow*dbBlockH+dbShrink);
					dot = tmp;
					pStroke->Add(dot);
					tmp.y = -(m_nRow*dbBlockH+dbRevWidth);
					dot = tmp;
					pStroke->Add(dot);
				}
			}
		}

		if(bRev && i==m_nCol-1)
		{
			//�������ͼ
			for(UINT n=0;n<nRevNum;n++)
			{
				pStroke = new CStroke(m_property);
				m_list.AddTail(pStroke);

				if(bLoop&&(n%2))
				{
					tmp.x = (m_nCol)*dbBlockH+n*dbOffset;
					tmp.y = -(m_nRow*dbBlockH+dbRevWidth);
					dot = tmp;
					pStroke->Add(dot);
					tmp.y = dbRevWidth;
					dot = tmp;
					pStroke->Add(dot);
				}
				else
				{
					tmp.x = (m_nCol)*dbBlockH+n*dbOffset;
					tmp.y = dbRevWidth;
					dot = tmp;
					pStroke->Add(dot);
					tmp.y = -(m_nRow*dbBlockH+dbRevWidth);
					dot = tmp;
					pStroke->Add(dot);
				}
			}
		}
	}
	
	return TRUE;
}

void CNodeCodebar::AddToPlt(CNode *pNodeplt)
{
	if (NULL == pNodeplt) 
	{
		return;
	}

	POSITION pos=m_list.GetHeadPosition();
	CStroke *pStroke,*pNew;
	while(pos)
	{
		pStroke=m_list.GetNext(pos);
		pNew=pStroke->Copy();
		(pNodeplt->GetList()).AddTail(pNew);
	}

	pNodeplt->m_property = m_property;
}


CNode * CNodeCodebar::Copy(UINT nStyle)
{
	CNodeCodebar *tmp = new CNodeCodebar();

	tmp->m_bRev = m_bRev;
	tmp->m_dbRevWidth = m_dbRevWidth;
	tmp->m_BarType = m_BarType;
	tmp->m_dbFillSpace = m_dbFillSpace;

	tmp->m_type			= m_type;
	tmp->m_bAutoSet		= m_bAutoSet;
	tmp->m_dbHeightBar 	= m_dbHeightBar ;
	tmp->m_dbshrink		= m_dbshrink;
	tmp->m_nOutline		= m_nOutline;
	tmp->m_nErrLev		= m_nErrLev;
	tmp->m_bTextVis		= m_bTextVis;
	tmp->m_nAlign		= m_nAlign;
	tmp->m_narrowW		= m_narrowW;
	tmp->m_bSelect		= m_bSelect;
	tmp->m_nHPoint		= m_nHPoint;
	tmp->m_nWPoint		= m_nWPoint;
	tmp->m_dotBase		= m_dotBase;
    tmp->m_dbAngle		= m_dbAngle;


    tmp->m_bGoBack		= m_bGoBack;
	tmp->m_bConnect		= m_bConnect;
	tmp->m_nSingleLineMarkCount	= m_nSingleLineMarkCount;

	tmp->m_property		= m_property;

	tmp->m_bPointDataMatrix = m_bPointDataMatrix;
	tmp->m_nPointRadiusPercent = m_nPointRadiusPercent;

	//////////////////����ʱ�Զ�����
	if(0x00 == nStyle)
	{
		CText* pText;
		UINT nID = g_MgrText->GetNextID();
		pText = new CText(0);
		*pText = *(g_MgrText->GetAt(m_nTextID));
		pText->m_nTextID = nID;
		if(g_dwJumpCnt != -1)
		{
			pText->m_nJumpStart += g_dwJumpCnt;
		}

		g_MgrText->AddText(pText);
		tmp->m_nTextID = nID;

		// ������ʱ�������Զ�����
		CString str;
		g_MgrText->FlashText(nID,0x00);
		g_MgrText->GetString(nID,str);
		pText->m_strText = str;
	}
	else
	{
		tmp->m_nTextID = m_nTextID;
	}
	
	tmp->GenBarCode(0x00);
	tmp->CalLimit();

	return tmp;
}

BOOL CNodeCodebar::ClearBarList()
{
	int nCount = m_CodeBarList.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		StrokeList *pSList = m_CodeBarList.GetAt(i);
		if (NULL == pSList)
		{
			continue;
		}

		delete pSList;
		pSList = NULL;
	}

	m_CodeBarList.RemoveAll();

	return TRUE;
}

double CNodeCodebar::Distance(CDot &sDot, CDot &eDot)
{
	double dbD = sqrt( (sDot.x - eDot.x) * (sDot.x - eDot.x) +  (sDot.y - eDot.y) *(sDot.y - eDot.y) );
	return dbD;
}

BOOL CNodeCodebar::MinDsIsFirstDot(CDot &dt, CStroke *pStrk)
{
	if (NULL == pStrk)
	{
		return FALSE;
	}

	int nCount = pStrk->GetSize();
	if (nCount <= 1) 
	{
		return FALSE;
	}

	CDot sDt = pStrk->GetAt(0);
	CDot eDt = pStrk->GetAt(nCount -1);

	if (Distance(dt, sDt) < Distance(dt, eDt))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CNodeCodebar::ProOneStroke(CDot &dt, CStroke *pStrk)
{
	if (!pStrk)
	{
		return FALSE;
	}

	BOOL bRet = MinDsIsFirstDot(dt, pStrk);
	if (!bRet) 
	{
		pStrk->Reverse();
	}

	return TRUE;
}

BOOL CNodeCodebar::BetterOneCodeBar()
{
	int nCount = m_CodeBarList.GetSize();
	if (nCount <= 0) 
	{
		return FALSE;
	}

	// ȡ�õ�һ���ߣ����һ����ı�־
	BOOL bIsGetFirstDt = FALSE;

	CDot lsDt;

	//
	//�������Ƶ����л�����
/*	CString strPp("C:\\CodeInfo.txt");
	FILE *fp;	
	if( (fp = fopen(strPp,"wb")) != NULL )
	{
	}

	CMySerialize fs(fp);
*/	
	//

	for (int i = 0; i < nCount; i++)
	{
		StrokeList *pSList = m_CodeBarList.GetAt(i);
		if (NULL == pSList)
		{
			continue;
		}

		//
/*		POSITION Head = pSList->GetHeadPosition();
		CStroke *p1 = pSList->GetAt(Head);
		CDot dt1 = p1->GetAt(0);
		fs<<dt1.x;
		POSITION Tail = pSList->GetTailPosition();
		CStroke *p2 = pSList->GetAt(Tail);
		CDot dt2 = p2->GetAt(0);
		fs<<dt2.x;
		fs<<"------------------";
*/
		//

		int nStrkCount = pSList->GetCount();
// 		if (1 == nStrkCount)
// 		{
// 			CStroke *pStroke = NULL;
// 			pStroke = pSList->GetHead();
// 			if (NULL == pStroke) 
// 			{
// 				continue;
// 			}
// 
// 			int nDtCount = pStroke->GetSize();
// 			if (nDtCount <= 0) 
// 			{
// 				continue;
// 			}
// 
// 			CDot sDt = pStroke->GetAt(0);
// 			CDot eDT = pStroke->GetAt(nDtCount - 1);
// 			if (!bIsGetFirstDt)
// 			{
// 				for (int j = 0; j < (int)m_nSingleLineMarkCount; j++) 
// 				{
// 					if ( 0 == (j % 2) )
// 					{
// 						lsDt = sDt; // ż��
// 					}
// 					else
// 					{
// 						lsDt = eDT; // ����
// 					}
// 					pStroke->Add(lsDt);
// 				}
// 				bIsGetFirstDt = TRUE;
// 				continue;
// 			}
// 
// 			ProOneStroke(lsDt, pStroke);
// 			for (int k = 0; k < (int)m_nSingleLineMarkCount; k++) 
// 			{
// 				if ( 0 == (k % 2) )
// 				{
// 					lsDt = sDt; // ż��
// 				}
// 				else
// 				{
// 					lsDt = eDT; // ����
// 				}
// 				pStroke->Add(lsDt);
// 			}
// 
// 		}// if (1 == nStrkCount)
// 		else
		{
			POSITION pos = pSList->GetHeadPosition();
			CStroke *pStroke = NULL;
			while(pos!=NULL)
			{	
				pStroke = pSList->GetNext(pos);
				if (NULL == pStroke) 
				{
					continue;
				}

				int nDtCount = pStroke->GetSize();
				if (nDtCount <= 0) 
				{
					continue;
				}

				if (!bIsGetFirstDt)
				{
					lsDt = pStroke->GetAt(nDtCount - 1);
					bIsGetFirstDt = TRUE;
					continue;
				}

				ProOneStroke(lsDt, pStroke);
				lsDt = pStroke->GetAt(nDtCount-1);
			}// while(pos!=NULL)
		}// else

	}// for (int i = 0; i < nCount; i++)

//	fclose( fp );

	return TRUE;
}

BOOL CNodeCodebar::ConnectOneCodeBar()
{
	int nCount = m_CodeBarList.GetSize();
	if (nCount <= 0) 
	{
		return FALSE;
	}

	for (int i = 0; i < nCount; i++)
	{
		StrokeList *pSList = m_CodeBarList.GetAt(i);
		if (NULL == pSList)
		{
			continue;
		}

		POSITION pos  = pSList->GetHeadPosition();
		POSITION prev = NULL;
		CStroke* pPreStroke = pSList->GetNext(pos);
		if (NULL == pPreStroke)
		{
			continue;
		}
		
		while (pos != NULL)
		{
			prev=pos;
			CStroke* pStroke = pSList->GetNext(pos);
			if (NULL == pStroke) 
			{
				continue;
			}

			int nCnt = pStroke->GetSize();
			for (int k = 0; k < nCnt; k++)
			{
				CDot dt = pStroke->GetAt(k);
				pPreStroke->Add(dt);
			}
			
			pSList->RemoveAt(prev);
			if (pStroke) 
			{
				delete pStroke;
				pStroke = NULL;
			}
			
		}//end while (pos != NULL)
	}

	return TRUE;
}

BOOL CNodeCodebar::AddToList()
{
	int nCount = m_CodeBarList.GetSize();
	if (nCount <= 0) 
	{
		return FALSE;
	}
	
	for (int i = 0; i < nCount; i++)
	{
		StrokeList *pSList = m_CodeBarList.GetAt(i);
		if (NULL == pSList)
		{
			continue;
		}

		POSITION pos = pSList->GetHeadPosition();
		CStroke *pStroke = NULL;
		while(pos!=NULL)
		{
			pStroke = pSList->GetNext(pos);
			if (NULL == pStroke) 
			{
				continue;
			}

			m_list.AddTail(pStroke);
		}
	}//end for (int i = 0; i < nCount; i++)
	
	return TRUE;
}

void CNodeCodebar::DrawExtern(XDC *pDC)
{
	POSITION pos;
	CStroke *pStroke;
	
	pos=m_list.GetHeadPosition();
	while(pos!=NULL)	
	{
		pStroke=m_list.GetNext(pos);
		pStroke->DrawExtern(pDC);
	}
	
	pos=m_listFill.GetHeadPosition();
	while(pos!=NULL)
	{	
		pStroke=m_listFill.GetNext(pos);
		pStroke->DrawExtern(pDC);
	}
}

BOOL CNodeCodebar::CreateStrokeListPoint()
{
	//��ά���뽨������
	//ɾ����Ч����
	Delete();
	DeleteFill();

	BYTE byte;
	BOOL bEnter;
	UINT nS;
	CUIntArray nArray;
	CDot dot,tmp;
	CStroke* pStroke;

	double dbBlockH = m_dbHeightBar/m_nRow;	//��ά������
	double dbOffset = m_dbFillSpace;
	double dbShrink = m_dbshrink;	//������
	double dbWidth = double(m_nPointRadiusPercent)/(100);//Բ�ζ�ά���ȱ���
	BOOL bRev = FALSE;				//�Ƿ���
	BOOL bLoop = TRUE;				//�������
	double dbRevWidth = m_dbRevWidth;	//������
	BYTE s,e;	//��׼�㣬���ڷ���ʱȷ��λ��
	s=bRev?0:1;
	e=bRev?1:0;

	UINT nFillNum = (UINT)((dbBlockH-dbShrink*2)/dbOffset);
	dbOffset = dbBlockH/nFillNum;
	UINT nRevNum = (UINT)(dbRevWidth/dbOffset);
	dbRevWidth = nRevNum*dbOffset;

	CString str1;
	//DEBUG START
	{
		OutputDebugString("codebar start\n");
		CString str;

		for(UINT i=0;i<m_nCol;i++)
		{
			for(UINT j=0;j<m_nRow;j++)
			{
				byte = *(m_pByte+j*m_nCol+i);
				if(byte == 0)
					str += "0";
				else
					str += "1";
			}
			str += "\n";
			OutputDebugString(str);
		}
		OutputDebugString("codebar end\n");
		str1 = str;
	}
	//DEBUG END
	nS = 0;
	for(UINT i=0;i<m_nCol;i++)
	{
		BOOL bS,bE;
		bE = bS = FALSE;
		bEnter=FALSE;
		nArray.RemoveAll();
		for(UINT j=0;j<m_nRow;j++)
		{
			byte = *(m_pByte+j*m_nCol+i);
			if(1==byte){
				nArray.Add(j);
			}
		}
		int nSize = nArray.GetSize();
		for(int m=0;m<nSize;m++)
		{
			nS = nArray.GetAt(m);

			pStroke = new CStroke(m_property);
			m_list.AddTail(pStroke);

			tmp.x = i*dbBlockH+dbShrink;

			tmp.y = -(nS*dbBlockH+dbShrink);
			dot = tmp;
			CDot dotCenter;
			dotCenter.x = dot.x;
			dotCenter.y = dot.y;
			double R = (dbBlockH/2-dbShrink)*dbWidth;
			double dbStartAngel = 0;
			double dbEndAngel = 2*PAI;			

			int iNum = int(abs(int(50.0*R*(dbEndAngel-dbStartAngel))));
			double step = (dbEndAngel-dbStartAngel)/iNum;
			double angle= 0.0;
			CDot EllipseDot;
			CDot CStartDot;
			for (int i=0; i<=iNum; i++) 
			{
				angle = dbStartAngel+step*i;
				EllipseDot.x = dotCenter.x+R*cos(angle);
				EllipseDot.y = dotCenter.y+R*sin(angle);
				pStroke->Add(EllipseDot);
			}

		}
	}

	return TRUE;
}
