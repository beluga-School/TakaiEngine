#include "Shader.h"
#include "Result.h"
#include "DirectXInit.h"

ID3DBlob* Shader::Compile(const wchar_t *shaderFilename, 
	const LPCSTR& shadermodelname,ID3DBlob *blob , 
	const LPCSTR& entrypointname)
{
	//�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	sResult = D3DCompileFromFile(
		shaderFilename,	//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//�f�o�b�O�p�ݒ�
		entrypointname, shadermodelname,	//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�O�p�ݒ�
		0,
		&blob, &mErrorBlob);

	//�G���[�Ȃ�
	if (FAILED(sResult)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(mErrorBlob->GetBufferSize());

		std::copy_n((char*)mErrorBlob->GetBufferPointer(),
			mErrorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	return blob;
}