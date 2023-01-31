#pragma once

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <wrl.h>

class Shader
{
public:
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;		//���_�V�F�[�_�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;		//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> gsBlob;		//�W�I���g���V�F�[�_�[�I�u�W�F�N�g
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;		//�G���[�I�u�W�F�N�g

	ID3DBlob* Compile(const wchar_t* shaderFilename, LPCSTR shadermodelname, ID3DBlob* blob, LPCSTR entrypointname);
};

