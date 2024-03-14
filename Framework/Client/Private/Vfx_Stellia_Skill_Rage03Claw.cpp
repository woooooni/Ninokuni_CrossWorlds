#include "stdafx.h"
#include "Vfx_Stellia_Skill_Rage03Claw.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"

CVfx_Stellia_Skill_Rage03Claw::CVfx_Stellia_Skill_Rage03Claw(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Rage03Claw::CVfx_Stellia_Skill_Rage03Claw(const CVfx_Stellia_Skill_Rage03Claw& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Rage03Claw::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_RAGE3CLAW;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_E_TRAIL_01] = 22;
	m_pPositionOffset[TYPE_E_TRAIL_01] = _float3(0.f, 0.5f, 1.5f);
	m_pScaleOffset[TYPE_E_TRAIL_01]    = _float3(2.5f, 2.5f, 2.5f);
	m_pRotationOffset[TYPE_E_TRAIL_01] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_P_CIRCLES_01] = 23;
	m_pPositionOffset[TYPE_P_CIRCLES_01] = _float3(0.f, 0.5f, 5.f);
	m_pScaleOffset[TYPE_P_CIRCLES_01]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLES_01] = _float3(0.f, 0.f, 0.f);


	m_pFrameTriger[TYPE_P_SMOKE] = 23;
	m_pPositionOffset[TYPE_P_SMOKE] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_STONE] = 23;
	m_pPositionOffset[TYPE_P_STONE] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_P_STONE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_STONE] = _float3(0.f, 0.f, 0.f);






	m_pFrameTriger[TYPE_V_E_SPEED] = 22;
	m_pPositionOffset[TYPE_V_E_SPEED] = _float3(48.f, 0.f, 0.f); // x에 속도 저장
	m_pScaleOffset[TYPE_V_E_SPEED]    = _float3(0.f, 0.f, 0.f);  // TurnSpeed 저장
	m_pRotationOffset[TYPE_V_E_SPEED] = _float3(0.f, 1.f, 0.f);  // 회전 시킬 축 저장


	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage03Claw::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Rage03Claw::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_E_TRAIL_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Stellia_Skill_RightClaw"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_01], m_pScaleOffset[TYPE_E_TRAIL_01], m_pRotationOffset[TYPE_E_TRAIL_01], nullptr, &m_pHand01Effect, false);
			Safe_AddRef(m_pHand01Effect);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLES_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLES_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Claw_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLES_01], m_pScaleOffset[TYPE_P_CIRCLES_01], m_pRotationOffset[TYPE_P_CIRCLES_01]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Rage03Claw_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE], m_pScaleOffset[TYPE_P_SMOKE], m_pRotationOffset[TYPE_P_SMOKE]);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE], m_pScaleOffset[TYPE_P_SMOKE], m_pRotationOffset[TYPE_P_SMOKE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_STONE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_STONE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Rage03Claw_Stone"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_STONE], m_pScaleOffset[TYPE_P_STONE], m_pRotationOffset[TYPE_P_STONE]);
			m_iCount++;
		}

		if (m_pHand01Effect != nullptr)
		{
			CTransform* pHand01EffectTransform = m_pHand01Effect->Get_Component<CTransform>(TEXT("Com_Transform"));
			if (nullptr != pHand01EffectTransform)
			{
				pHand01EffectTransform->Move(pHand01EffectTransform->Get_Look(), m_pPositionOffset[TYPE_V_E_SPEED].x, fTimeDelta);
				pHand01EffectTransform->Turn(Vec3(m_pRotationOffset[TYPE_V_E_SPEED]), -m_pScaleOffset[TYPE_V_E_SPEED].y, fTimeDelta);
			}
		}
	}
}

void CVfx_Stellia_Skill_Rage03Claw::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Rage03Claw::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage03Claw::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Rage03Claw* CVfx_Stellia_Skill_Rage03Claw::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Rage03Claw* pInstance = new CVfx_Stellia_Skill_Rage03Claw(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Rage03Claw");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Rage03Claw::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Rage03Claw* pInstance = new CVfx_Stellia_Skill_Rage03Claw(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Rage03Claw");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Rage03Claw::Free()
{
	__super::Free();

	if (nullptr != m_pHand01Effect)
	{
		m_pHand01Effect->Set_Dead(true);
		Safe_Release(m_pHand01Effect);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}