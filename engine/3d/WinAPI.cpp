#include "WinAPI.h"
#include "Util.h"
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//staticにすべき
//ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	//ImGui用のウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {

		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対してアプリの終了を伝える(deleteみたいな、メモリ解放の役割)
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WinAPI::WinAPI()
{
	SetWindowClass();
}

void WinAPI::SetWindowClass()
{
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;	//ウィンドウプロシージャを設定
	w.lpszClassName = L"DirectXGame";		//ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr);	//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);

	//ウィンドウサイズ { X座標 Y座標 横幅 縦幅 }
	mWrc = { 0,0,(long)Util::WIN_WIDTH,(long)Util::WIN_HEIGHT };

	//自動でサイズを補正する
	AdjustWindowRect(&mWrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの生成
	mHwnd = CreateWindow(
		w.lpszClassName,		//クラス名
		L"TAKAIEngine",			//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//標準的なウィンドウスタイル
		CW_USEDEFAULT,			//表示X座標(OSに任せる)
		CW_USEDEFAULT,			//表示Y座標(OSに任せる)
		mWrc.right - mWrc.left,	//ウィンドウ横幅
		mWrc.bottom - mWrc.top,	//ウィンドウ縦幅
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		w.hInstance,			//呼び出しアプリケーションハンドル
		nullptr					//オプション
		);

	//ウィンドウを表示状態にする
	ShowWindow(mHwnd, SW_SHOW);

	mMsg = {};
}

WinAPI* WinAPI::Get()
{
	static WinAPI instance;
	return &instance;
}
