#include "StringUtil.h"
#include <iostream>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>

using namespace std;

std::string ConvertWCharToString(wchar_t* t) 
{
    wstring wString(t);

    //wstring → SJIS
    int iBufferSize = WideCharToMultiByte(
        CP_OEMCP,
        0,
        wString.c_str(),
        -1,
        (char*)NULL,
        0,
        NULL,
        NULL
    );

    //バッファの取得
    CHAR* cpMultiByte = new CHAR[iBufferSize];

    //wstring → SJIS
    WideCharToMultiByte(
        CP_OEMCP,
        0,
        wString.c_str(),
        -1,
        cpMultiByte,
        iBufferSize,
        NULL,
        NULL
    );

    //stringの生成
    string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

    //バッファの破棄
    delete[] cpMultiByte;

    //変換結果を返す
    return (oRet);
}

wstring ConvertStringToWChar(std::string str)
{
    string wString(str);

    //wstring → SJIS
    int iBufferSize = MultiByteToWideChar(
        CP_OEMCP,
        0,
        wString.c_str(),
        -1,
        (wchar_t*)NULL,
        0
    );

    //バッファの取得
    WCHAR* cpMultiByte = new WCHAR[iBufferSize];

    //wstring → SJIS
    MultiByteToWideChar(
        CP_OEMCP,
        0,
        wString.c_str(),
        -1,
        cpMultiByte,
        iBufferSize
    );

    //stringの生成
    wstring oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

    //バッファの破棄
    delete[] cpMultiByte;

    //変換結果を返す
    return (oRet);
}