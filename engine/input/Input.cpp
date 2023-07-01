#include "Input.h"
#include "Result.h"
#include "Util.h"

using namespace Input;

const float Pad::STICK_MAX = 32768.0f;
static float DEAD_ZONE = 0;

XINPUT_STATE Pad::sPState{};
XINPUT_STATE Pad::sOldpState{};

uint8_t Keyboard::sKey[256]{};
uint8_t Keyboard::sOldkey[256]{};

Keyboard::Keyboard()
{
	mKeyboard = nullptr;
	mDirectInput = nullptr;
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
	return sKey[keys];
}

bool Keyboard::TriggerKey(const uint8_t& keys)
{
	return sKey[keys] && !sOldkey[keys];
}

bool Keyboard::ReleaseKey(const uint8_t& keys)
{
	return !sKey[keys] && sOldkey[keys];;
}

void Keyboard::DirectInputInit()
{
	//����̓L�[�{�[�h�̓��͂����������Ă���ˁ`�`
	sResult = DirectInput8Create(
		WinAPI::Get()->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, nullptr);
	assert(SUCCEEDED(sResult));
}

void Keyboard::DirectInputCreate()
{
	//�L�[�{�[�h�f�o�C�X�̐���
	sResult = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	assert(SUCCEEDED(sResult));

	sResult = mKeyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(sResult));

	
	sResult = mKeyboard->SetCooperativeLevel(
		WinAPI::Get()->mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(sResult));
}

Pad::Pad()
{
	sPState = {};
	sOldpState = {};
}

void Input::Pad::Initialize()
{
	Pad::CheckConnectPad();
}

bool Pad::CheckConnectPad(const int32_t& padIndex)
{
	sResult = XInputGetState(padIndex,&sPState);
	return sResult == ERROR_SUCCESS;
}

bool Pad::PushPadButton(const PadButton& padButton)
{
	return sPState.Gamepad.wButtons & (UINT)padButton;
}

bool Pad::TriggerPadButton(const PadButton& padButton)
{
	return (sPState.Gamepad.wButtons & (UINT)padButton) != 0 && (sOldpState.Gamepad.wButtons & (UINT)padButton) == 0;
}

bool Pad::ReleasePadButton(const PadButton& padButton)
{
	return (sPState.Gamepad.wButtons & (UINT)padButton) == 0 && (sOldpState.Gamepad.wButtons & (UINT)padButton) != 0;
}

Vector2 Pad::GetLStickMove()
{
	Vector2 temp = {
		static_cast<float>(sPState.Gamepad.sThumbLX),
		static_cast<float>(sPState.Gamepad.sThumbLY)
	};

	if (Util::Abs(temp.x) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		temp.x = 0;
	}
	else
	{
		temp.x /= STICK_MAX;
	}
	if (Util::Abs(temp.y) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		temp.y = 0;
	}
	else
	{
		temp.y /= STICK_MAX;
	}

	return { temp.x , temp.y};
}

Vector2 Pad::GetRStickMove()
{
	Vector2 temp = {
		static_cast<float>(sPState.Gamepad.sThumbRX),
		static_cast<float>(sPState.Gamepad.sThumbRY)
	};
	if (Util::Abs(temp.x) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE){
		temp.x = 0;
	}
	else
	{
		temp.x /= STICK_MAX;
	}
	if (Util::Abs(temp.y) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		temp.y = 0;
	}
	else
	{
		temp.y /= STICK_MAX;
	}

	return { temp.x, temp.y};
}

bool Pad::LStickTilt(const STICK_VEC& vec)
{
	Vector2 temp = {
		static_cast<float>(sPState.Gamepad.sThumbLX),
		static_cast<float>(sPState.Gamepad.sThumbLY)
	};

	if (vec == UP)
	{
		return (sPState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	if (vec == DOWN)
	{
		return (sPState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	if (vec == RIGHT)
	{
		return (sPState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	if (vec == LEFT)
	{
		return (sPState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}

	return false;
}

bool Pad::RStickTilt(const STICK_VEC& vec)
{
	Vector2 temp = {
		static_cast<float>(sPState.Gamepad.sThumbRX),
		static_cast<float>(sPState.Gamepad.sThumbRY)
	};

	if (vec == UP)
	{
		return (sPState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	if (vec == DOWN)
	{
		return (sPState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	if (vec == RIGHT)
	{
		return (sPState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	if (vec == LEFT)
	{
		
		return (sPState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}

	return false;
}

BYTE Pad::PushLT()
{
	return sPState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

BYTE Pad::PushRT()
{
	return sPState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

}

bool Pad::TriggerLT(const bool& hard)
{
	if (hard)
	{
		return (sPState.Gamepad.bLeftTrigger > 128) &&
			(sOldpState.Gamepad.bLeftTrigger <= 128);
	}
	else
	{
		return (sPState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(sOldpState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

	}
}

bool Pad::TriggerRT(const bool& hard)
{
	if (hard)
	{
		return (sPState.Gamepad.bRightTrigger > 128) &&
			(sOldpState.Gamepad.bRightTrigger <= 128);
	}
	else
	{
		return (sPState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(sOldpState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

	}
}

bool Pad::ReleaseLT(const bool& hard)
{
	if (hard)
	{
		return (sPState.Gamepad.bLeftTrigger <= 128) &&
			(sOldpState.Gamepad.bLeftTrigger > 128);
	}
	else
	{
		return (sPState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(sOldpState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
}

bool Pad::ReleaseRT(const bool& hard)
{
	if (hard)
	{
		return (sPState.Gamepad.bRightTrigger <= 128) &&
			(sOldpState.Gamepad.bRightTrigger > 128);
	}
	else
	{
		return (sPState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(sOldpState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
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
	mKeyboard->Acquire();

	//�S�L�[�̓��͏�Ԃ��擾����
	for (int32_t i = 0; i < 256; i++)
	{
		sOldkey[i] = sKey[i];
	}
	mKeyboard->GetDeviceState(sizeof(sKey), sKey);
}

void Keyboard::Finalize()
{
	mKeyboard->Release();
	mDirectInput->Release();
}

Vector2 Input::Mouse::GetPos()
{
	return Mouse::Get()->mCurser;
}

Vector2 Input::Mouse::GetVelocity()
{
	return Vector2((float)Mouse::Get()->mState.lX, (float)Mouse::Get()->mState.lY);
}

void Input::Mouse::Initialize()
{
	//�L�[�{�[�h�f�o�C�X�̐���
	sResult = Keyboard::Get()->mDirectInput->CreateDevice(GUID_SysMouse, &Mouse::Get()->mMouse, NULL);
	assert(SUCCEEDED(sResult));

	//�f�o�C�X�̃t�H�[�}�b�g��ݒ�
	sResult = Mouse::Get()->mMouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(sResult));

	//�E�B���h�E�O�Ȃ�}�E�X�̎擾������
	//���̃A�v���P�[�V���������̃f�o�C�X���擾�ł���悤��
	sResult = Mouse::Get()->mMouse->SetCooperativeLevel(WinAPI::Get()->mHwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(sResult));

	CurserLock(CurserLockState::UNLOCK);
}

void Mouse::Update()
{
	Mouse* instance = Mouse::Get();
	//�O��t���[���̓��͂�ۑ�
	instance->mOldState = instance->mState;

	//���̓f�o�C�X�̐�����J�n
	instance->mMouse->Acquire();
	instance->mMouse->Poll();
	
	//�f�o�C�X�̏�Ԃ��擾
	instance->mMouse->GetDeviceState(sizeof(DIMOUSESTATE), &instance->mState);

	//�}�E�X���W�擾
	POINT p;
	GetCursorPos(&p);

	ScreenToClient(WinAPI::Get()->mHwnd, &p);
	instance->mCurser = {(float)p.x,(float)p.y};
}

void Mouse::Finalize()
{
	Mouse::Get()->mMouse->Unacquire();
	Mouse::Get()->mMouse->Release();
}

void Input::Mouse::CurserLock(const CurserLockState& state)
{
	switch (state)
	{
	case CurserLockState::LOCK:
		ShowCursor(false);
		SetCursorPos(Util::WIN_WIDTH / 2, Util::WIN_HEIGHT / 2);

		break;
	case CurserLockState::UNLOCK:
		ShowCursor(true);
		
		break;
	}
}

bool Input::Mouse::Down(const Click& c)
{
	return Mouse::Get()->mState.rgbButtons[static_cast<int32_t>(c)] & (0x80);
}

bool Input::Mouse::Triggered(const Click& c)
{
	return (Mouse::Get()->mState.rgbButtons[static_cast<int32_t>(c)] & (0x80)) && !(Mouse::Get()->mOldState.rgbButtons[static_cast<int32_t>(c)] & (0x80));
}

bool Input::Mouse::Released(const Click& c)
{
	return !(Mouse::Get()->mState.rgbButtons[static_cast<int32_t>(c)] & (0x80)) && (Mouse::Get()->mOldState.rgbButtons[static_cast<int32_t>(c)] & (0x80));
}

float Input::Mouse::Wheel()
{
	return (float)Mouse::Get()->mState.lZ;
}

Mouse* Mouse::Get()
{
	static Mouse obj;
	return &obj;
}

void Input::Pad::Update()
{
	sOldpState = sPState;

	CheckConnectPad();

	//�f�b�h�]�[�����̓��͂�0�ɗ}����
	if ((sPState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		sPState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(sPState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			sPState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		sPState.Gamepad.sThumbLX = 0;
		sPState.Gamepad.sThumbLY = 0;
	}

	if ((sPState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		sPState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(sPState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			sPState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		sPState.Gamepad.sThumbRX = 0;
		sPState.Gamepad.sThumbRY = 0;
	}
}