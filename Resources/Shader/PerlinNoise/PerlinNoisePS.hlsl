#include "PerlinNoise.hlsli"

float random(float2 fact)
{
    return frac(sin(dot(float2(fact.x, fact.y), float2(12.9898, 78.233))) * 43758.5453);
}

float2 randomVec(float2 fact)
{
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

float4 main(VSOutput i) : SV_TARGET
{
    float density = 30;
                
    //densityを倍に、返り値を半減した物を加算して出力
    float fn = 0;
    fn += PerlinNoise(density * 1, i.uv) * 1.0 / 2;
    fn += PerlinNoise(density * 2, i.uv) * 1.0 / 4;
    fn += PerlinNoise(density * 4, i.uv) * 1.0 / 8;
    fn += PerlinNoise(density * 8, i.uv) * 1.0 / 16;
                
    float4 col = float4(fn, fn, fn, 1);

    return col;
}