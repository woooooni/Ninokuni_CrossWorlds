#include "stdafx.h"
#include "Vfx_Destroyer_Skill_BrutalStrike.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Decal.h"

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
		m_pPositionOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.5f);
		m_pScaleOffset[TYPE_ET1_D_RECT]    = _float3(4.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_SMOKED] = 12;
		m_pPositionOffset[TYPE_ET2_D_SMOKED] = _float3(0.25f, 0.f, 0.5f);
		m_pScaleOffset[TYPE_ET2_D_SMOKED] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_SMOKED] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_FIRE] = 14;
		m_pPositionOffset[TYPE_ET2_P_FIRE] = _float3(1.f, 0.5f, 1.8f);
		m_pScaleOffset[TYPE_ET2_P_FIRE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_SMOKELINE] = 14;
		m_pPositionOffset[TYPE_ET2_E_SMOKELINE] = _float3(-30.f, 0.5f, -50.f);
		m_pScaleOffset[TYPE_ET2_E_SMOKELINE] = _float3(0.05f, 0.05f, 0.05f);
		m_pRotationOffset[TYPE_ET2_E_SMOKELINE] = _float3(0.f, 180.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_STONE]    = 14;
		m_pPositionOffset[TYPE_ET2_P_STONE] = _float3(1.f, 0.f, 2.f);
		m_pScaleOffset[TYPE_ET2_P_STONE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_STONE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES]    = 14;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(1.f, 0.f, 2.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_HITUP_01] = 15;
		m_pPositionOffset[TYPE_ET2_P_HITUP_01] = _float3(0.9f, 0.5f, 1.2f);
		m_pScaleOffset[TYPE_ET2_P_HITUP_01] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET2_P_HITUP_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_HITUP_02] = 17;
		m_pPositionOffset[TYPE_ET2_P_HITUP_02] = _float3(0.89f, 0.5f, 1.5f);
		m_pScaleOffset[TYPE_ET2_P_HITUP_02] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET2_P_HITUP_02] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_HITUP_03] = 19;
		m_pPositionOffset[TYPE_ET2_P_HITUP_03] = _float3(0.88f, 0.5f, 1.8f);
		m_pScaleOffset[TYPE_ET2_P_HITUP_03] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET2_P_HITUP_03] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_HITUP_04] = 21;
		m_pPositionOffset[TYPE_ET2_P_HITUP_04] = _float3(0.88f, 0.5f, 2.1f);
		m_pScaleOffset[TYPE_ET2_P_HITUP_04] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET2_P_HITUP_04] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_HITUP_05] = 23;
		m_pPositionOffset[TYPE_ET2_P_HITUP_05] = _float3(0.88f, 0.5f, 2.4f);
		m_pScaleOffset[TYPE_ET2_P_HITUP_05] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET2_P_HITUP_05] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_D_SMOKED] = 27;
		m_pPositionOffset[TYPE_ET3_D_SMOKED] = _float3(-0.25f, 0.f, 0.5f);
		m_pScaleOffset[TYPE_ET3_D_SMOKED] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_D_SMOKED] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_P_FIRE] = 29;
		m_pPositionOffset[TYPE_ET3_P_FIRE] = _float3(-1.f, 0.5f, 1.8f);
		m_pScaleOffset[TYPE_ET3_P_FIRE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_E_SMOKELINE] = 29;
		m_pPositionOffset[TYPE_ET3_E_SMOKELINE] = _float3(-30.f, 0.5f, 50.f);
		m_pScaleOffset[TYPE_ET3_E_SMOKELINE] = _float3(0.05f, 0.05f, 0.05f);
		m_pRotationOffset[TYPE_ET3_E_SMOKELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_STONE] = 29;
		m_pPositionOffset[TYPE_ET3_P_STONE] = _float3(-1.f, 0.f, 2.f);
		m_pScaleOffset[TYPE_ET3_P_STONE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_STONE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES] = 29;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES] = _float3(-1.f, 0.f, 2.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_P_HITUP_01] = 30;
		m_pPositionOffset[TYPE_ET3_P_HITUP_01] = _float3(-0.9f, 0.5f, 1.2f);
		m_pScaleOffset[TYPE_ET3_P_HITUP_01] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET3_P_HITUP_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_HITUP_02] = 32;
		m_pPositionOffset[TYPE_ET3_P_HITUP_02] = _float3(-0.89f, 0.5f, 1.5f);
		m_pScaleOffset[TYPE_ET3_P_HITUP_02] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET3_P_HITUP_02] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_HITUP_03] = 34;
		m_pPositionOffset[TYPE_ET3_P_HITUP_03] = _float3(-0.88f, 0.5f, 1.8f);
		m_pScaleOffset[TYPE_ET3_P_HITUP_03] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET3_P_HITUP_03] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_HITUP_04] = 36;
		m_pPositionOffset[TYPE_ET3_P_HITUP_04] = _float3(-0.88f, 0.5f, 2.1f);
		m_pScaleOffset[TYPE_ET3_P_HITUP_04] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET3_P_HITUP_04] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_HITUP_05] = 38;
		m_pPositionOffset[TYPE_ET3_P_HITUP_05] = _float3(-0.88f, 0.5f, 2.4f);
		m_pScaleOffset[TYPE_ET3_P_HITUP_05] = _float3(1.2f, 1.5f, 1.2f);
		m_pRotationOffset[TYPE_ET3_P_HITUP_05] = _float3(0.f, 0.f, 0.f);
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
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_RECT], m_pScaleOffset[TYPE_ET1_D_RECT], m_pRotationOffset[TYPE_ET1_D_RECT], nullptr, &m_pEt1_D_RECT, false);
			Safe_AddRef(m_pEt1_D_RECT);
			m_iCount++;
		}
		// -----------------------------------------------------------------------------------------------------------
		else if (m_iCount == TYPE_ET2_D_SMOKED && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_SMOKED])
		{
			if (nullptr != m_pEt1_D_RECT)
			{
				m_pEt1_D_RECT->Start_AlphaDeleate();
				Safe_Release(m_pEt1_D_RECT);
			}

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Destroyer_Skill_BrutalStrike_SmokedLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_D_SMOKED], m_pScaleOffset[TYPE_ET2_D_SMOKED], m_pRotationOffset[TYPE_ET2_D_SMOKED]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_FIRE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_BrutalStrike_Fire"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_FIRE], m_pScaleOffset[TYPE_ET2_P_FIRE], m_pRotationOffset[TYPE_ET2_P_FIRE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_SMOKELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_SMOKELINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_CirecleLine_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_SMOKELINE], m_pScaleOffset[TYPE_ET2_E_SMOKELINE], m_pRotationOffset[TYPE_ET2_E_SMOKELINE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_STONE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_STONE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_BrutalStrike_Stone"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_STONE], m_pScaleOffset[TYPE_ET2_P_STONE], m_pRotationOffset[TYPE_ET2_P_STONE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_BrutalStrike_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_HITUP_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_HITUP_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_HITUP_01], m_pScaleOffset[TYPE_ET2_P_HITUP_01], m_pRotationOffset[TYPE_ET2_P_HITUP_01]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_HITUP_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_HITUP_02])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_HITUP_02], m_pScaleOffset[TYPE_ET2_P_HITUP_02], m_pRotationOffset[TYPE_ET2_P_HITUP_02]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_HITUP_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_HITUP_03])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_HITUP_03], m_pScaleOffset[TYPE_ET2_P_HITUP_03], m_pRotationOffset[TYPE_ET2_P_HITUP_03]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_HITUP_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_HITUP_04])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_HITUP_04], m_pScaleOffset[TYPE_ET2_P_HITUP_04], m_pRotationOffset[TYPE_ET2_P_HITUP_04]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_HITUP_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_HITUP_05])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_HITUP_05], m_pScaleOffset[TYPE_ET2_P_HITUP_05], m_pRotationOffset[TYPE_ET2_P_HITUP_05]);
			m_iCount++;
		}

		// -----------------------------------------------------------------------------------------------------------
		else if (m_iCount == TYPE_ET3_D_SMOKED && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_D_SMOKED])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Destroyer_Skill_BrutalStrike_SmokedLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_D_SMOKED], m_pScaleOffset[TYPE_ET3_D_SMOKED], m_pRotationOffset[TYPE_ET3_D_SMOKED]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_P_FIRE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_FIRE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_BrutalStrike_Fire"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_FIRE], m_pScaleOffset[TYPE_ET3_P_FIRE], m_pRotationOffset[TYPE_ET3_P_FIRE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_E_SMOKELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_SMOKELINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_CirecleLine_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_E_SMOKELINE], m_pScaleOffset[TYPE_ET3_E_SMOKELINE], m_pRotationOffset[TYPE_ET3_E_SMOKELINE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_STONE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_STONE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_BrutalStrike_Stone"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_STONE], m_pScaleOffset[TYPE_ET3_P_STONE], m_pRotationOffset[TYPE_ET3_P_STONE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_BrutalStrike_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CIRCLES], m_pScaleOffset[TYPE_ET3_P_CIRCLES], m_pRotationOffset[TYPE_ET3_P_CIRCLES]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_P_HITUP_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_HITUP_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_HITUP_01], m_pScaleOffset[TYPE_ET3_P_HITUP_01], m_pRotationOffset[TYPE_ET3_P_HITUP_01]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_HITUP_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_HITUP_02])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_HITUP_02], m_pScaleOffset[TYPE_ET3_P_HITUP_02], m_pRotationOffset[TYPE_ET3_P_HITUP_02]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_HITUP_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_HITUP_03])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_HITUP_03], m_pScaleOffset[TYPE_ET3_P_HITUP_03], m_pRotationOffset[TYPE_ET3_P_HITUP_03]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_HITUP_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_HITUP_04])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_HITUP_04], m_pScaleOffset[TYPE_ET3_P_HITUP_04], m_pRotationOffset[TYPE_ET3_P_HITUP_04]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_HITUP_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_HITUP_05])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BrutalStrike_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_HITUP_05], m_pScaleOffset[TYPE_ET3_P_HITUP_05], m_pRotationOffset[TYPE_ET3_P_HITUP_05]);
			m_iCount++;
		}
		// -----------------------------------------------------------------------------------------------------------

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

	if (nullptr != m_pEt1_D_RECT)
	{
		m_pEt1_D_RECT->Start_AlphaDeleate();
		Safe_Release(m_pEt1_D_RECT);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}