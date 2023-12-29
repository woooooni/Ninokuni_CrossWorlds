
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix,    g_ViewMatrix,    g_ProjMatrix;
matrix      g_WorldMatrixInv, g_ViewMatrixInv, g_ProjMatrixInv;

TextureCube g_DiffuseTexture;

texture2D   g_DepthTarget;
texture2D   g_Diffuse2DTexture, g_Alpha2DTexture;

float       g_fAlpha_Discard;
float3      g_fBlack_Discard;

float       g_fColor_Add_01_Alpha;
float3      g_fColor_Add_01;
float3      g_fColor_Add_02;

float       g_fColor_Alpha;

float3      g_fBloom_Power;
float       g_fBlur_Power;

struct VS_IN
{
	float3		vPosition : POSITION;	
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;	
	float3		vTexUV    : TEXCOORD0;	
    float4      vProjPos  : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV    = In.vTexUV;
    Out.vProjPos  = Out.vPosition;
	
	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV    : TEXCOORD0;
    float4      vProjPos  : TEXCOORD1;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//Out.vColor = (vector)0.f;

	return Out;
}

struct PS_OUT_EFFECT
{
    float4 vDiffuse_All    : SV_TARGET0;
    float4 vDiffuse_None   : SV_TARGET1;
    float4 vDiffuse_Low    : SV_TARGET2;
    float4 vDiffuse_Middle : SV_TARGET3;
    float4 vDiffuse_High   : SV_TARGET4;
    float4 vBloom          : SV_TARGET5;
};

float4 Caculation_Brightness(float4 vColor)
{
    float4 vBrightnessColor = float4(0.f, 0.f, 0.f, 0.f);

    float fPixelBrightness = dot(vColor.rgb, g_fBloom_Power.rgb);
    if (fPixelBrightness > 0.99f)
        vBrightnessColor = float4(vColor.rgb, 1.0f);

    return vBrightnessColor;
}

PS_OUT_EFFECT PS_MAIN_DECAL(PS_IN In)
{
    PS_OUT_EFFECT Out = (PS_OUT_EFFECT) 0;

	// Decal Box의 x,y 위치를 UV좌표로 변환.
    float2 vTexUV;
    vTexUV.x = In.vProjPos.x / In.vProjPos.w * 0.5f + 0.5f;
    vTexUV.y = In.vProjPos.y / In.vProjPos.w * -0.5f + 0.5f;
	
	// Decal Box x,y 위치 기준으로 깊이 값을 가져옴.
    vector vDepthDesc = g_DepthTarget.Sample(PointSampler, vTexUV);
    // 캐릭터 픽셀은 생략
    if (vDepthDesc.z == 1.f)
        discard;
    
    float fViewZ = vDepthDesc.y * 1000.f;

    vector vProjPos = (vector) 0.f;
    vProjPos.x = (vTexUV.x * 2.f - 1.f) * fViewZ;
    vProjPos.y = (vTexUV.y * -2.f + 1.f) * fViewZ;
    vProjPos.z = (vDepthDesc.x) * fViewZ;
    vProjPos.w = fViewZ;

    vector vViewPos  = mul(vProjPos,  g_ProjMatrixInv);
    vector vWorldPos = mul(vViewPos,  g_ViewMatrixInv);
    vector vLocalPos = mul(vWorldPos, g_WorldMatrixInv);

	// 중점을 기준으로 0.5 이기때문에, 0.5를 뺀 음수값은 상자 밖이므로 자름.
    float3 fObjectAbsPos = abs(vLocalPos.xyz);
    clip(0.5f - fObjectAbsPos);

	// 데칼박스 버퍼가 -0.5 ~0.5 사이므로, 0.5를 더해줘서 UV좌표로 만들어줌.
    float2 fDecalUV      = vLocalPos.xz + 0.5f;
    vector vDiffuseColor = g_Diffuse2DTexture.Sample(LinearSampler, fDecalUV);
    vDiffuseColor.a -= g_fColor_Alpha;
    if (vDiffuseColor.a <= g_fAlpha_Discard ||
		vDiffuseColor.r <= g_fBlack_Discard.r && vDiffuseColor.g <= g_fBlack_Discard.g && vDiffuseColor.b <= g_fBlack_Discard.b)
        discard;
        
    Out.vDiffuse_All    = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_None   = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Low    = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_Middle = float4(0.f, 0.f, 0.f, 0.f);
    Out.vDiffuse_High   = float4(0.f, 0.f, 0.f, 0.f);
    Out.vBloom          = float4(0.f, 0.f, 0.f, 0.f);
    
    if (vDiffuseColor.a >= g_fColor_Add_01_Alpha)
    {
        vDiffuseColor.rgb += g_fColor_Add_01;
        Out.vBloom         = Caculation_Brightness(vDiffuseColor);
    }
    else
        vDiffuseColor.rgb += g_fColor_Add_02;
    
    Out.vDiffuse_All  = vDiffuseColor;
    if (g_fBlur_Power <= 0.0f)
        Out.vDiffuse_None = vDiffuseColor;
    else if (g_fBlur_Power > 0.0f && g_fBlur_Power <= 0.3f)
        Out.vDiffuse_Low = vDiffuseColor;
    else if (g_fBlur_Power > 0.3f && g_fBlur_Power <= 0.7f)
        Out.vDiffuse_Middle = vDiffuseColor;
    else
        Out.vDiffuse_High = vDiffuseColor;
    
    return Out;
}

BlendState DefaultBlendState
{
	BlendEnable[0] = true;
	SrcBlend  = Src_Alpha;
	DestBlend = Inv_Src_Alpha;
	BlendOp   = Add;	
};

technique11 DefaultTechnique
{
	pass SkyBox
	{
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
	    /*SetBlendState(DefaultBlendState);
		SetRasterizerState();
		*/

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}	

    pass Decal
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader   = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader    = compile ps_5_0 PS_MAIN_DECAL();
    }
}