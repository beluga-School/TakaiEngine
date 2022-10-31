#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ	　ルートパラメータ
SamplerState smp : register(s0);	//0番スロットに設定されたサンプラー

//float4 main(VSOutput input) : SV_TARGET
float4 main(GSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp,input.uv));
	float3 light = normalize(float3(1,-1,1));		//右下奥　向きのライト
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;	//光源へのベクトルとベクトルの内積
	return float4(texcolor.rgb * brightness, texcolor.a) * color;//輝度をRGBに代入して出力

	//return float4(input.normal,1);	//RGBをそれぞれ法線のXYZ、Aを1で出力
	//return float4(input.uv,0,1);
	//return float4(1,1,1,1);
}