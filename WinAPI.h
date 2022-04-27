#pragma once
#include <Windows.h>

//ウィンドウサイズ
const int window_width = 1200;
const int window_height = 720;

class WinAPI
{
public:

	RECT wrc;
	HWND hwnd;
	MSG msg;


	//ウィンドウクラスの設定
	WNDCLASSEX w{};
	void SetWindowClass();

private:

};

