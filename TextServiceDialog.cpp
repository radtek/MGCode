// TextServiceDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "HL.h"
#include "TextServiceDialog.h"
#include "afxdialogex.h"
#include "HLView.h"
#include"TextServiceFile.h"
#include"SocketClient.h"

// TextServiceDialog 对话框

IMPLEMENT_DYNAMIC(TextServiceDialog, CDialogEx)

TextServiceDialog::TextServiceDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(TextServiceDialog::IDD, pParent)
	, ui_text_data(_T(""))
	, m_filePath(_T(""))
	, selected_Combvalue(_T(""))
	, m_data_fr_tcp(_T(""))
	, m_ip(_T(""))
	, m_ip_port(_T(""))
	, m_changeMdoe(0)
	, m_modeNum(0)
	, m_bortrate(0)
	, m_test_spiner(0)
	, m_SerialNum(_T(""))
	, m_port_down_data(_T(""))
	, m_port_up_value(_T(""))
{

	EnableAutomation();
}

TextServiceDialog::~TextServiceDialog()
{
}

void TextServiceDialog::OnFinalRelease()
{
	// 释放了对自动化对象的最后一个引用后，将调用
	// OnFinalRelease。基类将自动
	// 删除该对象。在调用该基类之前，请添加您的
	// 对象所需的附加清理代码。

	CDialogEx::OnFinalRelease();
}

void TextServiceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, ui_text_data);  //文本替换绑定
	DDX_Text(pDX, IDC_EDIT_File_Dir, m_filePath);
	DDX_CBString(pDX, IDC_COMBO1, selected_Combvalue);
	DDX_Control(pDX, IDC_COMBO1, m_fields);
	DDX_Text(pDX, IDC_EDIT4, m_data_fr_tcp);
	DDX_Text(pDX, IDC_EDIT1, m_ip);
	DDX_Text(pDX, IDC_EDIT3, m_ip_port);
	DDX_Radio(pDX, IDC_RADIO1, m_changeMdoe);
	DDX_Text(pDX, IDC_EDIT6, m_modeNum);
	DDX_Control(pDX, IDC_COMBO3, m_Comb_bits);
	DDX_Control(pDX, IDC_COMBO5, m_Comb_stobit);
	DDX_Control(pDX, IDC_COMBO7, m_Comb_parity);
	DDX_Control(pDX, IDC_COMBO6, m_Comb_flowc);
	DDX_Text(pDX, IDC_EDIT7, m_bortrate);
	DDX_Text(pDX, IDC_EDIT5, m_test_spiner);
	DDX_Text(pDX, IDC_EDIT8, m_SerialNum);
	DDX_Text(pDX, IDC_EDIT9, m_port_down_data);
	DDX_Text(pDX, IDC_EDIT10, m_port_up_value);
}


BEGIN_MESSAGE_MAP(TextServiceDialog, CDialogEx)
	//ON_BN_CLICKED(IDC_BUTTON1, &CTextEditDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &TextServiceDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_FileText,&TextServiceDialog::get_data_fr_file)
	ON_BN_CLICKED(IDC_BUTTON_SQLText,&TextServiceDialog::get_data_fr_mdb)
	ON_BN_CLICKED(IDC_BUTTON1, &TextServiceDialog::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_PORT, &TextServiceDialog::OnBnClickedButtonSerialPort)
	
	ON_CBN_SELCHANGE(IDC_COMBO1, &TextServiceDialog::OnCbnSelchangeCombo1)

	ON_WM_CLOSE(WM_CLOSE,&TextServiceDialog::OnClose)
	ON_WM_DESTROY(WM_DESTROY,&TextServiceDialog::OnDestroy)
	ON_BN_CLICKED(IDC_RADIO1, &TextServiceDialog::on_ModelRadioClick)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(TextServiceDialog, CDialogEx)
END_DISPATCH_MAP()

// 注意: 我们添加 IID_ITextServiceDialog 支持
//  以支持来自 VBA 的类型安全绑定。此 IID 必须同附加到 .IDL 文件中的
//  调度接口的 GUID 匹配。

// {8339CFAC-5B7A-45A3-9644-DDE7A6DDE261}
static const IID IID_ITextServiceDialog =
{ 0x8339CFAC, 0x5B7A, 0x45A3, { 0x96, 0x44, 0xDD, 0xE7, 0xA6, 0xDD, 0xE2, 0x61 } };

BEGIN_INTERFACE_MAP(TextServiceDialog, CDialogEx)
	INTERFACE_PART(TextServiceDialog, IID_ITextServiceDialog, Dispatch)
END_INTERFACE_MAP()


static boost::atomic<bool> async_Tcp_running(false);

BOOL TextServiceDialog::OnInitDialog() {
	CDialogEx::OnInitDialog();
	//initMDB();    //数据源读取
	initSerialPort_Control();
	this->Set_exit_window(0);
	this->OnClosing.push_back(std::bind(&TextServiceDialog::wait_Tcp,this));
	return true;
}

extern DBPARA		g_para;
bool TextServiceDialog::initMDB(){
	
	if(!m_ado.ConnectToDB(g_para)) return false;

	CString sql(_T("select * from test"));
	m_ado.OpenSQLQuery((_bstr_t)sql,m_recordset);

	CStringArray fields;
	m_recordset.GetFieldsName(fields,-1);

	for(int i=0;i<fields.GetSize();i++){
		m_fields.AddString( fields.GetAt(i));
	}
	m_fields.SetCurSel(0);
	return true;
}

void TextServiceDialog::initSerialPort_Control() {
	CString bits[] = { "5","6","7","8" };
	std::all_of(bits, bits+4 ,[&](const CString& a)mutable ->bool {
		m_Comb_bits.AddString(a);
		return true;
	}); m_Comb_bits.SetCurSel(0);

	CString stop_bits[] = { "1","1.5","2" };
	std::for_each(stop_bits, stop_bits + 3, [&](const CString& a)mutable->void {
		m_Comb_stobit.AddString(a);
	}); m_Comb_stobit.SetCurSel(0);


	CString flow_controls[] = { "None","SoftWare","HardWare"};
	for (int i = 0; i < 3;i++) {
		m_Comb_flowc.AddString(flow_controls[i]);
	}; m_Comb_flowc.SetCurSel(0);

	CString paritys[] = { "None","Odd","Even" };
	for (int i = 0; i < 3; i++) {
		m_Comb_parity.AddString(paritys[i]);
	};m_Comb_parity.SetCurSel(0);
}

void TextServiceDialog::OnClose(){
	auto canExit=!std::any_of(OnClosing.begin(),OnClosing.end(),[](const std::function<bool(void)>& t)->bool{return !t();});
	if(!canExit) return;

	CDialogEx::OnClose();
}

void TextServiceDialog::OnCancel(){
		bool canExit=true;
	for(int i=0;i<OnClosing.size();i++){
		canExit=OnClosing[i]();
		if(!canExit){
			break;
		}
	}
	if(!canExit) return;
	CDialogEx::OnCancel();
}

void TextServiceDialog::OnDestroy(){

		bool canExit=true;
	for(int i=0;i<OnClosing.size();i++){
		canExit=OnClosing[i]();
		if(!canExit){
			break;
		}
	}
	if(!canExit) return;

	CDialogEx::OnDestroy();
}

bool TextServiceDialog::wait_Tcp(){
		if(async_Tcp_running)
		//if(false)
			return false;
		return true;
}


// TextServiceDialog 消息处理程序
void TextServiceDialog::OnBnClickedOk(){
	UpdateData(true);
	update_data(ui_text_data);
	//CDialogEx::OnOK();
	Set_exit_window(1);
}

void TextServiceDialog::update_data(const CString& data)
{
	auto a = m_changeMdoe;
	//更改单一文本对象				
	if (m_changeMdoe == 0) {
		if (this->selected_node == nullptr) return;
		this->selected_node->m_strText = data;
		selected_node->UpadateMgrText();

		//手动刷新界面	20180626		
		CHLDoc	*pDoc = (CHLDoc*)CHLView::m_pActiveView->GetDocument();
		////pFWnd->UpdateWindow();
		//pDoc->UpdateAllViews(NULL);

		CTree *pTree = new CTree(this->selected_node);
		CQuad quad = pTree->CalLimit();
		pDoc->SetTrackerQuad(quad);
		//pDoc->SelectDraw(pTree);
	}
	else if (m_changeMdoe == 1) {//按照模板号更改文本			2018.7.4
		CFrameWnd *pFWnd = (CFrameWnd*)AfxGetMainWnd();
		CHLView   *pView = (CHLView*)((pFWnd->GetActiveFrame())->GetActiveView());
		CHLDoc *pDoc = (CHLDoc*)pView->GetDocument();		//Mfc 获取win view doc 的通用方式
		//CHLDoc	*pDoc = (CHLDoc*)CHLView::m_pActiveView->GetDocument();
		pDoc->GetChain().ChangeText(data, CString(to_string((LONGLONG)m_modeNum).c_str()));
		pDoc->UpdateAllViews(NULL);

	}
	return ;
}

void TextServiceDialog::get_data_fr_file(){
	UpdateData(true);
	auto& text_s_f=text_service_file::Instance();
	auto file=text_s_f.file_name;
	CStringW strw=CStrA2CStrW(m_filePath);
	text_s_f.file_name=strw;
	std::wstring content;
	text_s_f.read_frm_file(content,0);
	text_s_f.file_name=file;
	//CStringW wdata(content.c_str());
	update_data(CString(content.c_str()));
	Set_exit_window(1);
}


void TextServiceDialog::get_data_fr_mdb(){         //数据库文本
	CString strCBText;
	m_fields.GetLBText(m_fields.GetCurSel(), strCBText);

	CStringArray fields;
	m_recordset.GetFieldsName(fields,-1);
	int index=m_recordset.LocateFieldPos(&fields,strCBText);

	CStringArray contends;
	if(index>=0){
		m_recordset.GetRecordValue(contends,index);
	}else{
		contends.Add("No data");
	}

	update_data(contends.GetAt(0));
	Set_exit_window(1);
}


void TextServiceDialog::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}



void TextServiceDialog::OnBnClickedButton1()  //TCP
{

	static CSocketClient worker;
	UpdateData(true);
	if(worker.IsConnected()) return;

	std::string ip_port=m_ip_port;
	worker.SetSocket(1,m_ip,  atoi(ip_port.c_str()));
	if(worker.Connet()){

		//AfxMessageBox(_T("succesfully connect to serever"));
		char* data=nullptr;
		auto act=[&]()mutable->void{
			async_Tcp_running=true;
			int data_length=0;
			Redo:
			worker.Receive(&data_length);
			if(data_length>0){
				std::string data_back( worker.GetData());
				data=worker.GetData();
				//AfxMessageBox(_T("get data"));
				
			}else{
				boost::this_thread::sleep_for(boost::chrono::duration<int,boost::ratio<1> >(1)); 
				goto Redo;
			}
			worker.Close();

			m_data_fr_tcp=CString(data);
			UpdateData(false);
			this->update_data(m_data_fr_tcp);
			this->Set_exit_window(1);

			async_Tcp_running=false;
		};

		boost::thread tcp_handler(act);
		tcp_handler.detach();

		//m_data_fr_tcp=CString(data);
		//UpdateData(false);
		//this->update_data(m_data_fr_tcp);
		//this->exit_window=1;
	}

}

void TextServiceDialog::OnBnClickedButtonSerialPort(){

	UpdateData(true);

	/*此处代码测试Boost asio 串口通信的代码效果	 2018.7.5
	using namespace boost::asio;
	io_service iosev;
	serial_port* sp=new serial_port(iosev, m_SerialNum);
	
	sp->set_option(serial_port::baud_rate(m_bortrate));
	sp->set_option(serial_port::flow_control(serial_port::flow_control::type(m_Comb_flowc.GetCurSel())));
	sp->set_option(serial_port::parity(serial_port::parity::type(m_Comb_parity.GetCurSel())));
	sp->set_option(serial_port::stop_bits(serial_port::stop_bits::type(m_Comb_stobit.GetCurSel())));
	sp->set_option(serial_port::character_size(m_Comb_bits.GetCurSel()+5));

	write(*sp,buffer("hello world",12));
	//char buf[10];
	std::vector<char> buf(10);
	boost::asio::read(*sp,buffer(buf.data(), buf.size()));
	buf.push_back('\0');
	std::ostringstream a_ost; //a_ost.imbue(std::locale());
	a_ost << buf.data();
	std::string back_str = a_ost.str();

	sp->close();
	delete sp;
	iosev.run();
	update_data(CString(back_str.data()));
	//*/

	std::string down_data = m_port_down_data;
	std::vector<std::string> con;
	string_split(down_data, ' ', con);
	if (con.size() < 1) return;

	std::vector<int> down_i_data;
	std::back_insert_iterator<std::vector<int> > bin(down_i_data);
	std::transform(con.begin(), con.end(), bin,[](std::string& str)->int{
		return std::atoi(str.c_str());
	});




	Set_exit_window(1);
}

void TextServiceDialog::on_ModelRadioClick()
{
	//UpdateData(true);     //为之何用
}

