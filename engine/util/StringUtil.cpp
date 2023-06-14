#include "StringUtil.h"
#include <iostream>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>

using namespace std;

std::string ConvertWCharToString(const wchar_t* t) 
{
    wstring wString(t);

    //wstring �� SJIS
    int32_t iBufferSize = WideCharToMultiByte(
        CP_OEMCP,
        0,
        wString.c_str(),
        -1,
        (char*)NULL,
        0,
        NULL,
        NULL
    );

    //�o�b�t�@�̎擾
    CHAR* cpMultiByte = new CHAR[iBufferSize];

    //wstring �� SJIS
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

    //string�̐���
    string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

    //�o�b�t�@�̔j��
    delete[] cpMultiByte;

    //�ϊ����ʂ�Ԃ�
    return (oRet);
}

wstring ConvertStringToWChar(const std::string& str)
{
    string wString(str);

    //wstring �� SJIS
    int32_t iBufferSize = MultiByteToWideChar(
        CP_OEMCP,
        0,
        wString.c_str(),
        -1,
        (wchar_t*)NULL,
        0
    );

    //�o�b�t�@�̎擾
    WCHAR* cpMultiByte = new WCHAR[iBufferSize];

    //wstring �� SJIS
    MultiByteToWideChar(
        CP_OEMCP,
        0,
        wString.c_str(),
        -1,
        cpMultiByte,
        iBufferSize
    );

    //string�̐���
    wstring oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

    //�o�b�t�@�̔j��
    delete[] cpMultiByte;

    //�ϊ����ʂ�Ԃ�
    return (oRet);
}