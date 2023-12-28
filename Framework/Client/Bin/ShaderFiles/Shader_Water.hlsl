#include "Engine_Shader_Defines.hpp"


cbuffer MatrixBuffer
{
    matrix World, View, Projection, ReflectionMatrix;
};

cbuffer WaterOption
{
    float fWaterTranslation;
    float fReflectRefractScale;
    bool bFresnel = false;
};

cbuffer CameraBuffer
{
    float4 vCameraPosition;
};

struct VertexInputType
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct WaterVertexToPixel
{
    float4 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    
    float4 vRefractionPos : TEXCOORD2;
    float4 vReflectionPos : TEXCOORD3;
    //float4 vReflectionMapSamplingPos : TEXCOORD1;
    //float2 vBumpMapSamplingPos : TEXCOORD2;
    //float4 vRefractionMapSamplingPos : TEXCOORD3;
    //float4 vWorldPos : TEXCOORD4;
};

WaterVertexToPixel WaterVertexShader(VertexInputType input)
{
    WaterVertexToPixel output = (WaterVertexToPixel) 0;
    
    matrix matWV, matWVP;
    matrix matReflectPW;
    matrix matVPWorld;
    
    matWV = mul(World, View);
    matWVP = mul(matWV, Projection);
    
    matReflectPW = mul(ReflectionMatrix, Projection);
    matReflectPW = mul(World, matReflectPW);
    output.vReflectionPos = mul(float4(input.vPosition, 1.0f), matReflectPW);
    
    matVPWorld = mul(View, Projection);
    matVPWorld = mul(World, matVPWorld);
    output.vRefractionPos = mul(float4(input.vPosition, 1.0f), matVPWorld);
    
    output.vPosition = mul(float4(input.vPosition, 1.f), matWVP);
    output.vTexCoord = input.vTexcoord;
    
    output.vProjPos = output.vPosition;
    
    return output;
}

struct WaterPixelToFrame
{
    float4 vColor : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    //float4 vSunMask : SV_TARGET3;
};


Texture2D ReflectionMap : register(t0);
Texture2D RefractionMap : register(t1);
Texture2D WaterBumpMap : register(t2); // Normal
Texture2D WaterDiffuse : register(t3);

WaterPixelToFrame WaterPS (WaterVertexToPixel input)
{
    WaterPixelToFrame output = (WaterPixelToFrame) 0;

    float2 vReflectTexCoord;
    float2 vRefractTexCoord;
    float4 vNormalMap;
    float3 vNormal;
    float4 vReflectionColor;
    float4 vRefractionColor;
    
    float4 vMtrlDiffuse = WaterDiffuse.Sample(LinearSampler, input.vTexCoord);
    
    input.vTexCoord.y += fWaterTranslation;
    
    vNormalMap = WaterBumpMap.Sample(LinearSampler, input.vTexCoord);
    vNormal = (vNormalMap.xyz * 2.0f) - 1.0f;

    //vReflectTexCoord.x = input.vReflectionPos.x / input.vReflectionPos.w / 2.0f + 0.5f;
    //vReflectTexCoord.y = -input.vReflectionPos.y / input.vReflectionPos.w / 2.0f + 0.5f;

    //vRefractTexCoord.x = input.vRefractionPos.x / input.vRefractionPos.w / 2.0f + 0.5f;
    //vRefractTexCoord.y = -input.vRefractionPos.y / input.vRefractionPos.w / 2.0f + 0.5f;
    
    //vReflectTexCoord = vReflectTexCoord + (vNormal.xy * fReflectRefractScale);
    //vRefractTexCoord = vRefractTexCoord + (vNormal.xy * fReflectRefractScale);

    //vReflectionColor = ReflectionMap.Sample(LinearSampler, vReflectTexCoord);
    //vRefractionColor = RefractionMap.Sample(LinearSampler, vRefractTexCoord);
    
    //float fFresnelTerm = 0.0f;
    //float4 vLook = input.vProjPos - vCameraPosition;
    //vLook = normalize(vLook);
    
    output.vColor = vMtrlDiffuse;

    //if(true == bFresnel)
    //{
    //    fFresnelTerm = 0.02f + 0.97f * pow((1 - dot(vLook, float4(vNormal, 1.0f))), 5.0f);
    //    float4 vCombinedColor = vRefractionColor * (1 - fFresnelTerm) *
    //        vRefractionColor.a * vReflectionColor.a + vReflectionColor * fFresnelTerm * vReflectionColor.a * vRefractionColor.a;
    //    output.vColor = vCombinedColor;
    //}
    //else
    //    output.vColor = lerp(vReflectionColor, vRefractionColor, 0.5f);
    
    output.vNormal = float4(vNormal, 0.0f);
    output.vDepth = float4(input.vProjPos.z / input.vProjPos.w, input.vProjPos.w / 1000.f, 0.0f, 0.0f);
    
    return output;
}



technique11 Water
{
    pass Evermore_Water
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 WaterVertexShader();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 WaterPS();
    }
}