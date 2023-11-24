#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Float4.h"
#include "ConstBuffer.h"

/*! DirectionalLight
	ディレクショナルライト
*/
class DirectionalLight
{
public://変数
	Vector3 mDirection = { 1,0,0 };
	Vector3 mColor = { 1,1,1 };

	struct ConstBufferData
	{
		Vector3 direction;
		bool active;
		Vector3 color;
		float pad;
	};

public://関数

	void Initialize();
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="rootParameterIndex">使用するルートパラメータのインデックス</param>
	void Draw(const UINT& rootParameterIndex);

	//バッファに値を転送
	void TransferBuffer();

	//ライトの方向を設定
	void SetLightDirection(const Vector3& lightdir);

	//ライトの色を設定
	void SetLightColor(const Vector3& color);

	bool mActive = false;

private://変数
	//定数バッファ
	ConstBuffer<ConstBufferData> mConstBuff;

	bool mDirty = false;
};