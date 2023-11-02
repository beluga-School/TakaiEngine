#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "DirectXInit.h"
#include <map>

enum BLEND_MODE
{
	ADD,
	SUB,
	INV,
	ALPHA,
};

struct ShaderName
{
	std::string shaderName = "";
	enum class ShaderType
	{
		VS,
		PS,
		GS,
		ER,
	}shaderType = ShaderType::ER;
};

struct PipelineSet
{
	//パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPipelinestate{};
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> mRootsignature{};

	//--設定用変数群(この変数を事前に書き換えてからCreate()を呼び出す)
	//シェーダー名
	ShaderName vs = { "",ShaderName::ShaderType::VS };
	ShaderName ps = { "",ShaderName::ShaderType::PS };
	ShaderName gs = { "",ShaderName::ShaderType::GS };

	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//ブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc;

	//インプットレイアウト
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

	//デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange{};

	//ルートパラメータ
	std::vector<D3D12_ROOT_PARAMETER> rootParams;
	int32_t paramSize = 2;

	//サンプラー設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};

	//ルートシグネチャ設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};

	int32_t blendMode = ALPHA;

	//生成されたらデフォの設定が入るように
	PipelineSet();

	//設定を元にパイプラインを生成
	void Create();
};

/*! PipelineManager
	パイプラインを管理するクラス
	Initializeで先にパイプラインを登録し、文字列で使用するパイプラインを切り変える
*/
class PipelineManager
{
public:
	//文字列でパイプラインを取得
	static PipelineSet GetPipeLine(const std::string& name);

	static PipelineManager* Get() {
		static PipelineManager instance;
		return &instance;
	};

	//パイプラインを先に生成しておく
	void Initialize();

private:
	static std::map<std::string, PipelineSet> sPipelines;

	//3Dオブジェクト用パイプライン
	void Object3DPipeLine();

	//スプライト用パイプライン
	void SpritePipeLine();

	//ポストエフェクト掛けてないときのパイプライン
	void NonePostEffectPipeLine();

	//マルチレンダーパイプライン
	void MultiRenderPipeLine();

	//ガウシアンブラー用パイプライン
	void GaussianBlurPipeLine();

	//シフトブラー用パイプライン
	void ShiftBlurPipeLine();

	//phongシェーダー
	void PhongPipeLine();

	//単色シェーダー
	void SingleColorPipeLine();

	//トゥーンシェーダー
	void ToonPipeLine();
	
	//壁が透けるときに切り替える用のパイプライン/
	void ToonNoDepthWritePipeLine();

	//アウトラインシェーダー
	void OutLinePipeLine();

	//スカイドーム用(なんの影響も受けない)シェーダー
	void SkydomePipeLine();

	//地面用トゥーンシェーダー
	void GroundToonPipeLine();
	
	//壁が透けるときに切り替える用のパイプライン
	//void GroundToonNoDepthWritePipeLine();

	//ワイヤーフレーム用パイプライン
	void WireFramePipeLine();

	//パーリンノイズ実装時に使ったパイプライン(まだ実践向けじゃない	)
	void PerlinNoisePipeLine();

	//テクスチャブレンド用パイプライン
	void TextureBlendPipeLine();

	//ディゾルブ用パイプライン
	void DisolvePipeLine();

	//パーリンノイズをポストエフェクトとして使うパイプライン
	void PerlinNoisePostEffectPipeLine();

	//ライン描画パイプライン
	void LinePipeLine();

	//ディザ抜きパイプライン
	void DitherTransparentPipeLine();

	PipelineManager(){};
	~PipelineManager(){};
};

PipelineSet CreateLambertPipeline();