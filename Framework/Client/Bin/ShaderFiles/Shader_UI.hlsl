#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D	g_DiffuseTexture;
Texture2D	g_AlphaTexture;
Texture2D	g_DepthTexture;

Texture2D	g_FXTexture;
Texture2D	g_GlowTexture;
Texture2D	g_MaskTexture;
Texture2D	g_DissolveTexture;

Texture2D	g_LerpTexture;
Texture2D	g_HPGaugeTexture;

float g_Alpha = 1.f;
float g_Ratio;
float g_Time;
float g_LimitX;
float g_LimitY;
float g_FXAlpha;
float g_SinValue;

float g_LoadingProgress;
float4 g_Diffusecolor;

float g_MaxHP;
float g_CurrentHP;
float g_LerpHP;

float2	g_DissolveStart;
float	g_DissolveRange;


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	
	return Out;
}

VS_OUT VS_MAIN_CLOUD(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float fSpeed = 0.005f;

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV + float2(g_Time * fSpeed, 0);

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

//	if (0.f >= Out.vColor.a)
//		discard;
	if (0.0001f >= Out.vColor.a)
		discard;

	return Out;	
}

PS_OUT PS_MAIN_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a *= g_Alpha;

	if (0.0001f >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_MAIN_FADE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a *= g_Alpha;

	return Out;
}

PS_OUT PS_MAIN_CLOUD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a *= g_Alpha;
//	if (Out.vColor.r <= 0.2f && Out.vColor.g <= 0.2f && Out.vColor.b <= 0.2f)
//		discard;

	return Out;
}

PS_OUT PS_MAIN_LOADING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float fLoadingPer = 1.f;

	if (In.vTexUV.x < g_LoadingProgress || In.vTexUV.x >(g_LoadingProgress + fLoadingPer))
	{
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	}
	else
	{
		discard;
		//Out.vColor = float4(0.0, 0.0, 0.0, 0.0);
	}

	return Out;
}

PS_OUT PS_USING_MASK(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV); // Diffuse Tex Sampling
	float4 vMaskColor = g_MaskTexture.Sample(LinearSampler, In.vTexUV); // Mask Tex Sampling

	if (vMaskColor.r > 0.9f && vMaskColor.g > 0.9f && vMaskColor.b > 0.9f)
	{
		Out.vColor = saturate(vColor);
		if (Out.vColor.a < 0.1f)
			discard;
	}
	else
		discard;

	return Out;
}

PS_OUT PS_CIRCLE_MASK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0; // 초기화

	float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV); // Diffuse Tex Sampling
	float4 vMaskColor = g_MaskTexture.Sample(LinearSampler, In.vTexUV); // Mask Tex Sampling

	if (vMaskColor.r > 0.9f && vMaskColor.g > 0.9f && vMaskColor.b > 0.9f)
	{
		Out.vColor = saturate(vColor);
		if (Out.vColor.a < 0.1f)
			discard;
	}
	else
		discard;

	// 여기까지 마스크를 씌운 상태

	float2 vDir = In.vTexUV - float2(0.5f, 0.5f); // float2(0.5f, 0.5f)는 중점이다.
	vDir = normalize(vDir); // 방향벡터 Normalize
	float2 vUpDir = float2(0.0f, sign(vDir.x));
	vUpDir = normalize(vUpDir);

	float fDot = dot(vUpDir, vDir); // 두 벡터를 내적한다.
	float fDotRatio = g_Ratio;

	// 방향벡터가 음수인 경우, 비교할 기준 벡터의 방향은 위
	if (vDir.x < 0.f)
	{
		fDotRatio -= 0.5f;
	}

	fDotRatio = fDotRatio * 4.f - 1.f;

	if (fDotRatio < fDot) // 잔여 쿨타임이 직관적으로 보여지는 픽셀이다
	{
		Out.vColor.rgb = lerp(vColor.rgb, float3(0.0f, 0.0f, 0.0f), 0.5f);
		Out.vColor.a = 1.0f;
	}

	// 특정 영역에서만 온전한 원본 이미지가 표시된다.
	return Out;
}

PS_OUT PS_USING_FX(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0; // 초기화

	float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vColor.a *= g_Alpha;

	float4 vfxColor = g_FXTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor = lerp(vColor, vfxColor, vfxColor.a);
	// 세번째 값은 가중치이다.

	if (0.0001f >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_MAIN_APPEAR(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float fLoadingPer = 1.f;

	if (In.vTexUV.x < g_LoadingProgress || In.vTexUV.x >(g_LoadingProgress + fLoadingPer))
	{
		Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
		Out.vColor.a *= g_Alpha;
	}
	else
	{
		//discard;
		Out.vColor.a * 0.1f;
	}

	return Out;
}

PS_OUT PS_MAIN_LIMITED_APPEAR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a *= g_Alpha;

	float fX = In.vPosition.x / In.vPosition.w;
	//g_LimitX
	//if (fX > 1500.0f)
	if (fX > g_LimitX)
		discard;

	return Out;
}

PS_OUT PS_MAIN_FILL_COLOR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vOriginColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	//g_Diffusecolor -> 곱할 색상값
	Out.vColor = vOriginColor * g_Diffusecolor;
	Out.vColor.a *= g_Alpha;

	if (0.0001f >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_MAIN_HPBAR_GAUGE_LERP(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vLerpColor = g_LerpTexture.Sample(LinearSampler, In.vTexUV);
	float4 vGaugeColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (g_LerpHP / g_MaxHP < In.vTexUV.x) // 현재 체력과 최대 체력의 비에 따라 UV좌표가 잘린다.
		discard;

	if (g_LerpHP / g_MaxHP >= In.vTexUV.x && g_CurrentHP / g_MaxHP <= In.vTexUV.x)
		Out.vColor = vLerpColor;

	if (g_CurrentHP / g_MaxHP > In.vTexUV.x)
		Out.vColor = vGaugeColor;

	//if (vGaugeColor.a < 0.3f)
	//	discard;
	//Out.vColor = lerp(vLerpColor, vGaugeColor, vGaugeColor.a);

	return Out;
}

PS_OUT PS_MAIN_LIMITED_VERTICAL(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	Out.vColor.a *= g_Alpha;

	float fY = In.vPosition.y / In.vPosition.w;


	if (fY > g_LimitY)
		discard;

	return Out;
}

PS_OUT PS_USING_FX_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0; // 초기화

	float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vColor.a *= g_Alpha;

	float4 vfxColor = g_FXTexture.Sample(LinearSampler, In.vTexUV);
	vfxColor.a *= g_FXAlpha;
	Out.vColor = lerp(vColor, vfxColor, vfxColor.a);

	if (0.0001f >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_HPBAR_GAUGE(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	float4 vBackColor = g_HPGaugeTexture.Sample(LinearSampler, In.vTexUV);

	if (g_CurrentHP / g_MaxHP < In.vTexUV.x) // 현재 체력과 최대 체력의 비에 따라 UV좌표가 잘린다.
	{
		Out.vColor = vBackColor;
		return Out;
	}

	float4 vGaugeColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (vGaugeColor.a < 0.3f)
		discard;

	Out.vColor = lerp(vBackColor, vGaugeColor, vGaugeColor.a);

	return Out;
}

PS_OUT PS_MAIN_GRAY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float3 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV).rgb;
	float vGrayScale = dot(vColor, float3(0.299, 0.587, 0.114)); // 그레이 스케일로 변환하는데 사용하는 가중치
	// RGB값과 그레이스케일을 내적한다. 0.299×R+0.587×G+0.114×B -> RGB를 같은값으로 넣으면 흑백으로 출력된다.
	Out.vColor = float4(vGrayScale, vGrayScale, vGrayScale, g_DiffuseTexture.Sample(LinearSampler, In.vTexUV).a);

	if (0.0001f >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_MAIN_HPBAR_LERP_AND_GLOW(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4 vLerpColor = g_LerpTexture.Sample(LinearSampler, In.vTexUV);
	float4 vGaugeColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	float4 vFXColor = g_GlowTexture.Sample(LinearSampler, float2(In.vTexUV.x + g_Time, In.vTexUV.y));

	if (g_LerpHP / g_MaxHP < In.vTexUV.x) // 현재 체력과 최대 체력의 비에 따라 UV좌표가 잘린다.
		discard;

	if (g_LerpHP / g_MaxHP >= In.vTexUV.x && g_CurrentHP / g_MaxHP <= In.vTexUV.x)
		Out.vColor = vLerpColor;
	else if (g_CurrentHP / g_MaxHP > In.vTexUV.x)
	{
		if (0.1f > vFXColor.a)
			Out.vColor = vGaugeColor;
		else
		{
			if (0.001f > vGaugeColor.a)
			{
				Out.vColor.a = 0.f;
			}
			else
			{
				Out.vColor = vFXColor;
			}
		}
	}

	return Out;
}

PS_OUT PS_MAIN_DISSOLVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

//	float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
//	float4 vDissolve = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
//
//	if (vColor.a == 0.f)
//		clip(-1);
//
//	if (vDissolve.r >= g_SinValue)
//		vColor.a = 1;
//	else
//		vColor.a = 0;
//
//	if (vDissolve.r >= g_SinValue - 0.05f && vDissolve.r <= g_SinValue + 0.05f)
//		vColor = float4(0.f, 0.9f, 1.f, 1.f);
//	else
//		;
//
//	if (vDissolve.r >= g_SinValue - 0.03f && vDissolve.r <= g_SinValue + 0.03f)
//		vColor = float4(0.502f, 0.949f, 1.f, 1.f);
//	else
//		;
//
//	if (vDissolve.r >= g_SinValue - 0.025 && vDissolve.r <= g_SinValue + 0.025)
//		vColor = float4(1.f, 1.f, 1.f, 1.f);
//	else
//		;
//
//	Out.vColor = vColor;

	vector vMtrlDissolve = g_DissolveTexture.Sample(LinearSampler, In.vTexUV * 3.f);
	float fDissolveAlpha = saturate(1.0 - g_Time / 5.f + vMtrlDissolve.r);
	//float fDissolveAlpha = saturate(1.0 - g_Time / g_DissolveTotalTime + vMtrlDissolve.r);

	if (fDissolveAlpha < 0.3)
		discard;

	if (fDissolveAlpha < 0.5)
	{
		Out.vColor = float4(1.f, 0.2f, 0.2f, 1.f);
	}

	return Out;
}

PS_OUT PS_SKILLGAUGE_MASK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0; // 초기화

	float4 vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV); // Diffuse Tex Sampling
	float4 vMaskColor = g_MaskTexture.Sample(LinearSampler, In.vTexUV); // Mask Tex Sampling

	if (vMaskColor.r > 0.9f && vMaskColor.g > 0.9f && vMaskColor.b > 0.9f)
	{
		Out.vColor = saturate(vColor);
		if (Out.vColor.a < 0.1f)
			discard;
	}
	else
		discard;

	// 여기까지 마스크를 씌운 상태

	float2 vDir = In.vTexUV - float2(0.5f, 0.5f); // float2(0.5f, 0.5f)는 중점이다.
	vDir = normalize(vDir); // 방향벡터 Normalize
	float2 vUpDir = float2(0.0f, sign(vDir.x));
	vUpDir = normalize(vUpDir);

	float fDot = dot(vUpDir, vDir); // 두 벡터를 내적한다.
	float fDotRatio = g_Ratio;

	// 방향벡터가 음수인 경우, 비교할 기준 벡터의 방향은 위
	if (vDir.x < 0.f)
	{
		fDotRatio -= 0.5f;
	}

	fDotRatio = fDotRatio * 4.f - 1.f;

	if (fDotRatio < fDot) // 잔여 쿨타임이 직관적으로 보여지는 픽셀이다
	{
		//Out.vColor.rgb = lerp(vColor.rgb, float3(0.0f, 0.0f, 0.0f), 0.5f);
		Out.vColor.a = 0.f;
	}

	// 특정 영역에서만 온전한 원본 이미지가 표시된다.
	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultPass // 0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass AlphaBlendPass // 1
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ALPHA();
	}

	pass NoDiscardPass // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FADE();
	}

	pass CloudPass // 3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_CLOUD();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CLOUD();
	}

	pass LoadingPass // 4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LOADING();
	}

	pass UsingMaskTexture // 5
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_USING_MASK();
	}

	pass CircleMaskTexture // 6
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CIRCLE_MASK();
	}

	pass UsingFXTexture // 7
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_USING_FX();
	}

	pass AppearTexture // 8
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_APPEAR();
	}

	pass LimitedHorizonTexture // 9 -> X부분을 잘라줌
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIMITED_APPEAR();
	}

	pass FillColor // 10
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FILL_COLOR();
	}

	pass HPBarGauge_Lerp // 11
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HPBAR_GAUGE_LERP();
	}

	pass LimitedVerticalTexture // 12 -> Y부분을 잘라줌
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIMITED_VERTICAL();
	}

	pass UsingFXTextureWithAlpha // 13
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_USING_FX_ALPHA();
	}

	pass HPBarGauge // 14
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_HPBAR_GAUGE();
	}

	pass NoneCull // 15
	{
		SetRasterizerState(RS_Sky);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ALPHA();
	}

	pass GrayScale // 16
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_GRAY();
	}

	pass HPBarGauge_Lerp_WithGlow // 17
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HPBAR_LERP_AND_GLOW();
	}

	pass HPBar_Dissolve // 18 사용 안함
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISSOLVE();
	}

	pass SkillGaugeMaskTexture // 19
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(1.f, 1.f, 1.f, 1.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SKILLGAUGE_MASK();
	}
}


/*


PS_OUT PS_REVERSE_OPACTITY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vAlphaColor = g_AlphaTexture.Sample(LinearSampler, In.vTexUV);

	if (vAlphaColor.r >= 0.2f)
		discard;

	Out.vColor = vector(vAlphaColor.rgb, 1.f - vAlphaColor.r);

	if (0.f >= Out.vColor.a)
		discard;

	return Out;
}

PS_OUT PS_USING_ALPHA_VALUE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vTextureColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if (vTextureColor.a == 0.f || g_fAlpha == 0.f)
		discard;

	vTextureColor.a = g_fAlpha;
	Out.vColor = vTextureColor;

	if (0.f >= Out.vColor.a)
		discard;

	return Out;

}

PS_OUT PS_OPACTITY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_AlphaTexture.Sample(LinearSampler, In.vTexUV);
	if (0.f >= Out.vColor.a)
		discard;

	return Out;
}

	pass ReverseOpacityPass // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_REVERSE_OPACTITY();
	}

	pass UsingAlphaValue // 3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_USING_ALPHA_VALUE();
	}

		pass OpacityPass // 6
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_OPACTITY();
	}

*/