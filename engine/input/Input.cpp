#include "Input.h"
#include "Result.h"

Input::Input()
{
	keyboard = nullptr;
	directInput = nullptr;
}

Input::~Input()
{

}

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}


bool Input::PushKey(unsigned char keys)
{
	return key[keys];
}

bool Input::TriggerKey(unsigned char keys)
{
	return key[keys] && !oldkey[keys];
}

bool Input::ReleaseKey(unsigned char keys)
{
	return !key[keys] && oldkey[keys];;
}

void Input::DirectInputInit()
{
	result = DirectInput8Create(
		winApi->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
}

void Input::DirectInputCreate()
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

void Input::Update()
{
	//キーボード情報の取得開始
	keyboard->Acquire();

	//全キーの入力状態を取得する
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
	keyboard->GetDeviceState(sizeof(key), key);
}