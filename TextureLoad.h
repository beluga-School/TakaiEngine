#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include <DirectXTex.h>

//自分でクラス化したやつ
#include "WinAPI.h"
#include "Result.h"
#include "Input.h"
#include "DirectXInit.h"
#include "Vertex.h"
#include "Shader.h"
#include "TextureLoad.h"

//自分でクラス化したやつ
#include "DirectXInit.h"

class TextureLoad
{
public:
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	void WIC(const wchar_t* file_pass);
};

