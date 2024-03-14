#include "Engine_Shader_Defines.hpp"

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D	g_DiffuseTexture;
Texture2D	g_DepthTexture;
Texture2D	g_MaskTexture;
Texture2D	g_RedTexture;

Texture2D	g_DissolveTexture;

float4		g_vDiffuseColor = { 0.1f, 0.1f, 0.1f, 1.f };
float		g_Alpha = 1.f;
float		g_Time;

float g_LoadingProgress;

//
Texture2D g_JitterTexture;
Texture2D g_ScreenDepth;
Texture2D g_PermTexture;
Texture2D g_FireShape;
float4 EyePos;
matrix WorldViewProj;
float Time;

cbuffer CB_Fire
{
    bool bJitter = false;
    float StepSize; // 1 / 16(MAX)
    float NoiseScale;
    float Roughness;
    float FrequencyWeights[5];
};

//

cbuffer Gradients
{
    // 3D gradients
    
    const float3 Grad3[] =
    {
        float3(-1, -1, 0), // 0
        float3(-1, +1, 0), // 1
        float3(+1, -1, 0), // 2
        float3(+1, +1, 0), // 3

        float3(-1, 0, -1), // 4
        float3(-1, 0, +1), // 5
        float3(+1, 0, -1), // 6
        float3(+1, 0, +1), // 7

        float3(0, -1, -1), // 8
        float3(0, -1, +1), // 9
        float3(0, +1, -1), // 10
        float3(0, +1, +1), // 11

        // padding   
        float3(+1, +1, 0), // 12
        float3(-1, -1, 0), // 13
        float3(0, -1, +1), // 14
        float3(0, -1, -1) // 15
    };

    // 4D case is more regular

    const float4 Grad4[] =
    {
        // x, y, z
        float4(-1, -1, -1, 0), // 0
        float4(-1, -1, +1, 0), // 1
        float4(-1, +1, -1, 0), // 2
        float4(-1, +1, +1, 0), // 3
        float4(+1, -1, -1, 0), // 4
        float4(+1, -1, +1, 0), // 5
        float4(+1, +1, -1, 0), // 6
        float4(+1, +1, +1, 0), // 7
        // w, x, y
        float4(-1, -1, 0, -1), // 8
        float4(-1, +1, 0, -1), // 9
        float4(+1, -1, 0, -1), // 10
        float4(+1, +1, 0, -1), // 11
        float4(-1, -1, 0, +1), // 12
        float4(-1, +1, 0, +1), // 13
        float4(+1, -1, 0, +1), // 14
        float4(+1, +1, 0, +1), // 15
        // z, w, x
        float4(-1, 0, -1, -1), // 16
        float4(+1, 0, -1, -1), // 17
        float4(-1, 0, -1, +1), // 18
        float4(+1, 0, -1, +1), // 19
        float4(-1, 0, +1, -1), // 20
        float4(+1, 0, +1, -1), // 21
        float4(-1, 0, +1, +1), // 22
        float4(+1, 0, +1, +1), // 23
        // y, z, w
        float4(0, -1, -1, -1), // 24
        float4(0, -1, -1, +1), // 25
        float4(0, -1, +1, -1), // 26
        float4(0, -1, +1, +1), // 27
        float4(0, +1, -1, -1), // 28
        float4(0, +1, -1, +1), // 29
        float4(0, +1, +1, -1), // 30
        float4(0, +1, +1, +1) // 31
    };
};

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

struct VolumeVertex
{
    float4 ClipPos : SV_Position;
    float3 Pos : TEXCOORD0; // Vertex Position -> Local Space
    float3 RayDir : TEXCOORD1; // Ray Direction -> Local Space
};

SamplerState ClampSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

cbuffer NoiseBuffer
{
    float frameTime;
    float3 scrollSpeeds;
    float3 scales;
};

struct FlameInput
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vTexcoord1 : TEXCOORD1;
    float2 vTexcoord2 : TEXCOORD2;
    float2 vTexcoord3 : TEXCOORD3;
};

FlameInput FireVertexShader(VS_IN input)
{
    FlameInput output = (FlameInput) 0;

    output.vPosition = mul(float4(input.vPosition, 1.0f), WorldViewProj);
    output.vTexcoord = input.vTexUV;
    
    output.vTexcoord1 = input.vTexUV * scales.x;
    output.vTexcoord1.y = output.vTexcoord1.y + (frameTime * scrollSpeeds.x);
    
    output.vTexcoord2 = input.vTexUV * scales.y;
    output.vTexcoord2.y = output.vTexcoord2.y + (frameTime * scrollSpeeds.y);
    
    output.vTexcoord3 = input.vTexUV * scales.z;
    output.vTexcoord3.y = output.vTexcoord3.y + (frameTime * scrollSpeeds.z);
    
    return output;
}

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

VolumeVertex PerlinFireVS(VS_IN In)
{
    VolumeVertex Out = (VolumeVertex) 0;

    Out.ClipPos = mul(float4(In.vPosition, 1.0f), WorldViewProj);
	
    Out.RayDir = In.vPosition.xyz - EyePos.xyz;
    Out.Pos = In.vPosition.xyz; // Supposed to have range : -0.5 ~ 0.5

    return Out;
}

#define F3 0.333333333333
#define G3 0.166666666667

void Simplex3D(const in float3 P, out float3 simplex[4])
{
    float3 T = P.xzy >= P.yxz;
    simplex[0] = 0;
    simplex[1] = T.xzy > T.yxz;
    simplex[2] = T.yxz <= T.xzy;
    simplex[3] = 1;
}

int Hash(float3 P)
{
    return (int) g_PermTexture.Load(int3(P.xy, 0)).r ^ (int) g_PermTexture.Load(int3(P.z, 0, 0)).r;
}

int Hash(float4 P)
{
    return (int) g_PermTexture.Load(int3(P.xy, 0)).r ^ (int) g_PermTexture.Load(int3(P.zw, 0)).r;
}

float Snoise3D(float3 p)
{
    float s = dot(p, F3);
    float3 Pi = floor(p + s);
    float t = dot(Pi, G3);
	
    float3 PO = Pi - t;
    float3 Pf0 = p - PO;
	
    float3 simplex[4];
    Simplex3D(Pf0, simplex);
	
    float n = 0;
	
	[unroll]
    for (int i = 0; i < 4; ++i)
    {
        float3 Pf = Pf0 - simplex[i] + G3 * i;
        int h = Hash(Pi + simplex[i]);
        float d = saturate(0.6f - dot(Pf, Pf));
        d *= d;
        n += d * d * dot(Grad3[h & 31], Pf);
    }
    
    return 32.0f * n;
}

float Turbulence3D(float3 p)
{
    float res = 0;
	
	[loop]
    for (int i = 0; i < 5; ++i, p*=2)
        res += FrequencyWeights[i] * Snoise3D(p);

    return res;
}

float4 PerlinFire3DPS(VolumeVertex input) : SV_Target
{
    float3 Dir = normalize(input.RayDir) * StepSize;
    float Offset = bJitter ? g_JitterTexture.Sample(LinearSampler, input.ClipPos.xy / 256.0f).r : 0;
	
	// Jitter Initial position
    float3 Pos = input.Pos + Dir * Offset;
	
    float3 resultColor = 0;
    float SceneZ = g_ScreenDepth.Load(int3(input.ClipPos.xy, 0));

    while (true)
    {
        float4 ClipPos = mul(float4(Pos, 1.0f), WorldViewProj);
        ClipPos.z /= ClipPos.w;
		
        if (ClipPos.z > SceneZ || any(abs(Pos) > 0.5))
            break;
		
        float3 NoiseCoord = Pos;
        NoiseCoord.y -= Time;
		
        float Turbulence = abs(Turbulence3D(NoiseCoord * NoiseScale));
        
        float2 tc;
        tc.x = length(Pos.xz) * 2;
        tc.y = 0.5f - Pos.y - Roughness * Turbulence * pow((0.5f + Pos.y), 0.5f);

        resultColor += StepSize * 12.0f * g_FireShape.SampleLevel(ClampSampler, tc, 0);

        Pos += Dir;
    }
    
    return float4(resultColor, 1);
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
    float4		vBloom : SV_TARGET3;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

	if (0.0001f >= Out.vColor.a)
		discard;

	return Out;	
}

Texture2D fireTexture;
Texture2D noiseTexture;
Texture2D alphaTexture;

cbuffer DistortionBuffer
{
    float2 distortion1;
    float2 distortion2;
    float2 distortion3;
    float distortionScale;
    float distortionBias;
};

PS_OUT FirePixelShader(FlameInput input) : SV_TARGET
{
    PS_OUT output = (PS_OUT) 0;
    
    float4 noise1;
    float4 noise2;
    float4 noise3;
    float4 finalNoise;
    float perturb;
    float2 noiseCoords;
    float4 fireColor;
    float4 alphaColor;
    
    noise1 = noiseTexture.Sample(LinearSampler, input.vTexcoord1);
    noise2 = noiseTexture.Sample(LinearSampler, input.vTexcoord2);
    noise3 = noiseTexture.Sample(LinearSampler, input.vTexcoord3);
    
    noise1 = (noise1 - 0.5f) * 2.0f;
    noise2 = (noise2 - 0.5f) * 2.0f;
    noise3 = (noise3 - 0.5f) * 2.0f;

    noise1.xy = noise1.xy * distortion1.xy;
    noise2.xy = noise2.xy * distortion2.xy;
    noise3.xy = noise3.xy * distortion3.xy;
    
    finalNoise = noise1 + noise2 + noise3;
    
    perturb = ((1.0f - input.vTexcoord.y) * distortionScale) + distortionBias;
    
    noiseCoords.xy = (finalNoise.xy * perturb) + input.vTexcoord.xy;
    
    fireColor = fireTexture.Sample(ClampSampler, noiseCoords.xy);

    alphaColor = alphaTexture.Sample(ClampSampler, noiseCoords.xy);
    
    fireColor.a = alphaColor;
    
    output.vColor = fireColor;
    output.vBloom = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    return output;
}

cbuffer CB_Moon
{
    bool bStartChange = false;
    float4 vBloomColor = float4(0.0f, 0.0f, 0.0f, 0.0);
    float fTime = 0.0f;
	
    float fDissolveWeight = 0.0f;
    float fDissolveTime = 5.0f;
};

//float2 RotateUV(float2 uv, float angle)
//{
//    float s = sin(angle);
//    float c = cos(angle);
//    float2x2 rotationMatrix = float2x2(c, -s, s, c);
//    return mul(uv - 0.5, rotationMatrix) + 0.5;
//}

PS_OUT PS_MOON_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    float4 vWhiteColor  = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
    float4 vRedColor    = g_RedTexture.Sample(LinearSampler, In.vTexUV);
    if (0.2f >= vWhiteColor.a)
        discard;
	
	// 텍스처 좌표 회전
    //float  rotationAngle = fDissolveWeight / 2.f; // 시간에 따라 회전 각도 계산
    //float2 rotatedUV = RotateUV(In.vTexUV, rotationAngle);

    //float4 vDissolve = g_DissolveTexture.Sample(LinearSampler, rotatedUV);
    float4 vDissolve = g_DissolveTexture.Sample(LinearSampler, In.vTexUV);
    float fDissolveAlpha = bStartChange == true ? saturate(1.0f - fDissolveWeight / fDissolveTime + vDissolve.r) : 1.0f;
    // Dissolve의 Alpha의 Weight가 늘어날수록 Dissolve의 알파가 0이 된다.
	
    if (fDissolveAlpha < 0.45f)
    {
        Out.vBloom = vBloomColor;
        Out.vColor = vRedColor;

    }
	
    else if (fDissolveAlpha < 0.47f)
    {
        Out.vBloom = float4(1.000f, 0.311f, 0.f, 1.f);
        Out.vColor = Out.vBloom;
    }
    else if (fDissolveAlpha < 0.52f)
    {
        Out.vBloom = float4(1.f, 0.816f, 0.275f, 1.f);
        Out.vColor = Out.vBloom;
    }
	
    else if (fDissolveAlpha < 0.53f)
    {
        Out.vBloom = float4(1.f, 1.f, 1.f, 1.f);
        Out.vColor = Out.vBloom;
    }
	
    else if (fDissolveAlpha < 0.54f)
    {
        Out.vBloom = float4(1.f, 0.816f, 0.275f, 1.f);
        Out.vColor = Out.vBloom;
    }
    else if (fDissolveAlpha < 0.55f)
    {
        Out.vBloom = float4(1.000f, 0.311f, 0.f, 1.f);
        Out.vColor = Out.vBloom;
    }

    else // 원래색
    {
        Out.vBloom = vBloomColor;
        Out.vColor = vWhiteColor;
    }
	
    return Out;
}

PS_OUT PS_COLLIDER(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = float4(0.5f, 0.5f, 0.5f, 0.5f);

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
	//  검은색 0 0 0
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
	{
		discard;
	}

	return Out;
}

DepthStencilState DisableDepthWrites
{
    DepthEnable = true;
    DepthWriteMask = 0;
};

BlendState EnableAdditiveBlending
{
    SrcBlend = INV_DEST_COLOR; // to avoid oversaturation
    DestBlend = One;
    BlendOp = Add;
    BlendEnable[0] = true;
};

RasterizerState MultisamplingEnabled
{
    MultisampleEnable = true;
};

technique11 DefaultTechnique
{
	pass DefaultPass // 0
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass AlphaBlendPass // 1
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ALPHA();
	}

	pass NoDiscardPass // 2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FADE();
	}

	pass CloudPass // 3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_CLOUD();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CLOUD();
	}

	pass LoadingPass // 4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LOADING();
	}

	pass UsingMaskPass // 5
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_None, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_USING_MASK();
	}

    pass ColliderPass // 6
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_COLLIDER();
    }

    pass DefaultNonCullPass // 7
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MOON_MAIN();
    }

    pass PerlinFire3D // 8
    {
        SetRasterizerState(RS_NoneCull);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 FireVertexShader();
        GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = compile ps_5_0 FirePixelShader();
    }
}