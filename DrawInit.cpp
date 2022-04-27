#include "DrawInit.h"

DrawInit::DrawInit()
{
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用
}

void DrawInit::SetResDesc()
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;	//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}