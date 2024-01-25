
#include "Engine_Shader_Defines.hpp"
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DiffuseTexture;
Texture2D	g_NormalTexture;
float4		g_vCamPosition;

Texture2D		g_DissolveTexture;
float			g_fDissolveWeight;
float4			g_vDissolveColor = { 0.6f, 0.039f, 0.039f, 1.f };

//
Texture2D GrassMaskTexture;

float4 g_vClipPlane;

float fGrassAngle;

cbuffer InversTransposeMatBuffer
{
    matrix WorldInvTransposeView;
};


void ComputeNormalMapping(inout float3 normal, float3 tangent, float2 texcoord)
{
	// [0,255] 범위에서 [0,1]로 변환
    float4 map = g_NormalTexture.Sample(LinearSampler, texcoord);
    if (any(map.rgb) == false)
        return;

    float3 N = normalize(normal.xyz); // z
    float3 T = normalize(tangent); // x
    float3 B = normalize(cross(N, T)); // y
    float3x3 TBN = float3x3(T, B, N); // TS -> WS

	// [0,1] 범위에서 [-1,1] 범위로 변환
    float3 tangentSpaceNormal = (map.rgb * 2.0f - 1.0f);
    float3 worldNormal = mul(tangentSpaceNormal, TBN);

    normal = float4(worldNormal, 0.f);
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
	uint		iInstanceID : SV_INSTANCEID;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vWorldPosition : TEXCOORD2;
	uint		iInstanceID : SV_INSTANCEID;
	
    float3 vViewNormal : NORMAL1;
    float3 vPositionView : POSITION;
};

struct VS_REFRACT_OUT
{
    float4 vPosition : SV_POSITION; //
    float3 vNormal : NORMAL; // 
    float2 vTexcoord : TEXCOORD0; // 
    float4 vWorldPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float4 vProjPos : TEXCOORD2;
    
	float3 vBinormal : BINORMAL;
    uint iInstanceID : SV_INSTANCEID;
	
    float fClip : SV_ClipDistance0;
};

struct VS_REFLECT_OUT
{
    float4 vPosition : SV_POSITION; //
    float3 vNormal : NORMAL; // 
    float2 vTexcoord : TEXCOORD0; // 
    float4 vWorldPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float4 vProjPos : TEXCOORD2;
    
    float3 vBinormal : BINORMAL;
    uint iInstanceID : SV_INSTANCEID;
	
    float fClip : SV_ClipDistance0;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP, matVP;
	float4x4	InstanceWorld = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);



	matWV = mul(InstanceWorld, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vWorldPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), InstanceWorld)).xyz;
	Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), InstanceWorld)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.iInstanceID = In.iInstanceID;

		// SSAO
    Out.vViewNormal = mul(Out.vNormal.xyz, (float3x3) g_ViewMatrix);
    Out.vPositionView = mul(float4(In.vPosition, 1.0f), matWV);
	
	return Out;
}

VS_OUT VS_GRASS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP, matVP;
    float4x4 InstanceWorld = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	
	if(In.vTexUV.y <= 0.5f)
    {
        In.vPosition.z += fGrassAngle * 0.5f;
    }
	
    matWV = mul(InstanceWorld, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
	
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vWorldPosition = mul(float4(In.vPosition, 1.0f), g_WorldMatrix);
	
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), InstanceWorld)).xyz;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), InstanceWorld)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;
    Out.iInstanceID = In.iInstanceID;

			// SSAO
    Out.vViewNormal = mul(Out.vNormal.xyz, (float3x3) g_ViewMatrix);
    Out.vPositionView = mul(float4(In.vPosition, 1.0f), matWV);
    return Out;
}

// 굴절
VS_REFRACT_OUT VS_REFRACT_MAIN(VS_IN In)
{
    VS_REFRACT_OUT Out = (VS_REFRACT_OUT) 0;

    matrix matWV, matWVP, matVP;
    float4x4 InstanceWorld = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);



    matWV = mul(InstanceWorld, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), InstanceWorld)).xyz;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), InstanceWorld)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
    Out.vTexcoord = In.vTexUV;
    Out.vProjPos = Out.vPosition;
    Out.iInstanceID = In.iInstanceID;

    Out.fClip = dot(mul(float4(In.vPosition, 1.0f), InstanceWorld), g_vClipPlane);
	
    return Out;
}

// 반사
VS_REFLECT_OUT VS_REFLECT_MAIN(VS_IN In)
{
    VS_REFLECT_OUT Out = (VS_REFLECT_OUT) 0;

    matrix matWV, matWVP, matVP;
    float4x4 InstanceWorld = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);



    matWV = mul(InstanceWorld, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), InstanceWorld)).xyz;
    Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), InstanceWorld)).xyz;
    Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
    Out.vTexcoord = In.vTexUV;
    Out.vProjPos = Out.vPosition;
    Out.iInstanceID = In.iInstanceID;

    Out.fClip = dot(mul(float4(In.vPosition, 1.0f), InstanceWorld), g_vClipPlane);
	
    return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vWorldPosition : TEXCOORD2;
	uint		iInstanceID : SV_INSTANCEID;
	
    float3 vViewNormal : NORMAL1;
    float3 vPositionView : POSITION;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
    float4		vBloom : SV_TARGET3;
    float4		vViewNormal : SV_TARGET4;
};

struct PS_OUT_SHADOW_DEPTH
{
	float4		vDepth : SV_TARGET0;
};

struct PS_REFRACT_IN
{
    float4 vPosition : SV_POSITION; //
    float3 vNormal : NORMAL; // 
    float2 vTexcoord : TEXCOORD0; // 
    float4 vWorldPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float4 vProjPos : TEXCOORD2;
    
    float3 vBinormal : BINORMAL;
    uint iInstanceID : SV_INSTANCEID;
	
    float fClip : SV_ClipDistance0;
};

struct PS_REFLECT_IN
{
    float4 vPosition : SV_POSITION; //
    float3 vNormal : NORMAL; // 
    float2 vTexcoord : TEXCOORD0; // 
    float4 vWorldPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float4 vProjPos : TEXCOORD2;
    
    float3 vBinormal : BINORMAL;
    uint iInstanceID : SV_INSTANCEID;
	
    float fClip : SV_ClipDistance0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = (vector)1.f;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);
	if (0.3 >= Out.vDiffuse.a)
		discard;

	return Out;	
}

PS_OUT PS_NoneRight_GRASS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vDiffuse = (vector) 1.f;

    Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);


    Out.vNormal = vector(1.0f,1.0f,1.0f, 0.0f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);
    if (0.8 >= Out.vDiffuse.a)
        discard;

    return Out;
}

PS_OUT PS_MAIN_NORMAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	vector		vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	float3		vNormal = vNormalDesc.xyz * 2.f - 1.f;
	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);
	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);
	if (0.5 >= Out.vDiffuse.a)
		discard;

	return Out;
}

PS_OUT PS_DISSOLVE_DEAD(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vDissolve = g_DissolveTexture.Sample(ModelSampler, In.vTexUV);

	if (vDissolve.r <= g_fDissolveWeight)
		discard;

	if ((vDissolve.r - g_fDissolveWeight) < 0.1f)
		Out.vDiffuse = g_vDissolveColor;
	else if ((vDissolve.r - g_fDissolveWeight) < 0.115f)
		Out.vDiffuse = g_vDissolveColor - 0.1f;
	else if ((vDissolve.r - g_fDissolveWeight) < 0.125f)
		Out.vDiffuse = g_vDissolveColor - 0.1f;
	else
		Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);



	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
    Out.vViewNormal = float4(normalize(In.vViewNormal), In.vPositionView.z);
	if (0 == Out.vDiffuse.a)
		discard;

	return Out;
}

PS_OUT PS_REFRACT_MAIN(PS_REFRACT_IN In)
{
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexcoord);
	
    PS_OUT Out = (PS_OUT) 0;

    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
	if(vMtrlDiffuse.a < 0.3f)
        discard;
	
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.0f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.0f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
	
    return Out;
}

PS_OUT PS_REFLECT_MAIN(PS_REFLECT_IN In)
{
    ComputeNormalMapping(In.vNormal, In.vTangent, In.vTexcoord);
	
    PS_OUT Out = (PS_OUT) 0;

    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
	
    if (vMtrlDiffuse.a < 0.3f)
        discard;
	
    Out.vDiffuse = vMtrlDiffuse;
    Out.vNormal = float4(In.vNormal.xyz * 0.5f + 0.5f, 0.0f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.0f, 0.0f, 0.0f);
    Out.vBloom = vector(0.0f, 0.0f, 0.0f, 0.0f);
	
    return Out;
}


// 그림자 픽셀 쉐이더
PS_OUT_SHADOW_DEPTH PS_SHADOW_DEPTH(PS_IN In)
{
	PS_OUT_SHADOW_DEPTH		Out = (PS_OUT_SHADOW_DEPTH)0;


	vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);;
	if (vColor.a <= 0.3f)
		discard;


	Out.vDepth = vector(In.vProjPos.w / 1000.0f, 0.f, 0.f, 1.f);

	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultPass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass NormalPass
	{
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}

	pass DissolveDead
	{
		// 2
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DISSOLVE_DEAD();
	}

	pass Refract // 굴절
	{
		// 3
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Reflect // 반사
	{
		// 4
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass RealTimeGrass // Grass
	{
		// 5
		SetRasterizerState(RS_NoneCull);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_GRASS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_NoneRight_GRASS_MAIN();
    }

	pass RealTimeGrass2
	{
		// 6
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_GRASS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

	pass Temp7
	{
		// 7
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp8
	{
		// 8
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp9
	{
		// 9
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Shadow_Depth
	{
		// 10
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Shadow, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW_DEPTH();
	}
}