#include "PerlinNoisePE.hlsli"

Texture2D<float4> tex0 : register(t0); //0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

float random(float2 fact)
{
    return frac(sin(dot(float2(fact.x, fact.y), float2(12.9898, 78.233))) * 43758.5453);
}

float2 randomVec(float2 fact)
{
    fact.x += t;
    fact.y += t;
    
    float2 angle = float2(
            dot(fact, float2(127.1, 311.7)),
            dot(fact, float2(269.5, 183.3))
            );

    return frac(sin(angle) * 43758.5453) * 2 - 1;
}

//パーリンノイズ生成
float PerlinNoise(float density, float2 uv)
{
    float2 uvFloor = floor(uv * density);
    float2 uvFrac = frac(uv * density);
            
    //各頂点のランダムなベクトルを取る
    float2 v00 = randomVec(uvFloor + float2(0, 0));
    float2 v01 = randomVec(uvFloor + float2(0, 1));
    float2 v10 = randomVec(uvFloor + float2(1, 0));
    float2 v11 = randomVec(uvFloor + float2(1, 1));
               
    //上で決定したランダムベクトルを用いて、
    //各頂点ごとにランダムな色を内積から作成
    float c00 = dot(v00, uvFrac - float2(0, 0));
    float c01 = dot(v01, uvFrac - float2(0, 1));
    float c10 = dot(v10, uvFrac - float2(1, 0));
    float c11 = dot(v11, uvFrac - float2(1, 1));
            
    float2 u = uvFrac * uvFrac * (3 - 2 * uvFrac);

    float v0010 = lerp(c00, c10, u.x);
    float v0111 = lerp(c01, c11, u.x);

    return lerp(v0010, v0111, u.y) / 2 + 0.5;
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 colortex0 = tex0.Sample(smp, input.uv);
    float4 colortex1 = tex1.Sample(smp, input.uv);
    
    float density = 30;
                
    //densityを倍に、返り値を半減した物を加算して出力
    float fn = 0;
    float2 srand = { 10, 10 };
    fn += PerlinNoise(density * 1, input.uv) * 1.0 / 2;
    fn += PerlinNoise(density * 2, input.uv) * 1.0 / 4;
    fn += PerlinNoise(density * 4, input.uv) * 1.0 / 8;
    fn += PerlinNoise(density * 8, input.uv) * 1.0 / 16;
    
    float4 noiseColor = float4(1, fn + 0.5f, fn, 1);
    colortex0.rgb *= noiseColor.rgb;
    
    return colortex0;
}