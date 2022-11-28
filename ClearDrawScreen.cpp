#include "ClearDrawScreen.h"
#include "DirectXInit.h"
#include "Result.h"
#include "Util.h"
#include "Texture.h"

ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;

ComPtr<ID3D12Resource> depthBuff;

D3D12_RESOURCE_BARRIER barrierDesc{};

void CreateDepthView()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Util::window_width;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = Util::window_height;	//�����_�[�^�[�Q�b�g�ɍ��킹��
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
	
	result = dx12->device->CreateCommittedResource(
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
	
	result = dx12->device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	assert(SUCCEEDED(result));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx12->device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void ClearDrawScreen()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };

	UINT bbIndex = dx12->swapChain->GetCurrentBackBufferIndex();

	//1.���\�[�X�o���A�ŏ������݉��ɕύX
	//D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = dx12->backBuffers[bbIndex].Get();	//�o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	//�\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		//�`���Ԃ�
	dx12->commandList->ResourceBarrier(1, &barrierDesc);
	
	//2.�`���̕ύX
	//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx12->rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dx12->device->GetDescriptorHandleIncrementSize(dx12->rtvHeapDesc.Type);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dx12->commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//3.��ʃN���A
	dx12->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	dx12->commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void BasicObjectPreDraw(PipelineSet objectPipelineSet)
{
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::window_width;
	viewport.Height = Util::window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	dx12->commandList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									//�؂蔲�����W��
	scissorRect.right = scissorRect.left + Util::window_width;	//�؂蔲�����W�E
	scissorRect.top = 0;									//�؂蔲�����W��
	scissorRect.bottom = scissorRect.top + Util::window_height;	//�؂蔲�����W��
	//�V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	dx12->commandList->RSSetScissorRects(1, &scissorRect);

	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//�X�v���C�g����Ȃ���
	dx12->commandList->SetPipelineState(objectPipelineSet.pipelinestate.Get());
	dx12->commandList->SetGraphicsRootSignature(objectPipelineSet.rootsignature.Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//SRV�q�[�v�̐ݒ�R�}���h
	dx12->commandList->SetDescriptorHeaps(1, texM->srvHeap.GetAddressOf());
}

void GeometryObjectPreDraw(PipelineSet geometryPipelineSet)
{
	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::window_width;
	viewport.Height = Util::window_height;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	dx12->commandList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									//�؂蔲�����W��
	scissorRect.right = scissorRect.left + Util::window_width;	//�؂蔲�����W�E
	scissorRect.top = 0;									//�؂蔲�����W��
	scissorRect.bottom = scissorRect.top + Util::window_height;	//�؂蔲�����W��
	//�V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	dx12->commandList->RSSetScissorRects(1, &scissorRect);

	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//�X�v���C�g����Ȃ���
	dx12->commandList->SetPipelineState(geometryPipelineSet.pipelinestate.Get());
	dx12->commandList->SetGraphicsRootSignature(geometryPipelineSet.rootsignature.Get());

	dx12->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//SRV�q�[�v�̐ݒ�R�}���h
	dx12->commandList->SetDescriptorHeaps(1, texM->srvHeap.GetAddressOf());
}

void PostDraw()
{
	DirectX12* dx12 = DirectX12::GetInstance();

	//5.���\�[�X�o���A��߂�
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;	//�\����Ԃ�
	dx12->commandList->ResourceBarrier(1, &barrierDesc);

	//���߂̃N���[�Y
	result = dx12->commandList->Close();
	assert(SUCCEEDED(result));

	//�R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = { dx12->commandList.Get() };
	dx12->commandQueue->ExecuteCommandLists(1, commandLists);

	//��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
	result = dx12->swapChain->Present(1, 0);
	assert(SUCCEEDED(result));

	//�R�}���h�̎��s������҂�
	dx12->commandQueue->Signal(dx12->fence.Get(), ++dx12->fenceVal);
	if (dx12->fence->GetCompletedValue() != dx12->fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		dx12->fence->SetEventOnCompletion(dx12->fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//�L���[���N���A
	result = dx12->commandAllocator->Reset();
	assert(SUCCEEDED(result));

	//�ĂуR�}���h���X�g�𒙂߂鏀��
	result = dx12->commandList->Reset(dx12->commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));
}
