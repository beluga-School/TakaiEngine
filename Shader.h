#pragma once

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <wrl.h>

class Shader
{
public:
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;		//頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;		//ピクセルシェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob;		//ジオメトリシェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;		//エラーオブジェクト

	ID3DBlob* Compile(const wchar_t* shaderFilename, LPCSTR shadermodelname, ID3DBlob* blob, LPCSTR entrypointname);
};

