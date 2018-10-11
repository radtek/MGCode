// FntShx.cpp: implementation of the CFntShx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hl.h"
#include "FntShx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CFntShx,CObject,2000)

CFntShx::CFntShx()
{
	m_strChineseFont="简体宋体";
	m_strEnglishFont="romans";
	m_strNumberFont="romans";
}

CFntShx::~CFntShx()
{
}

BOOL CFntShx::MakeFixStroke()
{
	FONT_INFO fontInfo;
	char *numbers=m_strText.GetBuffer(m_strText.GetLength());
	unsigned __int8 letter;

	m_dotCur.x=0;
	m_dotCur.y=0;
	m_dotPrev=m_dotCur;

	//打开相应的字体文件
	m_pFileEnglish=new CFile;
	m_pFileChinese=new CFile;

	CHLApp *pApp=(CHLApp *)AfxGetApp();
	CString path=pApp->GetInstallPath();

	if (0 == m_strChineseFont.CompareNoCase(""))
	{
		AfxMessageBox(IDS_NOFINDFONT);
		return FALSE;
	}

	if (0 == m_strEnglishFont.CompareNoCase(""))
	{
		AfxMessageBox(IDS_NOFINDFONT);
		return FALSE;
	}

	if(!m_pFileChinese->Open(path+"\\pfonts\\"+m_strChineseFont+".shx",CFile::modeRead))
		return FALSE;
	if(!m_pFileEnglish->Open(path+"\\efonts\\"+m_strEnglishFont+".shx",CFile::modeRead))
		return FALSE;
	if(m_strEnglishFont==m_strNumberFont)
	{
		m_pFileNumber=m_pFileEnglish;	
	}
	else
	{	
		m_pFileNumber=new CFile;
		if(!m_pFileNumber->Open(path+"\\efonts\\"+m_strNumberFont+".shx",CFile::modeRead))
			return FALSE;
	}

	//中文
	char buf[27];
	UINT n=m_pFileChinese->Read(buf,27);
	if((CString)buf!="AutoCAD-86 bigfont 1.0\r\n\032\x008")
	{
		return FALSE;//与标准文件头不符，则退出
	}
	
	BIGSHX_INDEX indexBigshx;
	m_pFileChinese->Read(&(m_BigshxHead),sizeof(BIGSHX_HEAD));			//读文件头
	m_pFileChinese->Seek(m_BigshxHead.number_esc*4,CFile::current);		//跳过换义表

	m_posBigshxIndexStart=(DWORD)m_pFileChinese->GetPosition();

	m_pFileChinese->Read(&(indexBigshx),sizeof(BIGSHX_INDEX));			//读第一个散列表（形名区）
	m_pFileChinese->Seek(indexBigshx.pos,CFile::begin);					//跳到形名数据区
	m_pFileChinese->Seek(indexBigshx.len-4,CFile::current);				//跳到文件信息区
	m_pFileChinese->Read(&(fontInfo),sizeof(FONT_INFO));				//读文件信息
	m_dbUnitBigshx=fontInfo.ascent+fontInfo.descent;
	
	//英文
	//由于某些版本为1.0或1.1,因此，只比较到小数点即可。
	//autocad13提供的字库文件头为"AutoCAD-86 unifont 1.0",格式有所不同。
	//暂时跳过此种字体，但大体结构应该相同，以后再作处理。
	m_pFileEnglish->Read(&(m_EnshxHeadEnglish),sizeof(ENSHX_HEAD));
	m_EnshxHeadEnglish.c1[20]=0;			
	if((CString)m_EnshxHeadEnglish.c1!="AutoCAD-86 shapes 1.")
	{
		return FALSE;
	}

	m_pIndexEnglish=new ENSHX_INDEX[m_EnshxHeadEnglish.total_ch];
	m_pFileEnglish->Read(m_pIndexEnglish,m_EnshxHeadEnglish.total_ch*4);
	m_pFileEnglish->Seek(m_pIndexEnglish->ch_bytes-4,CFile::current);
	m_pFileEnglish->Read(&(fontInfo),sizeof(FONT_INFO));
	m_dbUnitEnglish=fontInfo.ascent+fontInfo.descent;

	//数字
	m_pFileNumber->SeekToBegin();
	m_pFileNumber->Read(&(m_EnshxHeadNumber),sizeof(ENSHX_HEAD));
	m_EnshxHeadNumber.c1[20]=0;
	if((CString)m_EnshxHeadNumber.c1!="AutoCAD-86 shapes 1.")
	{
		return FALSE;
	}

	m_pIndexNumber=new ENSHX_INDEX[m_EnshxHeadNumber.total_ch];
	m_pFileNumber->Read(m_pIndexNumber,m_EnshxHeadNumber.total_ch*4);
	m_pFileNumber->Seek(m_pIndexNumber->ch_bytes-4,CFile::current);
	m_pFileNumber->Read(&(fontInfo),sizeof(FONT_INFO));
	m_dbUnitNumber=fontInfo.ascent+fontInfo.descent;

	CDot dotTmp = m_dotCur;

	while((letter=*numbers)!='\0')
	{	
		if((unsigned __int8)(*numbers)>0xa0 && (unsigned __int8)(*(numbers+1))>0xa0)
		{
			//汉字的处理
			unsigned __int8 codeH=(unsigned __int8)(*numbers);
			numbers++;
			unsigned __int8 codeL=(unsigned __int8)(*numbers);
			unsigned __int16 letter=codeH*256+codeL;
			process(letter);
		}
		else
		{
			if(letter>=0x30&&letter<=0X39)
			{
				//数字的处理
				m_pFile=m_pFileNumber;
				m_EnshxHead=m_EnshxHeadNumber;
				m_pIndexEnshx=m_pIndexNumber;
				m_dbUnitEnshx=m_dbUnitNumber;
			}
			else
			{
				//英文的处理
				m_pFile=m_pFileEnglish;
				m_EnshxHead=m_EnshxHeadEnglish;
				m_pIndexEnshx=m_pIndexEnglish;
				m_dbUnitEnshx=m_dbUnitEnglish;
			}

			//回车换行的处理
			if(letter==0x0A)
			{
 				dotTmp.x = 0;
				m_dotCur.x=m_dotPrev.x=0;
				m_dotCur.y=m_dotPrev.y-=m_dbLineSpace/m_dbHeight;
			}

			process(letter);
		}
		numbers++;
		//字间距的处理
		if(letter!=0x0a)
		{
 			dotTmp.x += m_dbCharSpace/m_dbHeight/m_dbWidthFactor;
 			m_dotPrev.x = dotTmp.x;
			m_dotCur.x=m_dotPrev.x;
		}
	}

	delete []m_pIndexEnglish;
	delete []m_pIndexNumber;

	m_pFileChinese->Close();
	delete m_pFileChinese;

	m_pFileEnglish->Close();
	delete m_pFileEnglish;

	if(m_strEnglishFont!=m_strNumberFont)
	{
		m_pFileNumber->Close();
		delete m_pFileNumber;
	}

	return TRUE;
}

BOOL CFntShx::MakeOldStroke()
{
	FONT_INFO fontInfo;
	char *numbers=m_strText.GetBuffer(m_strText.GetLength());
	unsigned __int8 letter;

	m_dotCur.x=0;
	m_dotCur.y=0;
	m_dotPrev=m_dotCur;

	//打开相应的字体文件
	m_pFileEnglish=new CFile;
	m_pFileChinese=new CFile;

	CHLApp *pApp=(CHLApp *)AfxGetApp();
	CString path=pApp->GetInstallPath();

	if(!m_pFileChinese->Open(path+"\\pfonts\\"+m_strChineseFont+".shx",CFile::modeRead))
		return FALSE;
	if(!m_pFileEnglish->Open(path+"\\efonts\\"+m_strEnglishFont+".shx",CFile::modeRead))
		return FALSE;
	if(m_strEnglishFont==m_strNumberFont)
	{
		m_pFileNumber=m_pFileEnglish;	
	}
	else
	{	
		m_pFileNumber=new CFile;
		if(!m_pFileNumber->Open(path+"\\efonts\\"+m_strNumberFont+".shx",CFile::modeRead))
			return FALSE;
	}

	//中文
	char buf[27];
	UINT n=m_pFileChinese->Read(buf,27);
	if((CString)buf!="AutoCAD-86 bigfont 1.0\r\n\032\x008")
	{
		return FALSE;//与标准文件头不符，则退出
	}
	
	BIGSHX_INDEX indexBigshx;
	m_pFileChinese->Read(&(m_BigshxHead),sizeof(BIGSHX_HEAD));			//读文件头
	m_pFileChinese->Seek(m_BigshxHead.number_esc*4,CFile::current);		//跳过换义表

	m_posBigshxIndexStart=(DWORD)m_pFileChinese->GetPosition();

	m_pFileChinese->Read(&(indexBigshx),sizeof(BIGSHX_INDEX));			//读第一个散列表（形名区）
	m_pFileChinese->Seek(indexBigshx.pos,CFile::begin);					//跳到形名数据区
	m_pFileChinese->Seek(indexBigshx.len-4,CFile::current);				//跳到文件信息区
	m_pFileChinese->Read(&(fontInfo),sizeof(FONT_INFO));				//读文件信息
	m_dbUnitBigshx=fontInfo.ascent+fontInfo.descent;
	
	//英文
	//由于某些版本为1.0或1.1,因此，只比较到小数点即可。
	//autocad13提供的字库文件头为"AutoCAD-86 unifont 1.0",格式有所不同。
	//暂时跳过此种字体，但大体结构应该相同，以后再作处理。
	m_pFileEnglish->Read(&(m_EnshxHeadEnglish),sizeof(ENSHX_HEAD));
	m_EnshxHeadEnglish.c1[20]=0;			
	if((CString)m_EnshxHeadEnglish.c1!="AutoCAD-86 shapes 1.")
	{
		return FALSE;
	}

	m_pIndexEnglish=new ENSHX_INDEX[m_EnshxHeadEnglish.total_ch];
	m_pFileEnglish->Read(m_pIndexEnglish,m_EnshxHeadEnglish.total_ch*4);
	m_pFileEnglish->Seek(m_pIndexEnglish->ch_bytes-4,CFile::current);
	m_pFileEnglish->Read(&(fontInfo),sizeof(FONT_INFO));
	m_dbUnitEnglish=fontInfo.ascent+fontInfo.descent;

	//数字
	m_pFileNumber->SeekToBegin();
	m_pFileNumber->Read(&(m_EnshxHeadNumber),sizeof(ENSHX_HEAD));
	m_EnshxHeadNumber.c1[20]=0;
	if((CString)m_EnshxHeadNumber.c1!="AutoCAD-86 shapes 1.")
	{
		return FALSE;
	}

	m_pIndexNumber=new ENSHX_INDEX[m_EnshxHeadNumber.total_ch];
	m_pFileNumber->Read(m_pIndexNumber,m_EnshxHeadNumber.total_ch*4);
	m_pFileNumber->Seek(m_pIndexNumber->ch_bytes-4,CFile::current);
	m_pFileNumber->Read(&(fontInfo),sizeof(FONT_INFO));
	m_dbUnitNumber=fontInfo.ascent+fontInfo.descent;

	while((letter=*numbers)!='\0')
	{	
		if((unsigned __int8)(*numbers)>0xa0 && (unsigned __int8)(*(numbers+1))>0xa0)
		{
			//汉字的处理
			unsigned __int8 codeH=(unsigned __int8)(*numbers);
			numbers++;
			unsigned __int8 codeL=(unsigned __int8)(*numbers);
			unsigned __int16 letter=codeH*256+codeL;
			process(letter);
		}
		else
		{
			if(letter>=0x30&&letter<=0X39)
			{
				//数字的处理
				m_pFile=m_pFileNumber;
				m_EnshxHead=m_EnshxHeadNumber;
				m_pIndexEnshx=m_pIndexNumber;
				m_dbUnitEnshx=m_dbUnitNumber;
			}
			else
			{
				//英文的处理
				m_pFile=m_pFileEnglish;
				m_EnshxHead=m_EnshxHeadEnglish;
				m_pIndexEnshx=m_pIndexEnglish;
				m_dbUnitEnshx=m_dbUnitEnglish;
			}

			//回车换行的处理
			if(letter==0x0A)
			{
				m_dotCur.x=m_dotPrev.x=0;
				m_dotCur.y=m_dotPrev.y-=m_dbLineSpace/m_dbHeight;
			}

			process(letter);
		}
		numbers++;
		//字间距的处理
		if(letter!=0x0a)
		{
			m_dotPrev.x += m_dbCharSpace/m_dbHeight/m_dbWidthFactor;
			m_dotCur.x=m_dotPrev.x;
		}
	}

	delete []m_pIndexEnglish;
	delete []m_pIndexNumber;

	m_pFileChinese->Close();
	delete m_pFileChinese;

	m_pFileEnglish->Close();
	delete m_pFileEnglish;

	if(m_strEnglishFont!=m_strNumberFont)
	{
		m_pFileNumber->Close();
		delete m_pFileNumber;
	}

	return TRUE;
}

BOOL CFntShx::MakeStroke()
{
	if(m_bFixSpace)
		return MakeFixStroke();
	else
		return MakeOldStroke();
}

__int8 CFntShx::decode(unsigned __int8 code)
{
	//取补码的真值

	BOOL neg;
	neg=code&0x80;

	if(neg)		//bit 7 is 1
		return(code-0xFF-1);
	else
		return(code);
}

void CFntShx::CreateLine()
{
	CDot dot;

	if(m_bFirst)
	{
		m_bFirst=FALSE;

		CStroke *pStroke=new CStroke((m_property));
		m_pStrokeCur=pStroke;
		m_list.AddTail(m_pStrokeCur);
		
		dot.y=m_dotPrev.y*m_dbHeight;
		dot.x=m_dotPrev.x*m_dbWidthFactor*m_dbHeight;
		m_pStrokeCur->Add(dot);
	}

	dot.y=m_dotCur.y*m_dbHeight;
	dot.x=m_dotCur.x*m_dbWidthFactor*m_dbHeight;
	m_pStrokeCur->Add(dot);
}


void CFntShx::CreateArc(CDot dotCenter,double r,double angleStart,double angle)
{
	CDot dot;

	if(m_bFirst)
	{
		m_bFirst=FALSE;

		CStroke *pStroke=new CStroke((m_property));
		m_pStrokeCur=pStroke;
		m_list.AddTail(m_pStrokeCur);
		
		dot.y=m_dotPrev.y*m_dbHeight;
		dot.x=m_dotPrev.x*m_dbWidthFactor*m_dbHeight;
		m_pStrokeCur->Add(dot);
	}
	
	int limit=20;
	double step=angle/limit;
	for(int i=1;i<=limit;i++)
	{
		angle=i*step+angleStart;
		dot.x=r*cos(angle)+dotCenter.x;
		dot.y=r*sin(angle)+dotCenter.y;
		dot.y=dot.y*m_dbHeight;
		dot.x=dot.x*m_dbWidthFactor*m_dbHeight;
		m_pStrokeCur->Add(dot);	
	}
}

void CFntShx::process(unsigned __int8 letter)
{
	int i=0;
	long move_bytes=0;
	UINT read_bytes=0;
	double lx,ly;
	int Draw=1;
	int Vert=0;
	double ratio=1/m_dbUnitEnshx; 

	//定义堆栈
	double stack[10][2];
	int ps=0;

	m_bFirst=TRUE;

	while(letter>(m_pIndexEnshx+i)->ch_asc)
	{	
		move_bytes+=(m_pIndexEnshx+i)->ch_bytes;
		i++;
	}

	read_bytes=(m_pIndexEnshx+i)->ch_bytes;
	m_pFile->Seek(sizeof(ENSHX_HEAD)+m_EnshxHead.total_ch*4+move_bytes,CFile::begin);
	unsigned __int8 *ptr=new unsigned __int8[read_bytes+1];
	m_pFile->Read(ptr,read_bytes);
	unsigned __int8 *p=ptr;
	p++;         //jump head 0

	while(*p)
	{	
		switch(*p)
		{
		case 0x01:
				if(Vert)
					Vert=0;
				else
				{
					Draw=1;
					m_bFirst=TRUE;
				}
				p++;
				break;
		case 0x02:
				if(Vert)
					Vert=0;
				else
				{
					Draw=0;
					m_bFirst=FALSE;
				}
				p++;
				break;
		case 0x03:
				if(Vert)
				{
					Vert=0;
					p++;
				}
				else
					ratio/=*(++p);
				p++;
				break;
		case 0x04:
				if(Vert)
				{
					Vert=0;
					p++;
				}
				else
					ratio*=*(++p);
				p++;
				break;
		case 0x05:
				if(Vert)
					Vert=0;
				else
				{
					stack[ps][0]=m_dotCur.x;
					stack[ps][1]=m_dotCur.y;
					ps++;
				}
				p++;
				break;
		case 0x06:
				if(Vert)
					Vert=0;
				else
				{
					ps--;
					m_dotCur.x=stack[ps][0];
					m_dotCur.y=stack[ps][1];
					m_dotPrev=m_dotCur;
				}
				p++;
				break;
		case 0x07:
				if(Vert)
				{
					Vert=0;
					p++;
				}
				else
				{
					process(*(++p));
				}
				
				p++;
				break;

		case 0x08:
				lx=ratio*decode(*(++p));
				ly=ratio*decode(*(++p));

				if(Vert)
					Vert=0;
				else
				{	
					m_dotCur.x+=lx;
					m_dotCur.y+=ly;
					
					if(Draw)
						CreateLine();

					m_dotPrev=m_dotCur;
				}
				p++;
				break;
		case 0x09:
				p++;
				do{
					lx=ratio*decode(*(p));
					ly=ratio*decode(*(++p));
					if(!Vert)
					{
						m_dotCur.x+=lx;
						m_dotCur.y+=ly;
						
						if(Draw)
							CreateLine();

						m_dotPrev=m_dotCur;
					}
					p++;
				}while(*p||*(p+1));
				if(Vert)
					Vert=0;
				p++;
				p++;
				break;
		case 0x0a:
				if(Vert)
				{
					Vert=0;
					p+=2;
				}
				else
				{
					//定义八分圆
					//r为圆弧半径
					//s为起始位置 0：0，1：45，2：90;
					//c为8分圆个数，0表示8个
					CDot dotCenter;
					double r,angleStart,angle;
					int s,c;
					r=*(++p)*ratio;
					s=(*(++p)&0xf0)>>4;
					c=*p&0x0f;
					if(c==0)c=8;
					angleStart=s*PAI/4;
					dotCenter.x=m_dotCur.x-r*cos(angleStart);
					dotCenter.y=m_dotCur.y-r*sin(angleStart);
					
					if(s<8)
					{	
						angle=c*PAI/4;
						m_dotCur.x=r*cos((s+c)*PAI/4)+dotCenter.x;
						m_dotCur.y=r*sin((s+c)*PAI/4)+dotCenter.y;
					}
					else
					{	
						angle=-c*PAI/4;
						m_dotCur.x=r*cos((s-c)*PAI/4)+dotCenter.x;
						m_dotCur.y=r*sin((s-c)*PAI/4)+dotCenter.y;
					}

					if(Draw)
						CreateArc(dotCenter,r,angleStart,angle);
					m_dotPrev=m_dotCur;
				}
				p++;
				break;
				
		case 0x0b:
				if(Vert)
				{
					Vert=0;
					p+=5;
				}
				else
				{	
					//定义小段圆弧,基于8分圆定义的基础上，加上相对起始角和相对终止角
					//S相对起始角
					//E相对终止角
					//H圆弧半径高8位
					//L圆弧半径低8位
					//r为圆弧半径
					//s为起始位置 0：0，1：45，2：90;
					//c为8分圆个数，0表示8个
					int S,E,H,L,s,c;
					double r,angleStart,angle;
					double ang1,ang2;
					CDot dotCenter;

					S=*(++p);
					E=*(++p);
					H=*(++p);
					L=*(++p);
					r=(H*256+L)*ratio;
					
					s=(*(++p)&0xf0)>>4;
					c=*p&0x0f;
					if(c==0)c=8;

					ang1=(S*45.0/255)*PAI/180;
					ang2=((255-E)*45.0/255)*PAI/180;
					if(E==0||E==1||E==2||E==255)ang2=0;
					
					if(s<8)
					{
						angleStart=s*PAI/4+ang1;
						angle=c*PAI/4-ang2-ang1;

						dotCenter.x=m_dotCur.x-r*cos(angleStart);
						dotCenter.y=m_dotCur.y-r*sin(angleStart);
						m_dotCur.x=r*cos((s+c)*PAI/4-ang2)+dotCenter.x;
						m_dotCur.y=r*sin((s+c)*PAI/4-ang2)+dotCenter.y;
					}
					else
					{ 	
						angleStart=s*PAI/4-ang1;
						angle=-c*PAI/4+ang2+ang1;

						dotCenter.x=m_dotCur.x-r*cos(angleStart);
						dotCenter.y=m_dotCur.y-r*sin(angleStart);
						m_dotCur.x=r*cos((s-c)*PAI/4+ang2)+dotCenter.x;
						m_dotCur.y=r*sin((s-c)*PAI/4+ang2)+dotCenter.y;
					}

					if(Draw)
						CreateArc(dotCenter,r,angleStart,angle);

					m_dotPrev=m_dotCur;
				}
				p++;
				break;

		case 0x0c:
				if(Vert)
				{
					Vert=0;
					p+=3;
				}
				else
				{
					//由位移和凸度定义圆弧
					CDot dotCenter;
					int B;
					double L1,L2,r,angleStart,angle;
					double ang1,ang2;

					lx=ratio*decode(*(++p));
					ly=ratio*decode(*(++p));

					L1=sqrt(lx*lx+ly*ly)/2;
					B=decode(*(++p));
					
					L2=L1*abs(B)/127;

					r=(L1*L1+L2*L2)/L2/2;

					ang1=atan2(ly,lx);
					ang2=atan2(r-L2,L1);
					if(B>=0)
					{
						ang1+=ang2;
						angleStart=ang1-PAI;
						angle=2*(PAI/2-ang2);
					}
					else
					{
						ang1-=ang2;
						angleStart=ang1+PAI;
						angle=-2*(PAI/2-ang2);
					}

					dotCenter.x=r*cos(ang1)+m_dotCur.x;
					dotCenter.y=r*sin(ang1)+m_dotCur.y;

					m_dotCur.x+=lx;
					m_dotCur.y+=ly;

					if(Draw)
						CreateArc(dotCenter,r,angleStart,angle);

					m_dotPrev=m_dotCur;
				}
				p++;
				break;
				
		case 0x0d:
			{
				p++;
				//多个凸度定义圆弧，由（0，0）终止
				CDot dotCenter;
				int B;
				double L1,L2,r,angleStart,angle;
				double ang1,ang2;

				do
				{
					lx=ratio*decode(*(p));
					ly=ratio*decode(*(++p));
					B=decode(*(++p));

					if(!Vert)
					{
						L1=sqrt(lx*lx+ly*ly)/2;
						L2=L1*abs(B)/127;
						r=(L1*L1+L2*L2)/L2/2;
					
						ang1=atan2(ly,lx);
						ang2=atan2(r-L2,L1);
						if(B>=0)
						{
							ang1+=ang2;
							angleStart=ang1-PAI;
							angle=2*(PAI/2-ang2);
						}
						else
						{
							ang1-=ang2;
							angleStart=ang1+PAI;
							angle=-2*(PAI/2-ang2);
						}

						dotCenter.x=r*cos(ang1)+m_dotCur.x;
						dotCenter.y=r*sin(ang1)+m_dotCur.y;
						m_dotCur.x+=lx;
						m_dotCur.y+=ly;

						if(Draw)
							CreateArc(dotCenter,r,angleStart,angle);

						m_dotPrev=m_dotCur;
					}
					p++;
				}while(*p||*(p+1));
			
				if(Vert)
					Vert=0;
				p++;
				p++;
				break;
			}	
		case 0x0E:	
				Vert=1;
				p++;
				break;
		default	:
				if(*p&&(!Vert))
				{       	
					//01 decode
					lx=((*p)&0xF0)*(cad_vec[(*p)&0x0F][0])/32;
					ly=((*p)&0xF0)*(cad_vec[(*p)&0x0F][1])/32;
					m_dotCur.x+=ratio*lx;
					m_dotCur.y+=ratio*ly;
					if(Draw)
						CreateLine();
					m_dotPrev=m_dotCur;
				}

				if(Vert)
					Vert=0;
				p++;
				break;
		}	//end of switch
	}	//end of while(*p)

	delete []ptr;
}

void CFntShx::process(unsigned __int16 letter)
{
	UINT read_bytes=0;
	double lx,ly;
	int Draw=1;
	int Vert=0;
	double ratio=1/m_dbUnitBigshx; 

	//定义堆栈
	double stack[10][2];
	int ps=0;
	
	m_bFirst=TRUE;

	BIGSHX_INDEX indexBigshx;
	m_pFileChinese->Seek(m_posBigshxIndexStart,CFile::begin);
	for(unsigned __int16 i=0;i<m_BigshxHead.number_index;i++)
	{
		m_pFileChinese->Read(&indexBigshx,sizeof(BIGSHX_INDEX));
		if(letter==indexBigshx.code)
		{
			read_bytes=indexBigshx.len;
			break;
		}
	}

	if(read_bytes==0)
	{
		return;
	}

	m_pFileChinese->Seek(indexBigshx.pos,CFile::begin);
	unsigned __int8 *ptr=new unsigned __int8[read_bytes+1];
	m_pFileChinese->Read(ptr,read_bytes);
	unsigned __int8 *p=ptr;
	p++;         //jump head 0

	while(*p)
	{	
		
		switch(*p)
		{
		case 0x01:
				if(Vert)
					Vert=0;
				else
				{
					Draw=1;
					m_bFirst=TRUE;
				}
				
				p++;
				break;
		case 0x02:
				
				if(Vert)
					Vert=0;
				else
				{
					Draw=0;
					m_bFirst=FALSE;
				}
				p++;
				break;
		case 0x03:
				if(Vert)
				{
					Vert=0;
					p++;
				}
				else
					ratio/=*(++p);
				p++;
				break;
		case 0x04:
				if(Vert)
				{
					Vert=0;
					p++;
				}
				else
					ratio*=*(++p);
				p++;
				break;
		case 0x05:
				if(Vert)
					Vert=0;
				else
				{
					stack[ps][0]=m_dotCur.x;
					stack[ps][1]=m_dotCur.y;
					ps++;
				}
				p++;
				break;
		case 0x06:
				if(Vert)
					Vert=0;
				else
				{
					ps--;
					m_dotCur.x=stack[ps][0];
					m_dotCur.y=stack[ps][1];
					m_dotPrev=m_dotCur;
				}
				p++;
				break;
		case 0x07:
				if(Vert)
				{
					Vert=0;
					p++;
				}
				else
				{
					//子图形的处理略过
					//process(*(++p));
				}
				
				p++;
				break;

		case 0x08:
				lx=ratio*decode(*(++p));
				ly=ratio*decode(*(++p));

				if(Vert)
					Vert=0;
				else
				{	
					m_dotCur.x+=lx;
					m_dotCur.y+=ly;
					
					if(Draw)
						CreateLine();

					m_dotPrev=m_dotCur;

				}
				p++;
				break;
		case 0x09:
				p++;
				do{
					lx=ratio*decode(*(p));
					ly=ratio*decode(*(++p));
					if(!Vert)
					{
						m_dotCur.x+=lx;
						m_dotCur.y+=ly;
						
						if(Draw)
							CreateLine();

						m_dotPrev=m_dotCur;
					}
					p++;
				}while(*p||*(p+1));
				if(Vert)
					Vert=0;
				p++;
				p++;
				break;
		case 0x0a:
				if(Vert)
				{
					Vert=0;
					p+=2;
				}
				else
				{
					//定义八分圆
					//r为圆弧半径
					//s为起始位置 0：0，1：45，2：90;
					//c为8分圆个数，0表示8个
					CDot dotCenter;
					double r,angleStart,angle;
					int s,c;
					r=*(++p)*ratio;
					s=(*(++p)&0xf0)>>4;
					c=*p&0x0f;
					if(c==0)c=8;
					angleStart=s*PAI/4;
					dotCenter.x=m_dotCur.x-r*cos(angleStart);
					dotCenter.y=m_dotCur.y-r*sin(angleStart);
					
					if(s<8)
					{	
						angle=c*PAI/4;
						m_dotCur.x=r*cos((s+c)*PAI/4)+dotCenter.x;
						m_dotCur.y=r*sin((s+c)*PAI/4)+dotCenter.y;
					}
					else
					{	
						angle=-c*PAI/4;
						m_dotCur.x=r*cos((s-c)*PAI/4)+dotCenter.x;
						m_dotCur.y=r*sin((s-c)*PAI/4)+dotCenter.y;
					}

					if(Draw)
						CreateArc(dotCenter,r,angleStart,angle);

					m_dotPrev=m_dotCur;
					
				}
				p++;
				break;
				
		case 0x0b:
				if(Vert)
				{
					Vert=0;
					p+=5;

				}
				else
				{	
					//定义小段圆弧,基于8分圆定义的基础上，加上相对起始角和相对终止角
					//S相对起始角
					//E相对终止角
					//H圆弧半径高8位
					//L圆弧半径低8位
					//r为圆弧半径
					//s为起始位置 0：0，1：45，2：90;
					//c为8分圆个数，0表示8个
					int S,E,H,L,s,c;
					double r,angleStart,angle;
					double ang1,ang2;
					CDot dotCenter;

					S=*(++p);
					E=*(++p);
					H=*(++p);
					L=*(++p);
					r=(H*256+L)*ratio;
					
					s=(*(++p)&0xf0)>>4;
					c=*p&0x0f;
					if(c==0)c=8;

					ang1=(S*45.0/255)*PAI/180;
					ang2=((255-E)*45.0/255)*PAI/180;
					if(E==0||E==1||E==2||E==255)ang2=0;
					
					if(s<8)
					{
						angleStart=s*PAI/4+ang1;
						angle=c*PAI/4-ang2-ang1;

						dotCenter.x=m_dotCur.x-r*cos(angleStart);
						dotCenter.y=m_dotCur.y-r*sin(angleStart);
						m_dotCur.x=r*cos((s+c)*PAI/4-ang2)+dotCenter.x;
						m_dotCur.y=r*sin((s+c)*PAI/4-ang2)+dotCenter.y;
					}
					else
					{ 	
						angleStart=s*PAI/4-ang1;
						angle=-c*PAI/4+ang2+ang1;

						dotCenter.x=m_dotCur.x-r*cos(angleStart);
						dotCenter.y=m_dotCur.y-r*sin(angleStart);
						m_dotCur.x=r*cos((s-c)*PAI/4+ang2)+dotCenter.x;
						m_dotCur.y=r*sin((s-c)*PAI/4+ang2)+dotCenter.y;
					}

					if(Draw)
						CreateArc(dotCenter,r,angleStart,angle);
					m_dotPrev=m_dotCur;				
				}
				p++;
				break;

		case 0x0c:
				if(Vert)
				{
					Vert=0;
					p+=3;
				}
				else
				{
					//由位移和凸度定义圆弧
					CDot dotCenter;
					int B;
					double L1,L2,r,angleStart,angle;
					double ang1,ang2;

					lx=ratio*decode(*(++p));
					ly=ratio*decode(*(++p));

					L1=sqrt(lx*lx+ly*ly)/2;
					B=decode(*(++p));
					
					L2=L1*abs(B)/127;

					r=(L1*L1+L2*L2)/L2/2;

					ang1=atan2(ly,lx);
					ang2=atan2(r-L2,L1);
					if(B>=0)
					{
						ang1+=ang2;
						angleStart=ang1-PAI;
						angle=2*(PAI/2-ang2);
					}
					else
					{
						ang1-=ang2;
						angleStart=ang1+PAI;
						angle=-2*(PAI/2-ang2);
					}

					dotCenter.x=r*cos(ang1)+m_dotCur.x;
					dotCenter.y=r*sin(ang1)+m_dotCur.y;

					m_dotCur.x+=lx;
					m_dotCur.y+=ly;

				
					if(Draw)
						CreateArc(dotCenter,r,angleStart,angle);

					m_dotPrev=m_dotCur;
				}
				p++;
				break;
				
		case 0x0d:
			{
				p++;
				//多个凸度定义圆弧，由（0，0）终止
				CDot dotCenter;
				int B;
				double L1,L2,r,angleStart,angle;
				double ang1,ang2;

				do
				{
					lx=ratio*decode(*(p));
					ly=ratio*decode(*(++p));
					B=decode(*(++p));

					if(!Vert)
					{

						L1=sqrt(lx*lx+ly*ly)/2;
						L2=L1*abs(B)/127;
						r=(L1*L1+L2*L2)/L2/2;
					
						ang1=atan2(ly,lx);
						ang2=atan2(r-L2,L1);
						if(B>=0)
						{
							ang1+=ang2;
							angleStart=ang1-PAI;
							angle=2*(PAI/2-ang2);
						}
						else
						{
							ang1-=ang2;
							angleStart=ang1+PAI;
							angle=-2*(PAI/2-ang2);
						}

						dotCenter.x=r*cos(ang1)+m_dotCur.x;
						dotCenter.y=r*sin(ang1)+m_dotCur.y;
						m_dotCur.x+=lx;
						m_dotCur.y+=ly;

						if(Draw)
							CreateArc(dotCenter,r,angleStart,angle);

						m_dotPrev=m_dotCur;
					}
					p++;
				}while(*p||*(p+1));
			
				if(Vert)
					Vert=0;
				p++;
				p++;
				break;
			}	
		case 0x0E:	
				Vert=1;
				p++;
				break;
		default	:
				if(*p&&(!Vert))
				{       	
					//01 decode
					lx=((*p)&0xF0)*(cad_vec[(*p)&0x0F][0])/32;
					ly=((*p)&0xF0)*(cad_vec[(*p)&0x0F][1])/32;
					m_dotCur.x+=ratio*lx;
					m_dotCur.y+=ratio*ly;
					if(Draw)
						CreateLine();

					m_dotPrev=m_dotCur;

				}

				if(Vert)
					Vert=0;
				p++;
				break;
		}	//end of switch
	}	//end of while(*p)

	delete []ptr;
}

void CFntShx::Serialize(CArchive& ar)
{
}


