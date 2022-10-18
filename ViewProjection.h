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
	XMFLOAT3 eye;	//視点座標
	XMFLOAT3 target;	//注視点座標
	XMFLOAT3 up;		//上方向ベクトル

public:
	void Initialize();
	void UpdatematView();
};

