#include <Windows.h>

//ウィンドウプロシージャ
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	
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

//windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//ウィンドウサイズ
	const int window_width = 1200;
	const int window_height = 720;

	//ウィンドウクラスの設定
	WNDCLASSEX w{};
	w.cbSize = sizeof(WNDCLASSEX);			//ウィンドウプロシージャを設定
	w.lpfnWndProc = (WNDPROC)WindowProc;	//ウィンドウクラス名
	w.lpszClassName = L"DirectXGame";		//ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW);//カーソル指定

	//ウィンドウクラスをOSに登録する
	RegisterClassEx(&w);

	//ウィンドウサイズ { X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0,0,window_width,window_height };

	//自動でサイズを補正する
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウオブジェクトの生成

	//コンソールへの文字出力
	OutputDebugStringA("Hello,DirectX!!\n");

	return 0;
}