#pragma once
#include <DirectXMath.h>
#pragma warning (push)
#pragma warning( disable : 26813 )
#include <DirectXTex.h>
#pragma warning (pop)

#include "DirectXInit.h"
#include <map>

const size_t K_MAX_SRV_COUNT = 2056;

class Texture
{
public:

	void Load(const wchar_t& t);

	void CreateWhiteTexture();

	//外部から参照する用のリソース設定(書き換えてもテクスチャ側には影響しない)
	D3D12_RESOURCE_DESC mGetResDesc;

	Microsoft::WRL::ComPtr<ID3D12Resource> mTexBuff;
	
	D3D12_GPU_DESCRIPTOR_HANDLE mGpuHandle = { 0 };

	D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle = { 0 };

private:
	DirectX::TexMetadata mMetadata{};
	DirectX::ScratchImage mScratchImg{};
	DirectX::ScratchImage mMipChain{};

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc{};//設定構造体
	//リソース設定
	D3D12_RESOURCE_DESC mResDesc{};

};

class TextureManager
{
public:
	static TextureManager* Get() {
		static TextureManager instance;
		return &instance;
	};

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC mSrvHeapDesc = {};

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap;

	//SRVヒープの大きさ
	UINT mSRVHandleSize = 0;

	void Initialize();
	void PreLoad();

	static void Load(const std::string &filepath,const std::string &handle);
	
	static Texture* GetTexture(const std::string &handle);


private:
	//テクスチャの1枚目が、デバッグテキストの物になっちゃってるバグがあるのでそれを入れる用のテクスチャ
	Texture bugfix_;

	static std::map<std::string, Texture> sTextures;

	TextureManager(){};
	~TextureManager(){};
	TextureManager(const TextureManager& a) = delete;
	TextureManager operator=(const TextureManager&) = delete;

};