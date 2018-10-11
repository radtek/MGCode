#pragma once
#include<string>
#include"CStringToolEx.h"
#include"cpp11_std_files.h"
enum text_source:short{User_Input,File,Sql};

void textLsReplae(std::wstring& data,const std::wstring& beforeText,const std::wstring& afterText);

void string_split(std::string data,char c,std::vector<std::string> con);