
#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D		g_DiffuseTexture;
Texture2D		g_NormalTexture;

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

#define QUATERNION_IDENTITY float4(0, 0, 0, 1)
float4 q_slerp(in float4 a, in float4 b, float t)
{
	// if either input is zero, return the other.
	if (length(a) == 0.0)
	{
		if (length(b) == 0.0)
		{
			return QUATERNION_IDENTITY;
		}
		return b;
	}
	else if (length(b) == 0.0)
	{
		return a;
	}

	float cosHalfAngle = a.w * b.w + dot(a.xyz, b.xyz);

	if (cosHalfAngle >= 1.0 || cosHalfAngle <= -1.0)
	{
		return a;
	}
	else if (cosHalfAngle < 0.0)
	{
		b.xyz = -b.xyz;
		b.w = -b.w;
		cosHalfAngle = -cosHalfAngle;
	}

	float blendA;
	float blendB;
	if (cosHalfAngle < 0.99)
	{
		// do proper slerp for big angles
		float halfAngle = acos(cosHalfAngle);
		float sinHalfAngle = sin(halfAngle);
		float oneOverSinHalfAngle = 1.0 / sinHalfAngle;
		blendA = sin(halfAngle * (1.0 - t)) * oneOverSinHalfAngle;
		blendB = sin(halfAngle * t) * oneOverSinHalfAngle;
	}
	else
	{
		// do lerp if angle is really small.
		blendA = 1.0 - t;
		blendB = t;
	}

	float4 result = float4(blendA * a.xyz + blendB * b.xyz, blendA * a.w + blendB * b.w);
	if (length(result) > 0.0)
	{
		return normalize(result);
	}
	return QUATERNION_IDENTITY;
}

float4 matrix_to_quaternion(float4x4 m)
{
	float tr = m[0][0] + m[1][1] + m[2][2];
	float4 q = float4(0, 0, 0, 0);

	if (tr > 0)
	{
		float s = sqrt(tr + 1.0) * 2; // S=4*qw 
		q.w = 0.25 * s;
		q.x = (m[2][1] - m[1][2]) / s;
		q.y = (m[0][2] - m[2][0]) / s;
		q.z = (m[1][0] - m[0][1]) / s;
	}
	else if ((m[0][0] > m[1][1]) && (m[0][0] > m[2][2]))
	{
		float s = sqrt(1.0 + m[0][0] - m[1][1] - m[2][2]) * 2; // S=4*qx 
		q.w = (m[2][1] - m[1][2]) / s;
		q.x = 0.25 * s;
		q.y = (m[0][1] + m[1][0]) / s;
		q.z = (m[0][2] + m[2][0]) / s;
	}
	else if (m[1][1] > m[2][2])
	{
		float s = sqrt(1.0 + m[1][1] - m[0][0] - m[2][2]) * 2; // S=4*qy
		q.w = (m[0][2] - m[2][0]) / s;
		q.x = (m[0][1] + m[1][0]) / s;
		q.y = 0.25 * s;
		q.z = (m[1][2] + m[2][1]) / s;
	}
	else
	{
		float s = sqrt(1.0 + m[2][2] - m[0][0] - m[1][1]) * 2; // S=4*qz
		q.w = (m[1][0] - m[0][1]) / s;
		q.x = (m[0][2] + m[2][0]) / s;
		q.y = (m[1][2] + m[2][1]) / s;
		q.z = 0.25 * s;
	}

	return q;
}



float4x4 quaternion_to_matrix(float4 quat)
{
	float4x4 m = float4x4(float4(0, 0, 0, 0), float4(0, 0, 0, 0), float4(0, 0, 0, 0), float4(0, 0, 0, 0));

	float x = quat.x, y = quat.y, z = quat.z, w = quat.w;
	float x2 = x + x, y2 = y + y, z2 = z + z;
	float xx = x * x2, xy = x * y2, xz = x * z2;
	float yy = y * y2, yz = y * z2, zz = z * z2;
	float wx = w * x2, wy = w * y2, wz = w * z2;

	m[0][0] = 1.0 - (yy + zz);
	m[0][1] = xy - wz;
	m[0][2] = xz + wy;

	m[1][0] = xy + wz;
	m[1][1] = 1.0 - (xx + zz);
	m[1][2] = yz - wx;

	m[2][0] = xz - wy;
	m[2][1] = yz + wx;
	m[2][2] = 1.0 - (xx + yy);

	m[3][3] = 1.0;

	return m;
}


float4x4 m_scale(float4x4 m, float3 v)
{
	float x = v.x, y = v.y, z = v.z;

	m[0][0] *= x; m[1][0] *= y; m[2][0] *= z;
	m[0][1] *= x; m[1][1] *= y; m[2][1] *= z;
	m[0][2] *= x; m[1][2] *= y; m[2][2] *= z;
	m[0][3] *= x; m[1][3] *= y; m[2][3] *= z;

	return m;
}

float4x4 m_translate(float4x4 m, float3 v)
{
	float x = v.x, y = v.y, z = v.z;
	m[0][3] = x;
	m[1][3] = y;
	m[2][3] = z;
	return m;
}



void decompose(in float4x4 m, out float3 position, out float4 rotation, out float3 scale)
{
	float sx = length(float3(m[0][0], m[0][1], m[0][2]));
	float sy = length(float3(m[1][0], m[1][1], m[1][2]));
	float sz = length(float3(m[2][0], m[2][1], m[2][2]));

	// if determine is negative, we need to invert one scale
	float det = determinant(m);
	if (det < 0) {
		sx = -sx;
	}

	position.x = m[3][0];
	position.y = m[3][1];
	position.z = m[3][2];

	// scale the rotation part

	float invSX = 1.0 / sx;
	float invSY = 1.0 / sy;
	float invSZ = 1.0 / sz;

	m[0][0] *= invSX;
	m[0][1] *= invSX;
	m[0][2] *= invSX;

	m[1][0] *= invSY;
	m[1][1] *= invSY;
	m[1][2] *= invSY;

	m[2][0] *= invSZ;
	m[2][1] *= invSZ;
	m[2][2] *= invSZ;

	rotation = matrix_to_quaternion(m);

	scale.x = sx;
	scale.y = sy;
	scale.z = sz;
}

float4x4 compose(float3 position, float4 quat, float3 scale)
{
	float4x4 m = quaternion_to_matrix(quat);
	m = m_scale(m, scale);
	m = m_translate(m, position);
	return m;
}



matrix GetAnimationMatrix(VS_IN input)
{
	float indices[4] = { input.vBlendIndex.x, input.vBlendIndex.y, input.vBlendIndex.z, input.vBlendIndex.w };
	float weights[4] = { input.vBlendWeight.x, input.vBlendWeight.y, input.vBlendWeight.z, input.vBlendWeight.w };

	int animIndex[2];
	int currFrame[2];
	int nextFrame[2];
	float ratio[2];

	/* cur */
	animIndex[0] = g_TweenFrames.cur.iAnimIndex;
	currFrame[0] = g_TweenFrames.cur.iCurFrame;
	nextFrame[0] = g_TweenFrames.cur.iNextFrame;
	ratio[0] = g_TweenFrames.cur.fRatio;

	/* next */
	animIndex[1] = g_TweenFrames.next.iAnimIndex;
	currFrame[1] = g_TweenFrames.next.iCurFrame;
	nextFrame[1] = g_TweenFrames.next.iNextFrame;
	ratio[1] = g_TweenFrames.next.fRatio;

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

		float3 vCurrPos, vCurrScale;
		float4 vCurrQuaternion;

		decompose(curr, vCurrPos, vCurrQuaternion, vCurrScale);

		float3 vNextPos, vNextScale;
		float4 vNextQuaternion;

		decompose(next, vNextPos, vNextQuaternion, vNextScale);
		


		float3 vLerpPos = lerp(vCurrPos, vNextPos, ratio[0]);
		float3 vLerpScale = lerp(vCurrScale, vNextScale, ratio[0]);
		float4 vLerpQuaternion = q_slerp(vCurrQuaternion, vNextQuaternion, ratio[0]);

		float4x4 result = compose(vLerpPos, vLerpQuaternion, vLerpScale);


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

			/*matrix nextResult = matrix(QSlerp(c0, n0, ratio[0]), QSlerp(c1, n1, ratio[0]), QSlerp(c2, n2, ratio[0]), lerp(c3, n3, ratio[0]));


			result = matrix(QSlerp(result._11_12_13_14, nextResult._11_12_13_14, g_TweenFrames.fTweenRatio),
				QSlerp(result._21_22_23_24, nextResult._21_22_23_24, g_TweenFrames.fTweenRatio),
				QSlerp(result._31_32_33_34, nextResult._31_32_33_34, g_TweenFrames.fTweenRatio),
				lerp(result._41_42_43_44, nextResult._41_42_43_44, g_TweenFrames.fTweenRatio));*/
		}

		transform += mul(result, weights[i]);
	}

	return transform;
}


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;


	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	float4x4	BoneMatrix = GetAnimationMatrix(In);

	vector		vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
	vector		vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);

	Out.vPosition = mul(vPosition, matWVP);
	Out.vWorldPosition = mul(vPosition, g_WorldMatrix);
	Out.vWorldPosition = mul(vPosition, g_WorldMatrix);
	Out.vNormal = normalize(mul(vNormal, g_WorldMatrix));
	Out.vTangent = normalize(mul(vTangent, g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

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

	if (0.f == Out.vDiffuse.a)
		discard;

	return Out;
}

PS_OUT PS_MAIN_NORMAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	
	Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);


	vector		vTextureNormal = g_NormalTexture.Sample(ModelSampler, In.vTexUV);

	float3		vNormal = vTextureNormal.xyz * 2.f - 1.f;
	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal.xyz);

	
	vNormal = normalize(mul(vNormal, WorldMatrix));

	Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);

	

	float fRimPower = 1.f - saturate(dot(vNormal.xyz, normalize((-1.f * (In.vWorldPosition - g_vCamPosition)))));
	fRimPower = pow(fRimPower, 2.f);

	vector vRimColor = g_vRimColor * fRimPower;
	Out.vDiffuse += vRimColor;

	if (0.f == Out.vDiffuse.a)
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
	else if((vDissolve.r - g_fDissolveWeight) < 0.115f)
		Out.vDiffuse = g_vDissolveColor - 0.1f;
	else if ((vDissolve.r - g_fDissolveWeight) < 0.125f)
		Out.vDiffuse = g_vDissolveColor - 0.1f;
	else
		Out.vDiffuse = g_DiffuseTexture.Sample(ModelSampler, In.vTexUV);

	

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1000.f, 0.0f, 0.0f);

	if (0 == Out.vDiffuse.a)
		discard;

	return Out;
}


// ±×¸²ÀÚ ÇÈ¼¿ ½¦ÀÌ´õ.
struct PS_OUT_SHADOW_DEPTH
{
	float4		vDepth : SV_TARGET0;
};


PS_OUT_SHADOW_DEPTH PS_SHADOW_DEPTH(PS_IN In)
{
	PS_OUT_SHADOW_DEPTH Out = (PS_OUT_SHADOW_DEPTH)0;

	vector vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	if(vColor.a <= 0.01f)
		discard;


	Out.vDepth = vector(In.vProjPos.w / 1000.0f, 0.f, 0.f, 1.f);

	return Out;
}




technique11 DefaultTechnique
{
	pass DefaultPass
	{
		// 0
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass NormalPass
	{
		// 1
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
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
		PixelShader = compile ps_5_0 PS_DISSOLVE_DEAD();
	}

	pass Temp0
	{
		// 3
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp1
	{
		// 4
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp2
	{
		// 5
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp3
	{
		// 6
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp4
	{
		// 6
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp5
	{
		// 7
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Temp6
	{
		// 8
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
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
		PixelShader = compile ps_5_0 PS_SHADOW_DEPTH();
	}
};