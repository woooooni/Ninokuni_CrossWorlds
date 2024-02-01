
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

VS_OUT VS_WITCHGRASS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP, matVP;
    float4x4 InstanceWorld = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
	
    if (In.vTexUV.y >= 0.5f)
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

#define UNITY_PI            3.14159265359f
#define UNITY_TWO_PI        6.28318530718f
#define UNITY_FOUR_PI       12.56637061436f

Texture2D BladeTexture;
Texture2D WindTexture;

struct VS_GRASS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;

    float4 vRight : TEXCOORD3;
    float4 vUp : TEXCOORD4;
    float4 vLook : TEXCOORD5;
    float4 vTranslation : TEXCOORD6;
    

    uint iInstanceID : SV_INSTANCEID;
};

float rand(float3 co)
{
    return frac(sin(dot(co.xyz, float3(12.9898f, 78.233f, 53.339f))) * 43758.5453f);
}

float3x3 AngleAxis3x3(float angle, float3 axis)
{
    float c, s;
    sincos(angle, s, c);
    
    float t = 1 - c;
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    
    return float3x3
    (
          t * x * x + c, t * x * y - s * z, t * x * z + s * y,
          t * x * y + s * z, t * y * y + c, t * y * z - s * x,
          t * x * z - s * y, t * y * z + s * x, t * z * z + c
    );
}


VS_GRASS_OUT tessVert(VS_IN v)
{
    VS_GRASS_OUT o;
    
    o.vPosition = float4(v.vPosition, 1.0f);
    o.vNormal = v.vNormal;
    o.vTexUV = v.vTexUV;
    o.vTangent = v.vTangent;
    
    o.vRight = v.vRight;
    o.vUp = v.vUp;
    o.vLook = v.vLook;
    o.vTranslation = v.vTranslation;
    
    matrix world = matrix(v.vRight, v.vUp, v.vLook, v.vTranslation);
    
    o.iInstanceID = v.iInstanceID;
    
    return o;
}

cbuffer CB_GrassBuffer
{
    float fBladeWidthMin = 0.04f;
    float fBladeWidthMax = 0.08f;
    float fBladeHeightMin = 0.3f;
    float fBladeHeightMax = 0.5f;
    
    float fBladeBendDistance = 0.38f;
    float fBladeBendCurve = 2.0f;
    float fBendDelta = 0.2f;
    float fTessellationGrassDistance = 0.1f;
    
    float fGrassThreshold = 0.5f;
    float fGrassFalloff = 0.05f;
    float fWindFrequency = 0.01f;
    float fFad = 0.0f;
    
    
    float4 vWindVelocity = float4(1.0f, 0.0f, 0.0f, 0.0f);
    float4 vWindMap_ST = float4(1.0f, 1.0f, 3.0f, 3.0f); // xy = scale zw = offset?
};

struct TessellationFactors
{
    float edge[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

cbuffer CB_Time
{
    float fTime = 0.0f;
};

float tessellationEdgeFactor(VS_GRASS_OUT vert0, VS_GRASS_OUT vert1)
{
    float3 v0 = vert0.vPosition.xyz;
    float3 v1 = vert1.vPosition.xyz;
    float edgeLength = distance(v0, v1);
    return edgeLength / fTessellationGrassDistance;
}

TessellationFactors patchConstantFunc(InputPatch<VS_GRASS_OUT, 3> patch)
{
    TessellationFactors f;
    
    f.edge[0] = tessellationEdgeFactor(patch[1], patch[2]);
    f.edge[1] = tessellationEdgeFactor(patch[2], patch[0]);
    f.edge[2] = tessellationEdgeFactor(patch[0], patch[1]);

    f.inside = (f.edge[0] + f.edge[1] + f.edge[2]) / 3.0f;
    
    return f;
}

[domain("tri")]
[outputcontrolpoints(3)]
[outputtopology("triangle_cw")]
[partitioning("integer")]
[patchconstantfunc("patchConstantFunc")]
VS_GRASS_OUT hull(InputPatch<VS_GRASS_OUT, 3> patch, uint id : SV_OutputControlPointID)
{
    return patch[id];
}

[domain("tri")]
VS_GRASS_OUT domain(TessellationFactors factors, OutputPatch<VS_GRASS_OUT, 3> patch, float3 barycentricCoordinates : SV_DomainLocation)
{
    VS_IN i;

#define INTERPOLATE(fieldname) i.fieldname = \
					patch[0].fieldname * barycentricCoordinates.x + \
					patch[1].fieldname * barycentricCoordinates.y + \
					patch[2].fieldname * barycentricCoordinates.z;
    
    INTERPOLATE(vPosition)
    INTERPOLATE(vNormal)
    INTERPOLATE(vTexUV)
    INTERPOLATE(vTangent)
   
    
    i.iInstanceID = patch[0].iInstanceID;
    i.vRight = patch[0].vRight;
    i.vUp = patch[0].vUp;
    i.vLook = patch[0].vLook;
    i.vTranslation = patch[0].vTranslation;
    
    return tessVert(i);
}

struct GeomData
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float2 vProjPos : TEXCOORD1;
    float3 vTangent : TANGENT;
    float3 vBinormal : BINORMAL;
    float3 vWorldPos : TEXCOORD2;
    
    uint iInstanceID : SV_INSTANCEID;
    
    float3 vViewNormal : NORMAL1;
    float3 vPositionView : POSITION;
};

GeomData ObjectToClipPos(float3 pos, float2 uv, float4 vRight, float4 vUp, float4 vLook, float4 vTranslation, float3 normal, float3 vTangent, int iInstanceID)
{
    GeomData o = (GeomData) 0;
    
    float4x4 matWV, matWVP;
    float4x4 InstanceWorld = float4x4(vRight, vUp, vLook, vTranslation);
    
    matWV = mul(InstanceWorld, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    o.vPosition = mul(float4(pos, 1.0f), matWVP);
    o.vWorldPos = mul(float4(pos, 1.0f), InstanceWorld);
    
    o.vNormal = normalize(mul(float4(normal, 0.0f), InstanceWorld)).xyz;
    o.vTangent = normalize(mul(float4(vTangent, 0.0f), InstanceWorld)).xyz;
    o.vBinormal = normalize(cross(o.vNormal, o.vTangent));
    o.vTexcoord = uv;
    o.vProjPos = o.vPosition;
    o.iInstanceID = iInstanceID;
    
    o.vViewNormal = mul(o.vNormal, (float3x3)g_ViewMatrix);
    o.vPositionView = mul(float4(pos, 1.0f), matWV);

    return o;
}

#define BLADE_SEGMENTS 4

[maxvertexcount(BLADE_SEGMENTS * 2 + 1)]
void geom(point VS_GRASS_OUT input[1], inout TriangleStream<GeomData> triStream)
{
    GeomData o = (GeomData) 0;
    
    float3 pos = input[0].vPosition;
    float3 vNormal = input[0].vNormal;
    float3 vTangent = input[0].vTangent;
    float3 vBiNormal = cross(vNormal, vTangent);
    
    float4 vRight = input[0].vRight;
    float4 vUp = input[0].vUp;
    float4 vLook = input[0].vLook;
    float4 vTranslation = input[0].vTranslation;
    
    uint iInstanceID = input[0].iInstanceID;
    
    float3x3 tangentToLocal = float3x3(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    );
    
    float3x3 randRotMatrix = AngleAxis3x3(rand(pos) * UNITY_TWO_PI, float3(0.0f, 1.0f, 0.0f));
    float3x3 randBendMatrix = AngleAxis3x3(rand(pos.zzx) * fBendDelta * UNITY_PI * 0.5f, float3(-1.0f, 0.0f, 0.0f));

    float2 windUV = pos.xz * vWindMap_ST.xy + vWindMap_ST.zw + normalize(vWindVelocity.xzy) * fWindFrequency * fTime;
    float2 windSample = (WindTexture.SampleLevel(PointSampler, input[0].vTexUV.y * fTime, 0.0f) * 2.0f - 1.0f) * length(vWindVelocity);
    float3 windAxis = normalize(float3(windSample.x, windSample.y, 0.0f));
    float3x3 windMatrix = AngleAxis3x3(UNITY_PI * windSample, windAxis);
     
    float3x3 baseTransformationMatrix = mul(tangentToLocal, randRotMatrix);
    float3x3 tipTransformationMatrix = mul(mul(mul(tangentToLocal, windMatrix), randBendMatrix), randRotMatrix);
    
    float width = lerp(fBladeWidthMin, fBladeWidthMax, rand(pos.xzy));
    float height = lerp(fBladeHeightMin, fBladeHeightMax, rand(pos.zyx));
    float foward = rand(pos.yyz) * fBladeBendDistance;
    
    for (int i = 0; i < BLADE_SEGMENTS; ++i)
    {
        float t = i / (float) BLADE_SEGMENTS;
        float3 offset = float3(width * (1 - t), pow(t, fBladeBendCurve) * foward, height * t);
        
        float3x3 transformationMatrix = (i == 0) ? baseTransformationMatrix : tipTransformationMatrix;
        
        triStream.Append(ObjectToClipPos(pos + mul(transformationMatrix, float3(offset.x, offset.y, offset.z)), float2(0, t), vRight, vUp, vLook, vTranslation, vNormal, vTangent, iInstanceID));
        triStream.Append(ObjectToClipPos(pos + mul(transformationMatrix, float3(-offset.x, offset.y, offset.z)), float2(1, t), vRight, vUp, vLook, vTranslation, vNormal, vTangent, iInstanceID));
    }
    triStream.Append(ObjectToClipPos(pos + mul(tipTransformationMatrix, float3(0, foward, height)), float2(0.5f, 1.0f), vRight, vUp, vLook, vTranslation, vNormal, vTangent, iInstanceID));
    triStream.RestartStrip();
}

float4 PS_Grass(GeomData i) : SV_Target
{
    float4 bottomColor = float4(0.29f, 0.431f, 0.294f, 1.0f);
    float4 TopColor = float4(1.0f, 0.639f, 0.447f, 1.0f);
   
    float4 vColor = lerp(bottomColor, TopColor, i.vTexcoord.y);
    
    return vColor;
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

	pass WitchGrass
	{
		// 7
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_WITCHGRASS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

	pass TessGrass
	{
		// 8
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 tessVert();
        HullShader = compile hs_5_0 hull();
        DomainShader = compile ds_5_0 domain();
        GeometryShader = compile gs_5_0 geom();
        PixelShader = compile ps_5_0 PS_Grass();
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