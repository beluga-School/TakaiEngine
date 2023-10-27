#pragma once
#include <string>

//ワイド文字列をstring型に変換する
std::string ConvertWCharToString(const wchar_t* t);

//string型をワイド文字列に変換する
std::wstring ConvertStringToWChar(const std::string& str);

