#include "GaussianBlur.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	float totalWeight = 0;
	const float SIGMA = 0.005;
	const float STEMPWIDTH = 0.001;

	float4 col = float4(0,0,0,0);

	for (float py = -SIGMA * 2; py < SIGMA * 2; py += STEMPWIDTH)
	{
        for (float px = -SIGMA * 2; px < SIGMA * 2; px += STEMPWIDTH)
        {
			float2 pickUV = input.uv + float2(px, py);
			float weight = Gaussian(input.uv, pickUV, SIGMA);
			col += tex.Sample(smp, pickUV) * weight;
            totalWeight += weight;
        }
    }
	
    col.rgb = col.rgb / totalWeight;
    col.a = 1;
	
    return col;
}