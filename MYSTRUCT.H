


double MyDegToRad(double val);
double MyRadToDeg(double val);
CString IntToStr(int nNum);
CString dbToStr(double dbVal);

CString GetPatch(CString strInit,CString strEnd,BOOL bLeft = TRUE);//得当补充零位后的值
CString GetPatch(double init,double end,BOOL bLeft = TRUE);//得当补充零位后的值
CString GetPatch(double init,int Len,BOOL bLeft = TRUE);
CString GetPatch(CString strInit,int Len,BOOL bLeft = TRUE);//得当补充零位后的值

