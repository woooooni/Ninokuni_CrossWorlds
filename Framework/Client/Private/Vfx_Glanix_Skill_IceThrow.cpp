#include "stdafx.h"
#include "Vfx_Glanix_Skill_IceThrow.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"
#include "Effect.h"

CVfx_Glanix_Skill_IceThrow::CVfx_Glanix_Skill_IceThrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_IceThrow::CVfx_Glanix_Skill_IceThrow(const CVfx_Glanix_Skill_IceThrow& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_IceThrow::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_SNOWBALL;

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
		m_pScaleOffset[TYPE_ET1_1_D_RECTWARNING]    = _float3(5.f, 2.f, 15.f);
		m_pRotationOffset[TYPE_ET1_1_D_RECTWARNING] = _float3(0.f, 0.f, 0.f);
	}
#pragma endregion

#pragma region EVENT2
	// 01 : 12 (¿Þ)
	{
		m_pFrameTriger[TYPE_ET2_1_P_SPARKLE]    = 12;
		m_pPositionOffset[TYPE_ET2_1_P_SPARKLE] = _float3(-1.f, 3.6f, -1.f);
		m_pScaleOffset[TYPE_ET2_1_P_SPARKLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_1_P_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_1_P_SMOKE]    = 12;
		m_pPositionOffset[TYPE_ET2_1_P_SMOKE] = _float3(-1.f, 3.6f, -1.f);
		m_pScaleOffset[TYPE_ET2_1_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_1_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

	// 02 : 18 (¿À)
	{
		m_pFrameTriger[TYPE_ET2_2_P_SPARKLE]    = 18;
		m_pPositionOffset[TYPE_ET2_2_P_SPARKLE] = _float3(1.f, 3.6f, -1.5f);
		m_pScaleOffset[TYPE_ET2_2_P_SPARKLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_2_P_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_2_P_SMOKE]    = 18;
		m_pPositionOffset[TYPE_ET2_2_P_SMOKE] = _float3(1.f, 3.6f, -1.5f);
		m_pScaleOffset[TYPE_ET2_2_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}
#pragma endregion

#pragma region EVENT3
	// 01 : 37 (¹Ù´Ú Â¤À»¶§)
	{
		m_pFrameTriger[TYPE_ET3_1_D_CRACK]    = 37;
		m_pPositionOffset[TYPE_ET3_1_D_CRACK] = _float3(-0.4f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_1_D_CRACK]    = _float3(6.f, 5.f, 8.f);
		m_pRotationOffset[TYPE_ET3_1_D_CRACK] = _float3(0.f, 90.f, 0.f);

		m_pFrameTriger[TYPE_ET3_1_P_SMOKE]    = 37;
		m_pPositionOffset[TYPE_ET3_1_P_SMOKE] = _float3(0.f, 0.5f, 2.f);
		m_pScaleOffset[TYPE_ET3_1_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_1_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_1_P_CIRCLES]    = 37;
		m_pPositionOffset[TYPE_ET3_1_P_CIRCLES] = _float3(0.f, 0.5f, 2.f);
		m_pScaleOffset[TYPE_ET3_1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// 02 : 45 (¿Ã¶ó¿Ã¶§)
	{
		m_pFrameTriger[TYPE_ET3_2_E_SPRINGUP] = 45;
		m_pPositionOffset[TYPE_ET3_2_E_SPRINGUP] = _float3(0.f, 0.f, 2.f);
		m_pScaleOffset[TYPE_ET3_2_E_SPRINGUP]    = _float3(1.f, 1.5f, 1.f);
		m_pRotationOffset[TYPE_ET3_2_E_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_2_P_CIRCLES] = 45;
		m_pPositionOffset[TYPE_ET3_2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}
#pragma endregion

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_IceThrow::Initialize(void* pArg)
{
	m_bEvent_1 = false;
	m_bEvent_2 = false;
	m_bEvent_3 = false;

	return S_OK;
}

void CVfx_Glanix_Skill_IceThrow::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (!m_bEvent_1)
		{
			if (m_iCount == TYPE_ET1_1_D_RECTWARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_1_D_RECTWARNING])
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_FourHandSwing_Warning"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_1_D_RECTWARNING], m_pScaleOffset[TYPE_ET1_1_D_RECTWARNING], m_pRotationOffset[TYPE_ET1_1_D_RECTWARNING]);
				m_iCount++;

				m_bEvent_1 = true;
			}
		}
		else if (!m_bEvent_2)
		{
			if (m_iCount == TYPE_ET2_1_P_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_1_P_SPARKLE])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_1_P_SPARKLE], m_pScaleOffset[TYPE_ET2_1_P_SPARKLE], m_pRotationOffset[TYPE_ET2_1_P_SPARKLE]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET2_1_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_1_P_SMOKE])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_1_P_SMOKE], m_pScaleOffset[TYPE_ET2_1_P_SMOKE], m_pRotationOffset[TYPE_ET2_1_P_SMOKE]);
				m_iCount++;
			}

			else if (m_iCount == TYPE_ET2_2_P_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_2_P_SPARKLE])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_2_P_SPARKLE], m_pScaleOffset[TYPE_ET2_2_P_SPARKLE], m_pRotationOffset[TYPE_ET2_2_P_SPARKLE]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET2_2_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_2_P_SMOKE])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_2_P_SMOKE], m_pScaleOffset[TYPE_ET2_2_P_SMOKE], m_pRotationOffset[TYPE_ET2_2_P_SMOKE]);
				m_iCount++;

				m_bEvent_2 = true;
			}
		}
		else if (!m_bEvent_3)
		{
			if (m_iCount == TYPE_ET3_1_D_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_1_D_CRACK])
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_IceThrow_Crack"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_1_D_CRACK], m_pScaleOffset[TYPE_ET3_1_D_CRACK], m_pRotationOffset[TYPE_ET3_1_D_CRACK]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET3_1_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_1_P_SMOKE])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_IceThrow_Smoke"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_1_P_SMOKE], m_pScaleOffset[TYPE_ET3_1_P_SMOKE], m_pRotationOffset[TYPE_ET3_1_P_SMOKE]);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET3_1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_1_P_CIRCLES])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_IceThrow_DownCircle"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_1_P_CIRCLES], m_pScaleOffset[TYPE_ET3_1_P_CIRCLES], m_pRotationOffset[TYPE_ET3_1_P_CIRCLES]);
				m_iCount++;
			}

			else if (m_iCount == TYPE_ET3_2_E_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_2_E_SPRINGUP])
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_IceThrow_SpringUp"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_2_E_SPRINGUP], m_pScaleOffset[TYPE_ET3_2_E_SPRINGUP], m_pRotationOffset[TYPE_ET3_2_E_SPRINGUP], nullptr, &m_pEv3_SpringUp, false);
				Safe_AddRef(m_pEv3_SpringUp);
				m_iCount++;
			}
			else if (m_iCount == TYPE_ET3_2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_2_P_CIRCLES])
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_IceThrow_UpCircle"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_2_P_CIRCLES], m_pScaleOffset[TYPE_ET3_2_P_CIRCLES], m_pRotationOffset[TYPE_ET3_2_P_CIRCLES]);
				m_iCount++;

				m_bEvent_3 = true;
			}
		}
		else if (m_iCount >= TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_IceThrow::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_IceThrow::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_IceThrow::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_IceThrow* CVfx_Glanix_Skill_IceThrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_IceThrow* pInstance = new CVfx_Glanix_Skill_IceThrow(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_IceThrow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_IceThrow::Clone(void* pArg)
{
	CVfx_Glanix_Skill_IceThrow* pInstance = new CVfx_Glanix_Skill_IceThrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_IceThrow");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_IceThrow::Free()
{
	__super::Free();

	if (nullptr != m_pEv3_SpringUp)
	{
		m_pEv3_SpringUp->Start_Dissolve(73, // Index
			_float4(1.f, 1.f, 1.f, 1.f),          // Color
			4.f,   // Speed
			10.f); // Total

		Safe_Release(m_pEv3_SpringUp);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}