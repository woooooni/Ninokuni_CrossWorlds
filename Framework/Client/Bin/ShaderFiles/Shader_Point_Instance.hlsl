
#include "Engine_Shader_Defines.hpp"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture[100];
Texture2D	    g_AlphaTexture[100];

vector			g_vCamPosition;

struct EffectDesc //16 배수로 나눠떨어져야함.
{
	float2   g_fUVIndex;
	float2   g_fMaxCount;

	float4   g_fColor;

	vector   g_vAxis;

	bool     g_bBillboard; //4
	float    g_fAngle;
	float    g_fAlpha;
	float    g_fTemp0;
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
	float4		vProjPos  : TEXCOORD1;

	uint	iInstanceID : SV_INSTANCEID;
};

[maxvertexcount(20)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
	GS_OUT		Out[4];

	// 회전 행렬 생성
	float3 vRight;
	float3 vUp;
	if (g_EffectDesc[In[0].iInstanceID].g_bBillboard)
	{
		vector		vLook = g_vCamPosition - In[0].vPosition;

		vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook.xyz)) * In[0].vPSize.x * 0.5f;
		vUp    = normalize(cross(vLook.xyz, vRight.xyz)) * In[0].vPSize.y * 0.5f;
	}
	else
	{
		float4 fAxis = g_EffectDesc[In[0].iInstanceID].g_vAxis;
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
	Out[0].vProjPos  = Out[0].vPosition;

	Out[1].vPosition = vector(In[0].vPosition.xyz - vRight + vUp, 1.f);
	Out[1].vPosition = mul(Out[1].vPosition, matVP);
	//Out[1].vTexcoord = float2(1.0f, 0.f);
	Out[1].vTexcoord = float2(
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.x + 1.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.x),
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.y + 0.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.y));
	Out[1].vProjPos = Out[1].vPosition;

	Out[2].vPosition = vector(In[0].vPosition.xyz - vRight - vUp, 1.f);
	Out[2].vPosition = mul(Out[2].vPosition, matVP);
	//Out[2].vTexcoord = float2(1.0f, 1.0f);
	Out[2].vTexcoord = float2(
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.x + 1.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.x),
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.y + 1.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.y));
	Out[2].vProjPos = Out[2].vPosition;

	Out[3].vPosition = vector(In[0].vPosition.xyz + vRight - vUp, 1.f);
	Out[3].vPosition = mul(Out[3].vPosition, matVP);
	//Out[3].vTexcoord = float2(0.0f, 1.0f);
	Out[3].vTexcoord = float2(
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.x + 0.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.x),
		((g_EffectDesc[In[0].iInstanceID].g_fUVIndex.y + 1.f) / g_EffectDesc[In[0].iInstanceID].g_fMaxCount.y));
	Out[3].vProjPos = Out[3].vPosition;

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
	float4		vProjPos  : TEXCOORD1;

	uint	iInstanceID : SV_INSTANCEID;
};

struct PS_OUT
{
	float4	vDiffuse : SV_TARGET0;
	float4	vBlurPower : SV_TARGET1;
	float4	vBrightness : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vDiffuse   = g_DiffuseTexture[0].Sample(PointSampler, In.vTexcoord);
	Out.vDiffuse.r = saturate((Out.vDiffuse.r + g_EffectDesc[In.iInstanceID].g_fColor.r));
	Out.vDiffuse.g = saturate((Out.vDiffuse.g + g_EffectDesc[In.iInstanceID].g_fColor.g));
	Out.vDiffuse.b = saturate((Out.vDiffuse.b + g_EffectDesc[In.iInstanceID].g_fColor.b));
	//Out.vDiffuse.a = saturate(Out.vDiffuse.a - g_EffectDesc[In.iInstanceID].g_fAlpha);

	if (Out.vDiffuse.a < 0.5f)
		discard;

	// 알파 채널 반전
	//Out.vDiffuse.a = 1.f - Out.vDiffuse.a;
	if (Out.vDiffuse.r < 0.5f && Out.vDiffuse.g < 0.5f && Out.vDiffuse.b < 0.5f)
		discard;

	Out.vBlurPower  = float4(1.f, 1.f, 1.f, 0.f);
	Out.vBrightness = Out.vDiffuse;

	return Out;
}

technique11 DefaultTechnique
{
	pass Particle
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}




