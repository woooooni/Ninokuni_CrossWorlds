#include "stdafx.h"
#include "Vfx_Destroyer_Skill_BrutalStrike.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_BrutalStrike::CVfx_Destroyer_Skill_BrutalStrike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_BrutalStrike::CVfx_Destroyer_Skill_BrutalStrike(const CVfx_Destroyer_Skill_BrutalStrike& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_BrutalStrike::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_1;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_RECT]    = 0;
		m_pPositionOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_RECT]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_SMOKED]    = 10;
		m_pPositionOffset[TYPE_ET2_D_SMOKED] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_SMOKED]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_SMOKED] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_P_HITUP_01] = 0;
		m_pPositionOffset[TYPE_ET2_P_HITUP_01] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_HITUP_01] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_HITUP_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_HITUP_02] = 0;
		m_pPositionOffset[TYPE_ET2_P_HITUP_02] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_HITUP_02] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_HITUP_02] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_HITUP_03] = 0;
		m_pPositionOffset[TYPE_ET2_P_HITUP_03] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_HITUP_03] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_HITUP_03] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_HITUP_04] = 0;
		m_pPositionOffset[TYPE_ET2_P_HITUP_04] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_HITUP_04] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_HITUP_04] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_P_FIRE] = 0;
		m_pPositionOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_SMOKELINE] = 0;
		m_pPositionOffset[TYPE_ET2_E_SMOKELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_SMOKELINE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_SMOKELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_D_SMOKED] = 0;
		m_pPositionOffset[TYPE_ET3_D_SMOKED] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_D_SMOKED] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_D_SMOKED] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET3_P_HITUP_01] = 0;
		m_pPositionOffset[TYPE_ET3_P_HITUP_01] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_HITUP_01] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_HITUP_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_HITUP_02] = 0;
		m_pPositionOffset[TYPE_ET3_P_HITUP_02] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_HITUP_02] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_HITUP_02] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_HITUP_03] = 0;
		m_pPositionOffset[TYPE_ET3_P_HITUP_03] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_HITUP_03] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_HITUP_03] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_HITUP_04] = 0;
		m_pPositionOffset[TYPE_ET3_P_HITUP_04] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_HITUP_04] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_HITUP_04] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET3_P_FIRE] = 0;
		m_pPositionOffset[TYPE_ET3_P_FIRE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_FIRE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_E_SMOKELINE] = 0;
		m_pPositionOffset[TYPE_ET3_E_SMOKELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_SMOKELINE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_E_SMOKELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_BrutalStrike::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_BrutalStrike::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_RECT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_RECT])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_SipohoningLunge_Square"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_RECT], m_pScaleOffset[TYPE_ET1_D_RECT], m_pRotationOffset[TYPE_ET1_D_RECT]);
			m_iCount++;
		}
		
		else if (m_iCount == TYPE_ET2_D_SMOKED && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_SMOKED])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Destroyer_Skill_BrutalStrike_SmokedLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_D_SMOKED], m_pScaleOffset[TYPE_ET2_D_SMOKED], m_pRotationOffset[TYPE_ET2_D_SMOKED]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_HITUP_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_HITUP_01])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_HITUP_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_HITUP_02])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_HITUP_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_HITUP_03])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_HITUP_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_HITUP_04])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_FIRE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_SMOKELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_SMOKELINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_D_SMOKED && m_iOwnerFrame    >= m_pFrameTriger[TYPE_ET3_D_SMOKED])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Destroyer_Skill_BrutalStrike_SmokedLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_D_SMOKED], m_pScaleOffset[TYPE_ET3_D_SMOKED], m_pRotationOffset[TYPE_ET3_D_SMOKED]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_HITUP_01 && m_iOwnerFrame  >= m_pFrameTriger[TYPE_ET3_P_HITUP_01])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_HITUP_02 && m_iOwnerFrame  >= m_pFrameTriger[TYPE_ET3_P_HITUP_02])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_HITUP_03 && m_iOwnerFrame  >= m_pFrameTriger[TYPE_ET3_P_HITUP_03])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_HITUP_04 && m_iOwnerFrame  >= m_pFrameTriger[TYPE_ET3_P_HITUP_04])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_FIRE && m_iOwnerFrame      >= m_pFrameTriger[TYPE_ET3_P_FIRE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_E_SMOKELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_SMOKELINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLES && m_iOwnerFrame   >= m_pFrameTriger[TYPE_ET3_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_BrutalStrike::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_BrutalStrike::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_BrutalStrike::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_BrutalStrike* CVfx_Destroyer_Skill_BrutalStrike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_BrutalStrike* pInstance = new CVfx_Destroyer_Skill_BrutalStrike(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_BrutalStrike");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_BrutalStrike::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_BrutalStrike* pInstance = new CVfx_Destroyer_Skill_BrutalStrike(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_BrutalStrike");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_BrutalStrike::Free()
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