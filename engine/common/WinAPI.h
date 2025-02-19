#pragma once
#include <Windows.h>

/*! WinAPI
	ウィンドウを生成するクラス
*/
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

	//ウィンドウの情報を設定
	void SetWindowClass();
};