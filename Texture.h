#pragma once
#include "DirectXInit.h"

class Texture
{
public:
	void Load(const char t);
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle();
};

