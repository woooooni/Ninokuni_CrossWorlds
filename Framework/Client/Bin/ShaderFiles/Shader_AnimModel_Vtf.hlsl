#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D		g_DiffuseTexture;
Texture2D		g_NormalTexture;

Texture2D		g_MatrixPallete;
Texture2D		g_DissolveTexture;
float			g_fDissolveWeight;

float4			g_vDissolveColor = { 0.6f, 0.039f, 0.039f, 1.f };
float4			g_vRimColor = { 0.f, 0.f, 0.f, 0.f };
float4			g_vCamPosition;

struct KeyframeDesc
{
	int iAnimIndex;
	uint iCurFrame;
	uint iNextFrame;
	float fRatio;
	float fFrameAcc;

	uint	iFinish;
	uint	iFix;
	uint	iStop;
};

struct TweenFrameDesc
{
	KeyframeDesc cur;
	KeyframeDesc next;

	float fTweenDuration;
	float fTweenRatio;
	float fTweenAcc;
	float fPadding;
};

TweenFrameDesc  g_TweenFrames;
Texture2DArray  g_TransformMap;

struct VS_IN
{
	float3		vPosition		: POSITION;
	float3		vNormal			: NORMAL;
	float2		vTexUV			: TEXCOORD0;
	float3		vTangent		: TANGENT;
	uint4		vBlendIndex		: BLENDINDEX;
	float4		vBlendWeight	: BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition		: SV_POSITION0;
	float4		vNormal			: NORMAL;
	float2		vTexUV			: TEXCOORD0;
	float3		vTangent		: TANGENT;
	float3		vBinormal		: BINORMAL;
	float4		vProjPos		: TEXCOORD1;
	float4		vWorldPosition	: TEXCOORD2;
};

matrix GetAnimationMatrix(VS_IN input)
{
	float indices[4] = { input.vBlendIndex.x, input.vBlendIndex.y, input.vBlendIndex.z, input.vBlendIndex.w };
	float weights[4] = { input.vBlendWeight.x, input.vBlendWeight.y, input.vBlendWeight.z, input.vBlendWeight.w };

	int animIndex[2];
	int currFrame[2];
	int nextFrame[2];
	float ratio[2];

	/* cur */
	animIndex[0]	= g_TweenFrames.cur.iAnimIndex;
	currFrame[0]	= g_TweenFrames.cur.iCurFrame;
	nextFrame[0]	= g_TweenFrames.cur.iNextFrame;
	ratio[0]		= g_TweenFrames.cur.fRatio;

	/* next */
	animIndex[1]	= g_TweenFrames.next.iAnimIndex;
	currFrame[1]	= g_TweenFrames.next.iCurFrame;
	nextFrame[1]	= g_TweenFrames.next.iNextFrame;
	ratio[1]		= g_TweenFrames.next.fRatio;

	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	matrix curr = 0;
	matrix next = 0;
	matrix transform = 0;

	for (int i = 0; i < 4; i++)
	{
		/* cur */
		c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
		c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
		c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
		c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
		curr = matrix(c0, c1, c2, c3);

		n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
		n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
		n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
		n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
		next = matrix(n0, n1, n2, n3);

		matrix result = lerp(curr, next, ratio[0]);

		/* if next */
		if (animIndex[1] >= 0)
		{
			c0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
			c1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
			c2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
			c3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
			curr = matrix(c0, c1, c2, c3);

			n0 = g_TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
			n1 = g_TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
			n2 = g_TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
			n3 = g_TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
			next = matrix(n0, n1, n2, n3);

			matrix nextResult = lerp(curr, next, ratio[1]);

			result = lerp(result, nextResult, g_TweenFrames.fTweenRatio);
		}

		transform += mul(weights[i], result);
	}

	return transform;
}

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix BoneMatrix	= GetAnimationMatrix(In);
	vector vPosition	= mul(vector(In.vPosition, 1.f), BoneMatrix);

	matrix matWV	= mul(g_WorldMatrix, g_ViewMatrix);
	matrix matWVP	= mul(matWV, g_ProjMatrix);

	vector vNormal	= mul(vector(In.vNormal, 0.f), BoneMatrix);
	vector vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);

	Out.vPosition		= mul(vPosition, matWVP);
	Out.vWorldPosition	= mul(vPosition, g_WorldMatrix);
	Out.vNormal			= normalize(mul(vNormal, g_WorldMatrix));
	Out.vTangent		= normalize(mul(vTangent, g_WorldMatrix)).xyz;
	Out.vBinormal		= normalize(cross(Out.vNormal, Out.vTangent));
	Out.vTexUV			= In.vTexUV;
	Out.vProjPos		= Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
	float4		vProjPos : TEXCOORD1;
	float4		vWorldPosition : TEXCOORD2;
};

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);


	float fRimPower = 1.f - saturate(dot(In.vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
	fRimPower = pow(fRimPower, 2.f);

	vector vRimColor = g_vRimColor * fRimPower;
	Out.vDiffuse += vRimColor;

	if (0 == Out.vDiffuse.a)
		discard;

	return Out;
}

technique11 DefaultTechnique
{
	pass DefaultPass // 0
	{		
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
};