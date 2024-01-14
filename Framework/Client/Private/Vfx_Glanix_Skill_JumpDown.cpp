#include "stdafx.h"
#include "Vfx_Glanix_Skill_JumpDown.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Game_Manager.h"
#include "Glanix.h"
#include "Player.h"
#include "Decal.h"
#include "Effect.h"

CVfx_Glanix_Skill_JumpDown::CVfx_Glanix_Skill_JumpDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_JumpDown::CVfx_Glanix_Skill_JumpDown(const CVfx_Glanix_Skill_JumpDown& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_JumpDown::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_JUMPSTAMP;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_D_START_WARNING]    = 0;
		m_pPositionOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_D_START_WARNING]    = _float3(12.f, 2.f, 12.f);
		m_pRotationOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_D_PREACT_HANDCRACK_LEFT]    = 38;
		m_pPositionOffset[TYPE_D_PREACT_HANDCRACK_LEFT] = _float3(-0.5f, 0.f, 0.3f);
		m_pScaleOffset[TYPE_D_PREACT_HANDCRACK_LEFT]    = _float3(3.f, 3.f, 3.f);
		m_pRotationOffset[TYPE_D_PREACT_HANDCRACK_LEFT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_D_PREACT_HANDCRACK_RIGHT]    = 40;
		m_pPositionOffset[TYPE_D_PREACT_HANDCRACK_RIGHT] = _float3(0.5f, 0.f, 0.35f);
		m_pScaleOffset[TYPE_D_PREACT_HANDCRACK_RIGHT]    = _float3(3.f, 3.f, 3.f);
		m_pRotationOffset[TYPE_D_PREACT_HANDCRACK_RIGHT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_PREACT_SMOKE]    = 46;
		m_pPositionOffset[TYPE_P_PREACT_SMOKE] = _float3(0.f, 0.7f, 0.f);
		m_pScaleOffset[TYPE_P_PREACT_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_PREACT_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_PREACT_CIRCLES]    = 47;
		m_pPositionOffset[TYPE_P_PREACT_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_P_PREACT_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_PREACT_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_D_ATTACK_DOWNCRACK]    = 74;
		m_pPositionOffset[TYPE_D_ATTACK_DOWNCRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_D_ATTACK_DOWNCRACK]    = _float3(25.f, 2.f, 25.f);
		m_pRotationOffset[TYPE_D_ATTACK_DOWNCRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_ATTACK_CIRCLES]    = 75;
		m_pPositionOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_P_ATTACK_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_SPRINGUP]    = 75;
		m_pPositionOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_ATTACK_SPRINGUP]    = _float3(9.f, 14.f, 9.f);
		m_pRotationOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_ATTACK_SMOKE]    = 76;
		m_pPositionOffset[TYPE_P_ATTACK_SMOKE] = _float3(0.f, 0.7f, 0.f);
		m_pScaleOffset[TYPE_P_ATTACK_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_ATTACK_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_CIRCLELINE]    = 77;
		m_pPositionOffset[TYPE_E_ATTACK_CIRCLELINE] = _float3(0.f, 0.1f, 0.f);
		m_pScaleOffset[TYPE_E_ATTACK_CIRCLELINE]    = _float3(2.5f, 2.5f, 2.5f);
		m_pRotationOffset[TYPE_E_ATTACK_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

	m_pFrameTriger[TYPE_EVENT_DISSOLVE] = 73;
	m_pPositionOffset[TYPE_EVENT_DISSOLVE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_EVENT_DISSOLVE]    = _float3(0.f, 0.f, 0.f);
	m_pRotationOffset[TYPE_EVENT_DISSOLVE] = _float3(0.f, 0.f, 0.f);
	
 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_JumpDown::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_JumpDown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_START_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_START_WARNING])
		{
			CCharacter* pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
			if (nullptr != pPlayer)
			{
				CTransform* pOwnerTransform = pPlayer->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr != pOwnerTransform)
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_Warning"),
						pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_D_START_WARNING], m_pScaleOffset[TYPE_D_START_WARNING], m_pRotationOffset[TYPE_D_START_WARNING], pPlayer, &m_pDecal, false);
					Safe_AddRef(m_pDecal);
				}
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_D_PREACT_HANDCRACK_LEFT && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_PREACT_HANDCRACK_LEFT])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_HandCrack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_PREACT_HANDCRACK_LEFT], m_pScaleOffset[TYPE_D_PREACT_HANDCRACK_LEFT], m_pRotationOffset[TYPE_D_PREACT_HANDCRACK_LEFT]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_D_PREACT_HANDCRACK_RIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_PREACT_HANDCRACK_RIGHT])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_HandCrack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_PREACT_HANDCRACK_RIGHT], m_pScaleOffset[TYPE_D_PREACT_HANDCRACK_RIGHT], m_pRotationOffset[TYPE_D_PREACT_HANDCRACK_RIGHT]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_PREACT_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_PREACT_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_JumpDown_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_PREACT_SMOKE], m_pScaleOffset[TYPE_P_PREACT_SMOKE], m_pRotationOffset[TYPE_P_PREACT_SMOKE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_PREACT_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_PREACT_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FootDown_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_PREACT_CIRCLES], m_pScaleOffset[TYPE_P_PREACT_CIRCLES], m_pRotationOffset[TYPE_P_PREACT_CIRCLES]);
			
			m_pDecal->Set_Owner(nullptr);
			m_iCount++;
		}

		else if (m_iCount == TYPE_D_ATTACK_DOWNCRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_ATTACK_DOWNCRACK])
		{
			m_pDecal->Set_Dead(true);
			Safe_Release(m_pDecal);

			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr != m_pOwnerObject)
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_DownCrack"),
					pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_D_ATTACK_DOWNCRACK], m_pScaleOffset[TYPE_D_ATTACK_DOWNCRACK], m_pRotationOffset[TYPE_D_ATTACK_DOWNCRACK]);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_ATTACK_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_ATTACK_CIRCLES])
		{
			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr != m_pOwnerObject)
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_JumpDown_Circle"),
					pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_P_ATTACK_CIRCLES], m_pScaleOffset[TYPE_P_ATTACK_CIRCLES], m_pRotationOffset[TYPE_P_ATTACK_CIRCLES]);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_ATTACK_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_SPRINGUP])
		{
			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr != m_pOwnerObject)
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_JumpDown_SpringUp"),
					pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_E_ATTACK_SPRINGUP], m_pScaleOffset[TYPE_E_ATTACK_SPRINGUP], m_pRotationOffset[TYPE_E_ATTACK_SPRINGUP], nullptr, &m_pEffect, false);
				Safe_AddRef(m_pEffect);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_ATTACK_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_ATTACK_SMOKE])
		{
			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr != m_pOwnerObject)
			{
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FootDown_Smoke"),
					pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_P_ATTACK_SMOKE], m_pScaleOffset[TYPE_P_ATTACK_SMOKE], m_pRotationOffset[TYPE_P_ATTACK_SMOKE]);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_ATTACK_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_CIRCLELINE])
		{
			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr != m_pOwnerObject)
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_JumpDown_TrailLine"),
					pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_E_ATTACK_CIRCLELINE], m_pScaleOffset[TYPE_E_ATTACK_CIRCLELINE], m_pRotationOffset[TYPE_E_ATTACK_CIRCLELINE]);
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_JumpDown::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_JumpDown::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_JumpDown::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_JumpDown* CVfx_Glanix_Skill_JumpDown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_JumpDown* pInstance = new CVfx_Glanix_Skill_JumpDown(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_JumpDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_JumpDown::Clone(void* pArg)
{
	CVfx_Glanix_Skill_JumpDown* pInstance = new CVfx_Glanix_Skill_JumpDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_JumpDown");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_JumpDown::Free()
{
	__super::Free();

	if (nullptr != m_pDecal)
	{
		m_pDecal->Set_Dead(true);

		Safe_Release(m_pDecal);
	}

	if (nullptr != m_pEffect)
	{
		m_pEffect->Start_Dissolve((_uint)m_pFrameTriger[TYPE_EVENT_DISSOLVE],    // Index
			_float4(1.f, 1.f, 1.f, 1.f), // Color
			4.f,   // Speed
			10.f); // Total

		Safe_Release(m_pEffect);
    }

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}