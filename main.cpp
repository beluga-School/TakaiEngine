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
#include "Vector3.h"
#include "Texture.h"

#include "Sprite.h"
#include "Pipeline.h"
#include "ClearDrawScreen.h"
#include "Billboard.h"

#include "GameScene.h"

#include "Sound.h"
#include "DebugText.h"

#include "Particle.h"
#include "MathF.h"

#include "SceneManager.h"

#include "ImguiManager.h"
#include <string>

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
	DirectX12* DX12 = DirectX12::GetInstance();

#ifdef  _DEBUG
	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(DX12->device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION);//���o�C�G���[���ɂƂ܂�
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR);	 //�G���[���Ɏ~�܂�
		infoQueue->Release();
	}
#endif  _DEBUG

	Input* input_ = Input::GetInstance();

	//DirectInput�̏�����
	input_->DirectInputInit();

	//�L�[�{�[�h�f�o�C�X�̐���
	input_->DirectInputCreate();

	///---DirectX�����������@�����܂�---///

	TimeManager::Init();

#pragma region �`�揉��������

	//depth�֘A�̏���
	CreateDepthView();

	//�e�N�X�`���̓ǂݍ���
	TextureManager* tManager = TextureManager::GetInstance();
	tManager->Initialize();
	tManager->PreLoad();

	//imgui�̏�����
	ImguiManager *imguiManager = ImguiManager::GetInstance();
	imguiManager->Initialize();

	SpriteCommon::spriteCommon.Initialize();

	//���f���̓ǂݍ���
	ModelManager::GetInstance()->PreLoad();

	//�f�o�b�O�e�L�X�g����
	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(L"Resources/debugfont.png");

	DebugText debugText;
	debugText.Initialize(debugFont.get());

	GameScene gameScene_;
	gameScene_.Initialize();

#pragma endregion �`�揉��������

	SoundManager soundManager;
	soundManager.Initialize();

	SoundData curser = soundManager.SoundLoadWave("Resources\\sound\\curser.wav");

	//�Q�[�����[�v���Ŏg���ϐ��̐錾
	ParticleEmitter pEmitter;
	pEmitter.Initialize();
	pEmitter.SetInfo({ -10,-10,20 }, 10, 5,{0,0.5f,1,1}, 1, true);

	GUI gui("GUI");
	GUI gui2("How to operate");

	float hoge = 0;

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

		gui.Begin({10,10},{500,200});
		ImGui::SliderFloat("fov", &gameScene_.camera->fovAngle, 30, 100);
		ImGui::SliderFloat("camEyeY", &gameScene_.camera->eye.y, -50, 50);
		ImGui::SliderFloat("camTargetY", &gameScene_.camera->target.y, -50, 50);
		ImGui::SliderFloat("darumaPosY", &gameScene_.daruma.position.y, -50, 50);
		ImGui::SliderFloat("darumarota", &gameScene_.daruma.rotation.z, -50, 50);
		ImGui::SliderFloat("darumarota", &gameScene_.daruma.rotation.y, -50, 50);
		ImGui::Text("colflag %d",gameScene_.daruma.onGround);

		gui.End();

		gui2.Begin({Util::window_width - 200,10},{200,200});
		ImGui::Text("W,A,S,D:darumaObject move");
		ImGui::Text("ArrowKey:camera move");
		ImGui::Text("RKey:daruma pos reset");
		
		gui2.End();

		//�X�V����
		input_->Update();
	
		gameScene_.Update();

		debugText.Print(SpriteCommon::spriteCommon,
			"a" + std::to_string(1),
			50, 100);

		pEmitter.Update();

		///---DirectX���t���[������ �����܂�---///
#pragma endregion DirectX���t���[������

#pragma region �O���t�B�b�N�X�R�}���h
		//--4.�`��R�}���h��������--//
		gameScene_.Draw();

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

	gameScene_.End();

	//�����f�[�^�͐��xAudio2��������Ȃ���΂Ȃ�Ȃ�
	//xAudio2�̉��
	soundManager.End();

	//�����f�[�^�̉��
	soundManager.SoundUnload(&curser);

	//�E�B���h�E�N���X��o�^����
	UnregisterClass(winApi->w.lpszClassName, winApi->w.hInstance);

	return 0;
}