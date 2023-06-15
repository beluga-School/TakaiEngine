#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#pragma comment(lib,"xinput.lib")
#include <Xinput.h>
#include "Vector2.h"
#include "WinAPI.h"
#include <stdint.h>

enum STICK_VEC
{
	UP,
	DOWN,
	RIGHT,
	LEFT,
};

enum class Click
{
	LEFT = 0,
	RIGHT = 1,
	MIDDLE = 2,
	MB4 = 3,
};

namespace Input
{
	class Mouse
	{
	public:

		static void Initialize();
		static void Update();
		static void Finalize();

		//座標を取得
		static Vector2 GetPos();
		//1フレーム内での移動量を取得
		static Vector2 GetVelocity();

		//押している間
		static bool Down(const Click& c);
		//押した瞬間
		static bool Triggered(const Click& c);
		//離した瞬間
		static bool Released(const Click& c);
		//ホイールの回転
		static float Wheel();

		static Mouse* Get();

	private:
		IDirectInputDevice8* mMouse = nullptr;
		Vector2 mCurser{};
		DIMOUSESTATE mState{};
		DIMOUSESTATE mOldState{};

	};

	class Pad
	{
	public:
		Pad();

		static void Initialize();

		/// <summary>
		/// パッドの更新
		/// 一度だけ呼べば問題ない
		/// 
		static void Update();

	public://コントローラー

		static const float STICK_MAX;

		static bool CheckConnectPad(const int32_t& padIndex = 0);

		/// <summary>
		/// ゲームパッドのボタンが押されているか
		/// </summary>
		/// <param name="pad_num">XINPUT_GAMEPAD_Aなど、XINPUTのマクロを入れる</param>
		/// <returns></returns>
		static bool PushPadButton(const UINT& pad_num);

		/// <summary>
		/// ゲームパッドのボタンが押された瞬間か
		/// </summary>
		/// <param name="pad_num">XINPUT_GAMEPAD_Aなど、XINPUTのマクロを入れる</param>
		/// <returns></returns>
		static bool TriggerPadButton(const UINT& pad_num);

		/// <summary>
		/// ゲームパッドのボタンが離された瞬間か
		/// </summary>
		/// <param name="pad_num">XINPUT_GAMEPAD_Aなど、XINPUTのマクロを入れる</param>
		/// <returns></returns>
		static bool ReleasePadButton(const UINT& pad_num);

		/// <summary>
		/// Lスティックの傾きを取得する
		/// </summary>
		/// <returns></returns>
		static Vector2 GetLStickMove();

		/// <summary>
		/// Rスティックの傾きを取得する
		/// </summary>
		/// <returns></returns>
		static Vector2 GetRStickMove();

		/// <summary>
		/// Lスティックが指定した方向に傾いているかを取得する
		/// </summary>
		/// <param name="vec">UP,DOWNなど</param>
		/// <returns></returns>
		static bool LStickTilt(const STICK_VEC& vec);

		/// <summary>
		/// Rスティックが指定した方向に傾いているかを取得する
		/// </summary>
		/// <param name="vec">UP,DOWNなど</param>
		/// <returns></returns>
		static bool RStickTilt(const STICK_VEC& vec);

		/// <summary>
		/// LTriggerを押しているか
		/// </summary>
		/// <returns></returns>
		static BYTE PushLT();

		/// <summary>
		/// RTriggerを押しているか
		/// </summary>
		/// <returns></returns>
		static BYTE PushRT();

		/// <summary>
		///  LTriggerを押した瞬間か
		/// </summary>
		/// <param name="hard">trueの場合、より強く押し込まないと判定されなくなる</param>
		/// <returns></returns>
		static bool TriggerLT(const bool& hard = false);

		/// <summary>
		///  RTriggerを押した瞬間か
		/// </summary>
		/// <param name="hard">trueの場合、より強く押し込まないと判定されなくなる</param>
		/// <returns></returns>
		static bool TriggerRT(const bool& hard = false);

		/// <summary>
		/// LTriggerを離した瞬間か
		/// </summary>
		/// <param name="hard">trueの場合、より強く押し込まないと判定されなくなる</param>
		/// <returns></returns>
		static bool ReleaseLT(const bool& hard = false);

		/// <summary>
		/// RTriggerを離した瞬間か
		/// </summary>
		/// <param name="hard">trueの場合、より強く押し込まないと判定されなくなる</param>
		/// <returns></returns>
		static bool ReleaseRT(const bool& hard = false);
	private://コントローラー
		static XINPUT_STATE sPState;
		static XINPUT_STATE sOldpState;
	};

	class Keyboard
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

		void Finalize();

	public://キーボード

		static Keyboard* Get();

		static bool PushKey(const uint8_t& keys);
		static bool TriggerKey(const uint8_t& keys);
		static bool ReleaseKey(const uint8_t& keys);

		IDirectInput8* mDirectInput;
	private://キーボード
		IDirectInputDevice8* mKeyboard;

		static uint8_t sKey[256];
		static uint8_t sOldkey[256];

		void DirectInputInit();
		void DirectInputCreate();

	private:
		Keyboard();
		~Keyboard();
		Keyboard(const Keyboard& a) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
	};
}

//キーボード、パッド、マウスでクラスわけして、namespaceでまとめる形にする