#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma comment(lib,"xinput.lib")
#include <Xinput.h>
#include "Vector2.h"
#include "WinAPI.h"

enum STICK_VEC
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

class Input
{
public:
	/// <summary>
	/// キーボード、パッドの初期化
	/// 一度だけ呼べば問題ない
	/// </summary>
	void Initialize();

	/// <summary>
	/// キーボード、パッドの更新
	/// 一度だけ呼べば問題ない
	/// </summary>
	void Update();

public://キーボード

	static Input* Get();

	bool PushKey(unsigned char keys);
	bool TriggerKey(unsigned char keys);
	bool ReleaseKey(unsigned char keys);

public://コントローラー

	const float STICK_MAX = 32768.0f;

	bool CheckConnectPad(int padIndex = 0);

	/// <summary>
	/// ゲームパッドのボタンが押されているか
	/// </summary>
	/// <param name="pad_num">XINPUT_GAMEPAD_Aなど、XINPUTのマクロを入れる</param>
	/// <returns></returns>
	bool PushPadButton(UINT pad_num);
	
	/// <summary>
	/// ゲームパッドのボタンが押された瞬間か
	/// </summary>
	/// <param name="pad_num">XINPUT_GAMEPAD_Aなど、XINPUTのマクロを入れる</param>
	/// <returns></returns>
	bool TriggerPadButton(UINT pad_num);
	
	/// <summary>
	/// ゲームパッドのボタンが離された瞬間か
	/// </summary>
	/// <param name="pad_num">XINPUT_GAMEPAD_Aなど、XINPUTのマクロを入れる</param>
	/// <returns></returns>
	bool ReleasePadButton(UINT pad_num);
	
	/// <summary>
	/// Lスティックの傾きを取得する
	/// </summary>
	/// <returns></returns>
	Vector2 GetLStickMove();

	/// <summary>
	/// Rスティックの傾きを取得する
	/// </summary>
	/// <returns></returns>
	Vector2 GetRStickMove();

	/// <summary>
	/// Lスティックが指定した方向に傾いているかを取得する
	/// </summary>
	/// <param name="vec">UP,DOWNなど</param>
	/// <returns></returns>
	bool LStickTilt(STICK_VEC vec);

	/// <summary>
	/// Rスティックが指定した方向に傾いているかを取得する
	/// </summary>
	/// <param name="vec">UP,DOWNなど</param>
	/// <returns></returns>
	bool RStickTilt(STICK_VEC vec);

	/// <summary>
	/// LTriggerを押しているか
	/// </summary>
	/// <returns></returns>
	BYTE PushLT();
	
	/// <summary>
	/// RTriggerを押しているか
	/// </summary>
	/// <returns></returns>
	BYTE PushRT();

	/// <summary>
	///  LTriggerを押した瞬間か
	/// </summary>
	/// <param name="hard">trueの場合、より強く押し込まないと判定されなくなる</param>
	/// <returns></returns>
	bool TriggerLT(bool hard = false);

	/// <summary>
	///  RTriggerを押した瞬間か
	/// </summary>
	/// <param name="hard">trueの場合、より強く押し込まないと判定されなくなる</param>
	/// <returns></returns>
	bool TriggerRT(bool hard = false);

	/// <summary>
	/// LTriggerを離した瞬間か
	/// </summary>
	/// <param name="hard">trueの場合、より強く押し込まないと判定されなくなる</param>
	/// <returns></returns>
	bool ReleaseLT(bool hard = false);

	/// <summary>
	/// RTriggerを離した瞬間か
	/// </summary>
	/// <param name="hard">trueの場合、より強く押し込まないと判定されなくなる</param>
	/// <returns></returns>
	bool ReleaseRT(bool hard = false);

private://キーボード
	IDirectInputDevice8* keyboard;
	IDirectInput8* directInput;

	unsigned char key[256] = {};
	unsigned char oldkey[256] = {};

	void DirectInputInit();
	void DirectInputCreate();

private://コントローラー
	
	XINPUT_STATE pState;
	XINPUT_STATE oldpState;

private:
	Input();
	~Input();
	Input(const Input& a) = delete;
	Input& operator=(const Input&) = delete;

	WinAPI *winApi = WinAPI::GetInstance();
};

