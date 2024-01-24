#include "stdafx.h"
#include "Vfx_Destroyer_Skill_FrengeCharge.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_FrengeCharge::CVfx_Destroyer_Skill_FrengeCharge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_FrengeCharge::CVfx_Destroyer_Skill_FrengeCharge(const CVfx_Destroyer_Skill_FrengeCharge& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_0;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_P_SPARCKE] = 0;
		m_pPositionOffset[TYPE_ET1_P_SPARCKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_SPARCKE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_SPARCKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_TRAIL] = 0;
		m_pPositionOffset[TYPE_ET1_E_TRAIL] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_TRAIL] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_TRAIL] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_CRACK] = 0;
		m_pPositionOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_CRACK] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_ELECT] = 0;
		m_pPositionOffset[TYPE_ET2_E_ELECT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_ELECT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_ELECT] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_E_CIRCLELINE] = 0;
		m_pPositionOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_CIRCLELINE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLESRIGID] = 0;
		m_pPositionOffset[TYPE_ET3_P_CIRCLESRIGID] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLESRIGID] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLESRIGID] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_FrengeCharge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_P_SPARCKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_SPARCKE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_TRAIL && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_TRAIL])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_D_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_CRACK])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_ELECT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_ELECT])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_CIRCLELINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLES])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLESRIGID && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLESRIGID])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_FrengeCharge::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_FrengeCharge* CVfx_Destroyer_Skill_FrengeCharge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_FrengeCharge* pInstance = new CVfx_Destroyer_Skill_FrengeCharge(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_FrengeCharge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_FrengeCharge::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_FrengeCharge* pInstance = new CVfx_Destroyer_Skill_FrengeCharge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_FrengeCharge");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_FrengeCharge::Free()
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