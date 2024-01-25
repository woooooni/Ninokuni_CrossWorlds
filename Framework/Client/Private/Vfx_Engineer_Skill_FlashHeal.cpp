#include "stdafx.h"
#include "Vfx_Engineer_Skill_FlashHeal.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Engineer_Skill_FlashHeal::CVfx_Engineer_Skill_FlashHeal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_FlashHeal::CVfx_Engineer_Skill_FlashHeal(const CVfx_Engineer_Skill_FlashHeal& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_FlashHeal::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_0;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET1_P_LIGHT] = 0;
		m_pPositionOffset[TYPE_ET1_P_LIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_LIGHT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_LIGHT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CROSSSGLITTER] = 0;
		m_pPositionOffset[TYPE_ET1_P_CROSSSGLITTER] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CROSSSGLITTER] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_CROSSSGLITTER] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_AURA_01] = 0;
		m_pPositionOffset[TYPE_ET2_E_AURA_01] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_AURA_01] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_AURA_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_AURA_02] = 0;
		m_pPositionOffset[TYPE_ET2_E_AURA_02] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_AURA_02] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_AURA_02] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_AURA_03] = 0;
		m_pPositionOffset[TYPE_ET2_E_AURA_03] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_AURA_03] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_AURA_03] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_AURA_04] = 0;
		m_pPositionOffset[TYPE_ET2_E_AURA_04] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_AURA_04] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_AURA_04] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_AURA_05] = 0;
		m_pPositionOffset[TYPE_ET2_E_AURA_05] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_AURA_05] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_AURA_05] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_AURA_06] = 0;
		m_pPositionOffset[TYPE_ET2_E_AURA_06] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_AURA_06] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_AURA_06] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_FlashHeal::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_FlashHeal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			m_iCount++;
		}
		
		else if (m_iCount == TYPE_ET1_P_LIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_LIGHT])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CROSSSGLITTER && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CROSSSGLITTER])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_AURA_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_AURA_01])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_AURA_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_AURA_02])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_AURA_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_AURA_03])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_AURA_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_AURA_04])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_AURA_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_AURA_05])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_AURA_06 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_AURA_06])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_FlashHeal::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_FlashHeal::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_FlashHeal::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_FlashHeal* CVfx_Engineer_Skill_FlashHeal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_FlashHeal* pInstance = new CVfx_Engineer_Skill_FlashHeal(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_FlashHeal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_FlashHeal::Clone(void* pArg)
{
	CVfx_Engineer_Skill_FlashHeal* pInstance = new CVfx_Engineer_Skill_FlashHeal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_FlashHeal");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_FlashHeal::Free()
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