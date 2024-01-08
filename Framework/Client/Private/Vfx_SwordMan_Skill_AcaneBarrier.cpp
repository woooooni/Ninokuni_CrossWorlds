#include "stdafx.h"
#include "Vfx_SwordMan_Skill_AcaneBarrier.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_AcaneBarrier::CVfx_SwordMan_Skill_AcaneBarrier(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_AcaneBarrier::CVfx_SwordMan_Skill_AcaneBarrier(const CVfx_SwordMan_Skill_AcaneBarrier& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_0;
	m_iMaxCount = 12;

	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	m_pFrameTriger[0] = 0;
	m_pPositionOffset[0] = _float3(0.f, 0.f, 0.2f);
	m_pScaleOffset[0] = _float3(5.f, 5.f, 5.f);
	m_pRotationOffset[0] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_AcaneBarrier::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// 
		if (m_iCount == 0 && m_iOwnerFrame >= m_pFrameTriger[0])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT(""), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[0], m_pScaleOffset[0], m_pRotationOffset[0]);
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 1)
			m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_AcaneBarrier::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_AcaneBarrier* CVfx_SwordMan_Skill_AcaneBarrier::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_AcaneBarrier* pInstance = new CVfx_SwordMan_Skill_AcaneBarrier(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_AcaneBarrier");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_AcaneBarrier::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_AcaneBarrier* pInstance = new CVfx_SwordMan_Skill_AcaneBarrier(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_AcaneBarrier");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_AcaneBarrier::Free()
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