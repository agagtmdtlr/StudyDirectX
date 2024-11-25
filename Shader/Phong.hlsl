cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

static float3 lightDir = float3(0.5, -0.5, 0.5);


struct Material
{
    uint diff;
};

struct InstanceConst
{
    uint materialID;
};

#define INSTANCE_MAX_COUNT 1024

cbuffer WorldBuffer
{
    matrix worlds[INSTANCE_MAX_COUNT];
};

cbuffer MaterialBuffer
{
    Material materials[INSTANCE_MAX_COUNT];
};

struct VSInput
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

Texture2D baseTexture: register(t0);
SamplerState baseSampler : register(s0);

Texture2D bumpTexture : register(t1);
SamplerState bumpSampler : register(s1);


struct Light
{
    float4 position;
};


VSOutput VSmain(VSInput vsInput)
{
    VSOutput vsOutput;
    
    vsInput.position.w = 1;    
    vsOutput.position = mul(vsInput.position, worldMatrix);
    vsOutput.position = mul(vsOutput.position, viewMatrix);
    vsOutput.position = mul(vsOutput.position, projectionMatrix);
    
    vsOutput.normal = mul(vsInput.normal, (float3x3)worldMatrix);
    vsOutput.normal = normalize(vsOutput.normal);
    
    vsOutput.uv = vsInput.uv;
    return vsOutput;
}

float4 SampleMaterial(in Texture2D tx, in SamplerState samp , in float2 uv)
{
    float4 color = float4(0, 0, 0, 1);
    color = tx.Sample(samp, uv);
    return color;
}

float4 PSmain(VSOutput vsOutput) : SV_TARGET
{
    float3 l = -normalize(lightDir);
    float3 n = normalize(vsOutput.normal);
    float LdotN = dot(l, n);
    float2 uv = vsOutput.uv;
    
    float4 color = float4(0.2, 0.1, 0.1, 1);
    
    const uint diff_id = materials[0].diff;
    
    float4 diff = SampleMaterial(baseTexture, baseSampler, uv);
    
    color += diff * LdotN;
    return color;
    
    //return baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
}