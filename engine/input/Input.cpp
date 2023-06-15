#include "Input.h"
#include "Result.h"

using namespace Input;

const float Pad::STICK_MAX = 32768.0f;
XINPUT_STATE Pad::mPState{};
XINPUT_STATE Pad::mOldpState{};

uint8_t Keyboard::mKey[256]{};
uint8_t Keyboard::mOldkey[256]{};

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
	return mKey[keys];
}

bool Keyboard::TriggerKey(const uint8_t& keys)
{
	return mKey[keys] && !mOldkey[keys];
}

bool Keyboard::ReleaseKey(const uint8_t& keys)
{
	return !mKey[keys] && mOldkey[keys];;
}

void Keyboard::DirectInputInit()
{
	//����̓L�[�{�[�h�̓��͂����������Ă���ˁ`�`
	result = DirectInput8Create(
		WinAPI::Get()->w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDirectInput, nullptr);
	assert(SUCCEEDED(result));
}

void Keyboard::DirectInputCreate()
{
	//�L�[�{�[�h�f�o�C�X�̐���
	result = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, NULL);
	assert(SUCCEEDED(result));

	result = mKeyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	assert(SUCCEEDED(result));

	
	result = mKeyboard->SetCooperativeLevel(
		WinAPI::Get()->mHwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

Pad::Pad()
{
	mPState = {};
	mOldpState = {};
}

bool Pad::CheckConnectPad(const int32_t& padIndex)
{
	result = XInputGetState(padIndex,&mPState);
	return result == ERROR_SUCCESS;
}

bool Pad::PushPadButton(const UINT& pad_num)
{
	return mPState.Gamepad.wButtons & pad_num;
}

bool Pad::TriggerPadButton(const UINT& pad_num)
{
	return (mPState.Gamepad.wButtons & pad_num) != 0 && (mOldpState.Gamepad.wButtons & pad_num) == 0;
}

bool Pad::ReleasePadButton(const UINT& pad_num)
{
	return (mPState.Gamepad.wButtons & pad_num) == 0 && (mOldpState.Gamepad.wButtons & pad_num) != 0;
}

Vector2 Pad::GetLStickMove()
{
	Vector2 temp = {
		static_cast<float>(mPState.Gamepad.sThumbLX),
		static_cast<float>(mPState.Gamepad.sThumbLY)
	};
	return { temp.x / STICK_MAX, temp.y / STICK_MAX };
}

Vector2 Pad::GetRStickMove()
{
	Vector2 temp = {
		static_cast<float>(mPState.Gamepad.sThumbRX),
		static_cast<float>(mPState.Gamepad.sThumbRY)
	};
	return { temp.x / STICK_MAX, temp.y / STICK_MAX };
}

bool Pad::LStickTilt(const STICK_VEC& vec)
{
	Vector2 temp = {
		static_cast<float>(mPState.Gamepad.sThumbLX),
		static_cast<float>(mPState.Gamepad.sThumbLY)
	};

	if (vec == UP)
	{
		return (mPState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	if (vec == DOWN)
	{
		return (mPState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	if (vec == RIGHT)
	{
		return (mPState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	if (vec == LEFT)
	{
		
		return (mPState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}

	return false;
}

bool Pad::RStickTilt(const STICK_VEC& vec)
{
	Vector2 temp = {
		static_cast<float>(mPState.Gamepad.sThumbRX),
		static_cast<float>(mPState.Gamepad.sThumbRY)
	};

	if (vec == UP)
	{
		return (mPState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	if (vec == DOWN)
	{
		return (mPState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	if (vec == RIGHT)
	{
		return (mPState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	if (vec == LEFT)
	{
		
		return (mPState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}

	return false;
}

BYTE Pad::PushLT()
{
	return mPState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
}

BYTE Pad::PushRT()
{
	return mPState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;

}

bool Pad::TriggerLT(const bool& hard)
{
	if (hard)
	{
		return (mPState.Gamepad.bLeftTrigger > 128) &&
			(mOldpState.Gamepad.bLeftTrigger <= 128);
	}
	else
	{
		return (mPState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(mOldpState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

	}
}

bool Pad::TriggerRT(const bool& hard)
{
	if (hard)
	{
		return (mPState.Gamepad.bRightTrigger > 128) &&
			(mOldpState.Gamepad.bRightTrigger <= 128);
	}
	else
	{
		return (mPState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(mOldpState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

	}
}

bool Pad::ReleaseLT(const bool& hard)
{
	if (hard)
	{
		return (mPState.Gamepad.bLeftTrigger <= 128) &&
			(mOldpState.Gamepad.bLeftTrigger > 128);
	}
	else
	{
		return (mPState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(mOldpState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}
}

bool Pad::ReleaseRT(const bool& hard)
{
	if (hard)
	{
		return (mPState.Gamepad.bRightTrigger <= 128) &&
			(mOldpState.Gamepad.bRightTrigger > 128);
	}
	else
	{
		return (mPState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) &&
			(mOldpState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
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
		mOldkey[i] = mKey[i];
	}
	mKeyboard->GetDeviceState(sizeof(mKey), mKey);
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
	result = Keyboard::Get()->mDirectInput->CreateDevice(GUID_SysMouse, &Mouse::Get()->mMouse, NULL);
	assert(SUCCEEDED(result));

	//�f�o�C�X�̃t�H�[�}�b�g��ݒ�
	result = Mouse::Get()->mMouse->SetDataFormat(&c_dfDIMouse);
	assert(SUCCEEDED(result));

	//�E�B���h�E�O�Ȃ�}�E�X�̎擾������
	//���̃A�v���P�[�V���������̃f�o�C�X���擾�ł���悤��
	result = Mouse::Get()->mMouse->SetCooperativeLevel(WinAPI::Get()->mHwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	assert(SUCCEEDED(result));
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
	mOldpState = mPState;

	CheckConnectPad();

	//�f�b�h�]�[�����̓��͂�0�ɗ}����
	if ((mPState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		mPState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(mPState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			mPState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		mPState.Gamepad.sThumbLX = 0;
		mPState.Gamepad.sThumbLY = 0;
	}

	if ((mPState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		mPState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(mPState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			mPState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		mPState.Gamepad.sThumbRX = 0;
		mPState.Gamepad.sThumbRY = 0;
	}
}