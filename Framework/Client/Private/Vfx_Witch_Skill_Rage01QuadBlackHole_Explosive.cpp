#include "stdafx.h"
#include "Vfx_Witch_Skill_Rage01QuadBlackHole_Explosive.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Particle.h"

CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive(const CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::Initialize_Prototype()
{
	// m_bOwnerStateIndex = CStellia::STELLIA_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	m_pFrameTriger[TYPE_D_WARNING] = 0;
	m_pPositionOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_WARNING]    = _float3(15.f, 3.f, 15.f);
	m_pRotationOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);

	// 0
	m_pPositionOffset[TYPE_E_SPHERE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE]    = _float3(0.1f, 0.1f, 0.1f);
	m_pRotationOffset[TYPE_E_SPHERE] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pPositionOffset[TYPE_E_DRAIN] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_DRAIN]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_DRAIN] = _float3(0.f, 0.f, 0.f); 


	// 2
	m_pPositionOffset[TYPE_E_SPHERE_SMALLER] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_SMALLER]    = _float3(1.6f, 1.6f, 1.6f);
	m_pRotationOffset[TYPE_E_SPHERE_SMALLER] = _float3(0.f, 0.f, 0.f);

	// 3
	m_pPositionOffset[TYPE_E_DRAND_SMALLER] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_DRAND_SMALLER]    = _float3(3.f, 3.f, 3.f);
	m_pRotationOffset[TYPE_E_DRAND_SMALLER] = _float3(0.f, 0.f, 0.f);


	// 4
	m_pPositionOffset[TYPE_E_SPHERE_SMALLER02] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_SMALLER02]    = _float3(1.6f, 1.6f, 1.6f);
	m_pRotationOffset[TYPE_E_SPHERE_SMALLER02] = _float3(0.f, 0.f, 0.f);

	// 5
	m_pPositionOffset[TYPE_E_DRAND_SMALLER02] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_DRAND_SMALLER02]    = _float3(3.f, 3.f, 3.f);
	m_pRotationOffset[TYPE_E_DRAND_SMALLER02] = _float3(0.f, 0.f, 0.f);


	m_pPositionOffset[TYPE_E_SMOKELINE] = _float3(0.f, -0.5f, 0.f);
	m_pScaleOffset[TYPE_E_SMOKELINE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_SMOKELINE] = _float3(0.f, 0.f, 0.f);

	m_pPositionOffset[TYPE_P_ELECT] = _float3(0.f, 0.5f, 0.f);
	m_pScaleOffset[TYPE_P_ELECT]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_ELECT] = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::Tick(_float fTimeDelta)
{
	// 블랙홀 최초 생성
	if (!m_bIsCreate && m_pBlackHole == nullptr)
	{
		// Decal
		GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_Warning"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_WARNING], m_pScaleOffset[TYPE_D_WARNING], m_pRotationOffset[TYPE_D_WARNING]);

		// Marble
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_Sphere"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE], m_pScaleOffset[TYPE_E_SPHERE], m_pRotationOffset[TYPE_E_SPHERE], nullptr, &m_pBlackHole);
		if (nullptr != m_pBlackHole)
			m_pBlackHole->Set_Color(_float3(0.877, 0.4f, 1.f));
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_Sphere"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE], m_pScaleOffset[TYPE_E_SPHERE], m_pRotationOffset[TYPE_E_SPHERE]);

		// OutLine
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_Drain"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_DRAIN], m_pScaleOffset[TYPE_E_DRAIN], m_pRotationOffset[TYPE_E_DRAIN]);


		// Effect
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_SmokeLine"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SMOKELINE], m_pScaleOffset[TYPE_E_SMOKELINE], m_pRotationOffset[TYPE_E_SMOKELINE]);
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_QuadBalckHole_Electricity"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_ELECT], m_pScaleOffset[TYPE_P_ELECT], m_pRotationOffset[TYPE_P_ELECT], nullptr, &m_pElect);
		Safe_AddRef(m_pElect);

		m_bIsCreate = true;
	}
	else if(!m_bIsSmaller && m_pBlackHoleSmaller == nullptr)
	{
		// 블랙홀 이펙트가 끝났다면
		if (m_pBlackHole->Is_Dead() || m_pBlackHole->Get_DieEffect())
		{
			// 작아지는 블랙홀 생성
			if (!m_bIsSmaller && m_pBlackHoleSmaller == nullptr)
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_Sphere_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_SMALLER], m_pScaleOffset[TYPE_E_SPHERE_SMALLER], m_pRotationOffset[TYPE_E_SPHERE_SMALLER], nullptr, &m_pBlackHoleSmaller);
				if (nullptr != m_pBlackHoleSmaller)
					m_pBlackHoleSmaller->Set_Color(_float3(0.877, 0.4f, 1.f));
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_Sphere_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_SMALLER], m_pScaleOffset[TYPE_E_SPHERE_SMALLER], m_pRotationOffset[TYPE_E_SPHERE_SMALLER]);


				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_Drain_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_DRAND_SMALLER], m_pScaleOffset[TYPE_E_DRAND_SMALLER], m_pRotationOffset[TYPE_E_DRAND_SMALLER]);
		
				m_bIsSmaller = true;
			}
		}
	}
	// 작아지는 이펙트가 끝났다면
	else if (!m_bIsSmaller02 && m_pBlackHoleSmaller02 == nullptr)
	{
		if (m_pBlackHoleSmaller->Is_Dead() || m_pBlackHoleSmaller->Get_DieEffect())
		{
			if (nullptr != m_pElect)
			{
				m_pElect->Set_Dead(true);
				Safe_Release(m_pElect);
			}

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_Sphere_Smaller02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_SMALLER02], m_pScaleOffset[TYPE_E_SPHERE_SMALLER02], m_pRotationOffset[TYPE_E_SPHERE_SMALLER02], nullptr, &m_pBlackHoleSmaller02);
			if (nullptr != m_pBlackHoleSmaller02)
				m_pBlackHoleSmaller02->Set_Color(_float3(0.877, 0.4f, 1.f));
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_Sphere_Smaller02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_SMALLER02], m_pScaleOffset[TYPE_E_SPHERE_SMALLER02], m_pRotationOffset[TYPE_E_SPHERE_SMALLER02]);

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_QuadBalckHole_Drain_Smaller02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_DRAND_SMALLER], m_pScaleOffset[TYPE_E_DRAND_SMALLER02], m_pRotationOffset[TYPE_E_DRAND_SMALLER02]);

			m_bIsSmaller02 = true;
		}
	}

	// 모든 블랙홀이 끝났다면 
	if (m_pBlackHoleSmaller02 != nullptr)
	{
		if (m_pBlackHoleSmaller02->Is_Dead() || m_pBlackHoleSmaller02->Get_DieEffect())
		{
			Set_Dead(true);
		}
	}
}

void CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive* CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive* pInstance = new CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::Clone(void* pArg)
{
	CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive* pInstance = new CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Skill_Rage01QuadBlackHole_Explosive::Free()
{
	__super::Free();

	if (nullptr != m_pElect)
	{
		m_pElect->Set_Dead(true);
		Safe_Release(m_pElect);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}