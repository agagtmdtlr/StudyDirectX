cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

static float3 lightDir = float3(0.5, -0.5, 0.5);

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
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;    
};

Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

struct Light
{
    float4 position;
};


VSOutput VSmain(VSInput vsInput)
{
    VSOutput vsOutput;
    
    vsOutput.position = vsInput.position;
    
    vsOutput.position = mul(vsOutput.position, worldMatrix);
    vsOutput.position = mul(vsOutput.position, viewMatrix);
    vsOutput.position = mul(vsOutput.position, projectionMatrix);
    
    vsOutput.normal = mul(vsInput.normal, (float3x3)worldMatrix);
    vsOutput.normal = normalize(vsOutput.normal);
    return vsOutput;
}

float4 PSmain(VSOutput vsOutput) : SV_TARGET
{
    float3 l = -normalize(lightDir);
    float3 n = normalize(vsOutput.normal);
    float LdotN = dot(l, n);
        
    //return float4(1 * LdotN, 0, 0, 1);
    return float4(1 , 0, 0, 1) * LdotN;
    
    //return baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
}