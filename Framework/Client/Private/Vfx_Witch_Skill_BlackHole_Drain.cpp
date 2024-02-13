#include "stdafx.h"
#include "Vfx_Witch_Skill_BlackHole_Drain.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Decal.h"

CVfx_Witch_Skill_BlackHole_Drain::CVfx_Witch_Skill_BlackHole_Drain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Skill_BlackHole_Drain::CVfx_Witch_Skill_BlackHole_Drain(const CVfx_Witch_Skill_BlackHole_Drain& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Skill_BlackHole_Drain::Initialize_Prototype()
{
	// m_bOwnerStateIndex = CStellia::STELLIA_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_E_SPHERE] = 0;
	m_pPositionOffset[TYPE_E_SPHERE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE] = _float3(0.1f, 0.1f, 0.1f);
	m_pRotationOffset[TYPE_E_SPHERE] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_E_DRAIN] = 0;
	m_pPositionOffset[TYPE_E_DRAIN] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_DRAIN] = _float3(2.5f, 2.5f, 2.5f);  
	m_pRotationOffset[TYPE_E_DRAIN] = _float3(0.f, 0.f, 0.f); 

	// 1_2
	m_pFrameTriger[TYPE_D_WARNING] = 0;
	m_pPositionOffset[TYPE_D_WARNING] = _float3(0.f, -2.3f, 0.f);
	m_pScaleOffset[TYPE_D_WARNING] = _float3(4.f, 2.f, 4.f);
	m_pRotationOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);

	// 2
	m_pFrameTriger[TYPE_E_SPHERE_SMALLER] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_SMALLER] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_SMALLER] = _float3(1.1f, 1.1f, 1.1f);
	m_pRotationOffset[TYPE_E_SPHERE_SMALLER] = _float3(0.f, 0.f, 0.f);

	// 3
	m_pFrameTriger[TYPE_E_DRAND_SMALLER] = 0;
	m_pPositionOffset[TYPE_E_DRAND_SMALLER] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_DRAND_SMALLER] = _float3(2.5f, 2.5f, 2.5f);
	m_pRotationOffset[TYPE_E_DRAND_SMALLER] = _float3(0.f, 0.f, 0.f);

	// 4
	m_pFrameTriger[TYPE_E_SPHERE_SMALLER02] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_SMALLER02] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_SMALLER02] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_SPHERE_SMALLER02] = _float3(0.f, 0.f, 0.f);

	// 5
	m_pFrameTriger[TYPE_E_DRAND_SMALLER02] = 0;
	m_pPositionOffset[TYPE_E_DRAND_SMALLER02] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_DRAND_SMALLER02] = _float3(2.5f, 2.5f, 2.5f);
	m_pRotationOffset[TYPE_E_DRAND_SMALLER02] = _float3(0.f, 0.f, 0.f);


	return S_OK;
}

HRESULT CVfx_Witch_Skill_BlackHole_Drain::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Skill_BlackHole_Drain::Tick(_float fTimeDelta)
{
	// 블랙홀 최초 생성
	if (!m_bIsCreate && m_pBlackHole == nullptr)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Sphere"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE], m_pScaleOffset[TYPE_E_SPHERE], m_pRotationOffset[TYPE_E_SPHERE], nullptr, &m_pBlackHole);
	
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Drain"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_DRAIN], m_pScaleOffset[TYPE_E_DRAIN], m_pRotationOffset[TYPE_E_DRAIN]);
	
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
				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Witch_BlackHole"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_WARNING], m_pScaleOffset[TYPE_D_WARNING], m_pRotationOffset[TYPE_D_WARNING], nullptr, &m_pWarningDecal);

				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Sphere_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_SMALLER], m_pScaleOffset[TYPE_E_SPHERE_SMALLER], m_pRotationOffset[TYPE_E_SPHERE_SMALLER], nullptr, &m_pBlackHoleSmaller);

				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Drain_Smaller"),
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
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Sphere_Smaller02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_SMALLER02], m_pScaleOffset[TYPE_E_SPHERE_SMALLER02], m_pRotationOffset[TYPE_E_SPHERE_SMALLER02], nullptr, &m_pBlackHoleSmaller02);

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Drain_Smaller02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_DRAND_SMALLER], m_pScaleOffset[TYPE_E_DRAND_SMALLER02], m_pRotationOffset[TYPE_E_DRAND_SMALLER02]);

			m_bIsSmaller02 = true;
		}
	}

	// 모든 블랙홀이 끝났다면 
	if (m_pBlackHoleSmaller02 != nullptr)
	{
		if (m_pBlackHoleSmaller02->Is_Dead() || m_pBlackHoleSmaller02->Get_DieEffect())
		{
			m_pWarningDecal->Set_Dead(true);
			Set_Dead(true);
		}
	}
}

void CVfx_Witch_Skill_BlackHole_Drain::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Skill_BlackHole_Drain::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Skill_BlackHole_Drain::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Skill_BlackHole_Drain* CVfx_Witch_Skill_BlackHole_Drain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Skill_BlackHole_Drain* pInstance = new CVfx_Witch_Skill_BlackHole_Drain(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Skill_BlackHole_Drain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Skill_BlackHole_Drain::Clone(void* pArg)
{
	CVfx_Witch_Skill_BlackHole_Drain* pInstance = new CVfx_Witch_Skill_BlackHole_Drain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Witch_Skill_BlackHole_Drain");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Skill_BlackHole_Drain::Free()
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