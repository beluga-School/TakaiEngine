#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include "WinAPI.h"

class Input
{
public:
	IDirectInputDevice8* keyboard;
	IDirectInput8* directInput;

public:
	Input();
	~Input();

	static Input* GetInstance();

	unsigned char key[256] = {};
	unsigned char oldkey[256] = {};

	bool PushKey(unsigned char keys);
	bool TriggerKey(unsigned char keys);
	bool ReleaseKey(unsigned char keys);

	void DirectInputInit();
	void DirectInputCreate();

	void Update();
private:
	WinAPI *winApi = WinAPI::GetInstance();
};

