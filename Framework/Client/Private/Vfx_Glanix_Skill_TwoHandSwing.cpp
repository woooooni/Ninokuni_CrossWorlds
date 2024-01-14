#include "stdafx.h"
#include "Vfx_Glanix_Skill_TwoHandSwing.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"

CVfx_Glanix_Skill_TwoHandSwing::CVfx_Glanix_Skill_TwoHandSwing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_TwoHandSwing::CVfx_Glanix_Skill_TwoHandSwing(const CVfx_Glanix_Skill_TwoHandSwing& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_TwoHandSwing::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_ATTACK1;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_E_TRAIL_01]    = 40;
	m_pPositionOffset[TYPE_E_TRAIL_01] = _float3(0.f, 0.3f, 0.f);
	m_pScaleOffset[TYPE_E_TRAIL_01]    = _float3(6.f, 5.5f, 6.f);
	m_pRotationOffset[TYPE_E_TRAIL_01] = _float3(0.f, 10.f, 10.f);

	// 1
	m_pFrameTriger[TYPE_P_SMOKE_01_01]    = 42;//¿À-¿Þ
	m_pPositionOffset[TYPE_P_SMOKE_01_01] = _float3(1.f, 2.f, 1.5f);
	m_pScaleOffset[TYPE_P_SMOKE_01_01]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_01_01] = _float3(0.f, 0.f, 0.f);

	// 2
	m_pFrameTriger[TYPE_P_SMOKE_01_02] = 43;
	m_pPositionOffset[TYPE_P_SMOKE_01_02] = _float3(0.f, 1.5f, 1.5f);
	m_pScaleOffset[TYPE_P_SMOKE_01_02] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_01_02] = _float3(0.f, 0.f, 0.f);

	// 3
	m_pFrameTriger[TYPE_P_SMOKE_01_03] = 44;
	m_pPositionOffset[TYPE_P_SMOKE_01_03] = _float3(-1.f, 1.f, 1.5f);
	m_pScaleOffset[TYPE_P_SMOKE_01_03] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_01_03] = _float3(0.f, 0.f, 0.f);

	// 4
	m_pFrameTriger[TYPE_P_CIRCLE_01]    = 45;
	m_pPositionOffset[TYPE_P_CIRCLE_01] = _float3(0.f, 1.5f, 0.5f);
	m_pScaleOffset[TYPE_P_CIRCLE_01]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLE_01] = _float3(0.f, 0.f, 0.f);

	// 5
	m_pFrameTriger[TYPE_E_TRAIL_02]    = 90; 
	m_pPositionOffset[TYPE_E_TRAIL_02] = _float3(0.f, 0.5f, 0.f);
	m_pScaleOffset[TYPE_E_TRAIL_02]    = _float3(6.f, 5.5f, 6.f);
	m_pRotationOffset[TYPE_E_TRAIL_02] = _float3(0.f, -10.f, 25.f);

	// 6
	m_pFrameTriger[TYPE_P_SMOKE_02_01]    = 91; // ¿Þ->¿À
	m_pPositionOffset[TYPE_P_SMOKE_02_01] = _float3(-1.f, 2.f, 1.5f);
	m_pScaleOffset[TYPE_P_SMOKE_02_01]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_02_01] = _float3(0.f, 0.f, 0.f);

	// 7
	m_pFrameTriger[TYPE_P_SMOKE_02_02]    = 92;
	m_pPositionOffset[TYPE_P_SMOKE_02_02] = _float3(0.f, 2.5f, 1.5f);
	m_pScaleOffset[TYPE_P_SMOKE_02_02]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_02_02] = _float3(0.f, 0.f, 0.f);

	// 8
	m_pFrameTriger[TYPE_P_SMOKE_02_03]    = 93;
	m_pPositionOffset[TYPE_P_SMOKE_02_03] = _float3(1.f, 3.f, 1.5f);
	m_pScaleOffset[TYPE_P_SMOKE_02_03]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_02_03] = _float3(0.f, 0.f, 0.f);

	// 9
	m_pFrameTriger[TYPE_P_CIRCLE_02]    = 94;
	m_pPositionOffset[TYPE_P_CIRCLE_02] = _float3(0.f, 2.5f, 0.5f);
	m_pScaleOffset[TYPE_P_CIRCLE_02]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLE_02] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_TwoHandSwing::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_TwoHandSwing::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_E_TRAIL_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_TwoHandSwing_Trail_Left"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_01], m_pScaleOffset[TYPE_E_TRAIL_01], m_pRotationOffset[TYPE_E_TRAIL_01]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SMOKE_01_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_01_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_01_01], m_pScaleOffset[TYPE_P_SMOKE_01_01], m_pRotationOffset[TYPE_P_SMOKE_01_01]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_01_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_01_02])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_01_02], m_pScaleOffset[TYPE_P_SMOKE_01_02], m_pRotationOffset[TYPE_P_SMOKE_01_02]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_01_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_01_03])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_01_03], m_pScaleOffset[TYPE_P_SMOKE_01_03], m_pRotationOffset[TYPE_P_SMOKE_01_03]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_CIRCLE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLE_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLE_01], m_pScaleOffset[TYPE_P_CIRCLE_01], m_pRotationOffset[TYPE_P_CIRCLE_01]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAIL_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_02])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_TwoHandSwing_Trail_Right"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_02], m_pScaleOffset[TYPE_E_TRAIL_02], m_pRotationOffset[TYPE_E_TRAIL_02]);
			m_iCount++;
		}
		
		else if (m_iCount == TYPE_P_SMOKE_02_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_02_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_02_01], m_pScaleOffset[TYPE_P_SMOKE_02_01], m_pRotationOffset[TYPE_P_SMOKE_02_01]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_02_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_02_02])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_02_02], m_pScaleOffset[TYPE_P_SMOKE_02_02], m_pRotationOffset[TYPE_P_SMOKE_02_02]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_02_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_02_03])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_02_03], m_pScaleOffset[TYPE_P_SMOKE_02_03], m_pRotationOffset[TYPE_P_SMOKE_02_03]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_CIRCLE_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLE_02])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_TwoHandSwing_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLE_02], m_pScaleOffset[TYPE_P_CIRCLE_02], m_pRotationOffset[TYPE_P_CIRCLE_02]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_TwoHandSwing::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_TwoHandSwing::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_TwoHandSwing::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_TwoHandSwing* CVfx_Glanix_Skill_TwoHandSwing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_TwoHandSwing* pInstance = new CVfx_Glanix_Skill_TwoHandSwing(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_TwoHandSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_TwoHandSwing::Clone(void* pArg)
{
	CVfx_Glanix_Skill_TwoHandSwing* pInstance = new CVfx_Glanix_Skill_TwoHandSwing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_TwoHandSwing");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_TwoHandSwing::Free()
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