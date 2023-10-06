#pragma once
#include <d3dcompiler.h>
#include <wrl.h>

class Shader
{
public:
	Microsoft::WRL::ComPtr<ID3DBlob> mVsBlob;		//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> mPsBlob;		//ピクセルシェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> mGsBlob;		//ジオメトリシェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob;		//エラーオブジェクト

	ID3DBlob* Compile(const wchar_t* shaderFilename, const LPCSTR& shadermodelname, ID3DBlob* blob,const LPCSTR& entrypointname);
};

