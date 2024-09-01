Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

struct VSInput
{
    float4 poistion : POSITION;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};



float4 VSmain() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}


float4 PSmain(VSOutput vsOutput) : SV_TARGET
{
    return baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
}