#include "stdafx.h"
#include"TextService.h"
#include"cpp11_std_files.h"

void textLsReplae(std::wstring& data,const std::wstring& beforeText,const std::wstring& afterText){
	int pos=0;
	while(pos<data.size()){
		pos=data.find(beforeText,pos);
		data.replace(pos,beforeText.size(),afterText);
		pos+=afterText.size()+1;
		if(pos>100000){
			break;
		}
	}
}

void string_split(std::string data, char c, std::vector<std::string> con) {
	int begin = 0, next = 0;
	while (next < data.size()) {
		next = data.find_first_of(c, begin);
		con.push_back(data.substr(begin, next));
		begin = next;
	}
}