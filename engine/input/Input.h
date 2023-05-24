#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInput�̃o�[�W�����w��
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
	/// �L�[�{�[�h�A�p�b�h�̏�����
	/// ��x�����ĂׂΖ��Ȃ�
	/// </summary>
	void Initialize();

	/// <summary>
	/// �L�[�{�[�h�A�p�b�h�̍X�V
	/// ��x�����ĂׂΖ��Ȃ�
	/// </summary>
	void Update();

public://�L�[�{�[�h

	static Input* Get();

	bool PushKey(unsigned char keys);
	bool TriggerKey(unsigned char keys);
	bool ReleaseKey(unsigned char keys);

public://�R���g���[���[

	const float STICK_MAX = 32768.0f;

	bool CheckConnectPad(int padIndex = 0);

	/// <summary>
	/// �Q�[���p�b�h�̃{�^����������Ă��邩
	/// </summary>
	/// <param name="pad_num">XINPUT_GAMEPAD_A�ȂǁAXINPUT�̃}�N��������</param>
	/// <returns></returns>
	bool PushPadButton(UINT pad_num);
	
	/// <summary>
	/// �Q�[���p�b�h�̃{�^���������ꂽ�u�Ԃ�
	/// </summary>
	/// <param name="pad_num">XINPUT_GAMEPAD_A�ȂǁAXINPUT�̃}�N��������</param>
	/// <returns></returns>
	bool TriggerPadButton(UINT pad_num);
	
	/// <summary>
	/// �Q�[���p�b�h�̃{�^���������ꂽ�u�Ԃ�
	/// </summary>
	/// <param name="pad_num">XINPUT_GAMEPAD_A�ȂǁAXINPUT�̃}�N��������</param>
	/// <returns></returns>
	bool ReleasePadButton(UINT pad_num);
	
	/// <summary>
	/// L�X�e�B�b�N�̌X�����擾����
	/// </summary>
	/// <returns></returns>
	Vector2 GetLStickMove();

	/// <summary>
	/// R�X�e�B�b�N�̌X�����擾����
	/// </summary>
	/// <returns></returns>
	Vector2 GetRStickMove();

	/// <summary>
	/// L�X�e�B�b�N���w�肵�������ɌX���Ă��邩���擾����
	/// </summary>
	/// <param name="vec">UP,DOWN�Ȃ�</param>
	/// <returns></returns>
	bool LStickTilt(STICK_VEC vec);

	/// <summary>
	/// R�X�e�B�b�N���w�肵�������ɌX���Ă��邩���擾����
	/// </summary>
	/// <param name="vec">UP,DOWN�Ȃ�</param>
	/// <returns></returns>
	bool RStickTilt(STICK_VEC vec);

	/// <summary>
	/// LTrigger�������Ă��邩
	/// </summary>
	/// <returns></returns>
	BYTE PushLT();
	
	/// <summary>
	/// RTrigger�������Ă��邩
	/// </summary>
	/// <returns></returns>
	BYTE PushRT();

	/// <summary>
	///  LTrigger���������u�Ԃ�
	/// </summary>
	/// <param name="hard">true�̏ꍇ�A��苭���������܂Ȃ��Ɣ��肳��Ȃ��Ȃ�</param>
	/// <returns></returns>
	bool TriggerLT(bool hard = false);

	/// <summary>
	///  RTrigger���������u�Ԃ�
	/// </summary>
	/// <param name="hard">true�̏ꍇ�A��苭���������܂Ȃ��Ɣ��肳��Ȃ��Ȃ�</param>
	/// <returns></returns>
	bool TriggerRT(bool hard = false);

	/// <summary>
	/// LTrigger�𗣂����u�Ԃ�
	/// </summary>
	/// <param name="hard">true�̏ꍇ�A��苭���������܂Ȃ��Ɣ��肳��Ȃ��Ȃ�</param>
	/// <returns></returns>
	bool ReleaseLT(bool hard = false);

	/// <summary>
	/// RTrigger�𗣂����u�Ԃ�
	/// </summary>
	/// <param name="hard">true�̏ꍇ�A��苭���������܂Ȃ��Ɣ��肳��Ȃ��Ȃ�</param>
	/// <returns></returns>
	bool ReleaseRT(bool hard = false);

private://�L�[�{�[�h
	IDirectInputDevice8* keyboard;
	IDirectInput8* directInput;

	unsigned char key[256] = {};
	unsigned char oldkey[256] = {};

	void DirectInputInit();
	void DirectInputCreate();

private://�R���g���[���[
	
	XINPUT_STATE pState;
	XINPUT_STATE oldpState;

private:
	Input();
	~Input();
	Input(const Input& a) = delete;
	Input& operator=(const Input&) = delete;

	WinAPI *winApi = WinAPI::GetInstance();
};

