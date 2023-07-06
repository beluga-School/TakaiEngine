#include "LineObject.h"

void LineObject::Initialize()
{
	MODEL = ModelManager::GetModel("LineSphere");
}

void LineObject::Update(const Camera& camera)
{
	Matrix4 matScale;	//スケーリング行列
	Matrix4 matRot;	//回転行列
	Matrix4 matTrans;	//平行移動行列

	//スケールを設定
	matScale = Matrix4::Identity();
	matScale = Matrix4::scale(scale);

	//回転を設定
	matRot = Matrix4::Identity();
	matRot = Matrix4::rotateZ(rotation.z) * Matrix4::rotateX(rotation.x) * Matrix4::rotateY(rotation.y);

	matTrans = Matrix4::Identity();
	matTrans = Matrix4::translate(position);

	matWorld = Matrix4::Identity();
	matWorld = matScale * matRot * matTrans;

	if (parent != nullptr)
	{
		matWorld = matWorld * parent->matWorld;
	}

	constBufferLine.mConstBufferData->world = matWorld;
	constBufferLine.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	constBufferLine.mConstBufferData->color = {
		color_.x,color_.y,color_.z,color_.w
	};
}

void LineObject::Draw()
{
	//見えないフラグが立ってるなら描画を行わない
	if (mIsVisiable == false)
	{
		return;
	}

	DirectX12* dx12 = DirectX12::Get();

	//頂点バッファの設定
	dx12->mCmdList->IASetVertexBuffers(0, 1, &MODEL->mVbView);

	//インデックスバッファの設定
	dx12->mCmdList->IASetIndexBuffer(&MODEL->mIbView);

	//定数バッファビュー(CBV)の設定コマンド
	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, constBufferLine.mBuffer->GetGPUVirtualAddress());

	//描画コマンド
	dx12->mCmdList->DrawIndexedInstanced((UINT)MODEL->mMesh.indices.size(), 1, 0, 0, 0);
}
