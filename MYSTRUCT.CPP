#include "stdafx.h"
#include "quad.h"

CString IntToStr(int nNum)
{
	CString str;
	str.Format("%d",nNum);
	return str;
}

CString dbToStr(double dbVal)
{
	CString str;
	str.Format("%.3f",dbVal);
	return str;
}

CString GetPatch(CString strInit,CString strEnd,BOOL bLeft)//得当补充零位后的值
{
	CString strini = strInit;
	int Lenini = strInit.GetLength();
	int Lenend = strEnd.GetLength();
	int n =Lenend-Lenini; 
	if(n>0){
		while(Lenend-Lenini>0){
			strini = "0"+strini;
			Lenini++;
		}
	}
	if(n<0){
		if(bLeft)
			strini = strini.Left(Lenend);
		else
			strini = strini.Right(Lenend);
	}
	return strini;
}

CString GetPatch(double init,double end,BOOL bLeft)
{
	CString strini = "";
	CString strend = "";	
	int sign,dec;
	strini = _fcvt(init,0,&dec,&sign);
	strend = _fcvt(end,0,&dec,&sign);
	int Lenini = strini.GetLength();
	int Lenend = strend.GetLength();
	int n =Lenend-Lenini; 
	if(n>0){
		while(Lenend-Lenini){
			strini = "0"+strini;
			Lenini++;
		}
	}
	if(Lenend-Lenini<0){
		if(bLeft)
			strini = strini.Left(Lenend);
		else
			strini = strini.Right(Lenend);
	}
	return strini;
}

CString GetPatch(double init,int Len,BOOL bLeft)
{
	CString strini = "";
	int sign,dec;
	strini = _fcvt(init,0,&dec,&sign);
	int Lenini = strini.GetLength();
	if(Len-Lenini>0){
	while(Len-Lenini){
		strini = "0"+strini;
		Lenini++;
	}
	}
	if(Len-Lenini<0){
		if(bLeft)
			strini = strini.Left(Len);
		else
			strini = strini.Right(Len);
	}
	return strini;
}

CString GetPatch(CString strInit,int Len,BOOL bLeft)//得当补充零位后的值
{
	CString strini = strInit;
	int Lenini = strInit.GetLength();
	int n =Len-Lenini; 
	if(n>0){
		while(Len-Lenini>0){
			strini = "0"+strini;
			Lenini++;
		}
	}
	if(n<0){
		if(bLeft)
			strini = strini.Left(Len);
		else
			strini = strini.Right(Len);
	}
	return strini;
}
