#include "Ground.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float> depthTex : register(t1); //1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);

static const int pattern[16] =
{
    0, 8, 2, 10,
    12, 4, 14, 6,
    3, 11, 1, 9,
    15, 7, 13, 5
};

float3 magnitude(float3 v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(smp, input.uv * tiling + offset);
    float maskTexcolor = depthTex.Sample(smp, input.uv * tiling + offset);
    
    int ditherUV_x = (int) fmod(input.svpos.x, 4.0f); //パターンの大きさで割ったときの余りを求める
    int ditherUV_y = (int) fmod(input.svpos.y, 4.0f); //今回のパターンサイズは4x4なので4で除算
    float dither = pattern[ditherUV_x + ditherUV_y * 4]; //求めた余りからパターン値を取得
    
    //bool checkPZ = input.camWorldPos.z > input.camPlayerPos.z;
    //if (input.camWorldPos.z < input.camPlayerPos.z)
    //{
    //    discard;   
    //}
    
    float camlength = magnitude(playerpos - input.worldPos.xyz);
    
    float clamp_dither = saturate(dither / 16);
    float clamp_length = saturate(camlength / 200);
    
    clip(clamp_length - clamp_dither); //閾値が0以下なら描画しない
    //clip(1 - checkPZ);
    
    float4 shadecolor = { 0, 0, 0, 1 };
    
    float threshold = 0.01f;
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            float3 ambient = texcolor.rgb * m_ambient * dirLights[i].lightcolor;
            
            float intensity = step(threshold, saturate(dot(normalize(input.normal), dirLights[i].lightv)));
            
            float3 difColor = texcolor.rgb;
            
            float3 diffuse = difColor * intensity * dirLights[i].lightcolor;
 
            shadecolor.rgb += (ambient + diffuse);
            shadecolor.a += 1;
        }
    }
    
    return float4(shadecolor.rgb * color.rgb, color.a * texcolor.a);
    //return float4(maskTexcolor, 0, 0, 1);
}