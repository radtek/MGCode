#include "stdafx.h"
#include "ManageLayPara.h"
#include "hl.h"
#include "MFCProertyItem.h"

CManageLayPara::CManageLayPara(void) :
m_nCount(1),m_hAdvParaModule(NULL),m_pShowAdvParaDlg(NULL)
{
	memset(m_pDefaultPara, 0, sizeof(PARA));
}

CManageLayPara::~CManageLayPara(void)
{
	ClearArray();
	if (m_hAdvParaModule != NULL)
	{
		FreeLibrary(m_hAdvParaModule);
	}
}

void CManageLayPara::InitPropertyGrid(CMFCPropertyGridCtrl &propertyGrid)
{
	DSF_TRACE("Enter InitPropertyGrid\n");
	propertyGrid.EnableHeaderCtrl(FALSE);
	propertyGrid.EnableDescriptionArea();
	propertyGrid.SetVSDotNetLook();
//	propertyGrid.MarkModifiedProperties();
	CMFCPropertyGridProperty::m_strFormatDouble = _T("%lg");
	CMFCPropertyGridProperty::m_strFormatFloat = _T("%g");
	int j = 0;
	int nGrow = sizeof(double);
	BOOL bAddMarkCount = FALSE;
	for (int i=0; i<m_arraySectionName.GetSize(); i++)
	{
		CString &strSectionName = m_arraySectionName.GetAt(i);
		CMFCPropertyGridProperty *p = new CMFCPropertyGridProperty(strSectionName);
		propertyGrid.AddProperty(p);
		for (;j<m_arrayParaType.GetSize(); j++)
		{
			const ItemInfo &info = m_arrayParaType.GetAt(j);
			if (info.nSectionIndex == i)
			{
				switch(info.nItemType)
				{
				case TYPE_INT:
					{
						double *pDouble = (double *)(m_pDefaultPara+nGrow);
						CString &strItemName = m_arrayParaName.GetAt(j);
						COleVariant v;
						v.vt = VT_I4;
						v.lVal = long(*pDouble);
						CMFCPropertyGridProperty *pItem = new CMFCPropertyGridProperty(strItemName, v, strItemName);
						pItem->SetData(j);
						//pItem->SetOwnerWnd(propertyGrid.GetOwner());
						p->AddSubItem(pItem);
						m_propertyItemList.Add(pItem);
						pItem->Show(info.bShow);
						nGrow += sizeof(double);
					}
					break;
				case TYPE_FLOAT:
					{
						double *pFloat = (double*)(m_pDefaultPara+nGrow);
						double dbTemp = (double)*pFloat;
						CString &strItemName = m_arrayParaName.GetAt(j);
						COleVariant v;
						v.vt = VT_R8;
						v.dblVal = dbTemp;
						CMFCPropertyGridProperty *pItem = new CMFCPropertyGridProperty(strItemName, v, strItemName);	
						pItem->SetData(j);
						//pItem->SetOwnerWnd(propertyGrid.GetOwner());
						p->AddSubItem(pItem);
						m_propertyItemList.Add(pItem);
						pItem->Show(info.bShow);
						nGrow += sizeof(double);
					}
					break;
				case TYPE_ENUM:
					{
						int nEnumType = info.nEnumType;
						if (info.nEnumType >= m_arrayEnum.GetSize())
						{
							nEnumType = m_arrayEnum.GetSize()-1;
						}
						int *pInt = (int*)(m_pDefaultPara+nGrow);
						CString &strItemName = m_arrayParaName.GetAt(j);

						StringList *pList = m_arrayEnum.GetAt(nEnumType);
						POSITION pos1 = pList->GetHeadPosition();
						int i=0;
						CString strCur;
						while(pos1)
						{
							CString &str = pList->GetNext(pos1);
							if (i == *pInt)
							{
								strCur = str;
							}
							i++;
						}
						if (*pInt >= pList->GetCount())
						{
							strCur = pList->GetHead();
						}

						CMFCPropertyGridProperty *pItem = new CMFCPropertyGridProperty(strItemName, strCur, strItemName);
						pItem->SetData(j);
						//pItem->SetOwnerWnd(propertyGrid.GetOwner());
						pItem->AllowEdit(FALSE);
						p->AddSubItem(pItem);
						m_propertyItemList.Add(pItem);
						pItem->Show(info.bShow);
						POSITION pos = pList->GetHeadPosition();
						while (pos)
						{
							CString &str = pList->GetNext(pos);
							pItem->AddOption(str);
						}
						nGrow += sizeof(int);
					}
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
//	CMFCPropertyGridProperty *pSection = new CMFCPropertyGridProperty(m_strAdvSectionName);
//	propertyGrid.AddProperty(pSection);
//	m_pAdvItem = new CAdvParaItem(m_strAdvItemName, m_strAdvItemName);
//	pSection->AddSubItem(m_pAdvItem);
	DSF_TRACE("Leave InitPropertyGrid\n");
}

void CManageLayPara::GetPara(BYTE *pBUFF, int nSize, CMFCPropertyGridCtrl &propertyGrid)
{
	DSF_TRACE("Enter GetPara\n");
//	m_pAdvItem->GetAdvPara((PARA*)pBUFF);
	int nGrow = 0;
	for (int i=0; i<m_arrayParaType.GetSize(); i++)
	{
		const ItemInfo &info = m_arrayParaType.GetAt(i);
		switch(info.nItemType)
		{
		case TYPE_ENUM:
			{
				if(info.bShow)
				{
					int *pInt = (int*)(pBUFF + nGrow);
					const COleVariant &v = m_propertyItemList.GetAt(i)->GetValue();
					CString strVal = _bstr_t(v.bstrVal);
					for (int j=0; j<m_propertyItemList.GetAt(i)->GetOptionCount(); j++)
					{
						CString strTemp = m_propertyItemList.GetAt(i)->GetOption(j);
						if (strVal.Compare(strTemp) == 0)
						{
							*pInt = j;
							break;
						}
					}
				}
				nGrow += sizeof(int);
			}
			break;
		case TYPE_INT:
			{
				if(info.bShow)
				{
					double *pDouble = (double *)(pBUFF+nGrow);
					const COleVariant &v = m_propertyItemList.GetAt(i)->GetValue();
					*pDouble = v.lVal;
				}
				nGrow += sizeof(double);
			}
			break;
		case TYPE_FLOAT:
			{
				if(info.bShow)
				{
					double *pFloat = (double *)(pBUFF + nGrow);
					double dbTemp = 0.0;
					const COleVariant &v = m_propertyItemList.GetAt(i)->GetValue();
					dbTemp = v.dblVal;
					*pFloat = (double)dbTemp;
				}
				nGrow += sizeof(double);
			}
			break;
		}
		if (nGrow >= nSize)
		{
			break;
		}
	}
	DSF_TRACE("Leave GetPara\n");
}
void CManageLayPara::SetPara(BYTE *pBUFF, int nSize, CMFCPropertyGridCtrl &propertyGrid)
{
	DSF_TRACE("Enter SetPara\n");
	COleVariant v;
//	m_pAdvItem->SetAdvPara((PARA*)pBUFF);
	int nGrow = 0;
	for (int i=0; i<m_arrayParaType.GetSize(); i++)
	{
		const ItemInfo &info = m_arrayParaType.GetAt(i);
		switch(info.nItemType)
		{
		case TYPE_ENUM:
			{
				if(info.bShow)
				{
					const int *pInt = (int*)(pBUFF + nGrow);
					CString strValue = m_propertyItemList.GetAt(i)->GetOption(*pInt);
					m_propertyItemList.GetAt(i)->SetValue(_variant_t(strValue));
				}
				nGrow += sizeof(int);
			}
			break;
		case TYPE_INT:
			{
				if(info.bShow)
				{
					double *pDouble = (double *)(pBUFF + nGrow);
					v.vt = VT_I4;
					v.lVal = long(*pDouble);
					m_propertyItemList.GetAt(i)->SetValue(v);
				}
				nGrow += sizeof(double);
			}
			break;
		case TYPE_FLOAT:
			{
				if(info.bShow)
				{
					const double *pFloat = (double*)(pBUFF + nGrow);
					const double dbTemp = (double)(*pFloat);
					v.vt = VT_R8;
					v.dblVal = dbTemp;
					m_propertyItemList.GetAt(i)->SetValue(v);
				}
				nGrow += sizeof(double);
			}
			break;
		}
		if (nGrow >= nSize)
		{
			break;
		}
	}
	DSF_TRACE("Leave SetPara\n");
}
void CManageLayPara::LoadParaInfo()
{
	DSF_TRACE("Enter LoadParaInfo\n");
	TCHAR pszTemp[1024];
	GetExepath(pszTemp);
	CString strPath = pszTemp;
	CString strSection;
	CString strItem;
	CString strParaFile = strPath + g_sys.m_strCardName + "\\" + c_strParaIni;
	//CString strParaDescFile = strPath + g_sys.m_strCardName + "\\" + c_strParaDescIni;

	CString strParaDescFileFirst = strPath + g_sys.m_strCardName + "\\";// + c_strParaDescIni;
	CString strParaDescFile;
	strParaDescFile.Format(_T("%sMarkParaDesc%x.ini"),strParaDescFileFirst,g_sys.m_langID);
	ItemInfo itemInfo;
	CString strItemDesc;
	CString strSectionDesc;
	ClearArray();
	for(int i=0;;i++)
	{
		itemInfo.nSectionIndex = i;
		strSection.Format("%d", i);
		for (int j=0;;j++)
		{
			itemInfo.nItemIndex = j;
			strItem.Format("%d_%d", i, j);
			DWORD dwRet1 = GetPrivateProfileString(strSection, strItem, "", pszTemp, 1024, strParaFile);
			if (dwRet1 == 0)
			{
				break;
			}
			CString strItemType,strEnumType,strShow;
			if(!AfxExtractSubString(strItemType, pszTemp, 0, ','))
			{
				break;
			}
			if(!AfxExtractSubString(strEnumType, pszTemp, 1, ','))
			{
				break;
			}
			if (!AfxExtractSubString(strShow, pszTemp, 2, ','))
			{
				break;
			}
			itemInfo.nItemType = atoi(strItemType);
			itemInfo.nEnumType = atoi(strEnumType);
			itemInfo.bShow = atoi(strShow);
			DWORD dwRet2 = GetPrivateProfileString("item", strItem, "", pszTemp, 1024, strParaDescFile);
			if (dwRet2 == 0)
			{
				break;
			}
			strItemDesc = pszTemp;
			m_arrayParaType.Add(itemInfo);
			m_arrayParaName.Add(strItemDesc);
		}
		DWORD dwRet3 = GetPrivateProfileString("section", strSection, "", pszTemp, 1024, strParaDescFile);
		if (dwRet3 == 0)
		{
			break;
		}
		strSectionDesc = pszTemp;
		m_arraySectionName.Add(strSectionDesc);

	}
	CString strEnumItem;
	for (int i=0;; i++)
	{
		strEnumItem.Format("%d", i);
		DWORD dwRet4 = GetPrivateProfileString("enum", strEnumItem, "", pszTemp, 1024, strParaDescFile);
		if (dwRet4 == 0)
		{
			break;
		}
		StringList *pList = new StringList;
		int j = 0;
		while(AfxExtractSubString(strEnumItem, pszTemp, j, ','))
		{
			j++;
			pList->AddTail(strEnumItem);
		}
		m_arrayEnum.Add(pList);
	}
	GetPrivateProfileString("list", "list", "1,2,3", pszTemp, 1024, strParaFile);
	CString strLayListItem;
	for (int i=0; i<3; i++)
	{
		if (AfxExtractSubString(strLayListItem, pszTemp, i, ','))
		{
			m_nShowListIndex[i] = atoi(strLayListItem);
		}
		else
		{
			m_nShowListIndex[i] = 0;
		}
	}
//	GetPrivateProfileString("advance", "sectionName", "Advance", pszTemp, 1024, strParaDescFile);
//	m_strAdvSectionName = pszTemp;
//	GetPrivateProfileString("advance", "itemName", "other", pszTemp, 1024, strParaDescFile);
//	m_strAdvItemName = pszTemp;
	DSF_TRACE("Leave LoadParaInfo\n");
}

void CManageLayPara::LoadDefaultPara()
{
	CLayer ly;
	char szPath[256];
	GetExepath(szPath);
	char szFileName[256];
	sprintf(szFileName, "%s%s\\%s", szPath, g_sys.m_strCardName, "default.yh");

	FILE *fp;
	if((fp=fopen(szFileName,"rb"))==NULL)
	{
		PARA para;
		ly.m_para=para;//³õÊ¼»¯
	}
	else
	{
		ly.Serialize(fp, FALSE);
		fclose(fp);
	}
	if (m_propertyItemList.GetCount()>0)
	{
		BYTE *pBUFF = (BYTE*)&ly.m_para;
		int nGrow = 0;
		for (int i=0; i<m_arrayParaType.GetSize(); i++)
		{
			const ItemInfo &info = m_arrayParaType.GetAt(i);
			switch(info.nItemType)
			{
			case TYPE_ENUM:
				{
					if(info.bShow)
					{
						const int *pInt = (int*)(pBUFF + nGrow);
						CString strValue = m_propertyItemList.GetAt(i)->GetOption(*pInt);
						m_propertyItemList.GetAt(i)->SetOriginalValue(_variant_t(strValue));
					}
					nGrow += sizeof(int);
				}
				break;
			case TYPE_INT:
				{
					if(info.bShow)
					{
						double *pDouble = (double *)(pBUFF + nGrow);
						COleVariant v;
						v.vt = VT_I4;
						v.lVal = long(*pDouble);
						m_propertyItemList.GetAt(i)->SetOriginalValue(v);
					}
					nGrow += sizeof(double);
				}
				break;
			case TYPE_FLOAT:
				{
					if(info.bShow)
					{
						const double *pFloat = (double*)(pBUFF + nGrow);
						const double dbTemp = (double)(*pFloat);
						COleVariant v;
						v.vt = VT_R8;
						v.dblVal = dbTemp;
						m_propertyItemList.GetAt(i)->SetOriginalValue(v);
					}
					nGrow += sizeof(double);
				}
				break;
			}
			if (nGrow >= sizeof(PARA))
			{
				break;
			}
		}
	}
}

void CManageLayPara::ClearArray()
{
	for (int i=0; i<m_arrayEnum.GetSize(); i++)
	{
		StringList* pList = m_arrayEnum.GetAt(i);
		delete pList;
	}
	m_arrayEnum.RemoveAll();
	m_arrayParaName.RemoveAll();
	m_arrayParaType.RemoveAll();
	m_arraySectionName.RemoveAll();
}

CString CManageLayPara::GetLayItemName(int index)
{
	if (index > 2)
	{
		return "";
	}
	if (m_nShowListIndex[index]>=m_arrayParaName.GetSize())
	{
		return "";
	}
	CString strTemp = m_arrayParaName.GetAt(m_nShowListIndex[index]);
	int i = strTemp.ReverseFind('(');
	strTemp = strTemp.Left(i);
	return strTemp;
}
CString CManageLayPara::GetLayItemValue(int index, BYTE *pBuff, int nSize)
{
	if (index > 2)
	{
		return "";
	}
	int nGrow = 0;
	for (int i=0; i<m_arrayParaType.GetSize(); i++)
	{
		const ItemInfo &info = m_arrayParaType.GetAt(i);
		switch(info.nItemType)
		{
		case TYPE_INT:
		case TYPE_ENUM:
			{
				double *pDouble = (double *)(pBuff + nGrow);
				if (i == m_nShowListIndex[index])
				{
					CString strTemp;
					strTemp.Format("%d", long(*pDouble));
					return strTemp;
				}
				nGrow += sizeof(double);
			}
			break;
		case  TYPE_FLOAT:
			{
				double *pFloat = (double*)(pBuff+nGrow);
				if (i == m_nShowListIndex[index])
				{
					CString strTemp;
					strTemp.Format("%0.0f", *pFloat);
					return strTemp;
				}
				nGrow+=sizeof(double);
			}
			break;
		}
		if (nGrow>=nSize)
		{
			break;
		}
	}
	return "";
}

void CManageLayPara::LoadAdvParaLib()
{
	/*
	TCHAR pszTemp[1024];
	GetExepath(pszTemp);
	CString strFile;
	strFile.Format("%s%s\\advparaset.dll", pszTemp, g_sys.m_strCardName);
	m_hAdvParaModule = LoadLibrary(strFile);
	if (m_hAdvParaModule != NULL)
	{
		m_pShowAdvParaDlg = GetProcAddress(m_hAdvParaModule, "ShowAdvParaSetDlg");
		m_pAdvItem->SetFunction(m_pShowAdvParaDlg);
	}
	*/
}