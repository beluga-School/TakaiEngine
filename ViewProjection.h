#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "DirectXInit.h"

class View
{
public:
	XMMATRIX matView;
	XMFLOAT3 eye;	//���_���W
	XMFLOAT3 target;	//�����_���W
	XMFLOAT3 up;		//������x�N�g��

public:
	void Initialize();
	void UpdatematView();
};

