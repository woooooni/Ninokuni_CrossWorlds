#include "stdafx.h"
#include "Vfx_Engineer_Skill_TimeLab.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Engineer_Skill_TimeLab::CVfx_Engineer_Skill_TimeLab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_TimeLab::CVfx_Engineer_Skill_TimeLab(const CVfx_Engineer_Skill_TimeLab& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_TimeLab::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_1;

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


		m_pFrameTriger[TYPE_ET1_E_TIME_BIG] = 0;
		m_pPositionOffset[TYPE_ET1_E_TIME_BIG] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_TIME_BIG] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_TIME_BIG] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_LIGHT] = 0;
		m_pPositionOffset[TYPE_ET1_P_LIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_LIGHT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_LIGHT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_TIME_SMALL] = 0;
		m_pPositionOffset[TYPE_ET2_E_TIME_SMALL] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_TIME_SMALL] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_TIME_SMALL] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_LIGHTLINE] = 0;
		m_pPositionOffset[TYPE_ET2_P_LIGHTLINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_LIGHTLINE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_LIGHTLINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_TimeLab::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_TimeLab::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET1_E_TIME_BIG && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_TIME_BIG])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_LIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_LIGHT])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_TIME_SMALL && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_TIME_SMALL])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_LIGHTLINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_LIGHTLINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_TimeLab::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_TimeLab::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_TimeLab::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_TimeLab* CVfx_Engineer_Skill_TimeLab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_TimeLab* pInstance = new CVfx_Engineer_Skill_TimeLab(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_TimeLab");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_TimeLab::Clone(void* pArg)
{
	CVfx_Engineer_Skill_TimeLab* pInstance = new CVfx_Engineer_Skill_TimeLab(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_TimeLab");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_TimeLab::Free()
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