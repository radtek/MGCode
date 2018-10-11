#pragma once

#include"cpp11_std_files.h"
#include"TextService.h"
using namespace std;
class text_service_file{
private:
	wfstream fs;
	text_service_file(){};

public:
	wstring file_name;

public:
	void read_frm_file(wstring& content,ios:: ios_base::openmode mode){
		try
		{
			fs.open(file_name,ios_base::in|mode);
			if(fs.is_open()){
				if(mode&ios_base::binary){
					//暂时没实现二进制输入
				}else{
					locale loc("");
					fs.imbue(loc);
					wostringstream wos;
					wos<<fs.rdbuf();
					wstring rlt=wos.str();
					content+=rlt;
					textLsReplae(content,L"\n",L"\r\n");

					//another way,but with bad efficiency,just comment it for now
					//wstring temp;
					//while(fs){
					//	getline(fs,temp);
					//	content+=std::move(temp);
					//	temp.clear();
					//}
				}
			}
			fs.close();
		}catch(const exception& e){
			fs.close();
			auto ms=e.what();
		}
	}

	void write_to_file(wstring& contenet,ios_base::openmode mode){
		//no implemention now
	}

	static text_service_file& Instance()
	{
		static text_service_file singleton;
		return singleton;
	}

};
