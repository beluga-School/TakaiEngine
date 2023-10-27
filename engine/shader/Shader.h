#pragma once
#include <d3dcompiler.h>
#include <wrl.h>

/*! Shader
	シェーダーコンパイルに必要なものを持ち、
	コンパイルの機能も持つ
*/
class Shader
{
public:
	Microsoft::WRL::ComPtr<ID3DBlob> mVsBlob;		//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> mPsBlob;		//ピクセルシェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> mGsBlob;		//ジオメトリシェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob;		//エラーオブジェクト

	ID3DBlob* Compile(const wchar_t* shaderFilename, const LPCSTR& shadermodelname, ID3DBlob* blob,const LPCSTR& entrypointname);
};

