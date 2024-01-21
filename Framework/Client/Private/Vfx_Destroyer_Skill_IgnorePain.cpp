#include "stdafx.h"
#include "Vfx_Destroyer_Skill_IgnorePain.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_IgnorePain::CVfx_Destroyer_Skill_IgnorePain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_IgnorePain::CVfx_Destroyer_Skill_IgnorePain(const CVfx_Destroyer_Skill_IgnorePain& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_IgnorePain::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_1;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE] = 0;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_BARRIER] = 0;
		m_pPositionOffset[TYPE_ET1_E_BARRIER] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_BARRIER] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_BARRIER] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_SPARCKE] = 0;
		m_pPositionOffset[TYPE_ET2_P_SPARCKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SPARCKE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_SPARCKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 0;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_IgnorePain::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_IgnorePain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_BARRIER && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_BARRIER])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_SPARCKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SPARCKE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_IgnorePain::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_IgnorePain::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_IgnorePain::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_IgnorePain* CVfx_Destroyer_Skill_IgnorePain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_IgnorePain* pInstance = new CVfx_Destroyer_Skill_IgnorePain(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_IgnorePain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_IgnorePain::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_IgnorePain* pInstance = new CVfx_Destroyer_Skill_IgnorePain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_IgnorePain");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_IgnorePain::Free()
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