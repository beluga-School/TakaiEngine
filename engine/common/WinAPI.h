#pragma once
#include <Windows.h>

class WinAPI
{
public:

	RECT mWrc;
	HWND mHwnd;
	MSG mMsg;

	//ウィンドウクラスの設定
	WNDCLASSEX w{};

	static WinAPI* Get();

private:
	WinAPI();
	~WinAPI(){};

	WinAPI(const WinAPI& a) = delete;
	WinAPI& operator=(const WinAPI&) = delete;

	void SetWindowClass();
};