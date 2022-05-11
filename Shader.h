#pragma once

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

class Shader
{
public:
	ID3DBlob* vsBlob = nullptr;		//頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr;		//ピクセルシェーダーオブジェクト
	ID3DBlob* errorBlob = nullptr;	//エラーオブジェクト

	ID3DBlob* Compile(const wchar_t* shaderFilename, LPCSTR shadermodelname, ID3DBlob* blob, LPCSTR entrypointname);
};

