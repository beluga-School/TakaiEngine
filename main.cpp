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

#include "GeometryObject.h"


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

	Model cube = Cube();
	Model plane = Plate();
	Model triangle = Triangle();
	Model line = Line();

	Model triangleM = Model();
	Model skydome = Model();

	//���_�f�[�^�쐬
	cube.CreateDefaultModel();
	plane.CreateDefaultModel();
	triangle.CreateDefaultModel();
	line.CreateDefaultModel();

	//gitignore�̐ݒ�~�X��obj������ĂȂ�������
	triangleM.CreateModel("triangle_mat");
	skydome.CreateModel("skydome");

	//WIC�e�N�X�`���̃��[�h
	const wchar_t* msg[4] = 
	{ L"Resources/zawa_sironuri.png", 
		L"Resources/slime.png",
		L"Resources/pizza_sironuri.png",
		L"Resources/tyusiten.png"
	};

	std::unique_ptr<Texture> zawa = std::make_unique<Texture>();
	zawa->Load(msg[0]);

	std::unique_ptr<Texture> slime = std::make_unique<Texture>();
	slime->Load(msg[1]);

	std::unique_ptr<Texture> pizza = std::make_unique<Texture>();
	pizza->Load(msg[2]);

	std::unique_ptr<Texture> tyusiten = std::make_unique<Texture>();
	tyusiten->Load(msg[3]);

	std::unique_ptr<Texture> sinomiya = std::make_unique<Texture>();
	sinomiya->Load(L"Resources/triangle_mat/tex1.png");

	std::unique_ptr<Texture> white = std::make_unique<Texture>();
	white->CreateWhiteTexture();

	PipelineSet object3dPipelineSet = CreateObject3DPipeline();

	SpriteCommon spriteCommon;
	//spriteCommon = SpriteCommon::GetInstance();
	spriteCommon = SpriteCommonCreate();

	//�X�v���C�g
	Sprite pizzaSprite;
	pizzaSprite = SpriteCreate(pizza.get(), {0.5f,0.5f});
	SpriteInit(pizzaSprite, spriteCommon,{ 100,100}, 0);
	SpriteSetSize(pizzaSprite, { 100,100 });

	Sprite slimeSprite;
	slimeSprite = SpriteCreate(slime.get(), { 0.5f,0.5f });
	SpriteInit(slimeSprite, spriteCommon,{ 200,200 }, 45,{1,0,1,1});

	//�f�o�b�O�e�L�X�g����

	std::unique_ptr<Texture> debugFont = std::make_unique<Texture>();
	debugFont->Load(L"Resources/debugfont.png");

	DebugText debugText;
	debugText.Initialize(debugFont.get());

	//�W�I���g���I�u�W�F�N�g�p�p�C�v���C������
	PipelineSet geometryObjectPipelineSet = CreateGeometryPipeline();

	//�W�I���g���I�u�W�F�N�g����
	GeometryObject GObject;
	GObject.CreateModel();

	const int kObjectCount = 3;
	std::unique_ptr<Obj3d[]> object3ds;
	object3ds = std::make_unique<Obj3d[]>(kObjectCount);

	for (int i = 0; i < kObjectCount; i++)
	{
		object3ds[i].Initialize();
	}

	//�ǂ̃��f���̌`���g������ݒ�
	object3ds[0].SetModel(&cube);
	object3ds[1].SetModel(&plane);
	object3ds[2].SetModel(&skydome);

	object3ds[0].scale.x = 10.0f;
	object3ds[0].scale.y = 10.0f;

	object3ds[1].position.x = 0;
	object3ds[1].position.y = -10;
	object3ds[1].scale.x = 10.0f;
	object3ds[1].scale.y = 10.0f;
	object3ds[1].rotation.x += XMConvertToRadians(90.0f);
	
	////���̃��f�����g���ݒ�
	//const int kLineCountX = 15;
	//const int kLineCountY = 15;
	//std::unique_ptr<Obj3d[]> LineX;
	//LineX = std::make_unique<Obj3d[]>(kLineCountX);

	//for (int i = 0; i < kLineCountX; i++)
	//{
	//	LineX[i].Initialize();
	//	LineX[i].SetModel(&line);
	//	LineX[i].position.x += 10 * i - (10 * kLineCountX / 2);
	//}

	//std::unique_ptr<Obj3d[]> LineY;
	//LineY = std::make_unique<Obj3d[]>(kLineCountY);

	//for (int i = 0; i < kLineCountY; i++)
	//{
	//	LineY[i].Initialize();
	//	LineY[i].SetModel(&line);
	//	//LineY[i].position.x += 10 * i;
	//	LineY[i].position.z += 10 * i - (10 * kLineCountY / 2);

	//	LineY[i].rotation.y += XMConvertToRadians(90.0f);
	//}

	//�ˉe�ϊ��s��(�������e)
	XMMATRIX matProjection =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),
			(float)window_width / window_height,
			0.1f, 1000.0f
		);

	//�r���[�ϊ��s��(�������e)���v�Z
	View view_;
	view_.Initialize();

	view_.eye.y = 50.0f;
	view_.UpdatematView();

	Billboard billboard = Billboard(&view_,false);
	billboard.Initialize();
	billboard.SetModel(&triangleM);

	billboard.position = { 10,10,0 };
	billboard.scale = { 100,100,100 };

#pragma endregion �`�揉��������

	SoundManager soundManager;
	soundManager.Initialize();

	SoundData curser = soundManager.SoundLoadWave("Resources\\sound\\curser.wav");

	gameScene_.Initialize();

	//�Q�[�����[�v���Ŏg���ϐ��̐錾

	float angle = 0.0f;	//�J�����̉�]�p
	float angleY = 0.0f;

	float cameraY = 100;

	//float count = 0;

	int count = 0;

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

		if (input_->TriggerKey(DIK_SPACE))
		{
			//soundManager.SoundPlayWave(curser);
		}

		//�J�������W�𓮂���
		if (input_->PushKey(DIK_RIGHT) || 
			input_->PushKey(DIK_LEFT) || 
			input_->PushKey(DIK_UP) || 
			input_->PushKey(DIK_DOWN)||
			input_->PushKey(DIK_SPACE))
		{
			if (input_->PushKey(DIK_RIGHT)) { 
				view_.eye.x += 2;
			}
			if (input_->PushKey(DIK_LEFT)) { 
				view_.eye.x -= 2; 
			}
			if (input_->PushKey(DIK_UP)) {
				view_.eye.y += 2;
			}
			if (input_->PushKey(DIK_DOWN)) {
				view_.eye.y -= 2;
			}

			if (input_->PushKey(DIK_SPACE))
			{
				if (input_->PushKey(DIK_RIGHT)) {
				
					view_.target.x += 2;
				}
				if (input_->PushKey(DIK_LEFT)) {
					
					view_.target.x -= 2;
				}
				if (input_->PushKey(DIK_UP)) {
					
					view_.target.y += 2;
				}
				if (input_->PushKey(DIK_DOWN)) {
					
					view_.target.y -= 2;
				}
			}
			view_.UpdatematView();
		}

		if (input_->TriggerKey(DIK_R))
		{
			for (int i = 0; i < kObjectCount; i++)
			{
				object3ds[i].Initialize();
			}
			view_.Initialize();
			view_.eye.y = 50.0f;
			view_.UpdatematView();
		}

		/*if (input_->PushKey(DIK_D))
		{
			billboard.position.x += 10.0f * TimeManager::deltaTime;
		}

		if (input_->PushKey(DIK_A))
		{
			billboard.position.x -= 10.0f * TimeManager::deltaTime;
		}*/

		//���΋��������߂�
		XMFLOAT3 offset = { 0,0,0 };
		offset.x = view_.eye.x - object3ds[0].position.x;
		offset.y = view_.eye.y - object3ds[0].position.y;
		offset.z = view_.eye.z - object3ds[0].position.z;

		view_.eye.x = object3ds[0].position.x + offset.x;
		view_.eye.y = object3ds[0].position.y + offset.y;
		view_.eye.z = object3ds[0].position.z + offset.z;

		view_.UpdatematView();
		//�I�u�W�F�N�g�̍X�V
		if (input_->PushKey(DIK_D))
		{
			billboard.position.x += 60.0f * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_A))
		{
			billboard.position.x -= 60.0f * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_W))
		{
			billboard.position.y += 60.0f * TimeManager::deltaTime;
		}
		if (input_->PushKey(DIK_S))
		{
			billboard.position.y -= 60.0f * TimeManager::deltaTime;
		}
		for (size_t i = 0; i < kObjectCount; i++)
		{
			object3ds[i].Update(view_.matView, matProjection);
		}

		if (input_->PushKey(DIK_Q))
		{
			billboard.yBillboardMode = true;
		}
		else
		{
			billboard.yBillboardMode = false;
		}

		if (input_->PushKey(DIK_1))
		{
			TimeManager::fixFPS = 30;
		}
		if (input_->PushKey(DIK_2))
		{
			TimeManager::fixFPS = 60;
		}
		if (input_->PushKey(DIK_3))
		{
			TimeManager::fixFPS = 120;
		}

		GObject.Update(view_.matView,matProjection);

		billboard.Update(matProjection);

		SpriteUpdate(pizzaSprite, spriteCommon);
		SpriteUpdate(slimeSprite, spriteCommon);

	
		count++;
		
		debugText.Print(spriteCommon,
			"deltaTime " + std::to_string(TimeManager::deltaTime), 50, 100, 1.0f);

		debugText.Print(spriteCommon,
			"fps " + std::to_string(TimeManager::fps), 50, 200, 1.0f);

		///---DirectX���t���[������ �����܂�---///
#pragma endregion DirectX���t���[������

#pragma region �O���t�B�b�N�X�R�}���h
		//--4.�`��R�}���h��������--//
		BasicObjectPreDraw( object3dPipelineSet);

		//�`�揈��
		gameScene_.Draw();

		//object3ds[0].MaterialDraw(DX12->commandList.Get());
		//object3ds[0].Draw(slime.get());
		object3ds[2].MaterialDraw();

		//billboard.Draw(slime.get());

		GeometryObjectPreDraw(geometryObjectPipelineSet);
		GObject.Draw(slime.get());


		//�X�v���C�g�̑O�`��(���ʃR�}���h)
		SpriteCommonBeginDraw(spriteCommon);

		//�X�v���C�g�P�̕`��
		SpriteDraw(pizzaSprite);
		SpriteDraw(slimeSprite);

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