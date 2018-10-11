#pragma once

#include"Nodetext.h"
#include"cpp11_std_files.h"
#include "afxwin.h"
#include "AdoDb.h"

// TextServiceDialog 对话框

class TextServiceDialog : public CDialogEx
{
	DECLARE_DYNAMIC(TextServiceDialog)

public:
	TextServiceDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TextServiceDialog();

	virtual void OnFinalRelease();

// 对话框数据
	enum { IDD = IDD_DIALOG_TextService };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()


public:
	std::wstring data;
	CNodeText* selected_node;
	

	
public://properties setting
	unsigned Get_exit_window(){return exit_window;};
	void Set_exit_window(unsigned value){
		if(value!=exit_window){
			exit_window=value;
		}
	};

public:
	void OnBnClickedOk();
	void get_data_fr_file();
	void get_data_fr_mdb();
	void OnBnClickedButtonSerialPort();
	afx_msg void OnClose();
	afx_msg void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();

private:
	CAdoDB m_ado;
	CAdoRecordset m_recordset;
	unsigned exit_window;

private:
	void update_data(const CString& data);
	bool initMDB();
	void initSerialPort_Control();
	bool wait_Tcp();

protected:
	

public:
	// 跟界面inputtext控件绑定
	CString ui_text_data;     //文本替换输入框（IDC_EDIT_INTERVAL）
	CString m_filePath;
	CString selected_Combvalue;
	CComboBox m_fields;
	CString m_data_fr_tcp;
	int m_modeNum;		//模板号编辑选择; IDC_EDIT6
	int m_changeMdoe;	//替换当前选中文本；IDC_RADIO1
	CString m_ip;
	CString m_ip_port;

	//boost::atomic<bool> exit_dlg;

	std::vector<std::function<bool(void)> > OnClosing;
	
	afx_msg void on_ModelRadioClick();
	
	CComboBox m_Comb_bits;
	CComboBox m_Comb_stobit;
	CComboBox m_Comb_parity;
	CComboBox m_Comb_flowc;
	UINT m_bortrate;
	UINT m_test_spiner;
	CString m_SerialNum;
	CString m_port_down_data;
	CString m_port_up_value;
};
