#pragma once
#include <Windows.h>

//ウィンドウサイズ
const int window_width = 1280;
const int window_height = 720;

class WinAPI
{
public:

	RECT wrc;
	HWND hwnd;
	MSG msg;


	//ウィンドウクラスの設定
	WNDCLASSEX w{};

	static WinAPI* GetInstance();

private:
	WinAPI();
	~WinAPI(){};

	WinAPI(const WinAPI& a) = delete;
	WinAPI& operator=(const WinAPI&) = delete;

	void SetWindowClass();
};