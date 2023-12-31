#include "Engine_Shader_Defines.hpp"

cbuffer VS_Ocean : register(b2)
{
    float fWaveFrequency;
    float fWaveAmplitude;
    float fBumpHeight;
    float fpadding2;
    
    
    float2 vBumpSpeed;
    float2 fTextureScale;
}

cbuffer PS_Ocean : register(b0)
{
    float fFresnelBias;
    float fFresnelPower;
    float fFresnelAmount;
    float fShoreBlend; // 알파값
    
    float fHeightRatio;
    float2 vOceanSize; // 버텍스 전체 크기

    
    float4 vShallowColor;
    float4 vDeepColor;
}


cbuffer MatrixBuffer : register(b0)
{
    matrix World, View, Projection;
};

cbuffer CameraBuffer : register(b1)
{
    float4 vCameraPosition;
}

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PixelInput
{
    float4 vPosition : SV_POSITION;
    float4 oPosition : POSITION0;
    float4 wPosition : POSITION1;
    float4 vProjPos  : POSITION2;
    float2 vTexScale : TEXSCALE0;
    float3 vNormal : NORMAL0;
    float3 View : VIEW0;
    
    float2 vBump[3] : BUMP0;
    float3 vTangent[3] : TANGENT0;
};

struct Wave
{
    float fFrequency; // 파동
    float fAmplitude; // 진폭
    float Phase; // 위상
    float2 vDirection; // 방향
};

float EvaluateWave(Wave wave, float2 position, float time) // Axis Y
{
    float s = sin(dot(wave.vDirection, position) * wave.fFrequency + time + wave.Phase);
    return wave.fAmplitude * s;
}

float EvaluateDifferent(Wave wave, float2 position, float time) // Axis XZ
{
    float c = cos(dot(wave.vDirection, position) * wave.fFrequency + time + wave.Phase);
    return wave.fAmplitude * wave.fFrequency * c;
}

static const int WaveCount = 3;
float fTime;
float4 vDiffuseColor = float4(0.600f, 0.721f, 0.937f, 0.627f);
float4 vAmbient = float4(0.25f, 0.20f, 1.0f, 1.0f);
float4 vSpecular = float4(1.0f, 1.0f, 1.0f, 1.0f);

float4 vSunAmbient = float4(1.0f, 1.0f, 1.0f, 1.0f);

PixelInput VS_WAVE(VS_IN input)
{
    PixelInput output = (PixelInput) 0;

    Wave wave[WaveCount] =
    {
        0.0f, 0.0f, 0.50f, float2(-1.0f, 0.0f),
        0.0f, 0.0f, 1.30f, float2(-0.7f, -0.7f),
        0.0f, 0.0f, 0.25f, float2(0.2f, 0.1f)
    };

    wave[0].fFrequency = fWaveFrequency;
    wave[0].fAmplitude = fWaveAmplitude;
    wave[1].fFrequency = fWaveFrequency * 2.0f;
    wave[1].fAmplitude = fWaveAmplitude * 0.5f;
    wave[2].fFrequency = fWaveFrequency * 3.0f;
    wave[2].fAmplitude = fWaveAmplitude * 1.0f;
    
    float ddx = 0, ddy = 0;
    
    float runTime = fTime;
    
    for (int i = 0; i < WaveCount; ++i)
    {
        // TODO Runtime 
        input.vPosition.y += EvaluateWave(wave[i], input.vPosition.xz, runTime);
        
        float diff = EvaluateDifferent(wave[i], input.vPosition.xz, runTime);
        ddx += diff * wave[i].vDirection.x;
        ddy += diff * wave[i].vDirection.y;
    }
    
    float3 T = float3(1, ddx, 0);
    float3 B = float3(-ddx, 1, -ddy);
    float3 N = float3(0, ddy, 1);

    float3x3 matTangent = float3x3(normalize(T) * fBumpHeight, normalize(B) * fBumpHeight, normalize(N));
    
    output.vTangent[0] = mul(World[0].xyz, matTangent);
    output.vTangent[1] = mul(World[1].xyz, matTangent);
    output.vTangent[2] = mul(World[2].xyz, matTangent);
    
    matrix matWV, matWVP;
    matWV = mul(World, View);
    matWVP = mul(matWV, Projection);
    
    output.vPosition = mul(float4(input.vPosition, 1.0f), matWVP);
    output.oPosition = float4(input.vPosition, 1.0f);
    output.vTexScale = input.vTexcoord * fTextureScale;
    output.wPosition = mul(float4(input.vPosition, 1.0f), World);
    float4 wPosition = mul(float4(input.vPosition, 1.0f), World);
    output.View = vCameraPosition.xyz - wPosition.xyz;
    output.vTexcoord = input.vTexcoord;
    
    float tempTime = fmod(runTime, 100);
    output.vBump[0] = output.vTexScale + tempTime * vBumpSpeed;
    output.vBump[1] = output.vTexScale + 2.0f + tempTime * vBumpSpeed * 4.0f;
    output.vBump[2] = output.vTexScale + 4.0f + tempTime * vBumpSpeed * 8.0f;
    
    output.vProjPos = output.vPosition; 
    
    return output;
}

Texture2D HeightMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D DiffuseMap : register(t2);

bool WithInBound(float3 position)
{
    return (position.x > 0.0f && position.z > 0.0f && 
    position.x < vOceanSize.x && position.z < vOceanSize.y);
}

float EvalueateShoreBlend(float3 position)
{
    float2 temp = float2(position.x / vOceanSize.x, position.z / vOceanSize.y);
    float color = HeightMap.Sample(LinearSampler, temp).r / fHeightRatio;
    
    return 1.0f - color * fShoreBlend;
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vBloom : SV_TARGET3;
};

PS_OUT WAVE_PS(PixelInput input)
{
    
    PS_OUT output = (PS_OUT) 0;
    
    float4 t1 = NormalMap.Sample(LinearSampler, input.vBump[0]) * 2.0f - 1.0f;
    float4 t2 = NormalMap.Sample(LinearSampler, input.vBump[1]) * 2.0f - 1.0f;
    float4 t0 = NormalMap.Sample(LinearSampler, input.vBump[2]) * 2.0f - 1.0f;

    float3 normal = t0.xyz + t1.xyz + t2.xyz;
    
    float3x3 matTangent;
    matTangent[0] = input.vTangent[0];
    matTangent[1] = input.vTangent[1];
    matTangent[2] = input.vTangent[2];
    
    
    normal = mul(normal, matTangent);
    normal = normalize(normal);
    
    float4 color = 0;
    
    float facing = 1.0f - saturate(dot(input.View, normal));
    float fresnel = fFresnelBias + (1.0f - fFresnelBias) * pow(facing, fFresnelPower);
    
    color = lerp(vDeepColor, vShallowColor, facing);
    if (fShoreBlend > 0 && WithInBound(input.oPosition.xyz))
    {
        color.a = EvalueateShoreBlend(input.oPosition.xyz);
        color.rgb = lerp(2, color.rgb, color.a);
    }
    
    color.rgb = color.rgb * vSunAmbient.rgb * fresnel;
    
    output.vColor = color;
    output.vNormal = float4(normal.xyz * 0.5f + 0.5f, 0.f);
    output.vDepth = float4(input.vProjPos.z / input.vProjPos.w, input.vProjPos.w / 1000.f, 1.0f, 0.0f);
    output.vBloom = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    return output;
}

RasterizerState WireFrame
{
    FillMode = wireframe;
    CullMode = Back;
    FrontCounterClockwise = false;
};

technique11 Ocean
{
    pass Wave
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_WAVE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 WAVE_PS();
    }
}