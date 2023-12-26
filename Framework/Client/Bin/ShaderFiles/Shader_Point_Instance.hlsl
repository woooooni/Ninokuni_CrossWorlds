
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D	g_DiffuseTexture;
Texture2D	g_AlphaTexture;

vector		g_vCamPosition;

bool        g_bBillboard;
float       g_fAlpha_Discard;
float3      g_fBlack_Discard;

struct EffectDesc //16 배수로 나눠떨어져야함.
{
	float2   g_fUVIndex;  // 8
	float2   g_fMaxCount; // 8

	float3   g_fColor; // 12
	float    g_fAlpha; // 4

	float3   g_fAxis;  // 12
	float    g_fAngle; // 4

	float3   g_fBlurColor; // 12
	float    g_fBlurPower; // 4
};
EffectDesc g_EffectDesc[1000]; // 8096

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vPSize    : PSIZE;

	float4		vRight : WORLD0;
	float4		vUp    : WORLD1;
	float4		vLook  : WORLD2;
	float4		vTranslation : WORLD3;

	uint	    iInstanceID : SV_INSTANCEID;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;

	uint	    iInstanceID : SV_INSTANCEID;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;	

	matrix		TransformMatrix = float4x4(normalize(In.vRight), normalize(In.vUp), normalize(In.vLook), In.vTranslation);	

	vector		vPosition = mul(float4(In.vPosition, 1.f), TransformMatrix);

	Out.vPosition = mul(vPosition, g_WorldMatrix);
	Out.vPSize    = float2(In.vPSize.x * length(In.vRight), In.vPSize.y * length(In.vUp));

	Out.iInstanceID = In.iInstanceID;

	return Out;	
}

struct GS_IN
{	
	float4		vPosition : POSITION;
	float2		vPSize : PSIZE;

	uint	iInstanceID : SV_INSTANCEID;
};

struct GS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	//float4		vProjPos  : TEXCOORD1;

	uint	iInstanceID : SV_INSTANCEID;
};

[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];

	// 회전 행렬 생성
	float3 vRight;
	float3 vUp;
	if (g_bBillboard)
	{
		vector		vLook = g_vCamPosition - In[0].vPosition;

		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vPSize.x * 0.5f;
		vUp    = normalize(cross(vLook.xyz, vRight.xyz)) * In[0].vPSize.y * 0.5f;
	}
	else
	{
		float3 fAxis = g_EffectDesc[In[0].iInstanceID].g_fAxis;
		float fAngle = radians(g_EffectDesc[In[0].iInstanceID].g_fAngle);

		float3x3 RotationMatrix = float3x3(
			fAxis.x * fAxis.x * (1.0 - cos(fAngle)) + cos(fAngle), fAxis.x * fAxis.y * (1.0 - cos(fAngle)) + fAxis.z * sin(fAngle), fAxis.x * fAxis.z * (1.0 - cos(fAngle)) - fAxis.y * sin(fAngle),
			fAxis.x * fAxis.y * (1.0 - cos(fAngle)) - fAxis.z * sin(fAngle), fAxis.y * fAxis.y * (1.0 - cos(fAngle)) + cos(fAngle), fAxis.y * fAxis.z * (1.0 - cos(fAngle)) + fAxis.x * sin(fAngle),
			fAxis.x * fAxis.z * (1.0 - cos(fAngle)) + fAxis.y * sin(fAngle), fAxis.y * fAxis.z * (1.0 - cos(fAngle)) - fAxis.x * sin(fAngle), fAxis.z * fAxis.z * (1.0 - cos(fAngle)) + cos(fAngle));

		vRight = mul(float4(float3(1.0f, 0.0f, 0.0f), 0), RotationMatrix).xyz * In[0].vPSize.x;
		vUp    = mul(float4(float3(0.0f, 1.0f, 0.0f), 0), RotationMatrix).xyz * In[0].vPSize.y;
	}


	matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Out[0].vPosition = vector(In[0].vPosition.xyz + vRight + vUp, 1.f);
	Out[0].vPosition = mul(Out[0].vPosition, matVP);
	//Out[0].vTexcoord = float2(0.0f, 0.f);
	Out[0].vTexcoord = float2(
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.x + 0.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.x),
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.y + 0.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.y));
	//Out[0].vProjPos  = Out[0].vPosition;

	Out[1].vPosition = vector(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	//Out[1].vTexcoord = float2(1.0f, 0.f);
	Out[1].vTexcoord = float2(
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.x + 1.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.x),
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.y + 0.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.y));
	//Out[1].vProjPos = Out[1].vPosition;

	Out[2].vPosition = vector(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	//Out[2].vTexcoord = float2(1.0f, 1.0f);
	Out[2].vTexcoord = float2(
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.x + 1.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.x),
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.y + 1.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.y));
	//Out[2].vProjPos = Out[2].vPosition;

	Out[3].vPosition = vector(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	//Out[3].vTexcoord = float2(0.0f, 1.0f);
	Out[3].vTexcoord = float2(
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.x + 0.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.x),
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.y + 1.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.y));
	//Out[3].vProjPos = Out[3].vPosition;

	Out[0].iInstanceID = In[0].iInstanceID;
	Out[1].iInstanceID = In[0].iInstanceID;
	Out[2].iInstanceID = In[0].iInstanceID;
	Out[3].iInstanceID = In[0].iInstanceID;

	OutStream.Append(Out[0]);
	OutStream.Append(Out[1]);
	OutStream.Append(Out[2]);
	OutStream.RestartStrip();

	OutStream.Append(Out[0]);
	OutStream.Append(Out[2]);
	OutStream.Append(Out[3]);
	OutStream.RestartStrip();
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexcoord : TEXCOORD0;
	//float4		vProjPos  : TEXCOORD1;

	uint	iInstanceID : SV_INSTANCEID;
};

struct PS_OUT
{
	float4 vDiffuse  : SV_TARGET0;

	float4 vDiffuse1 : SV_TARGET1;
	float4 vDiffuse2 : SV_TARGET2;
	float4 vDiffuse3 : SV_TARGET3;
	float4 vDiffuse4 : SV_TARGET4;
	float4 vDiffuse5 : SV_TARGET5;

	float4 vBloom : SV_TARGET6;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector vDiffuseColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	if (vDiffuseColor.a < g_fAlpha_Discard ||
		vDiffuseColor.r < g_fBlack_Discard.r && vDiffuseColor.g < g_fBlack_Discard.g && vDiffuseColor.b < g_fBlack_Discard.b)
		discard;
	vDiffuseColor.rgb = saturate((vDiffuseColor.rgb + g_EffectDesc[In.iInstanceID].g_fColor.rgb));
	vDiffuseColor.a = saturate(vDiffuseColor.a - g_EffectDesc[In.iInstanceID].g_fAlpha);

	Out.vDiffuse  = vDiffuseColor;
	Out.vDiffuse1 = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse2 = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse3 = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse4 = float4(0.f, 0.f, 0.f, 0.f);
	Out.vDiffuse5 = float4(0.f, 0.f, 0.f, 0.f);
	if (g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.2f)
		Out.vDiffuse1 = vDiffuseColor;
	else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.2f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.4f)
		Out.vDiffuse2 = vDiffuseColor;
	//else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.4f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.6f)
	else
		Out.vDiffuse3 = vDiffuseColor;
	//else if (g_EffectDesc[In.iInstanceID].g_fBlurPower > 0.6f && g_EffectDesc[In.iInstanceID].g_fBlurPower <= 0.8f)
	//	Out.vDiffuse4 = vDiffuseColor;
	//else
	//	Out.vDiffuse5 = vDiffuseColor;

	Out.vBloom = float4(1.f, 1.f, 1.f, 1.f);

	return Out;
}

struct PS_OUT_BLEND
{
	float4	vColor : SV_TARGET0;
};

PS_OUT_BLEND PS_MAIN_BLEND(PS_IN In)
{
	PS_OUT_BLEND Out = (PS_OUT_BLEND)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);

	Out.vColor.rgb = saturate((Out.vColor.rgb + g_EffectDesc[In.iInstanceID].g_fColor.rgb));
	Out.vColor.a   = saturate(Out.vColor.a - g_EffectDesc[In.iInstanceID].g_fAlpha);

	// 알파 채널 반전
	//Out.vDiffuse.a = 1.f - Out.vDiffuse.a;

	return Out;
}

technique11 DefaultTechnique
{
	// UI
	pass Particle_UI_Basic_Default
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass Particle_UI_Basic_Blend
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		
		VertexShader = compile   vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}

	// 기본
	pass Particle_Basic_Default
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass Particle_Basic_Blend
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile   vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND();
	}
}