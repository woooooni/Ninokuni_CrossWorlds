#include "stdafx.h"
#include "Vfx_Witch_Skill_BlackHole_Drain.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"

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
	m_pPositionOffset[TYPE_E_SPHERE] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE] = _float3(0.1f, 0.1f, 0.1f);
	m_pRotationOffset[TYPE_E_SPHERE] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_E_DRAIN] = 0;
	m_pPositionOffset[TYPE_E_DRAIN] = _float3(0.f, 0.f, 0.f); 
	m_pScaleOffset[TYPE_E_DRAIN] = _float3(2.5f, 2.5f, 2.5f);  
	m_pRotationOffset[TYPE_E_DRAIN] = _float3(0.f, 0.f, 0.f); 

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


	return S_OK;
}

HRESULT CVfx_Witch_Skill_BlackHole_Drain::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Skill_BlackHole_Drain::Tick(_float fTimeDelta)
{
	// 블랙홀 최초 생성
	if (m_pBlackHole == nullptr)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Sphere"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE], m_pScaleOffset[TYPE_E_SPHERE], m_pRotationOffset[TYPE_E_SPHERE], nullptr, &m_pBlackHole);
	
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Drain"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_DRAIN], m_pScaleOffset[TYPE_E_DRAIN], m_pRotationOffset[TYPE_E_DRAIN]);
	}
	else
	{
		// 블랙홀 이펙트가 끝났다면
		if (m_pBlackHole->Is_Dead() || m_pBlackHole->Get_DieEffect())
		{
			// 작아지는 블랙홀 생성
			if (m_pBlackHoleSmaller == nullptr)
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Sphere_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_SMALLER], m_pScaleOffset[TYPE_E_SPHERE_SMALLER], m_pRotationOffset[TYPE_E_SPHERE_SMALLER], nullptr, &m_pBlackHoleSmaller);

				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Drain_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_DRAND_SMALLER], m_pScaleOffset[TYPE_E_DRAND_SMALLER], m_pRotationOffset[TYPE_E_DRAND_SMALLER]);
			}
		}
	}

	if (m_pBlackHoleSmaller != nullptr)
	{
		// 작아지는 블랙홀이 끝났다면 
		if (m_pBlackHoleSmaller->Is_Dead() || m_pBlackHole->Get_DieEffect())
		{
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