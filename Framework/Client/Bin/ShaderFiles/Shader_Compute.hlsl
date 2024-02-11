#include "Engine_Shader_Defines.hpp"

Texture2D g_IconTexture;
StructuredBuffer<int> Buffer0 : register(t0);
RWStructuredBuffer<int> BufferOut : register(u0);


[numthreads(64, 1, 1)]
void CS_ENGINEER_DEADSKILL(uint2 id : SV_DispatchThreadID)
{
    uint4 dtl = uint4(id, 0, 0);
    float2 inverseSize = float2(1.0f / 63.f, 1.0f / 63.f);
    float2 coord = float2(id.x * inverseSize.x, id.y * inverseSize.y);

    
    float4 vColor = g_IconTexture.SampleLevel(PointSampler, coord, 0);
    
    if (vColor.r <= 0.f)
        BufferOut[id.x * id.y / 64.f] = 0;
    else
        BufferOut[id.x * id.y / 64.f] = 1;
    
    
    //BufferOut[id] = vColor.r;
    //float t = temperature.SampleLevel(zeroBoundarySampler, coord, 0).x;
    //outputTemperature[dtid] = statict + t;
}

technique11 DefaultTechnique
{
	pass Compute_Engineer_DeadSkillPositions
	{
		VertexShader = NULL;
		GeometryShader = NULL;
        HullShader = NULL;
        DomainShader = NULL;
        PixelShader = NULL;
        ComputeShader = compile cs_5_0 CS_ENGINEER_DEADSKILL();
    }


}