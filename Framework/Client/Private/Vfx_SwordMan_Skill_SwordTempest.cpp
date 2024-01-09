#include "stdafx.h"
#include "Vfx_SwordMan_Skill_SwordTempest.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_SwordTempest::CVfx_SwordMan_Skill_SwordTempest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_SwordTempest::CVfx_SwordMan_Skill_SwordTempest(const CVfx_SwordMan_Skill_SwordTempest& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_SwordTempest::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_2;

	m_iMaxCount = 2;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 1. 
	m_pFrameTriger[0]    = 0;
	m_pPositionOffset[0] = _float3(0.f, 0.f, 0.5f);
	m_pScaleOffset[0]    = _float3(4.f, 5.f, 8.f);
	m_pRotationOffset[0] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SwordTempest::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_SwordTempest::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// 1. 
		if (m_iCount == 0 && m_iOwnerFrame >= m_pFrameTriger[0])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_FrozenStorm_Square"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[0], m_pScaleOffset[0], m_pRotationOffset[0]);
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 1)
			m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_SwordTempest::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_SwordTempest::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SwordTempest::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_SwordTempest* CVfx_SwordMan_Skill_SwordTempest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_SwordTempest* pInstance = new CVfx_SwordMan_Skill_SwordTempest(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_SwordTempest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_SwordTempest::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_SwordTempest* pInstance = new CVfx_SwordMan_Skill_SwordTempest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_SwordTempest");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_SwordTempest::Free()
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