#include "stdafx.h"
#include "Vfx_Glanix_Skill_Spawn.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"
#include "Effect.h"

#include "Game_Manager.h"
#include "Player.h"

CVfx_Glanix_Skill_Spawn::CVfx_Glanix_Skill_Spawn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_Spawn::CVfx_Glanix_Skill_Spawn(const CVfx_Glanix_Skill_Spawn& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_Spawn::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_SPAWN;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	{
		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 14;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_SMOKE] = 15;
		m_pPositionOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.7f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE] = 16;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.1f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE]    = _float3(2.5f, 2.5f, 2.5f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

	m_pFrameTriger[TYPE_ET2_E_ROAR] = 44;
	m_pPositionOffset[TYPE_ET2_E_ROAR] = _float3(0.f, 25.f, 0.f);
	m_pScaleOffset[TYPE_ET2_E_ROAR]    = _float3(0.2f, 0.2f, 0.2f);
	m_pRotationOffset[TYPE_ET2_E_ROAR] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_Spawn::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_Spawn::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr != pOwnerTransform)
				m_WorldMatrix = pOwnerTransform->Get_WorldFloat4x4();

			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_JumpDown_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES], m_pScaleOffset[TYPE_ET1_P_CIRCLES], m_pRotationOffset[TYPE_ET1_P_CIRCLES]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FootDown_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_SMOKE], m_pScaleOffset[TYPE_ET1_P_SMOKE], m_pRotationOffset[TYPE_ET1_P_SMOKE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_JumpDown_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_CIRCLELINE], m_pScaleOffset[TYPE_ET1_E_CIRCLELINE], m_pRotationOffset[TYPE_ET1_E_CIRCLELINE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_ROAR && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_ROAR])
		{
			if (false == m_bRadialBlur)
			{
				// 레디얼 블러 활성화
				CGame_Manager::GetInstance()->Set_RadialBlur(true, 16.f, 0.1f);
				m_bRadialBlur = true;
			}

			GET_INSTANCE(CEffect_Manager)->Tick_Generate_Effect(&m_fTimeAcc, 0.225f, fTimeDelta, TEXT("Effect_Glanix_Roar_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_ROAR], m_pScaleOffset[TYPE_ET2_E_ROAR], m_pRotationOffset[TYPE_ET2_E_ROAR]);

			if (m_iOwnerFrame >= 69)
				m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_Spawn::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_Spawn::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_Spawn::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_Spawn* CVfx_Glanix_Skill_Spawn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_Spawn* pInstance = new CVfx_Glanix_Skill_Spawn(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_Spawn::Clone(void* pArg)
{
	CVfx_Glanix_Skill_Spawn* pInstance = new CVfx_Glanix_Skill_Spawn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_Spawn");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_Spawn::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
	else
	{
		CGame_Manager::GetInstance()->Set_RadialBlur(false);
	}
}