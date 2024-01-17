#include "Engine_Shader_Defines.hpp"

Texture2D ShaderTexture : register(t0);

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix, viewMatrix, projectionMatrix;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(worldMatrix, viewMatrix);
    matWVP = mul(matWV, projectionMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vWorldPosition = mul(float4(In.vPosition, 1.f), worldMatrix);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), worldMatrix)).xyz;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), worldMatrix)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
    Out.vTexcoord = In.vTexcoord;
    Out.vProjPos = Out.vPosition;

	
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float4 vWorldPosition : TEXCOORD2;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = ShaderTexture.Sample(PointSampler, In.vTexcoord);
    
    if (0.3 >= Out.vDiffuse.a)
        discard;
	
    return Out;
}

// Default Stencil
DepthStencilState DS_Mirror_Default
{
    DepthEnable = true;
    DepthWriteMask = ALL;
    DepthFunc = LESS;
    StencilEnable = true;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;

// Front
    FrontFaceStencilFail = KEEP;
    FrontFaceStencilDepthFail = INCR;
    FrontFaceStencilPass = KEEP;
    FrontFaceStencilFunc = ALWAYS;

// Back
    BackFaceStencilFail = KEEP;
    BackFaceStencilDepthFail = DECR;
    BackFaceStencilPass = REPLACE;
    BackFaceStencilFunc = ALWAYS;
};


// 처음 Mirror DepthStencil State
DepthStencilState DS_Mirror_Reflect
{
    DepthEnable = true;
    DepthWriteMask = ZERO;
    DepthFunc = LESS;
    StencilEnable = true;
    StencilReadMask = 0xff;
    StencilWriteMask = 0xff;

// Front
    FrontFaceStencilFail = KEEP;
    FrontFaceStencilDepthFail = KEEP;
    FrontFaceStencilPass = REPLACE;
    FrontFaceStencilFunc = ALWAYS;

// Back
    BackFaceStencilFail = KEEP;
    BackFaceStencilDepthFail = KEEP;
    BackFaceStencilPass = REPLACE;
    BackFaceStencilFunc = ALWAYS;
};

BlendState BS_Mirror_NoneWrite // TurnOnWriteAlphaBlend
{
    AlphaToCoverageEnable = false;
    BlendEnable[0] = false;
    SrcBlend[0] = ONE;
    DestBlend[0] = ZERO;
    BlendOp[0] = ADD;
    SrcBlendAlpha[0] = ONE;
    DestBlendAlpha[0] = ZERO;
    BlendOpAlpha[0] = ADD;
    RenderTargetWriteMask[0] = 0;
};

BlendState BS_Mirror_Default
{
    AlphaToCoverageEnable = false;
    BlendEnable[0] = true;
    SrcBlend[0] = SRC_ALPHA;
    DestBlend[0] = INV_SRC_ALPHA;
    BlendOp[0] = ADD;
    SrcBlendAlpha[0] = ONE;
    DestBlendAlpha[0] = ZERO;
    BlendOpAlpha[0] = ADD;
};

RasterizerState RS_Mirror_Default
{
    AntialiasedLineEnable = false;
    CullMode = BACK;
    DepthBias = 0;
    DepthBiasClamp = 0.0f;
    DepthClipEnable = true;
    FillMode = SOLID;
    FrontCounterClockwise = false;
    MultisampleEnable = false;
    ScissorEnable = false;
    SlopeScaledDepthBias = 0.0f;
};



technique11 DefaultTechnique
{
    pass ReflectMirror // 처음에 먼저 이 패스 사용. -> 0번패스
    {
        SetRasterizerState(RS_Mirror_Default); // RS_Default
        SetDepthStencilState(DS_Mirror_Reflect, 0); // SetMirrorMarkDepthStencilState
        SetBlendState(BS_Mirror_NoneWrite, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff); // TurnOnWriteAlphaBlend

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();

    }

    pass DefaultMirror // 캐릭터 반사 후 사용 -> 1번 패스 
    {
        SetRasterizerState(RS_Mirror_Default);
        SetDepthStencilState(DS_Mirror_Default, 0);
        SetBlendState(BS_Mirror_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}