#include "Engine_Shader_Defines.hpp"

texture2D g_Texture; // 디버그용 텍스쳐

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix g_ViewMatrixInv, g_ProjMatrixInv;

matrix g_CamProjMatrix;
matrix g_LightViewMatrix;

texture2D g_DiffuseTarget;
texture2D g_NormalTarget;
texture2D g_DepthTarget;
		  
texture2D g_ShadeTarget;
texture2D g_SpecularTarget;
		  
texture2D g_SSAOTarget;
texture2D g_ShadowTarget;
texture2D g_OutlineTarget;

// 조명
vector g_vCamPosition;

vector g_vLightDir;
vector g_vLightPos;
float  g_fLightRange;

vector g_vLightDiffuse;
vector g_vLightAmbient;
vector g_vLightSpecular;

vector g_vMtrlAmbient  = vector(0.4f, 0.4f, 0.4f, 1.f);
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

// 안개
float2 g_vFogStartEnd = { 1.f, 5.f };
float4 g_vFogColor    = { .5f, .5f, .5f, 1.f };

float  g_fBias        = 0.2f;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct VS_OUT
{	
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	float		fFogFactor : FOG;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;
	
	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexcoord = In.vTexcoord;

	return Out;
}

// DEBUG
struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord);

	return Out;
}

// LIGHT
struct PS_OUT_LIGHT
{
	float4	vShade : SV_TARGET0;	
	float4	vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc = g_NormalTarget.Sample(PointSampler, In.vTexcoord);
	vector		vDepthDesc = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
	float		fViewZ = vDepthDesc.y * 1000.f;

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient));

	vector		vReflect = reflect(normalize(g_vLightDir), vNormal);

	vector		vWorldPos;

	/* 투영스페이스 상의 위치를 구한다. */
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	/* 뷰스페이스 상의 위치를 구한다. */
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector	vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 50.f);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)0;

	vector		vNormalDesc = g_NormalTarget.Sample(PointSampler, In.vTexcoord);
	vector		vDepthDesc = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
	float		fViewZ = vDepthDesc.y * 1000.f;

	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	vector		vWorldPos;

	/* 투영스페이스 상의 위치를 구한다. */
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	/* 뷰스페이스 상의 위치를 구한다. */
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector		vLightDir = vWorldPos - g_vLightPos;
	float		fDistance = length(vLightDir);

	float		fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);

	Out.vShade = fAtt * g_vLightDiffuse * (saturate(dot(normalize(vLightDir) * -1.f, vNormal)) + (g_vLightAmbient * g_vMtrlAmbient));

	vector		vReflect = reflect(normalize(vLightDir), vNormal);

	vector		vLook = vWorldPos - g_vCamPosition;

	Out.vSpecular = fAtt * (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vLook) * -1.f, normalize(vReflect))), 50.f);

	return Out;
}

// SHADOW
float PCF_ShadowCaculation(float4 vLightPos, float3 vLightDir)
{
	float3 projCoords = vLightPos.xyz / vLightPos.w;

	projCoords.x = projCoords.x * 0.5f + 0.5f;
	projCoords.y = projCoords.y * -0.5f + 0.5f;

	float fCurrentDepth = projCoords.z;
	if (fCurrentDepth >= 1.f)
	{
		fCurrentDepth = 1.f;
		return fCurrentDepth;
	}

	float fShadow = 0.0f;
	float2 texelSize = float2(1.f / 1600.f, 1.f / 900.f);
	texelSize /= 3.f;

	[unroll]
	for (int x = -1; x <= 1; ++x)
	{
		[unroll]
		for (int y = -1; y <= 1; ++y)
		{
			float fPCFDepth = g_ShadowTarget.Sample(PointSampler, projCoords.xy + (float2(x, y) * texelSize)).r;
			fShadow += vLightPos.w - g_fBias > fPCFDepth * 1000.f ? 0.5f : 1.0f;
		}
	}
	fShadow /= 9.0f;

	return fShadow;
}

float4 Shadow_Caculation(PS_IN In)
{
	vector vWorldPos;
	vector vDepthDesc = g_DepthTarget.Sample(PointSampler, In.vTexcoord);
	float  fViewZ = vDepthDesc.y * 1000.f;

	/* 투영스페이스 상의 위치를 구한다. */
	vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
	vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	/* 뷰스페이스 상의 위치를 구한다. */
	vWorldPos = vWorldPos * fViewZ;
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드까지 가자. */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector	vLightPos = mul(vWorldPos, g_LightViewMatrix);
	float3 vLightDir = normalize(float3(0.f, 0.f, 0.f) - vLightPos.xyz);

	vLightPos = mul(vLightPos, g_CamProjMatrix);

	float fShadowColor = PCF_ShadowCaculation(vLightPos, vLightDir);

	return vector(fShadowColor, fShadowColor, fShadowColor, 1.f);
}

// DEFERRED
PS_OUT PS_MAIN_DEFERRED(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	// vDiffuse
	vector vDiffuse = g_DiffuseTarget.Sample(LinearSampler, In.vTexcoord);
	if (vDiffuse.a == 0.f)
		discard;

	// vShade
	vector vShade = g_ShadeTarget.Sample(LinearSampler, In.vTexcoord);
	vShade = saturate(vShade);
	vShade = ceil(vShade * 5.f) / 5.f;

	/* vSpecular
    vector vSpecular = g_SpecularTarget.Sample(LinearSampler, In.vTexcoord);
	vSpecular = saturate(vSpecular);
	vSpecular = ceil(vSpecular * 5.f) / 5.f;*/

	// Shadow
	vector vShadow = Shadow_Caculation(In);

    // SSAO
	vector vSSAO = g_SSAOTarget.Sample(LinearSampler, In.vTexcoord);

	// Outline
	vector vOutline = g_OutlineTarget.Sample(LinearSampler, In.vTexcoord);

	/* Fog
	float fFogFactor = saturate(((g_vFogStartEnd.y - (fViewZ)) / (g_vFogStartEnd.y - g_vFogStartEnd.x)));
	Out.vColor = fFogFactor * Out.vColor + (1.f - fFogFactor) * g_vFogColor;*/

	// Output
	Out.vColor = vDiffuse * vShade * vShadow * vSSAO * vOutline; // +vSpecular;

	return Out;
}

technique11 DefaultTechnique
{
	// 0
	pass Target_Debug
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	// 1
	pass Light_Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
	}

	// 2
	pass Light_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_OneBlend, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POINT();
	}

	// 3
	pass Deferred
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL; 
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED();
	}
}