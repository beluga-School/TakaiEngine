#include "ViewProjection.h"

void View::Initialize()
{
	eye = { 0, 0, -5 };	//���_���W
	target = {0, 0, 0};		//�����_���W
	up = { 0, 1, 0 };		//������x�N�g��
	UpdatematView();
}

void View::UpdatematView()
{
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}