static float3 lightDir = float3(0.5, -0.5, 0.5);

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

float4 main(VSOutput vsOutput) : SV_TARGET
{
    float3 l = -normalize(lightDir);
    float3 n = normalize(vsOutput.normal);
    float LdotN = dot(l, n);
    float2 uv = vsOutput.uv;
        
    return float4(1, 0, 0, 1);
    //float4 color = float4(0.2, 0.1, 0.1, 1);
    //color += float4(1, 0, 0, 1) * LdotN;
    //return color;
    
    //return baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
}