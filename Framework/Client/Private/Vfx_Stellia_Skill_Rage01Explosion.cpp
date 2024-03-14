#include "stdafx.h"
#include "Vfx_Stellia_Skill_Rage01Explosion.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Particle.h"

CVfx_Stellia_Skill_Rage01Explosion::CVfx_Stellia_Skill_Rage01Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Rage01Explosion::CVfx_Stellia_Skill_Rage01Explosion(const CVfx_Stellia_Skill_Rage01Explosion& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Rage01Explosion::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_RAGE1LOOP_EXPLOSION;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	m_pFrameTriger[TYPE_E_PULL] = 4;
	m_pPositionOffset[TYPE_E_PULL] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_PULL] = _float3(20.f, 1.f, 20.f);
	m_pRotationOffset[TYPE_E_PULL] = _float3(0.f, 0.f, 0.f);

	/* 0. Warning Decal */
	m_pFrameTriger[TYPE_D_DECAL_00] = 0;
	m_pPositionOffset[TYPE_D_DECAL_00] = _float3(0.f, 0.25f, 0.f);
	m_pScaleOffset[TYPE_D_DECAL_00] = _float3(100.f, 1.f, 100.f);
	m_pRotationOffset[TYPE_D_DECAL_00] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_CIRCLES] = 0;
	m_pPositionOffset[TYPE_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

	/* 1. SafeZone Decal */
	m_pFrameTriger[TYPE_D_DECAL_01] = 1;
	m_pPositionOffset[TYPE_D_DECAL_01] = _float3(0.26f, 0.3f, 0.f);
	m_pScaleOffset[TYPE_D_DECAL_01] = _float3(20.f, 2.f, 19.5f);
	m_pRotationOffset[TYPE_D_DECAL_01] = _float3(0.f, -90.f, 0.f);

	m_pFrameTriger[TYPE_E_SHOCK_01] = 64;
	m_pPositionOffset[TYPE_E_SHOCK_01] = _float3(0.f, -0.5f, 0.f);
	m_pScaleOffset[TYPE_E_SHOCK_01]    = _float3(0.7f, 0.85f, 0.7f);
	m_pRotationOffset[TYPE_E_SHOCK_01] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_E_SPRINGUP] = 65;
	m_pPositionOffset[TYPE_E_SPRINGUP] = _float3(1.f, 0.f, 0.2f);
	m_pScaleOffset[TYPE_E_SPRINGUP]    = _float3(3.5f, 3.f, 5.f);
	m_pRotationOffset[TYPE_E_SPRINGUP] = _float3(0.f, 90.f, 0.f);

	m_pFrameTriger[TYPE_P_SMOKE] = 65;
	m_pPositionOffset[TYPE_P_SMOKE] = _float3(0.f, 3.f, 0.f);
	m_pScaleOffset[TYPE_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_E_WIND_00] = 65;
	m_pPositionOffset[TYPE_E_WIND_00] = _float3(0.f, 0.35f, 0.f);
	m_pScaleOffset[TYPE_E_WIND_00]    = _float3(5.f, 5.f, 5.f);
	m_pRotationOffset[TYPE_E_WIND_00] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_E_WIND_01] = 65;
	m_pPositionOffset[TYPE_E_WIND_01] = _float3(0.f, 0.4f, 0.f);
	m_pScaleOffset[TYPE_E_WIND_01]    = _float3(5.1f, 5.1f, 5.1f);
	m_pRotationOffset[TYPE_E_WIND_01] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_EXPOLSION_CIRCLES] = 66;
	m_pPositionOffset[TYPE_P_EXPOLSION_CIRCLES] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_EXPOLSION_CIRCLES]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_EXPOLSION_CIRCLES] = _float3(0.f, 0.f, 0.f);


	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01Explosion::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Rage01Explosion::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_E_PULL && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_PULL])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_BigBang_Smoke_In"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_PULL], m_pScaleOffset[TYPE_E_PULL], m_pRotationOffset[TYPE_E_PULL], nullptr, &m_pSmokeIn_01, false);
			Safe_AddRef(m_pSmokeIn_01);
			m_iCount++;
		}
		else if (m_iCount == TYPE_D_DECAL_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_DECAL_00])
		{
			m_WorldMatrix = m_pOwnerObject->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldFloat4x4();

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Stellia_Rage01Crack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_DECAL_00], m_pScaleOffset[TYPE_D_DECAL_00], m_pRotationOffset[TYPE_D_DECAL_00]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Rage01_Explosion_Decal_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLES], m_pScaleOffset[TYPE_P_CIRCLES], m_pRotationOffset[TYPE_P_CIRCLES], nullptr, &m_pCircles, false);
			Safe_AddRef(m_pCircles);
			m_iCount++;
		}
		else if (m_iCount == TYPE_D_DECAL_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_DECAL_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Stellia_Rage01SafeZone"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_DECAL_01], m_pScaleOffset[TYPE_D_DECAL_01], m_pRotationOffset[TYPE_D_DECAL_01]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_SHOCK_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_SHOCK_01])
		{
			if (nullptr != m_pCircles)
			{
				m_pCircles->Set_LoopParticle(false);
				Safe_Release(m_pCircles);
			}

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_JumpStamp_Shock01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SHOCK_01], m_pScaleOffset[TYPE_E_SHOCK_01], m_pRotationOffset[TYPE_E_SHOCK_01]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_SPRINGUP])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Rage01_Explosion_SpringUp"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPRINGUP], m_pScaleOffset[TYPE_E_SPRINGUP], m_pRotationOffset[TYPE_E_SPRINGUP], nullptr, &m_pSpringUp, false);
			Safe_AddRef(m_pSpringUp);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Rage01Explosion_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE], m_pScaleOffset[TYPE_P_SMOKE], m_pRotationOffset[TYPE_P_SMOKE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_WIND_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_WIND_00])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage01Explosion_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_WIND_00], m_pScaleOffset[TYPE_E_WIND_00], m_pRotationOffset[TYPE_E_WIND_00]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_WIND_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_WIND_01])
		{
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Skill_Rage01Explosion_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_WIND_01], m_pScaleOffset[TYPE_E_WIND_01], m_pRotationOffset[TYPE_E_WIND_01], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(_float3(1.f, 1.f, 1.f));
				pEffect->Set_ScaleSpeed(15.f);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_EXPOLSION_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_EXPOLSION_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Rage01_Explosion_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_EXPOLSION_CIRCLES], m_pScaleOffset[TYPE_P_EXPOLSION_CIRCLES], m_pRotationOffset[TYPE_P_EXPOLSION_CIRCLES]);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Rage01_Explosion_Circles_02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_EXPOLSION_CIRCLES], m_pScaleOffset[TYPE_P_EXPOLSION_CIRCLES], m_pRotationOffset[TYPE_P_EXPOLSION_CIRCLES]);
			m_iCount++;
		}
	}
}

void CVfx_Stellia_Skill_Rage01Explosion::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Rage01Explosion::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01Explosion::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Rage01Explosion* CVfx_Stellia_Skill_Rage01Explosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Rage01Explosion* pInstance = new CVfx_Stellia_Skill_Rage01Explosion(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Rage01Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Rage01Explosion::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Rage01Explosion* pInstance = new CVfx_Stellia_Skill_Rage01Explosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Rage01Explosion");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Rage01Explosion::Free()
{
	__super::Free();

	if (nullptr != m_pSmokeIn_01)
	{
		m_pSmokeIn_01->Set_UVLoop(1);
		Safe_Release(m_pSmokeIn_01);
	}
	if (nullptr != m_pCircles)
	{
		m_pCircles->Set_LoopParticle(false);
		Safe_Release(m_pCircles);
	}
	if (nullptr != m_pSpringUp)
	{
		m_pSpringUp->Start_Dissolve(73,       // Index
			_float4(0.760f, 0.611f, 1.f, 1.f),// Color
			4.f,   // Speed
			10.f); // Total
		Safe_Release(m_pSpringUp);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}