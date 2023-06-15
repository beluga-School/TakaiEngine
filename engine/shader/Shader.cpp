#include "Shader.h"
#include "Result.h"
#include "DirectXInit.h"

ID3DBlob* Shader::Compile(const wchar_t *shaderFilename, 
	const LPCSTR& shadermodelname,ID3DBlob *blob , 
	const LPCSTR& entrypointname)
{
	//シェーダの読み込みとコンパイル
	sResult = D3DCompileFromFile(
		shaderFilename,	//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//デバッグ用設定
		entrypointname, shadermodelname,	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバッグ用設定
		0,
		&blob, &mErrorBlob);

	//エラーなら
	if (FAILED(sResult)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(mErrorBlob->GetBufferSize());

		std::copy_n((char*)mErrorBlob->GetBufferPointer(),
			mErrorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	return blob;
}