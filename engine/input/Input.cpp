#include "Input.h"
#include "Result.h"

using namespace Input;

const float Pad::STICK_MAX = 32768.0f;
XINPUT_STATE Pad::pState{};
XINPUT_STATE Pad::oldpState{};

uint8_t Keyboard::key[256]{};
uint8_t Keyboard::oldkey[256]{};

Keyboard::Keyboard()
{
	keyboard = nullptr;
	directInput = nullptr;
}

Keyboard::~Keyboard()
{

}

Keyboard* Keyboard::Get()
{
	static Keyboard instance;
	return &instance;
}

bool Keyboard::PushKey(const uint8_t& keys)
{
	return key[keys];
}

bool Keyboard::TriggerKey(const uint8_t& keys)
{
	return key[keys] && !oldkey[keys];
}

bool Keyboard::ReleaseKey(const uint8_t& keys)
{
	return !key[keys] && oldkey[keys];;
}

void Keyboard::DirectInputInit()
{
	//����̓L�[�{�[�h�̓��͂����������Ă���ˁ`�`
	result = DirectInput8Create(
		winApi->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
}

void Keyboard::DirectInputCreate()
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

Pad::Pad()
{
	pState = {};
	oldpState = {};
}

bool Pad::CheckConnectPad(const int32_t& padIndex)
{
	result = XInputGetState(padIndex,&pState);
	return result == ERROR_SUCCESS;
}

bool Pad::PushPadButton(const UINT& pad_num)
{
	return pState.Gamepad.wButtons & pad_num;
}

bool Pad::TriggerPadButton(const UINT& pad_num)
{
	return (pState.Gamepad.wButtons & pad_num) != 0 && (oldpState.Gamepad.wButtons & pad_num) == 0;
}

bool Pad::ReleasePadButton(const UINT& pad_num)
{
	return (pState.Gamepad.wButtons & pad_num) == 0 && (oldpState.Gamepad.wButtons & pad_num) != 0;
}

Vector2 Pad::GetLStickMove()
{
	Vector2 temp = {
		static_cast<float>(pState.Gamepad.sThumbLX),
		static_cast<float>(pState.Gamepad.sThumbLY)
	};
	return { temp.x / STICK_MAX, temp.y / STICK_MAX };
}

Vector2 Pad::GetRStickMove()
{
	Vector2 temp = {
		static_cast<float>(pState.Gamepad.sThumbRX),
		static_cast<float>(pState.Gamepad.sThumbRY)
	};
	return { temp.x / STICK_MAX, temp.y / STICK_MAX };
}

bool Pad::LStickTilt(const STICK_VEC& vec)
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

bool Pad::RStickTilt(const STICK_VEC& vec)
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

BYTE Pad::PushLT()
{
	return pState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

BYTE Pad::PushRT()
{
	return pState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

}

bool Pad::TriggerLT(const bool& hard)
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

bool Pad::TriggerRT(const bool& hard)
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

bool Pad::ReleaseLT(const bool& hard)
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

bool Pad::ReleaseRT(const bool& hard)
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

void Keyboard::Initialize()
{
	//DirectInput�̏�����
	Get()->DirectInputInit();

	//�L�[�{�[�h�f�o�C�X�̐���
	Get()->DirectInputCreate();
}

void Keyboard::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	//�S�L�[�̓��͏�Ԃ��擾����
	for (int32_t i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
	keyboard->GetDeviceState(sizeof(key), key);
}

void Keyboard::Finalize()
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
	//�L�[�{�[�h�f�o�C�X�̐���
	result = Keyboard::Get()->directInput->CreateDevice(GUID_SysMouse, &Mouse::Get()->mouse, NULL);
	assert(SUCCEEDED(result));

	//�f�o�C�X�̃t�H�[�}�b�g��ݒ�
	result = Mouse::Get()->mouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	//�E�B���h�E�O�Ȃ�}�E�X�̎擾������
	//���̃A�v���P�[�V���������̃f�o�C�X���擾�ł���悤��
	result = Mouse::Get()->mouse->SetCooperativeLevel(WinAPI::Get()->hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(result));
}

void Mouse::Update()
{
	Mouse* instance = Mouse::Get();
	//�O��t���[���̓��͂�ۑ�
	instance->oldState = instance->state;

	//���̓f�o�C�X�̐�����J�n
	instance->mouse->Acquire();
	instance->mouse->Poll();
	
	//�f�o�C�X�̏�Ԃ��擾
	instance->mouse->GetDeviceState(sizeof(DIMOUSESTATE), &instance->state);

	//�}�E�X���W�擾
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

bool Input::Mouse::Down(const Click& c)
{
	return Mouse::Get()->state.rgbButtons[static_cast<int32_t>(c)] & (0x80);
}

bool Input::Mouse::Triggered(const Click& c)
{
	return (Mouse::Get()->state.rgbButtons[static_cast<int32_t>(c)] & (0x80)) && !(Mouse::Get()->oldState.rgbButtons[static_cast<int32_t>(c)] & (0x80));
}

bool Input::Mouse::Released(const Click& c)
{
	return !(Mouse::Get()->state.rgbButtons[static_cast<int32_t>(c)] & (0x80)) && (Mouse::Get()->oldState.rgbButtons[static_cast<int32_t>(c)] & (0x80));
}

float Input::Mouse::Wheel()
{
	return (float)Mouse::Get()->state.lZ;
}

Mouse* Mouse::Get()
{
	static Mouse obj;
	return &obj;
}

void Input::Pad::Update()
{
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