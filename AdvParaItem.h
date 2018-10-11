#pragma once

#include "Layer.h"

class CAdvParaItem : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CAdvParaItem)
public:
	CAdvParaItem(const CString& strName, LPCTSTR lpszDescr);
	virtual ~CAdvParaItem(void);
	virtual void OnClickButton(CPoint point);
	void SetFunction(LPVOID pFunc);
	void SetAdvPara(PARA *para);
	void GetAdvPara(PARA *para);
private:
	LPVOID m_pShowAdvParaDlg;
	PARA m_para;
};
