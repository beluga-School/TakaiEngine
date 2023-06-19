#include "ClearDrawScreen.h"
#include "DirectXInit.h"
#include "Result.h"
#include "Util.h"
#include "Texture.h"
#include "LightGroup.h"
#include "Obj.h"

void CreateDepthView()
{
	DirectX12* dx12 = DirectX12::Get();
	Screen* screen = Screen::Get();

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = Util::WIN_WIDTH;	//�����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = Util::WIN_HEIGHT;	//�����_�[�^�[�Q�b�g�ɍ��킹��
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
	sResult = dx12->mDevice->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,	//�[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&screen->depthBuff));

	//�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;	//�[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;	//�f�v�X�X�e���V���r���[
	
	sResult = dx12->mDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&screen->dsvHeap));

	assert(SUCCEEDED(sResult));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx12->mDevice->CreateDepthStencilView(
		screen->depthBuff.Get(),
		&dsvDesc,
		screen->dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void ClearDrawScreen()
{
	DirectX12* dx12 = DirectX12::Get();
	Screen* screen = Screen::Get();

	FLOAT clearColor[] = { 0.1f,0.25f,0.5f,0.0f };

	UINT bbIndex = dx12->mSwapChain->GetCurrentBackBufferIndex();

	//1.���\�[�X�o���A�ŏ������݉��ɕύX
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = dx12->mBackBuffers[bbIndex].Get();	//�o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;	//�\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;		//�`���Ԃ�
	dx12->mCmdList->ResourceBarrier(1, &barrierDesc);
	
	//2.�`���̕ύX
	//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dx12->mRtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * dx12->mDevice->GetDescriptorHandleIncrementSize(dx12->mRtvHeapDesc.Type);

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = screen->dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dx12->mCmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	//3.��ʃN���A
	dx12->mCmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	dx12->mCmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PreDraw()
{
	DirectX12* dx12 = DirectX12::Get();

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::WIN_WIDTH;
	viewport.Height = Util::WIN_HEIGHT;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	dx12->mCmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									//�؂蔲�����W��
	scissorRect.right = scissorRect.left + Util::WIN_WIDTH;	//�؂蔲�����W�E
	scissorRect.top = 0;									//�؂蔲�����W��
	scissorRect.bottom = scissorRect.top + Util::WIN_HEIGHT;	//�؂蔲�����W��
	//�V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	dx12->mCmdList->RSSetScissorRects(1, &scissorRect);


}

void BasicObjectPreDraw(const PipelineSet& objectPipelineSet)
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//�X�v���C�g����Ȃ���
	dx12->mCmdList->SetPipelineState(objectPipelineSet.mPipelinestate.Get());
	dx12->mCmdList->SetGraphicsRootSignature(objectPipelineSet.mRootsignature.Get());

	//�v���~�e�B�u�`��̐ݒ�R�}���h
	dx12->mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//SRV�q�[�v�̐ݒ�R�}���h
	dx12->mCmdList->SetDescriptorHeaps(1, texM->mSrvHeap.GetAddressOf());

	LightGroup::sLightGroup->Draw(4);
}

void GeometryObjectPreDraw(const PipelineSet& geometryPipelineSet)
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	D3D12_VIEWPORT viewport{};
	viewport.Width = Util::WIN_WIDTH;
	viewport.Height = Util::WIN_HEIGHT;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	dx12->mCmdList->RSSetViewports(1, &viewport);

	D3D12_RECT scissorRect{};
	scissorRect.left = 0;									//�؂蔲�����W��
	scissorRect.right = scissorRect.left + Util::WIN_WIDTH;	//�؂蔲�����W�E
	scissorRect.top = 0;									//�؂蔲�����W��
	scissorRect.bottom = scissorRect.top + Util::WIN_HEIGHT;	//�؂蔲�����W��
	//�V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
	dx12->mCmdList->RSSetScissorRects(1, &scissorRect);

	//�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//�X�v���C�g����Ȃ���
	dx12->mCmdList->SetPipelineState(geometryPipelineSet.mPipelinestate.Get());
	dx12->mCmdList->SetGraphicsRootSignature(geometryPipelineSet.mRootsignature.Get());

	dx12->mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	//SRV�q�[�v�̐ݒ�R�}���h
	dx12->mCmdList->SetDescriptorHeaps(1, texM->mSrvHeap.GetAddressOf());
}

void PostDraw()
{
	DirectX12* dx12 = DirectX12::Get();

	UINT bbIndex = dx12->mSwapChain->GetCurrentBackBufferIndex();

	//5.���\�[�X�o���A��߂�
	D3D12_RESOURCE_BARRIER barrierDesc{};
	barrierDesc.Transition.pResource = dx12->mBackBuffers[bbIndex].Get();	//�o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	//�`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;	//�\����Ԃ�
	dx12->mCmdList->ResourceBarrier(1, &barrierDesc);

	//���߂̃N���[�Y
	sResult = dx12->mCmdList->Close();
	assert(SUCCEEDED(sResult));

	//�R�}���h���X�g�̎��s
	ID3D12CommandList* commandLists[] = { dx12->mCmdList.Get() };
	dx12->mCmdQueue->ExecuteCommandLists(1, commandLists);

	//��ʂɕ\������o�b�t�@���t���b�v(���\�̓���ւ�)
	sResult = dx12->mSwapChain->Present(1, 0);
	assert(SUCCEEDED(sResult));

	//�R�}���h�̎��s������҂�
	dx12->mCmdQueue->Signal(dx12->mFence.Get(), ++dx12->mFenceVal);
	if (dx12->mFence->GetCompletedValue() != dx12->mFenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		dx12->mFence->SetEventOnCompletion(dx12->mFenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	//�L���[���N���A
	sResult = dx12->mCommandAllocator->Reset();
	assert(SUCCEEDED(sResult));

	//�ĂуR�}���h���X�g�𒙂߂鏀��
	sResult = dx12->mCmdList->Reset(dx12->mCommandAllocator.Get(), nullptr);
	assert(SUCCEEDED(sResult));
}
