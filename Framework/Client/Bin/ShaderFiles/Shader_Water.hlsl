#include "Engine_Shader_Defines.hpp"

Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D MaskTexture : register(t2);
Texture2D MaskTexture2 : register(t3);

float4 g_vCamPosition;


cbuffer MatrixBuffer
{
    matrix World, View, Projection, ReflectionMatrix;
};

cbuffer WaterOption
{
    float fWaterTranslationSpeed;
    float fWaterTime;
    float fReflectRefractScale;
	
    float fTiling = 5.0f;
    float fBloomTiling = 5.0f;
    float fStrength = 2.0f;
    float2 flowDirection;
	
    bool bFresnel = false;
};

cbuffer CameraBuffer
{
    float4 vCameraPosition;
};

struct VS_IN
{
    float3 vPosition : POSITION; // WVP Æ÷Áö¼Ç
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct WaterVertexToPixel
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
    
    float4 vRefractionPos : TEXCOORD3;
    float4 vReflectionPos : TEXCOORD4;
    //float4 vReflectionMapSamplingPos : TEXCOORD1;
    //float2 vBumpMapSamplingPos : TEXCOORD2;
    //float4 vRefractionMapSamplingPos : TEXCOORD3;
    //float4 vWorldPos : TEXCOORD4;
};

WaterVertexToPixel WaterVertexShader(VS_IN input)
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
    
	// 
    output.vPosition = mul(float4(input.vPosition, 1.f), matWVP);
    output.vWorldPosition = mul(float4(input.vPosition, 1.f), World);
    output.vNormal = normalize(mul(float4(input.vNormal, 0.f), World)).xyz;
    output.vTangent = normalize(mul(float4(input.vTangent, 0.f), World)).xyz;
    output.vBinormal = normalize(cross(output.vNormal, output.vTangent));
    output.vTexUV = input.vTexUV;
    output.vProjPos = output.vPosition;
    
    return output;
}

struct WaterPixelToFrame
{
    float4 vColor : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vBloom : SV_TARGET3;
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
    float4 vNormalMap2;
    float3 vNormal;
    float4 vReflectionColor;
    float4 vRefractionColor;
	
	// TODO 
    float2 vFlowDir = normalize(flowDirection);
    float2 flow = vFlowDir * fWaterTime * fWaterTranslationSpeed;
	
    vNormalMap = NormalTexture.Sample(LinearSampler, input.vTexUV * fTiling + flow) * fStrength;
    vNormalMap2 = NormalTexture.Sample(LinearSampler, input.vTexUV * fTiling - flow * 0.3f) * fStrength * 0.5f;
    vNormal = (vNormalMap + vNormalMap2) * 0.5f;
	
    float4 vMtrlDiffuse = float4(0.0f, 144.0f / 255.0f, 233.0f / 255.0f, 0.85f);
    vNormal = (vNormalMap.xyz * 2.0f) - 1.0f;
	
    float4 vMaskMap = MaskTexture.Sample(LinearSampler, input.vTexUV * fTiling + flow) * fStrength;
    float4 vMaskMap2 = MaskTexture.Sample(LinearSampler, input.vTexUV * fTiling - flow * 0.3f) * fStrength * 0.5f;
	
    float4 vFinalMask = (vMaskMap + vMaskMap) * 0.5f;
	
    if (vFinalMask.r >= 0.5f && vFinalMask.g >= 0.5f && vFinalMask.b >= 0.5f)
        vMtrlDiffuse.rgb += 0.3f;

    float4 vMaskMap3 = MaskTexture2.Sample(LinearSampler, input.vTexUV * fBloomTiling + flow) * 1.05f;
	
    if (vMaskMap3.r >= 0.99999999f && vMaskMap3.g >= 0.99999999f && vMaskMap3.b >= 0.99999999f)
        output.vBloom = float4(1.0f, 1.0f, 1.0f, 0.5f);
    else
        output.vBloom = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    float3x3 WorldMatrix = float3x3(input.vTangent, input.vBinormal, input.vNormal.xyz);
	
    vNormal = normalize(mul(vNormal, WorldMatrix));
	
	

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
    vMtrlDiffuse.a = 0.7f;
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
    
    output.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
    output.vDepth  = float4(input.vProjPos.z / input.vProjPos.w, input.vProjPos.w / 1000.f, 0.0f, 1.0f);
    
    return output;
}



technique11 Water
{
    pass Evermore_Water
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 WaterVertexShader();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 WaterPS();
    }
}