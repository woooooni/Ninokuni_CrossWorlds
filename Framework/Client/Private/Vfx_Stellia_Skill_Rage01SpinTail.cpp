#include "stdafx.h"
#include "Vfx_Stellia_Skill_Rage01SpinTail.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Decal.h"

CVfx_Stellia_Skill_Rage01SpinTail::CVfx_Stellia_Skill_Rage01SpinTail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Rage01SpinTail::CVfx_Stellia_Skill_Rage01SpinTail(const CVfx_Stellia_Skill_Rage01SpinTail& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Rage01SpinTail::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_RAGE1LOOP_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_D_WARNING] = 0;
	m_pPositionOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_WARNING]    = _float3(24.f, 5.f, 24.f);
	m_pRotationOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_E_TRAIL_01] = 57;
	m_pPositionOffset[TYPE_E_TRAIL_01] = _float3(0.f, 0.250, 0.f);
	m_pScaleOffset[TYPE_E_TRAIL_01]    = _float3(5.f, 5.f, 5.f);
	m_pRotationOffset[TYPE_E_TRAIL_01] = _float3(0.f, 90.f, 0.f);

	// 2
	m_pFrameTriger[TYPE_E_TRAIL_02] = 60;
	m_pPositionOffset[TYPE_E_TRAIL_02] = _float3(0.f, 0.50, 0.f);
	m_pScaleOffset[TYPE_E_TRAIL_02]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_TRAIL_02] = _float3(0.f, 90.f, 0.f);

	// 3
	m_pFrameTriger[TYPE_P_SMOKE_01] = 62;
	m_pPositionOffset[TYPE_P_SMOKE_01] = _float3(8.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_P_SMOKE_01] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_01] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SMOKE_02] = 63;
	m_pPositionOffset[TYPE_P_SMOKE_02] = _float3(6.f, 1.f, 5.5f);
	m_pScaleOffset[TYPE_P_SMOKE_02] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_02] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SMOKE_03] = 64;
	m_pPositionOffset[TYPE_P_SMOKE_03] = _float3(0.f, 1.f, 8.f);
	m_pScaleOffset[TYPE_P_SMOKE_03] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_03] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SMOKE_04] = 65;
	m_pPositionOffset[TYPE_P_SMOKE_04] = _float3(-6.f, 1.f, 5.5f);
	m_pScaleOffset[TYPE_P_SMOKE_04] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_04] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SMOKE_05] = 66;
	m_pPositionOffset[TYPE_P_SMOKE_05] = _float3(-8.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_P_SMOKE_05] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE_05] = _float3(0.f, 0.f, 0.f);

	//m_pFrameTriger[TYPE_V_E_SPEED_01] = 110;
	//m_pPositionOffset[TYPE_V_E_SPEED_01] = _float3(0.f, 0.f, 0.f); // x에 속도 저장
	//m_pScaleOffset[TYPE_V_E_SPEED_01] = _float3(0.f, 4.f, 0.f);  // x에 TurnSpeed 저장
	//m_pRotationOffset[TYPE_V_E_SPEED_01] = _float3(0.f, 1.f, 0.f); // 회전 시킬 축 저장

	//m_pFrameTriger[TYPE_V_E_SPEED_02] = 110;
	//m_pPositionOffset[TYPE_V_E_SPEED_02] = _float3(0.f, 0.f, 0.f); // x에 속도 저장
	//m_pScaleOffset[TYPE_V_E_SPEED_02] = _float3(0.f, 4.f, 0.f);  // x에 TurnSpeed 저장
	//m_pRotationOffset[TYPE_V_E_SPEED_02] = _float3(0.f, 1.f, 0.f); // 회전 시킬 축 저장

	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01SpinTail::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Rage01SpinTail::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_WARNING])
		{
			CEffect_Manager::GetInstance()->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_Warning"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_WARNING], m_pScaleOffset[TYPE_D_WARNING], m_pRotationOffset[TYPE_D_WARNING], nullptr, &m_pWarning, false);
			Safe_AddRef(m_pWarning);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAIL_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Stellia_Skill_SpinTail"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_01], m_pScaleOffset[TYPE_E_TRAIL_01], m_pRotationOffset[TYPE_E_TRAIL_01]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_TRAIL_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_02])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Stellia_Skill_TailSmoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_02], m_pScaleOffset[TYPE_E_TRAIL_02], m_pRotationOffset[TYPE_E_TRAIL_02]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SMOKE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_01], m_pScaleOffset[TYPE_P_SMOKE_01], m_pRotationOffset[TYPE_P_SMOKE_01]);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_01], m_pScaleOffset[TYPE_P_SMOKE_01], m_pRotationOffset[TYPE_P_SMOKE_01]);

			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_02])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_02], m_pScaleOffset[TYPE_P_SMOKE_02], m_pRotationOffset[TYPE_P_SMOKE_02]);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_02], m_pScaleOffset[TYPE_P_SMOKE_02], m_pRotationOffset[TYPE_P_SMOKE_02]);

			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_03])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_03], m_pScaleOffset[TYPE_P_SMOKE_03], m_pRotationOffset[TYPE_P_SMOKE_03]);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_03], m_pScaleOffset[TYPE_P_SMOKE_03], m_pRotationOffset[TYPE_P_SMOKE_03]);

			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_04])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_04], m_pScaleOffset[TYPE_P_SMOKE_04], m_pRotationOffset[TYPE_P_SMOKE_04]);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_04], m_pScaleOffset[TYPE_P_SMOKE_04], m_pRotationOffset[TYPE_P_SMOKE_04]);

			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE_05])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_05], m_pScaleOffset[TYPE_P_SMOKE_05], m_pRotationOffset[TYPE_P_SMOKE_05]);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Swing_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE_05], m_pScaleOffset[TYPE_P_SMOKE_05], m_pRotationOffset[TYPE_P_SMOKE_05]);

			m_iCount++;
		}

		//if (m_pTail01Effect != nullptr)
		//{
		//	CTransform* pHand01EffectTransform = m_pTail01Effect->Get_Component<CTransform>(TEXT("Com_Transform"));
		//	// pHand01EffectTransform->Move(pHand01EffectTransform->Get_Look(), m_pPositionOffset[TYPE_V_E_SPEED].x, fTimeDelta);
		//	pHand01EffectTransform->Turn(Vec3(m_pRotationOffset[TYPE_V_E_SPEED_01]), -m_pScaleOffset[TYPE_V_E_SPEED_01].y, fTimeDelta);
		//}

		//if (m_pTail02Effect != nullptr)
		//{
		//	CTransform* pHand01EffectTransform = m_pTail02Effect->Get_Component<CTransform>(TEXT("Com_Transform"));
		//	// pHand01EffectTransform->Move(pHand01EffectTransform->Get_Look(), m_pPositionOffset[TYPE_V_E_SPEED].x, fTimeDelta);
		//	pHand01EffectTransform->Turn(Vec3(m_pRotationOffset[TYPE_V_E_SPEED_02]), -m_pScaleOffset[TYPE_V_E_SPEED_02].y, fTimeDelta);
		//}
	}
}

void CVfx_Stellia_Skill_Rage01SpinTail::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Rage01SpinTail::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01SpinTail::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Rage01SpinTail* CVfx_Stellia_Skill_Rage01SpinTail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Rage01SpinTail* pInstance = new CVfx_Stellia_Skill_Rage01SpinTail(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Rage01SpinTail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Rage01SpinTail::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Rage01SpinTail* pInstance = new CVfx_Stellia_Skill_Rage01SpinTail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Rage01SpinTail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Rage01SpinTail::Free()
{
	__super::Free();

	if (nullptr != m_pWarning)
	{
		m_pWarning->Start_AlphaDeleate();
		Safe_Release(m_pWarning);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}