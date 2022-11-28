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

	//--depth��������

	CreateDepthView();

	//--depth�����܂�

	GameScene gameScene_;

	//WIC�e�N�X�`���̃��[�h
	TextureManager::GetInstance()->Initialize();
	TextureManager::GetInstance()->PreLoad();

	SpriteCommon::spriteCommon.Initialize();

	ModelManager::GetInstance()->PreLoad();

	PipelineSet object3dPipelineSet = CreateObject3DPipeline();

	//�W�I���g���I�u�W�F�N�g�p�p�C�v���C������
	PipelineSet geometryObjectPipelineSet = CreateGeometryPipeline();

	//�X�v���C�g
	Sprite pizzaSprite;
	SpriteCreate(&pizzaSprite,&TextureManager::GetInstance()->pizza, {0.5f,0.5f});
	SpriteInit(pizzaSprite, SpriteCommon::spriteCommon,{ 100,100}, 0);
	SpriteSetSize(pizzaSprite, { 100,100 });

	Sprite slimeSprite;
	SpriteCreate(&slimeSprite, &TextureManager::GetInstance()->pizza, { 0.5f,0.5f });
	SpriteInit(slimeSprite, SpriteCommon::spriteCommon,{ 200,200 }, 45,{1,0,1,1});

	//�f�o�b�O�e�L�X�g����

	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(L"Resources/debugfont.png");

	DebugText debugText;
	debugText.Initialize(debugFont.get());

	//�W�I���g���I�u�W�F�N�g����
	ParticleManager *particleManager = ParticleManager::Getinstance();
	particleManager->Initialize();

	Obj3d skydome;
	skydome.Initialize();

	//�ǂ̃��f���̌`���g������ݒ�
	skydome.SetModel(&ModelManager::GetInstance()->skyDomeM);

	skydome.SetTexture(&TextureManager::GetInstance()->slime);

	//�r���[�ϊ��s��(�������e)���v�Z
	Camera *camera = Camera::camera;
	camera->Initialize();

	Obj3d cubeObj;
	cubeObj.Initialize();
	cubeObj.SetModel(&ModelManager::GetInstance()->cubeM);
	cubeObj.SetTexture(&TextureManager::GetInstance()->slime);

	cubeObj.position = { 0,0,10 };

	Billboard billboard = Billboard(camera,false);
	billboard.Initialize();
	billboard.SetModel(&ModelManager::GetInstance()->boardM);
	billboard.SetTexture(&TextureManager::GetInstance()->pizza);

	billboard.position = { 10,10,10 };
	billboard.scale = { 3,3,3 };

	camera->target = {
		0,0,0
	};

	camera->SetEye({ 0,20,-10 });

	//camera->SetEye({ 10,10,10 });

#pragma endregion �`�揉��������

	SoundManager soundManager;
	soundManager.Initialize();

	SoundData curser = soundManager.SoundLoadWave("Resources\\sound\\curser.wav");

	gameScene_.Initialize();

	//�Q�[�����[�v���Ŏg���ϐ��̐錾

	float angle = 0.0f;	//�J�����̉�]�p
	float angleY = 0.0f;

	ParticleEmitter pEmitter;
	pEmitter.Initialize();
	pEmitter.SetInfo({ -10,-10,20 }, 10, 5,{0,0.5f,1,1}, 1, true);

	ParticleEmitter pEmitter2;
	pEmitter2.Initialize();
	pEmitter2.SetInfo({ 10,10,10 }, 3, 1, { 1,1,1,1 }, 1);	
	
	ParticleEmitter pEmitter3;
	pEmitter3.Initialize();
	pEmitter3.SetInfo({ 20,-20,10 }, 10, 1, { 0,0.1f,0.1f,1 }, 10);

	SceneManager sceneManager;

	Scene Tscene("Title");
	Scene Nscene("newGame");
	Scene Cscene("gameClear");
	Scene Oscene("gameOver");

	sceneManager.LoadScene(Tscene);
	sceneManager.LoadScene(Nscene);
	sceneManager.LoadScene(Cscene);
	sceneManager.LoadScene(Oscene);

	sceneManager.ChangeScene("Title");

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

		//�X�V����

		gameScene_.Update();

		input_->Update();

		pEmitter.Update();
		pEmitter2.Update();
		pEmitter3.Update();

		//�J�������W�𓮂���
		if (input_->PushKey(DIK_RIGHT)) {
			camera->eye.x += 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_LEFT)) {
			camera->eye.x -= 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_UP)) {
			camera->eye.y += 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_DOWN)) {
			camera->eye.y -= 20 * TimeManager::deltaTime;
		}

		if (input_->PushKey(DIK_D)) {

			camera->eye.x += 20 * TimeManager::deltaTime;
			camera->target.x += 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_A)) {

			camera->eye.x -= 20 * TimeManager::deltaTime;
			camera->target.x -= 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_W)) {

			camera->eye.z += 20 * TimeManager::deltaTime;
			camera->target.z += 20 * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_S)) {

			camera->eye.z -= 20 * TimeManager::deltaTime;
			camera->target.z -= 20 * TimeManager::deltaTime;
		}

		camera->UpdatematView();

		if (input_->TriggerKey(DIK_R))
		{
			camera->Initialize();
			camera->eye.y = 20.0f;
			camera->UpdatematView();
		}

		camera->UpdatematView();
	
		if (input_->TriggerKey(DIK_1))
		{
			sceneManager.ChangeScene("Title");
			//TimeManager::fixFPS = 30;
			//TimeManager::fps = 0;
		}
		if (input_->TriggerKey(DIK_2))
		{
			sceneManager.ChangeScene("newGame");
			//TimeManager::fixFPS = 60;
			//TimeManager::fps = 0;
		}
		if (input_->TriggerKey(DIK_3))
		{
			sceneManager.ChangeScene("gameClear");
			//TimeManager::fixFPS = 120;
			//TimeManager::fps = 0;
		}
		if (input_->TriggerKey(DIK_4))
		{
			sceneManager.ChangeScene("gameOver");
			//TimeManager::fixFPS = 120;
			//TimeManager::fps = 0;
		}

		//�I�u�W�F�N�g�̍X�V
		particleManager->Update(camera->matView, camera->matProjection);

		skydome.Update(camera->matView, camera->matProjection);

		cubeObj.Update(camera->matView, camera->matProjection);
		billboard.Update(camera->matProjection);

		SpriteUpdate(pizzaSprite, SpriteCommon::spriteCommon);
		SpriteUpdate(slimeSprite, SpriteCommon::spriteCommon);

		debugText.Print(SpriteCommon::spriteCommon,
			"deltaTime " + std::to_string(TimeManager::deltaTime), 50, 500);

		debugText.Print(SpriteCommon::spriteCommon,
			"fps " + std::to_string(TimeManager::fps), 50, 550);

		debugText.Print(SpriteCommon::spriteCommon,
			"FixFPS " + std::to_string(TimeManager::fixFPS), 50, 600);
		
		debugText.Print(SpriteCommon::spriteCommon,
			"particles.size " + std::to_string(particleManager->particles.size()), 50, 350);

		debugText.Print(SpriteCommon::spriteCommon,
			sceneManager.nowScene->str, 50, 400);

		///---DirectX���t���[������ �����܂�---///
#pragma endregion DirectX���t���[������

#pragma region �O���t�B�b�N�X�R�}���h
		//--4.�`��R�}���h��������--//
		BasicObjectPreDraw( object3dPipelineSet);

		//�`�揈��
		gameScene_.Draw();

		billboard.Draw();
		cubeObj.Draw();
		
		skydome.DrawMaterial();


		GeometryObjectPreDraw(geometryObjectPipelineSet);
		particleManager->Draw(&TextureManager::GetInstance()->particle);


		//�X�v���C�g�̑O�`��(���ʃR�}���h)
		SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

		//�X�v���C�g�P�̕`��
		//SpriteDraw(pizzaSprite);
		//SpriteDraw(slimeSprite);

		debugText.DrawAll();

		//--4.�`��R�}���h�����܂�--//

#pragma endregion �O���t�B�b�N�X�R�}���h

#pragma region ��ʓ���ւ�

		PostDraw();

		debugText.PostDraw();

#pragma endregion ��ʓ���ւ�

		TimeManager::Update();

		if (input_->PushKey(DIK_ESCAPE))
		{
			break;
		}
	}

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