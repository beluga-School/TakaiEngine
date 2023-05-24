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

Input* Input::Get()
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
	//�L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	result = keyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));

	
	result = keyboard->SetCooperativeLevel(
		winApi->hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

bool Input::CheckConnectPad(int padIndex)
{
	result = XInputGetState(padIndex,&pState);
	return result == ERROR_SUCCESS;
}

bool Input::PushPadButton(UINT pad_num)
{
	return pState.Gamepad.wButtons & pad_num;
}

bool Input::TriggerPadButton(UINT pad_num)
{
	return (pState.Gamepad.wButtons & pad_num) != 0 && (oldpState.Gamepad.wButtons & pad_num) == 0;
}

bool Input::ReleasePadButton(UINT pad_num)
{
	return (pState.Gamepad.wButtons & pad_num) == 0 && (oldpState.Gamepad.wButtons & pad_num) != 0;
}

Vector2 Input::GetLStickMove()
{
	Vector2 temp = {
		static_cast<float>(pState.Gamepad.sThumbLX),
		static_cast<float>(pState.Gamepad.sThumbLY)
	};
	return { temp.x / STICK_MAX, temp.y / STICK_MAX };
}

Vector2 Input::GetRStickMove()
{
	Vector2 temp = {
		static_cast<float>(pState.Gamepad.sThumbRX),
		static_cast<float>(pState.Gamepad.sThumbRY)
	};
	return { temp.x / STICK_MAX, temp.y / STICK_MAX };
}

bool Input::LStickTilt(STICK_VEC vec)
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

bool Input::RStickTilt(STICK_VEC vec)
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

BYTE Input::PushLT()
{
	return pState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

BYTE Input::PushRT()
{
	return pState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

}

bool Input::TriggerLT(bool hard)
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

bool Input::TriggerRT(bool hard)
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

bool Input::ReleaseLT(bool hard)
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

bool Input::ReleaseRT(bool hard)
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

void Input::Initialize()
{
	//DirectInput�̏�����
	Get()->DirectInputInit();

	//�L�[�{�[�h�f�o�C�X�̐���
	Get()->DirectInputCreate();

	//�p�b�h���ڑ�����Ă��邩
	CheckConnectPad();
}

void Input::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	//�S�L�[�̓��͏�Ԃ��擾����
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
	keyboard->GetDeviceState(sizeof(key), key);

	oldpState = pState;

	CheckConnectPad();

	//�f�b�h�]�[�����̓��͂�0�ɗ}����
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