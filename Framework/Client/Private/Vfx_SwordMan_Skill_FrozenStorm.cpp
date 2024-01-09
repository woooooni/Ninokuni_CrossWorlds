#include "stdafx.h"
#include "Vfx_SwordMan_Skill_FrozenStorm.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_FrozenStorm::CVfx_SwordMan_Skill_FrozenStorm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_FrozenStorm::CVfx_SwordMan_Skill_FrozenStorm(const CVfx_SwordMan_Skill_FrozenStorm& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_FrozenStorm::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_1;
	m_iMaxCount = 2;

	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0 : Decal 1
	m_pFrameTriger[0]    = 0;
	m_pPositionOffset[0] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[0]    = _float3(7.f, 5.f, 7.f);
	m_pRotationOffset[0] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_FrozenStorm::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_FrozenStorm::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// 1. Decal
		if (m_iCount == 0 && m_iOwnerFrame >= m_pFrameTriger[0])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_FrozenStorm_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[0], m_pScaleOffset[0], m_pRotationOffset[0]);
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 1)
			m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_FrozenStorm::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_FrozenStorm::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_FrozenStorm::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_FrozenStorm* CVfx_SwordMan_Skill_FrozenStorm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_FrozenStorm* pInstance = new CVfx_SwordMan_Skill_FrozenStorm(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_FrozenStorm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_FrozenStorm::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_FrozenStorm* pInstance = new CVfx_SwordMan_Skill_FrozenStorm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_FrozenStorm");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_FrozenStorm::Free()
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