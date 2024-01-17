#include "stdafx.h"
#include "Vfx_Glanix_Skill_Rush.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"

CVfx_Glanix_Skill_Rush::CVfx_Glanix_Skill_Rush(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_Rush::CVfx_Glanix_Skill_Rush(const CVfx_Glanix_Skill_Rush& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_Rush::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_CHARGE;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

#pragma region EVENT1
	// 01 : 0
	{
		m_pFrameTriger[TYPE_ET1_1_D_RECTWARNING]    = 0;
		m_pPositionOffset[TYPE_ET1_1_D_RECTWARNING] = _float3(0.f, 0.f, 0.5f);
		m_pScaleOffset[TYPE_ET1_1_D_RECTWARNING]    = _float3(5.f, 2.f, 30.f);
		m_pRotationOffset[TYPE_ET1_1_D_RECTWARNING] = _float3(0.f, 0.f, 0.f);
	}
#pragma endregion

#pragma region EVENT2
	// 01 : 11
	{
		m_pFrameTriger[TYPE_ET2_1_D_CRACK_R] = 11;
		m_pPositionOffset[TYPE_ET2_1_D_CRACK_R] = _float3(0.5f, 0.f, -0.5f);
		m_pScaleOffset[TYPE_ET2_1_D_CRACK_R]    = _float3(3.f, 3.f, 3.f);
		m_pRotationOffset[TYPE_ET2_1_D_CRACK_R] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_1_P_SMOKE_R] = 11;
		m_pPositionOffset[TYPE_ET2_1_P_SMOKE_R] = _float3(1.5f, 0.5f, -1.2f);
		m_pScaleOffset[TYPE_ET2_1_P_SMOKE_R]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_1_P_SMOKE_R] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_1_P_CIRCLES_R] = 11;
		m_pPositionOffset[TYPE_ET2_1_P_CIRCLES_R] = _float3(1.5f, 0.5f, -1.2f);
		m_pScaleOffset[TYPE_ET2_1_P_CIRCLES_R]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_1_P_CIRCLES_R] = _float3(0.f, 0.f, 0.f);
	}

	// 02 : 23
	{
		m_pFrameTriger[TYPE_ET2_2_D_CRACK_L] = 23;
		m_pPositionOffset[TYPE_ET2_2_D_CRACK_L] = _float3(-0.5f, 0.f, -0.5f);
		m_pScaleOffset[TYPE_ET2_2_D_CRACK_L]    = _float3(3.f, 3.f, 3.f);
		m_pRotationOffset[TYPE_ET2_2_D_CRACK_L] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_2_P_SMOKE_L] = 23;
		m_pPositionOffset[TYPE_ET2_2_P_SMOKE_L] = _float3(-1.5f, 0.5f, -1.2f);
		m_pScaleOffset[TYPE_ET2_2_P_SMOKE_L] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_2_P_SMOKE_L] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_2_P_CIRCLES_L] = 23;
		m_pPositionOffset[TYPE_ET2_2_P_CIRCLES_L] = _float3(-1.5f, 0.5f, -1.2f);
		m_pScaleOffset[TYPE_ET2_2_P_CIRCLES_L] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_2_P_CIRCLES_L] = _float3(0.f, 0.f, 0.f);
	}

	// 03 : 36
	{
		m_pFrameTriger[TYPE_ET2_3_D_CRACK_R] = 36;
		m_pPositionOffset[TYPE_ET2_3_D_CRACK_R] = _float3(0.5f, 0.f, -0.5f);
		m_pScaleOffset[TYPE_ET2_3_D_CRACK_R]    = _float3(3.f, 3.f, 3.f);
		m_pRotationOffset[TYPE_ET2_3_D_CRACK_R] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_3_P_SMOKE_R] = 36;
		m_pPositionOffset[TYPE_ET2_3_P_SMOKE_R] = _float3(1.5f, 0.5f, -1.2f);
		m_pScaleOffset[TYPE_ET2_3_P_SMOKE_R]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_3_P_SMOKE_R] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_3_P_CIRCLES_R] = 36;
		m_pPositionOffset[TYPE_ET2_3_P_CIRCLES_R] = _float3(1.5f, 0.5f, -1.2f);
		m_pScaleOffset[TYPE_ET2_3_P_CIRCLES_R]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_3_P_CIRCLES_R] = _float3(0.f, 0.f, 0.f);
	}
#pragma endregion

#pragma region EVENT3
	// 01 : 48
	{
		m_pFrameTriger[TYPE_ET3_1_P_SMOKE]    = 48;
		m_pPositionOffset[TYPE_ET3_1_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_1_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_1_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_1_P_CIRCLES]    = 48;
		m_pPositionOffset[TYPE_ET3_1_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 02 : 56
	{
		m_pFrameTriger[TYPE_ET3_2_P_SMOKE]    = 56;
		m_pPositionOffset[TYPE_ET3_2_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_2_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_2_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_2_P_CIRCLES]    = 56;
		m_pPositionOffset[TYPE_ET3_2_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 03 : 64
	{
		m_pFrameTriger[TYPE_ET3_3_P_SMOKE] = 64;
		m_pPositionOffset[TYPE_ET3_3_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_3_P_SMOKE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_3_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_3_P_CIRCLES] = 64;
		m_pPositionOffset[TYPE_ET3_3_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_3_P_CIRCLES] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 04 : 71
	{
		m_pFrameTriger[TYPE_ET3_4_P_SMOKE]    = 71;
		m_pPositionOffset[TYPE_ET3_4_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_4_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_4_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_4_P_CIRCLES]    = 71;
		m_pPositionOffset[TYPE_ET3_4_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_4_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_4_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 05 : 79
	{
		m_pFrameTriger[TYPE_ET3_5_P_SMOKE] = 79;
		m_pPositionOffset[TYPE_ET3_5_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_5_P_SMOKE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_5_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_5_P_CIRCLES] = 79;
		m_pPositionOffset[TYPE_ET3_5_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_5_P_CIRCLES] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_5_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 06 : 87
	{
		m_pFrameTriger[TYPE_ET3_6_P_SMOKE]    = 87;
		m_pPositionOffset[TYPE_ET3_6_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_6_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_6_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_6_P_CIRCLES]    = 87;
		m_pPositionOffset[TYPE_ET3_6_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_6_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_6_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 07 : 94
	{
		m_pFrameTriger[TYPE_ET3_7_P_SMOKE]    = 94;
		m_pPositionOffset[TYPE_ET3_7_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_7_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_7_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_7_P_CIRCLES]    = 94;
		m_pPositionOffset[TYPE_ET3_7_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_7_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_7_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 08 : 101
	{
		m_pFrameTriger[TYPE_ET3_8_P_SMOKE]    = 101;
		m_pPositionOffset[TYPE_ET3_8_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_8_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_8_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_8_P_CIRCLES]    = 101;
		m_pPositionOffset[TYPE_ET3_8_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_8_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_8_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 09 : 110
	{
		m_pFrameTriger[TYPE_ET3_9_P_SMOKE] = 110;
		m_pPositionOffset[TYPE_ET3_9_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_9_P_SMOKE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_9_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_9_P_CIRCLES] = 110;
		m_pPositionOffset[TYPE_ET3_9_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_9_P_CIRCLES] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_9_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 10 : 116
	{
		m_pFrameTriger[TYPE_ET3_10_P_SMOKE]    = 116;
		m_pPositionOffset[TYPE_ET3_10_P_SMOKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_10_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_10_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_10_P_CIRCLES]    = 116;
		m_pPositionOffset[TYPE_ET3_10_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_10_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_10_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}
#pragma endregion

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_Rush::Initialize(void* pArg)
{
	m_bEvent_1 = false;
	m_bEvent_2 = false;
	m_bEvent_3 = false;

	return S_OK;
}

void CVfx_Glanix_Skill_Rush::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (!m_bEvent_1)
		{
			if (m_iCount == TYPE_ET1_1_D_RECTWARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_1_D_RECTWARNING])
			{
				CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");

				if (pOwnerTransform != nullptr)
					m_WorldMatrix = pOwnerTransform->Get_WorldFloat4x4();

				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_FourHandSwing_Warning"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_1_D_RECTWARNING], m_pScaleOffset[TYPE_ET1_1_D_RECTWARNING], m_pRotationOffset[TYPE_ET1_1_D_RECTWARNING]);
				m_iCount++;

				m_bEvent_1 = true;
			}
		}
		else if (!m_bEvent_2)
		{
			if (m_iCount == TYPE_ET2_1_D_CRACK_R && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_1_D_CRACK_R])
			{
				CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");

				if (pOwnerTransform != nullptr)
					m_WorldMatrix = pOwnerTransform->Get_WorldFloat4x4();

				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_Rush_Crack"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_1_D_CRACK_R], m_pScaleOffset[TYPE_ET2_1_D_CRACK_R], m_pRotationOffset[TYPE_ET2_1_D_CRACK_R]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET2_1_P_SMOKE_R && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_1_P_SMOKE_R])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Smoke"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_1_P_SMOKE_R], m_pScaleOffset[TYPE_ET2_1_P_SMOKE_R], m_pRotationOffset[TYPE_ET2_1_P_SMOKE_R]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET2_1_P_CIRCLES_R && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_1_P_CIRCLES_R])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_HandDown_Circle"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_1_P_CIRCLES_R], m_pScaleOffset[TYPE_ET2_1_P_CIRCLES_R], m_pRotationOffset[TYPE_ET2_1_P_CIRCLES_R]);
				m_iCount++;
			}

			else if (m_iCount == TYPE_ET2_2_D_CRACK_L && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_2_D_CRACK_L])
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_Rush_Crack"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_2_D_CRACK_L], m_pScaleOffset[TYPE_ET2_2_D_CRACK_L], m_pRotationOffset[TYPE_ET2_2_D_CRACK_L]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET2_2_P_SMOKE_L && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_2_P_SMOKE_L])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Smoke"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_2_P_SMOKE_L], m_pScaleOffset[TYPE_ET2_2_P_SMOKE_L], m_pRotationOffset[TYPE_ET2_2_P_SMOKE_L]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET2_2_P_CIRCLES_L && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_2_P_CIRCLES_L])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_HandDown_Circle"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_2_P_CIRCLES_L], m_pScaleOffset[TYPE_ET2_2_P_CIRCLES_L], m_pRotationOffset[TYPE_ET2_2_P_CIRCLES_L]);
				m_iCount++;
			}

			else if (m_iCount == TYPE_ET2_3_D_CRACK_R && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_3_D_CRACK_R])
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_Rush_Crack"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_3_D_CRACK_R], m_pScaleOffset[TYPE_ET2_3_D_CRACK_R], m_pRotationOffset[TYPE_ET2_3_D_CRACK_R]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET2_3_P_SMOKE_R && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_3_P_SMOKE_R])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Smoke"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_3_P_SMOKE_R], m_pScaleOffset[TYPE_ET2_3_P_SMOKE_R], m_pRotationOffset[TYPE_ET2_3_P_SMOKE_R]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET2_3_P_CIRCLES_R && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_3_P_CIRCLES_R])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_HandDown_Circle"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_3_P_CIRCLES_R], m_pScaleOffset[TYPE_ET2_3_P_CIRCLES_R], m_pRotationOffset[TYPE_ET2_3_P_CIRCLES_R]);
				m_iCount++;

				m_bEvent_2 = true;
			}
		}
		else if (!m_bEvent_3)
		{
			if (m_iCount == TYPE_ET3_1_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_1_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_1_P_SMOKE);
			else if (m_iCount == TYPE_ET3_1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_1_P_CIRCLES])
				Set_Et3_CirclesParticle(TYPE_ET3_1_P_CIRCLES);

			else if (m_iCount == TYPE_ET3_2_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_2_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_2_P_SMOKE);
			else if (m_iCount == TYPE_ET3_2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_2_P_CIRCLES])
				Set_Et3_CirclesParticle(TYPE_ET3_2_P_CIRCLES);

			else if (m_iCount == TYPE_ET3_3_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_3_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_3_P_SMOKE);
			else if (m_iCount == TYPE_ET3_3_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_3_P_CIRCLES])
				Set_Et3_CirclesParticle(TYPE_ET3_3_P_CIRCLES);

			else if (m_iCount == TYPE_ET3_4_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_4_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_4_P_SMOKE);
			else if (m_iCount == TYPE_ET3_4_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_4_P_CIRCLES])
				Set_Et3_CirclesParticle(TYPE_ET3_4_P_CIRCLES);

			else if (m_iCount == TYPE_ET3_5_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_5_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_5_P_SMOKE);
			else if (m_iCount == TYPE_ET3_5_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_5_P_CIRCLES])
				Set_Et3_CirclesParticle(TYPE_ET3_5_P_CIRCLES);

			else if (m_iCount == TYPE_ET3_6_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_6_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_6_P_SMOKE);
			else if (m_iCount == TYPE_ET3_6_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_6_P_CIRCLES])
				Set_Et3_CirclesParticle(TYPE_ET3_6_P_CIRCLES);

			else if (m_iCount == TYPE_ET3_7_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_7_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_7_P_SMOKE);
			else if (m_iCount == TYPE_ET3_7_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_7_P_CIRCLES])
				Set_Et3_CirclesParticle(TYPE_ET3_7_P_CIRCLES);

			else if (m_iCount == TYPE_ET3_8_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_8_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_8_P_SMOKE);
			else if (m_iCount == TYPE_ET3_8_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_8_P_CIRCLES])
				Set_Et3_CirclesParticle(TYPE_ET3_8_P_CIRCLES);

			else if (m_iCount == TYPE_ET3_9_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_9_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_9_P_SMOKE);
			else if (m_iCount == TYPE_ET3_9_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_9_P_CIRCLES])
				Set_Et3_CirclesParticle(TYPE_ET3_9_P_CIRCLES);

			else if (m_iCount == TYPE_ET3_10_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_10_P_SMOKE])
				Set_Et3_SmokeParticle(TYPE_ET3_10_P_SMOKE);
			else if (m_iCount == TYPE_ET3_10_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_10_P_CIRCLES])
			{
				Set_Et3_CirclesParticle(TYPE_ET3_10_P_CIRCLES);

				m_bEvent_3 = true;
			}
		}
		else if(m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_Rush::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_Rush::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_Rush::Ready_Components()
{
	return S_OK;
}

void CVfx_Glanix_Skill_Rush::Set_Et3_SmokeParticle(TYPE eType)
{
	CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");

	if (pOwnerTransform != nullptr)
		m_WorldMatrix = pOwnerTransform->Get_WorldFloat4x4();

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_Rush_Smoke"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[eType], m_pScaleOffset[eType], m_pRotationOffset[eType]);

	m_iCount++;
}

void CVfx_Glanix_Skill_Rush::Set_Et3_CirclesParticle(TYPE eType)
{
	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FootDown_Circle"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[eType], m_pScaleOffset[eType], m_pRotationOffset[eType]);

	m_iCount++;
}

CVfx_Glanix_Skill_Rush* CVfx_Glanix_Skill_Rush::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_Rush* pInstance = new CVfx_Glanix_Skill_Rush(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_Rush");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_Rush::Clone(void* pArg)
{
	CVfx_Glanix_Skill_Rush* pInstance = new CVfx_Glanix_Skill_Rush(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_Rush");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_Rush::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}