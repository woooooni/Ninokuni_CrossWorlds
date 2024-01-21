#include "Engine_Shader_Defines.hpp"
#define FLT_MAX 3.402823466e+38F

Texture2D SunOccluderTexture : register(t0);


cbuffer MatrixBuffer : register(b0)
{
    matrix world, view, projection, worldInvTranspose;
}

cbuffer GodRayBuffer : register(b0)
{
    float4 vLightShaftValue = float4(0.975, 0.25f, 0.825f, 2.0f);
    float2 vScreenSunPosition;
}


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;
    
    matrix matWV, matWVP;
    
    matWV = mul(world, view);
    matWVP = mul(matWV, projection);

    output.vPosition = mul(float4(input.vPosition, 1.0f), matWVP);
    output.vTexcoord = input.vTexcoord;
    
    return output;
}

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

sampler LinearSamplerClamp = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
    MaxAnisotropy = 16;
    MaxLOD = FLT_MAX;
    ComparisonFunc = NEVER;

};

PS_OUT PS_LIGHTSHAFT(VS_OUT input)
{
    PS_OUT output = (PS_OUT) 0;
    float2 vTexcoord = input.vTexcoord;
    
    float4 lightData = vLightShaftValue;
    // x = Density, y = Weight, z = Decay , w = Exposure
    
    // MaskColor
    float3 vMaskColor = SunOccluderTexture.SampleLevel(LinearSamplerClamp, vTexcoord, 0).rgb;
    float2 lightPosition = vScreenSunPosition;
    float2 deltaTexcoord = (vTexcoord - lightPosition);
    #define NUM_SAMPLERS 64
    
    deltaTexcoord *= lightData.x / NUM_SAMPLERS; // Density / NUM_SAMPLERS
    
    float illuminationDecay = 1.0f;
    float3 accumulatedGodRays = float3(0.0f, 0.0f, 0.0f);
    float3 accumulated = 0.0f;
    
    for (int i = 0; i < NUM_SAMPLERS; ++i)
    {
        vTexcoord.xy -= deltaTexcoord;
        float3 sample = SunOccluderTexture.SampleLevel(LinearSamplerClamp, vTexcoord.xy, 0).rgb;
        sample *= illuminationDecay * lightData.y; // illuminationDecay * Weight
        accumulated += sample;
        illuminationDecay *= lightData.z; // illuminationDecay *= lightData.z (Decay)
    }
    
    accumulated *= lightData.w;
    output.vColor = saturate(float4(vMaskColor + accumulated, 1.0f));
    
   return output;
}

BlendState AdditiveBlendState
{
    AlphaToCoverageEnable = false;
    BlendEnable[0] = true;
    SrcBlend[0] = ONE;
    DestBlend[0] = ONE;
    BlendOp[0] = ADD;
    RenderTargetWriteMask[0] = 15;
};

//SSAO
Texture2D NormalDepthMap : register(t1);
Texture2D RandomVecMap : register(t2);
SamplerState SampRandVec : register(s0)
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
    AddressW = Wrap;
    ComparisonFunc = NEVER;
    BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    MinLOD = 0.0f;
    MaxLOD = FLT_MAX;
};
SamplerState SampNormalDepth : register(s1)
{
    Filter = MIN_MAG_LINEAR_MIP_POINT;
    AddressU = Border;
    AddressV = Border;
    AddressW = Border;
    ComparisonFunc = NEVER;
    BorderColor = float4(0.0f, 0.0f, 0.0f, 1e5f); // 큰 깊이 값으로 설정
    MinLOD = 0.0f;
    MaxLOD = FLT_MAX;
};

cbuffer CBFrustum : register(b1)
{
    float4 FrustumCorner[4];
};

cbuffer CBPerFrame : register(b2)
{
    float4 offsetVectors[14];
    
    // 카메라 공간에 주어진 좌표
    float OcclusionRadius = 0.5f;
    float OcclusionFadeStart = 0.2f;
    float OcclusionFadeEnd = 2.0f;
    float SurfaceEpsilon = 0.05f;
};

struct VertexIn
{
    float3 vPositionLocal : POSITION;
    float3 ToFarPlaneIndex : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VertexOut
{
    float4 vPosition : SV_POSITION;
    float3 ToFarPlane : TEXCOORD0;
    float2 vTexcoord : TEXCOORD1;
};

VertexOut VS_SSAO(VertexIn input)
{
    VertexOut output = (VertexOut) 0;

    matrix matWV, matWVP;
    
    matWV = mul(world, view);
    matWVP = mul(matWV, projection);
    // 정점 데이터의 특수성으로 인해 정점 정보가 뷰포트 전체를 덮는 NDC 좌표로 변환
    output.vPosition = mul(float4(input.vPositionLocal, 1.0f), matWVP);
    
    // 원점에서 각 원거리 클리핑 평면의 각 픽셀까지의 벡터를 얻기 위한 보간을 용이하게 하기 위해
    // 각 카메라 공간에서 원점부터 원거리 클리핑 평면의 모서리까지의 벡터를 찾는다.
    output.ToFarPlane = FrustumCorner[input.ToFarPlaneIndex.x].xyz;
    
    output.vTexcoord = input.vTexcoord;
    
    return output;
}

// 교합값을 찾는 기능
float OcclusionFunction(float distZ)
{
    //
    //    /|\ Occlusion
    // 1.0 |      ---------------\
    //     |      |             |  \
    //     |                         \
    //     |      |             |      \
    //     |                             \
    //     |      |             |          \
    //     |                                 \
    // ----|------|-------------|-------------|-------> zv
    //     0     Eps          zStart         zEnd

    float occlusion = 0.0f;
    
    // 너무 가까우면 두 저미 같은 평면에 있으면 폐색을 일으킬 수 없는 것으로 간주한다.
    if (distZ > SurfaceEpsilon)
    {
        float fadeLength = OcclusionFadeEnd - OcclusionFadeStart;
        
        // DistZ가 증가함에 따라 폐색의 값이 1에서 0으로 선형적으로 감소한다.
        occlusion = saturate((OcclusionFadeEnd - distZ) / fadeLength);
    }
    
    return occlusion;
}

float4 PS_SSAO(VertexOut input) : SV_Target
{
    // p : 점 p의 AmbientOcclusion 값을 계산한다.
    // n : 점 p에서 법선 벡터
    // q : 점 p로부터의 무작위 벡터
    // r : p를 모호하게 할 수 있는 잠재적인 점
    
    // 카메라 공간의 픽셀의 법선 벡터와 Z 값을 가지고온다.
    // ScreenQuad의 좌표는 이미 Texture 좌표공간에 있다.
    float4 normalDepth = NormalDepthMap.SampleLevel(SampNormalDepth, input.vTexcoord, 0.0f);
    
    // 카메라 공간의 픽셀좌표와 Z 값
    float3 n = normalDepth.xyz;
    float pz = normalDepth.w;
    
    // 유사 삼각형의 원리를 사용, 카메라 공간에서 픽셀의 위치를 재구성한다.
    // 해당 원점에서 먼 클리핑 평면까지 선분은 점 P를 통과한다.
    // P = t * input.ToFarPlane이 된다.
    // 따라서 pz = t * input.ToFarPlane.z가 된다.
    // pz = t * input.ToFarPlane.z가 있고
    // t = pz / input.ToFarPlane.z가 있다.
    // P = (Pz/input.ToFarPlane), z) * input.ToFarPlane
    
    float3 p = (pz / input.ToFarPlane.z) * input.ToFarPlane;
    
    // 랜덤 벡터 맵의 값을 구하고 값을 [0, 1]에서 [-1, 1] 범위로 변경한다.
    // 랜덤 벡터 타일링에 주의.
    float3 randVec = 2.0f * RandomVecMap.SampleLevel(SampRandVec, 4.0f * input.vTexcoord, 0.0f).rgb - 1.0f;
    
    // 폐색 값의 합
    float occlusionSum = 0.0f;
    
    // 샘플 수 설정
    const int sampleCount = 14;
   
    // n 벡터 방향으로 점 p의 반구 범위에 있는 인접 점을 샘플링하는 것은 일반적으로 8/16/32로 나누어진다.
    // 여기서는 14개의 변위 벡터, 즉 14개의 샘플링 점을 갖게 된다.
    [unroll]
    for (int i = 0; i < sampleCount; ++i)
    {
        // 변위 벡터는 고정되고 균일하게 분산된다
        // 왜냐하면 변위가 같은 방향에서 동일하기 때문에 변위 벡터를 사용하여 반영하면 무작위로 동일한 변위 분산 벡터를 얻는다.
        // 무작위 벡터를 사용하여 압사하고 고정한다.
        // 큐브 벡터는 법선 벡터로 사용되어 반사 벡터를 최종 변위 벡터로 찾는다.
        float3 offset = reflect(offsetVectors[i].xyz, randVec);
        
        // 변위 벡터가 표면 뒤에 있는 경우 Flip은 점 q가 항상 시점에 가까운 p측면에 있도록 변위 벡터를 뒤집는다.
        // sign()은 값의 부호를 찾는다.
        float flip = sign(dot(offset, n));
        
        // 점 p의 교합반경에서 점 q를 찾는다.
        float3 q = p + flip * OcclusionRadius * offset;
        
        // q를 투영하고 투영된 텍스쳐 좌표를 생성.
        // q 자체가 카메라 공간에 있으므로 투영된 텍스쳐 좌표로 변경된다.
        // 관점 분할은 마지막에 수행되어야 한다.
        float4 ProjQ = mul(float4(q, 1.0f), projection);
        ProjQ /= ProjQ.w;

        // 원점에서 q까지의 카메라 공간에서 NormalDepthMap에 해당하는 최소 깊이 값을 찾는다.
        // 이 최소 깊이는 카메라 공간의 q 깊이와 같지 않다.
        float rz = NormalDepthMap.SampleLevel(SampNormalDepth, ProjQ.xy, 0.0f).w;

        // 다시 유사 삼각형 원리를 사용한다.
        float3 r = (rz / q.z) * q;

        // dot(n, normalize(rp))는 점 p 앞의 점 r의 폐색 값을 계산한다. 각도가 작을 수록 폐색 값이 커지고,
        // r과 p가 Z거리에서 가까울수록 폐색이 커진다.(임계값에 가까움)
        // 셀프 섀도잉을 방지하기 위해 폐색 값은 0이다.
        float distZ = p.z - r.z;
        float dp = max(dot(normalize(n), normalize(r - p)), 0.0f);
        float occlusion = dp * OcclusionFunction(distZ);
        
        occlusionSum += occlusion;
    }
    
    // 최종 폐색값 노멀라이즈
    occlusionSum /= (float) sampleCount;
    
    // AmbientLight 허용 값 찾기
    float access = 1.0f - occlusionSum;
    
    // SSAOMap의 효과를 선명하게 할려면 pow를 사용
    // SSAOMap은 회색조이다.
    return saturate(pow(access, 4.0f));
}

technique11 DefaultTechnique
{
    pass GodRay
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(AdditiveBlendState, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_LIGHTSHAFT();
        ComputeShader = NULL;
    }

    pass SSAO
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		
        VertexShader = compile vs_5_0 VS_SSAO();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_SSAO();
        ComputeShader = NULL;
    }
}