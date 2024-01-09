#include "stdafx.h"
#include "Vfx_SwordMan_Skill_AcaneBarrier.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Effect.h"

CVfx_SwordMan_Skill_AcaneBarrier::CVfx_SwordMan_Skill_AcaneBarrier(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_AcaneBarrier::CVfx_SwordMan_Skill_AcaneBarrier(const CVfx_SwordMan_Skill_AcaneBarrier& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_0;
	m_iMaxCount = 5;

	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 1. 방어막 생성 시작1 / Effect_Swordman_Skill_AcaneBarrier_Barrier_Start
	m_pFrameTriger[0]    = 6;
	m_pPositionOffset[0] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[0]    = _float3(4.f, 4.f, 4.f);
	m_pRotationOffset[0] = _float3(0.f, 0.f, 0.f);

	// 2. 방어막 완전 생성 / Effect_Swordman_Skill_AcaneBarrier_Barrier_Move
	m_pFrameTriger[1]    = 34;
	m_pPositionOffset[1] = _float3(0.f, 0.1f, 0.f);
	m_pScaleOffset[1]    = _float3(5.5f, 5.5f, 5.5f);
	m_pRotationOffset[1] = _float3(0.f, 0.f, 0.f);

	// 3. 방어막 완전 생성 직전 파티클 / Particle_Swordman_Skill_AcaneBarrier_Sparkle_Circle
	m_pFrameTriger[2]    = 36;
	m_pPositionOffset[2] = _float3(0.f, 0.2f, 0.f);
	m_pScaleOffset[2]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[2] = _float3(0.f, 0.f, 0.f);

	// 4. 방어막 해제 시작 (이벤트)
	m_pFrameTriger[3]    = 50;
	m_pPositionOffset[3] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[3]    = _float3(0.f, 0.f, 0.f);
	m_pRotationOffset[3] = _float3(0.f, 0.f, 0.f);

	// 5. 라인 효과 / Effect_Swordman_Skill_AcaneBarrier_Barrier_LineEffect
	m_pFrameTriger[4]    = 62;
	m_pPositionOffset[4] = _float3(0.f, 0.1f, 0.f);
	m_pScaleOffset[4]    = _float3(6.f, 6.f, 6.f);
	m_pRotationOffset[4] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_AcaneBarrier::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// 1. 방어막 생성 시작1 / Effect_Swordman_Skill_AcaneBarrier_Barrier_Start
		if (m_iCount == 0 && m_iOwnerFrame >= m_pFrameTriger[0])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_AcaneBarrier_Barrier_Start"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[0], m_pScaleOffset[0], m_pRotationOffset[0]);
			m_iCount++;
		}

		// 2. 방어막 완전 생성 / Effect_Swordman_Skill_AcaneBarrier_Barrier_Move
		else if (m_iCount == 1 && m_iOwnerFrame >= m_pFrameTriger[1])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_AcaneBarrier_Barrier_Move"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[1], m_pScaleOffset[1], m_pRotationOffset[1], nullptr, &m_pEffect);
			Safe_AddRef(m_pEffect);
			m_iCount++;
		}

		// 3. 방어막 완전 생성 직전 파티클 / Particle_Swordman_Skill_AcaneBarrier_Sparkle_Circle
		else if (m_iCount == 2 && m_iOwnerFrame >= m_pFrameTriger[2])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_AcaneBarrier_Sparkle_Circle"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[2], m_pScaleOffset[2], m_pRotationOffset[2]);
			m_iCount++;
		}

		// 5. 방어막 해제 시작 (이벤트)
		else if (m_iCount == 3 && m_iOwnerFrame >= m_pFrameTriger[3])
		{
			m_pEffect->Set_UVFlow(1, _float2(0, 1), _float2(0.5f, 0.5f));
			Safe_Release(m_pEffect);
			m_iCount++;
		}

		// 4. 라인 효과 / Effect_Swordman_Skill_AcaneBarrier_Barrier_LineEffect
		else if (m_iCount == 4 && m_iOwnerFrame >= m_pFrameTriger[4])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_AcaneBarrier_Barrier_LineEffect"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[4], m_pScaleOffset[4], m_pRotationOffset[4]);
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 5)
			m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_AcaneBarrier::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_AcaneBarrier::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_AcaneBarrier* CVfx_SwordMan_Skill_AcaneBarrier::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_AcaneBarrier* pInstance = new CVfx_SwordMan_Skill_AcaneBarrier(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_AcaneBarrier");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_AcaneBarrier::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_AcaneBarrier* pInstance = new CVfx_SwordMan_Skill_AcaneBarrier(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_AcaneBarrier");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_AcaneBarrier::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}

	if(m_pEffect != nullptr)
		Safe_Release(m_pEffect);
}