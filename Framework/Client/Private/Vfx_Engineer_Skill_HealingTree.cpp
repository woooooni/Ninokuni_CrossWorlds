#include "stdafx.h"
#include "Vfx_Engineer_Skill_HealingTree.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Engineer_Skill_HealingTree::CVfx_Engineer_Skill_HealingTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_HealingTree::CVfx_Engineer_Skill_HealingTree(const CVfx_Engineer_Skill_HealingTree& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_HealingTree::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_0;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET1_E_DOME] = 0;
		m_pPositionOffset[TYPE_ET1_E_DOME] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_DOME] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_DOME] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_CROSS] = 0;
		m_pPositionOffset[TYPE_ET1_E_CROSS] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CROSS] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_CROSS] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_HealingTree::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_HealingTree::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET1_E_DOME && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_DOME])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CROSS && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CROSS])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_HealingTree::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_HealingTree::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_HealingTree::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_HealingTree* CVfx_Engineer_Skill_HealingTree::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_HealingTree* pInstance = new CVfx_Engineer_Skill_HealingTree(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_HealingTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_HealingTree::Clone(void* pArg)
{
	CVfx_Engineer_Skill_HealingTree* pInstance = new CVfx_Engineer_Skill_HealingTree(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_HealingTree");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_HealingTree::Free()
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