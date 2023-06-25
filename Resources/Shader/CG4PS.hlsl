#include "CG4.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{   
    float4 colortex0 = tex0.Sample(smp, input.uv);
    float4 colortex1 = tex1.Sample(smp, input.uv);
 
    float totalWeight = 0;
    const float SIGMA = 0.005;
    const float STEMPWIDTH = 0.001;
    
    float4 color = colortex0; 
  
    for (float py = -SIGMA * 2; py < SIGMA * 2; py += STEMPWIDTH)
    {
        for (float px = -SIGMA * 2; px < SIGMA * 2; px += STEMPWIDTH)
        {
            float2 pickUV = input.uv + float2(px, py);
            float weight = Gaussian(input.uv, pickUV, SIGMA);
            color += tex0.Sample(smp, pickUV) * weight;
            totalWeight += weight;
        }
    }
	
    color.rgb = color.rgb / totalWeight;
    color.a = 1;
    
    if(fmod(input.uv.y,0.1f) < 0.05f)
    {
        color = colortex1;
    }
    
    return float4(color.rgb, 1);
}