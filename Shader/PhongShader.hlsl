Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

struct Light
{
    float4 position;
};

struct Material
{
    float amb;
    float diff;
    float spec;
    float alpha;
    
    float reflection;
    float transparency;
    float2 temp;
};

struct VSInput
{
    float4 poistion : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;    
    float2 uv : TEXCOORD;
};

StructuredBuffer<Light> lights : register(t0); 
StructuredBuffer<Material> materials : register(t1);


VSOutput VSmain(VSInput vsInput)
{
    VSOutput vsOutput;
    
    vsOutput.position = vsInput.poistion;
    vsOutput.uv = vsInput.uv;
    
    return vsOutput;
}


float4 PSmain(VSOutput vsOutput) : SV_TARGET
{
    vsOutput.normal;
    return baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
}