#include "PostEffectt.h"

PostEffect::PostEffect() 
	: Sprite(
	)
{
	mTEXTURE = TextureManager::GetTexture("white");
	mPosition = { 0,0,0};
	mSize = { 500.f,500.f };
	mColor = { 1,1,1,1 };
	mAnchorpoint = { 0.f,0.f };

	Init();
}

void PostEffect::Draw()
{
	DirectX12* dx12 = DirectX12::Get();
	TextureManager* texM = TextureManager::Get();

	if (mIsInvisible)
	{
		return;
	}

	Sprite::Update();

	//パイプラインステートの設定
	dx12->mCmdList->SetPipelineState(SpriteCommon::sSpriteCommon.mPipelineSet.mPipelinestate.Get());
	//ルートシグネチャの設定
	dx12->mCmdList->SetGraphicsRootSignature(SpriteCommon::sSpriteCommon.mPipelineSet.mRootsignature.Get());
	//プリミティブ形状を設定
	dx12->mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	dx12->mCmdList->SetDescriptorHeaps(1, texM->mSrvHeap.GetAddressOf());

	dx12->mCmdList->IASetVertexBuffers(0, 1, &mVbView);

	dx12->mCmdList->SetGraphicsRootDescriptorTable(1, mTEXTURE->mGpuHandle);

	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, mConstBuffer.mBuffer->GetGPUVirtualAddress());

	dx12->mCmdList->DrawInstanced(4, 1, 0, 0);
}
