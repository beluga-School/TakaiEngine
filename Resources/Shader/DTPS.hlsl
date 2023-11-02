#include "DT.hlsli"

Texture2D<float4> mainTex : register(t0); //0番スロットに設定されたテクスチャ
Texture2D<float4> maskTex : register(t1); //1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); //0番スロットに設定されたサンプラー

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
    float4 main = mainTex.Sample(smp, input.uv);
    float4 mask = maskTex.Sample(smp, input.uv);
    
    float4 shadecolor = { 0, 0, 0, 1 };
  
    int ditherUV_x = (int) fmod(input.svpos.x, 4.0f); //パターンの大きさで割ったときの余りを求める
    int ditherUV_y = (int) fmod(input.svpos.y, 4.0f); //今回のパターンサイズは4x4なので4で除算
    float dither = pattern[ditherUV_x + ditherUV_y * 4]; //求めた余りからパターン値を取得
    
    float camlength = magnitude(cameraPos - input.worldPos.xyz);
    
    float clamp_dither = saturate(dither / 16);
    float clamp_length = saturate(camlength / 20);
    
    clip(clamp_length - clamp_dither); //閾値が0以下なら描画しない
    
    for (int i = 0; i < DIRLIGHT_NUM; i++)
    {
        if (dirLights[i].active)
        {
            //アンビエント = オブジェクトの元の色 * 暗くする率 * 光の色
            float3 ambient = main.rgb * m_ambient * dirLights[i].lightcolor;
            
            //ディフューズ
            //光との内積値
            float intensity = saturate(dot(normalize(input.normal), dirLights[i].lightv));
            //オブジェクトの色
            float3 difColor = main.rgb;
            
            //ディフューズ = オブジェクトの元の色 * 光との内積値 * 光の色
            float3 diffuse = difColor * intensity * dirLights[i].lightcolor;
            
            shadecolor.rgb += (ambient + diffuse);
            shadecolor.a += 1;
        }
    }
    
    //どっちが正解？
    return shadecolor * color;
}