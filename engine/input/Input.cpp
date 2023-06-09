#include "Input.h"
#include "Result.h"

using namespace Input;

InputKey::InputKey()
{
	keyboard = nullptr;
	directInput = nullptr;
}

InputKey::~InputKey()
{

}

InputKey* InputKey::Get()
{
	static InputKey instance;
	return &instance;
}

bool InputKey::PushKey(unsigned char keys)
{
	return key[keys];
}

bool InputKey::TriggerKey(unsigned char keys)
{
	return key[keys] && !oldkey[keys];
}

bool InputKey::ReleaseKey(unsigned char keys)
{
	return !key[keys] && oldkey[keys];;
}

void InputKey::DirectInputInit()
{
	//これはキーボードの入力を初期化してるんやね～～
	result = DirectInput8Create(
		winApi->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
}

void InputKey::DirectInputCreate()
{
	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	assert(SUCCEEDED(result));

	
	result = keyboard->SetCooperativeLevel(
		winApi->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

bool InputKey::CheckConnectPad(int padIndex)
{
	result = XInputGetState(padIndex,&pState);
	return result == ERROR_SUCCESS;
}

bool InputKey::PushPadButton(UINT pad_num)
{
	return pState.Gamepad.wButtons & pad_num;
}

bool InputKey::TriggerPadButton(UINT pad_num)
{
	return (pState.Gamepad.wButtons & pad_num) != 0 && (oldpState.Gamepad.wButtons & pad_num) == 0;
}

bool InputKey::ReleasePadButton(UINT pad_num)
{
	return (pState.Gamepad.wButtons & pad_num) == 0 && (oldpState.Gamepad.wButtons & pad_num) != 0;
}

Vector2 InputKey::GetLStickMove()
{
	Vector2 temp = {
		static_cast<float>(pState.Gamepad.sThumbLX),
		static_cast<float>(pState.Gamepad.sThumbLY)
	};
	return { temp.x / STICK_MAX, temp.y / STICK_MAX };
}

Vector2 InputKey::GetRStickMove()
{
	Vector2 temp = {
		static_cast<float>(pState.Gamepad.sThumbRX),
		static_cast<float>(pState.Gamepad.sThumbRY)
	};
	return { temp.x / STICK_MAX, temp.y / STICK_MAX };
}

bool InputKey::LStickTilt(STICK_VEC vec)
{
	Vector2 temp = {
		static_cast<float>(pState.Gamepad.sThumbLX),
		static_cast<float>(pState.Gamepad.sThumbLY)
	};

	if (vec == UP)
	{
		return (pState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	if (vec == DOWN)
	{
		return (pState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	if (vec == RIGHT)
	{
		return (pState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	if (vec == LEFT)
	{
		
		return (pState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}

	return false;
}

bool InputKey::RStickTilt(STICK_VEC vec)
{
	Vector2 temp = {
		static_cast<float>(pState.Gamepad.sThumbRX),
		static_cast<float>(pState.Gamepad.sThumbRY)
	};

	if (vec == UP)
	{
		return (pState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	if (vec == DOWN)
	{
		return (pState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	if (vec == RIGHT)
	{
		return (pState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	if (vec == LEFT)
	{
		
		return (pState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}

	return false;
}

BYTE InputKey::PushLT()
{
	return pState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

BYTE InputKey::PushRT()
{
	return pState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

}

bool InputKey::TriggerLT(bool hard)
{
	if (hard)
	{
		return (pState.Gamepad.bLeftTrigger > 128) &&
			(oldpState.Gamepad.bLeftTrigger <= 128);
	}
	else
	{
		return (pState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(oldpState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

	}
}

bool InputKey::TriggerRT(bool hard)
{
	if (hard)
	{
		return (pState.Gamepad.bRightTrigger > 128) &&
			(oldpState.Gamepad.bRightTrigger <= 128);
	}
	else
	{
		return (pState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(oldpState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

	}
}

bool InputKey::ReleaseLT(bool hard)
{
	if (hard)
	{
		return (pState.Gamepad.bLeftTrigger <= 128) &&
			(oldpState.Gamepad.bLeftTrigger > 128);
	}
	else
	{
		return (pState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(oldpState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
}

bool InputKey::ReleaseRT(bool hard)
{
	if (hard)
	{
		return (pState.Gamepad.bRightTrigger <= 128) &&
			(oldpState.Gamepad.bRightTrigger > 128);
	}
	else
	{
		return (pState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(oldpState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
}

void InputKey::Initialize()
{
	//DirectInputの初期化
	Get()->DirectInputInit();

	//キーボードデバイスの生成
	Get()->DirectInputCreate();

	//パッドが接続されているか
	CheckConnectPad();
}

void InputKey::Update()
{
	//キーボード情報の取得開始
	keyboard->Acquire();

	//全キーの入力状態を取得する
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
	keyboard->GetDeviceState(sizeof(key), key);

	oldpState = pState;

	CheckConnectPad();

	//デッドゾーン内の入力を0に抑える
	if ((pState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		pState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(pState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		pState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		pState.Gamepad.sThumbLX = 0;
		pState.Gamepad.sThumbLY = 0;
	}

	if ((pState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		pState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(pState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		pState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		pState.Gamepad.sThumbRX = 0;
		pState.Gamepad.sThumbRY = 0;
	}
}

void InputKey::Finalize()
{
	keyboard->Release();
	directInput->Release();
}

Vector2 Input::Mouse::GetPos()
{
	return Mouse::Get()->curser;
}

Vector2 Input::Mouse::GetVelocity()
{
	return Vector2((float)Mouse::Get()->state.lX, (float)Mouse::Get()->state.lY);
}

void Input::Mouse::Initialize()
{
	//キーボードデバイスの生成
	result = InputKey::Get()->directInput->CreateDevice(GUID_SysMouse, &Mouse::Get()->mouse, NULL);
	assert(SUCCEEDED(result));

	//デバイスのフォーマットを設定
	result = Mouse::Get()->mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	//ウィンドウ外ならマウスの取得をせず
	//他のアプリケーションもそのデバイスを取得できるように
	result = Mouse::Get()->mouse->SetCooperativeLevel(WinAPI::Get()->hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(result));
}

void Mouse::Update()
{
	Mouse* instance = Mouse::Get();
	//前回フレームの入力を保存
	instance->oldState = instance->state;

	//入力デバイスの制御を開始
	instance->mouse->Acquire();
	instance->mouse->Poll();
	
	//デバイスの状態を取得
	instance->mouse->GetDeviceState(sizeof(DIMOUSESTATE), &instance->state);

	//マウス座標取得
	POINT p;
	GetCursorPos(&p);

	ScreenToClient(WinAPI::Get()->hwnd, &p);
	instance->curser = {(float)p.x,(float)p.y};
}

void Mouse::Finalize()
{
	Mouse::Get()->mouse->Unacquire();
	Mouse::Get()->mouse->Release();
}

bool Input::Mouse::Down(Click c)
{
	return Mouse::Get()->state.rgbButtons[(int)c] & (0x80);
}

bool Input::Mouse::Triggered(Click c)
{
	return (Mouse::Get()->state.rgbButtons[(int)c] & (0x80)) && !(Mouse::Get()->oldState.rgbButtons[(int)c] & (0x80));
}

bool Input::Mouse::Released(Click c)
{
	return !(Mouse::Get()->state.rgbButtons[(int)c] & (0x80)) && (Mouse::Get()->oldState.rgbButtons[(int)c] & (0x80));
}

Mouse* Mouse::Get()
{
	static Mouse obj;
	return &obj;
}