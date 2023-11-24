#pragma once
#include <DirectXMath.h>
#pragma warning (push)
#pragma warning( disable : 26813 )
#pragma warning( disable : 5267 )
#include <DirectXTex.h>
#pragma warning (pop)

#include "DirectXInit.h"
#include <map>

const size_t K_MAX_SRV_COUNT = 2056;

/*! Texture
	テクスチャ単体のクラス
*/
class Texture
{
public:

	//リソースの読み込み
	void Load(const wchar_t& t);

	//白テクスチャ生成
	void CreateWhiteTexture();

	//テクスチャのメタデータ取得
	DirectX::TexMetadata* GetMetaData();

public:
	//外部から参照する用のリソース設定(書き換えてもテクスチャ側には影響しない)
	D3D12_RESOURCE_DESC mGetResDesc{};

	Microsoft::WRL::ComPtr<ID3D12Resource> mTexBuff;

	D3D12_GPU_DESCRIPTOR_HANDLE mGpuHandle = { 0 };

	D3D12_CPU_DESCRIPTOR_HANDLE mCpuHandle = { 0 };

	bool mCreated = false;

private:
	DirectX::TexMetadata mMetadata{};
	DirectX::ScratchImage mScratchImg{};
	DirectX::ScratchImage mMipChain{};

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC mSrvDesc{};//設定構造体
	//リソース設定
	D3D12_RESOURCE_DESC mResDesc{};

};

/*! TextureManager
	テクスチャ管理クラス
	Singletonパターン
*/
class TextureManager
{
public:
	//インスタンス取得
	static TextureManager* Get() {
		static TextureManager instance;
		return &instance;
	};

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC mSrvHeapDesc = {};

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mSrvHeap;

	//SRVヒープの大きさ
	UINT mSRVHandleSize = 0;

	//初期化
	void Initialize();
	
	//事前読み込み
	void PreLoad();

	/// <summary>
	/// ファイルパスを指定してリソースの読み込み
	/// </summary>
	/// <param name="filepath">リソースのファイルパス</param>
	/// <param name="handle">リソース取得時に使用するハンドル</param>
	static void Load(const std::string &filepath,const std::string &handle);
	
	/// <summary>
	/// ハンドルを使用したテクスチャ取得
	/// </summary>
	/// <param name="handle">読み込み時のリソースハンドル</param>
	/// <returns></returns>
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