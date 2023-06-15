#pragma once

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <wrl.h>

class Shader
{
public:
	Microsoft::WRL::ComPtr<ID3DBlob> mVsBlob;		//���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> mPsBlob;		//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> mGsBlob;		//�W�I���g���V�F�[�_�[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob;		//�G���[�I�u�W�F�N�g

	ID3DBlob* Compile(const wchar_t* shaderFilename, const LPCSTR& shadermodelname, ID3DBlob* blob,const LPCSTR& entrypointname);
};

