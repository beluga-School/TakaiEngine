#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>
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

#include "DemoScene.h"
#include "GameScene.h"
#include <FBXLoadDemoScene.h>
#include "SceneFactory.h"

#include "SceneManager.h"

#include "Sound.h"
#include "DebugText.h"

#include "Particle.h"
#include "MathF.h"

#include "ImguiManager.h"
#include <string>

//�\����x64�̂�

//windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

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

	WinAPI* winApi = WinAPI::GetInstance();
	DirectX12* DX12 = DirectX12::Get();

#ifdef  _DEBUG
	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(DX12->device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);	//���o�C�G���[���ɂƂ܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING,true);		//���[�j���O�����Ƃ܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR,true);		//�G���[���ɂƂ܂�
		infoQueue->Release();
	}
#endif  _DEBUG

	Input* input_ = Input::Get();

	input_->Initialize();

	///---DirectX�����������@�����܂�---///

	TimeManager::Init();

#pragma region �`�揉��������

	//depth�֘A�̏���
	CreateDepthView();

	//�e�N�X�`���̓ǂݍ���
	TextureManager* tManager = TextureManager::Get();
	tManager->Initialize();
	tManager->PreLoad();

	//�f�o�b�O�e�L�X�g����
	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(L"Resources/debugfont.png");

	DebugText debugText;
	debugText.Initialize(debugFont.get());

	//imgui�̏�����
	ImguiManager *imguiManager = ImguiManager::GetInstance();
	imguiManager->Initialize();

	SpriteCommon::spriteCommon.Initialize();

	//���f���̓ǂݍ���
	ModelManager::Get()->PreLoad();

	LightGroup::lightGroup = LightGroup::Create();

	SceneManager *scenemanager = SceneManager::Get();
	
	//�V�[���t�@�N�g���[�𐶐����A�}�l�[�W���ɃZ�b�g
	SceneFactory *sceneFactory = new SceneFactory();
	scenemanager->SetSceneFactory(sceneFactory);

	DemoScene *demo = new DemoScene();
	//FBXLoadDemoScene* loaddemo = new FBXLoadDemoScene();
	//Game *game = new Game();
	scenemanager->SetScene(demo);

#pragma endregion �`�揉��������

	SoundManager *soundManager = SoundManager::GetInstance();
	soundManager->Initialize();

	//�Q�[�����[�v���Ŏg���ϐ��̐錾
	//ParticleEmitter pEmitter;
	//pEmitter.Initialize();
	//pEmitter.SetInfo({ -10,-10,20 }, 10, 5,{0,0.5f,1,1}, 1, true);

	//soundManager->SoundPlayWave(bgm,true);


	//�Q�[�����[�v
	while (true){

#pragma region �E�B���h�E���b�Z�[�W
		if (PeekMessage(&winApi->msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&winApi->msg);		//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&winApi->msg);		//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}

		if (winApi->msg.message == WM_QUIT) {
			break;
		}
#pragma endregion �E�B���h�E���b�Z�[�W

#pragma region DirectX���t���[������
		///---DirectX���t���[������ ��������---///
		
		ClearDrawScreen();

		imguiManager->PreUpdate();

		//�X�V����
		input_->Update();
	
		
		//gameScene_.Update();


		scenemanager->Update();

		//pEmitter.Update();

		///---DirectX���t���[������ �����܂�---///
#pragma endregion DirectX���t���[������

#pragma region �O���t�B�b�N�X�R�}���h
		//--4.�`��R�}���h��������--//
		//gameScene_.Draw();

		scenemanager->Draw();

		debugText.DrawAll();

		//--4.�`��R�}���h�����܂�--//

#pragma endregion �O���t�B�b�N�X�R�}���h

#pragma region ��ʓ���ւ�

		imguiManager->PreDraw();

		imguiManager->Draw();

		PostDraw();

		debugText.PostDraw();

#pragma endregion ��ʓ���ւ�

		TimeManager::Update();

		if (input_->PushKey(DIK_ESCAPE))
		{
			break;
		}
	}

	imguiManager->Finalize();

	delete sceneFactory;

	//�����f�[�^�͐��xAudio2��������Ȃ���΂Ȃ�Ȃ�
	//xAudio2�̉��
	soundManager->End();

	//�E�B���h�E�N���X��o�^����
	UnregisterClass(winApi->w.lpszClassName, winApi->w.hInstance);

	return 0;
}