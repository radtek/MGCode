// AdoDB.cpp: implementation of the CAdoDB class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "AdoDB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAdoDB::CAdoDB():m_strSource(_T(""))
{
	if (!SUCCEEDED(::CoInitialize(NULL)))
	{
		AfxMessageBox(_T(" COM Init error! "));
	}
	if (!CreateAdoInstance())
	{
		::OutputDebugString(_T("CAdoDB::CAdoDB(): Create Ado instance error!\n"));
	}
}

CAdoDB::~CAdoDB()
{
	ReleaseAdoInstance();
	::CoUninitialize();
}

/*********************************************************\
	FUNCTION: ConnectToDB();
    作用:连接到数据库。
	Para: 连接参数；
	bCloseIfLive: 如果已经连接，是否关闭重连。
\*********************************************************/
BOOL CAdoDB::ConnectToDB(const DBPARA &Para,BOOL bCloseIfLive)
{
	_bstr_t _Source("");
	///////////////////////////////
	if (m_pConnection!=NULL && m_pConnection->GetState())
	{
		if (bCloseIfLive)	DisconnectDB();
		else  return TRUE;
	}
	m_pConnection->CommandTimeout=(Para.nTimeOut>0)?Para.nTimeOut:5;
	/////////////////////////////////////////
	switch(Para.nDriverType)
	{
	case driverODBC://ODBC DataSource(万能型)
		_Source=_bstr_t("Data Source=")+Para._Database;
		//_Source+=_bstr_t(";UID=")+Para._User;
		//_Source+=_bstr_t(";PWD=")+Para._Pwd+_bstr_t(";");
		break;

	case driverSQLServer://SQL Server
		_Source=_bstr_t("Driver={SQL SERVER};");
		_Source+=_bstr_t("Server=")+Para._Server;
		_Source+=_bstr_t(";Database=")+Para._Database;
		break;

	case driverACCESS97://ACCESS 97
		_Source=_bstr_t("Microsoft.Jet.OLEDB.3.51;");
		_Source+=_bstr_t("Data Source=")+Para._Database;
		break;

	case driverACCESS2000://ACCESS 2000 OR XP
		_Source=_bstr_t("Provider=Microsoft.Jet.OLEDB.4.0;");
		_Source+=_bstr_t("Data Source=")+Para._Database;
		break;

	case driverACCESSXP:
		_Source=_bstr_t("Provider=Microsoft.Jet.OLEDB.4.0;");
		_Source+=_bstr_t("Data Source=")+Para._Database;
		break;

	default:
		return FALSE;
	}

	//////////////////////////////////////////////////
	try
	{
		HRESULT hr;//
		hr = m_pConnection->Open(_Source,Para._User,Para._Pwd,adModeUnknown);
		if (SUCCEEDED(hr))
		{
			//Save datasource
			m_strSource = (const char*) _Source;
		}
	}
	catch(_com_error &e)
	{
		::OutputDebugString(_T("CAdoDB::ConnectToDB() Error:\n"));
		::OutputDebugString(e.ErrorMessage());
	}
	return m_pConnection->GetState();
}

/************************************\
	      断开连接
\************************************/
void CAdoDB::DisconnectDB()
{
	try
	{
		if (m_pConnection!=NULL)
		{
			if (m_pConnection->GetState())
				m_pConnection->Close();
		}
	}
	catch(_com_error &e)
	{
		::OutputDebugString(_T("CAdoDB::DisconnectDB() Error:\n"));
		::OutputDebugString(e.ErrorMessage());
	}
}

/**********************************************************************\
	执行SQL脚本，无返回数据集(适用于Insert .../Update ... / Delete ...)
	_strSqlScript: 要执行的SQL脚本；
	pAffectedRecord:执行该脚本后受影响的记录数指针，可为空
\**********************************************************************/
BOOL CAdoDB::ExecuteSQLQuery(/*in*/_bstr_t _strSqlScript,/*in/out*/long* pAffectedRecord)
{
	ASSERT(m_pConnection!=NULL);
	if  (!IsDBConnected())  return FALSE;
	/////////////////////////////////
	HRESULT hr;
	try
	{
		if (!m_pConnection->GetState()) return FALSE;
		_variant_t _varAffected;
		VariantInit(&_varAffected);
		hr = m_pConnection->Execute(_strSqlScript,&_varAffected,adCmdText);
		if (pAffectedRecord) *pAffectedRecord = (long)_varAffected;
	}
	catch(_com_error &e)
	{
		::OutputDebugString(_T("\nCAdoDb::ExecuteSQLQuery() Error:\n"));
		::OutputDebugString(e.ErrorMessage());
		return FALSE;
	}

	return 	SUCCEEDED(hr);
}

/*********************************************************\
	执行SQL脚本，返回数据集(适用于全部SQL语句....)
	_strSqlScript: 要执行的SQL脚本；
	_pRecordset : 返回来的记录集
	pAffectedRecord:执行该脚本后受影响的记录数指针，可为空
\*********************************************************/
BOOL CAdoDB::ExecuteSQLQuery(_bstr_t _strSqlScript, CAdoRecordset &Recordset, long *pAffectedRecord)
{
	ASSERT(m_pConnection!=NULL);
	if (!IsDBConnected()) return FALSE;
	/////////////////////////////////////
	try
	{
		Recordset.Close();
		_RecordsetPtr _pRecordset = Recordset.Detach();
		if (_pRecordset==NULL)
		{ 
			 if (!Recordset.CreateInstance()) return FALSE;
			 _pRecordset = Recordset.Detach();
		}
		_variant_t _varAffected;
		VariantInit(&_varAffected);
		m_pCommand->ActiveConnection = m_pConnection;
		m_pCommand->CommandText=_strSqlScript;
		_pRecordset = m_pCommand->Execute(&_varAffected,NULL,adCmdText);
		Recordset.Attach(_pRecordset);
		if (pAffectedRecord) *pAffectedRecord = (long)_varAffected;
	}
	catch(_com_error &e)
	{
		::OutputDebugString(_T("\nCAdoDB::ExecuteSQLQuery(ALL Type) Error:"));
		::OutputDebugString(e.ErrorMessage());
		return FALSE;
	}

	return 	TRUE;
}

/*********************************************************\
	执行SQL脚本，返回数据集(适用于查询:Select * from ....)
	_strSqlScript: 要执行的SQL脚本；
	_pRecordset : 返回来的记录集
\*********************************************************/
BOOL CAdoDB::OpenSQLQuery(/*in*/_bstr_t _strSqlScript, /*in,out*/CAdoRecordset &Recordset)
{
	ASSERT(m_pConnection!=NULL);
	if  (!IsDBConnected())  return FALSE;
	/////////////////////////////////
	HRESULT hr;
	try
	{
		Recordset.Close();
		_RecordsetPtr _pRecordset = Recordset.Detach();
		if (_pRecordset==NULL)
		{ 
			 if (!Recordset.CreateInstance()) return FALSE;
			 _pRecordset = Recordset.Detach();
		}
		hr = _pRecordset->Open(_strSqlScript,m_pConnection.GetInterfacePtr(),adOpenDynamic,
							 adLockOptimistic,adCmdText);
		Recordset.Attach(_pRecordset);
	}
	catch(_com_error &e)
	{
		::OutputDebugString(_T("\nCAdoDB::OpenSQLQuery() Error:"));
		::OutputDebugString(e.ErrorMessage());
		return FALSE;
	}

	return 	SUCCEEDED(hr);
}

/************************************\
	       开始事务处理
\************************************/
BOOL CAdoDB::BeginTrans()
{
	ASSERT(m_pConnection!=NULL);

	if (!IsDBConnected()) return FALSE;
	HRESULT hr=m_pConnection->BeginTrans();
	return  SUCCEEDED(hr);
}

/************************************\
	      提交事务处理
\************************************/
BOOL CAdoDB::CommitTrans()
{
	if (!IsDBConnected()) return FALSE;
	HRESULT hr=m_pConnection->CommitTrans();
	return  SUCCEEDED(hr);
}

/************************************\
	      事务回滚处理
\************************************/
BOOL CAdoDB::RollbackTrans()
{
	if (!IsDBConnected()) return FALSE;
	HRESULT hr=m_pConnection->RollbackTrans();
	return  SUCCEEDED(hr);
}

/************************************\
	      创建Ado实例
\************************************/
BOOL CAdoDB::CreateAdoInstance()
{
	HRESULT hr;
	if (m_pConnection==NULL)
	{
		hr=m_pConnection.CreateInstance(__uuidof(Connection));
		if (!SUCCEEDED(hr)) return FALSE;
	}
	if (m_pCommand==NULL)
	{
		hr=m_pCommand.CreateInstance(__uuidof(Command));
	}
	return SUCCEEDED(hr);
}

/************************************\
	      关闭并释放所有对象
\************************************/
void CAdoDB::ReleaseAdoInstance()
{
	try{
		DisconnectDB();
		if (m_pConnection!=NULL)
		{
			m_pConnection.Release();
			m_pConnection = NULL;
		}
		if (m_pCommand!=NULL)
		{
			m_pCommand.Release();
			m_pCommand = NULL;
		}
	}
	catch(_com_error &e)
	{
		::OutputDebugString(_T("CAdoDB::ReleaseAdoInstance() Error:"));
		::OutputDebugString(e.ErrorMessage());
	}
}


/*****************************************\
	 CSTRING TO VARIANT OBJECT
\*****************************************/
CString CAdoRecordset::VariantToCString(VARIANT var)
{
	if (var.vt==VT_EMPTY || var.vt==VT_NULL)
		return _T("");

	CString strValue;
	_bstr_t _strData(var);
	strValue.Format(_T("%s"),(LPCTSTR)_strData);

/*	switch(var.vt){
	case VT_EMPTY:
	case VT_NULL:
		strValue.Empty();
		break;
	case VT_INT:
		strValue.Format(_T("%d"),var.intVal);
		break;
	case VT_I1:
		strValue.Format(_T("%d"),var.bVal);
		break;
	case VT_I2:
		strValue.Format(_T("%d"),var.iVal);
		break;
	case VT_I4:
		strValue.Format(_T("%d"),var.lVal);
		break;
	case VT_DECIMAL:
		strValue.Format(_T("%ld"),var.decVal.Lo64);
		break;
	case VT_R4:
		strValue.Format(_T("%d"),var.fltVal);
		break;
	case VT_R8:
		strValue.Format(_T("%d"),var.dblVal);
		break;
	case VT_CY:
		{
		COleCurrency _varCurrency(var);
		strValue=_varCurrency.Format(0);
		break;
		}
	case VT_BSTR:
		{
		_bstr_t _strData(var);
		strValue.Format(_T("%s"),(const char*)_strData);
		break;
		}
	case VT_DATE:
		{
		_bstr_t _tdate(var);
		strValue.Format(_T("%s"),(const char*)_tdate);
		break;
		}
	case VT_BOOL:
		strValue.Format(_T("%d"),var.boolVal);
		break;
	}
*/

	return strValue;
}

/*****************************************\
	 VARIANT TO CSTRING OBJECT
\*****************************************/
BOOL CAdoRecordset::CStringToVariant(const CString strValue, int nType, VARIANT *var)
{
	_bstr_t _value(strValue);
	*var = variant_t(_value);
	return TRUE;
}

/***************************************\
	 取出表中的某个或全部字段名称
\***************************************/
BOOL CAdoRecordset::GetFieldsName(CStringArray &arryField,const int nId)
{
	Field *pField=NULL;
	arryField.RemoveAll();
	if (nId>-1)
	{
		pField = m_pRecordset->Fields->GetItem((long)nId);
		arryField.Add(pField->GetName());
		return TRUE;
	}
	//If nId<=-1 then get all field name
	for(int i=0;i<m_pRecordset->Fields->GetCount();i++)
	{
		pField = m_pRecordset->Fields->GetItem((long)i);
		arryField.Add(pField->GetName());
	}
	return TRUE;

}

/************************************************************\
  Function： GetRecordValue()
  PARA:
      nId: 字段项,如果nId<=-1,则提取所有项。
	  pFields:字段名称。
	  
  取出表中当前行某一个字段或所有字段的值，输出为String数组
\************************************************************/
BOOL CAdoRecordset::GetRecordValue(CStringArray &arryValue,const int nId)
{
	Field *pField=NULL;
	arryValue.RemoveAll();
	_variant_t varValue;

	if (nId>-1)
	{
		pField = m_pRecordset->Fields->GetItem((long)nId);
		pField->get_Value(&varValue);
		arryValue.Add(VariantToCString(varValue));
		return TRUE;
	}
	// Get all fields value of current record.
	for(int i=0;i<m_pRecordset->Fields->GetCount();i++)
	{
		pField = m_pRecordset->Fields->GetItem((long)i);
		pField->get_Value(&varValue);
		arryValue.Add(VariantToCString(varValue));
	}

	return TRUE;
}
/************************************************************\
  Function： ModifyRecord()
  PARA:
      arryField: 要修改的字段名称。
	  arryValue: 该字段修改后的值。
	  pRecordset:数据集指针。
	  
  修改表中当前行某一个字段或所有字段的值
\************************************************************/
BOOL CAdoRecordset::ModifyRecord(const CStringArray &arryField,const CStringArray &arryValue)
{
	int nPos(0);
	CStringArray arryAllField;
	if (!GetFieldsName(arryAllField))
		return FALSE;
	Field *pField = NULL;
	VARIANT _var;
	for (int n=0;n<arryField.GetSize();n++)
	{
		nPos = LocateFieldPos(&arryAllField,arryField.GetAt(n));
		if (nPos==-1) continue;
		pField = m_pRecordset->Fields->GetItem(long(nPos));
		CStringToVariant(arryValue.GetAt(nPos),0,&_var);
		if (FAILED(pField->put_Value(_var))) 
			return FALSE;
	}

	return TRUE;
}

/************************************************************\
  Function： AddNewRecord()
  PARA:
      arryField: 新记录数据对应的字段名称。
	  arryValue: 新记录数据相应字段的值。
	  pRecordset:数据集指针。
	  
  向表中添加一个新记录
\************************************************************/
BOOL CAdoRecordset::AddNewRecord(const CStringArray &arryField,const CStringArray &arryValue)
{
	if (FAILED(m_pRecordset->AddNew())) return FALSE;
	
	return ModifyRecord(arryField,arryValue);
}

void CAdoRecordset::Attach(_RecordsetPtr _pRecordset)
{
	VERIFY(_pRecordset!=NULL);
	/////////////////////////
	if (_pRecordset!=m_pRecordset)
	{
		ReleaseInstance();
		m_pRecordset = _pRecordset;
	}
}

BOOL CAdoRecordset::CreateInstance()
{
	if (m_pRecordset==NULL)
	{
		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if (!SUCCEEDED(hr)) return FALSE;
	}
	return TRUE;
}

BOOL CAdoRecordset::ReleaseInstance()
{
	if (m_pRecordset!=NULL)
	{
		Close();
		m_pRecordset.Release();
	}
	return TRUE;
}

BOOL CAdoRecordset::Delete(RowSelEnum rowSel)
{
	HRESULT hr;
	switch(rowSel)
	{
	case rowSelCur:
		hr=m_pRecordset->Delete(adAffectCurrent);
		break;
	case rowSelAll:
		hr=m_pRecordset->Delete(adAffectAll);
		break;
	default:
		return FALSE;
	}

	return SUCCEEDED(hr);
}

void CAdoRecordset::Update()
{
	m_pRecordset->UpdateBatch(adAffectAll);
}

int CAdoRecordset::LocateFieldPos(CStringArray *parryAllField, CString strField)
{
	int nCount =parryAllField->GetSize();
	int n = 0;
	for (n=0;n<nCount;n++)
		if (parryAllField->GetAt(n)==strField)
			break;

	if (n >= nCount) return -1;
	else  return n;

}

/*
CAdoDB在这段代码中，没有加入try-catch异常保护，有时会产生异常。
产生异常的原因有可能是：[Microsoft][ODBC SQL Server Driver]查询超时已过期
*/
BOOL CAdoRecordset::IsEOF()
{ 
	BOOL bIsEOF=TRUE;
	try
	{ 
		bIsEOF = m_pRecordset->adoEOF;
	}
	catch(_com_error &e)
	{
		::OutputDebugString((LPCTSTR)e.Description());
	}
	return bIsEOF;
}

BOOL CAdoRecordset::IsBOF()
{ 
	BOOL bIsBOF=TRUE;
	try
	{ 
		bIsBOF = m_pRecordset->adoBOF;
	}
	catch(_com_error &e)
	{
		::OutputDebugString((LPCTSTR)e.Description());
	}
	return bIsBOF;
}