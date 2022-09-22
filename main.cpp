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
#include "Result.h"
#include "Input.h"
#include "DirectXInit.h"
#include "Vertex.h"
#include "Shader.h"
#include "Obj.h"
#include "ViewProjection.h"
#include "Vector3.h"
#include "Model.h"
#include "Texture.h"
#include "GameScene.h"

WinAPI winApi_;

struct PipelineSet
{
	//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipelinestate;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootsignature;
};

enum BLEND_MODE
{
	ADD,
	SUB,
	INV,
	ALPHA,
};

PipelineSet CreateObject3DPipeline();

PipelineSet CreateSpritePipeline();

//windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	GameScene gameScene_;

	Input* input_ = new Input;

	//WindowsAPI����������
	winApi_.SetWindowClass();

	///---DirectX�����������@��������---///
	
#ifdef  _DEBUG
	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}

#endif _DEBUG

	DX12.yobidasi(winApi_);

#ifdef  _DEBUG
	ID3D12InfoQueue* infoQueue;
	if (SUCCEEDED(DX12.device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION);//���o�C�G���[���ɂƂ܂�
		infoQueue->GetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR);	 //�G���[���Ɏ~�܂�
		infoQueue->Release();
	}
#endif  _DEBUG

	//DirectInput�̏�����
	input_->DirectInputInit(winApi_);

	//�L�[�{�[�h�f�o�C�X�̐���
	input_->DirectInputCreate(winApi_);

	///---DirectX�����������@�����܂�---///

#pragma region �`�揉��������

	//--depth��������

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = window_width;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = window_height;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	//�[�x�l�p�q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	//�[�x�l�̃N���A�ݒ�
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	//�[�x�l1.0f(�ő�l)�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//���\�[�X����
	ComPtr<ID3D12Resource> depthBuff;
	result = DX12.device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//�f�v�X�X�e���V���r���[
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	result = DX12.device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	DX12.device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());

	//--depth�����܂�

	Model cube = Cube();
	Model sprite = Sprite();
	Model triangle = Triangle();
	Model line = Line();

	//���_�f�[�^�쐬
	cube.CreateModel(DX12);
	sprite.CreateModel(DX12);
	triangle.CreateModel(DX12);
	line.CreateModel(DX12);

	//WIC�e�N�X�`���̃��[�h
	const wchar_t* msg[4] = 
	{ L"Resources/zawa_sironuri.png", 
		L"Resources/slime.png",
		L"Resources/pizza_sironuri.png",
		L"Resources/tyusiten.png"
	};

	Texture zawa;
	zawa.Load(msg[0], DX12);

	Texture slime;
	slime.Load(msg[1], DX12);

	Texture pizza;
	pizza.Load(msg[2], DX12);

	Texture tyusiten;
	tyusiten.Load(msg[3], DX12);

	Texture white;
	white.CreateWhiteTexture(DX12);

	PipelineSet object3dPipelineSet = CreateObject3DPipeline();

	///��������X�v���C�g�p
#pragma region �X�v���C�g�p
	PipelineSet spritePipelineSet = CreateSpritePipeline();

#pragma endregion

	///�����܂ŃX�v���C�g�p

	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	//�q�[�v�ݒ�
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
	//���\�[�X�ݒ�
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//�萔�o�b�t�@�̐���
	ConstBuffer<ConstBufferDataMaterial> constBufferM;
	ConstBuffer<ConstBufferDataMaterial> constBufferM2;

	const int kObjectCount = 2;
	std::unique_ptr<Obj3d[]> object3ds;
	object3ds = std::make_unique<Obj3d[]>(kObjectCount);

	for (int i = 0; i < kObjectCount; i++)
	{
		object3ds[i].Initialize();
	}

	//�ǂ̃��f���̌`���g������ݒ�
	object3ds[0].SetModel(&cube);
	object3ds[1].SetModel(&sprite);
	//object3ds[2].SetModel(&triangle);
	//object3ds[3].SetModel(&sprite);

	//object3ds[1].position.x = 20;
	//object3ds[1].position.y = 10;

	//object3ds[2].position.x = -20;
	//object3ds[2].position.y = 0;

	object3ds[1].position.x = 0;
	object3ds[1].position.y = -10;
	object3ds[1].scale.x = 10.0f;
	object3ds[1].scale.y = 10.0f;
	object3ds[1].rotation.x += XMConvertToRadians(90.0f);
	
	//���̃��f�����g���ݒ�
	const int kLineCountX = 15;
	const int kLineCountY = 15;
	std::unique_ptr<Obj3d[]> LineX;
	LineX = std::make_unique<Obj3d[]>(kLineCountX);

	for (int i = 0; i < kLineCountX; i++)
	{
		LineX[i].Initialize();
		LineX[i].SetModel(&line);
		LineX[i].position.x += 10 * i - (10 * kLineCountX / 2);
	}

	std::unique_ptr<Obj3d[]> LineY;
	LineY = std::make_unique<Obj3d[]>(kLineCountY);

	for (int i = 0; i < kLineCountY; i++)
	{
		LineY[i].Initialize();
		LineY[i].SetModel(&line);
		//LineY[i].position.x += 10 * i;
		LineY[i].position.z += 10 * i - (10 * kLineCountY / 2);

		LineY[i].rotation.y += XMConvertToRadians(90.0f);
	}

	//�l���������ނƎ����I�ɓ]�������炵��
	constBufferM.constBufferData->color = XMFLOAT4(1, 1, 1, 1.0f);

	//�ˉe�ϊ��s��(�������e)
	XMMATRIX matProjection =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),
			(float)window_width / window_height,
			0.1f, 1000.0f
		);

	//�r���[�ϊ��s��(�������e)���v�Z
	ViewProjection viewProjection_;
	viewProjection_.Initialize();

	viewProjection_.eye.y = 50.0f;
	viewProjection_.UpdatematView();

#pragma endregion �`�揉��������

	gameScene_.Initialize();

	//�Q�[�����[�v���Ŏg���ϐ��̐錾
	FLOAT clearColor[] = { 0,0,0,0 };

	FLOAT redColor[] = { 0.5f,0.1f,0.25f,0.0f };	//�Ԃ��ۂ��F
	FLOAT blueColor[] = { 0.1f,0.25f,0.5f,0.0f };	//���ۂ��F

	for (int i = 0; i < _countof(clearColor); i++)
	{
		clearColor[i] = blueColor[i];
	}

	//XMFLOAT3 materialColor = { -0.005f,0.005f,0 };

	float angle = 0.0f;	//�J�����̉�]�p
	float angleY = 0.0f;

	float cameraY = 100;

	float count = 0;

	XMFLOAT4 color = { 0,1,0.5f,1.0f };
	XMFLOAT4 colorSpd = { 0.01f,0.01f,0.01f,0.01f };

	//�Q�[�����[�v
	while (true){

#pragma region �E�B���h�E���b�Z�[�W
		if (PeekMessage(&winApi_.msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&winApi_.msg);		//�L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&winApi_.msg);		//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}

		if (winApi_.msg.message == WM_QUIT) {
			break;
		}
#pragma endregion �E�B���h�E���b�Z�[�W

#pragma region DirectX���t���[������
		///---DirectX���t���[������ ��������---///
		
		UINT bbIndex = DX12.swapChain->GetCurrentBackBufferIndex();

		//1.���\�[�X�o���A�ŏ������݉��ɕύX
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = DX12.backBuffers[bbIndex].Get();	//�o�b�N�o�b�t�@���w��
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	//�\����Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		//�`���Ԃ�
		DX12.commandList->ResourceBarrier(1, &barrierDesc);

		//2.�`���̕ύX
		//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = DX12.rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * DX12.device->GetDescriptorHandleIncrementSize(DX12.rtvHeapDesc.Type);
		//DX12.commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		DX12.commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		//3.��ʃN���A
		DX12.commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		DX12.commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


		//�X�V����

		gameScene_.Update();

		input_->Update();

		//�J�������W�𓮂���
		if (input_->PushKey(DIK_RIGHT) || 
			input_->PushKey(DIK_LEFT) || 
			input_->PushKey(DIK_UP) || 
			input_->PushKey(DIK_DOWN)||
			input_->PushKey(DIK_SPACE))
		{
			if (input_->PushKey(DIK_RIGHT)) { 
				viewProjection_.eye.x += 2;
				//viewProjection_.target.x += 2;
			}
			if (input_->PushKey(DIK_LEFT)) { 
				viewProjection_.eye.x -= 2; 
				//viewProjection_.target.x -= 2;
			}
			if (input_->PushKey(DIK_UP)) {
				viewProjection_.eye.z += 2;
				//viewProjection_.target.z += 2;
			}
			if (input_->PushKey(DIK_DOWN)) {
				viewProjection_.eye.z -= 2;
				//viewProjection_.target.z -= 2;
			}
			if (input_->PushKey(DIK_SPACE))
			{
				if (input_->PushKey(DIK_RIGHT)) {
				
					viewProjection_.target.x += 2;
				}
				if (input_->PushKey(DIK_LEFT)) {
					
					viewProjection_.target.x -= 2;
				}
				if (input_->PushKey(DIK_UP)) {
					
					viewProjection_.target.z += 2;
				}
				if (input_->PushKey(DIK_DOWN)) {
					
					viewProjection_.target.z -= 2;
				}
			}
			viewProjection_.UpdatematView();
		}

		if (input_->TriggerKey(DIK_R))
		{
			for (int i = 0; i < kObjectCount; i++)
			{
				object3ds[i].Initialize();
			}
			viewProjection_.Initialize();
			viewProjection_.eye.y = 50.0f;
			viewProjection_.UpdatematView();
		}

		//���΋��������߂�
		XMFLOAT3 offset = { 0,0,0 };
		offset.x = viewProjection_.eye.x - object3ds[0].position.x;
		offset.y = viewProjection_.eye.y - object3ds[0].position.y;
		offset.z = viewProjection_.eye.z - object3ds[0].position.z;

		viewProjection_.eye.x = object3ds[0].position.x + offset.x;
		viewProjection_.eye.y = object3ds[0].position.y + offset.y;
		viewProjection_.eye.z = object3ds[0].position.z + offset.z;

		//viewProjection_.target.x = object3ds[0].position.x + offset.x;
		//viewProjection_.target.y = object3ds[0].position.y + offset.y;
		//viewProjection_.target.z = object3ds[0].position.z + offset.z;

		viewProjection_.UpdatematView();

		//�I�u�W�F�N�g�̍X�V
		for (size_t i = 0; i < kObjectCount; i++)
		{
			if (input_->PushKey(DIK_D))
			{
				object3ds[i].position.x += 1;
			}
			if (input_->PushKey(DIK_A))
			{
				object3ds[i].position.x -= 1;
			}
			if (input_->PushKey(DIK_W))
			{
				object3ds[i].position.z += 1;
			}
			if (input_->PushKey(DIK_S))
			{
				object3ds[i].position.z -= 1;
			}
			if (input_->PushKey(DIK_Q))
			{
				object3ds[i].rotation.z += 0.1f;
			}
			if (input_->PushKey(DIK_E))
			{
				object3ds[i].rotation.z -= 0.1f;
			}
			object3ds[i].Update(viewProjection_.matView, matProjection);
		}

		for (int i = 0; i < kLineCountX; i++)
		{
			LineX[i].Update(viewProjection_.matView, matProjection);
		}
		for (int i = 0; i < kLineCountY; i++)
		{
			LineY[i].Update(viewProjection_.matView, matProjection);
		}

		//�F��ς��鏈��
		if (false)
		{
			if (color.x < 0 || color.x > 1)
			{
				colorSpd.x *= -1;
			}
			if (color.y < 0 || color.y > 1)
			{
				colorSpd.y *= -1;
			}
			if (color.z < 0 || color.z > 1)
			{
				colorSpd.z *= -1;
			}
			if (color.z < 0 || color.z > 1)
			{
				colorSpd.w *= -1;
			}
			color.x += colorSpd.x;
			color.y += colorSpd.y;
			color.z += colorSpd.z;
			color.w += colorSpd.w;

			constBufferM.constBufferData->color = XMFLOAT4(color.x, color.y, color.z, 1.0f);
			constBufferM2.constBufferData->color = XMFLOAT4(1.0f, 1.0f, 1.0f, color.w);
		}

		///---DirectX���t���[������ �����܂�---///
#pragma endregion DirectX���t���[������

#pragma region �O���t�B�b�N�X�R�}���h
		//--4.�`��R�}���h��������--//
		
		D3D12_VIEWPORT viewport{};
		viewport.Width = window_width;
		viewport.Height = window_height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		//�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		DX12.commandList->RSSetViewports(1, &viewport);

		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									//�؂蔲�����W��
		scissorRect.right = scissorRect.left + window_width;	//�؂蔲�����W�E
		scissorRect.top = 0;									//�؂蔲�����W��
		scissorRect.bottom = scissorRect.top + window_height;	//�؂蔲�����W��
		//�V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		DX12.commandList->RSSetScissorRects(1, &scissorRect);

		//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		//�X�v���C�g����Ȃ���
		DX12.commandList->SetPipelineState(object3dPipelineSet.pipelinestate.Get());
		DX12.commandList->SetGraphicsRootSignature(object3dPipelineSet.rootsignature.Get());

		//�v���~�e�B�u�`��̐ݒ�R�}���h
		DX12.commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		object3ds[0].constBufferM = constBufferM;
		object3ds[1].constBufferM = constBufferM2;
		//object3ds[2].constBufferM = constBufferM2;

		//�`�揈��
		gameScene_.Draw();

		object3ds[0].Draw(DX12.commandList.Get(), pizza);
		
		if (input_->PushKey(DIK_SPACE))
		{
			object3ds[1].Draw(DX12.commandList.Get(), tyusiten);
		}

		for (int i = 0; i < kLineCountX; i++)
		{
			LineX[i].Draw(DX12.commandList.Get(), white);
		}
		for (int i = 0; i < kLineCountY; i++)
		{
			LineY[i].Draw(DX12.commandList.Get(), white);
		}

		//--4.�`��R�}���h�����܂�--//

#pragma endregion �O���t�B�b�N�X�R�}���h

#pragma region ��ʓ���ւ�

		//5.���\�[�X�o���A��߂�
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;	//�\����Ԃ�
		DX12.commandList->ResourceBarrier(1, &barrierDesc);

		//���߂̃N���[�Y
		result = DX12.commandList->Close();
		assert(SUCCEEDED(result));

		//�R�}���h���X�g�̎��s
		ID3D12CommandList* commandLists[] = { DX12.commandList.Get() };
		DX12.commandQueue->ExecuteCommandLists(1, commandLists);

		//��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
		result = DX12.swapChain->Present(1, 0);
		assert(SUCCEEDED(result));

		//�R�}���h�̎��s������҂�
		DX12.commandQueue->Signal(DX12.fence.Get(), ++DX12.fenceVal);
		if (DX12.fence->GetCompletedValue() != DX12.fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			DX12.fence->SetEventOnCompletion(DX12.fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		//�L���[���N���A
		result = DX12.commandAllocator->Reset();
		assert(SUCCEEDED(result));

		//�ĂуR�}���h���X�g�𒙂߂鏀��
		result = DX12.commandList->Reset(DX12.commandAllocator.Get(), nullptr);
		assert(SUCCEEDED(result));

#pragma endregion ��ʓ���ւ�

	}

	gameScene_.End();

	//�E�B���h�E�N���X��o�^����
	UnregisterClass(winApi_.w.lpszClassName, winApi_.w.hInstance);

	return 0;
}

PipelineSet CreateObject3DPipeline()
{
	//�p�C�v���C���\����
	PipelineSet pipelineSet;

	//�V�F�[�_�[
	Shader shader_;

	shader_.vsBlob = shader_.Compile(L"Shader\\BasicVS.hlsl", "vs_5_0", shader_.vsBlob.Get(), "main");
	shader_.psBlob = shader_.Compile(L"Shader\\BasicPS.hlsl", "ps_5_0", shader_.psBlob.Get(), "main");

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//�V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = shader_.vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = shader_.vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = shader_.psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = shader_.psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�

	//���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;	//�J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//�|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true;			//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBA���ׂẴ`�����l����`��

	blenddesc.BlendEnable = true;					//�u�����h��L���ɂ���

	//����ӂ��`�`�`�`
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ZERO;		//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��

	int blendMode = ALPHA;

	if (blendMode == ADD)
	{
		//���Z����
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode == SUB)
	{
		//���Z����
		blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
		blenddesc.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode == INV)
	{
		//�F���]
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
		blenddesc.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�
	}
	if (blendMode == ALPHA)
	{
		//����������
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//�\�[�X�̃A���t�@�l
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	}

	//���_���C�A�E�g
	static D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ //xyz���W
				"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{//�@���x�N�g��
				"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{ //uv���W
				"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
	};

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1;	//�`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0�`255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1;	//1�s�N�Z���ɂ�1��T���v�����O

	pipelineDesc.DepthStencilState.DepthEnable = true;	//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	//��������΋���
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;	//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//�萔�o�b�t�@0�� b0
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0�� t0
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;					//�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@1�� b1
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����

	//�e�N�X�`���T���v���[�̐��� s0
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;				//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);	//���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;		//�T���v���[�̐擪�A�h���X	�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���
	rootSignatureDesc.NumStaticSamplers = 1;				//�T���v���[�̐�			�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���

	//���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &shader_.errorBlob);
	assert(SUCCEEDED(result));
	result = DX12.device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = pipelineSet.rootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	//ComPtr<ID3D12PipelineState> pipelineState;
	result = DX12.device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelinestate));
	assert(SUCCEEDED(result));

	return pipelineSet;
}

PipelineSet CreateSpritePipeline()
{
	PipelineSet spritePipelineSet;

	//�V�F�[�_�[
	Shader shader2_;

	shader2_.vsBlob = shader2_.Compile(L"SpriteVS.hlsl", "vs_5_0", shader2_.vsBlob.Get(), "main");
	shader2_.psBlob = shader2_.Compile(L"SpritePS.hlsl", "ps_5_0", shader2_.psBlob.Get(), "main");

	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc2{};

	//�V�F�[�_�[�̐ݒ�
	pipelineDesc2.VS.pShaderBytecode = shader2_.vsBlob->GetBufferPointer();
	pipelineDesc2.VS.BytecodeLength = shader2_.vsBlob->GetBufferSize();
	pipelineDesc2.PS.pShaderBytecode = shader2_.psBlob->GetBufferPointer();
	pipelineDesc2.PS.BytecodeLength = shader2_.psBlob->GetBufferSize();

	//�T���v���}�X�N�̐ݒ�
	pipelineDesc2.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�

	//���X�^���C�U�̐ݒ�
	pipelineDesc2.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;	//�w�ʃJ�����O�����Ȃ�
	pipelineDesc2.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	//�|���S�����h��Ԃ�
	pipelineDesc2.RasterizerState.DepthClipEnable = true;			//�[�x�N���b�s���O��L����

	//�u�����h�X�e�[�g
	pipelineDesc2.BlendState.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;	//RGBA�S�Ẵ`�����l����`��

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc2 = pipelineDesc2.BlendState.RenderTarget[0];
	blenddesc2.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; //RGBA���ׂẴ`�����l����`��

	blenddesc2.BlendEnable = true;					//�u�����h��L���ɂ���

	//����ӂ��`�`�`�`
	blenddesc2.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc2.SrcBlendAlpha = D3D12_BLEND_ZERO;		//�\�[�X�̒l��100%�g��
	blenddesc2.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��  0%�g��


	int blendMode2 = ALPHA;

	if (blendMode2 == ADD)
	{
		//���Z����
		blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc2.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc2.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode2 == SUB)
	{
		//���Z����
		blenddesc2.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
		blenddesc2.SrcBlend = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc2.DestBlend = D3D12_BLEND_ONE;			//�f�X�g�̒l��100%�g��
	}
	if (blendMode2 == INV)
	{
		//�F���]
		blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc2.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;//1.0f-�f�X�g�J���[�̒l
		blenddesc2.DestBlend = D3D12_BLEND_ZERO;			//�g��Ȃ�
	}
	if (blendMode2 == ALPHA)
	{
		//����������
		blenddesc2.BlendOp = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc2.SrcBlend = D3D12_BLEND_SRC_ALPHA;		//�\�[�X�̃A���t�@�l
		blenddesc2.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;//1.0f-�\�[�X�̃A���t�@�l
	}

	//���_���C�A�E�g
	static D3D12_INPUT_ELEMENT_DESC inputLayout2[] = {
			{ //xyz���W
				"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
			{ //uv���W
				"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
			},
	};

	//���_���C�A�E�g�̐ݒ�
	pipelineDesc2.InputLayout.pInputElementDescs = inputLayout2;
	pipelineDesc2.InputLayout.NumElements = _countof(inputLayout2);

	//�}�`�̌`��ݒ�
	pipelineDesc2.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	pipelineDesc2.NumRenderTargets = 1;	//�`��Ώۂ�1��
	pipelineDesc2.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	//0�`255�w���RGBA
	pipelineDesc2.SampleDesc.Count = 1;	//1�s�N�Z���ɂ�1��T���v�����O

	//�[�x�X�e�[�g�̐ݒ�
	pipelineDesc2.DepthStencilState.DepthEnable = false;	//�[�x�e�X�g���s��Ȃ�
	pipelineDesc2.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
	pipelineDesc2.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;	//��ɏ㏑��
	pipelineDesc2.DSVFormat = DXGI_FORMAT_D32_FLOAT;	//�[�x�l�t�H�[�}�b�g

	//�f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange2{};
	descriptorRange2.NumDescriptors = 1;	//��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange2.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange2.BaseShaderRegister = 0;	//�e�N�X�`�����W�X�^0��
	descriptorRange2.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams2[3] = {};
	//�萔�o�b�t�@0�� b0
	rootParams2[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams2[0].Descriptor.ShaderRegister = 0;					//�萔�o�b�t�@�ԍ�
	rootParams2[0].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams2[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�e�N�X�`�����W�X�^0�� t0
	rootParams2[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	//���
	rootParams2[1].DescriptorTable.pDescriptorRanges = &descriptorRange2;					//�f�X�N���v�^�����W
	rootParams2[1].DescriptorTable.NumDescriptorRanges = 1;						//�f�X�N���v�^�����W��
	rootParams2[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//�S�ẴV�F�[�_���猩����
	//�萔�o�b�t�@1�� b1
	rootParams2[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	//���
	rootParams2[2].Descriptor.ShaderRegister = 1;					//�萔�o�b�t�@�ԍ�
	rootParams2[2].Descriptor.RegisterSpace = 0;						//�f�t�H���g�l
	rootParams2[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	//���ׂẴV�F�[�_���猩����

	//�e�N�X�`���T���v���[�̐��� s0
	D3D12_STATIC_SAMPLER_DESC samplerDesc2{};
	samplerDesc2.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc2.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc2.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc2.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc2.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc2.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc2.MinLOD = 0.0f;
	samplerDesc2.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc2.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//���[�g�V�O�l�`��
	//ComPtr<ID3D12RootSignature> rootSignature;

	//���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc2{};
	rootSignatureDesc2.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc2.pParameters = rootParams2;				//���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc2.NumParameters = _countof(rootParams2);	//���[�g�p�����[�^��
	rootSignatureDesc2.pStaticSamplers = &samplerDesc2;		//�T���v���[�̐擪�A�h���X	�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���
	rootSignatureDesc2.NumStaticSamplers = 1;				//�T���v���[�̐�			�z��Ȃ烋�[�g�p�����[�^�Ɠ����悤�ȏ��������ł���

	//���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob2;
	result = D3D12SerializeRootSignature(&rootSignatureDesc2, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob2, &shader2_.errorBlob);
	assert(SUCCEEDED(result));
	result = DX12.device->CreateRootSignature(0, rootSigBlob2->GetBufferPointer(), rootSigBlob2->GetBufferSize(),
		IID_PPV_ARGS(&spritePipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc2.pRootSignature = spritePipelineSet.rootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	//ComPtr<ID3D12PipelineState> pipelineState;
	result = DX12.device->CreateGraphicsPipelineState(&pipelineDesc2, IID_PPV_ARGS(&spritePipelineSet.pipelinestate));
	assert(SUCCEEDED(result));

	return spritePipelineSet;
}