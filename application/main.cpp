#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <iostream>
#include <memory>

//�����ŃN���X���������
#include "WinAPI.h"
#include "TimeManager.h"

#include "Result.h"
#include "Input.h"
#include "DirectXInit.h"
#include "Vertex.h"

#include "ViewProjection.h"
#include "Texture.h"

#include "Sprite.h"
#include "Pipeline.h"
#include "ClearDrawScreen.h"
#include "Billboard.h"

#include "SceneFactory.h"
#include "SceneManager.h"

#include "Sound.h"
#include "DebugText.h"

#include "Particle.h"
#include "MathF.h"

#include "ImguiManager.h"
#include <string>

#include "PostEffect.h"

#include "GameScene.h"
#include "DemoScene.h"
#include "ShaderTestScene.h"

//�@������ϊ��ƃT�E���h�f�[�^�������A�g�p���Ă��Ȃ�
//�A���g�̃R�[�h���ł�int�Ashort�͂��ׂ�int32_t�Ȃǂɒu�������� char�͈ꕔ�ł܂��g�p���Ă���
//�B�m�F�ł�������const& �������͒l�n���ɒu��������
//�C������ϊ��ƃT�E���h�f�[�^�������A�g�p���Ă��Ȃ�
//�D�����K���͈ȉ��̒ʂ�
//�����o�ϐ�->��O��m, m�̎��͑啶��(��:mHoge)
//�X�^�e�B�b�N�ϐ�->��O��s, s�̎��͑啶��(��:sHoge)
//�萔->�S���啶���A�敪���������Ƃ���_��t����(��:HOGE_HOGE_DANCE)
//���̑�(namespace���Ȃ�)->���ʂ̃L�������P�[�X

//windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int32_t WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int32_t nShowCmd)
{
	///---DirectX�����������@��������---///
	
#ifdef  _DEBUG
//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		//������O�Ƀf�o�C�X�𐶐�����ƃf�o�C�X���������炵��
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif _DEBUG

	WinAPI* winApi = WinAPI::Get();
	DirectX12* DX12 = DirectX12::Get();

#ifdef  _DEBUG
	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(DX12->mDevice->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	//���o�C�G���[���ɂƂ܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING,true);		//���[�j���O�����Ƃ܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR,true);		//�G���[���ɂƂ܂�
		infoQueue->Release();
	}
#endif  _DEBUG

	Input::Keyboard::Get()->Initialize();
	Input::Mouse::Initialize();
	Input::Pad::Initialize();

	///---DirectX�����������@�����܂�---///

	TimeManager::Init();

#pragma region �`�揉��������

	//depth�֘A�̏���
	CreateDepthView();

	//�e�N�X�`���̓ǂݍ���
	TextureManager* tManager = TextureManager::Get();
	tManager->Initialize();
	tManager->PreLoad();

	//imgui�̏�����
	ImguiManager *imguiManager = ImguiManager::GetInstance();
	imguiManager->Initialize();

	//���f���̓ǂݍ���
	ModelManager::Get()->PreLoad();

	//���C�e�B���O�̏�����
	LightGroup::Create();

	//�|�X�g�G�t�F�N�g�̏�����
	std::unique_ptr<PostEffect> postEffect;
	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();

	//�V�[���̏�����
	SceneManager *scenemanager = SceneManager::Get();
	//scenemanager->ChangeScene<GameScene>();
	scenemanager->ChangeScene<ShaderTestScene>();
	//scenemanager->ChangeScene<DemoScene>();

#pragma endregion �`�揉��������

	//���̏�����
	SoundManager *soundManager = SoundManager::GetInstance();
	soundManager->Initialize();

	//�p�C�v���C���̏�����
	PipelineManager::Get()->Initialize();

	SpriteCommon::sSpriteCommon.Initialize();

	//�Q�[�����[�v
	while (true){

#pragma region �E�B���h�E���b�Z�[�W
		if (PeekMessage(&winApi->mMsg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&winApi->mMsg);		//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&winApi->mMsg);		//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}

		if (winApi->mMsg.message == WM_QUIT) {
			break;
		}
#pragma endregion �E�B���h�E���b�Z�[�W

		///---DirectX���t���[������ ��������---///
		ClearDrawScreen();

		imguiManager->PreUpdate();

		//�X�V����
		Input::Keyboard::Get()->Update();
		Input::Mouse::Update();
		Input::Pad::Update();
		
		scenemanager->Update();
		postEffect->Update();

		///---DirectX���t���[������ �����܂�---///

		//--4.�`��R�}���h��������--//

#pragma region �|�X�g�G�t�F�N�g�Ή��̕`�����

		postEffect->PreDrawScene();
		scenemanager->Draw();
		postEffect->PostDrawScene();

		PreDraw();

		postEffect->Draw();

		//--4.�`��R�}���h�����܂�--//

		//--5.��ʓ��ꂩ��--//
		imguiManager->PreDraw();

		imguiManager->Draw();

		PostDraw();

#pragma endregion �|�X�g�G�t�F�N�g�Ή��̕`�����

#pragma region ���X�̕`�����
		/*{
			PreDraw();
			scenemanager->Draw();

			imguiManager->PreDraw();
			imguiManager->Draw();

			PostDraw();
		}*/
#pragma region ���X�̕`�����

		//--5.��ʓ��ꂩ�������--//

		TimeManager::Update();

		if (Input::Keyboard::Get()->PushKey(DIK_ESCAPE))
		{
			break;
		}
	}

	imguiManager->Finalize();

	//���̓f�o�C�X�̉��
	Input::Mouse::Finalize();
	Input::Keyboard::Get()->Finalize();

	//�����f�[�^�͐��xAudio2��������Ȃ���΂Ȃ�Ȃ�
	//xAudio2�̉��
	soundManager->End();

	//�E�B���h�E�N���X��o�^����
	UnregisterClass(winApi->w.lpszClassName, winApi->w.hInstance);

	return 0;
}