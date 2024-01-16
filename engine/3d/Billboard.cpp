#include "Billboard.h"
#include "DirectXInit.h"
#include "PlayerCamera.h"

Billboard::Billboard()
{
	Initialize();
}

void Billboard::Initialize()
{
	SetModel(ModelManager::GetModel("plate"));
	SetTexture(TextureManager::GetTexture("default"));
}

void Billboard::Update(const Camera& camera)
{
	Matrix4 MatBillboard = Matrix4::Identity();
	
	Matrix4 matScale = Matrix4::Identity();;	//スケーリング行列
	Matrix4 matRot = Matrix4::Identity();	//回転行列
	Matrix4 matTrans = Matrix4::Identity();	//平行移動行列

	//スケールを設定
	matScale = Matrix4::scale(scale);

	//回転を設定
	matRot = Matrix4::rotateZ(rotation.z) * Matrix4::rotateX(rotation.x) * Matrix4::rotateY(rotation.y);

	//位置を設定
	matTrans = Matrix4::translate(position);

	//ビルボード行列を作成
	MatBillboard = 
		Matrix4::rotateZ(PlayerCamera::Get()->rotation.z) *
		Matrix4::rotateX(PlayerCamera::Get()->rotation.x) * 
		Matrix4::rotateY(PlayerCamera::Get()->rotation.y);

	matWorld = Matrix4::Identity();
	matWorld *= matScale;
	matWorld *= MatBillboard;//ビルボード回転
	matWorld *= matRot;
	matWorld *= matTrans;

	constBufferT.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	constBufferT.mConstBufferData->world = matWorld;
	constBufferT.mConstBufferData->cameraPos = camera.mEye;
	constBufferT.mConstBufferData->tiling = mTiling;

	if (MODEL != nullptr)
	{
		constBufferB1.mConstBufferData->ambient = MODEL->mMaterial.mAmbient;
		constBufferB1.mConstBufferData->diffuse = MODEL->mMaterial.mDiffuse;
		constBufferB1.mConstBufferData->specular = MODEL->mMaterial.mSpecular;
		constBufferB1.mConstBufferData->alpha = MODEL->mMaterial.mAlpha;
		constBufferB.mConstBufferData->brightness = color_;
	}

	constBufferOutLine.mConstBufferData->color = mOutLineColor;
	constBufferOutLine.mConstBufferData->thickness = mOutLineThickness;
}

BillboardY::BillboardY()
{
	Initialize();
}

void BillboardY::Initialize()
{
	SetModel(ModelManager::GetModel("plate"));
	SetTexture(TextureManager::GetTexture("default"));
}

void BillboardY::Update(const Camera& camera)
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

	//ビルボード行列を作成
	Matrix4 MatBillboardY = Matrix4::Identity();
	Vector3 camVecX = PlayerCamera::Get()->matWorld.ExtractAxisX();
	MatBillboardY.m[0][0] = camVecX.x;
	MatBillboardY.m[0][1] = camVecX.y;
	MatBillboardY.m[0][2] = camVecX.z;
	
	Vector3 upVec = { 0,1,0 };
	MatBillboardY.m[1][0] = upVec.x;
	MatBillboardY.m[1][1] = upVec.y;
	MatBillboardY.m[1][2] = upVec.z;

	Vector3 camVecZ = PlayerCamera::Get()->matWorld.ExtractAxisX().GetCross(upVec);
	MatBillboardY.m[2][0] = camVecZ.x;
	MatBillboardY.m[2][1] = camVecZ.y;
	MatBillboardY.m[2][2] = camVecZ.z;

	matWorld = Matrix4::Identity();
	matWorld *= matScale;
	matWorld *= MatBillboardY;//ビルボード回転
	matWorld *= matRot;
	matWorld *= matTrans;

	constBufferT.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	constBufferT.mConstBufferData->world = matWorld;
	constBufferT.mConstBufferData->cameraPos = camera.mEye;
	constBufferT.mConstBufferData->tiling = mTiling;

	if (MODEL != nullptr)
	{
		constBufferB1.mConstBufferData->ambient = MODEL->mMaterial.mAmbient;
		constBufferB1.mConstBufferData->diffuse = MODEL->mMaterial.mDiffuse;
		constBufferB1.mConstBufferData->specular = MODEL->mMaterial.mSpecular;
		constBufferB1.mConstBufferData->alpha = MODEL->mMaterial.mAlpha;
		constBufferB.mConstBufferData->brightness = color_;
	}

	constBufferOutLine.mConstBufferData->color = mOutLineColor;
	constBufferOutLine.mConstBufferData->thickness = mOutLineThickness;

	//実は頂点シェーダーとかは共通化した方がいいんじゃないか
	cBufferNoise.mConstBufferData->viewproj = camera.mMatView * camera.mMatProjection;
	cBufferNoise.mConstBufferData->matWorld = matWorld;
}

void BillboardY::DrawNoise()
{
	//見えないフラグが立ってるなら描画を行わない
	if (mIsVisiable == false)
	{
		return;
	}

	DirectX12* dx12 = DirectX12::Get();
	
	//SRVヒープの先頭から順番にSRVをルートパラメータ1番に設定
	//ルートパラメータ1番はテクスチャバッファ
	//dx12->mCmdList->SetGraphicsRootDescriptorTable(1, TEXTURE->mGpuHandle);
	//commandList->SetGraphicsRootConstantBufferView(0, constBufferT.buffer->GetGPUVirtualAddress());

	//頂点バッファの設定
	dx12->mCmdList->IASetVertexBuffers(0, 1, &MODEL->mVbView);

	//インデックスバッファの設定
	dx12->mCmdList->IASetIndexBuffer(&MODEL->mIbView);

	//定数バッファビュー(CBV)の設定コマンド
	dx12->mCmdList->SetGraphicsRootConstantBufferView(0, constBufferB1.mBuffer->GetGPUVirtualAddress());
	
	//描画コマンド
	dx12->mCmdList->DrawIndexedInstanced((UINT)MODEL->mMesh.indices.size(), 1, 0, 0, 0);

}