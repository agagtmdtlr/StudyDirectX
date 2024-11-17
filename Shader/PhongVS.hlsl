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

VSOutput main(VSInput vsInput)
{
    VSOutput vsOutput;
    
    vsOutput.position = vsInput.position;
    
    vsOutput.position = mul(vsOutput.position, worldMatrix);
    vsOutput.position = mul(vsOutput.position, viewMatrix);
    vsOutput.position = mul(vsOutput.position, projectionMatrix);
    
    vsOutput.normal = mul(vsInput.normal, (float3x3) worldMatrix);
    vsOutput.normal = normalize(vsOutput.normal);
    
    vsOutput.uv = vsInput.uv;
    return vsOutput;
}
