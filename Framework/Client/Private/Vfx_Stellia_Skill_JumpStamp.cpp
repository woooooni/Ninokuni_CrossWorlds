#include "stdafx.h"
#include "Vfx_Stellia_Skill_JumpStamp.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Decal.h"

#include "Game_Manager.h"
#include "Player.h"

CVfx_Stellia_Skill_JumpStamp::CVfx_Stellia_Skill_JumpStamp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_JumpStamp::CVfx_Stellia_Skill_JumpStamp(const CVfx_Stellia_Skill_JumpStamp& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_JumpStamp::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_JUMPSTAMP;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	/* 0. Rising Trail */
	m_pFrameTriger[TYPE_E_RISING_00] = 35;
	m_pPositionOffset[TYPE_E_RISING_00] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_RISING_00] = _float3(9.f, 9.f, 9.f);
	m_pRotationOffset[TYPE_E_RISING_00] = _float3(0.f, 0.f, 0.f);

	/* 1. Rising Smoke */
	m_pFrameTriger[TYPE_E_RISING_01] = 35;
	m_pPositionOffset[TYPE_E_RISING_01] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_RISING_01] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_RISING_01] = _float3(0.f, 0.f, 0.f);

	/* 2. Warning Decal */
	m_pFrameTriger[TYPE_D_WARNING] = 50;
	m_pPositionOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_WARNING] = _float3(15.f, 2.f, 15.f);
	m_pRotationOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_V_DELETE] = 80;
	m_pPositionOffset[TYPE_V_DELETE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_V_DELETE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_V_DELETE] = _float3(0.f, 0.f, 0.f);

	/* 3. Crack Decal */
	m_pFrameTriger[TYPE_D_DECAL_00] = 90;
	m_pPositionOffset[TYPE_D_DECAL_00] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_DECAL_00] = _float3(12.f, 1.f, 12.f);
	m_pRotationOffset[TYPE_D_DECAL_00] = _float3(0.f, 0.f, 0.f);

	/* 4. Shock01 Effect */
	m_pFrameTriger[TYPE_E_SHOCK_00] = 90;
	m_pPositionOffset[TYPE_E_SHOCK_00] = _float3(0.f, -0.5f, 0.f);
	m_pScaleOffset[TYPE_E_SHOCK_00] = _float3(0.6f, 0.75f, 0.6f);
	m_pRotationOffset[TYPE_E_SHOCK_00] = _float3(0.f, 0.f, 0.f);

	/* 5. Shock02 Effect */
	m_pFrameTriger[TYPE_E_SHOCK_01] = 90;
	m_pPositionOffset[TYPE_E_SHOCK_01] = _float3(0.f, -0.5f, 0.f);
	m_pScaleOffset[TYPE_E_SHOCK_01] = _float3(0.6f, 0.75f, 0.6f);
	m_pRotationOffset[TYPE_E_SHOCK_01] = _float3(0.f, 90.f, 0.f);

	/* 6. Smoke Effect */
	m_pFrameTriger[TYPE_E_SMOKE_00] = 90;
	m_pPositionOffset[TYPE_E_SMOKE_00] = _float3(0.f, 0.5f, 0.f);
	m_pScaleOffset[TYPE_E_SMOKE_00] = _float3(4.f, 5.f, 4.f);
	m_pRotationOffset[TYPE_E_SMOKE_00] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SMOKE] = 91;
	m_pPositionOffset[TYPE_P_SMOKE] = _float3(0.f, 0.7f, 0.f);
	m_pScaleOffset[TYPE_P_SMOKE] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_CIRCLES] = 91;
	m_pPositionOffset[TYPE_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
	m_pScaleOffset[TYPE_P_CIRCLES] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Stellia_Skill_JumpStamp::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_JumpStamp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// 35
		if (m_iCount == TYPE_E_RISING_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_RISING_00])
		{
			if (nullptr != m_pWarningDecal)
				m_pWarningDecal->Set_Owner(nullptr);

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Rising_Trail"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_RISING_00], m_pScaleOffset[TYPE_E_RISING_00], m_pRotationOffset[TYPE_E_RISING_00]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_RISING_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_RISING_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Rising_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_RISING_01], m_pScaleOffset[TYPE_E_RISING_01], m_pRotationOffset[TYPE_E_RISING_01]);
			m_iCount++;
		}

		// 50
		else if (m_iCount == TYPE_D_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_WARNING])
		{
			CCharacter* pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
			if (nullptr != pPlayer)
			{
				CTransform* pOwnerTransform = pPlayer->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr != pOwnerTransform)
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_Warning"),
						XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_WARNING], m_pScaleOffset[TYPE_D_WARNING], m_pRotationOffset[TYPE_D_WARNING], pPlayer, &m_pWarningDecal, false);
					Safe_AddRef(m_pWarningDecal);
				}
			}
			m_iCount++;
		}
		// 80
		else if (m_iCount == TYPE_V_DELETE && m_iOwnerFrame >= m_pFrameTriger[TYPE_V_DELETE])
		{
			if (nullptr != m_pWarningDecal)
			{
				m_pWarningDecal->Start_AlphaDeleate();
				Safe_Release(m_pWarningDecal);
			}
			m_iCount++;
		}

		// 90
		else if (m_iCount == TYPE_D_DECAL_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_DECAL_00])
		{
			m_WorldMatrix = m_pOwnerObject->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldFloat4x4();

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Stellia_JumpStamp_Crack01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_DECAL_00], m_pScaleOffset[TYPE_D_DECAL_00], m_pRotationOffset[TYPE_D_DECAL_00]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_SHOCK_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_SHOCK_00])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_JumpStamp_Shock01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SHOCK_00], m_pScaleOffset[TYPE_E_SHOCK_00], m_pRotationOffset[TYPE_E_SHOCK_00], nullptr, &m_pShock01Effect, false);
			Safe_AddRef(m_pShock01Effect);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_SHOCK_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_SHOCK_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_JumpStamp_Shock01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SHOCK_01], m_pScaleOffset[TYPE_E_SHOCK_01], m_pRotationOffset[TYPE_E_SHOCK_01], nullptr, &m_pShock02Effect, false);
			Safe_AddRef(m_pShock02Effect);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_SMOKE_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_SMOKE_00])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_JumpStamp_Smoke01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SMOKE_00], m_pScaleOffset[TYPE_E_SMOKE_00], m_pRotationOffset[TYPE_E_SMOKE_00], nullptr, &m_pSmokeEffect, false);
			Safe_AddRef(m_pSmokeEffect);
			m_iCount++;
		}
	
	    // 91
		else if (m_iCount == TYPE_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Spawn_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE], m_pScaleOffset[TYPE_P_SMOKE], m_pRotationOffset[TYPE_P_SMOKE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Spawn_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLES], m_pScaleOffset[TYPE_P_CIRCLES], m_pRotationOffset[TYPE_P_CIRCLES]);

			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Spawn_Circle_02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLES], m_pScaleOffset[TYPE_P_CIRCLES], m_pRotationOffset[TYPE_P_CIRCLES]);
			m_iCount++;
		}
	}
}

void CVfx_Stellia_Skill_JumpStamp::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_JumpStamp::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_JumpStamp::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_JumpStamp* CVfx_Stellia_Skill_JumpStamp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_JumpStamp* pInstance = new CVfx_Stellia_Skill_JumpStamp(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_JumpStamp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_JumpStamp::Clone(void* pArg)
{
	CVfx_Stellia_Skill_JumpStamp* pInstance = new CVfx_Stellia_Skill_JumpStamp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_JumpStamp");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_JumpStamp::Free()
{
	__super::Free();

	if (nullptr != m_pWarningDecal)
	{
		m_pWarningDecal->Start_AlphaDeleate();
		Safe_Release(m_pWarningDecal);
	}

	if (nullptr != m_pRising01Effect)
	{
		m_pRising01Effect->Set_Dead(true);
		Safe_Release(m_pRising01Effect);
	}

	if (nullptr != m_pSmokeEffect)
	{
		m_pSmokeEffect->Set_Dead(true);
		Safe_Release(m_pSmokeEffect);
	}

	if (nullptr != m_pShock01Effect)
	{
		m_pShock01Effect->Set_Dead(true);
		Safe_Release(m_pShock01Effect);
	}

	if (nullptr != m_pShock02Effect)
	{
		m_pShock02Effect->Set_Dead(true);
		Safe_Release(m_pShock02Effect);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}